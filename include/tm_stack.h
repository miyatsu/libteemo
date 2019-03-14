#ifndef TM_STACK_H
#define TM_STACK_H

#include <stdint.h>

typedef struct tm_stack_s {
	int stack_type;
	union {
		void *array;
		void *header;
	};
} tm_stack_t;

/**
 * stack_init - Initialize a stack with minimal size of minimal_size
 *
 * @stack: Point to the stack
 * @minimal_size: minimal size of an new stack
 * @return:  0 - success
 *	    -1 - error
 * */
int tm_stack_init(tm_stack_t *stack, int minimal_size);

/**
 * stack_resize - Resize the stack with new size
 *
 * @stack: Point to the stack
 * @new_size: New size of this stack
 * @return:  0 - success
 *	    -1 - error
 * */
int tm_stack_resize(tm_stack_t stack, int new_size);

int tm_stack_push(void*);
void* tm_stack_pop(void);

#endif /* TM_STACK_H */

