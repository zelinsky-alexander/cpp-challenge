#include <cassert>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <vector>

struct Edge {
    std::size_t to;
    int weight;
};

using Graph = std::vector<std::vector<Edge>>;
constexpr long long INF = std::numeric_limits<long long>::max();

std::vector<long long> dijkstra(const Graph& graph, std::size_t source);

int main()
{
    Graph graph(5);
    graph[0] = {{1, 4}, {2, 1}};
    graph[2] = {{1, 2}, {3, 5}};
    graph[1] = {{3, 1}};
    graph[3] = {{4, 3}};

    assert(dijkstra(graph, 0) == std::vector<long long>({0, 3, 1, 4, 7}));
    assert(dijkstra(graph, 2) == std::vector<long long>({INF, 2, 0, 3, 6}));

    Graph disconnected(3);
    disconnected[0] = {{1, 7}};
    assert(dijkstra(disconnected, 0) == std::vector<long long>({0, 7, INF}));

    bool threw = false;
    try { static_cast<void>(dijkstra(disconnected, 3)); } catch (const std::out_of_range&) { threw = true; }
    assert(threw);

    return 0;
}
