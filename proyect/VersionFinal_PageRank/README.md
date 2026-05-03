# Version final: PageRank

Proyecto de Analisis de Algoritmos sobre PageRank para grafos dirigidos generales.

## Estructura

- `src/pagerank.cpp`: implementacion C++ con listas de adyacencia inversas.
- `src/generate_dataset.cpp`: generador C++ de grafos sinteticos.
- `data/`: datasets en formato `source,target`.
- `results/`: salidas de los experimentos.
- `report/`: manuscrito en formato LNCS/TICEC.
- `slides/`: presentacion base.

## Compilacion

```powershell
g++ -O2 -std=c++17 src/pagerank.cpp -o pagerank.exe
g++ -O2 -std=c++17 src/generate_dataset.cpp -o generate_dataset.exe
```

## Ejecucion

```powershell
.\pagerank.exe --input data\web_example.csv --output results\web_example_scores.csv
.\generate_dataset.exe --output data\synthetic_1000.csv --nodes 1000 --avg-degree 6 --seed 2026
.\pagerank.exe --input data\synthetic_1000.csv --output results\synthetic_1000_scores.csv --damping 0.85 --tol 1e-10 --max-iter 300
```

## Formato del dataset

Cada archivo representa un grafo dirigido como lista de aristas:

```csv
source,target
A,B
A,C
B,C
```

Una fila `u,v` significa que existe un enlace dirigido desde `u` hacia `v`.
