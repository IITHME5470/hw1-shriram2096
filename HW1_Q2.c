#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TOLERANCE 1e-12   // Tolerance value

void load_matrix(const char *file_name, int size, double **matrix);
void load_vector(const char *file_name, int size, double *vector);
int check_eigenvector(int size, double **matrix, double *vector, double *eigenvalue);
void save_eigenvalue(const char *file_name, double eigenvalue);
void process_eigenvectors(int size, double **matrix, const char *vector_file_pattern);

int main() {
    FILE *input_file = fopen("input.in", "r");   // Input file has the value of n
    if (input_file == NULL) {
        perror("Error opening the input file");
        return 1;
    }

    int n;
    fscanf(input_file, "%d", &n);
    fclose(input_file);

    // printf("Value of n: %d\n", n);

    double **matrix = (double **)malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        matrix[i] = (double *)malloc(n * sizeof(double));
    }

    char matrix_file_name[100];
    snprintf(matrix_file_name, sizeof(matrix_file_name), "inputfiles/mat_%06d.in", n);

    // printf("Matrix file to be read: %s\n", matrix_file_name);

    load_matrix(matrix_file_name, n, matrix);

    // printf("Vector file pattern: inputfiles/vec_%06d_{vecnum}.in\n", n);

    process_eigenvectors(n, matrix, "inputfiles/vec_%06d_%06d.in");

    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}

void load_matrix(const char *file_name, int size, double **matrix) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        perror("Error opening the matrix file");
        exit(1);
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fscanf(file, "%lf,", &matrix[i][j]);
        }
    }

    fclose(file);
}

void load_vector(const char *file_name, int size, double *vector) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        perror("Error opening the vector file");
        exit(1);
    }

    for (int i = 0; i < size; i++) {
        fscanf(file, "%lf,", &vector[i]);
    }

    fclose(file);
}

int check_eigenvector(int size, double **matrix, double *vector, double *eigenvalue) {
    int non_zero_found = 0;
    for (int i = 0; i < size; i++) {
        if (vector[i] != 0) {     //check for 0 vector
            non_zero_found = 1;
            break;
        }
    }
    if (!non_zero_found) {
        return 0; 
    }

    double *result = (double *)malloc(size * sizeof(double));

    for (int i = 0; i < size; i++) {
        result[i] = 0;
        for (int j = 0; j < size; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }

    int first_valid = 0;
    for (int i = 0; i < size; i++) {
        if (fabs(vector[i]) > TOLERANCE) {
            *eigenvalue = result[i] / vector[i];
            first_valid = 1;
            break;
        }
    }

    if (!first_valid) {
        free(result);
        return 0;
    }

    for (int i = 0; i < size; i++) {
        if (fabs(vector[i]) > TOLERANCE) {
            double current_eigenvalue = result[i] / vector[i];
            if (fabs(current_eigenvalue - *eigenvalue) > TOLERANCE) {
                free(result);
                return 0;
            }
        }
    }

    free(result);
    return 1;
}

void save_eigenvalue(const char *file_name, double eigenvalue) {
    FILE *file = fopen(file_name, "a");
    if (file == NULL) {
        perror("Error opening file ");
        exit(1);
    }

    fprintf(file, "\nEigenvalue: %.15lf", eigenvalue);
    fclose(file);
}

void process_eigenvectors(int size, double **matrix, const char *vector_file_pattern) {
    char vector_file_name[100];
    double *vector = (double *)malloc(size * sizeof(double));

    for (int vec_index = 1;; vec_index++) {
        snprintf(vector_file_name, sizeof(vector_file_name), vector_file_pattern, size, vec_index);

        FILE *file = fopen(vector_file_name, "r");
        if (file == NULL) {
            break;
        }
        printf("Processing file: %s\n", vector_file_name);
        fclose(file);

        load_vector(vector_file_name, size, vector);

        double eigenvalue;
        if (check_eigenvector(size, matrix, vector, &eigenvalue)) {
            printf("%s : Yes : %.15e\n", vector_file_name, eigenvalue);
            save_eigenvalue(vector_file_name, eigenvalue);
        } else {
            printf("%s : Not an eigenvector\n", vector_file_name);
        }
    }

    free(vector);
}
