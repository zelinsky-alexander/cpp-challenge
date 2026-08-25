# Task 028 — Thread Pool

## Goal

Implement a fixed-size thread pool:

```cpp
class ThreadPool {
public:
    explicit ThreadPool(std::size_t workerCount);
    ~ThreadPool();

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    void submit(std::function<void()> task);
    void waitIdle();
};
```

Workers should execute submitted tasks concurrently. `waitIdle()` must return only when all submitted tasks have finished.

## Requirements

- Use C++20.
- Use a fixed number of worker threads created by the constructor.
- Use a shared task queue protected correctly for concurrent access.
- Workers should sleep while no work is available; do not busy-spin.
- The destructor must stop workers and join them.
- Tasks already accepted before destruction must finish.
- Throw `std::invalid_argument` when `workerCount == 0`.
- Exceptions thrown by a task must not terminate a worker thread or the process.

## Build and run

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -pthread -g \
  tasks/028-thread-pool/solution.cpp -o /tmp/cpp-challenge-028
/tmp/cpp-challenge-028
```
