# Task 031 — C++20 Bounded Priority Thread Pool

## Goal

Implement a fixed-size C++20 thread pool for a security-processing engine. Unlike the simpler [Task 028](../028-thread-pool/question.md), this exercise adds **bounded admission**, **three priority levels**, **cooperative stop**, and **drain/cancel shutdown modes**.

Use only the C++20 standard library; do not add an external thread-pool dependency.

## Public interface

The implementation exposes:

- Priority: high, normal, and low.
- ThreadPool(worker_count, queue_capacity).
- submit(priority, task) returning success/failure.
- shutdown(drain = true).
- pending() and failed_tasks() accessors.
- Task is a callable taking std::stop_token.

## Requirements

1. Start a fixed number of std::jthread workers; reject zero workers or zero queue capacity.
2. Protect the shared queue using std::mutex and std::condition_variable. Idle workers must sleep, not busy-spin.
3. Maintain three FIFO subqueues. Dispatch high-priority tasks before normal-priority tasks, and normal before low.
4. Bound the number of **waiting** tasks. Reject new submissions when full or closing; do not silently evict accepted tasks.
5. Execute task code **after releasing the queue mutex**.
6. Catch task exceptions inside workers, increment a failure counter, and keep the worker alive.
7. Graceful shutdown drains accepted tasks before joining workers.
8. Cancel-oriented shutdown discards waiting tasks, requests cooperative stop, and joins workers.
9. Reject an invalid priority and an empty task.
10. Document starvation, memory-lifetime, cancellation and durability limits.

**Assumptions:** shutdown is called by an external owner, never by a worker; concurrent shutdown calls are out of scope. Callers must ensure that objects referenced by task closures outlive those tasks.

## Compile and run

From the repository root:

    g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -pthread -O2 tasks/031-cpp20-priority-thread-pool/solution.cpp -o /tmp/cpp-challenge-031
    /tmp/cpp-challenge-031

Optional sanitizers:

    g++ -std=c++20 -Wall -Wextra -Wpedantic -pthread -g -fsanitize=address,undefined -fno-omit-frame-pointer tasks/031-cpp20-priority-thread-pool/solution.cpp -o /tmp/cpp-challenge-031-san
    /tmp/cpp-challenge-031-san

The self-contained main() in solution.cpp runs checks (also active with -DNDEBUG) for invalid arguments, draining, priority ordering, queue capacity, exception isolation, concurrent producers, and cancellation.

Read [explanation.md](explanation.md) after solving or reviewing the implementation.
