CC=gcc
OPTS=-g -std=c99 -pthread

all:main.o cacheperformance.o
	$(CC) $(OPTS) -lm -o cacheperf main.o cacheperformance.o

main.o: main.c cacheperformance.h
	$(CC) $(OPTS) -c main.c

cacheperf.o: cacheperformance.h cacheperformance.c
	$(CC) $(OPTS) -c cacheperformance.c

clean:
	rm -f *.o cacheperformance;
