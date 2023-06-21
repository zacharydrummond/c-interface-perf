// Project includes.
#include <xoshiro256ss_rand.h>
#include <rand_interface.h>
#include "rand_utils.h"
// Libc includes.
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
// ``xoshiro256ss`` implementation is a modified version of the one found here:
// https://en.wikipedia.org/wiki/Xorshift


struct xoshiro256ss_rand {
    uint64_t state_array[4];
};


static
uint64_t
xoshiro256ss_random(struct rand_interface_o *const restrict state)
{
	struct xoshiro256ss_rand *const restrict self = (struct xoshiro256ss_rand *const restrict)state;

	// Calculate random result.
	uint64_t *state_array = self->state_array;
	const uint64_t result = rol64(state_array[1] * 5, 7) * 9;

	// Update state array.
	const uint64_t t = state_array[1] << 17;
	state_array[2] ^= state_array[0];
	state_array[3] ^= state_array[1];
	state_array[1] ^= state_array[2];
	state_array[0] ^= state_array[3];
	state_array[2] ^= t;
	state_array[3] = rol64(state_array[3], 45);

	return result;
}

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
		.state_array = {0}
	};
	memcpy(xoshiro256ss_rand, &xoshiro256ss_rand_init, sizeof *xoshiro256ss_rand);

	// Initialize state array with splitmix64 algorithm.
	for (size_t idx = 0; idx < (sizeof xoshiro256ss_rand->state_array / sizeof xoshiro256ss_rand->state_array[0]); idx++) {
		xoshiro256ss_rand->state_array[idx] = splitmix64(&seed);
	}

	return xoshiro256ss_rand;

xoshiro256ss_rand_malloc_error:
	xoshiro256ss_rand_destroy(xoshiro256ss_rand);

value_error:
	return (void *)0;
}

struct rand_interface_i *
xoshiro256ss_rand_as_interface(const struct xoshiro256ss_rand *const xoshiro256ss_rand)
{
	struct rand_interface_i *rand = malloc(sizeof *rand);
	if (!rand) {
		fprintf(stderr, "%s: failed to allocate %zu bytes for a ``struct rand_interface_i`` instance.\n", __func__, sizeof *rand);
		goto rand_malloc_error;
	}
	struct rand_interface_i rand_init = {
		.state = (struct rand_interface_o *const)xoshiro256ss_rand,
		.random = xoshiro256ss_random
	};
	memcpy(rand, &rand_init, sizeof *rand);

	return rand;

rand_malloc_error:
	free(rand);

	return (void *)0;
}

void
xoshiro256ss_rand_destroy(struct xoshiro256ss_rand *const xoshiro256ss_rand)
{
	if (xoshiro256ss_rand) {
		memset(xoshiro256ss_rand, 0, sizeof *xoshiro256ss_rand);
		free(xoshiro256ss_rand);
	}
}
