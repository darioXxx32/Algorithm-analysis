#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>

using namespace std;

const int INF = 999999;

struct Edge {
    int to;
    int weight;
};

void runDijkstraCpp(int n, const vector<string>& names, const vector<vector<Edge>>& adj, int src, const string& title, bool directed) {
    vector<int> dist(n, INF);
    vector<int> parent(n, -1);
    vector<bool> visited(n, false);

    dist[src] = 0;

    cout << "================================================================================\n";
    cout << "   DIJKSTRA EN C++: " << title << "\n";
    cout << "   Origen: " << names[src] << " | Grafo: " << (directed ? "Dirigido" : "No Dirigido") << " | Vertices: " << n << "\n";
    cout << "================================================================================\n\n";

    auto print_queue = [&]() {
        cout << "  Cola de Prioridad: ";
        bool first = true;
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                if (!first) cout << ", ";
                if (dist[i] == INF) cout << names[i] << "(-, inf)";
                else cout << names[i] << "(" << names[parent[i]] << ", " << dist[i] << ")";
                first = false;
            }
        }
        if (first) cout << "(vacia)";
        cout << "\n";
    };

    cout << "Paso 0: Vertice inicial " << names[src] << "(-, 0)\n";
    visited[src] = true;
    for (const auto& e : adj[src]) {
        if (!visited[e.to]) {
            dist[e.to] = e.weight;
            parent[e.to] = src;
        }
    }
    print_queue();
    cout << "\n";

    for (int step = 1; step < n; step++) {
        int u = -1;
        int min_d = INF;
        for (int i = 0; i < n; i++) {
            if (!visited[i] && dist[i] < min_d) {
                min_d = dist[i];
                u = i;
            }
        }
        if (u == -1) break;

        visited[u] = true;
        cout << "Paso " << step << ": Vertice agregado = " << names[u] 
             << " (Padre: " << names[parent[u]] << ", Distancia: " << dist[u] << ")\n";

        for (const auto& e : adj[u]) {
            int v = e.to;
            int w = e.weight;
            if (!visited[v] && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = u;
            }
        }
        print_queue();
        cout << "\n";
    }

    cout << "--------------------------------------------------------------------------------\n";
    cout << left << setw(10) << "Destino" << setw(12) << "Distancia" << setw(15) << "Padre" << "Camino Completo\n";
    cout << "--------------------------------------------------------------------------------\n";
    for (int i = 0; i < n; i++) {
        cout << left << setw(10) << names[i];
        if (dist[i] == INF) {
            cout << setw(12) << "INF" << setw(15) << "-" << "Inalcanzable\n";
        } else {
            cout << setw(12) << dist[i] << setw(15) << (parent[i] == -1 ? "-" : names[parent[i]]);
            // reconstruir ruta
            vector<int> path;
            for (int cur = i; cur != -1; cur = parent[cur]) {
                path.push_back(cur);
            }
            for (int p = (int)path.size() - 1; p >= 0; p--) {
                cout << names[path[p]] << (p > 0 ? " -> " : "");
            }
            cout << "\n";
        }
    }
    cout << "================================================================================\n\n";
}

int main() {
    // Instancia 3.a
    {
        vector<string> names = {"a", "b", "c", "d", "e"};
        vector<vector<Edge>> adj(5);
        adj[0].push_back({3, 7}); // a -> d
        adj[1].push_back({0, 3}); // b -> a
        adj[1].push_back({2, 4}); // b -> c
        adj[3].push_back({1, 2}); // d -> b
        adj[3].push_back({2, 5}); // d -> c
        adj[2].push_back({4, 6}); // c -> e
        adj[4].push_back({3, 4}); // e -> d
        runDijkstraCpp(5, names, adj, 0, "INSTANCIA 3.a (Dirigido)", true);
    }

    // Instancia 3.b
    {
        vector<string> names = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l"};
        vector<vector<Edge>> adj(12);
        auto add_undirected = [&](int u, int v, int w) {
            adj[u].push_back({v, w});
            adj[v].push_back({u, w});
        };
        add_undirected(0, 1, 3);
        add_undirected(0, 2, 5);
        add_undirected(0, 3, 4);
        add_undirected(1, 4, 3);
        add_undirected(1, 5, 6);
        add_undirected(2, 3, 2);
        add_undirected(2, 6, 4);
        add_undirected(3, 4, 1);
        add_undirected(3, 7, 5);
        add_undirected(4, 5, 2);
        add_undirected(4, 8, 4);
        add_undirected(5, 9, 5);
        add_undirected(6, 7, 3);
        add_undirected(6, 10, 6);
        add_undirected(7, 8, 6);
        add_undirected(7, 10, 7);
        add_undirected(8, 9, 3);
        add_undirected(8, 11, 5);
        add_undirected(9, 11, 9);
        add_undirected(10, 11, 8);
        runDijkstraCpp(12, names, adj, 0, "INSTANCIA 3.b (No Dirigido)", false);
    }
    return 0;
}
