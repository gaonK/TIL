#include <iostream>
#include <cmath>
using namespace std;

struct Node {
    int data;
    Node* next;
};

/// Helper method
int listLen(Node* head) {
    int count = 0;
    while (head) {
        head = head->next;
        count++;
    }
    return count;
}

/// time complexity: O(N)
/// space complexity: O(1)
Node* intersectionPoint(Node* head1, Node* head2) {
    int len1 = listLen(head1);
    int len2 = listLen(head2);

    Node* ptr1 = (len1 > len2) ? head1 : head2;
    Node* ptr2 = (len1 > len2) ? head2 : head1;

    int i = 0;
    while (i < abs(len1 - len2) && ptr1) {
        ptr1 = ptr1->next;
        i++;
    }

    while (ptr1 && ptr2) {
        if (ptr1 == ptr2) {
            return ptr1;
        }
        ptr1 = ptr1->next;
        ptr2 = ptr2->next;
    }
    
    return nullptr;
}