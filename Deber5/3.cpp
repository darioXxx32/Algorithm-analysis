/*
 * Exercise 3 - Analysis of Algorithms Lab 05
 * Topological Sort: demonstration that a solution exists iff the graph is a DAG.
 *
 * The proof has two directions:
 *   (=>) If topological order exists  => graph must be a DAG.
 *   (<=) If graph is a DAG            => topological order always exists.
 *
 * This program:
 *  1. Implements Kahn's algorithm (BFS-based) for topological sort.
 *  2. Detects cycles via the in-degree check: if the sorted list length
 *     equals the number of vertices, the graph is a DAG; otherwise there
 *     is at least one cycle.
 *  3. Demonstrates on three graphs:
 *       - A valid DAG              (solution exists)
 *       - A graph with a cycle     (no solution)
 *       - A single-node graph      (trivial DAG)
 */

#include <iostream>
#include <vector>
#include <queue>
#include <stdexcept>

using namespace std;

/* ------------------------------------------------------------------ */
/* Graph represented as adjacency list                                  */
/* ------------------------------------------------------------------ */
struct Graph {
    int              V;          /* number of vertices (0-indexed) */
    vector<vector<int>> adj;

    explicit Graph(int v) : V(v), adj(v) {}

    void addEdge(int u, int v) { adj[u].push_back(v); }
};

/* ------------------------------------------------------------------ */
/* Kahn's Algorithm (BFS topological sort)                             */
/* Returns the topological order.                                       */
/* Throws runtime_error if the graph has a cycle (not a DAG).          */
/* ------------------------------------------------------------------ */
vector<int> topoSort(const Graph& g) {
    vector<int> inDegree(g.V, 0);

    /* compute in-degree of every vertex */
    for (int u = 0; u < g.V; u++)
        for (int w : g.adj[u])
            inDegree[w]++;

    /* enqueue all source vertices (in-degree == 0) */
    queue<int> q;
    for (int v = 0; v < g.V; v++)
        if (inDegree[v] == 0)
            q.push(v);

    vector<int> order;
    order.reserve(g.V);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);

        for (int w : g.adj[u]) {
            if (--inDegree[w] == 0)
                q.push(w);
        }
    }

    /* if not all vertices are in order => cycle exists => not a DAG */
    if ((int)order.size() != g.V)
        throw runtime_error("Graph contains a cycle - NOT a DAG, "
                            "topological sort has NO solution.");

    return order;
}

/* ------------------------------------------------------------------ */
/* Utility: print result                                                */
/* ------------------------------------------------------------------ */
void runDemo(const string& title, Graph& g) {
    cout << "---------------------------------------------\n";
    cout << title << "\n";
    cout << "Vertices: " << g.V << "\n";
    try {
        vector<int> order = topoSort(g);
        cout << "Is a DAG? YES\n";
        cout << "Topological order: ";
        for (int v : order) cout << v << " ";
        cout << "\n";
    } catch (const runtime_error& e) {
        cout << "Is a DAG? NO\n";
        cout << "Error: " << e.what() << "\n";
    }
    cout << "\n";
}

/* ------------------------------------------------------------------ */
/* Main                                                                 */
/* ------------------------------------------------------------------ */
int main() {
    /* --- Demo 1: valid DAG ---------------------------------------- */
    /*   0 --> 1 --> 3
     *   0 --> 2 --> 3
     *   1 --> 4
     */
    Graph dag(5);
    dag.addEdge(0, 1);
    dag.addEdge(0, 2);
    dag.addEdge(1, 3);
    dag.addEdge(2, 3);
    dag.addEdge(1, 4);
    runDemo("Demo 1 - Valid DAG (5 vertices, 5 edges)", dag);

    /* --- Demo 2: graph with a cycle ------------------------------- */
    /*   0 --> 1 --> 2 --> 0  (cycle: 0->1->2->0)
     */
    Graph cyclic(3);
    cyclic.addEdge(0, 1);
    cyclic.addEdge(1, 2);
    cyclic.addEdge(2, 0);   /* back-edge creates cycle */
    runDemo("Demo 2 - Graph with cycle (3 vertices, cycle 0->1->2->0)", cyclic);

    /* --- Demo 3: single node (trivial DAG) ------------------------ */
    Graph single(1);
    runDemo("Demo 3 - Single vertex (trivial DAG)", single);

    /* --- Summary -------------------------------------------------- */
    cout << "==============================================\n";
    cout << "CONCLUSION:\n";
    cout << " A topological sort solution EXISTS  <=> the graph is a DAG.\n";
    cout << " (Kahn's algorithm detects this via the in-degree counter.)\n";
    cout << "==============================================\n";

    return 0;
}
