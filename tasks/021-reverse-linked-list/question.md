# Task 021 — Reverse Linked List

## Goal

Given a singly linked list, reverse it in place and return the new head.

```cpp
struct Node {
    int value;
    Node* next;
};

Node* reverseList(Node* head);
```

## Examples

```text
1 -> 2 -> 3 -> null
becomes
3 -> 2 -> 1 -> null
```

## Requirements

- Use C++20.
- Reverse the existing links; do not allocate replacement nodes.
- Use an iterative implementation.
- Return `nullptr` for an empty list.
- Target time complexity: `O(n)`.
- Target extra-space complexity: `O(1)`.

## Build and run

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -g \
  tasks/021-reverse-linked-list/solution.cpp -o /tmp/cpp-challenge-021
/tmp/cpp-challenge-021
```
