/*
 * Activity 3: DistributionCountingSort
 *
 * Works for integer keys in a known range [lo, hi].
 * Steps:
 *   1. Count frequency of each value  -> freq[]
 *   2. Compute prefix sums            -> dist[] (distribution / cumulative)
 *   3. Fill output right-to-left for stability
 *
 * Time complexity : O(n + k)  where k = hi - lo + 1
 * Space complexity: O(n + k)
 *
 * Compare with ComparisonCountingSort (Activity 2):
 *   - ComparisonCountingSort: O(n^2) time, O(n) space, any comparable data
 *   - DistributionCountingSort: O(n+k) time, O(n+k) space, integer keys only
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* ------------------------------------------------------------------
 * DistributionCountingSort
 * A[]  : input array of integers in range [lo, hi]
 * S[]  : output sorted array (pre-allocated, size n)
 * n    : number of elements
 * lo   : minimum possible value
 * hi   : maximum possible value
 * ------------------------------------------------------------------ */
void DistributionCountingSort(const int A[], int S[], int n, int lo, int hi) {
    int k = hi - lo + 1;

    /* Step 1 – frequency table */
    int *freq = (int *)calloc(k, sizeof(int));
    if (!freq) { fprintf(stderr, "malloc failed\n"); exit(1); }

    for (int i = 0; i < n; i++) freq[A[i] - lo]++;

    /* Step 2 – prefix sums (distribution) */
    int *dist = (int *)malloc(k * sizeof(int));
    if (!dist) { fprintf(stderr, "malloc failed\n"); exit(1); }
    dist[0] = freq[0];
    for (int j = 1; j < k; j++) dist[j] = dist[j - 1] + freq[j];

    /* Step 3 – place elements (right-to-left for stability) */
    for (int i = n - 1; i >= 0; i--) {
        S[--dist[A[i] - lo]] = A[i];
    }

    free(freq);
    free(dist);
}

/* ------------------------------------------------------------------
 * ComparisonCountingSort (from Activity 2, for comparison)
 * ------------------------------------------------------------------ */
void ComparisonCountingSort(const int A[], int S[], int n) {
    int *count = (int *)calloc(n, sizeof(int));
    for (int i = 1; i < n; i++)
        for (int j = 0; j < i; j++)
            if (A[j] <= A[i]) count[i]++;
            else count[j]++;
    for (int i = 0; i < n; i++) S[count[i]] = A[i];
    free(count);
}

/* ---- helpers ---- */
void print_array(const char *label, const int arr[], int n) {
    printf("%s [ ", label);
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("]\n");
}

int find_min(const int A[], int n) {
    int m = A[0]; for (int i=1;i<n;i++) if(A[i]<m) m=A[i]; return m;
}
int find_max(const int A[], int n) {
    int m = A[0]; for (int i=1;i<n;i++) if(A[i]>m) m=A[i]; return m;
}

int main(void) {
    printf("=== DistributionCountingSort ===\n\n");

    /* -- Test 1 -- */
    int A1[] = {13, 11, 12, 13, 12, 12};
    int n1 = sizeof(A1)/sizeof(A1[0]);
    int S1[6];
    print_array("Input :", A1, n1);
    DistributionCountingSort(A1, S1, n1, find_min(A1,n1), find_max(A1,n1));
    print_array("Sorted:", S1, n1);
    printf("\n");

    /* -- Test 2 -- */
    int A2[] = {5, 1, 3, 2, 4, 0, 5, 1};
    int n2 = sizeof(A2)/sizeof(A2[0]);
    int S2[8];
    print_array("Input :", A2, n2);
    DistributionCountingSort(A2, S2, n2, 0, 5);
    print_array("Sorted:", S2, n2);
    printf("\n");

    /* -- Performance comparison -- */
    srand((unsigned)time(NULL));
    int sizes[] = {1000, 3000, 5000};
    int range_lo = 0, range_hi = 9999;

    printf("%-8s | %-20s | %-20s\n", "n", "ComparisonCounting(s)", "DistributionCounting(s)");
    printf("---------|----------------------|----------------------\n");

    for (int t = 0; t < 3; t++) {
        int n = sizes[t];
        int *A = malloc(n * sizeof(int));
        int *S = malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) A[i] = rand() % (range_hi - range_lo + 1) + range_lo;

        clock_t c1 = clock();
        ComparisonCountingSort(A, S, n);
        clock_t c2 = clock();
        double t_cmp = (double)(c2 - c1) / CLOCKS_PER_SEC;

        /* restore original (ComparisonCounting doesn't modify A) */
        clock_t c3 = clock();
        DistributionCountingSort(A, S, n, range_lo, range_hi);
        clock_t c4 = clock();
        double t_dist = (double)(c4 - c3) / CLOCKS_PER_SEC;

        printf("%-8d | %-20.6f | %-20.6f\n", n, t_cmp, t_dist);

        free(A); free(S);
    }

    printf("\n--- Complexity Summary ---\n");
    printf("ComparisonCountingSort : O(n^2) comparisons, O(n) extra space\n");
    printf("DistributionCountingSort: O(n+k) time,       O(n+k) extra space\n");
    printf("(k = range of values)\n\n");
    printf("Differences:\n");
    printf("  1. Time: DistributionCounting is vastly faster for large n.\n");
    printf("  2. Input: Distribution requires integer keys in a bounded range.\n");
    printf("  3. Stability: DistributionCounting is stable; ComparisonCounting\n");
    printf("     as implemented here may not handle duplicates correctly.\n");
    printf("  4. Space: Distribution needs O(k) extra for the frequency table.\n");

    return 0;
}
