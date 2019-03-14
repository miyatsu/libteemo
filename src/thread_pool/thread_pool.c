#include <stdio.h>

#ifndef __STD_NO_THREAD__
#include <threads.h>
#endif

#include "thrd_pool.h"

typedef struct thrd_pool_s {
	cnd_t cnd;
} thrd_pool_t;

int thrd_pool_init(int n)
{
	if (n <= 0) {
		return THRD_POOL_ERROR;
	}
}

int thrd_pool_push(thrd_pool_entry_t func)
{
	return 0;
}

static void internal_thrd_pool_loop(void)
{
	while (1) {
		/* Get one task */
		thrd_pool_p->task_list.new();
	}
}

