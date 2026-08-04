#include <cassert>
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <deque>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <thread>
#include <utility>
#include <vector>

template <typename T>
class BoundedQueue {
public:
    explicit BoundedQueue(std::size_t capacity)
        : capacity_(capacity)
    {
        // TODO: Reject capacity == 0 with std::invalid_argument.
    }

    bool push(T value)
    {
        // TODO:
        // 1. Lock the mutex.
        // 2. Wait until space is available or the queue is closed.
        // 3. Return false without inserting if closed.
        // 4. Move value into the FIFO container.
        // 5. Unlock and notify one waiting consumer.
        (void)value;
        return false;
    }

    std::optional<T> pop()
    {
        // TODO:
        // 1. Lock the mutex.
        // 2. Wait until data is available or the queue is closed.
        // 3. Return std::nullopt only when closed and empty.
        // 4. Move the oldest value out and remove it from the queue.
        // 5. Unlock and notify one waiting producer.
        return std::nullopt;
    }

    void close()
    {
        // TODO:
        // 1. Mark the queue closed while holding the mutex.
        // 2. Make repeated calls harmless.
        // 3. Wake all blocked producers and consumers.
    }

    [[nodiscard]] bool isClosed() const
    {
        // TODO: Read closed_ while synchronized.
        return false;
    }

    [[nodiscard]] std::size_t size() const
    {
        // TODO: Read the container size while synchronized.
        return 0;
    }

private:
    // TODO: Document the queue invariant and the wait predicates.
    const std::size_t capacity_;
    mutable std::mutex mutex_;
    std::condition_variable not_empty_;
    std::condition_variable not_full_;
    std::deque<T> values_;
    bool closed_{false};
};

int main()
{
    using namespace std::chrono_literals;

    {
        bool threw = false;
        try {
            BoundedQueue<int> invalid(0);
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        assert(threw);
    }

    {
        BoundedQueue<int> queue(2);

        assert(queue.size() == 0);
        assert(!queue.isClosed());
        assert(queue.push(10));
        assert(queue.push(20));
        assert(queue.size() == 2);
        assert(queue.pop() == std::optional<int>{10});
        assert(queue.pop() == std::optional<int>{20});
        assert(queue.size() == 0);
    }

    {
        BoundedQueue<int> queue(2);

        assert(queue.push(1));
        assert(queue.push(2));
        queue.close();
        queue.close();

        assert(queue.isClosed());
        assert(!queue.push(3));
        assert(queue.pop() == std::optional<int>{1});
        assert(queue.pop() == std::optional<int>{2});
        assert(!queue.pop().has_value());
    }

    {
        BoundedQueue<std::unique_ptr<int>> queue(1);

        assert(queue.push(std::make_unique<int>(42)));
        auto value = queue.pop();
        assert(value.has_value());
        assert(*value.value() == 42);
    }

    {
        BoundedQueue<int> queue(1);
        assert(queue.push(7));

        auto producer = std::async(std::launch::async, [&queue] {
            return queue.push(8);
        });

        assert(producer.wait_for(50ms) == std::future_status::timeout);
        assert(queue.pop() == std::optional<int>{7});
        assert(producer.wait_for(1s) == std::future_status::ready);
        assert(producer.get());
        assert(queue.pop() == std::optional<int>{8});
    }

    {
        BoundedQueue<int> queue(1);

        auto consumer = std::async(std::launch::async, [&queue] {
            return queue.pop();
        });

        assert(consumer.wait_for(50ms) == std::future_status::timeout);
        queue.close();
        assert(consumer.wait_for(1s) == std::future_status::ready);
        assert(!consumer.get().has_value());
    }

    {
        constexpr int producer_count = 3;
        constexpr int values_per_producer = 100;
        constexpr int expected_total = producer_count * values_per_producer;

        BoundedQueue<int> queue(8);
        std::vector<std::thread> producers;
        std::vector<int> consumed;
        std::mutex consumed_mutex;

        std::thread consumer([&] {
            while (auto value = queue.pop()) {
                std::lock_guard lock(consumed_mutex);
                consumed.push_back(*value);
            }
        });

        for (int producer_id = 0; producer_id < producer_count; ++producer_id) {
            producers.emplace_back([&, producer_id] {
                for (int i = 0; i < values_per_producer; ++i) {
                    const int value = producer_id * values_per_producer + i;
                    assert(queue.push(value));
                }
            });
        }

        for (auto& producer : producers) {
            producer.join();
        }

        queue.close();
        consumer.join();

        assert(static_cast<int>(consumed.size()) == expected_total);

        std::vector<bool> seen(static_cast<std::size_t>(expected_total), false);
        for (const int value : consumed) {
            assert(value >= 0);
            assert(value < expected_total);
            assert(!seen[static_cast<std::size_t>(value)]);
            seen[static_cast<std::size_t>(value)] = true;
        }

        for (const bool was_seen : seen) {
            assert(was_seen);
        }
    }

    std::cout << "All tests passed\n";
}
