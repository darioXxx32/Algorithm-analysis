# Ejercicio 1 — 0/1 Knapsack Problem (Exhaustive Search)

## Descripción
Solución del problema 0/1 Knapsack mediante búsqueda exhaustiva (fuerza bruta).  
Se evalúan todos los $2^n$ subconjuntos posibles de ítems y se registra el tiempo de ejecución para cada $n = 3, 4, 5, \ldots$ hasta superar los 30 segundos.

## Archivos
| Archivo | Descripción |
|---|---|
| `knapsack_exhaustive.c` | Implementación en C del algoritmo de búsqueda exhaustiva |
| `s000.kp` | Dataset de benchmark: kplib — Weakly Correlated, n=50, R=1000 |
| `results.csv` | Tabla de resultados: n, combinaciones, tiempo, mejor valor |

## Compilar y ejecutar
```bash
gcc -O0 -Wall -o knapsack_exhaustive knapsack_exhaustive.c
./knapsack_exhaustive s000.kp
```

## Resultado clave
El mayor $n$ solucionable en menos de 30 segundos es **$n^* = 28$** ($2^{28} = 268{,}435{,}456$ subconjuntos).
