# Ejercicio 3 — Breadth-First Search (BFS)

## Descripción
Implementación del algoritmo **BFS (Breadth-First Search)** para recorrer un grafo e imprimir
el valor (etiqueta) de cada nodo en el orden en que es visitado, junto con su distancia
mínima (en saltos) desde el nodo fuente.

Se utiliza **lista de adyacencia + cola circular (FIFO)** como estructuras de datos.
La cola garantiza el recorrido nivel por nivel, lo que permite calcular caminos más cortos
en grafos no ponderados.

## Archivos
| Archivo | Descripción |
|---|---|
| `bfs.c` | Implementación en C del algoritmo BFS con cola circular y lista de adyacencia |

## Compilar y ejecutar
```bash
gcc -O0 -Wall -o bfs bfs.c
./bfs
```

## Complejidad
| Aspecto | Complejidad |
|---|---|
| Tiempo | $O(V + E)$ |
| Espacio | $O(V)$ (cola + arrays visited/dist) |

## Características
- Soporta grafos **dirigidos y no dirigidos**
- Soporta grafos **desconectados** (múltiples componentes)
- Calcula **distancias mínimas** (número de saltos) desde el nodo fuente
- Muestra el nivel BFS de cada nodo en la salida
