#include <iostream>

using namespace std;

// Estructura basica de un nodo del arbol
struct Node {
  int data;
  Node *left;
  Node *right;

  // Constructor
  Node(int val) {
    data = val;
    left = nullptr;
    right = nullptr;
  }
};

class BinaryTree {
private:
  Node *root;

  // Metodo recursivo privado para la insercion
  Node *insertRecursive(Node *current, int value) {
    // Caso base: encontramos un espacio vacio
    if (current == nullptr) {
      return new Node(value);
    }

    // Decidimos si ir a la izquierda o a la derecha
    if (value < current->data) {
      current->left = insertRecursive(current->left, value);
    } else if (value > current->data) {
      current->right = insertRecursive(current->right, value);
    }

    // Retornamos el nodo actual para no perder las conexiones
    return current;
  }

  // Metodo recursivo para imprimir en orden (Inorder Traversal)
  void inorderRecursive(Node *current) const {
    if (current != nullptr) {
      inorderRecursive(current->left);
      cout << current->data << " ";
      inorderRecursive(current->right);
    }
  }

public:
  // Constructor del arbol
  BinaryTree() { root = nullptr; }

  // Metodo publico de insercion
  void insert(int value) { root = insertRecursive(root, value); }

  // Metodo publico para imprimir
  void printInorder() const {
    inorderRecursive(root);
    cout << endl;
  }
};

int main() {
  BinaryTree myTree;

  cout << "--- Insertando elementos en el Arbol Binario ---" << endl;
  myTree.insert(50);
  myTree.insert(30);
  myTree.insert(70);
  myTree.insert(20);
  myTree.insert(40);
  myTree.insert(60);
  myTree.insert(80);

  cout << "Arbol creado con exito." << endl;

  // En un BST, un recorrido Inorder siempre imprime los elementos ordenados
  cout << "Recorrido Inorder (debe estar ordenado de menor a mayor):" << endl;
  myTree.printInorder();

  return 0;
}