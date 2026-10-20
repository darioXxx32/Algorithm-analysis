//dario pomasqui
#include <iostream>
using namespace std;

// Función recursiva
int fibonacci(int n) {
    if (n == 0) return 0; // Caso base 1
    if (n == 1) return 1; // Caso base 2
    
    // T(n) = T(n-1) + T(n-2)
    return fibonacci(n - 1) + fibonacci(n - 2); 
}

int main() {
    int limite;
    
    cout << "Cuantos numeros de la serie quieres ver? ";
    cin >> limite;

    cout << "Secuencia (Metodo Recursivo):" << endl;
    
    // Generamos
    for (int i = 0; i < limite; i++) {
        cout << fibonacci(i) << " ";
    }

    cout << endl;
    return 0;
}