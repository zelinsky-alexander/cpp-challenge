#include <cassert>
#include <cstddef>
#include <optional>
#include <vector>

std::optional<std::size_t> binarySearch(const std::vector<int>& values, int target);

int main()
{
    assert(binarySearch({}, 1) == std::nullopt);
    assert(binarySearch({5}, 5) == std::optional<std::size_t>{0});
    assert(binarySearch({5}, 2) == std::nullopt);
    assert(binarySearch({1, 3, 5, 7, 9}, 1) == std::optional<std::size_t>{0});
    assert(binarySearch({1, 3, 5, 7, 9}, 7) == std::optional<std::size_t>{3});
    assert(binarySearch({1, 3, 5, 7, 9}, 9) == std::optional<std::size_t>{4});
    assert(binarySearch({1, 3, 5, 7, 9}, 4) == std::nullopt);
    assert(binarySearch({-10, -3, 0, 4, 8, 12}, -3) == std::optional<std::size_t>{1});

    return 0;
}
