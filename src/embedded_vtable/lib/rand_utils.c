// Project includes.
#include "rand_utils.h"


// Declarations for inline functions to ensure symbol is emitted if inlining
// fails. Parameter names omitted to prevent issues with macros. See "Modern C"
// by Jens Gustedt for more details on best practice for inline functions.
uint64_t
rol64(const uint64_t, const int);

uint64_t
splitmix64(uint64_t *const);
