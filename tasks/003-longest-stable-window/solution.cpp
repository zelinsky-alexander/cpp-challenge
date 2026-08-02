#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <vector>

// TODO: Explain why checking every subarray is too slow.
// TODO: State the invariants maintained by the two monotonic deques.
// TODO: Explain why indices, rather than only values, should be stored.
// TODO: Give the amortized time and auxiliary-space complexity.

std::size_t longestStableWindow(
    const std::vector<int>& values,
    int limit)
{
    // TODO: Implement an O(n) sliding-window solution.
    // A window is valid when max(window) - min(window) <= limit.
    // Return 0 when values is empty or limit is negative.
    (void)values;
    (void)limit;
    return 0;
}

namespace {

void runTests()
{
    assert(longestStableWindow({8, 2, 4, 7}, 4) == 2);
    assert(longestStableWindow({10, 1, 2, 4, 7, 2}, 5) == 4);
    assert(longestStableWindow({4, 2, 2, 2, 4, 4, 2, 2}, 0) == 3);
    assert(longestStableWindow({}, 3) == 0);
    assert(longestStableWindow({5}, 0) == 1);
    assert(longestStableWindow({1, 2, 3, 4}, 3) == 4);
    assert(longestStableWindow({1, 100, 1, 100}, 0) == 1);
    assert(longestStableWindow({-5, -2, -3, -4}, 2) == 3);
    assert(longestStableWindow({1, 2, 3}, -1) == 0);
    assert(longestStableWindow({7, 7, 7, 7}, 0) == 4);
}

} // namespace

int main()
{
    runTests();
    std::cout << "All tests passed.\n";
    return 0;
}
