/*
 * * File name    : conv2.c
 * * Authors      : Avinash Eega (aeega) 
 * * Created on   : Apr 05, 2022
 * * Description  : This is the 2st stage kernel design 
 * */
//#include <omp.h>
#define BLOCK_SIZE3x 3
double sum = 0;
unsigned long long st3, et3, st5, et5;

//void ced_stages345(float **dest, float **D_new, float **D_new_temp, float *D, float **thresh, float **theta_new_temp, float *Theta, int rows, int cols){
void ced_stages345(float **dest, float **D_new, float *D, float **thresh, float *Theta, int rows, int cols){
    //memset(dest, 0, sizeof(dest));
    //memset(D_new, 0, sizeof(D_new));
    //memset(D_new_temp, 0, sizeof(D_new_temp));
    //memset(thresh, 0, sizeof(thresh));
    //memset(theta_new_temp, 0, sizeof(theta_new_temp));
    printf("BADCOW\n");

    //Non-Maximum suppression
    //#pragma omp num_threads(24)
    //int total_threads = omp_get_num_threads();  
    //int chunksize = (rows-2)*(cols-2)/ total_threads; 
    st3 = rdtsc();
    //#pragma omp parallel for schedule(dynamic,chunksize)
    for(int i = 0; i < rows-2; i++) {
        for(int j = 0; j < cols-2; j++) {
            /* For each shift in the stride of 1 on rows and columns 
               - o - 
               - o - 
               - o -  
            */   
            // @TODO: Likely hit this condition. Check
            if(Theta[(i+1)*cols + (j+1)] == 0) {
                if((D[(i+1)*cols + (j+1)] > D[(i+2)*cols + (j+1)]) 
                 && D[(i+1)*cols + (j+1)] > D[(i)*cols + (j+1)]) {
                    thresh[i+1][j+1] = 255;
                    D_new[i+1][j+1] = D[(i+1)*cols + (j+1)];
                }
            } else if(Theta[(i+1)*cols + (j+1)] == 45){
            /* For each shift in the stride of 1 on rows and columns 
               o - -
               - o - 
               - - o  
            */   
                if((D[(i+1)*cols + (j+1)] > D[(i+2)*cols + (j+2)]) 
                 && D[(i+1)*cols + (j+1)] > D[(i)*cols + (j)]) {
                    thresh[i+1][j+1] = 255;
                    D_new[i+1][j+1] = D[(i+1)*cols + (j+1)];
                }
            } else if(Theta[(i+1)*cols + (j+1)] == 90) {
                /* For each shift in the stride of 1 on rows and columns 
                   - - -
                   o o o 
                   - - -  
                */   
               if((D[(i+1)*cols + (j+1)] > D[(i+1)*cols + (j+2)]) 
                     && D[(i+1)*cols + (j+1)] > D[(i+1)*cols + (j)]) {
                        thresh[i+1][j+1] = 255;
                        D_new[i+1][j+1] = D[(i+1)*cols + (j+1)];
                    }
            } else if(Theta[(i+1)*cols + (j+1)] == 135) {
                /* For each shift in the stride of 1 on rows and columns 
                   - - o
                   - o - 
                   o - -  
                */   
                if((D[(i+1)*cols + (j+1)] > D[(i+2)*cols + (j)]) 
                 && D[(i+1)*cols + (j+1)] > D[(i)*cols + (j+2)]) {
                    thresh[i+1][j+1] = 255;
                    D_new[i+1][j+1] = D[(i+1)*cols + (j+1)];
                }
            }
        } //j
    } //i
    et3 = rdtsc() - st3;
    printf("[Custom]: Time for stage 3: %lld\n", et3);
    
   //@TODO:REMOVE DEBUG PRINTS
   // for(int i=0;i<rows; i++){
   //     for(int j=0;j<cols;j++){
   //         printf("D_new[%d][%d] = %f\t Thresh[%d][%d] = %f\n", i, j, D_new[i][j], i, j, thresh[i][j]);
   //     }
   // }

    // Stage 4
    // Hysteresis Thresholding
    
    // TODO: Use openMP to parallelize the mean finding
    // Mean value
    //for(int i = 0; i < rows; i++) {
    //    for(int j = 0; j < cols; j++) {
    //       sum += D_new[i][j];
    //    }
    //}
    //float meanValue = (float)(sum/(float)(rows*cols));
    //float t_high = meanValue;
    //float t_low = t_high/2;
    float t_high = 200;
    float t_low = t_high/2.3;

    // Check for continuity
    st5 = rdtsc();
    //#pragma omp parallel for schedule(dynamic,chunksize)
    for(int i = 0; i < rows-1; i++) {
        for(int j = 0; j < cols-1; j++) {
            // Check the center to decide 
            if(D_new[i+1][j+1] > t_high) {
                dest[i][j] = 255;
            } else if(D_new[i+1][j+1] < t_low) {
                continue;
            } else {
            // Check the whole BLOCK if center is ambiguous
                for(int k = 0; k < BLOCK_SIZE3x; k++) {
                    for(int l=0; l < BLOCK_SIZE3x; l++) {
                        if (k == 1 && l == 1){
                            continue;
                        } else if(D_new[i+k][j+l] > t_high) {
                            dest[i][j] = 255;
                        } 
                    }
                }
            } // end if
        }
    } // end for
     
    et5 = rdtsc() - st5;
    printf("[Custom]: Time for stage 4&5: %lld\n", et5);
}
