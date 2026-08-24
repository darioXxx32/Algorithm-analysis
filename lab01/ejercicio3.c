#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Sieve of Eratosthenes: returns an array of primes up to n
// Sets *count to the number of primes found
int* sieve(int n, int *count) {
    // Create boolean array initialized to 1 (true)
    char *is_prime = (char *)malloc((n + 1) * sizeof(char));
    memset(is_prime, 1, (n + 1) * sizeof(char));
    is_prime[0] = 0;
    is_prime[1] = 0;

    for (int p = 2; p * p <= n; p++) {
        if (is_prime[p]) {
            for (int j = p * p; j <= n; j += p) {
                is_prime[j] = 0;
            }
        }
    }

    // Count primes
    *count = 0;
    for (int i = 2; i <= n; i++) {
        if (is_prime[i]) (*count)++;
    }

    // Collect primes into array
    int *primes = (int *)malloc((*count) * sizeof(int));
    int idx = 0;
    for (int i = 2; i <= n; i++) {
        if (is_prime[i]) primes[idx++] = i;
    }

    free(is_prime);
    return primes;
}

// Middle-School Procedure for GCD
int middleSchoolGCD(int m, int n) {
    // Generate primes up to max(m, n)
    int max_val = (m > n) ? m : n;
    int prime_count;
    int *primes = sieve(max_val, &prime_count);

    // Step 1: Find prime factors of m
    int factorsM[200], countM = 0;
    int temp_m = m;
    for (int i = 0; i < prime_count && temp_m > 1; i++) {
        while (temp_m % primes[i] == 0) {
            factorsM[countM++] = primes[i];
            temp_m = temp_m / primes[i];
        }
    }

    // Step 2: Find prime factors of n
    int factorsN[200], countN = 0;
    int temp_n = n;
    for (int i = 0; i < prime_count && temp_n > 1; i++) {
        while (temp_n % primes[i] == 0) {
            factorsN[countN++] = primes[i];
            temp_n = temp_n / primes[i];
        }
    }

    // Step 3: Identify common factors (two-pointer)
    int common[200], countC = 0;
    int i = 0, j = 0;
    while (i < countM && j < countN) {
        if (factorsM[i] == factorsN[j]) {
            common[countC++] = factorsM[i];
            i++;
            j++;
        } else if (factorsM[i] < factorsN[j]) {
            i++;
        } else {
            j++;
        }
    }

    // Step 4: Compute product of common factors
    int gcd = 1;
    for (int k = 0; k < countC; k++) {
        gcd = gcd * common[k];
    }

    free(primes);
    return gcd;
}

int main() {
    int m, n;

    printf("Enter m: ");
    scanf("%d", &m);

    printf("Enter n: ");
    scanf("%d", &n);

    printf("gcd(%d, %d) = %d\n", m, n, middleSchoolGCD(m, n));

    return 0;
}
