#include <cassert>
#include <cstddef>
#include <optional>
#include <vector>

std::optional<long long> maxWindowSum(const std::vector<int>& values, std::size_t windowSize);

int main()
{
    assert(maxWindowSum({}, 1) == std::nullopt);
    assert(maxWindowSum({1, 2}, 0) == std::nullopt);
    assert(maxWindowSum({1, 2}, 3) == std::nullopt);
    assert(maxWindowSum({7}, 1) == std::optional<long long>{7});
    assert(maxWindowSum({2, 1, 5, 1, 3, 2}, 3) == std::optional<long long>{9});
    assert(maxWindowSum({1, 2, 3, 4, 5}, 2) == std::optional<long long>{9});
    assert(maxWindowSum({-4, -2, -7}, 2) == std::optional<long long>{-6});
    assert(maxWindowSum({3, 3, 3}, 3) == std::optional<long long>{9});

    return 0;
}
