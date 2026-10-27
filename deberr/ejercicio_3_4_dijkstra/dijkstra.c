#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INF 999999
#define MAX_V 20

typedef struct {
    int num_vertices;
    bool is_directed;
    char names[MAX_V];
    int adj[MAX_V][MAX_V];
} Graph;

void init_graph(Graph *g, int n, bool directed, const char *names) {
    g->num_vertices = n;
    g->is_directed = directed;
    for (int i = 0; i < n; i++) {
        g->names[i] = names[i];
        for (int j = 0; j < n; j++) {
            g->adj[i][j] = (i == j) ? 0 : INF;
        }
    }
}

void add_edge(Graph *g, int u, int v, int w) {
    g->adj[u][v] = w;
    if (!g->is_directed) {
        g->adj[v][u] = w;
    }
}

int extract_min_dijkstra(int dist[], bool visited[], int n) {
    int min = INF, min_idx = -1;
    for (int v = 0; v < n; v++) {
        if (!visited[v] && dist[v] < min) {
            min = dist[v];
            min_idx = v;
        }
    }
    return min_idx;
}

void print_dijkstra_queue(Graph *g, int dist[], int parent[], bool visited[]) {
    printf("  Cola de Prioridad (Vertices Restantes): ");
    bool first = true;
    for (int v = 0; v < g->num_vertices; v++) {
        if (!visited[v]) {
            if (!first) printf(", ");
            if (dist[v] == INF) {
                printf("%c(-, inf)", g->names[v]);
            } else {
                printf("%c(%c, %d)", g->names[v], g->names[parent[v]], dist[v]);
            }
            first = false;
        }
    }
    if (first) printf("(vacia)");
    printf("\n");
}

void print_path(Graph *g, int parent[], int current) {
    if (parent[current] == -1) {
        printf("%c", g->names[current]);
        return;
    }
    print_path(g, parent, parent[current]);
    printf(" -> %c", g->names[current]);
}

void dijkstra(Graph *g, int source, const char *instance_title) {
    int n = g->num_vertices;
    int dist[MAX_V];
    int parent[MAX_V];
    bool visited[MAX_V];

    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        parent[i] = -1;
        visited[i] = false;
    }

    dist[source] = 0;

    printf("================================================================================\n");
    printf("   ALGORITMO DE DIJKSTRA: %s\n", instance_title);
    printf("   Vertice Origen: %c | Tipo de Grafo: %s | Total Vertices: %d\n", 
           g->names[source], g->is_directed ? "Dirigido" : "No Dirigido", n);
    printf("================================================================================\n\n");

    printf("Paso 0 (Inicializacion):\n");
    printf("  Vertice inicial anadido al arbol: %c(-, 0)\n", g->names[source]);
    visited[source] = true;

    // Relajación inicial para vecinos del origen
    for (int v = 0; v < n; v++) {
        if (g->adj[source][v] != INF && v != source) {
            dist[v] = g->adj[source][v];
            parent[v] = source;
        }
    }
    print_dijkstra_queue(g, dist, parent, visited);
    printf("\n");

    for (int step = 1; step < n; step++) {
        int u = extract_min_dijkstra(dist, visited, n);
        if (u == -1) {
            printf("Aviso: Los vertices restantes no son alcanzables desde el origen %c.\n", g->names[source]);
            break;
        }

        visited[u] = true;
        printf("Paso %d:\n", step);
        printf("  Vertice anadido al arbol: %c (Padre: %c, Distancia Acumulada: %d)\n", 
               g->names[u], g->names[parent[u]], dist[u]);

        // Relajación de aristas salientes de u
        for (int v = 0; v < n; v++) {
            if (!visited[v] && g->adj[u][v] != INF) {
                if (dist[u] + g->adj[u][v] < dist[v]) {
                    printf("    -> Relajacion de arista (%c -> %c, peso %d): distancia previa = %s, nueva = %d\n",
                           g->names[u], g->names[v], g->adj[u][v],
                           dist[v] == INF ? "inf" : "mayor", dist[u] + g->adj[u][v]);
                    dist[v] = dist[u] + g->adj[u][v];
                    parent[v] = u;
                }
            }
        }

        print_dijkstra_queue(g, dist, parent, visited);
        printf("\n");
    }

    // Resumen de caminos más cortos
    printf("================================================================================\n");
    printf("                 TABLA FINAL DE CAMINOS MINIMOS DESDE %c\n", g->names[source]);
    printf("================================================================================\n");
    printf("%-10s %-12s %-15s %-30s\n", "Destino", "Distancia", "Padre Directo", "Camino Completo");
    printf("--------------------------------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        if (dist[i] == INF) {
            printf("%-10c %-12s %-15s %-30s\n", g->names[i], "INF", "Ninguno", "Inalcanzable");
        } else {
            char parent_str[4];
            if (parent[i] == -1) sprintf(parent_str, "-");
            else sprintf(parent_str, "%c", g->names[parent[i]]);

            printf("%-10c %-12d %-15s ", g->names[i], dist[i], parent_str);
            print_path(g, parent, i);
            printf("\n");
        }
    }
    printf("================================================================================\n\n\n");
}

