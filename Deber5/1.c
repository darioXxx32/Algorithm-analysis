/*
 * Exercise 1 - Analysis of Algorithms Lab 05
 * Insertion Sort for alphabetic characters
 *
 * The algorithm works identically with chars as with integers because
 * chars are stored as ASCII values. 'A'=65 ... 'Z'=90 and 'a'=97 ... 'z'=122,
 * so the natural comparison operators (<, >) produce the correct
 * lexicographic ordering without any modifications.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 16

/* ------------------------------------------------------------------ */
/* Insertion Sort                                                       */
/* Sorts arr[0..n-1] in ascending (lexicographic) order.               */
/* Complexity: O(n^2) worst/average, O(n) best (already sorted).       */
/* ------------------------------------------------------------------ */
void insertionSort(char arr[], int n) {
    for (int i = 1; i < n; i++) {
        char key = arr[i];   /* element to be inserted */
        int  j   = i - 1;

        /* shift elements that are greater than key one position right */
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;    /* place key in its correct position */
    }
}

/* ------------------------------------------------------------------ */
/* Helpers                                                              */
/* ------------------------------------------------------------------ */
void printArray(const char arr[], int n, const char *label) {
    printf("%s: [ ", label);
    for (int i = 0; i < n; i++) {
        printf("'%c'", arr[i]);
        if (i < n - 1) printf(", ");
    }
    printf(" ]\n");
}

/* Count comparisons (instrumented version for analysis) */
int insertionSortCounted(char arr[], int n) {
    int comparisons = 0;
    for (int i = 1; i < n; i++) {
        char key = arr[i];
        int  j   = i - 1;
        while (j >= 0 && (++comparisons, arr[j] > key)) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
    return comparisons;
}

/* ------------------------------------------------------------------ */
/* Main                                                                 */
/* ------------------------------------------------------------------ */
int main(void) {
    srand((unsigned)time(NULL));

    /* --- Uppercase random array ------------------------------------ */
    char upper[SIZE];
    for (int i = 0; i < SIZE; i++)
        upper[i] = 'A' + rand() % 26;

    printArray(upper, SIZE, "Unsorted (uppercase)");
    int cmp = insertionSortCounted(upper, SIZE);
    printArray(upper, SIZE, "Sorted   (uppercase)");
    printf("Comparisons made: %d\n\n", cmp);

    /* --- Mixed-case random array ----------------------------------- */
    char mixed[SIZE];
    for (int i = 0; i < SIZE; i++) {
        /* randomly pick upper or lower case letter */
        char base = (rand() % 2) ? 'A' : 'a';
        mixed[i]  = base + rand() % 26;
    }

    printArray(mixed, SIZE, "Unsorted (mixed-case)");
    insertionSort(mixed, SIZE);
    printArray(mixed, SIZE, "Sorted   (mixed-case, ASCII order)");

    printf("\nNote: uppercase letters (ASCII 65-90) sort before\n");
    printf("      lowercase letters (ASCII 97-122) in ASCII order.\n");

    return 0;
}