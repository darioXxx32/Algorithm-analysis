#include <stdio.h>

int min(int a, int b) {
    return (a < b) ? a : b;
}

int consecutiveIntegerChecking(int m, int n) {
    int t = min(m, n);
    while (t > 0) {
        if (m % t == 0) {
            if (n % t == 0) {
                return t;
            }
        }
        t = t - 1;
    }
    return 1;
}

int main() {
    int m, n;

    printf("Enter m: ");
    scanf("%d", &m);

    printf("Enter n: ");
    scanf("%d", &n);

    printf("gcd(%d, %d) = %d\n", m, n, consecutiveIntegerChecking(m, n));

    return 0;
}
