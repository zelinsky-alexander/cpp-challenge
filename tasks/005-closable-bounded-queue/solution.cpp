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
#include <chrono>

using namespace std::chrono_literals;

template <typename T>
class BoundedQueue {
public:
    explicit BoundedQueue(std::size_t capacity)
        : capacity_(capacity)
    {
        if (capacity == 0) 
			throw std::invalid_argument("capacity");
    }
	
	~BoundedQueue() 
	{
		//std::unique_lock lock(mutex_);
		//std::condition_variable all_done;
		//all_done.wait(lock, [this](){return values_.empty() || closed_;});
		std::cout << "BoundedQueue has been destroyed\n";
	}

    bool push(T value)
    {
		std::cout << "Enter push" << std::endl;
		std::unique_lock lock(mutex_);
		std::cout << "Acquired mutex" << std::endl;
		if (closed_) {
			std::cout << "Queue closed, go away";
			return false;
		}
		
		std::cout << "Wait until there is room in deque" << std::endl;
		not_full_.wait(lock, [this]() {return (closed_ || values_.size() < capacity_);});
		
		if (closed_) {
			std::cout << "Waking... Queue closed, go away";
			return false;
		}
		
		std::cout << "There is room, push_back " << value << std::endl;
		values_.push_back(std::move(value));
		lock.unlock();
		not_empty_.notify_one();
		std::cout << "Push done " << value << std::endl;
        return true;
    }

    std::optional<T> pop()
    {
		std::cout << "Enter pop" << std::endl;
		std::unique_lock lock(mutex_);
		
		std::cout << "Wait until there is value to pop in deque" << std::endl;
		not_empty_.wait(lock, [this](){return !values_.empty() || closed_;});
		
		if (values_.empty())
			return std::nullopt;
		
		T t = std::move(values_.front());
		//std::cout << "There is value, pop front " << t << std::endl;
		values_.pop_front();
		
		lock.unlock();
        not_full_.notify_one();
		std::cout << "Pop done " << t << std::endl;
		
		return t;
    }

    void close()
    {
		std::cout << "Enter close" << std::endl;
		
		{
			std::lock_guard lock(mutex_);
			closed_ = true;
		}
		
		std::cout << "Set close true" << std::endl;
					
		not_empty_.notify_all();
		not_full_.notify_all();
    }

    [[nodiscard]] bool isClosed() const
    {
		std::cout << "Enter isClosed" << std::endl;
        std::lock_guard lock(mutex_);
        return closed_;
    }

    [[nodiscard]] std::size_t size() const
    {
        std::lock_guard lock(mutex_);
		std::cout << "Current queue size is " << values_.size() << std::endl;
        return values_.size();
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
		std::cout << "TEST #1\n";
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
		std::cout << "\nTEST #2\n";
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
		std::cout << "\nTEST #3\n";
        BoundedQueue<std::unique_ptr<int>> queue(1);

        assert(queue.push(std::make_unique<int>(42)));
        auto value = queue.pop();
        assert(value.has_value());
        assert(*value.value() == 42);
    }

    {
		std::cout << "\nTEST #4\n";
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
		std::cout << "\nTEST #5\n";
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
		std::cout << "\nTEST #6\n";
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

    std::cout << "\nAll tests passed\n";
}
