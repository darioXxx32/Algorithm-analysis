#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INF 999999
#define V 5

const char node_names[V] = {'a', 'b', 'c', 'd', 'e'};

/* Matriz de adyacencia del Grafo del Ejercicio 1:
      a  b  c  d  e
   a [0, 5, 7, 0, 2]
   b [5, 0, 0, 6, 3]
   c [7, 0, 0, 4, 4]
   d [0, 6, 4, 0, 5]
   e [2, 3, 4, 5, 0]
*/

int graph[V][V] = {
    {0, 5, 7, 0, 2},
    {5, 0, 0, 6, 3},
    {7, 0, 0, 4, 4},
    {0, 6, 4, 0, 5},
    {2, 3, 4, 5, 0}
};

/* Función para imprimir el estado de la cola de prioridad de vértices restantes */
void print_priority_queue(int key[], int parent[], bool inMST[]) {
    printf("  Cola de Prioridad (Vertices restantes): ");
    bool first = true;
    for (int v = 0; v < V; v++) {
        if (!inMST[v]) {
            if (!first) printf(", ");
            if (key[v] == INF) {
                printf("%c(-, inf)", node_names[v]);
            } else {
                printf("%c(%c, %d)", node_names[v], node_names[parent[v]], key[v]);
            }
            first = false;
        }
    }
    if (first) {
        printf("(vacia)");
    }
    printf("\n");
}

/* Función para encontrar el vértice con la clave mínima en la cola de prioridad */
int extract_min(int key[], bool inMST[]) {
    int min = INF, min_index = -1;
    for (int v = 0; v < V; v++) {
        if (!inMST[v] && key[v] < min) {
            min = key[v];
            min_index = v;
        }
    }
    return min_index;
}

void primMST(int start_node) {
    int parent[V];  // Guarda el padre de cada vértice en el MST
    int key[V];     // Peso mínimo para conectar a cada vértice fuera del árbol
    bool inMST[V];  // Indica si el vértice ya está en el árbol

    // 1. Inicialización
    for (int i = 0; i < V; i++) {
        key[i] = INF;
        parent[i] = -1;
        inMST[i] = false;
    }

    // El vértice inicial entra con clave 0
    key[start_node] = 0;

    printf("============================================================\n");
    printf("       EJECUCION DEL ALGORITMO DE PRIM (INICIO EN %c)\n", node_names[start_node]);
    printf("============================================================\n\n");

    // Mostrar estado inicial de la cola
    printf("Paso 0 (Inicializacion):\n");
    printf("  Vertice raiz seleccionado: %c\n", node_names[start_node]);
    inMST[start_node] = true;

    // Actualizar los vecinos del nodo inicial
    for (int v = 0; v < V; v++) {
        if (graph[start_node][v] != 0 && !inMST[v]) {
            parent[v] = start_node;
            key[v] = graph[start_node][v];
        }
    }
    print_priority_queue(key, parent, inMST);
    printf("\n");

    // Construcción del árbol (agregar los V - 1 vértices restantes)
    for (int step = 1; step < V; step++) {
        // Extraer el mínimo de la cola de prioridad
        int u = extract_min(key, inMST);
        if (u == -1) break;

        inMST[u] = true;
        printf("Paso %d:\n", step);
        printf("  Vertice extraido de la cola (minimo): %c (Padre: %c, Peso: %d)\n", 
               node_names[u], node_names[parent[u]], key[u]);
        printf("  Arista agregada al arbol: (%c, %c) con costo %d\n", 
               node_names[parent[u]], node_names[u], key[u]);

        // Actualizar claves de los vértices adyacentes aún en la cola
        for (int v = 0; v < V; v++) {
            if (graph[u][v] != 0 && !inMST[v]) {
                if (graph[u][v] < key[v]) {
                    printf("    -> Actualizando vertice %c: costo previo = %s, nuevo costo = %d (via %c)\n",
                           node_names[v], 
                           key[v] == INF ? "inf" : "mayor", 
                           graph[u][v], 
                           node_names[u]);
                    key[v] = graph[u][v];
                    parent[v] = u;
                }
            }
        }

        print_priority_queue(key, parent, inMST);
        printf("\n");
    }

    // Imprimir el árbol recubridor mínimo resultante
    printf("============================================================\n");
    printf("        ARBOL RECUBRIDOR MINIMO RESULTANTE (MST)\n");
    printf("============================================================\n");
    printf("Arista       Peso\n");
    printf("-----------------\n");
    int total_weight = 0;
    for (int i = 0; i < V; i++) {
        if (i != start_node) {
            printf("(%c, %c)       %d\n", node_names[parent[i]], node_names[i], key[i]);
            total_weight += key[i];
        }
    }
    printf("-----------------\n");
    printf("Peso Total del MST = %d\n", total_weight);
    printf("============================================================\n");
}

int main(void) {
    primMST(0); // Inicia en el vértice 'a' (índice 0)
    return 0;
}
