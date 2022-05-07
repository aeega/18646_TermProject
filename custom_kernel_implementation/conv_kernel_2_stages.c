/*
 * * File name    : conv_kernel_2_stages.c
 * * Authors      : Avinash Eega (aeega) 
 * * Created on   : Apr 08, 2022
 * * Description  : This is the wrapper file for two kernel stages. It contains
 *                  all the initializations and instantiating the two stage kernels 
 * */
#include <immintrin.h>
#include <stdio.h>
#include <math.h>
#include "get_img_rc.h"
#include "conv.c"
#include "conv2.c"
#include "ced_stages345.c"

#define AVERAGED 1000

__m256d main() {

    float *a, *result, *result_x, *result_y, *D, **D_new, *Theta, **thresh, **dest, *resultx_ae, *resulty_ae, *theo_result, *theo_resultx, *theo_resulty, *dest_1D;
    unsigned long long theo_st, theo_et, st, et, st2, et2;
    int i, j;

    // Extract num rows and cols 
    char *str = (char*)malloc(20* sizeof(char));
    FILE *fp, *fpo, *fpto, *fpi, *fptox, *fptoy, *fpox, *fpoy, *fptotheta, *fptod, *fptodnew, *fptothresh, *fptodest;
    fp = fopen("./test.csv", "r");
    char ch;
    int s = 0;


    // Allocate memory for input image
    get_rows_n_columns(); 
    a = (float*)malloc(sizeof(float)*in_r*in_c);
    int out_r = in_r - 2;

    // Allocate memory for temp and result array memory location
    float *temp = (float*)malloc(sizeof(float)*2*num_c);
    float *tempx = (float*)malloc(sizeof(float)*2*num_c);
    float *tempy = (float*)malloc(sizeof(float)*2*num_c);
    result = (float*)malloc(sizeof(float)*out_r*out_c);
    result_x = (float*)malloc(sizeof(float)*(out_r-2)*out_c);
    result_y = (float*)malloc(sizeof(float)*(out_r-2)*out_c);
    D = (float*)malloc(sizeof(float)*(out_r-2)*out_c);
    Theta = (float*)malloc(sizeof(float)*(out_r-2)*out_c);
    D_new = malloc((out_r-2) *sizeof(float *));
    for (int i=0;i<(out_r-2);i++) {
        D_new[i] = (float *)malloc((out_c)*sizeof(float));
    }
    thresh = malloc((out_r-2) *sizeof(float *));
    for (int i=0;i<(out_r-2);i++) {
        thresh[i] = (float *)malloc((out_c)*sizeof(float));
    }
    dest = malloc((out_r-2) *sizeof(float *));
    for (int i=0;i<(out_r-2);i++) {
        dest[i] = (float *)malloc((out_c)*sizeof(float));
    }
    dest_1D = (float*)malloc(sizeof(float)*(out_r-2)*out_c);
    //resultx_ae = (float*)malloc(sizeof(float)*(out_r-2)*out_c);
    //resulty_ae = (float*)malloc(sizeof(float)*(out_r-2)*out_c);
    theo_result = (float*)malloc(sizeof(float)*out_r*out_c);
    theo_resultx = (float*)malloc(sizeof(float)*(out_r-2)*out_c);
    theo_resulty = (float*)malloc(sizeof(float)*(out_r-2)*out_c);

    // Kernel stage 1    
    float f[9] = {0.0625, 0.125, 0.0625, 0.125, 0.25, 0.125, 0.0625, 0.0125, 0.0625};
    // Kernel stage 2
    float fx[9] = {-1, 0 , 1, -2, 0, 2, -1, 0, 1};
    float fy[9] = {-1, -2 , -1, 0, 0, 0, 1, 2, 1};

    //Initialize SIMD registers
    out0 = _mm256_set1_ps(0);
    out1 = _mm256_set1_ps(0);
    out2 = _mm256_set1_ps(0);
    out3 = _mm256_set1_ps(0);
    out4 = _mm256_set1_ps(0);
    out5 = _mm256_set1_ps(0);
    out6 = _mm256_set1_ps(0);
    out7 = _mm256_set1_ps(0);
    out8 = _mm256_set1_ps(0);
    out9 = _mm256_set1_ps(0);
    
    int flag;
    char* token;
    char *line = NULL;
    //size_t len = in_c * 10 + 10;
    size_t len = 0;
    printf("size of len=%d\n", len);
    int line_row = 0;
    int line_col = 0;
    int counter = 0;
    int pixel;


    
    //Parse pixel values from text.csv file
    while (getline(&line, &len, fp) != -1) {
        token = strtok(line, ",");
        //counter++;
        //printf("I am here %d times\n", counter); 
        while (token != NULL) {
            pixel = atoi(token);
            a[line_row*in_c + line_col] = pixel;
            line_col++;
            token = strtok(NULL, ",");

        }
        line_row ++;
        line_col = 0;
    }
    fclose(fp);
    //printf("[aeega]: line_rows = %d\n", in_r);
    //printf("[aeega]: line_cols = %d\n", in_c);
    

    theo_st = rdtsc();

    // Calculating convolution theoretically
    for (int x = 0 ; x < out_r ; x++) {
        for (int y = 0 ; y < in_c - 1 ; y++) {
            theo_result[x*in_c + y] = (a[x*in_c + y]       * f[0]) + (a[x*in_c + y + 1]       * f[1]) + (a[x*in_c + y + 2]       * f[2]) + 
                                      (a[(x + 1)*in_c + y] * f[3]) + (a[(x + 1)*in_c + y + 1] * f[4]) + (a[(x + 1)*in_c + y + 2] * f[5]) + 
                                      (a[(x + 2)*in_c + y] * f[6]) + (a[(x + 2)*in_c + y + 1] * f[7]) + (a[(x + 2)*in_c + y + 2] * f[8]);
        }
    } 
    for (int x = 0 ; x < out_r-2 ; x++) {
        for (int y = 0 ; y < in_c ; y++) {
            theo_resultx[x*in_c + y] = (theo_result[x*in_c + y]       * fx[0]) + (theo_result[x*in_c + y + 1]       * fx[1]) + (theo_result[x*in_c + y + 2]       * fx[2]) + 
                                      (theo_result[(x + 1)*in_c + y] * fx[3]) + (theo_result[(x + 1)*in_c + y + 1] * fx[4]) + (theo_result[(x + 1)*in_c + y + 2] * fx[5]) + 
                                      (theo_result[(x + 2)*in_c + y] * fx[6]) + (theo_result[(x + 2)*in_c + y + 1] * fx[7]) + (theo_result[(x + 2)*in_c + y + 2] * fx[8]);
        }
    } 
    for (int x = 0 ; x < out_r-2 ; x++) {
        for (int y = 0 ; y < in_c ; y++) {
            theo_resulty[x*in_c + y] = (theo_result[x*in_c + y]       * fy[0]) + (theo_result[x*in_c + y + 1]       * fy[1]) + (theo_result[x*in_c + y + 2]       * fy[2]) + 
                                      (theo_result[(x + 1)*in_c + y] * fy[3]) + (theo_result[(x + 1)*in_c + y + 1] * fy[4]) + (theo_result[(x + 1)*in_c + y + 2] * fy[5]) + 
                                      (theo_result[(x + 2)*in_c + y] * fy[6]) + (theo_result[(x + 2)*in_c + y + 1] * fy[7]) + (theo_result[(x + 2)*in_c + y + 2] * fy[8]);
        }
    } 
    theo_et = rdtsc()-theo_st;

    printf("Theoretical time taken: %lld\n", theo_et);



   // Custom kernel implementation 
    st = rdtsc();
    for(int i = 0; i < AVERAGED; i++ ) {
        conv (a, f, in_r-2, result, temp); 
    }
    et = rdtsc() - st;
    printf("[Custom]: Time for stage 1: %lld\n", ((et)/AVERAGED));

    st2 = rdtsc();
    for(int i = 0; i < AVERAGED; i++ ) {
        conv2 (result, fx, fy, in_r-4, out_c, result_x, result_y, D, Theta, tempx, tempy);
    }
    et2 = rdtsc() - st2;
    printf("[Custom]: Time for stage 2: %lld\n", (et2/AVERAGED));
    ced_stages345(dest, D_new, D, thresh, Theta, in_r-4, out_c);
    
    // Convert the dest[][] to dest_1D[] to print the image
    for(int i = 0; i < (out_r-2); i++) {
        for(int j = 0; j < out_c; j++) {
            dest_1D[i*out_c+j] = dest[i][j];
        }
    }

    printf("Improvement = %f\n", (float)theo_et*1000/(float)(et + et));


    fpo = fopen("./output.csv", "w+");
    fpto = fopen("./output_theo.csv", "w+");
    char* out_str = malloc(sizeof(int)*20);

    // Dump the stage 1 outputs into csv files to display the images
    for (int i = 0 ; i < out_r ; i++) {
        for (int j = 0 ; j < out_c ; j++) {
            //printf("%lf ", result[i*out_c + j]);
            if (j != (out_c - 1)) {
                sprintf(out_str, "%d, ", (int)result[i*out_c + j]);
                fputs(out_str, fpo);
                sprintf(out_str, "%d, ", (int)theo_result[i*out_c + j]);
                fputs(out_str, fpto);
            } else {
                sprintf(out_str, "%d", (int)result[i*out_c + j]);
                fputs(out_str, fpo);
                sprintf(out_str, "%d", (int)theo_result[i*out_c + j]);
                fputs(out_str, fpto);
            }
            if (theo_result[i*out_c + j] != result[i*out_c + j]) {
                //printf("Error found in 1st stage at index : %d. %d\n", i, j);
            }
        }
        fputc('\n',fpo);
        fputc('\n',fpto);
        //printf("\n\n");
    }
    fclose(fpo);
    fclose(fpto);

    // Dump the stage 2 and final outputs into csv files to display the images
    fpox = fopen("./output_x.csv", "w+");
    fpoy = fopen("./output_y.csv", "w+");
    fptox = fopen("./output_theox.csv", "w+");
    fptoy = fopen("./output_theoy.csv", "w+");
    fptotheta = fopen("./output_theta.csv", "w+");
    fptod = fopen("./output_d.csv", "w+");
    fptodnew = fopen("./output_dnew.csv", "w+");
    fptothresh = fopen("./output_thresh.csv", "w+");
    fptodest = fopen("./output_final.csv", "w+");
    for (int i = 0 ; i < out_r - 2 ; i++) {
        for (int j = 0 ; j < out_c ; j ++) {
            if (j != (out_c - 1)) {
                sprintf(out_str, "%d, ", (int)result_x[i*out_c + j]);
                fputs(out_str, fpox);
                sprintf(out_str, "%d, ", (int)result_y[i*out_c + j]);
                fputs(out_str, fpoy);
                sprintf(out_str, "%d, ", (int)theo_resultx[i*out_c + j]);
                fputs(out_str, fptox);
                sprintf(out_str, "%d, ", (int)theo_resulty[i*out_c + j]);
                fputs(out_str, fptoy);
                sprintf(out_str, "%d, ", (int)Theta[i*out_c + j]);
                fputs(out_str, fptotheta);
                sprintf(out_str, "%d, ", (int)D[i*out_c + j]);
                fputs(out_str, fptod);
                sprintf(out_str, "%d, ", (int)D_new[i][j]);
                fputs(out_str, fptodnew);
                sprintf(out_str, "%d, ", (int)thresh[i][j]);
                fputs(out_str, fptothresh);
                sprintf(out_str, "%d, ", (int)dest[i][j]);
                fputs(out_str, fptodest);
            } else {
                //Dont put comma on the last line
                sprintf(out_str, "%d", (int)result_x[i*out_c + j]);
                fputs(out_str, fpox);
                sprintf(out_str, "%d", (int)result_y[i*out_c + j]);
                fputs(out_str, fpoy);
                sprintf(out_str, "%d", (int)theo_resultx[i*out_c + j]);
                fputs(out_str, fptox);
                sprintf(out_str, "%d", (int)theo_resulty[i*out_c + j]);
                fputs(out_str, fptoy);
                sprintf(out_str, "%d ", (int)Theta[i*out_c + j]);
                fputs(out_str, fptotheta);
                sprintf(out_str, "%d ", (int)D[i*out_c + j]);
                fputs(out_str, fptod);
                sprintf(out_str, "%d ", (int)D_new[i][j]);
                fputs(out_str, fptodnew);
                sprintf(out_str, "%d ", (int)thresh[i][j]);
                fputs(out_str, fptothresh);
                sprintf(out_str, "%d ", (int)dest[i][j]);
                fputs(out_str, fptodest);
 
            }
        }
        fputc('\n',fptox);
        fputc('\n',fptoy);
        fputc('\n',fpox);
        fputc('\n',fpoy);
        fputc('\n',fptotheta);
        fputc('\n',fptod);
        fputc('\n',fptodnew);
        fputc('\n',fptothresh);
        fputc('\n',fptodest);
    }
    fclose(fptox);
    fclose(fptoy);
    fclose(fpox);
    fclose(fpoy);
    fclose(fptotheta);
    fclose(fptod);
    fclose(fptodnew);
    fclose(fptothresh);
    fclose(fptodest);

    // Free the malloced space
    for(int i = 0; i < (out_r-2); i++) {
        free(D_new[i]);
    }
    free(D_new);
    for(int i = 0; i < (out_r-2); i++) {
        free(thresh[i]);
    }
    free(thresh);
    for(int i = 0; i < (out_r-2); i++) {
        free(dest[i]);
    }
    free(dest);

    free(temp);
    free(tempx);
    free(tempy);
    free(result);
    free(result_x);
    free(result_y);
    free(D);
    free(dest_1D);
    free(Theta);
    free(theo_result);
    free(theo_resultx);
    free(theo_resulty);
 }
