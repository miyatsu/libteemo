#include <stdio.h>
#include <stdbool.h>

#include "tm_stack.h"

typedef struct tm_stack_priv_s {
	long flag;
} tm_stack_priv_t;

typedef struct tm_stack_element_s {
	void *data;
	void *next;
} tm_stack_data_t;

static bool tm_stack_internal_is_stack_empty(tm_stack_t *stack)
{
	return true;
}

static bool tm_stack_internal_stack_integrity_check_ok(tm_stack_t *stack)
{
	void *data_p;
	int data_count;

	/* Stack empty */
	if (0 == stack->count) {
		/* Both pointer MUST be NULL */
		if ((NULL == stack->bottom) && (NULL == stack->top)) {
			return true;
		} else {
			return false;
		}
	} else {
		/* Both pointer MUST NOT be NULL */
		if ((NULL == stack->botton) || (NULL == stack->top)) {
			return false;
		}

		/* One data */
		if (1 == stack->count) {
			/* Point to the same address */
			if (stack->top == stack->bottom) {
				return true;
			} else {
				return false;
			}
		}

		data_p = stack->top;
		data_count = 0;
		do {
			data_count++;

			/* Check count first to avoid invalid memory access */

			/* Count number reached */
			if (data_count == stack->count) {
				/* MUST point to the last one */
				if (data_p != stack->bottom) {
					return false;
				} else {
					return true;
				}
			}

			/* Reach the bottom */
			if (data_p == stack->bottom) {
				if (data_count != stack->count) {
					return false;
				} else {
					return true;
				}
			}

			data_p = data_p->next;
		} while (NULL != data_p);
	}
}

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
	if (NULL == stack)
		return -1;

	tm_stack_element_t *p;

	p = malloc(sizeof(tm_stack_element_t));
	if (NULL == p)
		return -1;

	if (0 == stack->count)
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

