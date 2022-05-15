CC = gcc
CFLAGS = -g -Wall

build: editor.o list.o stack.o command.o utilityfile.o operations.o
	gcc -g editor.o list.o stack.o command.o utilityfile.o operations.o -o editor

editor.o: editor.c

list.o: list.h list.c

stack.o: stack.h stack.c

command.o: command.h command.c

utilityfile.o: utilityfile.h utilityfile.c

operations.o: operations.h operations.c

run:
	./editor

.PHONY: clean

memcheck:
	valgrind --leak-check=full ./editor

clean:
	rm -f *.o editor
