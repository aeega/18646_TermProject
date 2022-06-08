#pragma once

/*
 * * File name    : conv2.c
 * * Authors      : Avinash Eega (aeega)
 * * Created on   : Apr 05, 2022
 * * Description  : This is the 2st stage kernel design
 * */

/*
 * CJH Notes
 * Typically, system includes have angle brackets: <...>
 * and project includes have quotes "..."
 */

#include "wilkes/custom_kernel_implementation/globals.h"

#ifdef __cplusplus
extern "C" {
#endif

/*@brief: conv2 function is for implementing second stage of the CED aglorithm.
 * Function arguments:
 *  globals -> Global variables
 *  a       -> Input image matrix
 *  sobel_x -> Sobel kernel for x axis
 *  sobel_y -> Sobel kernel for y axis
 *  rows    -> Input image number of rows
 *  result_x-> Output image matrix -> windowImagex_ij sructure in openCV
 * implementation result_y-> Output image matrix -> windowImagey_ij sructure in
 * openCV implementation temp_*  -> Temporary variables used to store the data
 * while calculating result_x and result_y
 */
void conv2(Globals* globals,
           float* a,
           float sobel_x[9],
           float sobel_y[9],
           int rows,
           int cols,
           float* result_x,
           float* result_y,
           float* D,
           float* Theta,
           float* temp_x,
           float* temp_y);

#ifdef __cplusplus
}  // end extern "C"
#endif
