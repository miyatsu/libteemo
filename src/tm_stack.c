/*
 * Copyright (C) 2019 Ding Tao <i@dingtao.org>
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "tm_stack.h"

/**
 * struct tm_stack_attribute_s - Stack attribute
 *
 * This structure have some attributes we need to care about, like muti-thread
 * access mutex or binary compatible flags.
 *
 * TODO: Current not used.
 *
 * @reserved: Unused value.
 * */
typedef struct tm_stack_attribute_s {
	void *reserved;
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



static int tm_stack_internal_push(tm_stack_priv_t *priv, void *data)
{
	if (NULL == priv) {
		return -1;
	}

	tm_stack_item_t *item;

	/* Alloc space for data */
	item = (tm_stack_item_t*)malloc(sizeof(tm_stack_item_t));
	if (NULL == item) {
		return -1;
	}

	/* Save data */
	item->item = data;

	/* Push item to stack */

	if (NULL == priv->top) {
		/* Empty stack */

		item->next = NULL;

		priv->top = item;
	} else {
		/* None empty stack */

		item->next = priv->top;

		priv->top = item;
	}

	return 0;
}

static int tm_stack_internal_pop(tm_stack_priv_t *priv, void **data)
{
	if (NULL == priv) {
		return -1;
	}

	/* Stack empty */
	if (NULL == priv->top) {
		return -1;
	}

	tm_stack_item_t *item;

	/* Get item */
	item = priv->top;

	/* Pop item from stack */

	if (NULL == priv->top->next) {
		/* Stack have only one item */

		priv->top = NULL;
	} else {
		/* Stack have more than one item */

		priv->top = priv->top->next;
	}

	if (NULL != data) {
		*data = item->item;
	}
	free(item);

	return 0;
}

static int tm_stack_internal_init(tm_stack_priv_t *priv)
{
	if (NULL == priv) {
		return -1;
	}

	priv->attribute = (tm_stack_attribute_t*)malloc(sizeof(tm_stack_attribute_t));
	if (NULL == priv->attribute) {
		return -1;
	}

	/* TODO attribute currently not used, mark reserved as NULL */
	priv->attribute->reserved = NULL;

	priv->top = NULL;

	return 0;
}

static int tm_stack_internal_destroy(tm_stack_priv_t *priv)
{
	int ret;

	if (NULL == priv) {
		return -1;
	}

	free(priv->attribute);

	/* Remove all item */
	do {
		ret = tm_stack_internal_pop(priv, NULL);
	} while (ret == 0);

	return 0;
}

int tm_stack_init(tm_stack_t *stack)
{
	int ret;
	tm_stack_priv_t *priv;

	if (NULL == stack) {
		return -1;
	}

	priv = (tm_stack_priv_t*)malloc(sizeof(tm_stack_priv_t));
	if (NULL == priv) {
		return -1;
	}

	/* ret == 0 means initial success, other wise error */
	ret = tm_stack_internal_init(priv);
	if (ret) {
		free(priv);
		return ret;
	}

	stack->priv = priv;

	return 0;
}


int tm_stack_destroy(tm_stack_t *stack)
{
	int ret;

	if (NULL == stack) {
		return -1;
	}

	if (NULL == stack->priv) {
		return -1;
	}

	ret = tm_stack_internal_destroy((tm_stack_priv_t*)stack->priv);
	if (ret) {
		return ret;
	}

	free(stack->priv);

	return 0;
}


int tm_stack_push(tm_stack_t *stack, void *data)
{
	if (NULL == stack) {
		return -1;
	}

	if (NULL == stack->priv) {
		return -1;
	}

	return tm_stack_internal_push(stack->priv, data);
}

int tm_stack_pop(tm_stack_t *stack, void **data)
{
	if (NULL == stack) {
		return -1;
	}

	if (NULL == stack->priv) {
		return -1;
	}

	return tm_stack_internal_pop(stack->priv, data);
}

