dchess: main.o ai.o game.o log.o test.o uci.o
	gcc $(CFLAGS) -o dchess ai.o main.o game.o log.o test.o uci.o

ai.o: ai.c ai.h game.h
	gcc $(CFLAGS) -c -std=c11 ai.c

game.o: game.c game.h log.h
	gcc $(CFLAGS) -c -std=c11 game.c

log.o: log.c log.h
	gcc $(CFLAGS) -c -std=c11 log.c

main.o: main.c game.h log.h test.h
	gcc $(CFLAGS) -c -std=c11 main.c

test.o: test.c game.h log.h test.h 
	gcc $(CFLAGS) -c -std=c11 test.c

uci.o: uci.c ai.h game.h log.h
	gcc $(CFLAGS) -c -std=c11 uci.c

clean:
	rm -f *.o dchess
