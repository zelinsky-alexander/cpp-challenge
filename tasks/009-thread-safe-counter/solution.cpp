#include <cassert>
#include <cstdint>
#include <iostream>
#include <thread>
#include <vector>

class ThreadSafeCounter {
public:
    void increment()
    {
    }

    void add(std::int64_t amount)
    {
        (void)amount;
    }

    [[nodiscard]] std::int64_t value() const
    {
        return 0;
    }
};

namespace {

void runTests()
{
    {
        ThreadSafeCounter counter;
        assert(counter.value() == 0);
    }

    {
        ThreadSafeCounter counter;
        counter.increment();
        counter.add(4);
        counter.add(-2);
        assert(counter.value() == 3);
    }

    {
        ThreadSafeCounter counter;
        constexpr int threadCount = 8;
        constexpr int incrementsPerThread = 25'000;

        std::vector<std::jthread> threads;
        threads.reserve(threadCount);

        for (int threadIndex = 0; threadIndex < threadCount; ++threadIndex) {
            threads.emplace_back([&counter] {
                for (int i = 0; i < incrementsPerThread; ++i) {
                    counter.increment();
                }
            });
        }

        threads.clear();

        assert(counter.value() ==
               static_cast<std::int64_t>(threadCount) * incrementsPerThread);
    }

    {
        ThreadSafeCounter counter;
        constexpr int threadCount = 6;
        constexpr int operationsPerThread = 10'000;

        std::vector<std::jthread> threads;
        threads.reserve(threadCount);

        for (int threadIndex = 0; threadIndex < threadCount; ++threadIndex) {
            threads.emplace_back([&counter, threadIndex] {
                const std::int64_t amount =
                    (threadIndex % 2 == 0) ? std::int64_t{3} : std::int64_t{-1};

                for (int i = 0; i < operationsPerThread; ++i) {
                    counter.add(amount);
                }
            });
        }

        threads.clear();

        const std::int64_t expectedPerRound = 3 + (-1) + 3 + (-1) + 3 + (-1);
        assert(counter.value() == expectedPerRound * operationsPerThread);
    }
}

} // namespace

int main()
{
    runTests();
    std::cout << "All tests passed\n";
}
