# Task 024 — Min Heap

## Goal

Implement a min-heap of integers:

```cpp
class MinHeap {
public:
    void push(int value);
    int top() const;
    void pop();
    bool empty() const noexcept;
    std::size_t size() const noexcept;
};
```

The smallest value must always be available through `top()`.

## Requirements

- Use C++20.
- Store the heap in a contiguous container.
- Do not use `std::priority_queue`, `std::push_heap`, `std::pop_heap`, or `std::make_heap`.
- `push` and `pop` should be `O(log n)`.
- `top`, `empty`, and `size` should be `O(1)`.
- `top()` and `pop()` on an empty heap must throw `std::out_of_range`.
- Duplicates are allowed.

## Build and run

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -g \
  tasks/024-heap/solution.cpp -o /tmp/cpp-challenge-024
/tmp/cpp-challenge-024
```
