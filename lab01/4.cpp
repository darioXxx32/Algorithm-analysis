#include <iostream>
#include <list>

using namespace std;

int main() {
  list<int> stack_list;

  // Push elements onto the list to simulate a stack
  cout << "Pushing elements: 10, 20, 30" << endl;
  stack_list.push_back(10);
  stack_list.push_back(20);
  stack_list.push_back(30);

  // Pop elements from the list
  cout << "\nPopping elements from the stack:" << endl;
  while (!stack_list.empty()) {
    // Access the "top" element
    cout << "Top element: " << stack_list.back() << endl;
    // Remove the "top" element
    stack_list.pop_back();
  }

  return 0;
}