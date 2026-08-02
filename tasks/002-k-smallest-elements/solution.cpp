#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <vector>

using namespace std;

// TODO: Explain the full-sort baseline and its complexity.
// TODO: Name the STL data structure used for the bounded candidate set.
// TODO: State whether its top element is the smallest or largest retained value.
// TODO: State the invariant after each processed input element.
// TODO: Give total time and auxiliary-space complexity, including result ordering.

void insertSorted(std::vector<int>& sortedVector, int newVal) {
        auto pos = std::lower_bound(sortedVector.begin(), sortedVector.end(), newVal);
        cout << "about to insert " << newVal << endl;
        sortedVector.insert(pos, newVal);
}

std::vector<int> kSmallest(
    const std::vector<int>& values,
    std::size_t k)
{
        std::vector<int> candidates;

        if (k == 0)
                return candidates;

        for (size_t i = 0; i < values.size(); ++i) {
                if (candidates.empty()) {
                        cout << "candiadets empty, push back " << values[i] << endl;
                        candidates.push_back(values[i]);
                        continue;
                }
                if (candidates.size() >= k) {
                        if (values[i] >= candidates.back()) {
                                cout << values[i] << " too big, already enough candidates, ignore" << endl;
                                continue;
                        } else {
                                int largestSoFar = candidates.back();
                                cout << largestSoFar << " pop back largest so far" << endl;
                                candidates.pop_back();
                                insertSorted(candidates, values[i]);
                        }
                } else {
                        cout << candidates.size() << " is candidates size so far" << endl;
                        insertSorted(candidates, values[i]);
                }

        }

        return candidates;
}

namespace {

void runTests()
{
        cout << "***************" << endl << "kSmallest({7, 2, 9, 1, 5}, 3) == std::vector<int>{1, 2, 5}" << endl;
        assert((kSmallest({7, 2, 9, 1, 5}, 3) == std::vector<int>{1, 2, 5}));

        cout << "***************" << endl << "kSmallest({4, 1, 1, 3}, 3) == std::vector<int>{1, 1, 3}" << endl;
        assert((kSmallest({4, 1, 1, 3}, 3) == std::vector<int>{1, 1, 3}));

        cout << "***************" << endl << "kSmallest({5, 2}, 0).empty()" << endl;
        assert(kSmallest({5, 2}, 0).empty());

        cout << "***************" << endl;
        assert((kSmallest({5, 2}, 5) == std::vector<int>{2, 5}));

        cout << "***************" << endl;
        assert(kSmallest({}, 2).empty());

        cout << "***************" << endl;
        assert((kSmallest({-1, -5, 3, 0, -5}, 4) == std::vector<int>{-5, -5, -1, 0}));

        cout << "***************" << endl;
        assert((kSmallest({6, 6, 6, 6}, 2) == std::vector<int>{6, 6}));

        cout << "***************" << endl;
        assert((kSmallest({1, 2, 3, 4, 5}, 1) == std::vector<int>{1}));

        cout << "***************" << endl;
        assert((kSmallest({5, 4, 3, 2, 1}, 4) == std::vector<int>{1, 2, 3, 4}));
}

} // namespace

int main()
{
    runTests();
    std::cout << "\nAll tests passed.\n";
    return 0;
}
