#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

/*
    command to compilte:
    gcc -Wall -std=c99 -fopenmp -o strassenMatrixMulti matrix_multiplication.c
    command to execute:
    ./strassenMatrixMulti [input] [number of threads]
*/

int** allocateMatrix(int size) {
    int** matrix = (int**) malloc(size * sizeof(int*));
    if (matrix == NULL) {
        fprintf(stderr, "Memory allocation failed for matrix pointers\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < size; i++) {
        matrix[i] = (int*) malloc(size * sizeof(int));
        if (matrix[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for matrix row %d\n", i);
            // Free previously allocated rows
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            exit(EXIT_FAILURE);
        }
    }
    return matrix;
}

void freeMatrix(int **matrix, int size) {
    for (int i = 0; i < size; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void initializeMatrixFromFile(int **matrix, FILE *file, int size) {
    // Assume the size line has already been read, directly read the matrix data
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (fscanf(file, "%d", &matrix[i][j]) != 1) {
                fprintf(stderr, "Failed to read data for matrix at [%d][%d]\n", i, j);
                exit(EXIT_FAILURE);
            }
        }
    }
}

void addMatrix(int **A, int **B, int **result, int size) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            result[i][j] = A[i][j] + B[i][j];
        }
    }
}

void subtractMatrix(int **A, int **B, int **result, int size) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            result[i][j] = A[i][j] - B[i][j];
        }
    }
}

void multiplyMatrices(int **A, int **B, int **result, int size) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            result[i][j] = 0;
            for (int k = 0; k < size; k++) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void strassenMultiply(int **A, int **B, int **C, int n) {
    // Base case size, using direct multiplication for small matrices
    if (n <= 64) {  
        multiplyMatrices(A, B, C, n);
        return;
    }

    int new_size = n / 2;
    int** A11 = allocateMatrix(new_size);
    int** A12 = allocateMatrix(new_size);
    int** A21 = allocateMatrix(new_size);
    int** A22 = allocateMatrix(new_size);
    int** B11 = allocateMatrix(new_size);
    int** B12 = allocateMatrix(new_size);
    int** B21 = allocateMatrix(new_size);
    int** B22 = allocateMatrix(new_size);
    int** C11 = allocateMatrix(new_size);
    int** C12 = allocateMatrix(new_size);
    int** C21 = allocateMatrix(new_size);
    int** C22 = allocateMatrix(new_size);
    int** M1 = allocateMatrix(new_size);
    int** M2 = allocateMatrix(new_size);
    int** M3 = allocateMatrix(new_size);
    int** M4 = allocateMatrix(new_size);
    int** M5 = allocateMatrix(new_size);
    int** M6 = allocateMatrix(new_size);
    int** M7 = allocateMatrix(new_size);
    int** tempA = allocateMatrix(new_size);
    int** tempB = allocateMatrix(new_size);

    // Divide matrices into quarters and call strassenMultiply recursively
    #pragma omp parallel sections
    {
        #pragma omp section
        { addMatrix(A11, A22, tempA, new_size); addMatrix(B11, B22, tempB, new_size); strassenMultiply(tempA, tempB, M1, new_size); }
        #pragma omp section
        { addMatrix(A21, A22, tempA, new_size); strassenMultiply(tempA, B11, M2, new_size); }
        #pragma omp section
        { subtractMatrix(B12, B22, tempB, new_size); strassenMultiply(A11, tempB, M3, new_size); }
        #pragma omp section
        { subtractMatrix(B21, B11, tempB, new_size); strassenMultiply(A22, tempB, M4, new_size); }
        #pragma omp section
        { addMatrix(A11, A12, tempA, new_size); strassenMultiply(tempA, B22, M5, new_size); }
        #pragma omp section
        { subtractMatrix(A21, A11, tempA, new_size); addMatrix(B11, B12, tempB, new_size); strassenMultiply(tempA, tempB, M6, new_size); }
        #pragma omp section
        { subtractMatrix(A12, A22, tempA, new_size); addMatrix(B21, B22, tempB, new_size); strassenMultiply(tempA, tempB, M7, new_size); }
    }

    // Combine results into the final matrix C
    #pragma omp parallel sections
    {
        #pragma omp section
        { addMatrix(M1, M4, tempA, new_size); subtractMatrix(tempA, M5, tempB, new_size); addMatrix(tempB, M7, C11, new_size); }
        #pragma omp section
        { addMatrix(M3, M5, C12, new_size); }
        #pragma omp section
        { addMatrix(M2, M4, C21, new_size); }
        #pragma omp section
        { addMatrix(M1, M3, tempA, new_size); subtractMatrix(tempA, M2, tempB, new_size); addMatrix(tempB, M6, C22, new_size); }
    }

    // Deallocate temporary matrices
    freeMatrix(A11, new_size);
    freeMatrix(A12, new_size);
    freeMatrix(A21, new_size);
    freeMatrix(A22, new_size);
    freeMatrix(B11, new_size);
    freeMatrix(B12, new_size);
    freeMatrix(B21, new_size);
    freeMatrix(B22, new_size);
    freeMatrix(C11, new_size);
    freeMatrix(C12, new_size);
    freeMatrix(C21, new_size);
    freeMatrix(C22, new_size);
    freeMatrix(M1, new_size);
    freeMatrix(M2, new_size);
    freeMatrix(M3, new_size);
    freeMatrix(M4, new_size);
    freeMatrix(M5, new_size);
    freeMatrix(M6, new_size);
    freeMatrix(M7, new_size);
    freeMatrix(tempA, new_size);
    freeMatrix(tempB, new_size);
}


