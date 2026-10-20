#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <windows.h>

/* Global counters for detailed empirical operation profiling */
static unsigned long long g_comparisons = 0;
static unsigned long long g_swaps = 0;

static inline void swap(int *a, int *b) {
    g_swaps++;
    int t = *a;
    *a = *b;
    *b = t;
}

/* Standard Lomuto Partition */
int partition_lomuto(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        g_comparisons++;
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

/* Recursive Quick Sort using Lomuto partition */
void quick_sort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition_lomuto(arr, low, high);
        quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, high);
    }
}

/* Helper to check if an array is correctly sorted in non-decreasing order */
bool is_sorted(const int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) return false;
    }
    return true;
}

/* Generate array with uniform random integers */
void generate_random_array(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand();
    }
}

/* High-resolution timer using Windows QueryPerformanceCounter */
double get_time_sec(void) {
    LARGE_INTEGER freq, counter;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart / (double)freq.QuadPart;
}

int main(void) {
    srand(42);

    printf("========================================\n");
    printf("  TASK 4: Quick Sort Implementation\n");
    printf("========================================\n");

    /* Small test demo */
    int sample[] = {24, 9, 29, 14, 19, 27, 1, 10, 33, 5};
    int n_sample = sizeof(sample) / sizeof(sample[0]);

    printf("Original sample array: ");
    for (int i = 0; i < n_sample; i++) printf("%d ", sample[i]);
    printf("\n");

    quick_sort(sample, 0, n_sample - 1);

    printf("Sorted sample array:   ");
    for (int i = 0; i < n_sample; i++) printf("%d ", sample[i]);
    printf("\nIs sorted: %s\n\n", is_sorted(sample, n_sample) ? "TRUE" : "FALSE");

    /* Random list test */
    int test_n = 50;
    int *rand_test = (int *)malloc(test_n * sizeof(int));
    generate_random_array(rand_test, test_n);
    printf("Testing on 50-element random list...\n");
    quick_sort(rand_test, 0, test_n - 1);
    printf("Validation result: %s\n\n", is_sorted(rand_test, test_n) ? "PASSED" : "FAILED");
    free(rand_test);

    printf("========================================\n");
    printf("  TASK 5: Empirical Analysis on Quick Sort\n");
    printf("========================================\n");

    int sizes[] = {5000, 10000, 25000, 50000, 75000, 100000, 150000, 200000, 300000, 500000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    int trials = 7;

    FILE *csv = fopen("quicksort_empirical_results.csv", "w");
    if (!csv) {
        perror("Failed to open quicksort_empirical_results.csv");
        return 1;
    }
    fprintf(csv, "Size_N,Avg_Time_ms,StdDev_ms,Avg_Comparisons,Avg_Swaps\n");

    printf("%-10s | %-14s | %-14s | %-16s | %-14s\n",
           "Size (N)", "Avg Time (ms)", "Std Dev (ms)", "Avg Comparisons", "Avg Swaps");
    printf("--------------------------------------------------------------------------------\n");

    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];
        double times[15];
        double sum_time = 0.0;
        unsigned long long sum_comps = 0;
        unsigned long long sum_swaps = 0;

        int *arr = (int *)malloc(n * sizeof(int));

        for (int t = 0; t < trials; t++) {
            generate_random_array(arr, n);
            g_comparisons = 0;
            g_swaps = 0;

            double t_start = get_time_sec();
            quick_sort(arr, 0, n - 1);
            double t_end = get_time_sec();

            double elapsed_ms = (t_end - t_start) * 1000.0;
            times[t] = elapsed_ms;
            sum_time += elapsed_ms;
            sum_comps += g_comparisons;
            sum_swaps += g_swaps;

            if (!is_sorted(arr, n)) {
                fprintf(stderr, "Error: array not sorted at size %d trial %d\n", n, t);
            }
        }

        double avg_time = sum_time / trials;
        double variance = 0.0;
        for (int t = 0; t < trials; t++) {
            variance += (times[t] - avg_time) * (times[t] - avg_time);
        }
        double std_dev = (trials > 1) ? sqrt(variance / (trials - 1)) : 0.0;
        double avg_comps = (double)sum_comps / trials;
        double avg_swaps_d = (double)sum_swaps / trials;

        printf("%-10d | %-14.4f | %-14.4f | %-16.1f | %-14.1f\n",
               n, avg_time, std_dev, avg_comps, avg_swaps_d);

        fprintf(csv, "%d,%.6f,%.6f,%.1f,%.1f\n",
                n, avg_time, std_dev, avg_comps, avg_swaps_d);

        free(arr);
    }

    fclose(csv);
    printf("--------------------------------------------------------------------------------\n");
    printf("Data successfully exported to 'quicksort_empirical_results.csv'.\n");

    return 0;
}
