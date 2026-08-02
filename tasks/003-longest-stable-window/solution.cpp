#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <iostream>
#include <vector>
#include <cassert>

namespace {

void printDeque(
    const char* name,
    const std::deque<std::size_t>& indices,
    const std::vector<int>& values)
{
    std::cout << name << ": [";

    for (std::size_t i = 0; i < indices.size(); ++i) {
        const std::size_t index = indices[i];

        std::cout << "{index=" << index
                  << ", value=" << values[index] << "}";

        if (i + 1 < indices.size()) {
            std::cout << ", ";
        }
    }

    std::cout << "]\n";
}

void printWindow(
    const std::vector<int>& values,
    std::size_t left,
    std::size_t right)
{
    std::cout << "Current window [" << left << ", " << right << "]: {";

    for (std::size_t i = left; i <= right; ++i) {
        std::cout << values[i];

        if (i < right) {
            std::cout << ", ";
        }
    }

    std::cout << "}\n";
}

} // namespace

std::size_t longestStableWindow(
    const std::vector<int>& values,
    int limit)
{
    std::cout << "Starting longestStableWindow\n";
    std::cout << "limit = " << limit << "\n";

    if (values.empty()) {
        std::cout << "Input is empty. Returning 0.\n";
        return 0;
    }

    if (limit < 0) {
        std::cout << "Limit is negative. Returning 0.\n";
        return 0;
    }

    std::deque<std::size_t> minDeque;
    std::deque<std::size_t> maxDeque;

    std::size_t left = 0;
    std::size_t longest = 0;

    for (std::size_t right = 0; right < values.size(); ++right) {
        std::cout << "\n========================================\n";
        std::cout << "Moving right to index " << right
                  << ", value = " << values[right] << "\n";

        std::cout << "\nUpdating minimum deque\n";

        while (!minDeque.empty() &&
               values[minDeque.back()] > values[right]) {
            std::cout
                << "Removing index " << minDeque.back()
                << " with value " << values[minDeque.back()]
                << " from back of minDeque because it is larger than "
                << values[right] << "\n";

            minDeque.pop_back();
        }

        std::cout << "Adding index " << right
                  << " to minDeque\n";

        minDeque.push_back(right);
        printDeque("minDeque", minDeque, values);

        std::cout << "\nUpdating maximum deque\n";

        while (!maxDeque.empty() &&
               values[maxDeque.back()] < values[right]) {
            std::cout
                << "Removing index " << maxDeque.back()
                << " with value " << values[maxDeque.back()]
                << " from back of maxDeque because it is smaller than "
                << values[right] << "\n";

            maxDeque.pop_back();
        }

        std::cout << "Adding index " << right
                  << " to maxDeque\n";

        maxDeque.push_back(right);
        printDeque("maxDeque", maxDeque, values);

        printWindow(values, left, right);

        while (
            static_cast<std::int64_t>(values[maxDeque.front()]) -
                static_cast<std::int64_t>(values[minDeque.front()]) >
            static_cast<std::int64_t>(limit)) {

            const int currentMinimum = values[minDeque.front()];
            const int currentMaximum = values[maxDeque.front()];

            std::cout << "\nWindow is invalid\n";
            std::cout << "minimum = " << currentMinimum << "\n";
            std::cout << "maximum = " << currentMaximum << "\n";
            std::cout << "maximum - minimum = "
                      << static_cast<std::int64_t>(currentMaximum) -
                             static_cast<std::int64_t>(currentMinimum)
                      << "\n";
            std::cout << "limit = " << limit << "\n";

            std::cout << "Element leaving window: index "
                      << left << ", value = " << values[left] << "\n";

            if (!minDeque.empty() && minDeque.front() == left) {
                std::cout
                    << "Removing index " << minDeque.front()
                    << " from front of minDeque because it expires\n";

                minDeque.pop_front();
            }

            if (!maxDeque.empty() && maxDeque.front() == left) {
                std::cout
                    << "Removing index " << maxDeque.front()
                    << " from front of maxDeque because it expires\n";

                maxDeque.pop_front();
            }

            ++left;

            std::cout << "Moved left boundary to " << left << "\n";

            printDeque("minDeque", minDeque, values);
            printDeque("maxDeque", maxDeque, values);
            printWindow(values, left, right);
        }

        const int currentMinimum = values[minDeque.front()];
        const int currentMaximum = values[maxDeque.front()];
        const std::size_t currentLength = right - left + 1;

        std::cout << "\nWindow is valid\n";
        std::cout << "minimum = " << currentMinimum << "\n";
        std::cout << "maximum = " << currentMaximum << "\n";
        std::cout << "difference = "
                  << static_cast<std::int64_t>(currentMaximum) -
                         static_cast<std::int64_t>(currentMinimum)
                  << "\n";
        std::cout << "current length = " << currentLength << "\n";

        if (currentLength > longest) {
            std::cout << "New longest window found\n";
            std::cout << "Old longest = " << longest << "\n";
            std::cout << "New longest = " << currentLength << "\n";

            longest = currentLength;
        } else {
            std::cout << "Longest remains " << longest << "\n";
        }
    }

    std::cout << "\n========================================\n";
    std::cout << "Final result = " << longest << "\n";

    return longest;
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
    assert(longestStableWindow({7, 8, 9, -1, 22, 0, 7, 7, 7}, 0) == 3);
}

} // namespace

int main()
{
    //runTests();
    //std::cout << "All tests passed.\n";

    longestStableWindow({2,2,3,4,7}, 3);

    return 0;
}
