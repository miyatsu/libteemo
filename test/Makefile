CC = gcc

a.out: tm_test.c
	$(CC) tm_test.c ../src/tm_stack.c ../src/tm_queue.c \
		-O3 -I../include/ --std=c17 -lpthread
.PHONY: clean

clean:
	-rm a.out
