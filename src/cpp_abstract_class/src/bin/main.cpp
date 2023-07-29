// Project includes.
#include <rand_interface.hpp>
#include <xoshiro256p_rand.hpp>
#include <xoshiro256ss_rand.hpp>
// Library includes.
#include <cinttypes>
#include <iostream>
#include <vector>
#include <ctime>


// Configuration.
#define N_IMPLS (16U)
#define N_ITER (100000000U)


static
int
loop_array_of_rand_interfaces(
	std::vector<RandInterface *> &interfaces, uint64_t &result)
{
	int status = EXIT_FAILURE;

	std::vector<RandInterface *> rand_array;

	for (size_t idx = 0; idx < N_ITER; idx++) {
		size_t pick = drand48() * interfaces.size();
		rand_array.push_back(interfaces[pick]);
	}

	uint64_t result_ = 0;

	{ // Timed loop.
		clock_t start_time, end_time;
		start_time = clock();

		// Critical loop.
		for (RandInterface *rand : rand_array) {
			result_ = rand->random();
		}

		end_time = clock();
		double run_usecs = (double)(end_time - start_time) * 1e6 / CLOCKS_PER_SEC;

		printf("%.0f\n", run_usecs);
	}

	result = result_;
	status = EXIT_SUCCESS;

	return status;
}

static
int
loop_single_rand_interface(
	RandInterface *rand, uint64_t &result)
{
	int status = EXIT_FAILURE;
	uint64_t result_ = 0;

	{ // Timed loop.
		clock_t start_time, end_time;
		start_time = clock();

		// Critical loop.
		for (size_t idx = 0; idx < N_ITER; idx++) {
			result_ = rand->random();
		}

		end_time = clock();
		double run_usecs = (double)(end_time - start_time) * 1e6 / CLOCKS_PER_SEC;

		printf("%.0f\n", run_usecs);
	}

	result = result_;
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
	{ // Looping over many of a mix of RandInterface implementations.
		std::vector<Xoshiro256ssRand *> xoshiro256ss_rands;
		for (size_t idx = 0; idx < N_IMPLS; idx++) xoshiro256ss_rands.push_back(new Xoshiro256ssRand(seed++));

		std::vector<Xoshiro256pRand *> xoshiro256p_rands;
		for (size_t idx = 0; idx < N_IMPLS; idx++) xoshiro256p_rands.push_back(new Xoshiro256pRand(seed++));

		std::vector<RandInterface *> rands;
		for (size_t idx = 0; idx < N_IMPLS; idx++) rands.push_back(xoshiro256ss_rands[idx]);
		for (size_t idx = 0; idx < N_IMPLS; idx++) rands.push_back(xoshiro256p_rands[idx]);

		printf("many_mixed\t");
		int test_1_status = loop_array_of_rand_interfaces(rands, test_1_result);
		if (test_1_status == EXIT_FAILURE) {
			fprintf(stderr, "%s: error encountered while running test 1.\n", __func__);
			goto test_1_error;
		}

test_1_error:
		for (size_t idx = 0; idx < N_IMPLS; idx++) delete xoshiro256p_rands[idx];
		for (size_t idx = 0; idx < N_IMPLS; idx++) delete xoshiro256ss_rands[idx];
	}

	uint64_t test_2_result = 0;
	{ // Looping over many of a single RandInterface implementation.
		std::vector<Xoshiro256ssRand *> xoshiro256ss_rands;
		for (size_t idx = 0; idx < 2 * N_IMPLS; idx++) xoshiro256ss_rands.push_back(new Xoshiro256ssRand(seed++));

		std::vector<RandInterface *> rands;
		for (size_t idx = 0; idx < 2 * N_IMPLS; idx++) rands.push_back(xoshiro256ss_rands[idx]);

		printf("many_single\t");
		int test_2_status = loop_array_of_rand_interfaces(rands, test_2_result);
		if (test_2_status == EXIT_FAILURE) {
			fprintf(stderr, "%s: error encountered while running test 2.\n", __func__);
			goto test_2_error;
		}

test_2_error:
		for (size_t idx = 0; idx < 2 * N_IMPLS; idx++) delete xoshiro256ss_rands[idx];
	}

	uint64_t test_3_result = 0;
	{ // Looping with one randomly picked interface.
		Xoshiro256ssRand *xoshiro256ss_rand = new Xoshiro256ssRand(seed++);
		Xoshiro256pRand *xoshiro256p_rand = new Xoshiro256pRand(seed++);

		RandInterface *rand;
		if (drand48() < 0.5) {
			rand = xoshiro256ss_rand;
		} else {
			rand = xoshiro256p_rand;
		}

		printf("single_mixed\t");
		int test_3_status = loop_single_rand_interface(rand, test_3_result);
		if (test_3_status == EXIT_FAILURE) {
			fprintf(stderr, "%s: error encountered while running test 3.\n", __func__);
			goto test_3_error;
		}

test_3_error:
		delete xoshiro256ss_rand;
		delete xoshiro256p_rand;
	}

	uint64_t test_4_result = 0;
	{ // Looping with one known interface.
		Xoshiro256ssRand *xoshiro256ss_rand = new Xoshiro256ssRand(seed++);
		RandInterface *rand = xoshiro256ss_rand;

		printf("known_single\t");
		int test_4_status = loop_single_rand_interface(rand, test_4_result);
		if (test_4_status == EXIT_FAILURE) {
			fprintf(stderr, "%s: error encountered while running test 4.\n", __func__);
			goto test_4_error;
		}

test_4_error:
		delete xoshiro256ss_rand;
	}

	uint64_t test_result_aggregate = test_1_result & test_2_result & test_3_result & test_4_result;
	printf("\ntest aggregate: %" PRIu64 "\n", test_result_aggregate);

	app_status = EXIT_SUCCESS;

	return app_status;
}
