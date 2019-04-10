/*
 * Copyright (C) 2019 Ding Tao <i@dingtao.org>
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <threads.h>

#include "tm_stack.h"

/**
 * struct tm_stack_attribute_s - Stack attribute
 *
 * This structure have some attributes we need to care about, like muti-thread
 * access mutex or binary compatible flags.
 *
 * @option: Option of this stack
 *
 * @lock: Mutex lock for push/pop
 * */
typedef struct tm_stack_attribute_s {
	unsigned long option;
	mtx_t lock;
} tm_stack_attribute_t;

/**
 * struct tm_stack_item_s - Item structure to save elements
 *
 * @item: Item address
 *
 * @next: Link to next item structure
 * */
typedef struct tm_stack_item_s {
	void *item;
	struct tm_stack_item_s *next;
} tm_stack_item_t;

/**
 * struct tm_stack_priv_s - Private structure of stack
 *
 * @attribute: Attribute of this stack
 *
 * @top: Top pointer
 * */
typedef struct tm_stack_priv_s {
	tm_stack_attribute_t *attribute;
	tm_stack_item_t *top;
} tm_stack_priv_t;


static int tm_stack_internal_get_option(tm_stack_priv_t **priv,
					unsigned long *option)
{
	if (NULL == option) {
		return -1;
	}

	*option = (*priv)->attribute->option;

	return 0;
}

static int tm_stack_internal_set_option(tm_stack_priv_t **priv,
					unsigned long option)
{
	if (option >= TM_STACK_OPTION_MAX) {
		return -1;
	}

	(*priv)->attribute->option = option;

	return 0;
}

static int tm_stack_internal_push(tm_stack_priv_t **priv, void *data)
{
	tm_stack_item_t *item;

	/* Alloc space for data */
	item = (tm_stack_item_t*)malloc(sizeof(tm_stack_item_t));
	if (NULL == item) {
		return -1;
	}

	/* Save data */
	item->item = data;

	/* Push item to stack */

	if ((*priv)->attribute->option | TM_STACK_OPTION_MULTI_THREAD) {
		mtx_lock(&(*priv)->attribute->lock);
	}

	if (NULL == (*priv)->top) {
		/* Empty stack */

		item->next = NULL;

		(*priv)->top = item;
	} else {
		/* None empty stack */

		item->next = (*priv)->top;

		(*priv)->top = item;
	}

	if ((*priv)->attribute->option | TM_STACK_OPTION_MULTI_THREAD) {
		mtx_unlock(&(*priv)->attribute->lock);
	}

	return 0;
}

static int tm_stack_internal_pop(tm_stack_priv_t **priv, void **data)
{
	tm_stack_item_t *item;

	if ((*priv)->attribute->option | TM_STACK_OPTION_MULTI_THREAD) {
		mtx_lock(&(*priv)->attribute->lock);
	}

	/* Stack empty */
	if (NULL == (*priv)->top) {
		if ((*priv)->attribute->option | TM_STACK_OPTION_MULTI_THREAD) {
			mtx_unlock(&(*priv)->attribute->lock);
		}
		return -1;
	}

	/* Get item */
	item = (*priv)->top;

	/* Pop item from stack */

	if (NULL == (*priv)->top->next) {
		/* Stack have only one item */

		(*priv)->top = NULL;
	} else {
		/* Stack have more than one item */

		(*priv)->top = (*priv)->top->next;
	}

	if ((*priv)->attribute->option | TM_STACK_OPTION_MULTI_THREAD) {
		mtx_unlock(&(*priv)->attribute->lock);
	}

	if (NULL != data) {
		*data = item->item;
	}
	free(item);

	return 0;
}

static int tm_stack_internal_init(tm_stack_priv_t **priv, unsigned long option)
{
	(*priv) = (tm_stack_priv_t*)malloc(sizeof(tm_stack_priv_t));
	if (NULL == (*priv)) {
		return -1;
	}

	(*priv)->attribute =
		(tm_stack_attribute_t*)malloc(sizeof(tm_stack_attribute_t));
	if (NULL == (*priv)->attribute) {
		free(*priv);
		return -1;
	}

	(*priv)->attribute->option = option;

	mtx_init(&(*priv)->attribute->lock, mtx_plain);

	(*priv)->top = NULL;

	return 0;
}

static int tm_stack_internal_destroy(tm_stack_priv_t **priv)
{
	int ret;

	mtx_destroy(&(*priv)->attribute->lock);

	free((*priv)->attribute);

	/* Remove all item */
	do {
		ret = tm_stack_internal_pop(priv, NULL);
	} while (ret == 0);

	free(*priv);

	(*priv) = NULL;

	return 0;
}


int tm_stack_get_option(tm_stack_t *stack, unsigned long *option)
{
	if (NULL == stack) {
		return -1;
	}

	if (NULL == stack->priv) {
		return -1;
	}

	return tm_stack_internal_get_option((tm_stack_priv_t**)&stack->priv, option);
}

int tm_stack_set_option(tm_stack_t *stack, unsigned long option)
{
	if (NULL == stack) {
		return -1;
	}

	if (NULL == stack->priv) {
		return -1;
	}

	return tm_stack_internal_set_option((tm_stack_priv_t**)&stack->priv, option);
}

int tm_stack_init(tm_stack_t *stack, unsigned long option)
{
	if (NULL == stack) {
		return -1;
	}

	stack->priv = NULL;

	return tm_stack_internal_init((tm_stack_priv_t**)&stack->priv, option);
}

int tm_stack_destroy(tm_stack_t *stack)
{
	if (NULL == stack) {
		return -1;
	}

	if (NULL == stack->priv) {
		return -1;
	}

	return tm_stack_internal_destroy((tm_stack_priv_t**)&stack->priv);
}

int tm_stack_push(tm_stack_t *stack, void *data)
{
	if (NULL == stack) {
		return -1;
	}

	if (NULL == stack->priv) {
		return -1;
	}

	return tm_stack_internal_push((tm_stack_priv_t**)&stack->priv, data);
}

int tm_stack_pop(tm_stack_t *stack, void **data)
{
	if (NULL == stack) {
		return -1;
	}

	if (NULL == stack->priv) {
		return -1;
	}

	return tm_stack_internal_pop((tm_stack_priv_t**)&stack->priv, data);
}

