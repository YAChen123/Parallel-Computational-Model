#include <stdio.h>
#include <stdlib.h>
#include "quick_sort.h"
#include <time.h>
#include <omp.h>
#include <math.h>

/*
    command to compilte:
    gcc -Wall -std=c99 -fopenmp -o quicksort quick_sort.c
    command to execute:
    ./quicksort [input] [number of threads]
*/

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int *arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quickSort(int *arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        
        #pragma omp task
        {
            quickSort(arr, low, pi - 1);
        }
        #pragma omp task
        {
            quickSort(arr, pi + 1, high);
        }
    }
}

// Function to print an array
void printArray(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <num_of_threads>\n", argv[0]);
        return -1;
    }

    const char *input_filename = argv[1];
    int num_threads = atoi(argv[2]);  
    if (num_threads < 1) {
        fprintf(stderr, "Number of threads must be at least 1\n");
        return 1;
    }

    FILE *file = fopen(input_filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    // Determine the number of integers in the file
    int n = 0;
    int temp;
    while (fscanf(file, "%d", &temp) == 1) n++;

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
        fscanf(file, "%d", &arr[i]);
    }
    fclose(file);

    printf("Original array (first 10 elements): \n");
    printArray(arr, 10);  // Print first 10 elements

    omp_set_nested(1);
    omp_set_dynamic(0);
    omp_set_num_threads(num_threads);  

    double start_time = omp_get_wtime();  // Start time measurement
    #pragma omp parallel
    {
        #pragma omp single
        {
            quickSort(arr, 0, n - 1);
        }
    }
    double end_time = omp_get_wtime();    // End time measurement

    printf("Sorted array (first 10 elements): \n");
    printArray(arr, 10);  // Print first 10 elements for brevity

    printf("Time taken: %f seconds with %d threads\n", end_time - start_time, num_threads);

    free(arr);
    return 0;
}