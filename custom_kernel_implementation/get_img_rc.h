#include <stdio.h>

void get_rows_n_columns();

int in_c, in_r, out_c;

void get_rows_n_columns() {
    char* token1;
    char *line = NULL;
    size_t len_size = 0;
    int line_rows = -1;
    int line_colss = 0;
    int pixel1;
    FILE *fp1 = fopen("./test.csv", "r");
    
    //Parse pixel1 values from text.csv file
    while (getline(&line, &len_size, fp1) != -1) {
        token1 = strtok(line, ",");
        //counter++;
        //printf("I am here %d times\n", counter); 
        while (token1 != NULL) {
            pixel1 = atoi(token1);
            line_colss++;
            token1 = strtok(NULL, ",");
    
        }
        line_rows ++;
    }
    in_r = line_rows;
    in_c = (line_colss-1)/line_rows;
    out_c = (line_colss-1)/line_rows;
    fclose(fp1);
    printf("[dummy]: rows = %d; cols =%d, out_c = %d\n", in_r, in_c,out_c);
}

