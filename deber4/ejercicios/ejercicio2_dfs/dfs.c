#include <stdio.h>   // libreria de entrada y salida
#include <stdlib.h>  // libreria de gestion de memoria
#include <string.h>  // libreria para manipulacion de cadenas

#define MAX_VERTICES 64  // maximo numero de vertices soportados

// estructura para los nodos de la lista de adyacencia
typedef struct AdjNode {
  int dest;             // vertice destino
  struct AdjNode *next; // puntero al siguiente nodo adyacente
} AdjNode;

// estructura principal para representar el grafo
typedef struct {
  int V;                        // cantidad total de vertices
  int directed;                 // 1 si es dirigido, 0 si no lo es
  AdjNode *heads[MAX_VERTICES]; // arreglo de listas de adyacencia
  char label[MAX_VERTICES][32]; // etiquetas de los vertices
} Graph;

// crea e inicializa un grafo vacio de V vertices
Graph *graph_create(int V, int directed) {
  if (V > MAX_VERTICES) { // valida limite de vertices
    fprintf(stderr, "Error: V=%d exceeds MAX_VERTICES=%d\n", V, MAX_VERTICES); // error
    return NULL; // retorna nulo en error
  }
  Graph *g = (Graph *)calloc(1, sizeof(Graph)); // asigna memoria en cero
  g->V = V;                                     // asigna cantidad de vertices
  g->directed = directed;                       // asigna si es dirigido
  for (int i = 0; i < V; i++) {
    g->heads[i] = NULL;                         // inicializa cabezas vacias
    snprintf(g->label[i], sizeof(g->label[i]), "%d", i); // asigna etiqueta por defecto
  }
  return g;                                     // retorna el grafo creado
}

// agrega una arista dirigida src -> dest
static void add_directed_edge(Graph *g, int src, int dest) {
  AdjNode *node = (AdjNode *)malloc(sizeof(AdjNode)); // reserva memoria para nodo
  node->dest = dest;                                  // establece vertice destino
  node->next = g->heads[src];                         // inserta al inicio
  g->heads[src] = node;                               // actualiza cabeza de lista
}

// agrega una arista al grafo (dirigida o no dirigida)
void graph_add_edge(Graph *g, int src, int dest) {
  add_directed_edge(g, src, dest);     // agrega arista de ida
  if (!g->directed)
    add_directed_edge(g, dest, src);   // si no es dirigido, agrega retorno
}

// asigna etiqueta/nombre personalizado a un vertice
void graph_set_label(Graph *g, int v, const char *lbl) {
  strncpy(g->label[v], lbl, sizeof(g->label[v]) - 1); // copia el texto
  g->label[v][sizeof(g->label[v]) - 1] = '\0';         // garantiza terminacion nula
}

// libera la memoria asignada al grafo
void graph_free(Graph *g) {
  for (int i = 0; i < g->V; i++) {
    AdjNode *cur = g->heads[i]; // inicio de la lista
    while (cur) {
      AdjNode *tmp = cur->next; // guarda referencia al siguiente
      free(cur);                // libera nodo actual
      cur = tmp;                // avanza al siguiente
    }
  }
  free(g);                      // libera la estructura principal
}

// imprime la estructura del grafo
void graph_print(const Graph *g) {
  printf("Graph (%s, %d vertices):\n", g->directed ? "directed" : "undirected",
         g->V); // imprime tipo y total de nodos
  for (int i = 0; i < g->V; i++) {
    printf("  [%s] ->", g->label[i]); // imprime etiqueta de origen
    AdjNode *cur = g->heads[i];       // obtiene vecinos
    if (!cur)
      printf(" (none)");              // indica sin vecinos
    while (cur) {
      printf(" %s", g->label[cur->dest]); // imprime vecino
      cur = cur->next;                    // avanza nodo
    }
    printf("\n");
  }
  printf("\n");
}

// funcion recursiva para recorrido en profundidad DFS
static void dfs_recursive(const Graph *g, int v, int visited[], int depth) {
  visited[v] = 1; // marca vertice actual como visitado
  for (int i = 0; i < depth; i++)
    printf("  "); // sangria segun profundidad en arbol DFS
  printf("|-- [%s]\n", g->label[v]); // imprime etiqueta del nodo visitado

  AdjNode *cur = g->heads[v]; // obtiene puntero a la lista de adyacencia
  while (cur) {
    if (!visited[cur->dest]) // si el vecino no ha sido visitado
      dfs_recursive(g, cur->dest, visited, depth + 1); // llamada recursiva DFS
    cur = cur->next; // avanza al siguiente vecino
  }
}

// funcion principal DFS que maneja componentes desconectadas
void dfs(const Graph *g, int start) {
  int visited[MAX_VERTICES]; // vector de visitados
  memset(visited, 0, sizeof(visited)); // inicializa visitados a 0

  printf("DFS traversal starting from vertex [%s]:\n", g->label[start]);
  printf("(tree indented by depth; each component listed separately)\n\n");

  dfs_recursive(g, start, visited, 0); // inicia DFS en vertice inicial

  for (int v = 0; v < g->V; v++) {
    if (!visited[v]) { // verifica componentes desconectados
      printf("\n[disconnected component, starting from %s]\n", g->label[v]);
      dfs_recursive(g, v, visited, 0); // ejecuta DFS en componente disjunto
    }
  }
  printf("\n");
}

