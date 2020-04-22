#include <iostream>
using namespace std;

struct Node {
    int data;
    Node* next;
    Node(int d): data(d), next(nullptr) { }
};

/// time complexity: O(N)
/// space complexity: O(N)
Node* add_iterative(Node* list1, Node* list2) {
    if (list1 == nullptr) {
        return list2;
    }

    if (list2 == nullptr) {
        return list1;
    }

    Node* list3 = nullptr;
    Node* list3Tail = nullptr;

    int value = 0, carry = 0;

    while (list1 || list2) {
        value = carry + (list1 ? list1->data : 0) + (list2 ? list2->data : 0);

        carry = value / 10;
        value = value % 10;

        Node* temp = new Node(value);
        if (list3 == nullptr) {
            list3 = temp;
        } else {
            list3Tail->next = temp;
        }

        list3Tail = temp;

        if (list1) {
            list1 = list1->next;
        }

        if (list2) {
            list2 = list2->next;
        }
    }

    if (carry > 0) {
        list3Tail->next = new Node(carry);
    }

    return list3;
}