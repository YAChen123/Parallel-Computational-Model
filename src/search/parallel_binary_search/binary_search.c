#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

// Function to perform binary search in parallel
int parallelBinarySearch(int *arr, int n, int target) {
    int result = -1; // Index of the target element if found, otherwise -1

    #pragma omp parallel
    {
        int thread_start, thread_end, mid;
        #pragma omp single
        {
            int num_threads = omp_get_num_threads();
            thread_start = omp_get_thread_num() * (n / num_threads);
            thread_end = thread_start + (n / num_threads) - 1;
            if (omp_get_thread_num() == num_threads - 1) {
                thread_end = n-1; // last thread handles any remaining elements
            }
        }
        
        while (thread_start <= thread_end && result == -1) {
            mid = thread_start + (thread_end - thread_start) / 2;
            if (arr[mid] == target) {
                #pragma omp critical
                {
                    result = mid; // Set result to the index of the target
                }
            } else if (arr[mid] < target) {
                thread_start = mid + 1;
            } else {
                thread_end = mid - 1;
            }
        }
    }
    return result;
}

// Function to print an array
void printArray(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <input_file> <num_of_threads> <target>\n", argv[0]);
        return -1;
    }

    const char *input_filename = argv[1];
    int num_threads = atoi(argv[2]);
    if (num_threads < 1) {
        fprintf(stderr, "Number of threads must be at least 1\n");
        return 1;
    }
    int target = atoi(argv[3]);
    
    FILE *file = fopen(input_filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    // Determine the number of integers in the file
    int n = 0;
    double temp;
    while (fscanf(file, "%lf", &temp) == 1) n++;

    // Allocate memory based on the number of integers
    int *arr = malloc(n * sizeof(int));
    if (arr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        return -1;
    }

    // Reset file pointer to the beginning of the file
    rewind(file);

    // Read numbers from file into the array
    for (int i = 0; i < n; i++) {
        fscanf(file, "%lf", &temp);
        arr[i] = (int)temp;  // Convert double to int

    }
    fclose(file);

    printf("Array loaded. Array size: %d\n", n);
    

    omp_set_num_threads(num_threads);

    double start_time = omp_get_wtime();
    int index = parallelBinarySearch(arr, n, target);
    double end_time = omp_get_wtime();

    if (index != -1) {
        printf("Target %d found at index %d\n", target, index);
    } else {
        printf("Target %d not found in the array\n", target);
    }

    printf("Time taken: %f seconds with %d threads\n", end_time - start_time, num_threads);

    free(arr);
    return 0;
}
