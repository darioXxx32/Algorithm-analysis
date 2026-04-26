#include <iostream>
using namespace std;
int fibonacci(int n){
    //aca ponemos los casos bases 
    if (n == 0) return 0;
    if (n == 1) return 1;
    //recursivamente llamamos a la funcion para calcular el fibonacci de n-1 y n-2
    return fibonacci(n-1) + fibonacci(n-2);
}
int main() {
    int n;
    cout <<"ingresa n: ";
    cin >> n;
    cout <<"fibonacci(" << n << ") = " << fibonacci(n) << endl;
    return 0;  
}