CC = gcc
CFLAGS = -Wall -g -Werror

OBJS = myshell.o hash_table.o utils.o parse.o executor.o
DEPS = myshell.h hash_table.h utils.h parse.h executor.o

myshell: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

myshell.o: myshell.c $(DEPS)
	$(CC) $(CFLAGS) -c myshell.c

hash_table.o: hash_table.c hash_table.h
	$(CC) $(CFLAGS) -c hash_table.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

parse.o: parse.c parse.h
	$(CC) $(CFLAGS) -c parse.c

executor.o: executor.c executor.h
	$(CC) $(CFLAGS) -c executor.c	

.PHONY: clean

clean:
	rm -f *.o myshell

