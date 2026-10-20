# Diagnostic initial — Presentation I

## Estado encontrado

- Raíz: `generate_project.py`, `Project - Presentation I.pdf`, `proporsal.pdf`, logos e indicación VS Code.
- `generate_project.py` escribe a una ruta absoluta de otra carpeta (`Desktop\algo\Adam-Analysis`); no coincide con el workspace actual y no es necesario para compilar los archivos presentes.
- `proporsal.pdf` contiene 7 páginas, plantea una implementación Python/PyTorch y benchmarks con datasets/redes. Se conservó intacta como original; es la propuesta señalada por el profesor y no el informe final.
- `Adam-Analysis/` ya tenía optimizadores C++, funciones, experimentos, resultados CSV y plots, además de build Ninja preexistente. El README inicial solo describía carpetas.
- Código previo implementaba Adam vectorial con bias correction, pero no validaba hiperparámetros; actualizaciones no finitas se omitían sin avisar. Había Momentum y AMSGrad, con AMSGrad usando primer/segundo momentos sin corrección de sesgo.
- CSV ya contenía resultados, incluyendo Reddi; la batería se recompiló y se volvió a ejecutar después de añadir validaciones. Los timings son una corrida.

## Requisitos oficiales comprobados

`Project - Presentation I.pdf` indica: informe de 12–15 páginas IEEE Conference; presentación oral de 10 minutos grabada; entregar a Moodle PDF del informe y PDF de slides; anexo del informe con enlaces a Overleaf, GitHub y video. Los elementos mínimos son nombre/importancia, problema que resuelve, explicación, detalles de implementación C++, resultados y conclusiones. Recomienda revisar el calendario de curso para fecha, no fija una fecha concreta en el PDF.

## Hallazgos y acciones

La propuesta se separó del informe de Presentation I: se creó una propuesta corregida de una página y se dejó intacta la original. Se validaron referencias originales/académicas de Adam (Kingma–Ba), AMSGrad (Reddi–Kale–Kumar) y momentum (Polyak). Se documentó la formulación AMSGrad usada, sin presentarla como universal.

El informe generado compila en IEEEtran, pero tiene cuatro páginas; se marca como borrador y no conforme con extensión. La presentación tiene nueve láminas y guion con tiempos para 10 minutos. Overleaf, GitHub y video no existen aún como enlaces reales, así que quedan pendientes explícitos.
