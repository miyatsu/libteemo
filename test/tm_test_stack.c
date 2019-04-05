/*
 * Copyright (C) 2019 Ding Tao <i@dingtao.org>
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#include <stdio.h>
#include <stdlib.h>

#include "tm_stack.h"

int main(int argc, char *argv[])
{
	int i;
	int ret;
	void *data;
	tm_stack_t stack;

	ret = tm_stack_init(&stack, 0);
	if (ret) {
		printf("init error @%d\n", __LINE__);
		exit(-1);
	}

	for (i = 0; i < 100; i++) {
		data = malloc(sizeof(int));
		if (NULL == data) {
			printf("malloc error @%d\n", __LINE__);
			exit(-1);
		}
		*((int*)data) = i;

		ret = tm_stack_push(&stack, data);
		if (ret) {
			printf("push error @%d\n", __LINE__);
			exit(-1);
		}
	}

	int err_cnt = 0;
	for (i = 0; i < 100; i++) {
		ret = tm_stack_pop(&stack, &data);
		if (ret) {
			printf("pop error @%d\n", __LINE__);
			exit(-1);
		}
		if (*((int*)data) + i != 99) {
			err_cnt++;
		}
		free(data);
	}
	printf("ERR_CNT = %d\n", err_cnt);

	ret = tm_stack_destroy(&stack);
	if (ret) {
		printf("destory error @%d\n", __LINE__);
		exit(-1);
	}

	return 0;
}

