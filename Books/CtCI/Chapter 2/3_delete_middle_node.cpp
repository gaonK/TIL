#include <iostream>
using namespace std;

struct Node {
    char data;
    Node* next;
};

/// time complexity: O(1)
/// space complexity: O(1)
void deleteNode(Node* node) {
    if (node == nullptr || node->next == nullptr) {
        return;
    }

    Node* nextNode = node->next;
    node->data = nextNode->data;
    node->next = nextNode->next;

    delete nextNode;
}