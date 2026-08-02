#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

/*
 * Brute-force approach:
 * Check every possible contiguous subarray and calculate its sum.
 * A simple implementation takes O(n^3), or O(n^2) if the sum is
 * updated while extending each subarray.
 *
 * Selected pattern:
 * Sliding window with two boundaries: left and right.
 *
 * Invariant:
 * currentSum is always the sum of the current window:
 * values[left ... right].
 *
 * Because all input values are positive:
 * - moving right forward can only increase the sum;
 * - moving left forward can only decrease the sum.
 *
 * This allows us to shrink the window while it remains valid and
 * find the shortest valid window ending at each right position.
 *
 * Each element enters the window once and leaves it once.
 * Therefore:
 * - Time complexity: O(n)
 * - Extra-space complexity: O(1)
 *
 * std::int64_t is used for currentSum to reduce the risk of overflow
 * when many large int values are added together.
 */

std::size_t minSubarrayLength(
    const std::vector<int>& values,
    int target)
{
    if (values.empty()) {
        return 0;
    }

    std::size_t left = 0;
    std::size_t minLength = values.size() + 1;
    std::int64_t currentSum = 0;

    for (std::size_t right = 0; right < values.size(); ++right) {
        currentSum += values[right];

        while (currentSum >= static_cast<std::int64_t>(target)) {
            const std::size_t currentLength = right - left + 1;

            minLength = std::min(minLength, currentLength);

            currentSum -= values[left];
            ++left;
        }
    }

    if (minLength == values.size() + 1) {
        return 0;
    }

    return minLength;
}

namespace {

void runTests()
{
    assert(minSubarrayLength({2, 3, 1, 2, 4, 3}, 7) == 2);
    assert(minSubarrayLength({1, 1, 1, 1}, 5) == 0);
    assert(minSubarrayLength({9, 1, 1}, 7) == 1);
    assert(minSubarrayLength({}, 3) == 0);

    assert(minSubarrayLength({5}, 6) == 0);
    assert(minSubarrayLength({5}, 5) == 1);
    assert(minSubarrayLength({5}, 4) == 1);

    assert(minSubarrayLength({1, 2, 3}, 6) == 3);
    assert(minSubarrayLength({1, 10, 1}, 10) == 1);
    assert(minSubarrayLength({1, 1, 1, 10}, 10) == 1);

    assert(minSubarrayLength(
        {2'000'000'000, 2'000'000'000},
        2'100'000'000) == 2);
}

} // namespace

int main()
{
    runTests();
    std::cout << "All tests passed.\n";
    return 0;
}