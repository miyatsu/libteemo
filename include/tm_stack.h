/*
 * Copyright (C) 2019 Ding Tao <i@dingtao.org>
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef TM_STACK_H
#define TM_STACK_H

/**
 * tm_stack_t - Teemo stack data structure
 *
 * @count: How many data in the stack
 * @top: Point to the top element of the stack
 * @botton: Point to the bottom element of the stack
 * @priv: Teemo stack private data
 * */
typedef struct tm_stack_s {
	unsigned long	count;
	void		*top;
	void		*bottom;
	void		*priv;
} tm_stack_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * tm_stack_init - Initialize a stack
 *
 * @stack: Point to the stack
 * @return:  0 - success
 *	    -1 - error
 * */
int tm_stack_init(tm_stack_t *stack);

/**
 * tm_stack_destroy - Destroy a stack
 *
 * @stack: Point to the stack
 * @return:  0 - success
 *	    -1 - error
 * */
int tm_stack_destroy(tm_stack_t *stack);

/**
 * tm_stack_push - Push one element into stack
 *
 * @stack: Point to the stack
 * @data: Pointer of the data
 * @return:  0 - success
 *	    -1 - error
 * */
int tm_stack_push(tm_stack_t *stack, void *data);

/**
 * tm_stack_pop - Pop top element out of stack
 *
 * @stack: Point to the stack
 * @return: address - success
 *	       NULL - error
 * */
void* tm_stack_pop(tm_stack_t *stack);

/**
 * tm_stack_get_count - Get the total number of the data in stack
 *
 * @stack: Point to the stack
 * @return:  > 0 success
 *	      -1 error
 * */
static inline long tm_stack_get_count(tm_stack_t *stack)
{
	if (NULL == stack)
		return -1;
	return stack->count;
}

#ifdef __cplusplus
}
#endif

#endif /* TM_STACK_H */

