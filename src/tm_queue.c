/*
 * Copyright (C) 2019 Ding Tao <i@dingtao.org>
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <threads.h>

#include "tm_queue.h"

/**
 * struct tm_queue_attribute_s - Queue attribute
 *
 * This structure have some attributes we need to care about, like muti-thread
 * access mutex or binary compatible flags.
 *
 * @option: Option of this queue
 *
 * @lock: Mutex lock for push/pop
 * */
typedef struct tm_queue_attribute_s {
	unsigned long option;
	mtx_t lock;
} tm_queue_attribute_t;

/**
 * struct tm_queue_item_s - Item structure to save elements
 *
 * @item: Item address
 *
 * @next: Link to next item structure
 * */
typedef struct tm_queue_item_s {
	void *item;
	struct tm_queue_item_s *next;
} tm_queue_item_t;

/**
 * struct tm_queue_priv_s - Private structure of queue
 *
 * @attribute: Attribute of this queue
 *
 * @top: Top pointer
 * */
typedef struct tm_queue_priv_s {
	tm_queue_attribute_t *attribute;
	tm_queue_item_t *head;
	tm_queue_item_t *tail;
} tm_queue_priv_t;


static int tm_queue_internal_get_option(tm_queue_priv_t **priv,
					unsigned long *option)
{
	if (NULL == option) {
		return -1;
	}

	*option = (*priv)->attribute->option;

	return 0;
}

static int tm_queue_internal_set_option(tm_queue_priv_t **priv,
					unsigned long option)
{
	if (option >= TM_QUEUE_OPTION_MAX) {
		return -1;
	}

	(*priv)->attribute->option = option;

	return 0;
}

static int tm_queue_internal_push(tm_queue_priv_t **priv, void *data)
{
	tm_queue_item_t *item;

	/* Alloc space for data */
	item = (tm_queue_item_t*)malloc(sizeof(tm_queue_item_t));
	if (NULL == item) {
		return -1;
	}

	/* Save data */
	item->item = data;

	/* No next item */
	item->next = NULL;

	/* Push item to queue */

	if ((*priv)->attribute->option | TM_QUEUE_OPTION_MULTI_THREAD) {
		mtx_lock(&(*priv)->attribute->lock);
	}

	if (NULL == (*priv)->tail) {
		/* Empty queue */

		(*priv)->tail = item;
		(*priv)->head = item;
	} else {
		/* None empty queue */

		(*priv)->tail->next = item;
		(*priv)->tail = item;
	}

	if ((*priv)->attribute->option | TM_QUEUE_OPTION_MULTI_THREAD) {
		mtx_unlock(&(*priv)->attribute->lock);
	}

	return 0;
}

static int tm_queue_internal_pop(tm_queue_priv_t **priv, void **data)
{
	tm_queue_item_t *item;

	if ((*priv)->attribute->option | TM_QUEUE_OPTION_MULTI_THREAD) {
		mtx_lock(&(*priv)->attribute->lock);
	}

	/* Queue empty */
	if (NULL == (*priv)->head) {
		if ((*priv)->attribute->option | TM_QUEUE_OPTION_MULTI_THREAD) {
			mtx_unlock(&(*priv)->attribute->lock);
		}
		return -1;
	}

	/* Get item */
	item = (*priv)->head;

	/* Pop item from queue */

	if (NULL == (*priv)->head->next) {
		/* queue have only one item */

		(*priv)->head = NULL;
		(*priv)->tail = NULL;
	} else {
		/* queue have more than one item */

		(*priv)->head = (*priv)->head->next;
	}

	if ((*priv)->attribute->option | TM_QUEUE_OPTION_MULTI_THREAD) {
		mtx_unlock(&(*priv)->attribute->lock);
	}

	if (NULL != data) {
		*data = item->item;
	}
	free(item);

	return 0;
}

static int tm_queue_internal_init(tm_queue_priv_t **priv, unsigned long option)
{
	(*priv) = (tm_queue_priv_t*)malloc(sizeof(tm_queue_priv_t));
	if (NULL == (*priv)) {
		return -1;
	}

	(*priv)->attribute =
		(tm_queue_attribute_t*)malloc(sizeof(tm_queue_attribute_t));
	if (NULL == (*priv)->attribute) {
		free(*priv);
		return -1;
	}

	(*priv)->attribute->option = option;

	mtx_init(&(*priv)->attribute->lock, mtx_plain);

	(*priv)->head = NULL;
	(*priv)->tail = NULL;

	return 0;
}

static int tm_queue_internal_destroy(tm_queue_priv_t **priv)
{
	int ret;

	mtx_destroy(&(*priv)->attribute->lock);

	free((*priv)->attribute);

	/* Remove all item */
	do {
		ret = tm_queue_internal_pop(priv, NULL);
	} while (ret == 0);

	free(*priv);

	return 0;
}


int tm_queue_get_option(tm_queue_t *queue, unsigned long *option)
{
	if (NULL == queue) {
		return -1;
	}

	if (NULL == queue->priv) {
		return -1;
	}

	return tm_queue_internal_get_option((tm_queue_priv_t**)&queue->priv, option);
}

int tm_queue_set_option(tm_queue_t *queue, unsigned long option)
{
	if (NULL == queue) {
		return -1;
	}

	if (NULL == queue->priv) {
		return -1;
	}

	return tm_queue_internal_set_option((tm_queue_priv_t**)&queue->priv, option);
}

int tm_queue_init(tm_queue_t *queue, unsigned long option)
{
	if (NULL == queue) {
		return -1;
	}

	queue->priv = NULL;

	return tm_queue_internal_init((tm_queue_priv_t**)&queue->priv, option);
}

int tm_queue_destroy(tm_queue_t *queue)
{
	if (NULL == queue) {
		return -1;
	}

	if (NULL == queue->priv) {
		return -1;
	}

	return tm_queue_internal_destroy((tm_queue_priv_t**)&queue->priv);
}

int tm_queue_push(tm_queue_t *queue, void *data)
{
	if (NULL == queue) {
		return -1;
	}

	if (NULL == queue->priv) {
		return -1;
	}

	return tm_queue_internal_push((tm_queue_priv_t**)&queue->priv, data);
}

int tm_queue_pop(tm_queue_t *queue, void **data)
{
	if (NULL == queue) {
		return -1;
	}

	if (NULL == queue->priv) {
		return -1;
	}

	return tm_queue_internal_pop((tm_queue_priv_t**)&queue->priv, data);
}

