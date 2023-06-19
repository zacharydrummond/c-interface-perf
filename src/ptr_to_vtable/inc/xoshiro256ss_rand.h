#ifndef __XOSHIRO256SS_RAND_H__
#define __XOSHIRO256SS_RAND_H__


#include <stdint.h>


struct xoshiro256ss_rand *
xoshiro256ss_rand_create(uint64_t seed);

const struct rand_interface *const *
xoshiro256ss_rand_as_interface(const struct xoshiro256ss_rand *const xoshiro256ss);

void
xoshiro256ss_rand_destroy(struct xoshiro256ss_rand *const xoshiro256ss);


#endif /* __XOSHIRO256SS_RAND_H__ */
