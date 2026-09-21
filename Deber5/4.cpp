/*
 * Exercise 4 - Analysis of Algorithms Lab 05
 * Maximum number of distinct topological orderings for a DAG with n vertices.
 *
 * ANSWER: The maximum is n! (n factorial), achieved by the empty graph
 * (no edges at all, i.e., an independent set of n vertices).
 *
 * Reasoning:
 *   - With zero edges every vertex has in-degree 0 at every step, so
 *     every permutation of the n vertices is a valid topological order.
 *   - Adding any edge u->v forces u to appear before v, which eliminates
 *     some permutations, so the maximum cannot be exceeded by adding edges.
 *
 * This program:
 *  1. Enumerates ALL topological sorts of a given DAG using DFS backtracking.
 *  2. Computes n! and verifies that the empty graph achieves n! orderings.
 *  3. Shows how adding an edge reduces the count.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

/* ------------------------------------------------------------------ */
/* Enumerate all topological sorts via DFS backtracking                */
/* ------------------------------------------------------------------ */
struct AllTopoSorts {
    int              V;
    vector<vector<int>> adj;
    vector<int>      inDegree;
    vector<bool>     visited;
    vector<int>      path;
    vector<vector<int>> results;

    explicit AllTopoSorts(int v) : V(v), adj(v), inDegree(v, 0), visited(v, false) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        inDegree[v]++;
    }

    void backtrack() {
        bool found = false;
        for (int v = 0; v < V; v++) {
            if (!visited[v] && inDegree[v] == 0) {
                found = true;
                visited[v] = true;
                path.push_back(v);

                /* temporarily reduce in-degree of neighbours */
                for (int w : adj[v]) inDegree[w]--;

                backtrack();

                /* restore */
                for (int w : adj[v]) inDegree[w]++;
                path.pop_back();
                visited[v] = false;
            }
        }
        if (!found && (int)path.size() == V)
            results.push_back(path);
    }

    void solve() { backtrack(); }
};

/* ------------------------------------------------------------------ */
/* Factorial (for verification)                                         */
/* ------------------------------------------------------------------ */
long long factorial(int n) {
    long long f = 1;
    for (int i = 2; i <= n; i++) f *= i;
    return f;
}

/* ------------------------------------------------------------------ */
/* Demo helper                                                          */
/* ------------------------------------------------------------------ */
void runDemo(const string& title, AllTopoSorts& solver, bool printAll = true) {
    cout << "---------------------------------------------\n";
    cout << title << "\n";
    solver.solve();
    long long count = solver.results.size();
    cout << "Number of distinct topological sorts: " << count << "\n";
    if (printAll) {
        for (const auto& order : solver.results) {
            cout << "  [ ";
            for (int v : order) cout << v << " ";
            cout << "]\n";
        }
    }
    cout << "\n";
}

/* ------------------------------------------------------------------ */
/* Main                                                                 */
/* ------------------------------------------------------------------ */
int main() {
    cout << "==============================================\n";
    cout << " MAX DISTINCT TOPOLOGICAL SORTS FOR n VERTICES\n";
    cout << "==============================================\n\n";

    /* --- Demo: empty graphs of size n = 1..5 -------------------- */
    for (int n = 1; n <= 5; n++) {
        AllTopoSorts solver(n);   /* no edges added */
        solver.solve();
        long long count = solver.results.size();
        long long fact  = factorial(n);
        cout << "n=" << n
             << "  empty graph => " << count << " sorts"
             << "  (n! = " << fact << ")"
             << (count == fact ? "  [MATCH]" : "  [MISMATCH!]")
             << "\n";
    }
    cout << "\n";

    /* --- Demo: n=4 with one edge reduces the count --------------- */
    {
        cout << "---------------------------------------------\n";
        cout << "n=4, NO edges (empty graph):\n";
        AllTopoSorts solver(4);
        runDemo("", solver);
    }
    {
        cout << "---------------------------------------------\n";
        cout << "n=4, ONE edge (0->1): reduces permutations where 1 precedes 0\n";
        AllTopoSorts solver(4);
        solver.addEdge(0, 1);
        runDemo("", solver);
    }
    {
        cout << "---------------------------------------------\n";
        cout << "n=4, CHAIN (0->1->2->3): only ONE topological sort\n";
        AllTopoSorts solver(4);
        solver.addEdge(0, 1);
        solver.addEdge(1, 2);
        solver.addEdge(2, 3);
        runDemo("", solver);
    }

    /* --- Conclusion ---------------------------------------------- */
    cout << "==============================================\n";
    cout << "CONCLUSION:\n";
    cout << "  Maximum distinct topological sorts for n vertices = n!\n";
    cout << "  Achieved by the empty DAG (no edges).\n";
    cout << "  For n=4: 4! = " << factorial(4) << "\n";
    cout << "  For n=5: 5! = " << factorial(5) << "\n";
    cout << "  For n=n: n!\n";
    cout << "==============================================\n";
    return 0;
}
