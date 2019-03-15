#include <stdio.h>

#include "tm_stack.h"

typedef struct tm_stack_priv_s {
	long flag;
} tm_stack_priv_t;

typedef struct tm_stack_element_s {
	void *data;
	void *next;
} tm_stack_element_t;

int tm_stack_init(tm_stack_t *stack)
{
	if (NULL == stack)
		return -1;

	stack->count = 0;
	stack->top = NULL;
	stack->bottom = NULL;
	stack->priv = NULL;

	return 0;
}

int tm_stack_destroy(tm_stack_t *stack)
{
	if (NULL == stack)
		return -1;
}


int tm_stack_push(tm_stack_t *stack, void *data)
{
	return 0;
}

void* tm_stack_pop(tm_stack_t *stack)
{
	if (NULL == stack)
		return -1;

	if ((0 == stack->count) ||
	    (NULL == stack->botton) ||
	    (NULL == stack->top))
		return -1;
	return 0;
}

