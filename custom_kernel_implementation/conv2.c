/*
 * * File name    : conv2.c
 * * Authors      : Avinash Eega (aeega) 
 * * Created on   : Apr 05, 2022
 * * Description  : This is the 2st stage kernel design 
 * */

__m256 r[3];         // 3 rows loaded column wise
__m256 out0, out1, out2, out3, out4, out5, out6, out7, out8, out9;       // 10 independant FMA chains
__m256 k0, k1, k2;   // broadcasted values for each row of sobel_x

/*@brief: conv2 function is for implementing second stage of the CED aglorithm. 
* Function arguments:
*  a       -> Input image matrix
*  sobel_x -> Sobel kernel for x axis
*  sobel_y -> Sobel kernel for y axis
*  rows    -> Input image number of rows
*  result_x-> Output image matrix -> windowImagex_ij sructure in openCV implementation
*  result_y-> Output image matrix -> windowImagey_ij sructure in openCV implementation
*  temp_*  -> Temporary variables used to store the data while calculating result_x and result_y
*/
void conv2 (float *a , float sobel_x[9], float sobel_y[9], int rows, float *result_x, float *result_y, float *temp_x, float *temp_y) {

    double in_col = (double)in_c;
    double num_pan = ceil(in_col/num_c);
    int num_panels = (int)num_pan;
    int num_columns;
    int columns_left = in_c - (num_panels-1)*num_c;

    unsigned long long time;;

    float sobel_x0, sobel_x1, sobel_x2, sobel_x3, sobel_x4, sobel_x5, sobel_x6, sobel_x7, sobel_x8, sobel_x9;
    float sobel_y0, sobel_y1, sobel_y2, sobel_y3, sobel_y4, sobel_y5, sobel_y6, sobel_y7, sobel_y8, sobel_y9;
    sobel_x0 = sobel_x[0];
    sobel_x1 = sobel_x[1];
    sobel_x2 = sobel_x[2];
    sobel_x3 = sobel_x[3];
    sobel_x4 = sobel_x[4];
    sobel_x5 = sobel_x[5];
    sobel_x6 = sobel_x[6];
    sobel_x7 = sobel_x[7];
    sobel_x8 = sobel_x[8];
    sobel_x9 = sobel_x[9];

    sobel_y0 = sobel_y[0];
    sobel_y1 = sobel_y[1];
    sobel_y2 = sobel_y[2];
    sobel_y3 = sobel_y[3];
    sobel_y4 = sobel_y[4];
    sobel_y5 = sobel_y[5];
    sobel_y6 = sobel_y[6];
    sobel_y7 = sobel_y[7];
    sobel_y8 = sobel_y[8];
    sobel_y9 = sobel_y[9];

    for (int p = 0 ; p < num_panels ; p++) {
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
        for (int i = 0 ; i < num_columns/8 ; i ++) {
            float* index0 = a + in_c*0 + i*8 + p*num_c;
            float* index1 = a + in_c*1 + i*8 + p*num_c;
            float* index2 = a + in_c*2 + i*8 + p*num_c;
            
            r[0] = _mm256_loadu_ps(index0);
            r[1] = _mm256_loadu_ps(index1);
            r[2] = _mm256_loadu_ps(index2);


            k0 = _mm256_broadcast_ss(&sobel_x0);
            k1 = _mm256_broadcast_ss(&sobel_x3);
            k2 = _mm256_broadcast_ss(&sobel_x6);

            FMADD_PS(r[0], k0, out2);
            FMADD_PS(r[1], k0, out3);
            FMADD_PS(r[2], k0, out4);

            FMADD_PS(r[1], k1, out2);
            FMADD_PS(r[2], k1, out3);

            FMADD_PS(r[2], k2, out2);


            // Next set of independant chains

         
            k0 = _mm256_broadcast_ss(&sobel_y0);
            k1 = _mm256_broadcast_ss(&sobel_y3);
            k2 = _mm256_broadcast_ss(&sobel_y6);

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

            k0 = _mm256_broadcast_ss(&sobel_x1);
            k1 = _mm256_broadcast_ss(&sobel_x4);
            k2 = _mm256_broadcast_ss(&sobel_x7);

            FMADD_PS(r[0], k0, out2);
            FMADD_PS(r[1], k0, out3);
            FMADD_PS(r[2], k0, out4);

            FMADD_PS(r[1], k1, out2);
            FMADD_PS(r[2], k1, out3);

            FMADD_PS(r[2], k2, out2);


            // Next set of independant chain
            
        
            k0 = _mm256_broadcast_ss(&sobel_y1);
            k1 = _mm256_broadcast_ss(&sobel_y4);
            k2 = _mm256_broadcast_ss(&sobel_y7);
            
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

            k0 = _mm256_broadcast_ss(&sobel_x2);
            k1 = _mm256_broadcast_ss(&sobel_x5);
            k2 = _mm256_broadcast_ss(&sobel_x8);

            FMADD_PS(r[0], k0, out2);
            FMADD_PS(r[1], k0, out3);
            FMADD_PS(r[2], k0, out4);

            FMADD_PS(r[1], k1, out2);
            FMADD_PS(r[2], k1, out3);

            FMADD_PS(r[2], k2, out2);


            // Next set of independant chains

                     
            k0 = _mm256_broadcast_ss(&sobel_y2);
            k1 = _mm256_broadcast_ss(&sobel_y5);
            k2 = _mm256_broadcast_ss(&sobel_y8);

            FMADD_PS(r[0], k0, out7);
            FMADD_PS(r[1], k0, out8);
            FMADD_PS(r[2], k0, out9);

            FMADD_PS(r[1], k1, out7);
            FMADD_PS(r[2], k1, out8);

            FMADD_PS(r[2], k2, out7);


            // Next set of independant chains


            // Store full partial sums
            _mm256_storeu_ps(result_x + i*8 + p*num_c, out2);
            out2 = _mm256_setzero_ps();
            _mm256_storeu_ps(result_y + i*8 + p*num_c, out7);
            out7 = _mm256_setzero_ps();
            
            // Store partial sums
            _mm256_storeu_ps(temp_x + num_c*0 + i*8, out3);
            out3 = _mm256_setzero_ps();
            _mm256_storeu_ps(temp_x + num_c*1 + i*8, out4);
            out4 = _mm256_setzero_ps();
            _mm256_storeu_ps(temp_y + num_c*0 + i*8, out8);
            out8 = _mm256_setzero_ps();
            _mm256_storeu_ps(temp_y + num_c*1 + i*8, out9);
            out9 = _mm256_setzero_ps();
        }

        /******************************************************************/
        /************************* Epilogue ends **************************/
        /******************************************************************/
        
        /******************************************************************/
        /************************ Looping Iteration ***********************/
        /******************************************************************/
        #pragma omp parallel for
        for (int j = 3 ; j < rows ; j = j + 3) {
           for (int i = 0 ; i < num_columns/8 ; i ++) {
               out0 = _mm256_loadu_ps(temp_x + num_c*0 + i*8);
               out1 = _mm256_loadu_ps(temp_x + num_c*1 + i*8);
               out5 = _mm256_loadu_ps(temp_y + num_c*0 + i*8);
               out6 = _mm256_loadu_ps(temp_y + num_c*1 + i*8);


               out2 = _mm256_setzero_ps();
               out3 = _mm256_setzero_ps();
               out4 = _mm256_setzero_ps();

               out7 = _mm256_setzero_ps();
               out8 = _mm256_setzero_ps();
               out9 = _mm256_setzero_ps();

               float* index0 = a + in_c*(j + 0) + i*8 + p*num_c;
               float* index1 = a + in_c*(j + 1) + i*8 + p*num_c;
               float* index2 = a + in_c*(j + 2) + i*8 + p*num_c;

               r[0] = _mm256_loadu_ps(index0);
               r[1] = _mm256_loadu_ps(index1);
               r[2] = _mm256_loadu_ps(index2);

               k0 = _mm256_broadcast_ss(&sobel_x0);
               k1 = _mm256_broadcast_ss(&sobel_x3);
               k2 = _mm256_broadcast_ss(&sobel_x6);

               FMADD_PS(r[0], k2, out0);
               FMADD_PS(r[0], k1, out1);
               FMADD_PS(r[0], k0, out2);

               FMADD_PS(r[1], k2, out1);
               FMADD_PS(r[1], k1, out2);
               FMADD_PS(r[1], k0, out3);

               FMADD_PS(r[2], k2, out2);
               FMADD_PS(r[2], k1, out3);
               FMADD_PS(r[2], k0, out4);


               //Next set of independant chains

               k0 = _mm256_broadcast_ss(&sobel_y0);
               k1 = _mm256_broadcast_ss(&sobel_y3);
               k2 = _mm256_broadcast_ss(&sobel_y6);

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

               k0 = _mm256_broadcast_ss(&sobel_x1);
               k1 = _mm256_broadcast_ss(&sobel_x4);
               k2 = _mm256_broadcast_ss(&sobel_x7);

               FMADD_PS(r[0], k2, out0);
               FMADD_PS(r[0], k1, out1);
               FMADD_PS(r[0], k0, out2);

               FMADD_PS(r[1], k2, out1);
               FMADD_PS(r[1], k1, out2);
               FMADD_PS(r[1], k0, out3);

               FMADD_PS(r[2], k2, out2);
               FMADD_PS(r[2], k1, out3);
               FMADD_PS(r[2], k0, out4);

               //Next set of independant chains


               
               k0 = _mm256_broadcast_ss(&sobel_y1);
               k1 = _mm256_broadcast_ss(&sobel_y4);
               k2 = _mm256_broadcast_ss(&sobel_y7);

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


               k0 = _mm256_broadcast_ss(&sobel_x2);
               k1 = _mm256_broadcast_ss(&sobel_x5);
               k2 = _mm256_broadcast_ss(&sobel_x8);

               FMADD_PS(r[0], k2, out0);
               FMADD_PS(r[0], k1, out1);
               FMADD_PS(r[0], k0, out2);

               FMADD_PS(r[1], k2, out1);
               FMADD_PS(r[1], k1, out2);
               FMADD_PS(r[1], k0, out3);

               FMADD_PS(r[2], k2, out2);
               FMADD_PS(r[2], k1, out3);
               FMADD_PS(r[2], k0, out4);

               //Next set of independant chains


               k0 = _mm256_broadcast_ss(&sobel_y2);
               k1 = _mm256_broadcast_ss(&sobel_y5);
               k2 = _mm256_broadcast_ss(&sobel_y8);

               FMADD_PS(r[0], k2, out5);
               FMADD_PS(r[0], k1, out6);
               FMADD_PS(r[0], k0, out7);

               FMADD_PS(r[1], k2, out6);
               FMADD_PS(r[1], k1, out7);
               FMADD_PS(r[1], k0, out8);

               FMADD_PS(r[2], k2, out7);
               FMADD_PS(r[2], k1, out8);
               FMADD_PS(r[2], k0, out9);


               _mm256_storeu_ps(result_x + out_c*(j-2) + i*8 + p*num_c, out0);
                out0 = _mm256_setzero_ps();
               _mm256_storeu_ps(result_x + out_c*(j-1) + i*8 + p*num_c, out1);
                out1 = _mm256_setzero_ps();
               _mm256_storeu_ps(result_x + out_c*j + i*8 + p*num_c, out2);
                out2 = _mm256_setzero_ps();
               _mm256_storeu_ps(result_y + out_c*(j-2) + i*8 + p*num_c, out5);
                out5 = _mm256_setzero_ps();
               _mm256_storeu_ps(result_y + out_c*(j-1) + i*8 + p*num_c, out6);
                out6 = _mm256_setzero_ps();
               _mm256_storeu_ps(result_y + out_c*j + i*8 + p*num_c, out7);
                out7 = _mm256_setzero_ps();

               // Store partial sums
               _mm256_storeu_ps(temp_x + num_c*0 + i*8, out3);
                out3 = _mm256_setzero_ps();
               _mm256_storeu_ps(temp_x + num_c*1 + i*8, out4);
                out4 = _mm256_setzero_ps();
               _mm256_storeu_ps(temp_y + num_c*0 + i*8, out8);
                out8 = _mm256_setzero_ps();
               _mm256_storeu_ps(temp_y + num_c*1 + i*8, out9);
                out9 = _mm256_setzero_ps();
           }
        }
    }
}
