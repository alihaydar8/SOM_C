cc = gcc -Wall
SRC = som.c vec.c map.c
OBJ = som.o vec.o map.o

som : 	$(OBJ) som.h
	 	$(CC) -o Som $(OBJ) -lm

clean:
		rm  *.o Som