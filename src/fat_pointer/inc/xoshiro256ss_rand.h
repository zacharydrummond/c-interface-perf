#ifndef __XOSHIRO256SS_RAND_H__
#define __XOSHIRO256SS_RAND_H__
// ``xoshiro256ss`` implementation is a modified version of the one found here:
// https://en.wikipedia.org/wiki/Xorshift


#include <stdint.h>


struct xoshiro256ss_rand *
xoshiro256ss_rand_create(uint64_t seed);

struct rand_interface_i *
xoshiro256ss_rand_as_interface(const struct xoshiro256ss_rand *const xoshiro256ss_rand);

void
xoshiro256ss_rand_destroy(struct xoshiro256ss_rand *const xoshiro256ss_rand);


#endif /* __XOSHIRO256SS_RAND_H__ */
