#include <cassert>

struct Node {
    int value;
    Node* next;
};

Node* reverseList(Node* head);

int main()
{
    assert(reverseList(nullptr) == nullptr);

    Node single{42, nullptr};
    assert(reverseList(&single) == &single);
    assert(single.next == nullptr);

    Node c{3, nullptr};
    Node b{2, &c};
    Node a{1, &b};

    Node* head = reverseList(&a);
    assert(head == &c);
    assert(c.next == &b);
    assert(b.next == &a);
    assert(a.next == nullptr);

    return 0;
}
