dchess: main.o game.o log.o test.o
	gcc $(CFLAGS) -o dchess main.o game.o log.o test.o

game.o: game.c game.h log.h
	gcc $(CFLAGS) -c -std=c11 game.c

log.o: log.c log.h
	gcc $(CFLAGS) -c -std=c11 log.c

main.o: main.c game.h log.h test.h
	gcc $(CFLAGS) -c -std=c11 main.c

test.o: test.c game.h log.h test.h 
	gcc $(CFLAGS) -c -std=c11 test.c

clean:
	rm -f *.o dchess
