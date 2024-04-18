#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

/*
    command to compilte:
    gcc -Wall -std=c99 -fopenmp -o strassenMatrixMulti matrix_multiplication.c
    command to execute:
    ./strassenMatrixMulti [matrix_1] [matrix_2] [number of threads]
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

double task_total = 0.0;
int task_count = 0;
double start_time, end_time, total_time;
void parallel_strassenMultiply(int **A, int **B, int **C, int n) {
    // Base case size, using direct multiplication for small matrices
    if (n <= 32) {  
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

    start_time = omp_get_wtime();
    // Divide matrices into quarters and call parallel_strassenMultiply recursively
    #pragma omp parallel sections
    {
        #pragma omp section
        { 
            double local_start = omp_get_wtime();
            addMatrix(A11, A22, tempA, new_size); addMatrix(B11, B22, tempB, new_size); parallel_strassenMultiply(tempA, tempB, M1, new_size); 
            double local_end = omp_get_wtime();
            task_total += local_end - local_start;
            #pragma omp atomic
            task_count++;
        }

        #pragma omp section
        { 
            double local_start = omp_get_wtime();
            addMatrix(A21, A22, tempA, new_size); parallel_strassenMultiply(tempA, B11, M2, new_size); 
            double local_end = omp_get_wtime();
            task_total += local_end - local_start;
            #pragma omp atomic
            task_count++;
        }
        
        #pragma omp section
        { 
            double local_start = omp_get_wtime();
            subtractMatrix(B12, B22, tempB, new_size); parallel_strassenMultiply(A11, tempB, M3, new_size); 
            double local_end = omp_get_wtime();
            task_total += local_end - local_start;
            #pragma omp atomic
            task_count++;
        }


        #pragma omp section
        { 
            double local_start = omp_get_wtime();
            subtractMatrix(B21, B11, tempB, new_size); parallel_strassenMultiply(A22, tempB, M4, new_size); 
            double local_end = omp_get_wtime();
            task_total += local_end - local_start;
            #pragma omp atomic
            task_count++;
        }


        #pragma omp section
        { 
            double local_start = omp_get_wtime();
            addMatrix(A11, A12, tempA, new_size); parallel_strassenMultiply(tempA, B22, M5, new_size); 
            double local_end = omp_get_wtime();
            task_total += local_end - local_start;
            #pragma omp atomic
            task_count++;
        }

        #pragma omp section
        { 
            double local_start = omp_get_wtime();
            subtractMatrix(A21, A11, tempA, new_size); addMatrix(B11, B12, tempB, new_size); parallel_strassenMultiply(tempA, tempB, M6, new_size); 
            double local_end = omp_get_wtime();
            task_total += local_end - local_start;
            #pragma omp atomic
            task_count++;
        }


        #pragma omp section
        { 
            double local_start = omp_get_wtime();
            subtractMatrix(A12, A22, tempA, new_size); addMatrix(B21, B22, tempB, new_size); parallel_strassenMultiply(tempA, tempB, M7, new_size); 
            double local_end = omp_get_wtime();
            task_total += local_end - local_start;
            #pragma omp atomic
            task_count++;
        }
    }
    end_time = omp_get_wtime(); // Capture end time after parallel section
    total_time = end_time - start_time;

    // Combine results into the final matrix C
    start_time = omp_get_wtime();
    #pragma omp parallel sections
    {
        #pragma omp section
        { 
            double local_start = omp_get_wtime();
            addMatrix(M1, M4, tempA, new_size); subtractMatrix(tempA, M5, tempB, new_size); addMatrix(tempB, M7, C11, new_size); 
            double local_end = omp_get_wtime();
            task_total += local_end - local_start;
            #pragma omp atomic
            task_count++;
        }

        #pragma omp section
        { 
            double local_start = omp_get_wtime();
            addMatrix(M3, M5, C12, new_size); 
            double local_end = omp_get_wtime();
            task_total += local_end - local_start;
            #pragma omp atomic
            task_count++;
        }
        
        #pragma omp section
        { 
            double local_start = omp_get_wtime();
            addMatrix(M2, M4, C21, new_size); 
            double local_end = omp_get_wtime();
            task_total += local_end - local_start;
            #pragma omp atomic
            task_count++;
        }

        #pragma omp section
        { 
            double local_start = omp_get_wtime();
            addMatrix(M1, M3, tempA, new_size); subtractMatrix(tempA, M2, tempB, new_size); addMatrix(tempB, M6, C22, new_size); 
            double local_end = omp_get_wtime();
            task_total += local_end - local_start;
            #pragma omp atomic
            task_count++;
        }
    }
    end_time = omp_get_wtime(); // Capture end time after parallel section
    total_time += end_time - start_time;

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

void strassenMultiply(int **A, int **B, int **C, int n) {
    // Base case size, using direct multiplication for small matrices
    if (n <= 32) {  
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

    addMatrix(A11, A22, tempA, new_size); addMatrix(B11, B22, tempB, new_size); strassenMultiply(tempA, tempB, M1, new_size);
    addMatrix(A21, A22, tempA, new_size); strassenMultiply(tempA, B11, M2, new_size);
    subtractMatrix(B12, B22, tempB, new_size); strassenMultiply(A11, tempB, M3, new_size);
    subtractMatrix(B21, B11, tempB, new_size); strassenMultiply(A22, tempB, M4, new_size);
    addMatrix(A11, A12, tempA, new_size); strassenMultiply(tempA, B22, M5, new_size);
    subtractMatrix(A21, A11, tempA, new_size); addMatrix(B11, B12, tempB, new_size); strassenMultiply(tempA, tempB, M6, new_size);
    subtractMatrix(A12, A22, tempA, new_size); addMatrix(B21, B22, tempB, new_size); strassenMultiply(tempA, tempB, M7, new_size);

    addMatrix(M1, M4, tempA, new_size); subtractMatrix(tempA, M5, tempB, new_size); addMatrix(tempB, M7, C11, new_size);
    addMatrix(M3, M5, C12, new_size);
    addMatrix(M2, M4, C21, new_size);
    addMatrix(M1, M3, tempA, new_size); subtractMatrix(tempA, M2, tempB, new_size); addMatrix(tempB, M6, C22, new_size);

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

int** deepCopy2DArray(int** original, int rows, int cols) {
    // Allocate memory for the new 2D array
    int** copy = (int**)malloc(rows * sizeof(int*));
    if (copy == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    
    // Copy each row
    for (int i = 0; i < rows; i++) {
        copy[i] = (int*)malloc(cols * sizeof(int));
        if (copy[i] == NULL) {
            perror("Memory allocation failed");
            // Cleanup previously allocated rows
            for (int j = 0; j < i; j++) {
                free(copy[j]);
            }
            free(copy);
            exit(EXIT_FAILURE);
        }
        // Copy each element
        for (int j = 0; j < cols; j++) {
            copy[i][j] = original[i][j];
        }
    }
    
    return copy;
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

    int **copy_A = deepCopy2DArray(A, sizeA, sizeA);
    int **copy_B = deepCopy2DArray(B, sizeB, sizeB);
    int **copy_C = deepCopy2DArray(C, sizeA, sizeA);

    int num_threads = atoi(argv[3]);
    omp_set_num_threads(num_threads);
    omp_set_dynamic(0);

    double start_time = omp_get_wtime();
    parallel_strassenMultiply(A, B, C, sizeA);
    double end_time = omp_get_wtime();
    double parallel_time = end_time - start_time;

    printf("Time taken to multiply two %dx%d matrices with %d threads: %f seconds\n", sizeA, sizeA, num_threads, parallel_time);

    // After multiplication, print the result
    // printf("Resultant Matrix C after multiplication:\n");
    // printMatrix(C, sizeA);

    freeMatrix(A, sizeA);
    freeMatrix(B, sizeA);
    freeMatrix(C, sizeA);

    if (num_threads == 1){
        double start = omp_get_wtime();
        strassenMultiply(copy_A, copy_B, copy_C, sizeA);
        double end = omp_get_wtime();
        double work_time = end - start;
        printf("Work Time: %f seconds\n", work_time);
    } 
    // double total_time = end_time - start_time;
    // double avg_task_time = task_total / task_count;
    // double communication = total_time - avg_task_time;
    // printf("Task time: %f, Task Count: %d\n", task_total, task_count);
    // printf("Total time of parallel: %f seconds\n", total_time);
    // printf("Task time: %f seconds\n", avg_task_time);
    // printf("Communication: %f seconds\n", communication);
    

    return 0;
}
