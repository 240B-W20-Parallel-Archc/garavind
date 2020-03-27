CC=gcc
OPTS=-g -std=c99 -pthread

all:main.o cacheperf.o
	$(CC) $(OPTS) -lm -o cacheperf main.o cacheperf.o

main.o: main.c cacheperf.h
	$(CC) $(OPTS) -c main.c

cacheperf.o: cacheperf.h cacheperf.c
	$(CC) $(OPTS) -c cacheperf.c

clean:
	rm -f *.o cacheperf;
