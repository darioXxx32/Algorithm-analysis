#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Element structure to test stability: key is used for ordering,
   tag and original_pos identify the specific instance */
typedef struct {
    int key;
    char tag;
    int original_pos;
} Element;

/* Standard integer Merge function */
void merge_int(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    free(L);
    free(R);
}

/* Recursive Merge Sort for integer array */
void merge_sort_int(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort_int(arr, left, mid);
        merge_sort_int(arr, mid + 1, right);
        merge_int(arr, left, mid, right);
    }
}

/* Merge function for Element array (respecting stability via <=) */
void merge_elements(Element arr[], int left, int mid, int right, bool stable_comparison) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Element *L = (Element *)malloc(n1 * sizeof(Element));
    Element *R = (Element *)malloc(n2 * sizeof(Element));

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        /* When keys are equal:
           If stable_comparison is true: L[i].key <= R[j].key chooses L[i], preserving order.
           If stable_comparison is false: L[i].key < R[j].key chooses R[j] on tie, breaking stability. */
        bool take_left = stable_comparison ? (L[i].key <= R[j].key) : (L[i].key < R[j].key);
        if (take_left) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    free(L);
    free(R);
}

void merge_sort_elements(Element arr[], int left, int right, bool stable_comparison) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort_elements(arr, left, mid, stable_comparison);
        merge_sort_elements(arr, mid + 1, right, stable_comparison);
        merge_elements(arr, left, mid, right, stable_comparison);
    }
}

void print_elements(const Element arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("(%d, '%c', orig:%d) ", arr[i].key, arr[i].tag, arr[i].original_pos);
    }
    printf("\n");
}

int main(void) {
    printf("========================================\n");
    printf("  TASK 2: Standard Merge Sort in C\n");
    printf("========================================\n");
    int demo[] = {38, 27, 43, 3, 9, 82, 10};
    int n_demo = sizeof(demo) / sizeof(demo[0]);
    printf("Original array: ");
    for (int i = 0; i < n_demo; i++) printf("%d ", demo[i]);
    printf("\n");

    merge_sort_int(demo, 0, n_demo - 1);
    printf("Sorted array:   ");
    for (int i = 0; i < n_demo; i++) printf("%d ", demo[i]);
    printf("\n\n");

    printf("========================================\n");
    printf("  TASK 3: Stability Analysis & Experiment\n");
    printf("========================================\n");

    /* Create sample with duplicate keys to test relative order preservation */
    Element original[] = {
        {5, 'A', 0},
        {3, 'A', 1},
        {5, 'B', 2},
        {2, 'A', 3},
        {5, 'C', 4},
        {1, 'A', 5},
        {3, 'B', 6}
    };
    int n_elem = sizeof(original) / sizeof(original[0]);

    Element test_stable[7];
    Element test_unstable[7];
    memcpy(test_stable, original, sizeof(original));
    memcpy(test_unstable, original, sizeof(original));

    printf("Initial Array of Elements (Key, Tag, Original Index):\n");
    print_elements(original, n_elem);

    printf("\nSorting using standard Merge Sort (Left element preferred on equality '<='):\n");
    merge_sort_elements(test_stable, 0, n_elem - 1, true);
    print_elements(test_stable, n_elem);

    bool is_stable = true;
    for (int i = 0; i < n_elem - 1; i++) {
        if (test_stable[i].key == test_stable[i + 1].key) {
            if (test_stable[i].original_pos > test_stable[i + 1].original_pos) {
                is_stable = false;
            }
        }
    }
    printf("Result: %s (Relative order of identical keys {5A, 5B, 5C} and {3A, 3B} is preserved)\n",
           is_stable ? "STABLE" : "NOT STABLE");

    printf("\nContrast: Sorting using modified condition (Right element preferred on equality '<'):\n");
    merge_sort_elements(test_unstable, 0, n_elem - 1, false);
    print_elements(test_unstable, n_elem);

    bool is_alt_stable = true;
    for (int i = 0; i < n_elem - 1; i++) {
        if (test_unstable[i].key == test_unstable[i + 1].key) {
            if (test_unstable[i].original_pos > test_unstable[i + 1].original_pos) {
                is_alt_stable = false;
            }
        }
    }
    printf("Result: %s (Relative order of identical keys was inverted)\n",
           is_alt_stable ? "STABLE" : "NOT STABLE");

    return 0;
}
