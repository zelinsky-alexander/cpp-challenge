
#include <cassert>
#include <cstddef>
#include <vector>
#include <queue>
#include <functional>
#include <algorithm>

std::vector<int> topK(const std::vector<int>& values, std::size_t k)
{
   if (values.empty() || k == 0) return {};
   k = std::min(k, values.size());
   
   // Min-heap to maintain the top K elements
   std::priority_queue<int, std::vector<int>, std::greater<int>> minheap;
   for (auto v : values) {
      minheap.push(v);
      if (minheap.size() > k) minheap.pop();
   }

   // Extract elements from heap in descending order
   std::vector<int> results(k);
   for (std::size_t i = 0; i < k; ++i) {
      results[i] = minheap.top();
      minheap.pop();
   }
   return results;
}

int main()
{
    assert(topK({}, 3).empty());
    assert(topK({1, 2, 3}, 0).empty());
    assert(topK({5}, 1) == (std::vector<int>{5}));
    assert((topK({4, 2}, 5) == std::vector<int>{4, 2}));
    assert((topK({5, 1, 9, 3, 9, 7}, 3) == std::vector<int>{9, 9, 7}));
    assert((topK({-5, -1, -9, -3}, 2) == std::vector<int>{-1, -3}));
    assert((topK({2, 2, 2, 2}, 2) == std::vector<int>{2, 2}));

    return 0;
}


