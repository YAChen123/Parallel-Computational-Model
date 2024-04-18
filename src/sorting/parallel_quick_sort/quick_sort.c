#include <stdio.h>
#include <stdlib.h>
#include "quick_sort.h"
#include <time.h>
#include <omp.h>
#include <string.h>

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

double task_start, task_end, task_total, p_total, p_count = 0;
int task_count, max_depth = 0;
void parallel_quickSort(int *arr, int low, int high, int depth) {

    #pragma omp atomic write
    max_depth = depth > max_depth ? depth : max_depth;

    if (low < high) {
        double p_start = omp_get_wtime();
        int pi = partition(arr, low, high);
        double p_end = omp_get_wtime();
        #pragma omp atomic
        p_total += p_end - p_start;
        #pragma omp atomic
        p_count++;

        
        task_start = omp_get_wtime();
        #pragma omp task
        {
            parallel_quickSort(arr, low, pi - 1, depth+1);
        }
        task_end = omp_get_wtime();

        task_total += task_end - task_start;
        #pragma omp atomic
        task_count++;

        task_start = omp_get_wtime();
        #pragma omp task
        {
            parallel_quickSort(arr, pi + 1, high, depth+1);
        }
        task_end = omp_get_wtime();

        task_total += task_end - task_start;
        #pragma omp atomic
        task_count++;
    }
}

int cnt = 0;
double p_time_used = 0;
void quickSort(int *arr, int low, int high) {
    if (low < high) {
        double p_start = omp_get_wtime(); // Start partition timing
        int pi = partition(arr, low, high);
        double p_end = omp_get_wtime(); 
        p_time_used += p_end - p_start;
        cnt++;
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Function to print an array
void printArray(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int *intdup(int const *src, size_t len)
{
   int *p = malloc(len * sizeof(int));
   memcpy(p, src, len * sizeof(int));
   return p;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <num_of_threads>\n", argv[0]);
        return -1;
    }

    const char *input_filename = argv[1];
    FILE *file = fopen(input_filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    int num_threads = atoi(argv[2]);  
    if (num_threads < 1) {
        fprintf(stderr, "Number of threads must be at least 1\n");
        return 1;
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


    int *copy_arr = intdup(arr, n);
    omp_set_nested(1);
    omp_set_dynamic(0);
    omp_set_num_threads(num_threads);  
    double start_time = omp_get_wtime();  // Start time measurement
    #pragma omp parallel
    {
        #pragma omp single
        {
            parallel_quickSort(copy_arr, 0, n - 1, 1);
        }
    }
    double end_time = omp_get_wtime();    // End time measurement
    double parallel_time = end_time - start_time;
    printf("Sorted array (first 10 elements): \n");
    printArray(copy_arr, 10);  // Print first 10 elements for brevity
    printf("Time taken: %f seconds with %d threads\n", parallel_time, num_threads);

    if (num_threads == 1){
        double start = omp_get_wtime();
        quickSort(arr, 0, n - 1);
        double end = omp_get_wtime();
        double work_time = end - start;
        printf("Work Time: %f seconds\n", work_time);
        // printf("Computation count: %d\n", cnt);
        // printf("Computation Time: %f seconds\n", p_time_used);
    }
    
    // printf("Task time: %f seconds\n", task_total/task_count);
    // printf("Depth of recursion: %d\n", max_depth);
    // printf("P_time: %f, P_count: %f\n", p_total, p_count);
    // printf("Cost: %f seconds\n", total_latency/task_count);
    free(arr);
    free(copy_arr);

    return 0;
}