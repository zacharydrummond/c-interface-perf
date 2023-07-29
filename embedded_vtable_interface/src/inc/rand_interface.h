#ifndef __RAND_INTERFACE_H__
#define __RAND_INTERFACE_H__

#include <stdint.h>

struct rand_interface_i {
    struct rand_interface_o *const state;
    uint64_t (*const random)(struct rand_interface_o *const restrict state);
};

#define rand_random(rand) ((rand)->random((rand)->state))

#endif /* __RAND_INTERFACE_H__ */
