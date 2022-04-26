/*
 * * File name    : conv2.c
 * * Authors      : Avinash Eega (aeega) 
 * * Created on   : Apr 05, 2022
 * * Description  : This is the 2st stage kernel design 
 * */
#define BLOCK_SIZE3x 3
double sum = 0;

void ced_stages345(float **dest, float **D_new, float **D_new_temp, float *D, float **thresh, float **theta_new_temp, float *Theta, int rows, int cols){
    //memset(dest, 0, sizeof(dest));
    //memset(D_new, 0, sizeof(D_new));
    //memset(D_new_temp, 0, sizeof(D_new_temp));
    //memset(thresh, 0, sizeof(thresh));
    //memset(theta_new_temp, 0, sizeof(theta_new_temp));
    printf("BADCOW\n");

    // Convert the 1D arrays to 2D arrays
    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            D_new_temp[r][c] = D[r*cols + c];
        }
    }
    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            theta_new_temp[r][c] = Theta[r*cols + c];
        }
    }
    int theta_counter = 0;
    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            if((int)theta_new_temp[r][c] != (int)Theta[r*cols + c]){
                //printf("[CED_345]: theta_new_temp[%d][%d] = %f\t Theta = %f\n", r, c, theta_new_temp[r][c], Theta[r*cols+c]);
                theta_counter++;
            }
        }
    }
    printf("Changes in theta values = %d\n", theta_counter);
    
    
    //Non-Maximum suppression
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            /* For each shift in the stride of 1 on rows and columns 
               - o - 
               - o - 
               - o -  
            */   
            // @TODO: Likely hit this condition. Check
            if(theta_new_temp[i+1][j+1] == 0) {
                if((D_new_temp[i+1][j+1] > D_new_temp[i+2][j+1]) 
                 && D_new_temp[i+1][j+1] > D_new_temp[i][j+1]) {
                    thresh[i+1][j+1] = 255;
                    D_new[i+1][j+1] = D_new_temp[i+1][j+1];
                }
            } else if(theta_new_temp[i+1][j+1] == 45){
            /* For each shift in the stride of 1 on rows and columns 
               o - -
               - o - 
               - - o  
            */   
                if((D_new_temp[i+1][j+1] > D_new_temp[i+2][j+2]) 
                 && D_new_temp[i+1][j+1] > D_new_temp[i][j]) {
                    thresh[i+1][j+1] = 255;
                    D_new[i+1][j+1] = D_new_temp[i+1][j+1];
                }
            } else if(theta_new_temp[i+1][j+1] == 90) {
                /* For each shift in the stride of 1 on rows and columns 
                   - - -
                   o o o 
                   - - -  
                */   
               if((D_new_temp[i+1][j+1] > D_new_temp[i+1][j+2]) 
                     && D_new_temp[i+1][j+1] > D_new_temp[i+1][j]) {
                        thresh[i+1][j+1] = 255;
                        D_new[i+1][j+1] = D_new_temp[i+1][j+1];
                    }
            } else if(theta_new_temp[i+1][j+1] == 135) {
                /* For each shift in the stride of 1 on rows and columns 
                   - - o
                   - o - 
                   o - -  
                */   
                if((D_new_temp[i+1][j+1] > D_new_temp[i+2][j]) 
                 && D_new_temp[i+1][j+1] > D_new_temp[i][j+2]) {
                    thresh[i+1][j+1] = 255;
                    D_new[i+1][j+1] = D_new_temp[i+1][j+1];
                }
            }
        } //j
    } //i
    
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
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
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
     
}
