CC := gcc -O3

build: sort.o test.o
	$(CC) -o test test.o sort.o

test.o: test.c
	$(CC) -c test.c

sort.o: sort.c sort.h
	$(CC) -c sort.c
