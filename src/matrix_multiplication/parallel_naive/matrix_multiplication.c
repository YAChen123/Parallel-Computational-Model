#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

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

void multiplyMatrices(int **A, int **B, int **C, int size) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            C[i][j] = 0;
            for (int k = 0; k < size; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// void printMatrix(int **matrix, int size) {
//     if (matrix == NULL) {
//         printf("Matrix is NULL\n");
//         return;
//     }

//     printf("Matrix (%dx%d):\n", size, size);
//     for (int i = 0; i < size; i++) {
//         for (int j = 0; j < size; j++) {
//             printf("%4d ", matrix[i][j]);  // "%4d" provides a consistent column width of 4 characters
//         }
//         printf("\n");  // Newline at the end of each row
//     }
// }

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
    multiplyMatrices(A, B, C, sizeA);
    double end_time = omp_get_wtime();

    // After multiplication, print the result
    //printf("Resultant Matrix C after multiplication:\n");
    //printMatrix(C, sizeA);

    printf("Time taken to multiply two %dx%d matrices with %d threads: %f seconds\n", sizeA, sizeA, num_threads, end_time - start_time);

    freeMatrix(A, sizeA);
    freeMatrix(B, sizeA);
    freeMatrix(C, sizeA);

    return 0;
}
