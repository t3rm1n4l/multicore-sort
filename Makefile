CC := gcc -g -O3

build: sort.o select.o test.o
	$(CC) -o test test.o sort.o select.o

test.o: test.c
	$(CC) -c test.c

sort.o: sort.c sort.h
	$(CC) -c sort.c

select.o: select.c sort.h
	$(CC) -c select.c

clean:
	rm -f *.o
