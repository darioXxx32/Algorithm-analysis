/*
 * Activity 1: Swap two variables without extra storage
 * Three methods are demonstrated:
 *   1. XOR swap
 *   2. Arithmetic swap (addition/subtraction)
 *   3. Arithmetic swap (multiplication/division) — limited to non-zero values
 */

#include <stdio.h>

/* Method 1: XOR swap */
void swap_xor(int *u, int *v) {
    *u = *u ^ *v;
    *v = *u ^ *v;
    *u = *u ^ *v;
}

/* Method 2: Arithmetic swap (add/subtract) */
void swap_add(int *u, int *v) {
    *u = *u + *v;
    *v = *u - *v;
    *u = *u - *v;
}

int main(void) {
    int u, v;

    printf("=== Swap Without Extra Storage ===\n\n");

    /* --- Method 1: XOR --- */
    u = 42;
    v = 99;
    printf("Method 1 (XOR swap):\n");
    printf("  Before: u = %d, v = %d\n", u, v);
    swap_xor(&u, &v);
    printf("  After : u = %d, v = %d\n\n", u, v);

    /* --- Method 2: Arithmetic (add/subtract) --- */
    u = 1000;
    v = -250;
    printf("Method 2 (Arithmetic add/sub swap):\n");
    printf("  Before: u = %d, v = %d\n", u, v);
    swap_add(&u, &v);
    printf("  After : u = %d, v = %d\n\n", u, v);

    /* --- Inline XOR demo (no function call) --- */
    u = 7;
    v = 3;
    printf("Inline XOR swap (no function):\n");
    printf("  Before: u = %d, v = %d\n", u, v);
    u ^= v;
    v ^= u;
    u ^= v;
    printf("  After : u = %d, v = %d\n\n", u, v);

    printf("Explanation:\n");
    printf("  XOR swap relies on the property: a ^ a = 0, a ^ 0 = a\n");
    printf("  Step 1: u = u ^ v\n");
    printf("  Step 2: v = u ^ v = (u^v)^v = u\n");
    printf("  Step 3: u = u ^ v = (u^v)^u = v\n");
    printf("  No temporary variable is needed at any step.\n");

    return 0;
}
