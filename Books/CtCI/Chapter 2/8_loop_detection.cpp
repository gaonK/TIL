#include <iostream>
using namespace std;

struct Node {
    int data;
    Node* next;
};

Node* detect(Node* head) {
    if (head == nullptr) {
        return nullptr;
    }

    Node* fastPtr = head;
    Node* slowPtr = head;

    while (slowPtr && fastPtr && fastPtr->next) {
        fastPtr = fastPtr->next->next;
        slowPtr = slowPtr->next;

        if (fastPtr == slowPtr) {
            break;
        }
    }

    // no cycle
    if (fastPtr == nullptr || fastPtr->next == nullptr) {
        return nullptr;
    }

    slowPtr = head;
    while (slowPtr != fastPtr) {
        slowPtr = slowPtr->next;
        fastPtr = fastPtr->next;
    }

    return fastPtr;
}