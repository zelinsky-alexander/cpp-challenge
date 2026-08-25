# Task 026 — Dijkstra Shortest Paths

## Goal

Implement Dijkstra's algorithm for a directed weighted graph.

```cpp
struct Edge {
    std::size_t to;
    int weight;
};

using Graph = std::vector<std::vector<Edge>>;

std::vector<long long> dijkstra(const Graph& graph, std::size_t source);
```

Return the shortest distance from `source` to every vertex. For unreachable vertices, return `INF` where:

```cpp
constexpr long long INF = std::numeric_limits<long long>::max();
```

## Requirements

- Use C++20.
- All edge weights are non-negative.
- Throw `std::out_of_range` when `source` is not a valid vertex.
- Do not modify the graph.
- Use a priority queue rather than repeatedly scanning every vertex.
- Target complexity: `O((V + E) log V)`.

## Build and run

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -g \
  tasks/026-dijkstra/solution.cpp -o /tmp/cpp-challenge-026
/tmp/cpp-challenge-026
```
