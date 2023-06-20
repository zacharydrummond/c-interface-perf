// Project includes.
#include <rand_interface.h>
#include <xoshiro256p_rand.h>
#include <xoshiro256ss_rand.h>
// Libc includes.
#include <inttypes.h>
#define _XOPEN_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


// Configuration.
#define N_IMPLS (16U)
#define N_ITER (100000000U)


static
int
loop_array_of_rand_interfaces(
	const size_t n_interfaces, const struct rand_interface *const *const restrict rands[n_interfaces], uint64_t *const result)
{
	int status = EXIT_FAILURE;

	const struct rand_interface *const **rand_array = calloc(N_ITER, sizeof rand_array[0]);
	if (!rand_array) {
		fprintf(stderr, "%s: could not allocate %zu bytes for ``rand_array``.\n", __func__, N_ITER * sizeof rand_array[0]);
		goto rand_array_calloc_error;
	}

	for (size_t idx = 0; idx < N_ITER; idx++) {
		size_t pick = drand48() * n_interfaces;
		rand_array[idx] = rands[pick];
	}

	uint64_t result_ = 0;

	{ // Timed loop.
		clock_t start_time, end_time;
		start_time = clock();

		// Critical loop.
		for (size_t idx = 0; idx < N_ITER; idx++) {
			result_ = rand_random(rand_array[idx]);
		}

		end_time = clock();
		double run_usecs = (double)(end_time - start_time) * 1e6 / CLOCKS_PER_SEC;

		printf("%.0f\n", run_usecs);
	}

	*result = result_;
	status = EXIT_SUCCESS;

rand_array_calloc_error:
	free(rand_array);

	return status;
}

static
int
loop_single_rand_interface(
	const struct rand_interface *const *const restrict rand, uint64_t *const result)
{
	int status = EXIT_FAILURE;
	uint64_t result_ = 0;

	{ // Timed loop.
		clock_t start_time, end_time;
		start_time = clock();

		// Critical loop.
		for (size_t idx = 0; idx < N_ITER; idx++) {
			result_ = rand_random(rand);
		}

		end_time = clock();
		double run_usecs = (double)(end_time - start_time) * 1e6 / CLOCKS_PER_SEC;

		printf("%.0f\n", run_usecs);
	}

	*result = result_;
	status = EXIT_SUCCESS;

	return status;
}

