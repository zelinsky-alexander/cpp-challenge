#include <cassert>
#include <cstddef>
#include <vector>

std::vector<int> topK(const std::vector<int>& values, std::size_t k);

int main()
{
    assert(topK({}, 3).empty());
    assert(topK({1, 2, 3}, 0).empty());
    assert(topK({5}, 1) == std::vector<int>{5});
    assert(topK({4, 2}, 5) == std::vector<int>({4, 2}));
    assert(topK({5, 1, 9, 3, 9, 7}, 3) == std::vector<int>({9, 9, 7}));
    assert(topK({-5, -1, -9, -3}, 2) == std::vector<int>({-1, -3}));
    assert(topK({2, 2, 2, 2}, 2) == std::vector<int>({2, 2}));

    return 0;
}
