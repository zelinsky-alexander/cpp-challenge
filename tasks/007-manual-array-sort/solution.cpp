#include <cassert>
#include <vector>

void sortArray(std::vector<int>& values)
{
}

int main()
{
    {
        std::vector<int> values{7, 2, 9, 1, 5};
        sortArray(values);
        assert((values == std::vector<int>{1, 2, 5, 7, 9}));
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

    return 0;
}
