#pragma once

#include "wilkes/custom_kernel_implementation/globals.h"

#define BLOCK_SIZE3x 3

#ifdef __cplusplus
extern "C" {
#endif

void ced_stages345(Globals* globals,
                   float** dest,
                   float** D_new,
                   float* D,
                   float** thresh,
                   float* Theta,
                   int rows,
                   int cols);

#ifdef __cplusplus
}  // end extern C
#endif
