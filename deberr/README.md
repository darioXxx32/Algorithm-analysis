# Laboratorio 09: Técnica Voraz (Greedy Technique)
**Asignatura:** Análisis de Algoritmos (AoA)  
**Institución:** Escuela de Ciencias Matemáticas y Computacionales, Universidad Yachay Tech  
**Referencia:** Anany Levitin, *Introduction to the Design and Analysis of Algorithms* (3ra Edición), Capítulo 9.

---

## Estructura del Proyecto

El repositorio y la carpeta de trabajo han sido organizados de manera modular con carpetas independientes para cada tema del laboratorio, conteniendo sus códigos fuente en C y C++, ejecutables compilados, scripts de compilación automática y registros de salida:

```text
deberr/
├── AoA - Assignment 09.pdf            <- Guía original del laboratorio
├── informe.tex                        <- Informe técnico formal completo en LaTeX
├── informe.pdf                        <- Documento PDF compilado del informe
├── figures/                           <- Figuras y diagramas de los grafos originales
│   ├── graph_ex1.png
│   ├── graph_ex3a.png
│   └── graph_ex3b.png
│
├── ejercicio_1_2_prim/                <- Ejercicios 1 y 2: Algoritmo de Prim
│   ├── prim.c                         <- Código fuente en C (C11) con traza de cola de prioridad
│   ├── prim.cpp                       <- Código fuente en C++ (C++17)
│   ├── compile.bat                    <- Script para compilar y ejecutar en Windows
│   ├── salida_prim.txt                <- Registro de salida real en consola
│   ├── prim_c.exe                     <- Ejecutable compilado en C
│   └── prim_cpp.exe                   <- Ejecutable compilado en C++
│
├── ejercicio_3_4_dijkstra/            <- Ejercicios 3 y 4: Algoritmo de Dijkstra
│   ├── dijkstra.c                     <- Código fuente en C (Instancias 3.a y 3.b)
│   ├── dijkstra.cpp                   <- Código fuente en C++
│   ├── compile.bat                    <- Script para compilar y ejecutar en Windows
│   ├── salida_dijkstra.txt            <- Registro de salida con ambas instancias
│   ├── dijkstra_c.exe                 <- Ejecutable compilado en C
│   └── dijkstra_cpp.exe               <- Ejecutable compilado en C++
│
└── ejercicio_5_huffman/               <- Ejercicio 5: Códigos de Huffman
    ├── huffman.c                      <- Código fuente en C (Construcción, codificación y decodificación)
    ├── huffman.cpp                    <- Código fuente en C++
    ├── compile.bat                    <- Script para compilar y ejecutar en Windows
    ├── salida_huffman.txt             <- Registro de salida en consola
    ├── huffman_c.exe                  <- Ejecutable compilado en C
    └── huffman_cpp.exe                <- Ejecutable compilado en C++
```

---

## Resumen de Resultados

### 1. Ejercicios 1 y 2: Algoritmo de Prim (Árbol Recubridor Mínimo)
- **Vértice Inicial:** $a$.
- **Secuencia de selección voraz:**
  1. Paso 0: Inicio en $a$. Cola: $b(a, 5), c(a, 7), d(-, \infty), e(a, 2)$.
  2. Paso 1: Extrae $e(a, 2)$. Arista $(a, e)$, peso 2. Actualiza $b(e, 3), c(e, 4), d(e, 5)$.
  3. Paso 2: Extrae $b(e, 3)$. Arista $(e, b)$, peso 3.
  4. Paso 3: Extrae $c(e, 4)$. Arista $(e, c)$, peso 4. Actualiza $d(c, 4)$.
  5. Paso 4: Extrae $d(c, 4)$. Arista $(c, d)$, peso 4.
- **Aristas del MST:** $(a, e), (e, b), (e, c), (c, d)$.
- **Costo Total:** $W(T) = 2 + 3 + 4 + 4 = 13$.

### 2. Ejercicios 3 y 4: Algoritmo de Dijkstra (Caminos Mínimos con Origen $a$)
- **Instancia 3.a (Grafo Dirigido):**
  - $a \rightarrow d$: distancia 7
  - $a \rightarrow d \rightarrow b$: distancia 9
  - $a \rightarrow d \rightarrow c$: distancia 12
  - $a \rightarrow d \rightarrow c \rightarrow e$: distancia 18
- **Instancia 3.b (Grafo de 12 Vértices):**
  - $b$: 3 ($a \rightarrow b$)
  - $c$: 5 ($a \rightarrow c$)
  - $d$: 4 ($a \rightarrow d$)
  - $e$: 5 ($a \rightarrow d \rightarrow e$)
  - $f$: 7 ($a \rightarrow d \rightarrow e \rightarrow f$)
  - $g$: 9 ($a \rightarrow c \rightarrow g$)
  - $h$: 9 ($a \rightarrow d \rightarrow h$)
  - $i$: 9 ($a \rightarrow d \rightarrow e \rightarrow i$)
  - $j$: 12 ($a \rightarrow d \rightarrow e \rightarrow f \rightarrow j$)
  - $k$: 15 ($a \rightarrow c \rightarrow g \rightarrow k$)
  - $l$: 14 ($a \rightarrow d \rightarrow e \rightarrow i \rightarrow l$)

### 3. Ejercicio 5: Códigos de Huffman
- **5.1 Códigos Asignados:**
  - $A (0.40) \rightarrow$ `1` (1 bit)
  - $B (0.10) \rightarrow$ `000` (3 bits)
  - $C (0.20) \rightarrow$ `011` (3 bits)
  - $D (0.15) \rightarrow$ `001` (3 bits)
  - $\_ (0.15) \rightarrow$ `010` (3 bits)
  - **Longitud media ponderada:** $2.20$ bits/símbolo (ahorro del 26.67\% frente a código fijo de 3 bits).
- **5.2 Codificación de "ABACABAD":**
  - Resultado: `1000101110001001` (16 bits).
- **5.3 Decodificación de "100010111001010":**
  - Desglose: `1` ($A$), `000` ($B$), `1` ($A$), `011` ($C$), `1` ($A$), `001` ($D$), `010` ($\_$).
  - Mensaje: **"ABACAD_"** (o "ABACAD ").

---

## Instrucciones de Compilación y Ejecución

### Requisitos
- Compilador GCC/G++ (MinGW o MSYS2 en Windows).
- MiKTeX o TeXLive para compilar el informe `.tex`.

### Compilación Manual
1. **Algoritmo de Prim:**
   ```bash
   cd ejercicio_1_2_prim
   gcc -Wall -Wextra -O2 prim.c -o prim_c.exe
   ./prim_c.exe
   ```
2. **Algoritmo de Dijkstra:**
   ```bash
   cd ejercicio_3_4_dijkstra
   gcc -Wall -Wextra -O2 dijkstra.c -o dijkstra_c.exe
   ./dijkstra_c.exe
   ```
3. **Códigos de Huffman:**
   ```bash
   cd ejercicio_5_huffman
   gcc -Wall -Wextra -O2 huffman.c -o huffman_c.exe
   ./huffman_c.exe
   ```
4. **Compilar el Informe LaTeX:**
   ```bash
   pdflatex -interaction=nonstopmode informe.tex
   pdflatex -interaction=nonstopmode informe.tex
   ```
