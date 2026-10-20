def euclid(m, n):
    """
    Calcula el máximo común divisor (MCD) de m y n 
    usando el algoritmo de Euclides.
    
    Entrada: Dos enteros m y n (no negativos, no ambos cero).
    Salida: El máximo común divisor de m y n.
    """
    # Step 1: Si n no es 0, procedemos al ciclo
    while n != 0:
        # Step 2: Dividimos m entre n y asignamos el residuo a r
        r = m % n
        
        # Step 3: Asignamos el valor de n a m y el valor de r a n
        m = n
        n = r
        
    # Step 1 (condición de salida): Si n es 0, retornamos m
    return m

# Bloque de ejecución para probar el algoritmo
if __name__ == "__main__":
    # Puedes cambiar estos valores para probar con otros números
    numero_m = 60
    numero_n = 24
    
    resultado = euclid(numero_m, numero_n)
    
    print(f"El máximo común divisor de {numero_m} y {numero_n} es: {resultado}")