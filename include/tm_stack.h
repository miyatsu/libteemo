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
 * @priv: Teemo stack private data
 * */
typedef struct tm_stack_s {
	void *priv;
} tm_stack_t;

/**
 * enum tm_stack_option_e - Option when create a stack
 *
 * All flags here can use "|" to combine each one of them.
 *
 * @TM_STACK_FLAG_MULTI_THREAD: This stack may access by different thread,
 *				lower layer should make sure each operation
 *				is thread safe.
 * */
enum tm_stack_option_e {
	TM_STACK_OPTION_MULTI_THREAD = 0x00000001u,

	TM_STACK_OPTION_MAX = 0x00000002u,
} tm_stack_option_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * tm_stack_get_option - Get current stack option
 *
 * @stack: Point to the stack
 *
 * @option: Where to save option
 *
 * @return:  0 - success
 *	    -1 - error
 * */
int tm_stack_get_option(tm_stack_t *stack, unsigned long *option);

/**
 * tm_stack_set_option - Set current stack option
 *
 * @stack: Point to the stack
 *
 * @option: Option value
 *
 * @return:  0 - success
 *	    -1 - error
 * */
int tm_stack_set_option(tm_stack_t *stack, unsigned long option);

/**
 * tm_stack_init - Initialize a stack
 *
 * @stack: Point to the stack
 *
 * @option: Option of this stack, see tm_stack_option_t.
 *
 * @return:  0 - success
 *	    -1 - error
 * */
int tm_stack_init(tm_stack_t *stack, unsigned long option);

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

