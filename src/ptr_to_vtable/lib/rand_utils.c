// Project includes.
#include "rand_utils.h"


// Declarations for inline functions to ensure symbol is emitted if inlining fails.
inline
uint64_t
rol64(const uint64_t __rol64_x, const int __rol64_k);

inline
uint64_t
splitmix64(uint64_t *const __splitmix64_state);