static int order_idx = 0; // contador global del orden de visita

// subrutina para registrar el orden de visita DFS
static void dfs_order_rec(const Graph *g, int v, int visited[], int order[]) {
  visited[v] = 1; // marca visitado
  order[order_idx++] = v; // guarda en el vector de orden
  AdjNode *cur = g->heads[v]; // vecinos de v
  while (cur) {
    if (!visited[cur->dest]) // si no esta visitado
      dfs_order_rec(g, cur->dest, visited, order); // llamada recursiva
    cur = cur->next; // siguiente vecino
  }
}

// imprime el orden completo de visita DFS
void dfs_print_order(const Graph *g, int start) {
  int visited[MAX_VERTICES]; // arreglo visitados
  int order[MAX_VERTICES];   // arreglo para guardar orden
  memset(visited, 0, sizeof(visited)); // resetea visitados
  order_idx = 0; // resetea contador de orden

  dfs_order_rec(g, start, visited, order); // primera busqueda DFS
  for (int v = 0; v < g->V; v++)
    if (!visited[v]) // si restan nodos por visitar
      dfs_order_rec(g, v, visited, order); // busqueda en componentes restantes

  printf("DFS visit order: ");
  for (int i = 0; i < order_idx; i++) {
    printf("[%s]", g->label[order[i]]); // imprime etiqueta
    if (i + 1 < order_idx)
      printf(" -> "); // separador
  }
  printf("\n\n");
}

static void demo_undirected(void) {
  printf("=======================================================\n");
  printf(" DEMO 1: Undirected connected graph\n");
  printf("=======================================================\n\n");

  Graph *g = graph_create(6, 0); // crea grafo no dirigido de 6 nodos
  graph_set_label(g, 0, "A"); // asigna etiqueta A
  graph_set_label(g, 1, "B"); // asigna etiqueta B
  graph_set_label(g, 2, "C"); // asigna etiqueta C
  graph_set_label(g, 3, "D"); // asigna etiqueta D
  graph_set_label(g, 4, "E"); // asigna etiqueta E
  graph_set_label(g, 5, "F"); // asigna etiqueta F

  graph_add_edge(g, 0, 1); // arista A-B
  graph_add_edge(g, 0, 2); // arista A-C
  graph_add_edge(g, 1, 3); // arista B-D
  graph_add_edge(g, 1, 4); // arista B-E
  graph_add_edge(g, 2, 3); // arista C-D
  graph_add_edge(g, 3, 5); // arista D-F

  graph_print(g); // imprime lista de adyacencia
  dfs(g, 0); // ejecuta DFS desde nodo 0
  dfs_print_order(g, 0); // muestra orden de visita
  graph_free(g); // libera memoria
}

static void demo_directed(void) {
  printf("=======================================================\n");
  printf(" DEMO 2: Directed graph (with disconnected component)\n");
  printf("=======================================================\n\n");

  Graph *g = graph_create(7, 1); // crea grafo dirigido de 7 nodos

  graph_add_edge(g, 0, 1); // arista 0 -> 1
  graph_add_edge(g, 0, 2); // arista 0 -> 2
  graph_add_edge(g, 1, 3); // arista 1 -> 3
  graph_add_edge(g, 2, 4); // arista 2 -> 4
  graph_add_edge(g, 3, 4); // arista 3 -> 4
  graph_add_edge(g, 5, 6); // arista 5 -> 6

  graph_print(g); // imprime grafo
  dfs(g, 0); // ejecuta DFS
  dfs_print_order(g, 0); // imprime orden de visita
  graph_free(g); // libera memoria del grafo
}

static void demo_larger(void) {
  printf("=======================================================\n");
  printf(" DEMO 3: Larger undirected graph (9 nodes)\n");
  printf("=======================================================\n\n");

  Graph *g = graph_create(9, 0); // crea grafo de 9 nodos
  const char *names[] = {"Alice", "Bob",   "Carol", "Dave", "Eve",
                         "Frank", "Grace", "Hank",  "Iris"};
  for (int i = 0; i < 9; i++)
    graph_set_label(g, i, names[i]); // asigna nombres a los nodos

  graph_add_edge(g, 0, 1); // aristas
  graph_add_edge(g, 0, 2);
  graph_add_edge(g, 1, 3);
  graph_add_edge(g, 1, 4);
  graph_add_edge(g, 2, 5);
  graph_add_edge(g, 3, 6);
  graph_add_edge(g, 4, 7);
  graph_add_edge(g, 5, 8);
  graph_add_edge(g, 6, 8);
  graph_add_edge(g, 7, 8);

  graph_print(g); // imprime grafo
  dfs(g, 0); // ejecuta DFS
  dfs_print_order(g, 0); // muestra orden
  graph_free(g); // libera memoria
}

int main(void) {
  demo_undirected(); // demostracion 1
  demo_directed();   // demostracion 2
  demo_larger();     // demostracion 3
  return 0;          // fin del programa
}
