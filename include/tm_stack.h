/*
 * Copyright (C) 2019 Ding Tao <i@dingtao.org>
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef TM_STACK_H
#define TM_STACK_H

#include <stdatomic.h>

/**
 * tm_stack_t - Teemo stack data structure
 *
 * @priv: Teemo stack private data
 * */
typedef struct tm_stack_s {
	void *priv;
} tm_stack_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * tm_stack_init - Initialize a stack
 *
 * @stack: Point to the stack
 *
 * @return:  0 - success
 *	    -1 - error
 * */
int tm_stack_init(tm_stack_t *stack);

/**
 * tm_stack_destroy - Destroy a stack
 *
 * @stack: Point to the stack
 *
 * @return:  0 - success
 *	    -1 - error
 * */
int tm_stack_destroy(tm_stack_t *stack);

/**
 * tm_stack_push - Push one element into stack
 *
 * @stack: Point to the stack
 *
 * @data: Pointer of the data
 *
 * @return:  0 - success
 *	    -1 - error
 * */
int tm_stack_push(tm_stack_t *stack, void *data);

/**
 * tm_stack_pop - Pop top element out of stack
 *
 * @stack: Point to the stack
 *
 * @data: Where to save poped data
 *
 * @return: address - success
 *	       NULL - error
 * */
int tm_stack_pop(tm_stack_t *stack, void **data);

#ifdef __cplusplus
}
#endif

#endif /* TM_STACK_H */

