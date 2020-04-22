#include <iostream>
#include <unordered_map>
using namespace std;

struct Node {
    int data = 0;
    Node *next = nullptr;
};

/// time complexity: O(N^2)
/// space complexity: O(1)
void removeDuplicates(Node *head) {
    if (head == nullptr || (head && (head->next == nullptr))) {
        return;
    }

    Node *curr = head;
    while (curr) {
        Node *runner = curr;
        while (runner->next != nullptr) {
            if (runner->next->data == curr->data) {
                runner->next = runner->next->next;
            } else {
                runner = runner->next;
            }
        }
        curr = curr->next;
    }
}

/// time complexity: O(N)
/// space complexity: O(N)
void removeDuplicates2(Node *head) {
    if (head == nullptr || (head && (head->next == nullptr))) {
        return;
    }

    unordered_map<int, int> nodeMap;
    Node *prev = head;
    Node *curr = head->next;
    nodeMap[head->data] = 1;

    while (curr != nullptr) {
        while (curr && nodeMap.find(curr->data) != nodeMap.end()) {
            curr = curr->next;
        }
        prev->next = curr;
        prev = curr;
        if (curr) {
            nodeMap[curr->data] = 1;
            curr = curr->next;
        }
    }
}