void printMatrix(int **matrix, int size) {
    if (matrix == NULL) {
        printf("Matrix is NULL\n");
        return;
    }

    printf("Matrix (%dx%d):\n", size, size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%4d ", matrix[i][j]);  // "%4d" provides a consistent column width of 4 characters
        }
        printf("\n");  // Newline at the end of each row
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <input_file_matrix_A> <input_file_matrix_B> <num_of_threads>\n", argv[0]);
        return -1;
    }

    int sizeA = 0, sizeB = 0;
    FILE *fileA = fopen(argv[1], "r");
    FILE *fileB = fopen(argv[2], "r");
    if (!fileA || !fileB) {
        fprintf(stderr, "Error opening one of the matrix files.\n");
        if (fileA) fclose(fileA);
        if (fileB) fclose(fileB);
        return -1;
    }
    
    // Read sizes directly from the open file handles
    fscanf(fileA, "%d", &sizeA);
    fscanf(fileB, "%d", &sizeB);

    if (sizeA != sizeB) {
        fprintf(stderr, "Matrix dimensions must match!\n");
        fclose(fileA);
        fclose(fileB);
        return -1;
    }

    int **A = allocateMatrix(sizeA);
    int **B = allocateMatrix(sizeB);
    int **C = allocateMatrix(sizeA);

    // Pass the already opened file handles to the initializer
    initializeMatrixFromFile(A, fileA, sizeA);
    initializeMatrixFromFile(B, fileB, sizeB);

    fclose(fileA);
    fclose(fileB);

    int num_threads = atoi(argv[3]);
    omp_set_num_threads(num_threads);

    double start_time = omp_get_wtime();
    strassenMultiply(A, B, C, sizeA);
    double end_time = omp_get_wtime();

    // After multiplication, print the result
    // printf("Resultant Matrix C after multiplication:\n");
    // printMatrix(C, sizeA);

    printf("Time taken to multiply two %dx%d matrices with %d threads: %f seconds\n", sizeA, sizeA, num_threads, end_time - start_time);

    freeMatrix(A, sizeA);
    freeMatrix(B, sizeA);
    freeMatrix(C, sizeA);

    return 0;
}
