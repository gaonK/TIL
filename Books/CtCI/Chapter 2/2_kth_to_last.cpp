#include <iostream>

struct Node {
    int data;
    Node* next;
};

Node* kthToLastHelper(Node* head, int k, int &i) {
    if (head == nullptr) {
        return nullptr;
    }

    Node* node = kthToLastHelper(head->next, k, i);
    i += 1;

    if (i == k) {
        return head;
    }
    return node;
}

/// time complexity: O(N)
/// space complexity: O(N)
Node* kthToLastRecursive(Node* head, int k) {
    int i = 0;
    return kthToLastHelper(head, k, i);
}

/// time complexity: O(N)
/// space complexity: O(1)
Node* kthToLastIterative(Node* head, int k) {
    if (head == nullptr) {
        return head;
    }

    Node* ptr1 = head;
    Node* ptr2 = head;

    int i = 0;
    while (i < k && ptr1) {
        ptr1 = ptr1->next;
        i++;
    }

    // out of bounds
    if (i < k) {
        return nullptr;
    }

    while (ptr1 != nullptr) {
        ptr1 = ptr1->next;
        ptr2 = ptr2->next;
    }

    return ptr2;
}