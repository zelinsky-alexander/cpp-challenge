#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <queue>
#include <vector>

/*
 * A straightforward solution would copy all input elements, sort the copy,
 * and return the first k elements. That requires O(n log n) time and O(n)
 * additional space.
 *
 * This solution uses std::priority_queue<int>, which is a max-heap by default.
 * Its top element is the largest value currently retained.
 *
 * Invariant:
 * After processing each input element, the heap contains the smallest
 * min(k, processedElementCount) values seen so far.
 *
 * If the heap contains fewer than k elements, the new value is inserted.
 * Otherwise, the new value replaces the heap's largest retained value only
 * when it is smaller than that value.
 *
 * Each heap insertion or removal costs O(log k), so processing the input takes
 * O(n log k) when 0 < k < n. Extracting the heap takes O(k log k), and reversing
 * the result takes O(k). The heap uses O(k) auxiliary space.
 *
 * Duplicates are handled correctly because each occurrence is inserted and
 * stored independently. Equal values may therefore appear multiple times.
 */

std::vector<int> kSmallest(
    const std::vector<int>& values,
    std::size_t k)
{
    if (k == 0 || values.empty()) {
        return {};
    }

    k = std::min(k, values.size());

    std::priority_queue<int> candidates;

    for (int value : values) {
        if (candidates.size() < k) {
            candidates.push(value);
            continue;
        }

        if (value < candidates.top()) {
            candidates.pop();
            candidates.push(value);
        }
    }

    std::vector<int> result;
    result.reserve(candidates.size());

    while (!candidates.empty()) {
        result.push_back(candidates.top());
        candidates.pop();
    }

    // A max-heap produces values from largest to smallest.
    std::reverse(result.begin(), result.end());

    return result;
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

    assert((kSmallest({3, 1, 2, 2, 4}, 3) ==
            std::vector<int>{1, 2, 2}));
}

} // namespace

int main()
{
    runTests();
    std::cout << "All tests passed.\n";
    return 0;
}