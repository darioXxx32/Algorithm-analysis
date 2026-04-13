#include <iostream>
using namespace std;

class HashTable {
private:
    static const int SIZE = 10;

    struct Node {
        int key;
        int status; 
        // 0 = empty
        // 1 = occupied
        // 2 = deleted
    };

    Node table[SIZE];

    int hashFunction(int key) {
        return key % SIZE;
    }

public:
    HashTable() {
        for (int i = 0; i < SIZE; i++) {
            table[i].key = 0;
            table[i].status = 0; // empty
        }
    }

    void insert(int key) {
        int index = hashFunction(key);
        int startIndex = index;

        while (table[index].status == 1) {
            if (table[index].key == key) {
                cout << "Key " << key << " already exists at index " << index << endl;
                return;
            }
            index = (index + 1) % SIZE;

            if (index == startIndex) {
                cout << "Hash table is full. Cannot insert " << key << endl;
                return;
            }
        }

        table[index].key = key;
        table[index].status = 1;
        cout << "Inserted " << key << " at index " << index << endl;
    }

    bool search(int key) {
        int index = hashFunction(key);
        int startIndex = index;

        while (table[index].status != 0) {
            if (table[index].status == 1 && table[index].key == key) {
                return true;
            }

            index = (index + 1) % SIZE;

            if (index == startIndex) {
                break;
            }
        }

        return false;
    }

    void remove(int key) {
        int index = hashFunction(key);
        int startIndex = index;

        while (table[index].status != 0) {
            if (table[index].status == 1 && table[index].key == key) {
                table[index].status = 2; // deleted
                cout << "Key " << key << " deleted from index " << index << endl;
                return;
            }

            index = (index + 1) % SIZE;

            if (index == startIndex) {
                break;
            }
        }

        cout << "Key " << key << " not found, cannot delete." << endl;
    }

    void display() {
        cout << "\nHash Table:\n";
        for (int i = 0; i < SIZE; i++) {
            cout << i << ": ";
            if (table[i].status == 1) {
                cout << table[i].key;
            } else if (table[i].status == 2) {
                cout << "[deleted]";
            } else {
                cout << "[empty]";
            }
            cout << endl;
        }
    }
};

int main() {
    HashTable ht;

    ht.insert(23);
    ht.insert(43);
    ht.insert(13);
    ht.insert(27);
    ht.insert(33);

    ht.display();

    cout << "\nSearching values:\n";
    cout << "23 -> " << (ht.search(23) ? "Found" : "Not Found") << endl;
    cout << "50 -> " << (ht.search(50) ? "Found" : "Not Found") << endl;

    cout << "\nDeleting 43...\n";
    ht.remove(43);

    ht.display();

    cout << "\nSearching 43 again:\n";
    cout << "43 -> " << (ht.search(43) ? "Found" : "Not Found") << endl;

    return 0;
}