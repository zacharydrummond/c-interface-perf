// Project includes.
#include <xoshiro256ss_rand.hpp>
#include "rand_utils.hpp"
// Library includes.	
#include <cstdint>

Xoshiro256ssRand::Xoshiro256ssRand(uint64_t seed)
{
	for (std::size_t idx = 0; idx < (sizeof state / sizeof state[0]); idx++) {
		state[idx] = splitmix64(&seed);
	}
}

uint64_t Xoshiro256ssRand::random()
{
	// Calculate random result.
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
