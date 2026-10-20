# Adam: análisis teórico y experimental

Proyecto de **ECMC1018 Analysis of Algorithms**, Presentation I. La implementación principal es C++17 y estudia Adam en optimización determinista, con SGD con Momentum y AMSGrad como comparaciones. No se usa PyTorch. El PDF oficial pide informe IEEE Conference de 12–15 páginas, exposición oral grabada de 10 minutos, PDFs del informe y slides, y enlaces reales de Overleaf, GitHub y video en el anexo. La propuesta original de siete páginas se conserva en `../proporsal.pdf`; la corregida de una página está en `docs/proposal_one_page.pdf`.

## Pregunta de investigación

¿Cómo se comporta Adam al minimizar objetivos de prueba y cómo cambian convergencia y estabilidad al variar la función y la tasa de aprendizaje frente a Momentum y AMSGrad? Los resultados describen solo las condiciones ejecutadas, no establecen superioridad universal. Son funciones deterministas pequeñas, no entrenamiento de redes ni de datos reales.

## Formulación de Adam

Por coordenada, con `m0=v0=0`, contador `t` incrementado antes de corregir sesgo y epsilon fuera de la raíz:

```
g_t = ∇f(θ_t)
m_t = β1 m_(t-1) + (1-β1) g_t
v_t = β2 v_(t-1) + (1-β2) g_t²
mhat_t = m_t / (1-β1^t)
vhat_t = v_t / (1-β2^t)
θ_(t+1) = θ_t - α mhat_t / (sqrt(vhat_t) + ε)
```

Valores habituales: `α=0.01`, `β1=0.9`, `β2=0.999`, `ε=1e-8`; cada experimento declara excepciones. Momentum utiliza `u_t=βu_(t-1)+g_t`, `θ_(t+1)=θ_t-αu_t` y el código almacena `αu_t`. AMSGrad emplea el máximo acumulado del segundo momento sin corrección de sesgo y primer momento sin corregir, según la variante de Reddi et al.; no es intercambiable con variantes modernas de AdamW u otras implementaciones.

## Archivos y ejecución (Windows)

- `src/`: optimizadores, objetivos y corredor C++.
- `experiments/`: ejecutables Rosenbrock, Rastrigin y Reddi.
- `tests/optimizer_tests.cpp`: pruebas analíticas y de entradas.
- `results/csv/`: trazas y resúmenes generados por C++.
- `results/plots/`: figuras generadas desde CSV.
- `docs/`: diagnóstico, propuesta, manuscrito, slides, guion y checklist.

Requiere CMake 3.10+, C++17 y Ninja. Entorno usado: CMake 3.29, MinGW g++ 13.2, Python 3.13, pandas y matplotlib. Desde PowerShell:

```powershell
cd C:\Users\User\Desktop\8vo\Algorithms\proyect\Adam-Analysis
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build --clean-first
ctest --test-dir build --output-on-failure
New-Item -ItemType Directory -Force results\csv, results\plots | Out-Null
.\build\AdamAnalysis.exe
python plot_results.py
```

Ejecutar desde `Adam-Analysis` para las rutas relativas. También hay `build\exp_rosenbrock.exe`, `build\exp_rastrigin.exe` y `build\exp_reddi.exe`.

## Experimentos y resultados actuales

Se ejecutaron cuadrática separable 2D/10D, Rosenbrock, Rastrigin, gradiente periódico de Reddi y barrido de siete tasas de Adam en Rosenbrock. Cada comparación comparte inicio, límite y tolerancia por objetivo; las tasas se eligieron por separado, sin búsqueda con presupuesto equiparado. Los CSV incluyen iteración, pérdida, norma de gradiente, distancia al óptimo y coordenadas. `summary.csv` incluye hiperparámetros, tope, tolerancia, tiempo de una ejecución y estado.

En la corrida recién regenerada, las iteraciones hasta tolerancia fueron: cuadrática 2D (Adam 2619, Momentum 339, AMSGrad 1639), cuadrática 10D (2639, 362, 1666), Rosenbrock (4443, 3118, 16057) y Rastrigin (318, 304, 291). Tolerancias: `1e-8` en cuadráticas y `1e-6` en Rosenbrock/Rastrigin. En Rastrigin los valores finales fueron aproximadamente 7.96 para Adam/AMSGrad y 1.99 para Momentum, todos distintos del mínimo global cero. En el ejemplo de Reddi, tras 300000 pasos Adam terminó en `x≈0.9454` y AMSGrad en `x≈-0.9982`, con objetivo de referencia `-1`.

En el barrido de Rosenbrock, `α=0.0001` no alcanzó tolerancia antes del tope de 20000; para `0.0005`, `0.001`, `0.005`, `0.01`, `0.05`, `0.1` se necesitaron 15919, 11647, 6059, 4443, 2022 y 1463 pasos. Estos son resultados de un punto inicial y una cuadrícula finita. Los tiempos varían entre corridas; son de una ejecución y no sustentan afirmaciones de rendimiento.

## Complejidad

Suponiendo gradiente disponible, cada método recorre las `d` coordenadas: tiempo por actualización `O(d)`. Memoria auxiliar Momentum `O(d)` (un vector), Adam `O(d)` (dos vectores), AMSGrad `O(d)` (tres vectores). El coste de calcular el gradiente depende del objetivo y se cuenta aparte. Complejidad por paso no implica menos pasos ni convergencia universal.

## Validación y estado de entrega

CTest verifica gradientes en puntos conocidos, primeras actualizaciones, reset, argumentos inválidos, cambio de dimensiones y valores no finitos. Compilación limpia y CTest pasaron (1/1); CSV y plots se regeneraron.

1. D. P. Kingma y J. Ba, “Adam: A Method for Stochastic Optimization,” ICLR, 2015. [arXiv](https://arxiv.org/abs/1412.6980).
2. S. J. Reddi, S. Kale y S. Kumar, “On the Convergence of Adam and Beyond,” ICLR, 2018. [OpenReview](https://openreview.net/forum?id=ryQu7f-RZ).
3. B. T. Polyak, “Some methods of speeding up the convergence of iteration methods,” USSR Computational Mathematics and Mathematical Physics, 1964. [DOI](https://doi.org/10.1016/0041-5553(64)90137-5).

El manuscrito IEEE `docs/report_draft.pdf` compila en **12 paginas**, dentro del rango oficial de 12-15. Sigue siendo borrador hasta agregar los enlaces reales de Overleaf y del video; confirmar que GitHub incluya esta revision local. Ver `docs/CHECKLIST.md` y `docs/INITIAL_DIAGNOSTIC.md`.
