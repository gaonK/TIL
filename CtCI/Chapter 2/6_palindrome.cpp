#include <iostream>
#include <stack>
using namespace std;

struct Node {
    char data;
    Node* next;
    Node(int d): data(d), next(nullptr) { }
};

/// Helper method
void reverse(Node* &head) {
    if (head == nullptr || (head && (head->next == nullptr))) {
        return;
    }

    Node* newHead = nullptr;
    Node* nextNode = nullptr;

    while (head) {
        nextNode = head->next;
        head->next = newHead;
        newHead = head;
        head = nextNode;
    }
    head = newHead;
}


/// Iteratively determine if list is palindrome using reversing list
/// time complexity: O(N)
/// space complexity: O(1)
bool isPalindromeIter1(Node* head) {
    if (head == nullptr || head->next == nullptr) {
        return true;
    }

    Node* ptr1 = head;
    Node* ptr2 = head;
    Node* middleNode = nullptr;

    while (ptr2 && ptr1 && ptr1->next) {
        ptr1 = ptr1->next->next;
        ptr2 = ptr2->next;
    }

    // skip one node if there are odd number of nodes
    if (ptr1 && ptr1->next == nullptr) {
        ptr2 = ptr2->next;
    }

    reverse(ptr2);

    middleNode = ptr2;
    ptr1 = head;

    while (ptr1 && ptr2 && ptr1->data == ptr2->data) {
        ptr1 = ptr1->next;
        ptr2 = ptr2->next;
    }

    // reverse the list again
    reverse(middleNode);

    if (ptr2 == nullptr) {
        return true;
    } else {
        return false;
    }
}

/// Iteratively determine if list is palindrome using stack
/// time complexity: O(N)
/// space complexity: O(N)
bool isPalindromeIter2(Node* head) {
    if (head == nullptr || head->next == nullptr) {
        return true;
    }

    Node* ptr1 = head;
    Node* ptr2 = head;

    stack<Node*> nodeStack;

    while (ptr2 && ptr1 && ptr1->next) {
        ptr1 = ptr1->next->next;
        nodeStack.push(ptr2);
        ptr2 = ptr2->next;
    }

    if (ptr1 && ptr1->next == nullptr) {
        ptr2 = ptr2->next;
    }

    while (!nodeStack.empty() && ptr2) {
        Node* curr = nodeStack.top();
        nodeStack.pop();
        if (curr->data != ptr2->data) {
            return false;
        }
        ptr2 = ptr2->next;
    }

    return true;
}

/// time complexity: O(N)
/// space complexity: O(N)
bool isPalindromeRecurHelper(Node* &left, Node* right) {
    if (right == nullptr) {
        return true;
    }

    bool isPalindrome = isPalindromeRecurHelper(left, right->next);
    if (!isPalindrome) {
        return false;
    }

    isPalindrome = (left->data == right->data);

    left = left->next;

    return isPalindrome
}