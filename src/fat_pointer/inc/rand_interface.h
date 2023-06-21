#ifndef __RAND_INTERFACE_H__
#define __RAND_INTERFACE_H__

#include <stdint.h>

struct rand_interface_i {
    struct rand_interface_o *const state;
    struct rand_interface_fn {
	    uint64_t (*const random)(struct rand_interface_o *const restrict state);
    } *const fn;
};

#define rand_random(rand) ((rand)->fn->random((rand)->state))

#endif /* __RAND_INTERFACE_H__ */
