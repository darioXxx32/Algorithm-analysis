//dario pomasqui
#include <iostream>
using namespace std;

int main() {
    int n;
    
    
    cout << "Ingresa la cantidad de terminos de Fibonacci que deseas ver: ";
    cin >> n;

    // Definimos los valores iniciales (casos base)
    long long t0 = 0, t1 = 1, siguiente;

    cout << "Secuencia generada: " << endl;

    for (int i = 0; i < n; i++) {
        if (i <= 1) {
            // Los dos primeros términos son los valores iniciales
            siguiente = i;
        } else {
            // Aplicamos la recurrencia: T(n) = T(n-1) + T(n-2)
            siguiente = t0 + t1;
            t0 = t1;        // El anterior ahora es el penúltimo
            t1 = siguiente; // El actual ahora es el último
        }
        cout << siguiente << " ";
    }

    cout << endl;
    return 0;
}