#include <cassert>
#include <cstddef>
#include <stdexcept>

class MinHeap {
public:
    void push(int value);
    int top() const;
    void pop();
    bool empty() const noexcept;
    std::size_t size() const noexcept;
};

int main()
{
    MinHeap heap;
    assert(heap.empty());
    assert(heap.size() == 0);

    heap.push(5);
    heap.push(2);
    heap.push(8);
    heap.push(1);
    heap.push(2);

    assert(heap.size() == 5);
    assert(heap.top() == 1);
    heap.pop();
    assert(heap.top() == 2);
    heap.pop();
    assert(heap.top() == 2);
    heap.pop();
    assert(heap.top() == 5);
    heap.pop();
    assert(heap.top() == 8);
    heap.pop();
    assert(heap.empty());

    bool topThrew = false;
    try { static_cast<void>(heap.top()); } catch (const std::out_of_range&) { topThrew = true; }
    assert(topThrew);

    bool popThrew = false;
    try { heap.pop(); } catch (const std::out_of_range&) { popThrew = true; }
    assert(popThrew);

    return 0;
}
