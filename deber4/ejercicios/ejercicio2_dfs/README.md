# Ejercicio 2 — Depth-First Search (DFS)

## Descripción
Implementación del algoritmo **DFS (Depth-First Search)** para recorrer un grafo e imprimir
el valor (etiqueta) de cada nodo en el orden en que es visitado por primera vez.

Se utiliza **lista de adyacencia** como estructura de datos del grafo, que ofrece
complejidad $O(V + E)$ frente a $O(V^2)$ de la matriz de adyacencia.

## Archivos
| Archivo | Descripción |
|---|---|
| `dfs.c` | Implementación en C del algoritmo DFS recursivo con lista de adyacencia |

## Compilar y ejecutar
```bash
gcc -O0 -Wall -o dfs dfs.c
./dfs
```

## Complejidad
| Aspecto | Complejidad |
|---|---|
| Tiempo | $O(V + E)$ |
| Espacio | $O(V)$ (array visited + call stack) |

## Características
- Soporta grafos **dirigidos y no dirigidos**
- Soporta grafos **desconectados** (múltiples componentes)
- Salida con indentación visual del árbol DFS
