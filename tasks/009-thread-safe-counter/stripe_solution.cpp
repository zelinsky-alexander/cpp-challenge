#include <atomic>
#include <cstdint>
#include <numeric>
#include <thread>
#include <vector>
#include <new>

class StripedCounter {
public:
    // Default to the hardware concurrency or 8 stripes
    explicit StripedCounter(std::size_t numStripes = std::thread::hardware_concurrency())
        : _stripeCount(numStripes == 0 ? 8 : numStripes),
          _stripes(_stripeCount) {}

    void increment() {
        add(1);
    }

    void add(std::int64_t amount) {
        // Hash the thread ID to assign it to a stripe
        const std::size_t stripeIndex = getThreadStripeIndex();
        _stripes[stripeIndex].value.fetch_add(amount, std::memory_order_relaxed);
    }

    [[nodiscard]] std::int64_t value() const {
        std::int64_t total = 0;
        for (const auto& stripe : _stripes) {
            total += stripe.value.load(std::memory_order_relaxed);
        }
        return total;
    }

private:
    // Align each stripe to avoid false sharing between adjacent sub-counters
#ifdef __cpp_lib_hardware_interference_size
    using align_type = std::hardware_destructive_interference_size;
#else
    static constexpr std::size_t cache_line_size = 64;
#endif

    struct alignas(64) Stripe {
        std::atomic<std::int64_t> value{0};
    };

    std::size_t getThreadStripeIndex() const {
        // Thread-local cached index to avoid hashing thread::id on every increment
        static thread_local const std::size_t threadStripeId = 
            std::hash<std::thread::id>{}(std::this_thread::get_id());
        
        return threadStripeId % _stripeCount;
    }

    const std::size_t _stripeCount;
    std::vector<Stripe> _stripes;
};
