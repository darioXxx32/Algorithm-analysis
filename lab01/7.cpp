#include <iostream>
#include <list>
#include <stdexcept>
#include <string>


using namespace std;

template <typename T> class MyQueue {
private:
  // La lista se encapsula como un atributo privado
  list<T> elements;

public:
  // Verifica si la cola está vacía
  bool isEmpty() const { return elements.empty(); }

  // Devuelve el número de elementos en la cola
  size_t size() const { return elements.size(); }

  // Inserta un elemento al final de la cola (FIFO)
  void push(const T &value) { elements.push_back(value); }

  // Elimina el elemento al frente de la cola
  void pop() {
    if (isEmpty()) {
      throw out_of_range("Error: No se puede hacer pop, la cola esta vacia.");
    }
    elements.pop_front();
  }

  // Retorna el valor del elemento al frente sin eliminarlo
  T front() const {
    if (isEmpty()) {
      throw out_of_range("Error: La cola esta vacia, no hay frente.");
    }
    return elements.front();
  }
};

int main() {
  // 1. Probando el Queue con enteros
  MyQueue<int> intQueue;
  intQueue.push(10);
  intQueue.push(20);
  intQueue.push(30);

  cout << "--- MyQueue con Enteros ---" << endl;
  cout << "El frente actual es: " << intQueue.front() << endl; // Debe ser 10
  cout << "Tamanio de la cola: " << intQueue.size() << endl;   // Debe ser 3

  intQueue.pop(); // Saca el 10
  cout << "Hicimos un pop. Nuevo frente: " << intQueue.front()
       << endl; // Debe ser 20

  // 2. Probando el Queue con Strings
  MyQueue<string> stringQueue;
  stringQueue.push("Primer cliente");
  stringQueue.push("Segundo cliente");

  cout << "\n--- MyQueue con Strings ---" << endl;
  cout << "El frente actual es: " << stringQueue.front()
       << endl; // Debe ser "Primer cliente"

  return 0;
}