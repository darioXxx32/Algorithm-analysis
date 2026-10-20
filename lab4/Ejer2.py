# visualization_block.py
# Añade esto al final de tu script (o como script nuevo).
import matplotlib.pyplot as plt
import networkx as nx

# --- versiones de DFS que registran orden ---
def dfs_recursive_record(g, u, visited, order):
    visited[u] = True
    order.append(u)
    for v in g.adj[u]:
        if not visited[v]:
            dfs_recursive_record(g, v, visited, order)

def dfs_iter_record(g, start):
    visited = [False]*g.n
    stack = [start]
    order = []
    while stack:
        u = stack.pop()
        if visited[u]:
            continue
        visited[u] = True
        order.append(u)
        # si quieres imitar exactamente el orden recursivo,
        # usa reversed(g.adj[u]) al apilar
        for v in g.adj[u]:
            if not visited[v]:
                stack.append(v)
    return order

# --- construir el grafo con networkx ---
def build_nx_graph(g):
    G = nx.Graph()
    G.add_nodes_from(range(g.n))
    # añadir aristas (en g ya están añadidas)
    added = set()
    for u in g.adj:
        for v in g.adj[u]:
            if (u,v) not in added and (v,u) not in added:
                G.add_edge(u, v)
                added.add((u,v))
    return G

# --- preparar datos y ejecutar DFS contabilizando orden ---
# Reuse el grafo 'g' creado antes en tu script
rec_order = []
visited = [False]*g.n
dfs_recursive_record(g, 0, visited, rec_order)

iter_order = dfs_iter_record(g, 0)

# --- mapa nodo -> posición en la visita (0-based) ---
rec_pos = {node: i for i, node in enumerate(rec_order)}
iter_pos = {node: i for i, node in enumerate(iter_order)}

# --- dibujar: dos subplots lado a lado ---
Gnx = build_nx_graph(g)
pos = nx.spring_layout(Gnx, seed=42)  # layout estable

fig, axes = plt.subplots(1, 2, figsize=(12, 5))

# función auxiliar para preparar colores/labels
def draw_with_order(ax, order_map, title):
    # Colores basados en orden de visita; los no visitados toman valor -1
    max_order = max(order_map.values()) if order_map else 0
    node_color = []
    for node in Gnx.nodes():
        if node in order_map:
            # escala 0..1
            node_color.append(order_map[node] / max(1, max_order))
        else:
            node_color.append(-0.1)  # nodos no visitados (si los hay)
    # Nodo label: "id:value"
    labels = {n: f"{n}:{g.value[n]}" for n in Gnx.nodes()}
    # dibujar nodos (colormap)
    nx.draw_networkx_edges(Gnx, pos=pos, ax=ax, alpha=0.4)
    nodes = nx.draw_networkx_nodes(
        Gnx, pos=pos, ax=ax,
        node_size=600,
        node_color=node_color,
        cmap=plt.cm.viridis,
        vmin=-0.1, vmax=1.0
    )
    nx.draw_networkx_labels(Gnx, pos=pos, labels=labels, ax=ax, font_size=9)
    ax.set_title(title)
    ax.axis('off')
    # agregar colorbar que muestre "visit order" si hay visitantes
    sm = plt.cm.ScalarMappable(cmap=plt.cm.viridis, norm=plt.Normalize(vmin=0, vmax=max(1, max_order)))
    sm.set_array([])
    cbar = fig.colorbar(sm, ax=ax, fraction=0.046, pad=0.04)
    cbar.set_label('visit rank (0 = first)')

# dibujar recursiva
draw_with_order(axes[0], rec_pos, "DFS Recursive (visit order)")

# dibujar iterativa
draw_with_order(axes[1], iter_pos, "DFS Iterative (visit order)")

plt.tight_layout()
plt.savefig("dfs_traversal_comparison.png", dpi=150)
print("Saved figure to dfs_traversal_comparison.png")
plt.show()

# --- además, imprime los órdenes (útil para el informe) ---
print("\nRecursive order (node:id:value):")
for i, node in enumerate(rec_order):
    print(f"{i:2d}: node {node} value={g.value[node]}")

print("\nIterative order (node:id:value):")
for i, node in enumerate(iter_order):
    print(f"{i:2d}: node {node} value={g.value[node]}")