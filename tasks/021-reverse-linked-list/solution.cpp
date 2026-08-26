#include <cassert>
#include <iostream>

struct Node {
    int value;
    Node* next;
};

Node* reverseList(Node* head) {

    Node* rl = nullptr;
    Node* next = nullptr;

    while (head != nullptr) {
        next = head->next;
        head->next = rl;
        rl = head;
        head = next;
        std::cout << " ->" << rl->value << std::endl;
    }

    return rl;
}

void printList(Node* head) {
    while (head != nullptr) {
        std::cout << " ->" << head->value << std::endl;
        head = head->next;
    }
}

int main()
{
    assert(reverseList(nullptr) == nullptr);

    Node single{42, nullptr};
    assert(reverseList(&single) == &single);
    assert(single.next == nullptr);

    Node d{4, nullptr};
    Node c{3, &d};
    Node b{2, &c};
    Node a{1, &b};

    Node* head = reverseList(&a);
    assert(head == &d);
    assert(d.next == &c);
    assert(c.next == &b);
    assert(b.next == &a);
    assert(a.next == nullptr);

    printList(head);


    Node b1{200, nullptr};
    Node a1{100, &b1};
    Node* head1 = reverseList(&a1);
    assert(a1.next == nullptr);
    printList(head1);

    std::cout << "all tests passed" << std::endl;

    return 0;
}
