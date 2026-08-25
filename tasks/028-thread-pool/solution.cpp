#include <atomic>
#include <cassert>
#include <cstddef>
#include <functional>
#include <stdexcept>

class ThreadPool {
public:
    explicit ThreadPool(std::size_t workerCount);
    ~ThreadPool();

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    void submit(std::function<void()> task);
    void waitIdle();
};

int main()
{
    bool threw = false;
    try { ThreadPool invalid(0); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    ThreadPool pool(4);
    std::atomic<int> count{0};

    for (int i = 0; i < 100; ++i) {
        pool.submit([&count] { count.fetch_add(1, std::memory_order_relaxed); });
    }

    pool.submit([] { throw std::runtime_error("task failure"); });
    pool.submit([&count] { count.fetch_add(10, std::memory_order_relaxed); });

    pool.waitIdle();
    assert(count.load(std::memory_order_relaxed) == 110);

    return 0;
}
