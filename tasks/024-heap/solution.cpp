#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>

    /*
    This is a binary min-heap implemented on top of a std::vector
    A binary heap stores a complete binary tree inside a flat array, using index math instead of pointers:
    Parent of index i → (i-1)/2
    Left child of index i → 2i+1
    Right child of index i → 2i+2

        index:     0       1       2       3   4   5   6
        tree:      root
                  /    \
                 1      2
                / \    / \
               3   4  5   6

        Node 0's children are at 1 and 2 → 2*0+1=1, 2*0+2=2.
        Node 1's children are at 3 and 4 → 2*1+1=3, 2*1+2=4.
        Node 2's children are at 5 and 6 → 2*2+1=5, 2*2+2=6.

        The pattern 2i+1 / 2i+2 always holds for this layout implemented in siftDown 
        and it's the mirror of the parent formula (i-1)/2 used in siftUp

        Complexity
        push: O(log n) — sift up at most tree height.
        pop: O(log n) — sift down at most tree height.
        top: O(1).
    */

class MinHeap {
public:
    /* 
       Push appends value to the end of the vector (bottom of the tree).
       Calls siftUp to bubble it upward: while it's smaller than its parent, swap with the parent. 
       Stops when it finds a parent that's smaller, or reaches the root. 
    */
    void push(int value) {
        _buffer.push_back(value);
        siftUp(_buffer.size() - 1);
    }

    int top() const {
        if (_buffer.empty()) {
            throw std::out_of_range("Heap is empty");
        }
        return _buffer.front();
    }

    /*
       Removing the root without shifting the whole array would be O(n), so it uses the classic trick:
       Move the last element into slot 0 (overwriting the old min).
       Shrink the vector by one (pop_back).
       Call siftDown(0) to restore heap order: compare the new root to both children, 
       swap with the smaller child if it violates the heap property, and repeat until it settles into place (or has no children left).
    */
    void pop() {
        if (_buffer.empty()) {
            throw std::out_of_range("Heap is empty");
        }
        _buffer[0] = _buffer.back();
        _buffer.pop_back();
        if (!_buffer.empty()) {
            siftDown(0);
        }
    }

    bool empty() const noexcept {
        return _buffer.empty();
    }

    std::size_t size() const noexcept {
        return _buffer.size();
    }

private:
    std::vector<int> _buffer;

    void siftUp(std::size_t index) {
        while (index > 0) {
            std::size_t parent = (index - 1) / 2;
            if (_buffer[index] < _buffer[parent]) {
                std::swap(_buffer[index], _buffer[parent]);
                index = parent;
            } else {
                break;
            }
        }
    }

    void siftDown(std::size_t index) {
        std::size_t size = _buffer.size();
        while (true) {
            std::size_t smallest = index;
            // At any position index in the tree-as-array, its two children live at:
            std::size_t left = 2 * index + 1;
            std::size_t right = 2 * index + 2;

            if (left < size && _buffer[left] < _buffer[smallest]) {
                smallest = left;
            }
            if (right < size && _buffer[right] < _buffer[smallest]) {
                smallest = right;
            }

            if (smallest != index) {
                std::swap(_buffer[index], _buffer[smallest]);
                index = smallest;
            } else {
                break;
            }
        }
    }
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
