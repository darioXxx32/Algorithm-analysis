#include <iostream>
#include <list>

using namespace std;

int main() {
  list<int> queue_list;

  // Enqueue elements onto the list to simulate a queue
  cout << "Enqueueing elements: 10, 20, 30" << endl;
  queue_list.push_back(10);
  queue_list.push_back(20);
  queue_list.push_back(30);

  // Dequeue elements from the list
  cout << "\nDequeueing elements from the queue:" << endl;
  while (!queue_list.empty()) {
    // Access the "front" element
    cout << "Front element: " << queue_list.front() << endl;
    // Remove the "front" element
    queue_list.pop_front();
  }

  return 0;
}