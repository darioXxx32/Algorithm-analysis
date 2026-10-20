# Guion de presentación — 10 minutos

Borrador para convertir a diapositivas PDF. Mantener 9 láminas, 10 minutos, con una figura por resultado. No leer ecuaciones extensas.

## 1. Adam y pregunta (0:45)
**En pantalla:** título, autor, pregunta central.
**Notas:** “Analizo cómo Adam minimiza objetivos de prueba y qué diferencias observamos frente a Momentum y AMSGrad. Las conclusiones se limitan a las configuraciones ejecutadas.”

## 2. Por qué importa (0:55)
**En pantalla:** problema de optimización, gradiente, parámetros.
**Notas:** “Un optimizador decide cómo mover parámetros usando derivadas. La tasa global fija puede ser sensible a escalas distintas; Adam adapta el tamaño por coordenada usando historia de gradientes.”

## 3. Intuición de Adam (1:20)
**En pantalla:** flujo gradiente → primer momento → segundo momento → corrección de sesgo → paso.
**Notas:** “El primer momento suaviza la dirección. El segundo momento escala según magnitud reciente. Como ambos estados parten de cero, se corrigen al inicio. En esta implementación epsilon está después de la raíz, sumado al denominador. Actualización elemento a elemento.”

## 4. Implementación C++ y validación (1:00)
**En pantalla:** vector de parámetros y estados `m`, `v`; resultados de CTest.
**Notas:** “Los vectores son de dimensión configurable. Las funciones producen valor y gradiente; validamos dimensiones, hiperparámetros y finitud numérica. Los tests verifican gradientes en puntos conocidos, primeros pasos y errores de entrada.”

## 5. Diseño experimental (1:00)
**En pantalla:** tabla pequeña con funciones, inicio, tolerancia y métodos.
**Notas:** “Cuadrática 2D/10D, Rosenbrock, Rastrigin y secuencia de Reddi. Dentro de cada función usamos mismos puntos iniciales, topes y criterios; las tasas de los métodos se eligieron por separado y no hubo búsqueda equiparada.”

## 6. Cuadrática y Rosenbrock (1:20)
**En pantalla:** pérdida o gradiente frente a iteraciones; mostrar CSV/plots generados.
**Notas:** “Todos llegaron a las tolerancias configuradas. En cuadrática 2D: Adam 2619, Momentum 339, AMSGrad 1639. En Rosenbrock: 4443, 3118 y 16057. Esos conteos describen estos hiperparámetros; no son un ranking universal.”

## 7. Rastrigin y Reddi (1:25)
**En pantalla:** trayectoria/valor final de Rastrigin y gráfica Reddi Adam vs AMSGrad.
**Notas:** “Rastrigin alcanza gradiente pequeño en mínimos locales; los objetivos finales fueron aproximadamente 7.96 para Adam/AMSGrad y 1.99 para Momentum, lejos del mínimo global cero. En el ejemplo periódico, Adam terminó cerca de 0.945 y AMSGrad cerca de -0.998 frente a -1. La segunda es una construcción concreta, no una garantía general.”

## 8. Complejidad y límites (1:00)
**En pantalla:** tabla: coste update O(d); memoria Momentum O(d), Adam O(d), AMSGrad O(d).
**Notas:** “Si el gradiente ya se calculó, los tres recorren cada coordenada una vez por paso. El cálculo del gradiente es coste adicional dependiente del objetivo. El coste por paso no significa convergencia en menos pasos. Los tiempos actuales son una sola ejecución y no son benchmarking estadístico.”

## 9. Conclusión y siguientes pasos (1:15)
**En pantalla:** tres conclusiones acotadas, limitaciones y “gracias”.
**Notas:** “La implementación C++ y el análisis de funciones sintéticas están reproducibles. Gradiente pequeño no implica mínimo global en objetivos no convexos. Falta ampliar el artículo a 12–15 páginas con análisis sustantivo, inspección final de formato, enlaces reales de Overleaf/GitHub/video y grabar la presentación. Gracias.”

**Suma estimada:** 10:00. Ensayar con cronómetro; si excede, acortar la explicación matemática y conservar tiempo para resultados y límites.
