#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <iomanip>

using namespace std;

struct Node {
    char ch;
    double freq;
    Node *left;
    Node *right;

    Node(char c, double f, Node *l = nullptr, Node *r = nullptr)
        : ch(c), freq(f), left(l), right(r) {}
};

struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

void buildCodes(Node* root, const string& prefix, map<char, string>& codes) {
    if (!root) return;
    if (!root->left && !root->right) {
        codes[root->ch] = prefix;
        return;
    }
    buildCodes(root->left, prefix + "0", codes);
    buildCodes(root->right, prefix + "1", codes);
}

int main() {
    cout << "============================================================\n";
    cout << "          EJERCICIO 5: HUFFMAN CODING EN C++                \n";
    cout << "============================================================\n\n";

    // 5.1 Construcción del Árbol
    Node *B = new Node('B', 0.10);
    Node *D = new Node('D', 0.15);
    Node *BD = new Node('*', 0.25, B, D);

    Node *space = new Node('_', 0.15);
    Node *C = new Node('C', 0.20);
    Node *spaceC = new Node('*', 0.35, space, C);

    Node *left = new Node('*', 0.60, BD, spaceC);
    Node *A = new Node('A', 0.40);
    Node *root = new Node('*', 1.00, left, A);

    map<char, string> codes;
    buildCodes(root, "", codes);

    cout << "--- 5.1 CODIGOS OBTENIDOS ---\n";
    for (auto const& [sym, code] : codes) {
        cout << "Simbolo '" << sym << "' -> Codigo: " << code << " (" << code.length() << " bits)\n";
    }
    cout << "\n";

    // 5.2 Codificar ABACABAD
    string to_encode = "ABACABAD";
    string encoded = "";
    for (char c : to_encode) {
        encoded += codes[c];
    }
    cout << "--- 5.2 CODIFICACION DE " << to_encode << " ---\n";
    cout << "Resultado codificado: " << encoded << " (Total bits: " << encoded.length() << ")\n\n";

    // 5.3 Decodificar 100010111001010
    string bitstream = "100010111001010";
    cout << "--- 5.3 DECODIFICACION DE " << bitstream << " ---\n";
    Node *curr = root;
    string decoded = "";
    for (char bit : bitstream) {
        curr = (bit == '0') ? curr->left : curr->right;
        if (!curr->left && !curr->right) {
            decoded += curr->ch;
            curr = root;
        }
    }
    cout << "Resultado decodificado: \"" << decoded << "\"\n";
    cout << "============================================================\n";

    return 0;
}
