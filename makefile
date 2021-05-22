.PHONY: all
all: main

main: main.o
	gcc -o main main.o
	rm -rf main.o

main.o: main.c
	gcc -c main.c

main.o: conio.h

.PHONY: clean

clean:
	rm -rf main.o
