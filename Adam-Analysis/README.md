# Adam: análisis teórico y experimental

Proyecto de **ECMC1018 Analysis of Algorithms**, Presentation I. La implementación principal está en C++17 y estudia Adam en optimización determinista, con SGD con Momentum y AMSGrad como comparaciones. No usa PyTorch. El PDF oficial pide un informe de 12–15 páginas en formato IEEE Conference, presentación oral grabada de 10 minutos y entregar PDF de informe y slides; el anexo requiere enlaces reales a Overleaf, GitHub y video. La propuesta original de siete páginas `../proporsal.pdf` se conserva; la versión corregida de una página está en `docs/proposal_one_page.pdf`.

## Pregunta y alcance

¿Cómo se comporta Adam al minimizar objetivos de prueba y cómo varían convergencia, estabilidad y coste al cambiar la función y la tasa de aprendizaje, frente a SGD con Momentum y AMSGrad? Los resultados describen estas funciones, puntos iniciales e hiperparámetros; no justifican que un método sea siempre superior. Los experimentos son funciones deterministas de baja dimensión, no entrenamiento de redes o datos reales.

## Adam

En cada iteración se calcula `g_t = ∇f(θ_t)` y se actualizan, por coordenada, los promedios de primer y segundo momento:

```
m_t = β1 m_(t-1) + (1-β1) g_t
v_t = β2 v_(t-1) + (1-β2) g_t²
mhat_t = m_t / (1-β1^t)
vhat_t = v_t / (1-β2^t)
θ_(t+1) = θ_t - α mhat_t / (sqrt(vhat_t) + ε)
```

Los estados empiezan en cero, el contador `t` se incrementa antes de corregir sesgo y `ε` está fuera de la raíz, sumado a `sqrt(vhat)`. Valores habituales del corredor: `α=0.01`, `β1=0.9`, `β2=0.999`, `ε=1e-8`, con excepciones declaradas por experimento. SGD Momentum usa `u_t=βu_(t-1)+g_t`, `θ_(t+1)=θ_t-αu_t`; el código guarda `αu` como velocidad, equivalente con estado cero. AMSGrad sigue la variante de Reddi et al. (2018): máximo acumulado del segundo momento no corregido y primer momento no corregido; no usa la corrección de sesgo de Adam.

## Estructura y ejecución en Windows

- `src/`: optimizadores, funciones, utilidades y corredor de experimentos.
- `experiments/`: ejecutables focalizados de Rosenbrock, Rastrigin y Reddi.
- `tests/optimizer_tests.cpp`: verificaciones analíticas y de entradas.
- `results/csv/`: trayectorias y resúmenes generados en C++.
- `results/plots/`: gráficas del script Python.
- `docs/`: propuesta, borrador IEEE, diapositivas, guion y checklist.

Requiere CMake 3.10+, compilador C++17 y Ninja. La máquina usada tiene CMake 3.29, MinGW g++ 13.2 y Python 3.13 con pandas/matplotlib. Desde PowerShell:

```powershell
cd C:\Users\User\Desktop\8vo\Algorithms\proyect\Adam-Analysis
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build --clean-first
ctest --test-dir build --output-on-failure
New-Item -ItemType Directory -Force results\csv, results\plots | Out-Null
.\build\AdamAnalysis.exe
python plot_results.py
```

Ejecutar desde `Adam-Analysis` para que las rutas de salida relativas funcionen. Hay binarios separados `build\exp_rosenbrock.exe`, `build\exp_rastrigin.exe`, `build\exp_reddi.exe`.

## Experimentos, resultados y medidas

El corredor prueba `f(x)=Σx_i²` en 2D y 10D, Rosenbrock desde `(-1.5,2)`, Rastrigin desde `(2.5,2.5)`, la secuencia de Reddi y siete tasas para Adam en Rosenbrock. Los CSV registran iteración, objetivo, norma del gradiente, distancia al óptimo y coordenadas; `summary.csv` agrega hiperparámetros, tope, tolerancia, tiempo de una corrida y motivo de parada.

En la última ejecución, las cuadráticas convergieron a tolerancia `1e-8` (2D: Adam 2619, Momentum 339, AMSGrad 1639 iteraciones). Rosenbrock llegó a `1e-6` en 4443, 3118 y 16057 iteraciones respectivamente. En Rastrigin, gradiente pequeño no significó mínimo global: objetivos finales aproximados 7.96 (Adam/AMSGrad) y 1.99 (Momentum), mientras el óptimo global vale cero. Tras 300000 pasos de la construcción periódica de Reddi, Adam terminó cerca de `0.9454` y AMSGrad cerca de `-0.9982`, respecto al objetivo `-1`. En el barrido de Rosenbrock, `α=0.0001` no alcanzó tolerancia en 20000 pasos y `α=0.1` lo hizo en 1463. Son observaciones de configuraciones concretas; revisar CSV y figuras antes de citarlas.

Tiempos son mediciones de una corrida, sensibles al entorno; no sostienen comparaciones generales. Las comparaciones usan el mismo inicio, límite y tolerancia dentro de cada objetivo, pero no tasas equiparadas ni búsqueda de hiperparámetros con presupuesto común. No hay aleatoriedad en estas funciones deterministas.

## Complejidad

Con el gradiente disponible, cada optimizador recorre `d` coordenadas: tiempo `O(d)` por paso. Estado auxiliar: Momentum `O(d)`, Adam `O(d)` (dos vectores), AMSGrad `O(d)` (tres vectores). El coste de calcular el gradiente depende del objetivo y se suma por separado. Coste por iteración no determina cuántas iteraciones se requieren ni implica convergencia universal.

## Validación, fuentes y estado de entrega

CTest verifica gradientes en puntos conocidos, primera actualización, reset, parámetros inválidos, dimensiones y valores no finitos. Compilación Release limpia y prueba CTest (1/1) pasaron; se regeneraron CSV y figuras.

1. D. P. Kingma y J. Ba, “Adam: A Method for Stochastic Optimization,” ICLR, 2015. [arXiv:1412.6980](https://arxiv.org/abs/1412.6980).
2. S. J. Reddi, S. Kale y S. Kumar, “On the Convergence of Adam and Beyond,” ICLR, 2018. [OpenReview](https://openreview.net/forum?id=ryQu7f-RZ).
3. B. T. Polyak, “Some methods of speeding up the convergence of iteration methods,” USSR Computational Mathematics and Mathematical Physics, 1964. [DOI](https://doi.org/10.1016/0041-5553(64)90137-5).

El informe IEEE en `docs/report_draft.pdf` tiene **4 páginas**, frente a las 12–15 exigidas; no es final y no debe entregarse como tal. Slides preliminares de 9 páginas: `docs/presentation_slides.pdf`, notas `docs/presentation_script.md`. Pendientes: ampliar el informe con análisis sustantivo, inspección final IEEE, añadir URLs reales de Overleaf/GitHub/video y grabar la exposición. Revisar `docs/CHECKLIST.md`. Presentation II podría incorporar repeticiones, búsqueda equiparada y otros puntos iniciales.
