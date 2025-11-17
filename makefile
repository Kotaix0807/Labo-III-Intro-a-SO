build:
	clear && gcc recive.c -o recive -Wall -Werror && gcc send.c -o send -Wall -Werror
run:
	./recive&
	./send