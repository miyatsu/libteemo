CC :=
CFLAGS :=

CC += gcc
CFLAGS += -I ../include
CFLAGS += -Wall
CFLAGS += --std=c11

all: tm_stack.o

tm_stack.o: tm_stack.c
	$(CC) $(CFLAGS) -o tm_stack.o tm_stack.c

thread_pool.o: thread_pool.c
	$(CC) $(CFLAGS) -o thread_pool.o thread_pool.c

