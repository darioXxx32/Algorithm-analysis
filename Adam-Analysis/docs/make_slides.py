import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages, PdfFile
from pathlib import Path

root = Path(__file__).parent
plots = root / 'results' / 'plots'
out = root / 'presentation_slides.pdf'
slides = [
    ('Adam: momentos adaptativos y convergencia', 'Analysis of Algorithms · Presentation I\nDario Pomasqui\n\nPregunta: ¿cómo se comporta Adam en objetivos de prueba frente a Momentum y AMSGrad?', None),
    ('Motivación y alcance', 'Adam combina suavizado del gradiente con escalado adaptativo por coordenada.\n\nEstudio reproducible en C++17 con funciones analíticas deterministas.\n\nAlcance: describir configuraciones ejecutadas; no afirmar superioridad universal.', None),
    ('Cómo actualiza Adam', 'm_t = β₁m_(t−1) + (1−β₁)g_t\nv_t = β₂v_(t−1) + (1−β₂)g_t²\n\nCorrección de sesgo: m̂_t=m_t/(1−β₁ᵗ), v̂_t=v_t/(1−β₂ᵗ)\n\nθ_(t+1) = θ_t − α m̂_t /(√v̂_t + ε)\n\nEpsilon fuera de la raíz; operaciones elemento a elemento.', None),
    ('Implementación C++ y comprobaciones', 'Vectores de parámetros, primer momento y segundo momento.\n\nAdam, Momentum y AMSGrad comparten funciones objetivo y registro.\n\nValidación: parámetros, dimensiones, finitud numérica, ecuaciones en puntos conocidos.\n\nCTest: 1/1 pasa.', None),
    ('Diseño experimental', 'Cuadrática: 2D y 10D\nRosenbrock: inicio (−1.5, 2), mínimo (1,1)\nRastrigin: inicio (2.5,2.5), mínimo global (0,0)\nReddi: secuencia periódica de gradientes\n\nMismos inicios, topes y tolerancia dentro de cada función. Tasas declaradas, no afinadas con presupuesto igual.', None),
    ('Cuadrática y Rosenbrock', 'Iteraciones hasta la tolerancia del gradiente\n\nCuadrática 2D (1e−8): Adam 2619 · Momentum 339 · AMSGrad 1639\nRosenbrock (1e−6): Adam 4443 · Momentum 3118 · AMSGrad 16057\n\nCada resultado depende de la tasa configurada.', plots/'rosenbrock_comparison.png'),
    ('Rastrigin: estacionario no significa global', 'Rastrigin desde (2.5,2.5): gradiente pequeño para todos.\n\nObjetivo final: Adam ≈ 7.96 · Momentum ≈ 1.99 · AMSGrad ≈ 7.96\n\nEl mínimo global conocido vale 0; los tres quedaron en mínimos locales.', plots/'rastrigin_comparison.png'),
    ('Ejemplo periódico de Reddi', 'Después de 300,000 pasos: Adam x≈0.9454; AMSGrad x≈−0.9982.\n\nÓptimo de referencia: x=−1.\n\nEvidencia de distinta conducta en esta construcción e hiperparámetros; no garantía general.', plots/'reddi_counterexample.png'),
    ('Complejidad y limitaciones', 'Con gradiente disponible: tiempo por actualización O(d).\nMemoria auxiliar: Momentum O(d), Adam O(d), AMSGrad O(d).\n\nEl cálculo del gradiente cuesta aparte. Tiempos medidos una sola vez; no son benchmark estadístico. Funciones sintéticas, un inicio por caso.', None),
    ('Conclusiones y siguientes pasos', 'La implementación vectorial en C++ y la batería son reproducibles.\n\nGradiente pequeño puede dejar un mínimo local. El ejemplo de Reddi separa las variantes probadas.\n\nEl artículo actual es borrador de 4 páginas: faltan 12–15 páginas finales, enlaces reales y grabación.', None),
]
pdf = PdfPages(out, metadata={'Title': 'Adam Analysis Presentation I', 'Subject': 'Nine-slide oral presentation'})
with pdf:
    for title, body, image in slides:
        fig = plt.figure(figsize=(13.333, 7.5), facecolor='#101827')
        fig.text(.07, .88, title, color='white', fontsize=27, weight='bold', va='top')
        fig.text(.08, .72, body, color='#d8e2f0', fontsize=17, va='top', linespacing=1.5)
        if image and image.exists():
            ax = fig.add_axes([.53, .12, .42, .5])
            ax.imshow(plt.imread(image)); ax.axis('off')
        fig.text(.94, .04, 'ADAM · PRESENTATION I', color='#8292a8', fontsize=9, ha='right')
        pdf.savefig(fig, facecolor=fig.get_facecolor()); plt.close(fig)

print(out)






