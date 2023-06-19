// Project includes.
#include <xoshiro256p_rand.h>
#include <rand_interface.h>
#include "containerof.h"
#include "rand_utils.h"
// Libc includes.
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
// ``xoshiro256p`` implementation is a modified version of the one found here:
// https://en.wikipedia.org/wiki/Xorshift


struct xoshiro256p_rand {
    const struct rand_interface *const rand;
    uint64_t state[4];
};


static
uint64_t
xoshiro256p_random(const struct rand_interface *const *const restrict rand)
{
	struct xoshiro256p_rand *const restrict self = containerof(struct xoshiro256p_rand, rand, rand);

	// Calculate random result.
	uint64_t *state = self->state;
	const uint64_t result = s[0] + s[3];

	// Update State.
	const uint64_t t = state[1] << 17;
	state[2] ^= state[0];
	state[3] ^= state[1];
	state[1] ^= state[2];
	state[0] ^= state[3];
	state[2] ^= t;
	state[3] = rol64(state[3], 45);

	return result;
}

static
const struct rand_interface
xoshiro256p_rand_interface = {
    .random = xoshiro256p_random
};

struct xoshiro256p_rand *
xoshiro256p_rand_create(uint64_t seed)
{
	if (!seed) {
		fprintf(stderr, "%s: ``seed`` cannot be 0.\n", __func__);
		goto value_error;
	}

	struct xoshiro256p_rand *xoshiro256p_rand = malloc(sizeof *xoshiro256p_rand);
	if (!xoshiro256p_rand) {
		fprintf(stderr, "%s: Could not allocate %zu bytes for an instance of ``struct xoshiro256p_rand``.\n", __func__, sizeof *xoshiro256p_rand);
		goto xoshiro256p_rand_malloc_error;
	}

	struct xoshiro256p_rand xoshiro256p_rand_init = {
		.rand = &xoshiro256p_rand_interface,
		.state = {0}
	};
	memcpy(xoshiro256p_rand, &xoshiro256p_rand_init, sizeof *xoshiro256p_rand);

	// Initialize state array with splitmix64 algorithm.
	for (size_t idx = 0; idx < (sizeof xoshiro256p_rand->state / sizeof xoshiro256p_rand->state[0]); idx++) {
		xoshiro256p_rand->state[idx] = splitmix64(&seed);
	}

	return xoshiro256p_rand;

xoshiro256p_rand_malloc_error:
	xoshiro256p_rand_destroy(xoshiro256p_rand);

value_error:
	return (void *)0;
}

const struct rand_interface *const *
xoshiro256p_rand_as_interface(const struct xoshiro256p_rand *const xoshiro256p_rand)
{
	return &xoshiro256p_rand->rand;
}

void
xoshiro256p_rand_destroy(struct xoshiro256p_rand *const xoshiro256p_rand)
{
	if (xoshiro256p_rand) {
		memset(xoshiro256p_rand, 0, sizeof *xoshiro256p_rand);
		free(xoshiro256p_rand);
	}
}
