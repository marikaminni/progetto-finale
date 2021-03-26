all:
	gcc -c gamelib.c -Wall -std=c11
	gcc -c main.c -Wall -std=c11
	gcc -o gioco main.o gamelib.o  -Wall -std=c11
