#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

double delay_()
{
    double miliseg = 0, segs = 0;
    clock_t tiempo_inicio, tiempo_final;

    segs = (rand() % 501) + 500;

    miliseg = 1000 * segs;

    tiempo_inicio = clock();
    while (clock() < tiempo_inicio + miliseg);
    tiempo_final = clock();
    tiempo_final -= tiempo_inicio;

    return (double) tiempo_final/CLOCKS_PER_SEC;
}

double delay(){
    int numb = (rand() % 6) + 10;
    double dy = (double)numb / 10.0f;
    double count = 0.0f;
    while (count <= dy){
        count += 0.001;
    }
    return dy;
}

int main(){
    srand(time(NULL));


    printf("primero\n");
    double dy = delay_();
    printf("Segundo %f\n", dy);
    return 0;
}

