#include <stdio.h>

int euclid(int m, int n) {
  while (n != 0) {
    int r = m % n;
    m = n;
    n = r;
  }
  return m;
}

int main() {
  int m, n;

  printf("Enter m: ");
  scanf("%d", &m);

  printf("Enter n: ");
  scanf("%d", &n);

  printf("gcd(%d, %d) = %d\n", m, n, euclid(m, n));

  return 0;
}