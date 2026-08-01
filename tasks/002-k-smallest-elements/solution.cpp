#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <vector>

// TODO: Explain the full-sort baseline and its complexity.
// TODO: Name the STL data structure used for the bounded candidate set.
// TODO: State whether its top element is the smallest or largest retained value.
// TODO: State the invariant after each processed input element.
// TODO: Give total time and auxiliary-space complexity, including result ordering.

std::vector<int> kSmallest(
    const std::vector<int>& values,
    std::size_t k)
{
    // TODO: Implement without sorting the entire input vector.
    // The completed implementation should preserve duplicates and should not
    // modify values.
    (void)values;
    (void)k;
    return {};
}

namespace {

void runTests()
{
    assert((kSmallest({7, 2, 9, 1, 5}, 3) ==
            std::vector<int>{1, 2, 5}));

    assert((kSmallest({4, 1, 1, 3}, 3) ==
            std::vector<int>{1, 1, 3}));

    assert(kSmallest({5, 2}, 0).empty());

    assert((kSmallest({5, 2}, 5) ==
            std::vector<int>{2, 5}));

    assert(kSmallest({}, 2).empty());

    assert((kSmallest({-1, -5, 3, 0, -5}, 4) ==
            std::vector<int>{-5, -5, -1, 0}));

    assert((kSmallest({6, 6, 6, 6}, 2) ==
            std::vector<int>{6, 6}));

    assert((kSmallest({1, 2, 3, 4, 5}, 1) ==
            std::vector<int>{1}));

    assert((kSmallest({5, 4, 3, 2, 1}, 4) ==
            std::vector<int>{1, 2, 3, 4}));
}

} // namespace

int main()
{
    runTests();
    std::cout << "All tests passed.\n";
    return 0;
}
