#pragma once
// Project includes.
#include <rand_interface.hpp>
// Library includes.
#include <cstdint>

class Xoshiro256ssRand : public RandInterface
{
	private:
		uint64_t state[4];
	public:
		Xoshiro256ssRand(uint64_t seed);
		uint64_t random();
		~Xoshiro256ssRand() = default;
};
