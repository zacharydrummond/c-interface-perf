#pragma once
// Project includes.
#include <rand_interface.hpp>
// Library includes.
#include <cstdint>

class Xoshiro256pRand : public RandInterface
{
	private:
		uint64_t state[4];
	public:
		Xoshiro256pRand(uint64_t seed);
		uint64_t random();
		~Xoshiro256pRand() = default;
};
