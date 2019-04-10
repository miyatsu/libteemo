/*
 * Copyright (C) 2019 Ding Tao <i@dingtao.org>
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef TM_QUEUE_H
#define TM_QUEUE_H

/**
 * tm_queue_t - Teemo queue data structure
 *
 * @priv: Teemo queue private data
 * */
typedef struct tm_queue_s {
	void *priv;
} tm_queue_t;

/**
 * enum tm_queue_option_e - Option when create a queue
 *
 * All flags here can use "|" to combine each one of them.
 *
 * @TM_queue_FLAG_MULTI_THREAD: This queue may access by different thread,
 *				lower layer should make sure each operation
 *				is thread safe.
 * */
enum tm_queue_option_e {
	TM_QUEUE_OPTION_MULTI_THREAD = 0x00000001u,

	TM_QUEUE_OPTION_MAX = 0x00000002u,
} tm_queue_option_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * tm_queue_get_option - Get current queue option
 *
 * @queue: Point to the queue
 *
 * @option: Where to save option
 *
 * @return:  0 - success
 *	    -1 - error
 * */
int tm_queue_get_option(tm_queue_t *queue, unsigned long *option);

/**
 * tm_queue_set_option - Set current queue option
 *
 * @queue: Point to the queue
 *
 * @option: Option value
 *
 * @return:  0 - success
 *	    -1 - error
 * */
int tm_queue_set_option(tm_queue_t *queue, unsigned long option);

/**
 * tm_queue_init - Initialize a queue
 *
 * @queue: Point to the queue
 *
 * @option: Option of this queue, see tm_queue_option_t.
 *
 * @return:  0 - success
 *	    -1 - error
 * */
int tm_queue_init(tm_queue_t *queue, unsigned long option);

/**
 * tm_queue_destroy - Destroy a queue
 *
 * @queue: Point to the queue
 *
 * @return:  0 - success
 *	    -1 - error
 * */
int tm_queue_destroy(tm_queue_t *queue);

/**
 * tm_queue_push - Push one element into queue
 *
 * @queue: Point to the queue
 *
 * @data: Pointer of the data
 *
 * @return:  0 - success
 *	    -1 - error
 * */
int tm_queue_push(tm_queue_t *queue, void *data);

/**
 * tm_queue_pop - Pop top element out of queue
 *
 * @queue: Point to the queue
 *
 * @data: Where to save poped data
 *
 * @return: address - success
 *	       NULL - error
 * */
int tm_queue_pop(tm_queue_t *queue, void **data);

#ifdef __cplusplus
}
#endif

#endif /* TM_QUEUE_H */

