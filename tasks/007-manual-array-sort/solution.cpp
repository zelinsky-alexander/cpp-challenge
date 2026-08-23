#include <cassert>
#include <vector>
#include <iostream>

void sortArray(std::vector<int>& values)
{
    const size_t sz = values.size();

    if (sz <= 1) {
        //std::cout << "nothing to sort, go away" << std::endl;
        return;
    }
    
    for (size_t i = 0; i < sz; ++i) {
        size_t smallest = i;
        for (size_t j = i + 1; j < sz; ++j) {
            if (values[j] < values[smallest]) {
                smallest = j;
            }
        }
        std::swap(values[i], values[smallest]);
    }
}

int main()
{
    {
        std::vector<int> values{7, 2, 9, 1, 5, 0, 11, 100, -5, 999, 0, 1};
        sortArray(values);
        assert((values == std::vector<int>{-5, 0, 0, 1, 1, 2, 5, 7, 9, 11, 100, 999}));
    }

    {
        std::vector<int> values{4, 1, 1, 3};
        sortArray(values);
        assert((values == std::vector<int>{1, 1, 3, 4}));
    }

    {
        std::vector<int> values{};
        sortArray(values);
        assert(values.empty());
    }

    {
        std::vector<int> values{5};
        sortArray(values);
        assert((values == std::vector<int>{5}));
    }

    {
        std::vector<int> values{-1, -5, 3, 0, -5};
        sortArray(values);
        assert((values == std::vector<int>{-5, -5, -1, 0, 3}));
    }

    {
        std::vector<int> values{1, 2, 3, 4, 5};
        sortArray(values);
        assert((values == std::vector<int>{1, 2, 3, 4, 5}));
    }

    {
        std::vector<int> values{5, 4, 3, 2, 1};
        sortArray(values);
        assert((values == std::vector<int>{1, 2, 3, 4, 5}));
    }

    std::cout << "All passed\n";

    return 0;
}
