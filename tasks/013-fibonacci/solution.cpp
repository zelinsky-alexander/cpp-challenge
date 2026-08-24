#include <cassert>
#include <cstddef>
#include <cstdint>
#include <stdexcept>

std::uint64_t fibonacci(std::size_t n)
{
    if (n == 0) return 0;
    if (n > 93) throw std::overflow_error("Fibonacci value exceeds 64-bit unsigned integer bounds.");

    std::uint64_t a = 0;
    std::uint64_t b = 1;
    for (std::size_t i = 1; i < n; ++i) {
       std::uint64_t tmp = b;
       b = b + a;
       a = tmp;
    }
    return b;
}

int main()
{
    assert(fibonacci(0) == 0);
    assert(fibonacci(1) == 1);
    assert(fibonacci(2) == 1);
    assert(fibonacci(3) == 2);
    assert(fibonacci(5) == 5);
    assert(fibonacci(10) == 55);
    assert(fibonacci(20) == 6765);
    assert(fibonacci(50) == 12586269025ULL);
    assert(fibonacci(93) == 12200160415121876738ULL);

    return 0;
}
