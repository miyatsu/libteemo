/*
 * Copyright (C) 2019 Ding Tao <i@dingtao.org>
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#include <stdio.h>

#include <threads.h>

#include "thrd_pool.h"

/**
 * struct tm_thread_pool_attribute_s - Thread pool attribute
 *
 * This strucure have some attribute we need to care about, like which type of
 * this thread pool is, IO intensive or CPU intensive.
 *
 * @option: Option of this thread pool
 * */
typedef struct tm_thread_pool_attribute_s {
	unsigned long option;
} tm_thread_pool_attribute_t;

/**
 * struct tm_thread_pool_task_s - Thread pool task
 *
 * @entry_func: User specified thread entry function, this is typedef of
 *		"int (*)(void*)"
 *
 * @entry_func_arg: User specified thread argument
 *
 * @event_cb: Event callback
 * */
typedef struct tm_thread_pool_task_s {
	tm_thread_pool_task_entry_t entry_func;
	void *entry_func_arg;
	tm_thread_pool_task_event_t event_cb;
} tm_thread_pool_task_t;

typedef struct tm_thread_pool_task_excutor_s {
	thrd_t thread_id;
	unsigned long status;
} tm_thread_pool_task_excutor_t;

typedef struct tm_thread_pool_task_excutor_list_s {
	tm_thread_pool_task_excutor_t thread;
}

typedef struct tm_thread_pool_priv_s {
	tm_thread_pool_attribute_t attribute
	tm_queue_t task_queue;
} tm_thread_pool_priv_t;

typedef struct tm_thread_pool_task_att_s {
	unsigned long option;
}

typedef struct tm_thread_pool_task_priv_s {
	tm_thread_pool_task_option_t *option;
	tm_thread_pool_task_entry_t entry;
	tm_thread_pool_task_event_t event;
}

static int tm_thread_pool_internal_task_excutor_entry(void *arg)
{
	return 0;
}

static int tm_thread_pool_internal_init(tm_thread_pool_priv_t **priv, int number)
{
	int ret;

	(*priv) = (tm_thread_pool_priv_t*)malloc(sizeof(tm_thread_pool_priv_t));
	if (NULL == priv) {
		return -1;
	}

	ret = tm_queue_init(&(*priv)->task_queue, TM_QUEUE_OPTION_MULTI_THREAD);
	if (0 != ret) {
		free(*priv);
		*priv = NULL;
		return -1;
	}

	return 0;
}

static int tm_thread_pool_internal_task_commit(
					tm_thread_pool_priv_t **priv,
					tm_thread_pool_task_entry_t entry_func,
					tm_thread_pool_task_event_t event_cb)
{
	int ret;

	tm_thread_pool_task_t *task;

	task = (tm_thread_pool_task_t*)malloc(sizeof(tm_thread_pool_task_t));
	if (NULL == task) {
		return -1;
	}

	task->entry_func = entry_func;
	task->event_cb = event_cb;

	ret = tm_queue_push(&(*priv)->task_queue, task);
	if (0 != ret) {
		free(task);
		return -1;
	}

	return 0;
}

static int tm_thread_pool_internal_destroy(tm_thread_pool_priv_t **priv)
{
	int ret;

	ret = tm_queue_destroy(&(*priv)->task_queue);
	if (0 != ret) {
		return -1;
	}

	free(*priv);
	*priv = NULL;

	return 0;
}

int tm_thread_pool_init(tm_thread_pool_t *thread_pool, unsigned long number)
{
	if (NULL == thread_pool) {
		return -1;
	}

	thread_pool->priv = NULL;

	return tm_thread_pool_internal_init(
			(tm_thread_pool_priv_t**)&thread_pool->priv, number);
}

int tm_thread_pool_task_commit(tm_thread_pool_t *thread_pool,
			       tm_thread_pool_task_entry_t entry_func,
			       tm_thread_pool_task_event_t event_cb)
{
	if (NULL == thread_pool) {
		return -1;
	}

	if (NULL == thread_pool->priv) {
		return -1;
	}

	return tm_thread_pool_internal_task_commit(
			(tm_thread_pool_priv**)&thread_pool->priv,
			entry_func, event_cb);
}

int tm_thread_pool_destroy(tm_thread_pool_t *thread_pool)
{
	if (NULL == thread_pool) {
		return -1;
	}

	if (NULL == thread_pool->priv) {
		return -1;
	}

	return tm_thread_pool_internal_destroy(
			(tm_thread_pool_priv_t**)&thread_pool->priv);
}

