/*
 * Copyright (C) 2019 Ding Tao <i@dingtao.org>
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef TM_THREAD_POOL_H
#define TM_THREAD_POOL_H

/**
 * enum tm_thread_pool_option_e - Option when create a thread pool
 *
 * All flags here can use "|" to combine each one of them.
 *
 * @TM_THREAD_POOL_OPTION_INTENSIVE_CPU: This thread pool is used in CPU
 *					 intensive casese.
 *
 * @TM_THREAD_POOL_OPTION_INTENSIVE_IO: This thread pool is used in IO
 *					intensive casese.
 * */
typedef enum tm_thread_pool_option_e {
	TM_THREAD_POOL_OPTION_INTENSIVE_CPU = 0x00000001u,
	TM_THREAD_POOL_OPTION_INTENSIVE_IO = 0x00000002u,

	TM_THREAD_POOL_OPTION_MAX = 0x00000008,
} tm_thread_pool_option_t;

/**
 * tm_thread_pool_t - Teemo thread pool data structure
 *
 * @priv: Teemo thread pool private data
 * */
typedef struct tm_thread_pool_s {
	void *priv;
} tm_thread_pool_t;


/**
 * tm_thread_pool_task_entry_t - Task entry function type
 *
 * @arg: Entry point argument
 *
 * @return: This entry function should return a "void*" type of value, which
 *	    will be retrived via tm_thread_pool_task_event_t callback.
 * */
typedef void* (tm_thread_pool_task_entry_t *)(void *arg);

/**
 * enum tm_thread_pool_event_e - Task event callback type
 *
 * @TM_THREAD_POOL_EVENT_START: Callback with this type indicate this task
 *				is about to start, user can do some initial
 *				before really excute the task.
 *
 * @TM_THREAD_POOL_EVENT_END: Callback with this type indicate this task is
 *			      finished, user can do some deinitial.
 * */
typedef enum tm_thread_pool_event_e {
	TM_THREAD_POOL_EVENT_START = 1,
	TM_THREAD_POOL_EVENT_END = 2,
} tm_thread_pool_event_t;

/**
 * tm_thread_pool_task_event_t - Task event callback function type
 *
 * @event: Event of task, one of tm_thread_pool_event_t
 *
 * @task_status: Task return value
 * */
typedef void (tm_thread_pool_task_event_t *)(unsigned long event,
					     void *task_status);

typedef enum tm_thread_pool_task_option_e {
	TM_THREAD_POOL_TASK_OPTION_NO_RETURN = 0x00000001,

	TM_THREAD_POOL_TASK_OPTION_MAX = 0x00000001,
} tm_thread_pool_task_option_t;

typedef struct tm_thread_pool_task_s {
	void *priv;
} tm_thread_pool_task_t;


#ifdef __cplusplus
extern "C" {
#endif

int tm_thread_pool_init(tm_thread_pool_t *thread_pool, int thread_pool_size)

int tm_thread_pool_task_init(tm_thread_pool_task_t *task,
			     tm_thread_pool_task_entry_t entry,
			     tm_thread_pool_task_event_t event,
			     unsigned long option);

int tm_thread_pool_task_commit(tm_thread_pool_t *thread_pool,
			       tm_thread_pool_task_t *task);

int tm_thread_pool_task_destroy(tm_thread_pool_task_t *task)

int tm_thread_pool_destroy(tm_thread_pool_t *thread_pool);

#ifdef __cplusplus
}
#endif

#endif /* TM_THREAD_POOL_H */

