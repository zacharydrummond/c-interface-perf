// Project includes.
#include <rand_interface.h>
#include <xoshiro256p_rand.h>
#include <xoshiro256ss_rand.h>
// Libc includes.
#define _XOPEN_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


// Configuration.
#define N_XOSHIRO256P (16U)
#define N_XOSHIRO256SS (16U)
#define N_ITER (100000000U)


static
int
loop_array_of_rand_interfaces(size_t n_interfaces, const struct rand_interface *const *const restrict rands[n_interfaces], uint64_t *result)
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

		for (size_t idx = 0; idx < N_ITER; idx++) {
			result_ = rand_random(rand_array[idx]);
		}

		end_time = clock();
		double run_usecs = (double)(end_time - start_time) * 1e6 / CLOCKS_PER_SEC;

		printf("%.15f\n", run_usecs);
	}

	*result = result_;
	status = EXIT_SUCCESS;

rand_array_calloc_error:
	free(rand_array);

	return status;
}

static
int
loop_single_rand_interface(const struct rand_interface *const *const restrict rand)
{
	int status = EXIT_FAILURE;
	return status;
}

int main(void)
{
	int app_status = EXIT_FAILURE;

	long seed = time(NULL);
	srand48(seed++);

	struct xoshiro256ss_rand *xoshiro256ss_rands[N_XOSHIRO256SS] = {0};
	for (size_t idx = 0; idx < N_XOSHIRO256SS; idx++) {
		xoshiro256ss_rands[idx] = xoshiro256ss_rand_create(seed++);
		if (!xoshiro256ss_rands[idx]) {
			fprintf(stderr, "%s: failed to create ``struct xoshiro256ss_rand`` instance.\n", __func__);
			goto xoshiro256ss_rand_create_error;
		}
	}

	struct xoshiro256p_rand *xoshiro256p_rands[N_XOSHIRO256P] = {0};
	for (size_t idx = 0; idx < N_XOSHIRO256P; idx++) {
		xoshiro256p_rands[idx] = xoshiro256p_rand_create(seed++);
		if (!xoshiro256p_rands[idx]) {
			fprintf(stderr, "%s: failed to create ``struct xoshiro256p_rand`` instance.\n", __func__);
			goto xoshiro256p_rand_create_error;
		}
	}

	const struct rand_interface *const *rands[N_XOSHIRO256SS + N_XOSHIRO256P] = {0};
	for (size_t idx = 0; idx < N_XOSHIRO256SS; idx++) rands[idx] = xoshiro256ss_rand_as_interface(xoshiro256ss_rands[idx]);
	for (size_t idx = 0; idx < N_XOSHIRO256P; idx++) rands[idx + N_XOSHIRO256SS] = xoshiro256p_rand_as_interface(xoshiro256p_rands[idx]);

	printf("test_1\t");
	uint64_t test_1_result = 0;
	int test_1_status = loop_array_of_rand_interfaces(sizeof rands / sizeof rands[0], rands, &test_1_result);
	if (test_1_status == EXIT_FAILURE) {
		fprintf(stderr, "%s: error encountered while running test 1.\n", __func__);
		goto test_1_error;
	}

	app_status = EXIT_SUCCESS;

test_1_error:
xoshiro256p_rand_create_error:
	for (size_t idx = 0; idx < N_XOSHIRO256P; idx++) xoshiro256p_rand_destroy(xoshiro256p_rands[idx]);

xoshiro256ss_rand_create_error:
	for (size_t idx = 0; idx < N_XOSHIRO256SS; idx++) xoshiro256ss_rand_destroy(xoshiro256ss_rands[idx]);

	return app_status;
}
