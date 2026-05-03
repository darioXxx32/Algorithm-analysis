# PageRank for General Directed Graphs

Final project for the Analysis of Algorithms course. The project studies PageRank as a graph-ranking algorithm, implements it in C++17, and evaluates it with small and synthetic directed datasets.

The final manuscript follows the TICEC/LNCS format and is available in `report/ticecPaper.pdf`.

## Project Requirements Covered

- Formal definition of PageRank on directed graphs.
- C++ implementation of the algorithm.
- Handling of dangling nodes.
- Examples of problems solved by PageRank.
- Mathematical analysis and design paradigm.
- Best-case, average-case, and worst-case complexity.
- Empirical analysis using reproducible datasets.
- Final report and presentation files.

## Repository Structure

```text
VersionFinal_PageRank/
  README.md
  run_experiments.ps1
  pagerank.exe
  generate_dataset.exe
  src/
    pagerank.cpp
    generate_dataset.cpp
  data/
    web_example.csv
    synthetic_100.csv
    synthetic_500.csv
    synthetic_1000.csv
    synthetic_5000.csv
  results/
    *_scores.csv
    experiment_summary.csv
    damping_summary.csv
  report/
    ticecPaper.tex
    ticecPaper.pdf
    llncs.cls
  slides/
    presentation.tex
    presentation.pdf
```

## Requirements

- Windows PowerShell.
- A C++ compiler with C++17 support, such as `g++`.
- Optional for editing the report: a LaTeX distribution such as MiKTeX.

## Build

From the project root:

```powershell
g++ -O2 -std=c++17 src\pagerank.cpp -o pagerank.exe
g++ -O2 -std=c++17 src\generate_dataset.cpp -o generate_dataset.exe
```

## Input Format

The input graph is a CSV edge list. Each row represents a directed edge from `source` to `target`.

```csv
source,target
A,B
A,C
B,C
C,A
```

Node labels can be arbitrary strings. Internally, the program maps labels to integer identifiers for efficient processing.

## Run PageRank

Example using the small web graph:

```powershell
.\pagerank.exe --input data\web_example.csv --output results\web_example_scores.csv --damping 0.85 --tol 1e-10 --max-iter 300
```

Main parameters:

- `--input`: input CSV edge list.
- `--output`: output CSV ranking file.
- `--damping`: damping factor, with `0 < d < 1`.
- `--tol`: convergence tolerance.
- `--max-iter`: maximum number of iterations.

The output file contains:

```csv
rank,node,score,out_degree,in_degree
```

## Generate Synthetic Datasets

Example:

```powershell
.\generate_dataset.exe --output data\synthetic_1000.csv --nodes 1000 --avg-degree 6 --seed 2028
```

The generator creates directed graphs with a fixed seed so experiments can be reproduced.

## Reproduce All Experiments

Run:

```powershell
powershell -ExecutionPolicy Bypass -File .\run_experiments.ps1
```

This script compiles the programs, generates synthetic datasets, runs PageRank on all datasets, and produces result CSV files.

## Report and Presentation

- Final report: `report/ticecPaper.pdf`
- Report source: `report/ticecPaper.tex`
- Presentation: `slides/presentation.pdf`
- Presentation source: `slides/presentation.tex`

The report is written in English and follows the TICEC/LNCS template.

## Moodle Deliverables

Upload the following files or folders as required by the course instructions:

- PDF report: `report/ticecPaper.pdf`
- Presentation slides: `slides/presentation.pdf`
- C++ implementation: `src/`
- Datasets: `data/`
- Results, if requested: `results/`
- GitHub link for the implementation.
- Overleaf link for the manuscript.

The GitHub and Overleaf links should also be added to the Annexes section of the final report before submission.

## Author

Dario Pomasqui  
Yachay Tech University  
`dario.pomasqui@yachaytech.edu.ec`

