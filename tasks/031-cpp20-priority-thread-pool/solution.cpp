#include <array>
#include <atomic>
#include <cassert>
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <deque>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <stop_token>
#include <thread>
#include <utility>
#include <vector>

// Task 031: C++20 bounded priority thread pool.
// This is intentionally a small, standard-library-only teaching implementation.

enum class Priority : std::size_t { high = 0, normal = 1, low = 2 };

class ThreadPool {
public:
    using Task = std::function<void(std::stop_token)>;

    ThreadPool(std::size_t worker_count, std::size_t queue_capacity)
        : capacity_(queue_capacity) {
        if (worker_count == 0 || capacity_ == 0) {
            throw std::invalid_argument("worker count and queue capacity must be positive");
        }
        workers_.reserve(worker_count);
        try {
            for (std::size_t i = 0; i < worker_count; ++i) {
                workers_.emplace_back([this](std::stop_token token) {
                    worker_loop(token);
                });
            }
        } catch (...) {
            shutdown(false);
            throw;
        }
    }

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    // Nonblocking submission. Rejection leaves the caller responsible for retrying
    // or keeping the job in durable storage. The queued task owns its callable.
    bool submit(Priority priority, Task task) {
        if (!task) {
            throw std::invalid_argument("task must be callable");
        }
        const auto index = static_cast<std::size_t>(priority);
        if (index >= queues_.size()) {
            throw std::invalid_argument("invalid task priority");
        }
        {
            std::lock_guard lock(mutex_);
            if (closing_ || pending_ == capacity_) {
                return false;
            }
            queues_[index].push_back(std::move(task));
            ++pending_;
        }
        available_.notify_one();
        return true;
    }

