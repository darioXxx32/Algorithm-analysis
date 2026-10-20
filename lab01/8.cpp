#include <deque>
#include <iostream>
#include <queue>
#include <stack>
#include <string>


using namespace std;

void ejemploStack() {
  cout << "--- 1. std::stack (Pila LIFO) ---" << endl;
  stack<string> myStack;

  myStack.push("Primer plato");
  myStack.push("Segundo plato");
  myStack.push("Tercer plato");

  cout << "El tope de la pila es: " << myStack.top() << endl;
  cout << "Tamanio de la pila: " << myStack.size() << endl;

  while (!myStack.empty()) {
    cout << "Sacando: " << myStack.top() << endl;
    myStack.pop();
  }
  cout << endl;
}

void ejemploQueue() {
  cout << "--- 2. std::queue (Cola FIFO) ---" << endl;
  queue<string> myQueue;

  myQueue.push("Cliente 1");
  myQueue.push("Cliente 2");
  myQueue.push("Cliente 3");

  cout << "Atendiendo primero a: " << myQueue.front() << endl;
  cout << "El ultimo en la fila es: " << myQueue.back() << endl;

  while (!myQueue.empty()) {
    cout << "Atendiendo a " << myQueue.front() << " y retirandolo." << endl;
    myQueue.pop();
  }
  cout << endl;
}

void ejemploPriorityQueue() {
  cout << "--- 3. std::priority_queue (Cola de Prioridad) ---" << endl;
  priority_queue<int> pQueue;

  pQueue.push(10);
  pQueue.push(100);
  pQueue.push(50);

  cout << "Elementos en orden de prioridad:" << endl;
  while (!pQueue.empty()) {
    cout << pQueue.top() << " ";
    pQueue.pop();
  }
  cout << "\n\n";
}

void ejemploDeque() {
  cout << "--- 4. std::deque (Double-Ended Queue) ---" << endl;
  deque<int> d;

  d.push_back(20);
  d.push_front(10);
  d.push_back(30);

  cout << "Frente: " << d.front() << ", Atras: " << d.back() << endl;

  d.pop_front();
  d.pop_back();

  cout << "Queda: " << d.front() << endl;
  cout << endl;
}

int main() {
  // Llamamos a cada una de las funciones demostrativas
  ejemploStack();
  ejemploQueue();
  ejemploPriorityQueue();
  ejemploDeque();

  return 0;
}