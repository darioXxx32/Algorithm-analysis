#include <iostream>
#include <list>
#include <stdexcept> // Para manejar excepciones
#include <string>

using namespace std;

template <typename T> class MyStack {
private:
  // La lista se encapsula como un atributo privado
  list<T> elements;

public:
  // Verifica si la pila está vacía
  bool isEmpty() const { return elements.empty(); }

  // Devuelve el número de elementos en la pila
  size_t size() const { return elements.size(); }

  // Inserta un elemento en el tope de la pila (LIFO)
  void push(const T &value) { elements.push_back(value); }

  // Elimina el elemento en el tope de la pila
  void pop() {
    if (isEmpty()) {
      throw out_of_range("Error: No se puede hacer pop, la pila está vacía.");
    }
    elements.pop_back();
  }

  // Retorna el valor del elemento en el tope sin eliminarlo
  T top() const {
    if (isEmpty()) {
      throw out_of_range("Error: La pila está vacía, no hay tope.");
    }
    return elements.back();
  }
};

int main() {
  // 1. Probando el Stack con enteros
  cout << "--- MyStack con Enteros ---" << endl;
  MyStack<int> intStack;

  intStack.push(10);
  intStack.push(20);
  intStack.push(30);

  cout << "El tope actual es: " << intStack.top() << endl; // Debe ser 30
  cout << "Tamanio del stack: " << intStack.size() << endl;

  intStack.pop();
  cout << "Hicimos un pop. Nuevo tope: " << intStack.top()
       << endl; // Debe ser 20

  // 2. Probando el Stack con Strings para demostrar el uso del Template
  cout << "\n--- MyStack con Strings ---" << endl;
  MyStack<string> stringStack;

  stringStack.push("Primer elemento");
  stringStack.push("Segundo elemento");

  cout << "El tope actual es: " << stringStack.top() << endl;

  return 0;
}