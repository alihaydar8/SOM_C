cc = gcc -Wall
SRC = som.c vec.c map.c
OBJ = som.o vec.o map.o

som : 	$(OBJ) som.h
	 	$(CC) -o Som $(OBJ) -lm
%.o: 	%.c som.h
		$(CC) -c $< -Wall

clean:
		rm -f *.o 