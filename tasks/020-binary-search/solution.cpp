#include <cassert>
#include <cstddef>
#include <optional>
#include <vector>
#include <iostream>

std::optional<std::size_t> binarySearch(const std::vector<int>& values, int target)
{
    size_t sz = values.size();
    std::cout << "size: " << sz << " target:" << target << std::endl;

    size_t left = 0;
    size_t right = sz;

    while (left < right) {
        size_t mid = left + (right - left)/2;
        std::cout << "->" << mid << std::endl;
        int v = values[mid];
        if (target == v) return mid;
        if (target > v) 
            left = mid + 1;
        else
            right = mid;
    }

    return std::nullopt;
}

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
    std::cout << "all tests passed" << std::endl;

    return 0;
}
