import time

# Configuración de los parámetros según los consejos de tu diapositiva
m = 2**32                      # Un número muy grande
seed = int(time.time())        # La hora actual como semilla
a = int(0.05 * m)              # Un multiplicador grande
b = 1                          # El incremento usual





def linear_congruential_method(n, m, seed, a, b):
    secuencia = []
    r_prev = seed
    
    for _ in range(n):
        # La operación básica: multiplicación, suma y residuo (módulo)
        r_new = (a * r_prev + b) % m
        secuencia.append(r_new)
        r_prev = r_new
        
    return secuencia


# Tamaños de prueba (n): 1 millón, 2 millones, 4 millones, etc.
tamanos_n = [1000000, 2000000, 4000000, 8000000]

print(f"{'Cantidad (n)':<15} | {'Tiempo (segundos)'}")
print("-" * 35)

for n in tamanos_n:
    inicio = time.perf_counter()
    linear_congruential_method(n, m, seed, a, b)
    fin = time.perf_counter()
    
    print(f"{n:<15} | {fin - inicio:.6f}")

