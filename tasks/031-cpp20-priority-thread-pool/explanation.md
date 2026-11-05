# Task 031 — Bounded Priority Thread Pool: Explanation

## Architecture

This exercise implements an in-process executor using only standard C++20 facilities. It is distinct from a distributed scheduler: jobs are not persisted, and a process crash can lose queued work.

    Producer A     Producer B     Producer C
         \             |              /
                  submit(job)
                       |
              mutex-protected queues
              +--------------------+
              | HIGH     FIFO      |
              | NORMAL   FIFO      |
              | LOW      FIFO      |
              +--------------------+
                       |
                condition_variable
                       |
             +---------+---------+
             |         |         |
           jthread   jthread   jthread
             1         2         N
             |         |         |
            task      task      task
              (outside queue mutex)

Each worker repeatedly removes one task from the highest nonempty priority deque. Within a priority, queue order is FIFO. Concurrent execution means **completion order is not guaranteed**.

## 1. Why modern C++20?

- **std::jthread** owns a thread and automatically joins it during destruction.
- **std::stop_token** lets a running task cooperate with cancellation.
- **std::mutex** protects the queue and shutdown state.
- **std::unique_lock** is needed because condition-variable waiting temporarily releases the mutex.
- **std::condition_variable** blocks idle workers without busy spinning.
- **std::atomic<size_t>** counts failed tasks without requiring the queue mutex.

These are ISO/IEC 14882:2020 (C++20) standard-library facilities, not third-party implementations.

## 2. Queue invariant

The mutex protects the three deques, pending count, and closing flag.

At all times while holding the mutex:

    pending == high.size() + normal.size() + low.size()
    0 <= pending <= capacity

The pending count includes *waiting* jobs only. A worker removes a task before executing it, so a running task is not included.

The number of worker threads is fixed at construction; this implementation does not autoscale the thread count.

## 3. Why use the wait predicate?

A worker conceptually waits until either a task exists or shutdown starts:

    wait(lock, closing || pending > 0)

The wait operation releases the mutex atomically with sleeping, then reacquires it and checks the condition. A predicate handles spurious wakeups.

A producer inserts the task while holding the mutex, then calls notify_one(). During shutdown, notify_all() wakes idle threads.

After retrieving a task, the worker unlocks **before** invoking user code. Otherwise a slow scan could block every other worker and producer.

## 4. Priority and fairness

Three deques allow constant-time enqueueing and checking at most three deque fronts for the next task.

Selection order is:

    high > normal > low

**Important limitation:** strict priority can starve normal/low work under continuous high-priority arrivals. Production alternatives include aging, weighted fair scheduling, capacity reservation, or separate execution pools.

The current pool does not preempt already-running low-priority tasks.

## 5. Capacity and backpressure

If all waiting slots are occupied, submit() returns false. It also rejects submissions after shutdown begins.

The pool does not drop older accepted jobs to make room for newer ones. Rejected jobs belong to the caller: a security scheduler might keep them in durable storage and retry later.

A bounded task count is **not** a bound on overall memory, file contents, NAS I/O, or task runtime. Each running task may have its own resource budget.

## 6. Exceptions

Workers catch all task exceptions, increment failed_tasks(), then continue processing subsequent jobs.

The counter uses relaxed atomic memory ordering because it is just an independent statistic; it does not publish any task results.

This teaching example does not return exceptions to submitters or perform automatic retries. A production pipeline should report job IDs, failure reasons, and retryability to durable job state.

## 7. Shutdown modes

| Mode | Waiting jobs | Running jobs | New submissions |
| --- | --- | --- | --- |
| shutdown(true) | Drain and execute | Continue | Reject |
| shutdown(false) | Discard | Receive stop request | Reject |

Both modes join all worker threads. std::stop_token does **not** forcibly terminate a blocked task. A long-running scan must observe the token and use cancellable/bounded blocking I/O.

There is no timed join in this implementation. Shutdown must be called by an external owner, not from a task running in the same pool, and concurrent shutdown calls are not supported.

A task that holds references into external objects requires the caller to guarantee their lifetimes until the task ends.

## 8. Test coverage

The main() function in solution.cpp runs these tests:

1. Reject zero workers, zero capacity, empty tasks, and invalid priorities.
2. Run 300 submitted tasks and verify draining shutdown.
3. Verify high -> normal -> low order using a gate that keeps a single worker busy while tasks are enqueued.
4. Confirm that a full waiting queue rejects submission.
5. Confirm that an exception in one task does not terminate the pool.
6. Check concurrent submission from four producers to eight worker threads.
7. Verify that cancel-oriented shutdown requests stop of a running task while discarding waiting jobs.

The tests require no framework: they use standard assertions, atomics, futures/promises, and jthreads.

## 9. Performance and future improvements

- Task selection checks at most three subqueues, so priority selection is O(1).
- Enqueue and dequeue are typically constant time for std::deque.
- Maximum queued task count is O(capacity); running task memory is additional.
- A single mutex may become contended for very small tasks at high rates.
- Possible later optimizations: per-worker queues, work stealing, batching, and lower-allocation task representation.
- Measure contention and throughput before introducing lock-free complexity.
- std::function requires copyable callable targets and may allocate for larger closures. A move-only callable wrapper is a potential extension.

## Interview takeaway

A good first design is a bounded mutex-protected queue, fixed worker count, condition-variable waits, explicit priority and overload policy, and clear drain/cancel shutdown semantics. Prefer measurable, correct behavior over premature lock-free optimization.

## Dependencies and publication notes

**Dependencies:** C++20 standard library only; no external runtime packages or third-party source code included. A current, maintained C++20 compiler and standard library are needed. No third-party notices are required for the original task code.

Before publishing or using commercially, review correctness, security, thread lifetime guarantees, similarity, and the repository's licensing status. A standard-library-only implementation is not automatically production-hardened or legally cleared.
