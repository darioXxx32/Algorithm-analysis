#include <stdio.h>   // entrada y salida
#include <stdlib.h>  // gestion de memoria
#include <string.h>  // manipulacion de cadenas

#define MAX_VERTICES 64  // maximo numero de vertices
#define INF -1           // valor representativo de infinito

// nodo de la lista de adyacencia
typedef struct AdjNode {
  int dest;             // vertice destino
  struct AdjNode *next; // siguiente nodo en la lista
} AdjNode;

// estructura para representar el grafo
typedef struct {
  int V;                        // numero total de vertices
  int directed;                 // 1 si es dirigido, 0 si no
  AdjNode *heads[MAX_VERTICES]; // listas de adyacencia
  char label[MAX_VERTICES][32]; // etiquetas de los nodos
} Graph;

// estructura de cola circular para BFS
typedef struct {
  int data[MAX_VERTICES]; // elementos de la cola
  int front, back, size;  // punteros e indice de tamaño
} Queue;

// inicializa la cola vacia
static void queue_init(Queue *q) { q->front = q->back = q->size = 0; }

// verifica si la cola esta vacia
static int queue_empty(const Queue *q) { return q->size == 0; }

// inserta un elemento en la cola
static void queue_push(Queue *q, int v) {
  q->data[q->back] = v;                   // inserta dato al final
  q->back = (q->back + 1) % MAX_VERTICES; // avance circular
  q->size++;                              // incrementa cantidad
}

// extrae un elemento de la cola
static int queue_pop(Queue *q) {
  int v = q->data[q->front];                // obtiene elemento frontal
  q->front = (q->front + 1) % MAX_VERTICES; // avance circular
  q->size--;                                // decrementa cantidad
  return v;                                 // retorna valor extraido
}

// crea un nuevo grafo con V vertices
Graph *graph_create(int V, int directed) {
  Graph *g = (Graph *)calloc(1, sizeof(Graph)); // asigna memoria en cero
  g->V = V;                                     // asigna total de vertices
  g->directed = directed;                       // asigna si es dirigido
  for (int i = 0; i < V; i++) {
    g->heads[i] = NULL;                         // listas vacias
    snprintf(g->label[i], sizeof(g->label[i]), "%d", i); // etiqueta numerica por defecto
  }
  return g;                                     // retorna puntero al grafo
}

// agrega una arista dirigida src -> dest
static void add_directed_edge(Graph *g, int src, int dest) {
  AdjNode *node = (AdjNode *)malloc(sizeof(AdjNode)); // reserva nodo
  node->dest = dest;                                  // establece destino
  node->next = g->heads[src];                         // inserta al frente
  g->heads[src] = node;                               // actualiza cabeza
}

// agrega arista al grafo
void graph_add_edge(Graph *g, int src, int dest) {
  add_directed_edge(g, src, dest);     // agrega arista de ida
  if (!g->directed)
    add_directed_edge(g, dest, src);   // si es no dirigido, agrega retorno
}

// establece etiqueta personalizada a un vertice
void graph_set_label(Graph *g, int v, const char *lbl) {
  strncpy(g->label[v], lbl, sizeof(g->label[v]) - 1); // copia texto
  g->label[v][sizeof(g->label[v]) - 1] = '\0';         // garantiza terminacion nula
}

// libera toda la memoria del grafo
void graph_free(Graph *g) {
  for (int i = 0; i < g->V; i++) {
    AdjNode *cur = g->heads[i]; // puntero a la lista
    while (cur) {
      AdjNode *tmp = cur->next; // guarda referencia al siguiente
      free(cur);                // libera nodo actual
      cur = tmp;                // avanza al siguiente nodo
    }
  }
  free(g);                      // libera estructura principal
}

// imprime la lista de adyacencia del grafo
void graph_print(const Graph *g) {
  printf("Graph (%s, %d vertices):\n", g->directed ? "directed" : "undirected",
         g->V); // imprime tipo y total de vertices
  for (int i = 0; i < g->V; i++) {
    printf("  [%s] ->", g->label[i]); // imprime origen
    AdjNode *cur = g->heads[i];       // inicio de adyacencia
    if (!cur)
      printf(" (none)");              // sin vecinos
    while (cur) {
      printf(" %s", g->label[cur->dest]); // imprime vecino
      cur = cur->next;                    // avanza nodo
    }
    printf("\n");
  }
  printf("\n");
}

// subrutina BFS desde nodo origen
static void bfs_from(const Graph *g, int src, int visited[], int dist[]) {
  Queue q;
  queue_init(&q); // inicializa cola

  visited[src] = 1; // marca origen visitado
  dist[src] = 0;    // distancia cero al origen
  queue_push(&q, src); // encola origen

  while (!queue_empty(&q)) { // mientras cola no este vacia
    int u = queue_pop(&q);   // desencola nodo

    printf("  Level %d: [%s]\n", dist[u], g->label[u]); // imprime nivel

    AdjNode *cur = g->heads[u]; // puntero a vecinos
    while (cur) {
      int v = cur->dest; // nodo destino
      if (!visited[v]) { // si no se ha visitado
        visited[v] = 1;  // marca visitado
        dist[v] = dist[u] + 1; // distancia actual + 1
        queue_push(&q, v);     // encola vecino
      }
      cur = cur->next; // siguiente vecino
    }
  }
}

