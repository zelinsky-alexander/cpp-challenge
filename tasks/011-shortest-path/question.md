# Task 011 — Shortest Path

Implement shortest_path for a weighted directed graph in C++20.

Requirements:
- return the minimum total cost from source to target;
- return std::nullopt when unreachable;
- return 0 when source equals target;
- use uint64_t for accumulated distance;
- throw std::out_of_range for invalid vertex indices;
- handle cycles, duplicate edges, and zero-cost edges;
- target O((V + E) log V) time and O(V) extra space.

Use:

```cpp
struct Edge { std::size_t to; std::uint32_t weight; };
using Graph = std::vector<std::vector<Edge>>;
std::optional<std::uint64_t> shortest_path(const Graph& graph, std::size_t source, std::size_t target);
```
