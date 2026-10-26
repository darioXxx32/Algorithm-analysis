/*
 * Activity 2: ComparisonCountingSort
 *
 * Algorithm (Levitin, "Introduction to the Design & Analysis of Algorithms"):
 *   For each pair (i, j):
 *     - if A[i] > A[j]  => count[i]++
 *     - if A[i] == A[j] and i > j => count[i]++  (tie-breaking by index)
 * This correctly handles duplicates and is stable.
 * Time complexity: O(n^2)  |  Space complexity: O(n)
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ------------------------------------------------------------------
 * ComparisonCountingSort
 * Input : A[]  – array of n integers (not modified)
 * Output: S[]  – sorted output array (must be pre-allocated, size n)
 * ------------------------------------------------------------------ */
void ComparisonCountingSort(const int A[], int S[], int n) {
    int *count = (int *)calloc(n, sizeof(int));  /* count[i] = final position of A[i] */
    if (!count) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    /* Standard Levitin two-pass formulation */
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (A[j] <= A[i])
                count[i]++;
            else
                count[j]++;
        }
    }

    /* Place each element in its correct position */
    for (int i = 0; i < n; i++) {
        S[count[i]] = A[i];
    }

    free(count);
}

/* ---- helpers ---- */
void print_array(const char *label, const int arr[], int n) {
    printf("%s [ ", label);
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("]\n");
}

int main(void) {
    printf("=== ComparisonCountingSort ===\n\n");

    /* -- Test 1: small hand-crafted array -- */
    int A1[] = {62, 31, 84, 96, 19, 47};
    int n1 = sizeof(A1) / sizeof(A1[0]);
    int S1[6];

    print_array("Input :", A1, n1);
    ComparisonCountingSort(A1, S1, n1);
    print_array("Sorted:", S1, n1);
    printf("\n");

    /* -- Test 2: already sorted -- */
    int A2[] = {1, 2, 3, 4, 5};
    int n2 = sizeof(A2) / sizeof(A2[0]);
    int S2[5];

    print_array("Input :", A2, n2);
    ComparisonCountingSort(A2, S2, n2);
    print_array("Sorted:", S2, n2);
    printf("\n");

    /* -- Test 3: reverse sorted -- */
    int A3[] = {9, 7, 5, 3, 1};
    int n3 = sizeof(A3) / sizeof(A3[0]);
    int S3[5];

    print_array("Input :", A3, n3);
    ComparisonCountingSort(A3, S3, n3);
    print_array("Sorted:", S3, n3);
    printf("\n");

    /* -- Test 4: random large array with timing -- */
    srand((unsigned)time(NULL));
    int n4 = 5000;
    int *A4 = (int *)malloc(n4 * sizeof(int));
    int *S4 = (int *)malloc(n4 * sizeof(int));
    for (int i = 0; i < n4; i++) A4[i] = rand() % 10000;

    clock_t start = clock();
    ComparisonCountingSort(A4, S4, n4);
    clock_t end = clock();

    printf("n = %d random elements sorted in %.4f seconds\n",
           n4, (double)(end - start) / CLOCKS_PER_SEC);

    /* Verify sorted order */
    int ok = 1;
    for (int i = 1; i < n4; i++) {
        if (S4[i] < S4[i - 1]) { ok = 0; break; }
    }
    printf("Verification: %s\n\n", ok ? "PASSED" : "FAILED");

    free(A4);
    free(S4);

    printf("Algorithm complexity: O(n^2) comparisons, O(n) extra space\n");
    return 0;
}