// ejecuta BFS completo en el grafo
void bfs(const Graph *g, int start) {
  int visited[MAX_VERTICES]; // vector de estado
  int dist[MAX_VERTICES];    // vector de distancias
  memset(visited, 0, sizeof(visited)); // resetea visitados a 0
  for (int i = 0; i < g->V; i++)
    dist[i] = INF; // inicializa distancias como INF

  printf("BFS traversal starting from vertex [%s]:\n", g->label[start]);
  printf("(level = shortest-path distance from source)\n\n");

  bfs_from(g, start, visited, dist); // ejecuta BFS inicial

  for (int v = 0; v < g->V; v++) {
    if (!visited[v]) { // revisa componentes no conectados
      printf("\n[disconnected component, starting from %s]\n", g->label[v]);
      bfs_from(g, v, visited, dist); // BFS en componente conexa
    }
  }
  printf("\n");
}

// imprime orden de visita y resumen de distancias
void bfs_print_order(const Graph *g, int start) {
  int visited[MAX_VERTICES]; // arreglo visitados
  int dist[MAX_VERTICES];    // arreglo distancias
  int order[MAX_VERTICES];   // orden de nodos
  int order_idx = 0;         // indice del orden
  memset(visited, 0, sizeof(visited)); // resetea arreglo
  for (int i = 0; i < g->V; i++)
    dist[i] = INF; // inicializa INF

  Queue q;
  queue_init(&q); // inicializa cola
  visited[start] = 1; // marca inicio
  dist[start] = 0;    // distancia inicio
  queue_push(&q, start); // encola inicio

  while (!queue_empty(&q)) {
    int u = queue_pop(&q); // desencola
    order[order_idx++] = u; // registra orden
    AdjNode *cur = g->heads[u]; // recorre adyacentes
    while (cur) {
      if (!visited[cur->dest]) {
        visited[cur->dest] = 1; // marca visitado
        dist[cur->dest] = dist[u] + 1; // incrementa distancia
        queue_push(&q, cur->dest); // encola
      }
      cur = cur->next; // avanza
    }
  }

  for (int v = 0; v < g->V; v++) {
    if (!visited[v]) {
      visited[v] = 1; // procesa nodos restantes
      dist[v] = 0;
      queue_push(&q, v);
      while (!queue_empty(&q)) {
        int u = queue_pop(&q);
        order[order_idx++] = u; // registra en orden
        AdjNode *cur = g->heads[u];
        while (cur) {
          if (!visited[cur->dest]) {
            visited[cur->dest] = 1;
            dist[cur->dest] = dist[u] + 1;
            queue_push(&q, cur->dest);
          }
          cur = cur->next;
        }
      }
    }
  }

  printf("BFS visit order: ");
  for (int i = 0; i < order_idx; i++) {
    printf("[%s]", g->label[order[i]]); // imprime nombre de nodo
    if (i + 1 < order_idx)
      printf(" -> "); // flecha de conexion
  }
  printf("\n\n");

  printf("Shortest distances from [%s]:\n", g->label[start]);
  for (int v = 0; v < g->V; v++) {
    if (dist[v] == INF)
      printf("  [%s]: unreachable\n", g->label[v]); // nodo inalcanzable
    else
      printf("  [%s]: %d hop(s)\n", g->label[v], dist[v]); // nodo alcanzable y distancia
  }
  printf("\n");
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

  graph_add_edge(g, 0, 1); // conecta A-B
  graph_add_edge(g, 0, 2); // conecta A-C
  graph_add_edge(g, 1, 3); // conecta B-D
  graph_add_edge(g, 1, 4); // conecta B-E
  graph_add_edge(g, 2, 3); // conecta C-D
  graph_add_edge(g, 3, 5); // conecta D-F

  graph_print(g); // imprime lista de adyacencia
  bfs(g, 0); // ejecuta BFS desde nodo 0
  bfs_print_order(g, 0); // muestra orden y distancias
  graph_free(g); // libera memoria del grafo
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

  graph_print(g); // imprime el grafo
  bfs(g, 0); // ejecuta recorrido BFS
  bfs_print_order(g, 0); // muestra orden BFS
  graph_free(g); // libera grafo
}

static void demo_larger(void) {
  printf("=======================================================\n");
  printf(" DEMO 3: Larger undirected graph (9 nodes)\n");
  printf("=======================================================\n\n");

  Graph *g = graph_create(9, 0); // crea grafo de 9 nodos
  const char *names[] = {"Alice", "Bob",   "Carol", "Dave", "Eve",
                         "Frank", "Grace", "Hank",  "Iris"};
  for (int i = 0; i < 9; i++)
    graph_set_label(g, i, names[i]); // asigna nombres a cada vertice

  graph_add_edge(g, 0, 1); // agrega aristas
  graph_add_edge(g, 0, 2);
  graph_add_edge(g, 1, 3);
  graph_add_edge(g, 1, 4);
  graph_add_edge(g, 2, 5);
  graph_add_edge(g, 3, 6);
  graph_add_edge(g, 4, 7);
  graph_add_edge(g, 5, 8);
  graph_add_edge(g, 6, 8);
  graph_add_edge(g, 7, 8);

  graph_print(g); // muestra grafo
  bfs(g, 0); // ejecuta BFS
  bfs_print_order(g, 0); // imprime resultados
  graph_free(g); // libera memoria
}

int main(void) {
  demo_undirected(); // ejecuta demostracion 1
  demo_directed();   // ejecuta demostracion 2
  demo_larger();     // ejecuta demostracion 3
  return 0;          // termina programa
}
