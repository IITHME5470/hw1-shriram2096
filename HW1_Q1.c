#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void write_to_file(int n, double **A, int format_flag) {
    FILE *fptr;
    char filename[50];

    snprintf(filename, sizeof(filename), "array_%06d_%s.out", n, format_flag ? "bin" : "asc");
    if (format_flag) {
        
        fptr = fopen(filename, "wb");
        if (fptr == NULL) {
            perror("Error opening binary file");
            return;
        }
    
        for (int i = 0; i < n; i++) {
            if (fwrite(A[i], sizeof(double), n, fptr) != n) {
                perror("Error writing to binary file");
                fclose(fptr);
                return;
            }
        }
        fclose(fptr);

    } else {
    
        fptr = fopen(filename, "w");
        if (fptr == NULL) {
            perror("Error opening ASCII file");
            return;
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                fprintf(fptr, "%.15f ", A[i][j]);
            }
            fprintf(fptr, "\n");
        }
        fclose(fptr);
    }
}

int main() {
 
    FILE *input_file = fopen("input.in", "r");
    if (input_file == NULL) {
        perror("Error opening input.in");
        return 1;
    }

    int n;
    fscanf(input_file, "%d", &n);
    fclose(input_file);

   
    double **A = (double **)malloc(n * sizeof(double *));
    if (A == NULL) {
        perror("Memory allocation failed for A");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        A[i] = (double *)malloc(n * sizeof(double));
        if (A[i] == NULL) {
            perror("Memory allocation failed for A[i]");
            for (int j = 0; j < i; j++) {
                free(A[j]);
            }
            free(A);
            return 1;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = (double)(i + j);
        }
    }

    write_to_file(n, A, 1); // Binary format
    write_to_file(n, A, 0); // ASCII format

 
    for (int i = 0; i < n; i++) {
        free(A[i]);
    }
    free(A);

    return 0;
}