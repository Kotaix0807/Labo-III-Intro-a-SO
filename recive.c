#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define clave 234 
#define MAX 64
#define MAX_MSGS 100


int main()
{
    int msqid; /* identificador de la cola de mensajes */
    int shmid;
    int *shared_data;
    int created_shm = 0;
    key_t key = 1234;

    struct {
        long tipo;
        char cadena[MAX];
    } mensaje;

    int longitud = sizeof(mensaje) - sizeof(mensaje.tipo);

    /* Creación de la cola de mensajes */
    if((msqid = msgget(clave, IPC_CREAT | 0600)) == -1)
    {
        perror("Error al crear la cola de mensajes");
        exit(-1);
    }

    if ((shmid = shmget(key, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) < 0)
    {
        if (errno != EEXIST)
        {
            perror("Error al crear la memoria compartida");
            exit(-1);
        }
        shmid = shmget(key, sizeof(int), 0666);
        if (shmid < 0)
        {
            perror("Error al obtener memoria compartida existente");
            exit(-1);
        }
    }
    else
    {
        created_shm = 1;
    }

    shared_data = shmat(shmid, NULL, 0);
    if (shared_data == (void *)-1)
    {
        perror("Error al adjuntar memoria compartida");
        exit(-1);
    }

    if (created_shm)
    {
        *shared_data = 0;
    }

    int recibidos = 0;
    while (recibidos < MAX_MSGS)
    {
        if(msgrcv(msqid, &mensaje, longitud, 0, 0) == -1)
        {
            if (errno == EINTR)
            {
                continue;
            }

            if (errno == EIDRM || errno == EINVAL)
            {
                fprintf(stderr, "La cola de mensajes ya no es válida. Deteniendo recepción.\n");
                break;
            }

            perror("Error al leer un mensaje de la cola de mensajes");
            continue;
        }

        recibidos++;
        *shared_data = recibidos;
        printf("Mensaje #%d (tipo %ld) en RECEIVE: %s\n", recibidos, mensaje.tipo, mensaje.cadena);
    }

    printf("Se recibieron %d mensajes. Eliminando recursos IPC...\n", recibidos);

    /* Borrado de la cola de mensajes */
    if(msgctl(msqid, IPC_RMID, 0) == -1)
    {
        perror("Error al eliminar la cola de mensajes");
    }   

    if (shmdt(shared_data) == -1)
    {
        perror("Error al desacoplar memoria compartida");
    }

    if (shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        perror("Error al eliminar memoria compartida");
    }

    return 0;
}
