#ifndef __RAND_INTERFACE_H__
#define __RAND_INTERFACE_H__

#include <stdint.h>

struct rand_interface {
	uint64_t (*const random)(const struct rand_interface *const *const restrict rand);
};

#define rand_random(rand) ((*(rand))->random((rand)))

#endif /* __RAND_INTERFACE_H__ */
