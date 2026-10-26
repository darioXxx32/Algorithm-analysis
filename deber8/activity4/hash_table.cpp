/*
 * Activity 4: Hash Table with Closed Hashing (Open Addressing)
 * Language: C++  (no dictionaries or standard hash libraries)
 *
 * Collision resolution: Linear Probing
 *   probe sequence: h(k, i) = (h(k) + i) mod TABLE_SIZE
 *
 * Hash function: polynomial rolling hash mod TABLE_SIZE
 *
 * Operations supported:
 *   insert(key, value)
 *   search(key)  -> returns pointer to value or nullptr
 *   remove(key)
 *   display()
 */

#include <iostream>
#include <string>

// ──────────────────────────────────────────────────────────────────────────────
// Configuration
// ──────────────────────────────────────────────────────────────────────────────
static const int TABLE_SIZE = 17;   // prime number → better distribution

// ──────────────────────────────────────────────────────────────────────────────
// Slot states
// ──────────────────────────────────────────────────────────────────────────────
enum SlotState { EMPTY, OCCUPIED, DELETED };

// ──────────────────────────────────────────────────────────────────────────────
// Slot (bucket)
// ──────────────────────────────────────────────────────────────────────────────
struct Slot {
    std::string key;
    int         value;
    SlotState   state;

    Slot() : key(""), value(0), state(EMPTY) {}
};

// ──────────────────────────────────────────────────────────────────────────────
// HashTable class
// ──────────────────────────────────────────────────────────────────────────────
class HashTable {
private:
    Slot table[TABLE_SIZE];
    int  count;          // number of OCCUPIED slots

    /* Polynomial rolling hash */
    int hash(const std::string &key) const {
        const int BASE = 31;
        long long h = 0;
        for (char c : key) {
            h = (h * BASE + (c - 'a' + 1)) % TABLE_SIZE;
        }
        return static_cast<int>((h + TABLE_SIZE) % TABLE_SIZE);
    }

    /* Linear probe index */
    int probe(int h0, int i) const {
        return (h0 + i) % TABLE_SIZE;
    }

public:
    HashTable() : count(0) {}

    // ── insert ──────────────────────────────────────────────────────────────
    bool insert(const std::string &key, int value) {
        if (count >= TABLE_SIZE) {
            std::cout << "[insert] Table is full!\n";
            return false;
        }
        int h0 = hash(key);
        for (int i = 0; i < TABLE_SIZE; i++) {
            int idx = probe(h0, i);
            if (table[idx].state == OCCUPIED && table[idx].key == key) {
                // Key already exists → update value
                table[idx].value = value;
                std::cout << "[insert] Updated  key=\"" << key
                          << "\" value=" << value
                          << " at slot " << idx << "\n";
                return true;
            }
            if (table[idx].state != OCCUPIED) {
                table[idx].key   = key;
                table[idx].value = value;
                table[idx].state = OCCUPIED;
                count++;
                std::cout << "[insert] Inserted key=\"" << key
                          << "\" value=" << value
                          << " at slot " << idx
                          << " (probes=" << i + 1 << ")\n";
                return true;
            }
        }
        std::cout << "[insert] Could not insert \"" << key << "\" (all probes exhausted)\n";
        return false;
    }

    // ── search ──────────────────────────────────────────────────────────────
    int* search(const std::string &key) {
        int h0 = hash(key);
        for (int i = 0; i < TABLE_SIZE; i++) {
            int idx = probe(h0, i);
            if (table[idx].state == EMPTY) return nullptr;   // guaranteed not present
            if (table[idx].state == OCCUPIED && table[idx].key == key)
                return &table[idx].value;
        }
        return nullptr;
    }

    // ── remove ──────────────────────────────────────────────────────────────
    bool remove(const std::string &key) {
        int h0 = hash(key);
        for (int i = 0; i < TABLE_SIZE; i++) {
            int idx = probe(h0, i);
            if (table[idx].state == EMPTY) return false;
            if (table[idx].state == OCCUPIED && table[idx].key == key) {
                table[idx].state = DELETED;   // tombstone
                count--;
                std::cout << "[remove] Removed  key=\"" << key
                          << "\" from slot " << idx << "\n";
                return true;
            }
        }
        return false;
    }

    // ── display ─────────────────────────────────────────────────────────────
    void display() const {
        // Pure ASCII table (avoids encoding issues on Windows consoles)
        printf("\n+------+--------------------+----------+----------+\n");
        printf("| Slot | Key                |  Value   |  State   |\n");
        printf("+------+--------------------+----------+----------+\n");
        for (int i = 0; i < TABLE_SIZE; i++) {
            const char *state_str =
                table[i].state == EMPTY    ? "EMPTY    " :
                table[i].state == OCCUPIED ? "OCCUPIED " : "DELETED  ";
            if (table[i].state == OCCUPIED) {
                printf("| %-4d | %-18s | %-8d | %s|\n",
                       i,
                       table[i].key.substr(0, 18).c_str(),
                       table[i].value,
                       state_str);
            } else {
                printf("| %-4d | %-18s | %-8s | %s|\n",
                       i, "", "", state_str);
            }
        }
        printf("+------+--------------------+----------+----------+\n");
        printf("Occupied slots: %d / %d\n\n", count, TABLE_SIZE);
    }

    // ── load factor ─────────────────────────────────────────────────────────
    double loadFactor() const {
        return static_cast<double>(count) / TABLE_SIZE;
    }
};

// ──────────────────────────────────────────────────────────────────────────────
// main – demonstration & tests
// ──────────────────────────────────────────────────────────────────────────────
int main() {
    std::cout << "=== Hash Table: Closed Hashing with Linear Probing ===\n";
    std::cout << "Table size: " << TABLE_SIZE << " (prime)\n\n";

    HashTable ht;

    // Insert entries
    std::cout << "--- Inserting entries ---\n";
    ht.insert("alice",   85);
    ht.insert("bob",     92);
    ht.insert("charlie", 78);
    ht.insert("diana",   95);
    ht.insert("eve",     88);
    ht.insert("frank",   70);
    ht.insert("grace",   91);
    ht.insert("henry",   66);
    ht.insert("iris",    83);
    ht.insert("jack",    77);

    ht.display();

    // Search
    std::cout << "--- Searching ---\n";
    std::string keys[] = {"diana", "bob", "zara"};
    for (const auto &k : keys) {
        int *v = ht.search(k);
        if (v) std::cout << "Found  \"" << k << "\" -> " << *v << "\n";
        else   std::cout << "\"" << k << "\" NOT found\n";
    }
    std::cout << "\n";

    // Update existing key
    std::cout << "--- Updating \"bob\" -> 100 ---\n";
    ht.insert("bob", 100);
    std::cout << "\n";

    // Remove
    std::cout << "--- Removing \"charlie\" and \"eve\" ---\n";
    ht.remove("charlie");
    ht.remove("eve");
    ht.remove("nobody");   // key that doesn't exist
    std::cout << "\n";

    ht.display();

    // Re-insert after deletion (tombstone must not block re-use)
    std::cout << "--- Re-inserting \"charlie\" with new value ---\n";
    ht.insert("charlie", 99);
    std::cout << "\n";

    // Collision demo
    std::cout << "--- Collision stress test ---\n";
    const char *extra[] = {"aaa","bbb","ccc","ddd","eee","fff"};
    for (auto k : extra) ht.insert(k, 0);

    ht.display();
    std::cout << "Load factor: " << ht.loadFactor() << "\n";

    return 0;
}