    // drain=true: process all accepted tasks before returning.
    // drain=false: discard queued tasks and request cooperative stop of running tasks.
    // Call only from an external owner, never from a pool worker; shutdown calls
    // must not execute concurrently with one another.
    void shutdown(bool drain = true) {
        std::array<std::deque<Task>, 3> discarded;
        {
            std::lock_guard lock(mutex_);
            closing_ = true;
            if (!drain) {
                discarded.swap(queues_);
                pending_ = 0;
            }
        }
        if (!drain) {
            for (auto& worker : workers_) {
                worker.request_stop();
            }
        }
        available_.notify_all();
        for (auto& worker : workers_) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

    ~ThreadPool() { shutdown(); }

    std::size_t pending() const {
        std::lock_guard lock(mutex_);
        return pending_;
    }

    std::size_t failed_tasks() const noexcept {
        return failed_.load(std::memory_order_relaxed);
    }

private:
    void worker_loop(std::stop_token stop) {
        while (true) {
            Task task;
            {
                std::unique_lock lock(mutex_);
                available_.wait(lock, [this] {
                    return closing_ || pending_ != 0;
                });
                if (pending_ == 0) {
                    return;
                }
                // Strict priority: high first, FIFO within each priority.
                for (auto& queue : queues_) {
                    if (!queue.empty()) {
                        task = std::move(queue.front());
                        queue.pop_front();
                        --pending_;
                        break;
                    }
                }
            }
            try {
                task(stop); // Never execute arbitrary task code while holding mutex_.
            } catch (...) {
                failed_.fetch_add(1, std::memory_order_relaxed);
            }
        }
    }

    const std::size_t capacity_;
    std::array<std::deque<Task>, 3> queues_;
    std::size_t pending_ = 0;
    bool closing_ = false;
    mutable std::mutex mutex_;
    std::condition_variable available_;
    std::vector<std::jthread> workers_;
    std::atomic<std::size_t> failed_{0};
};

// Tests are kept in the same file as other cpp-challenge tasks.

void test_validation() {
    bool invalid_workers = false;
    try { ThreadPool pool(0, 2); }
    catch (const std::invalid_argument&) { invalid_workers = true; }
    assert(invalid_workers);

    bool invalid_capacity = false;
    try { ThreadPool pool(1, 0); }
    catch (const std::invalid_argument&) { invalid_capacity = true; }
    assert(invalid_capacity);

    ThreadPool pool(1, 2);
    bool invalid_task = false;
    try { pool.submit(Priority::normal, {}); }
    catch (const std::invalid_argument&) { invalid_task = true; }
    assert(invalid_task);

    bool invalid_priority = false;
    try { pool.submit(static_cast<Priority>(99), [](std::stop_token) {}); }
    catch (const std::invalid_argument&) { invalid_priority = true; }
    assert(invalid_priority);
}

void test_drain_and_parallel_execution() {
    std::atomic<int> completed{0};
    {
        ThreadPool pool(4, 300);
        for (int i = 0; i < 300; ++i) {
            assert(pool.submit(Priority::normal, [&](std::stop_token) {
                completed.fetch_add(1, std::memory_order_relaxed);
            }));
        }
        pool.shutdown(true);
        assert(pool.pending() == 0);
        assert(!pool.submit(Priority::high, [](std::stop_token) {}));
    }
    assert(completed.load(std::memory_order_relaxed) == 300);
}

void test_priority_and_fifo() {
    ThreadPool pool(1, 3);
    std::promise<void> entered;
    auto started = entered.get_future();
    std::promise<void> release;
    auto gate = release.get_future().share();
    std::vector<int> order;

    assert(pool.submit(Priority::normal, [&](std::stop_token) {
        entered.set_value();
        gate.wait();
    }));
    started.wait(); // Keep the only worker occupied while tasks are queued.
    assert(pool.submit(Priority::low, [&](std::stop_token) { order.push_back(3); }));
    assert(pool.submit(Priority::high, [&](std::stop_token) { order.push_back(1); }));
    assert(pool.submit(Priority::normal, [&](std::stop_token) { order.push_back(2); }));
    assert(pool.pending() == 3);
    release.set_value();
    pool.shutdown();
    assert((order == std::vector<int>{1, 2, 3}));
}

void test_queue_capacity() {
    ThreadPool pool(1, 2);
    std::promise<void> entered;
    auto started = entered.get_future();
    std::promise<void> release;
    auto gate = release.get_future().share();
    std::atomic<int> completed{0};

    assert(pool.submit(Priority::normal, [&](std::stop_token) {
        entered.set_value();
        gate.wait();
        ++completed;
    }));
    started.wait();
    assert(pool.submit(Priority::low, [&](std::stop_token) { ++completed; }));
    assert(pool.submit(Priority::high, [&](std::stop_token) { ++completed; }));
    assert(!pool.submit(Priority::high, [&](std::stop_token) { ++completed; }));
    assert(pool.pending() == 2);
    release.set_value();
    pool.shutdown();
    assert(completed == 3);
}

void test_exception_isolation() {
    ThreadPool pool(2, 4);
    std::atomic<int> completed{0};
    assert(pool.submit(Priority::normal, [](std::stop_token) {
        throw std::runtime_error("expected test failure");
    }));
    assert(pool.submit(Priority::normal, [&](std::stop_token) { ++completed; }));
    pool.shutdown();
    assert(pool.failed_tasks() == 1);
    assert(completed == 1);
}

void test_concurrent_producers() {
    ThreadPool pool(8, 1024);
    std::atomic<int> completed{0};
    std::vector<std::jthread> producers;
    for (int producer = 0; producer < 4; ++producer) {
        producers.emplace_back([&] {
            for (int n = 0; n < 250; ++n) {
                // This retry is test-only; production callers need bounded backoff.
                while (!pool.submit(Priority::normal, [&](std::stop_token) {
                    completed.fetch_add(1, std::memory_order_relaxed);
                })) {
                    std::this_thread::yield();
                }
            }
        });
    }
    for (auto& producer : producers) {
        producer.join();
    }
    pool.shutdown();
    assert(completed == 1000);
}

void test_cancel_pending_and_stop_running() {
    ThreadPool pool(1, 3);
    std::promise<void> entered;
    auto started = entered.get_future();
    std::atomic<int> discarded_should_not_run{0};
    std::atomic<bool> observed_stop{false};

    assert(pool.submit(Priority::normal, [&](std::stop_token stop) {
        entered.set_value();
        while (!stop.stop_requested()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        observed_stop = true;
    }));
    started.wait();
    assert(pool.submit(Priority::low, [&](std::stop_token) {
        ++discarded_should_not_run;
    }));
    pool.shutdown(false);
    assert(observed_stop);
    assert(discarded_should_not_run == 0);
    assert(pool.pending() == 0);
}

int main() {
    test_validation();
    test_drain_and_parallel_execution();
    test_priority_and_fifo();
    test_queue_capacity();
    test_exception_isolation();
    test_concurrent_producers();
    test_cancel_pending_and_stop_running();
    std::cout << "Task 031: all thread-pool tests passed\n";
}
