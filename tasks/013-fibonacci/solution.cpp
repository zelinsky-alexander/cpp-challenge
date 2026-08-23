#include <cassert>
#include <cstddef>
#include <cstdint>

std::uint64_t fibonacci(std::size_t n)
{
    return 0;
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
