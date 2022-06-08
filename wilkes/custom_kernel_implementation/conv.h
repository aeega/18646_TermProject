#pragma once

#include "wilkes/custom_kernel_implementation/globals.h"

#ifdef __cplusplus
extern "C" {
#endif

/*@brief: conv function is for implementing first stage of the CED aglorithm.
 * Function arguments:
 *  globals -> global variables like num_c, out1, ...
 *  a       -> Input image matrix
 *  kernel  -> Guassian kernel for convolution
 *  rows    -> Input image number of rows
 *  result  -> Output image matrix -> vgbValues structure in openCV
 * implementation in stage1 temp    -> Temporary variable used to store the data
 * while calculating result
 */

void conv(Globals* globals,
          float* a,
          float kernel[9],
          int rows,
          float* result,
          float* temp);

#ifdef __cplusplus
}  // end extern "C"
#endif
