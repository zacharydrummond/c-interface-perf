#ifndef __XOSIRO256P_RAND_H__
#define __XOSIRO256P_RAND_H__
// ``xoshiro256p`` implementation is a modified version of the one found here:
// https://en.wikipedia.org/wiki/Xorshift


#include <stdint.h>


struct xoshiro256p_rand *
xoshiro256p_rand_create(uint64_t seed);

struct rand_interface_i *
xoshiro256p_rand_as_interface(const struct xoshiro256p_rand *const xoshiro256p_rand);

void
xoshiro256p_rand_destroy(struct xoshiro256p_rand *const xoshiro256p_rand);


#endif /* __XOSIRO256P_RAND_H__ */
