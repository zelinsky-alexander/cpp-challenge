#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>

class MinHeap {
public:
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
