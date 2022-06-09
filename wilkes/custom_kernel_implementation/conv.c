/*
 * * File name    : conv.c
 * * Authors      : Avinash Eega (aeega)
 * * Created on   : Apr 02, 2022
 * * Description  : This is the 1st stage kernel design
 * */

/*
 * CJH Notes
 * Project include files should come before system include files
 * A good include-file methodology is:
 *
 * "this file's header, full path from project root"
 * --space--
 * "this project's other headers, full path from project root"
 * "wilkes/blah/blah_1.h"
 * "wilkes/blah/blah_2.h"
 * --space--
 * <third/party/includes>
 * <OpenCV/a_thing.h>
 * --space--
 * <system includes>
 * <stdio.h>
 * <math.h>
 *
 * clang-format will sort each of these blocks lexigraphically,
 * which is good because it prevents you from relying on include-order
 */

#include "wilkes/custom_kernel_implementation/conv.h"

#include <omp.h>
#include <stdint.h>
#include <stdio.h>

/*@brief: conv function is for implementing first stage of the CED aglorithm.
 * Function arguments:
 *  globals -> global variables
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
          float* temp) {
    // Get local copies of globals to work with
    DECLARE_AND_GLOBALS_TO_LOCALS(*globals);

    double in_col = (double)in_c;
    double num_pan = ceil(in_col / num_c);
    int num_panels = (int)num_pan;
    int num_columns;

    float kernel0 = kernel[0];
    float kernel1 = kernel[1];
    float kernel2 = kernel[2];
    float kernel3 = kernel[3];
    float kernel4 = kernel[4];
    float kernel5 = kernel[5];
    float kernel6 = kernel[6];
    float kernel7 = kernel[7];
    float kernel8 = kernel[8];

    int columns_left = in_c - (num_panels - 1) * num_c;
    for (int p = 0; p < num_panels; p++) {
        if (p < (num_panels - 1)) {
            num_columns = num_c + 2;
        } else {
            num_columns = columns_left;
        }

/************************** Kernel starts *************************/
/******************************************************************/
/************************* Epilogue starts ************************/
/******************************************************************/
#pragma omp parallel for
        for (int i = 0; i < num_columns / 16; i++) {
            float* index0 = a + in_c * 0 + i * 16 + p * num_c;
            float* index1 = a + in_c * 1 + i * 16 + p * num_c;
            float* index2 = a + in_c * 2 + i * 16 + p * num_c;

            r[0] = _mm256_loadu_ps(index0);
            r[1] = _mm256_loadu_ps(index1);
            r[2] = _mm256_loadu_ps(index2);

            k0 = _mm256_broadcast_ss(&kernel0);
            k1 = _mm256_broadcast_ss(&kernel3);
            k2 = _mm256_broadcast_ss(&kernel6);

            FMADD_PS(r[0], k0, out2);
            FMADD_PS(r[1], k0, out3);
            FMADD_PS(r[2], k0, out4);

            FMADD_PS(r[1], k1, out2);
            FMADD_PS(r[2], k1, out3);

            FMADD_PS(r[2], k2, out2);

            // Next set of independant chains

            r[0] = _mm256_loadu_ps(index0 + 8);
            r[1] = _mm256_loadu_ps(index1 + 8);
            r[2] = _mm256_loadu_ps(index2 + 8);

            FMADD_PS(r[0], k0, out7);
            FMADD_PS(r[1], k0, out8);
            FMADD_PS(r[2], k0, out9);

            FMADD_PS(r[1], k1, out7);
            FMADD_PS(r[2], k1, out8);

            FMADD_PS(r[2], k2, out7);

            // Next set of independant chains

            /* **************************************************************/

            r[0] = _mm256_loadu_ps(index0 + 1);
            r[1] = _mm256_loadu_ps(index1 + 1);
            r[2] = _mm256_loadu_ps(index2 + 1);

            k0 = _mm256_broadcast_ss(&kernel1);
            k1 = _mm256_broadcast_ss(&kernel4);
            k2 = _mm256_broadcast_ss(&kernel7);

            FMADD_PS(r[0], k0, out2);
            FMADD_PS(r[1], k0, out3);
            FMADD_PS(r[2], k0, out4);

            FMADD_PS(r[1], k1, out2);
            FMADD_PS(r[2], k1, out3);

            FMADD_PS(r[2], k2, out2);

            // Next set of independant chain

            r[0] = _mm256_loadu_ps(index0 + 9);
            r[1] = _mm256_loadu_ps(index1 + 9);
            r[2] = _mm256_loadu_ps(index2 + 9);

            FMADD_PS(r[0], k0, out7);
            FMADD_PS(r[1], k0, out8);
            FMADD_PS(r[2], k0, out9);

            FMADD_PS(r[1], k1, out7);
            FMADD_PS(r[2], k1, out8);

            FMADD_PS(r[2], k2, out7);

            // Next set of independant chain

            /* *************************************************************/

            r[0] = _mm256_loadu_ps(index0 + 2);
            r[1] = _mm256_loadu_ps(index1 + 2);
            r[2] = _mm256_loadu_ps(index2 + 2);

            k0 = _mm256_broadcast_ss(&kernel2);
            k1 = _mm256_broadcast_ss(&kernel5);
            k2 = _mm256_broadcast_ss(&kernel8);

            FMADD_PS(r[0], k0, out2);
            FMADD_PS(r[1], k0, out3);
            FMADD_PS(r[2], k0, out4);

            FMADD_PS(r[1], k1, out2);
            FMADD_PS(r[2], k1, out3);

            FMADD_PS(r[2], k2, out2);

            // Next set of independant chains

            r[0] = _mm256_loadu_ps(index0 + 10);
            r[1] = _mm256_loadu_ps(index1 + 10);
            r[2] = _mm256_loadu_ps(index2 + 10);

            FMADD_PS(r[0], k0, out7);
            FMADD_PS(r[1], k0, out8);
            FMADD_PS(r[2], k0, out9);

            FMADD_PS(r[1], k1, out7);
            FMADD_PS(r[2], k1, out8);

            FMADD_PS(r[2], k2, out7);

            // Next set of independant chains

            // Store full partial sums
            _mm256_storeu_ps(result + i * 16 + p * num_c, out2);
            out2 = _mm256_setzero_ps();
            _mm256_storeu_ps(result + i * 16 + 8 + p * num_c, out7);
            out7 = _mm256_setzero_ps();

            // Store partial sums
            _mm256_storeu_ps(temp + num_c * 0 + i * 16, out3);
            out3 = _mm256_setzero_ps();
            _mm256_storeu_ps(temp + num_c * 1 + i * 16, out4);
            out4 = _mm256_setzero_ps();
            _mm256_storeu_ps(temp + num_c * 0 + 8 + i * 16, out8);
            out8 = _mm256_setzero_ps();
            _mm256_storeu_ps(temp + num_c * 1 + 8 + i * 16, out9);
            out9 = _mm256_setzero_ps();
        }

