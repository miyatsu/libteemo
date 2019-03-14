#ifndef THRD_POOL_H
#define THRD_POOL_H

enum thrd_pool_return_status {
	THRD_POOL_OK = 0,
	THRD_POOL_ERROR = -1,
};

typedef void (thrd_pool_entry_t *)(void);

#endif /* THRD_POOL_H */

