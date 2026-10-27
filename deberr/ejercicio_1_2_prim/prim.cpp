#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <iomanip>

using namespace std;

const int INF = numeric_limits<int>::max();
const int NUM_V = 5;
const char node_names[NUM_V] = {'a', 'b', 'c', 'd', 'e'};

struct Element {
    int v;
    int key;
    int parent;

    bool operator>(const Element& other) const {
        return key > other.key;
    }
};

void runPrim(int start_node) {
    int graph[NUM_V][NUM_V] = {
        {0, 5, 7, 0, 2},
        {5, 0, 0, 6, 3},
        {7, 0, 0, 4, 4},
        {0, 6, 4, 0, 5},
        {2, 3, 4, 5, 0}
    };

    vector<int> key(NUM_V, INF);
    vector<int> parent(NUM_V, -1);
    vector<bool> inMST(NUM_V, false);

    key[start_node] = 0;

    cout << "============================================================\n";
    cout << "    EJECUCION DE PRIM EN C++ CON COLA DE PRIORIDAD (STL)    \n";
    cout << "============================================================\n\n";

    cout << "Paso 0: Vertice inicial seleccionado = " << node_names[start_node] << "\n";
    inMST[start_node] = true;
    for (int v = 0; v < NUM_V; v++) {
        if (graph[start_node][v] != 0 && !inMST[v]) {
            key[v] = graph[start_node][v];
            parent[v] = start_node;
        }
    }

    auto print_queue = [&]() {
        cout << "  Cola de Prioridad: ";
        bool first = true;
        for (int i = 0; i < NUM_V; i++) {
            if (!inMST[i]) {
                if (!first) cout << ", ";
                if (key[i] == INF) cout << node_names[i] << "(-, inf)";
                else cout << node_names[i] << "(" << node_names[parent[i]] << ", " << key[i] << ")";
                first = false;
            }
        }
        if (first) cout << "(vacia)";
        cout << "\n";
    };

    print_queue();
    cout << "\n";

    for (int step = 1; step < NUM_V; step++) {
        int u = -1;
        int min_key = INF;
        for (int v = 0; v < NUM_V; v++) {
            if (!inMST[v] && key[v] < min_key) {
                min_key = key[v];
                u = v;
            }
        }
        if (u == -1) break;

        inMST[u] = true;
        cout << "Paso " << step << ":\n";
        cout << "  Vertice extraido (minimo): " << node_names[u] 
             << " [Arista (" << node_names[parent[u]] << ", " << node_names[u] 
             << ") con peso " << key[u] << "]\n";

        for (int v = 0; v < NUM_V; v++) {
            if (graph[u][v] != 0 && !inMST[v]) {
                if (graph[u][v] < key[v]) {
                    key[v] = graph[u][v];
                    parent[v] = u;
                }
            }
        }
        print_queue();
        cout << "\n";
    }

    cout << "============================================================\n";
    cout << "          ARISTAS DEL ARBOL RECUBRIDOR MINIMO (MST)\n";
    cout << "============================================================\n";
    int total_cost = 0;
    for (int i = 0; i < NUM_V; i++) {
        if (i != start_node) {
            cout << "Arista (" << node_names[parent[i]] << ", " << node_names[i] 
                 << ") con peso = " << key[i] << "\n";
            total_cost += key[i];
        }
    }
    cout << "------------------------------------------------------------\n";
    cout << "Costo Total del MST = " << total_cost << "\n";
    cout << "============================================================\n";
}

int main() {
    runPrim(0);
    return 0;
}
