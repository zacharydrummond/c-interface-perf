#pragma once
#include <cstdint>

class RandInterface
{
	public:
		RandInterface() = default;
		virtual uint64_t random() = 0;
		virtual ~RandInterface() = default;
};