int main(void) {
    // ---------------------------------------------------------
    // INSTANCIA 3.a (Grafo Dirigido con 5 vertices)
    // ---------------------------------------------------------
    Graph g3a;
    const char names3a[] = {'a', 'b', 'c', 'd', 'e'};
    init_graph(&g3a, 5, true, names3a);
    // Aristas dirigidas:
    // a -> d: 7
    // b -> a: 3
    // b -> c: 4
    // d -> b: 2
    // d -> c: 5
    // c -> e: 6
    // e -> d: 4
    add_edge(&g3a, 0, 3, 7); // a -> d (7)
    add_edge(&g3a, 1, 0, 3); // b -> a (3)
    add_edge(&g3a, 1, 2, 4); // b -> c (4)
    add_edge(&g3a, 3, 1, 2); // d -> b (2)
    add_edge(&g3a, 3, 2, 5); // d -> c (5)
    add_edge(&g3a, 2, 4, 6); // c -> e (6)
    add_edge(&g3a, 4, 3, 4); // e -> d (4)

    dijkstra(&g3a, 0, "INSTANCIA 3.a (Grafo Dirigido)");

    // ---------------------------------------------------------
    // INSTANCIA 3.b (Grafo No Dirigido con 12 vertices)
    // ---------------------------------------------------------
    Graph g3b;
    const char names3b[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l'};
    init_graph(&g3b, 12, false, names3b);
    // a=0, b=1, c=2, d=3, e=4, f=5, g=6, h=7, i=8, j=9, k=10, l=11
    add_edge(&g3b, 0, 1, 3); // a - b : 3
    add_edge(&g3b, 0, 2, 5); // a - c : 5
    add_edge(&g3b, 0, 3, 4); // a - d : 4

    add_edge(&g3b, 1, 4, 3); // b - e : 3
    add_edge(&g3b, 1, 5, 6); // b - f : 6

    add_edge(&g3b, 2, 3, 2); // c - d : 2
    add_edge(&g3b, 2, 6, 4); // c - g : 4

    add_edge(&g3b, 3, 4, 1); // d - e : 1
    add_edge(&g3b, 3, 7, 5); // d - h : 5

    add_edge(&g3b, 4, 5, 2); // e - f : 2
    add_edge(&g3b, 4, 8, 4); // e - i : 4

    add_edge(&g3b, 5, 9, 5); // f - j : 5

    add_edge(&g3b, 6, 7, 3); // g - h : 3
    add_edge(&g3b, 6, 10, 6); // g - k : 6

    add_edge(&g3b, 7, 8, 6); // h - i : 6
    add_edge(&g3b, 7, 10, 7); // h - k : 7

    add_edge(&g3b, 8, 9, 3); // i - j : 3
    add_edge(&g3b, 8, 11, 5); // i - l : 5

    add_edge(&g3b, 9, 11, 9); // j - l : 9

    add_edge(&g3b, 10, 11, 8); // k - l : 8

    dijkstra(&g3b, 0, "INSTANCIA 3.b (Grafo de 12 vertices)");

    return 0;
}
