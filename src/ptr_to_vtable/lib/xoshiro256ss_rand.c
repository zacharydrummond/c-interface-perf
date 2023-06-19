// Project includes.
#include <xoshiro256ss_rand.h>
#include <rand_interface.h>
#include "containerof.h"
// Libc includes.
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


struct xoshiro256ss_rand {
    const struct rand_interface *const rand;
    uint64_t state[4];
};


static
uint64_t
xoshiro256ss_random(const struct rand_interface *const *const restrict rand)
{
	struct xoshiro256ss_rand *const restrict self = containerof(struct xoshiro256ss_rand, rand, rand);

	// Calculate random result.
	uint64_t *state = self->state;
	const uint64_t result = rol64(state[1] * 5, 7) * 9;

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
xoshiro256ss_rand_interface = {
    .random = xoshiro256ss_random
};

struct xoshiro256ss_rand *
xoshiro256ss_rand_create(uint64_t seed)
{
	if (!seed) {
		fprintf(stderr, "%s: ``seed`` cannot be 0.\n", __func__);
		goto value_error;
	}

	struct xoshiro256ss_rand *xoshiro256ss_rand = malloc(sizeof *xoshiro256ss_rand);
	if (!xoshiro256ss_rand) {
		fprintf(stderr, "%s: Could not allocate %zu bytes for an instance of ``struct xoshiro256ss_rand``.\n", __func__, sizeof *xoshiro256ss_rand);
		goto xoshiro256ss_rand_malloc_error;
	}

	struct xoshiro256ss_rand xoshiro256ss_rand_init = {
		.rand = &xoshiro256ss_rand_interface,
		.state = {0}
	};
	memcpy(xoshiro256ss_rand, &xoshiro256ss_rand_init, sizeof *xoshiro256ss_rand);

	// Initialize state array with splitmix64 algorithm.
	for (size_t idx = 0; idx < (sizeof xoshiro256ss_rand->state / sizeof xoshiro256ss_rand->state[0]); idx++) {
		xoshiro256ss_rand->state[idx] = splitmix64(&seed);
	}

	return xoshiro256ss_rand;

xoshiro256ss_rand_malloc_error:
	xoshiro256ss_rand_destroy(xoshiro256ss_rand);

value_error:
	return (void *)0;
}

const struct rand_interface *const *
xoshiro256ss_rand_as_interface(const struct xoshiro256ss_rand *const xoshiro256ss_rand)
{
	return &xoshiro256ss_rand->rand;
}

void
xoshiro256ss_rand_destroy(struct xoshiro256ss_rand *const xoshiro256ss_rand)
{
	if (xoshiro256ss_rand) {
		memset(xoshiro256ss_rand, 0, sizeof *xoshiro256ss_rand);
		free(xoshiro256ss_rand);
	}
}
