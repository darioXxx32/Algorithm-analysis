/*
 * knapsack_exhaustive.c
 *
 * 0/1 Knapsack Problem — Exhaustive Search (Brute Force)
 *
 * Dataset: kplib — "01 Weakly Correlated", n=50, R=1000, instance s000
 * Source : https://github.com/likr/kplib
 * Reference: Kellerer, Pferschy & Pisinger (2004). Knapsack Problems.
 *            Springer, Berlin, Heidelberg.
 *
 * Strategy:
 *   We read the full 50-item instance once.
 *   Then for n = 3, 4, 5, ..., we run exhaustive search on the first n items,
 *   measuring execution time.  We stop when elapsed time >= 30 seconds.
 *
 * Compile:
 *   gcc -O0 -o knapsack_exhaustive knapsack_exhaustive.c
 *   (use -O0 to avoid compiler optimisations that would distort timing)
 *
 * Run:
 *   ./knapsack_exhaustive s000.kp
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ITEMS 64
#define TIME_LIMIT_SEC 30.0

/* ---------------------------------------------------------------------------
 * parse_kp_file
 *   Reads a .kp file in kplib format:
 *       n
 *       capacity
 *       (blank line)
 *       p1 w1
 *       p2 w2
 *       ...
 *       pn wn
 * ---------------------------------------------------------------------------
 */
int parse_kp_file(const char *filename, int *n_out, long long *cap_out,
                  long long profits[], long long weights[]) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    fprintf(stderr, "Error: cannot open file '%s'\n", filename);
    return 0;
  }

  int n;
  long long cap;
  if (fscanf(fp, "%d", &n) != 1 || fscanf(fp, "%lld", &cap) != 1) {
    fprintf(stderr, "Error: bad header in '%s'\n", filename);
    fclose(fp);
    return 0;
  }

  if (n > MAX_ITEMS) {
    fprintf(stderr, "Error: n=%d exceeds MAX_ITEMS=%d\n", n, MAX_ITEMS);
    fclose(fp);
    return 0;
  }

  for (int i = 0; i < n; i++) {
    if (fscanf(fp, "%lld %lld", &profits[i], &weights[i]) != 2) {
      fprintf(stderr, "Error: expected item %d in '%s'\n", i + 1, filename);
      fclose(fp);
      return 0;
    }
  }

  fclose(fp);
  *n_out = n;
  *cap_out = cap;
  return 1;
}

/* ---------------------------------------------------------------------------
 * exhaustive_knapsack
 *   Tries all 2^n subsets.
 *   Returns the maximum profit achievable without exceeding capacity.
 * ---------------------------------------------------------------------------
 */
long long exhaustive_knapsack(int n, long long capacity,
                              const long long profits[],
                              const long long weights[]) {
  long long best = 0;                         // guarda el mejor beneficio encontrado
  uint64_t total = (uint64_t)1 << n;          // calcula el total de combinaciones 2^n

  for (uint64_t mask = 0; mask < total; mask++) { // itera sobre cada combinacion de elementos
    long long w = 0, p = 0;                   // acumula peso y beneficio del subconjunto
    for (int i = 0; i < n; i++) {             // verifica el estado de cada elemento i
      if (mask & ((uint64_t)1 << i)) {        // si el elemento i esta incluido en el subconjunto
        w += weights[i];                      // suma el peso del elemento i
        p += profits[i];                      // suma el beneficio del elemento i
      }
    }
    if (w <= capacity && p > best)            // valida si no supera capacidad y mejora ganancia
      best = p;                               // actualiza la mejor ganancia encontrada
  }
  return best;                                // retorna la ganancia maxima
}

/* ---------------------------------------------------------------------------
 * main
 * ---------------------------------------------------------------------------
 */
int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <instance.kp>\n", argv[0]);
    return 1;
  }

  int n_full;
  long long capacity;
  long long profits[MAX_ITEMS];
  long long weights[MAX_ITEMS];

  if (!parse_kp_file(argv[1], &n_full, &capacity, profits, weights))
    return 1;

  printf("Dataset   : %s\n", argv[1]);
  printf("Items (N) : %d\n", n_full);
  printf("Capacity  : %lld\n\n", capacity);

  /* Table header */
  printf("%-6s  %-20s  %-18s  %-14s\n", "n", "Combinations (2^n)",
         "Time (seconds)", "Best Value");
  printf("%-6s  %-20s  %-18s  %-14s\n", "------", "--------------------",
         "------------------", "--------------");

  /* CSV header to stderr so we can redirect it separately */
  fprintf(stderr, "n,combinations,time_seconds,best_value\n");

  int max_n = (n_full < MAX_ITEMS) ? n_full : MAX_ITEMS;
  double elapsed = 0.0;

  for (int n = 3; n <= max_n && elapsed < TIME_LIMIT_SEC; n++) {

    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);

    long long best = exhaustive_knapsack(n, capacity, profits, weights);

    clock_gettime(CLOCK_MONOTONIC, &t1);

    elapsed = (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) * 1e-9;

    uint64_t combos = (uint64_t)1 << n;

    printf("%-6d  %-20llu  %-18.9f  %-14lld\n", n, (unsigned long long)combos,
           elapsed, best);
    fflush(stdout);

    fprintf(stderr, "%d,%llu,%.9f,%lld\n", n, (unsigned long long)combos,
            elapsed, best);
    fflush(stderr);
  }

  printf("\n");
  if (elapsed >= TIME_LIMIT_SEC)
    printf("Stopped: 30-second limit reached.\n");
  else
    printf("Finished all %d items within the time limit.\n", max_n);

  return 0;
}
