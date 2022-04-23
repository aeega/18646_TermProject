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
#include "conv.c"
#include "conv2.c"

unsigned long long rdtsc() {   
    unsigned long long int x;   
    unsigned a, d;   
    __asm__ volatile("rdtsc" : "=a" (a), "=d" (d));
    return ((unsigned long long)a) | (((unsigned long long)d) << 32); 
}

__m256d main() {

    float *a, *result, *resultx, *resulty, *resultx_ae, *resulty_ae, *theo_result, *theo_resultx, *theo_resulty;
    unsigned long long theo_st, theo_et, st, et;
    int i, j;

    // Extract num rows and cols 
    char *str = (char*)malloc(20* sizeof(char));
    FILE *fp, *fpo, *fpto, *fpi, *fptox, *fptoy, *fpox, *fpoy;
    fp = fopen("./test.csv", "r");
    char ch;
    int s = 0;

    // Allocate memory for input image
    a = (float*)malloc(sizeof(float)*in_r*in_c);
    
    int out_r = in_r - 2;

    // Allocate memory for temp and result array memory location
    float *temp = (float*)malloc(sizeof(float)*2*num_c);
    float *tempx = (float*)malloc(sizeof(float)*2*num_c);
    float *tempy = (float*)malloc(sizeof(float)*2*num_c);
    result = (float*)malloc(sizeof(float)*out_r*out_c);
    resultx = (float*)malloc(sizeof(float)*(out_r-2)*out_c);
    resulty = (float*)malloc(sizeof(float)*(out_r-2)*out_c);
    resultx_ae = (float*)malloc(sizeof(float)*(out_r-2)*out_c);
    resulty_ae = (float*)malloc(sizeof(float)*(out_r-2)*out_c);
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
    size_t len = in_c * 10 + 10;
    int line_row = 0;
    int line_col = 0;
    int pixel;
    
    //Parse pixel values from text.csv file
    while (getline(&line, &len, fp) != -1) {
        token = strtok(line, ",");
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
    theo_et = rdtsc();
    theo_et = rdtsc();

    printf("Theoretical time taken: %lld\n", theo_et - theo_st);


    st = rdtsc();

   // Custom kernel implementation 
    for(int i = 0; i < 1000; i++ ) {
        conv (a, f, in_r-2, result, temp); 
        conv2 (result, fx, fy, in_r-4, resultx, resulty, tempx, tempy);
    }
    et = rdtsc() - st;

    printf("Observed avg time taken over 1000 iter Total: %lld\n", ((et)/1000));


    fpo = fopen("./output.csv", "w+");
    fpto = fopen("./output_theo.csv", "w+");
    char* out_str = malloc(sizeof(int)*20);
    for (int i = 0 ; i < out_r ; i++) {
        for (int j = 0 ; j < out_c ; j ++) {
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
                printf("Error found in 1st stage at index : %d. %d\n", i, j);
            }
        }
        fputc('\n',fpo);
        fputc('\n',fpto);
        //printf("\n\n");
    }
    fclose(fpo);
    fclose(fpto);
    fpox = fopen("./outputx.csv", "w+");
    fpoy = fopen("./outputy.csv", "w+");
    fptox = fopen("./output_theox.csv", "w+");
    fptoy = fopen("./output_theoy.csv", "w+");
    for (int i = 0 ; i < out_r - 2 ; i++) {
        for (int j = 0 ; j < out_c ; j ++) {
            if (j != (out_c - 1)) {
                sprintf(out_str, "%d, ", (int)resultx[i*out_c + j]);
                fputs(out_str, fpox);
                sprintf(out_str, "%d, ", (int)resulty[i*out_c + j]);
                fputs(out_str, fpoy);
                sprintf(out_str, "%d, ", (int)theo_resultx[i*out_c + j]);
                fputs(out_str, fptox);
                sprintf(out_str, "%d, ", (int)theo_resulty[i*out_c + j]);
                fputs(out_str, fptoy);
            } else {
                sprintf(out_str, "%d", (int)resultx[i*out_c + j]);
                fputs(out_str, fpox);
                sprintf(out_str, "%d", (int)resulty[i*out_c + j]);
                fputs(out_str, fpoy);
                sprintf(out_str, "%d", (int)theo_resultx[i*out_c + j]);
                fputs(out_str, fptox);
                sprintf(out_str, "%d", (int)theo_resulty[i*out_c + j]);
                fputs(out_str, fptoy);
            }
        }
        fputc('\n',fptox);
        fputc('\n',fptoy);
        fputc('\n',fpox);
        fputc('\n',fpoy);
    }
    fclose(fptox);
    fclose(fptoy);
 }
