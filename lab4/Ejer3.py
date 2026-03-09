import os
from copy import deepcopy
from collections import defaultdict, deque
import networkx as nx
import matplotlib.pyplot as plt

class Graph:
    def __init__(self, n):
        self.n = n
        self.adj = defaultdict(list)
        self.value = [0] * n
    def add_edge(self, u, v):
        self.adj[u].append(v)
        self.adj[v].append(u)
    def sort_adj(self):
        for u in range(self.n):
            self.adj[u].sort()

def bfs_from(g: Graph, start: int):
    visited = [False] * g.n
    q = deque()
    order = []
    visited[start] = True
    q.append(start)
    while q:
        u = q.popleft()
        order.append(u)
        for v in g.adj[u]:
            if not visited[v]:
                visited[v] = True
                q.append(v)
    return order

def build_nx_graph_from_graph(g):
    G = nx.Graph()
    G.add_nodes_from(range(g.n))
    seen = set()
    for u in range(g.n):
        for v in g.adj[u]:
            if (u, v) in seen or (v, u) in seen:
                continue
            G.add_edge(u, v)
            seen.add((u, v))
    return G

def draw_two_bfs_side_by_side(g_orig, order_orig, g_sorted, order_sorted, outpath):
    G1 = build_nx_graph_from_graph(g_orig)
    # use same layout for both plots so nodes stay in same positions
    pos = nx.spring_layout(G1, seed=42)

    fig, axs = plt.subplots(1, 2, figsize=(14,6))

    # left: insertion order
    labels1 = {n: (f"{n}:{g_orig.value[n]} ({order_orig.index(n)})" if n in order_orig else f"{n}:{g_orig.value[n]} (-)") for n in G1.nodes()}
    nx.draw_networkx_edges(G1, pos=pos, ax=axs[0], alpha=0.4)
    nx.draw_networkx_nodes(G1, pos=pos, ax=axs[0], node_size=650)
    nx.draw_networkx_labels(G1, pos=pos, labels=labels1, ax=axs[0], font_size=9)
    axs[0].set_title("BFS (insertion order) from node 0")
    axs[0].axis('off')

    # right: sorted adjacency order
    G2 = build_nx_graph_from_graph(g_sorted)
    labels2 = {n: (f"{n}:{g_sorted.value[n]} ({order_sorted.index(n)})" if n in order_sorted else f"{n}:{g_sorted.value[n]} (-)") for n in G2.nodes()}
    nx.draw_networkx_edges(G2, pos=pos, ax=axs[1], alpha=0.4)
    nx.draw_networkx_nodes(G2, pos=pos, ax=axs[1], node_size=650)
    nx.draw_networkx_labels(G2, pos=pos, labels=labels2, ax=axs[1], font_size=9)
    axs[1].set_title("BFS (sorted adjacency) from node 0")
    axs[1].axis('off')

    plt.tight_layout()
    fig.savefig(outpath, dpi=200)
    plt.close(fig)

def main():
    # Example graph (same as your code)
    g = Graph(7)
    for i in range(7):
        g.value[i] = i * 10 + 5
    edges = [(0,1),(0,2),(1,3),(1,4),(2,5),(5,6)]
    for u, v in edges:
        g.add_edge(u, v)

    order_orig = bfs_from(g, 0)
    g_sorted = deepcopy(g)
    g_sorted.sort_adj()
    order_sorted = bfs_from(g_sorted, 0)

    outpath = os.path.join(os.getcwd(), "bfs_comparison.png")
    draw_two_bfs_side_by_side(g, order_orig, g_sorted, order_sorted, outpath)
    print("Saved image:", outpath)
    print("Insertion-order BFS:", order_orig)
    print("Sorted-adj BFS:    ", order_sorted)

if __name__ == "__main__":
    main()