// printf("Epilogue ends\n");
/******************************************************************/
/************************* Epilogue ends **************************/
/******************************************************************/

/******************************************************************/
/************************ Looping Iteration ***********************/
/******************************************************************/
#pragma omp parallel for
        for (int j = 3; j < rows; j = j + 3) {
            for (int i = 0; i < num_columns / 16; i++) {
                out0 = _mm256_loadu_ps(temp + num_c * 0 + i * 16);
                out1 = _mm256_loadu_ps(temp + num_c * 1 + i * 16);
                out5 = _mm256_loadu_ps(temp + num_c * 0 + 8 + i * 16);
                out6 = _mm256_loadu_ps(temp + num_c * 1 + 8 + i * 16);

                out2 = _mm256_setzero_ps();
                out3 = _mm256_setzero_ps();
                out4 = _mm256_setzero_ps();

                out7 = _mm256_setzero_ps();
                out8 = _mm256_setzero_ps();
                out9 = _mm256_setzero_ps();

                float* index0 = a + in_c * (j + 0) + i * 16 + p * num_c;
                float* index1 = a + in_c * (j + 1) + i * 16 + p * num_c;
                float* index2 = a + in_c * (j + 2) + i * 16 + p * num_c;

                r[0] = _mm256_loadu_ps(index0);
                r[1] = _mm256_loadu_ps(index1);
                r[2] = _mm256_loadu_ps(index2);

                k0 = _mm256_broadcast_ss(&kernel0);
                k1 = _mm256_broadcast_ss(&kernel3);
                k2 = _mm256_broadcast_ss(&kernel6);

                FMADD_PS(r[0], k2, out0);
                FMADD_PS(r[0], k1, out1);
                FMADD_PS(r[0], k0, out2);

                FMADD_PS(r[1], k2, out1);
                FMADD_PS(r[1], k1, out2);
                FMADD_PS(r[1], k0, out3);

                FMADD_PS(r[2], k2, out2);
                FMADD_PS(r[2], k1, out3);
                FMADD_PS(r[2], k0, out4);

                // Next set of independant chains

                r[0] = _mm256_loadu_ps(index0 + 8);
                r[1] = _mm256_loadu_ps(index1 + 8);
                r[2] = _mm256_loadu_ps(index2 + 8);

                FMADD_PS(r[0], k2, out5);
                FMADD_PS(r[0], k1, out6);
                FMADD_PS(r[0], k0, out7);

                FMADD_PS(r[1], k2, out6);
                FMADD_PS(r[1], k1, out7);
                FMADD_PS(r[1], k0, out8);

                FMADD_PS(r[2], k2, out7);
                FMADD_PS(r[2], k1, out8);
                FMADD_PS(r[2], k0, out9);

                /*******************************************************************************/

                r[0] = _mm256_loadu_ps(index0 + 1);
                r[1] = _mm256_loadu_ps(index1 + 1);
                r[2] = _mm256_loadu_ps(index2 + 1);

                k0 = _mm256_broadcast_ss(&kernel1);
                k1 = _mm256_broadcast_ss(&kernel4);
                k2 = _mm256_broadcast_ss(&kernel7);

                FMADD_PS(r[0], k2, out0);
                FMADD_PS(r[0], k1, out1);
                FMADD_PS(r[0], k0, out2);

                FMADD_PS(r[1], k2, out1);
                FMADD_PS(r[1], k1, out2);
                FMADD_PS(r[1], k0, out3);

                FMADD_PS(r[2], k2, out2);
                FMADD_PS(r[2], k1, out3);
                FMADD_PS(r[2], k0, out4);

                // ext set of independant chains

                r[0] = _mm256_loadu_ps(index0 + 9);
                r[1] = _mm256_loadu_ps(index1 + 9);
                r[2] = _mm256_loadu_ps(index2 + 9);

                FMADD_PS(r[0], k2, out5);
                FMADD_PS(r[0], k1, out6);
                FMADD_PS(r[0], k0, out7);

                FMADD_PS(r[1], k2, out6);
                FMADD_PS(r[1], k1, out7);
                FMADD_PS(r[1], k0, out8);

                FMADD_PS(r[2], k2, out7);
                FMADD_PS(r[2], k1, out8);
                FMADD_PS(r[2], k0, out9);

                /*******************************************************************************/

                r[0] = _mm256_loadu_ps(index0 + 2);
                r[1] = _mm256_loadu_ps(index1 + 2);
                r[2] = _mm256_loadu_ps(index2 + 2);

                k0 = _mm256_broadcast_ss(&kernel2);
                k1 = _mm256_broadcast_ss(&kernel5);
                k2 = _mm256_broadcast_ss(&kernel8);

                FMADD_PS(r[0], k2, out0);
                FMADD_PS(r[0], k1, out1);
                FMADD_PS(r[0], k0, out2);

                FMADD_PS(r[1], k2, out1);
                FMADD_PS(r[1], k1, out2);
                FMADD_PS(r[1], k0, out3);

                FMADD_PS(r[2], k2, out2);
                FMADD_PS(r[2], k1, out3);
                FMADD_PS(r[2], k0, out4);

                // Next set of independant chains

                r[0] = _mm256_loadu_ps(index0 + 10);
                r[1] = _mm256_loadu_ps(index1 + 10);
                r[2] = _mm256_loadu_ps(index2 + 10);

                FMADD_PS(r[0], k2, out5);
                FMADD_PS(r[0], k1, out6);
                FMADD_PS(r[0], k0, out7);

                FMADD_PS(r[1], k2, out6);
                FMADD_PS(r[1], k1, out7);
                FMADD_PS(r[1], k0, out8);

                FMADD_PS(r[2], k2, out7);
                FMADD_PS(r[2], k1, out8);
                FMADD_PS(r[2], k0, out9);

                _mm256_storeu_ps(result + out_c * (j - 2) + i * 16 + p * num_c,
                                 out0);
                out0 = _mm256_setzero_ps();
                _mm256_storeu_ps(result + out_c * (j - 1) + i * 16 + p * num_c,
                                 out1);
                out1 = _mm256_setzero_ps();
                _mm256_storeu_ps(result + out_c * j + i * 16 + p * num_c, out2);
                out2 = _mm256_setzero_ps();
                _mm256_storeu_ps(
                  result + out_c * (j - 2) + 8 + i * 16 + p * num_c, out5);
                out5 = _mm256_setzero_ps();
                _mm256_storeu_ps(
                  result + out_c * (j - 1) + 8 + i * 16 + p * num_c, out6);
                out6 = _mm256_setzero_ps();
                _mm256_storeu_ps(result + out_c * j + 8 + i * 16 + p * num_c,
                                 out7);
                out7 = _mm256_setzero_ps();

                // Store partial sums
                _mm256_storeu_ps(temp + num_c * 0 + i * 16, out3);
                out3 = _mm256_setzero_ps();
                _mm256_storeu_ps(temp + num_c * 1 + i * 16, out4);
                out4 = _mm256_setzero_ps();
                _mm256_storeu_ps(temp + num_c * 0 + 8 + i * 16, out8);
                out8 = _mm256_setzero_ps();
                _mm256_storeu_ps(temp + num_c * 1 + 8 + i * 16, out9);
                out9 = _mm256_setzero_ps();
            }
        }
    }

    // Put results back into globals for output
    LOCALS_TO_GLOBALS(*globals);
}
