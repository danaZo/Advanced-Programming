CC = gcc
CFLAGS = -Wall -g -Werror

OBJS = myshell.o hash_table.o utils.o parse.o
DEPS = hash_table.h utils.h parse.h

myshell: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

myshell.o: myshell.c
	$(CC) $(CFLAGS) -c myshell.c

hash_table.o: hash_table.c
	$(CC) $(CFLAGS) -c hash_table.c

utils.o: utils.c
	$(CC) $(CFLAGS) -c utils.c

parse.o: parse.c
	$(CC) $(CFLAGS) -c parse.c

.PHONY: clean

clean:
	rm -f *.o myshell