int main(void)
{
	int app_status = EXIT_FAILURE;

	long seed = time(NULL);
	srand48(seed++);
	printf("test\t\tusec\n");

	uint64_t test_1_result = 0;
	{ // Looping over many of a mix of rand_interface implementations.
		struct xoshiro256ss_rand *xoshiro256ss_rands[N_IMPLS] = {0};
		for (size_t idx = 0; idx < N_IMPLS; idx++) {
			xoshiro256ss_rands[idx] = xoshiro256ss_rand_create(seed++);
			if (!xoshiro256ss_rands[idx]) {
				fprintf(stderr, "%s: failed to create ``struct xoshiro256ss_rand`` instance.\n", __func__);
				goto test_1_xoshiro256ss_rand_create_error;
			}
		}

		struct xoshiro256p_rand *xoshiro256p_rands[N_IMPLS] = {0};
		for (size_t idx = 0; idx < N_IMPLS; idx++) {
			xoshiro256p_rands[idx] = xoshiro256p_rand_create(seed++);
			if (!xoshiro256p_rands[idx]) {
				fprintf(stderr, "%s: failed to create ``struct xoshiro256p_rand`` instance.\n", __func__);
				goto test_1_xoshiro256p_rand_create_error;
			}
		}

		const struct rand_interface *const *rands[N_IMPLS + N_IMPLS] = {0};
		for (size_t idx = 0; idx < N_IMPLS; idx++) rands[idx] = xoshiro256ss_rand_as_interface(xoshiro256ss_rands[idx]);
		for (size_t idx = 0; idx < N_IMPLS; idx++) rands[idx + N_IMPLS] = xoshiro256p_rand_as_interface(xoshiro256p_rands[idx]);

		printf("many_mixed\t");
		int test_1_status = loop_array_of_rand_interfaces(sizeof rands / sizeof rands[0], rands, &test_1_result);
		if (test_1_status == EXIT_FAILURE) {
			fprintf(stderr, "%s: error encountered while running test 1.\n", __func__);
			goto test_1_error;
		}

test_1_error:
test_1_xoshiro256p_rand_create_error:
		for (size_t idx = 0; idx < N_IMPLS; idx++) xoshiro256p_rand_destroy(xoshiro256p_rands[idx]);

test_1_xoshiro256ss_rand_create_error:
		for (size_t idx = 0; idx < N_IMPLS; idx++) xoshiro256ss_rand_destroy(xoshiro256ss_rands[idx]);
	}

	uint64_t test_2_result = 0;
	{ // Looping over many of a single rand_interface implementation.
		struct xoshiro256ss_rand *xoshiro256ss_rands[2 * N_IMPLS] = {0};
		for (size_t idx = 0; idx < 2 * N_IMPLS; idx++) {
			xoshiro256ss_rands[idx] = xoshiro256ss_rand_create(seed++);
			if (!xoshiro256ss_rands[idx]) {
				fprintf(stderr, "%s: failed to create ``struct xoshiro256ss_rand`` instance.\n", __func__);
				goto test_2_xoshiro256ss_rand_create_error;
			}
		}

		const struct rand_interface *const *rands[2 * N_IMPLS] = {0};
		for (size_t idx = 0; idx < 2 * N_IMPLS; idx++) rands[idx] = xoshiro256ss_rand_as_interface(xoshiro256ss_rands[idx]);

		printf("many_single\t");
		int test_2_status = loop_array_of_rand_interfaces(sizeof rands / sizeof rands[0], rands, &test_2_result);
		if (test_2_status == EXIT_FAILURE) {
			fprintf(stderr, "%s: error encountered while running test 2.\n", __func__);
			goto test_2_error;
		}

test_2_error:
test_2_xoshiro256ss_rand_create_error:
		for (size_t idx = 0; idx < 2 * N_IMPLS; idx++) xoshiro256ss_rand_destroy(xoshiro256ss_rands[idx]);
	}

	uint64_t test_3_result = 0;
	{ // Looping with one randomly picked interface.
		struct xoshiro256ss_rand *xoshiro256ss_rand = xoshiro256ss_rand_create(seed++);
		if (!xoshiro256ss_rand) {
			fprintf(stderr, "%s: failed to create ``struct xoshiro256ss_rand`` instance.\n", __func__);
			goto test_3_xoshiro256ss_rand_create_error;
		}

		struct xoshiro256p_rand *xoshiro256p_rand = xoshiro256p_rand_create(seed++);
		if (!xoshiro256p_rand) {
			fprintf(stderr, "%s: failed to create ``struct xoshiro256p_rand`` instance.\n", __func__);
			goto test_3_xoshiro256p_rand_create_error;
		}

		const struct rand_interface *const *rand = {0};
		if (drand48() < 0.5) {
			rand = xoshiro256ss_rand_as_interface(xoshiro256ss_rand);
		} else {
			rand = xoshiro256p_rand_as_interface(xoshiro256p_rand);
		}

		printf("single_mixed\t");
		int test_3_status = loop_single_rand_interface(rand, &test_3_result);
		if (test_3_status == EXIT_FAILURE) {
			fprintf(stderr, "%s: error encountered while running test 3.\n", __func__);
			goto test_3_error;
		}

test_3_error:
test_3_xoshiro256p_rand_create_error:
		xoshiro256p_rand_destroy(xoshiro256p_rand);

test_3_xoshiro256ss_rand_create_error:
		xoshiro256ss_rand_destroy(xoshiro256ss_rand);
	}

	uint64_t test_4_result = 0;
	{ // Looping with one known interface.
		struct xoshiro256ss_rand *xoshiro256ss_rand = xoshiro256ss_rand_create(seed++);
		if (!xoshiro256ss_rand) {
			fprintf(stderr, "%s: failed to create ``struct xoshiro256ss_rand`` instance.\n", __func__);
			goto test_4_xoshiro256ss_rand_create_error;
		}
		const struct rand_interface *const *rand = xoshiro256ss_rand_as_interface(xoshiro256ss_rand);

		printf("known_single\t");
		int test_4_status = loop_single_rand_interface(rand, &test_4_result);
		if (test_4_status == EXIT_FAILURE) {
			fprintf(stderr, "%s: error encountered while running test 4.\n", __func__);
			goto test_4_error;
		}

test_4_error:
test_4_xoshiro256ss_rand_create_error:
		xoshiro256ss_rand_destroy(xoshiro256ss_rand);
	}

	uint64_t test_result_aggregate = test_1_result & test_2_result & test_3_result & test_4_result;
	printf("\ntest aggregate: %"PRIu64"\n", test_result_aggregate);

	app_status = EXIT_SUCCESS;

	return app_status;
}
