CC = gcc

a.out: tm_test.c
	$(CC) tm_test.c ../src/tm_stack.c ../src/tm_queue.c \
		-ggdb3 -march=native -I../include/ --std=c17 -lpthread
.PHONY: clean

clean:
	-rm a.out
