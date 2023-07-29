#ifndef __RAND_UTILS_H__
#define __RAND_UTILS_H__
// ``rol64`` and ``splitmix64`` are modified versions from those found here:
// https://en.wikipedia.org/wiki/Xorshift


#include <cstdint>


inline
uint64_t
rol64(const uint64_t __rol64_x, const int __rol64_k)
{
	return (__rol64_x << __rol64_k) | (__rol64_x >> (64 - __rol64_k));
}

inline
uint64_t
splitmix64(uint64_t *const __splitmix64_state)
{
	*__splitmix64_state += 0x9E3779B97f4A7C15;
	uint64_t out = *__splitmix64_state;
	out = (out ^ (out >> 30)) * 0xBF58476D1CE4E5B9;
	out = (out ^ (out >> 27)) * 0x94D049BB133111EB;
	return out;
}


#endif /* __RAND_UTILS_H__ */
