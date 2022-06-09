#include <stdio.h>
#include <stdlib.h>

int r = 0, c = 0, i = 0;

// void print_matrix(float (*D_new)[rows], rows, cols);
// void print_matrix(float (*D_new)[rows], rows, cols){
void print_matrix(float** D_new, int rows, int cols);
void print_matrix(float** D_new, int rows, int cols) {
    for (r = 0; r < rows; r++) {
        for (c = 0; c < cols; c++) {
            printf("D[%d][%d]=%f\n", r, c, D_new[r][c]);
        }
    }
}

int main() {
    int rows = 3, cols = 4;
    FILE* fp;
    char* out_str = malloc(sizeof(float) * 20);
    float* D = (float*)malloc(sizeof(float) * rows * cols);
    // float (*D_new)[rows] = malloc(sizeof(*D_new)*cols);
    float** D_new = malloc(cols * sizeof(float*));
    for (i = 0; i < cols; i++) { D_new[i] = malloc(rows * sizeof(float)); }
    for (i = 0; i < (rows * cols); i++) { D[i] = i; }
    fp = fopen("./dummy_op.csv", "w+");
    for (r = 0; r < rows; r++) {
        for (c = 0; c < cols; c++) {
            sprintf(out_str, "%f, ", D[r * cols + c]);
            fputs(out_str, fp);
            D_new[r][c] = D[r * cols + c];
        }
        fputs("\n", fp);
    }
    fclose(fp);
    print_matrix(D_new, rows, cols);

    return 0;
}
