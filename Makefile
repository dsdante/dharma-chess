dchess: main.o game.o
	gcc $(CFLAGS) -o dchess main.o game.o

main.o: main.c game.h
	gcc $(CFLAGS) -c -std=c11 main.c

game.o: game.c game.h
	gcc $(CFLAGS) -c -std=c11 game.c

clean:
	rm -f *.o dchess
