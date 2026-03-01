import time

# Variable global para contar los movimientos en el algoritmo recursivo
movimientos_rec = 0

def hanoi_recursivo(n, origen, destino, auxiliar):
    global movimientos_rec
    if n == 1:
        movimientos_rec += 1
        return
    hanoi_recursivo(n-1, origen, auxiliar, destino)
    movimientos_rec += 1
    hanoi_recursivo(n-1, auxiliar, destino, origen)


def hanoi_iterativo(n, origen_real, destino_real, auxiliar_real):
    # Representación física de los postes
    pilas = {
        origen_real: list(range(n, 0, -1)), 
        destino_real: [], 
        auxiliar_real: []
    }
    
    origen, destino, auxiliar = origen_real, destino_real, auxiliar_real
    if n % 2 == 0:
        destino, auxiliar = auxiliar, destino
        
    movimientos_ite = 0
    
    def mover_entre(poste1, poste2):
        nonlocal movimientos_ite
        movimientos_ite += 1
        if not pilas[poste1]:
            pilas[poste1].append(pilas[poste2].pop())
        elif not pilas[poste2]:
            pilas[poste2].append(pilas[poste1].pop())
        elif pilas[poste1][-1] < pilas[poste2][-1]:
            pilas[poste2].append(pilas[poste1].pop())
        else:
            pilas[poste1].append(pilas[poste2].pop())

    i = 1
    # El bucle corre empíricamente hasta que la pila destino tenga los n discos
    while len(pilas[destino_real]) < n:
        if i % 3 == 1:
            mover_entre(origen, destino)
        elif i % 3 == 2:
            mover_entre(origen, auxiliar)
        elif i % 3 == 0:
            mover_entre(auxiliar, destino)
        i += 1
        
    return movimientos_ite


# --- EJECUCIÓN DEL EXPERIMENTO Y COMPARACIÓN ---

tamanos_n = [10, 15, 18, 20]

print(f"{'n':<4} | {'Teoría (2^n - 1)':<18} | {'Mov. Empíricos':<15} | {'Tiempo Rec. (s)':<18} | {'Tiempo Ite. (s)'}")
print("-" * 80)

for n in tamanos_n:
    # Cálculo teórico de la clase
    movimientos_teoricos = (2**n) - 1
    
    # Prueba Recursiva
    movimientos_rec = 0 # Reiniciar contador
    inicio_rec = time.perf_counter()
    hanoi_recursivo(n, 'A', 'C', 'B')
    tiempo_rec = time.perf_counter() - inicio_rec
    
    # Prueba Iterativa
    inicio_ite = time.perf_counter()
    movs_iterativos = hanoi_iterativo(n, 'A', 'C', 'B')
    tiempo_ite = time.perf_counter() - inicio_ite
    
    print(f"{n:<4} | {movimientos_teoricos:<18} | {movimientos_rec:<15} | {tiempo_rec:<18.6f} | {tiempo_ite:.6f}")