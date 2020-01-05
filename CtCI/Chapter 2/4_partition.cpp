#include <iostream>
using namespace std;

struct Node {
    int data;
    Node* next;
};

/// time complexity: O(N)
/// space complexity: O(1)
Node* partition(Node* listhead, int x) {
    Node* head = nullptr;
    Node* headInitial = nullptr;
    Node* tail = nullptr;
    Node* tailInitial = nullptr;
    Node* curr = listhead;

    while (curr != nullptr) {
        if (curr->data < x) {
            if (head == nullptr) {
                head = curr;
                headInitial = head;
            }
            head->next = curr;
            head = curr;
        } else {
            if (tail == nullptr) {
                tail = curr;
                tailInitial = tail;
            }
            tail->next = curr;
            tail = curr;
        }
        curr = curr->next;
    }
    head->next = tailInitial;
    tail->next = nullptr;
    return headInitial;
}