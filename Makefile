dchess: main.o game.o log.o
	gcc $(CFLAGS) -o dchess main.o game.o log.o

main.o: main.c game.h log.h
	gcc $(CFLAGS) -c -std=c11 main.c

game.o: game.c game.h log.h
	gcc $(CFLAGS) -c -std=c11 game.c

log.o: log.c log.h
	gcc $(CFLAGS) -c -std=c11 log.c

clean:
	rm -f *.o dchess
