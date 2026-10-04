#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <optional>
#include <queue>
#include <stdexcept>
#include <utility>
#include <vector>

struct Edge {
    std::size_t to;
    std::uint32_t weight;
};

using Graph = std::vector<std::vector<Edge>>;

std::optional<std::uint64_t> shortest_path(
    const Graph& graph,
    std::size_t source,
    std::size_t target)
{
    if (source >= graph.size() || target >= graph.size()) {
        throw std::out_of_range("invalid vertex index");
    }

    if (source == target) {
        return 0;
    }

    const auto infinity = std::numeric_limits<std::uint64_t>::max();

    std::vector<std::uint64_t> distance(graph.size(), infinity);
    distance[source] = 0;

    using QueueItem = std::pair<std::uint64_t, std::size_t>;
    std::priority_queue<QueueItem, std::vector<QueueItem>, std::greater<>> queue;

    queue.push({0, source});

    while (!queue.empty()) {
        const auto [current_distance, vertex] = queue.top();
        queue.pop();

        if (current_distance != distance[vertex]) {
            continue;
        }

        if (vertex == target) {
            return current_distance;
        }

        for (const Edge& edge : graph[vertex]) {
            if (edge.to >= graph.size()) {
                throw std::out_of_range("invalid edge destination");
            }

            const std::uint64_t new_distance =
                current_distance + static_cast<std::uint64_t>(edge.weight);

            if (new_distance < distance[edge.to]) {
                distance[edge.to] = new_distance;
                queue.push({new_distance, edge.to});
            }
        }
    }

    return std::nullopt;
}

namespace {

void test_simple_path()
{
    const Graph graph{
        {{1, 4}, {2, 10}},
        {{2, 3}},
        {}
    };

    assert(shortest_path(graph, 0, 2) == std::optional<std::uint64_t>{7});
}

void test_direct_edge_is_shorter()
{
    const Graph graph{
        {{1, 10}, {2, 3}},
        {{3, 2}},
        {{3, 4}},
        {}
    };

    assert(shortest_path(graph, 0, 3) == std::optional<std::uint64_t>{7});
}

void test_unreachable()
{
    const Graph graph{
        {{1, 1}},
        {},
        {}
    };

    assert(!shortest_path(graph, 0, 2).has_value());
}

void test_source_equals_target()
{
    const Graph graph{
        {{1, 5}},
        {}
    };

    assert(shortest_path(graph, 1, 1) == std::optional<std::uint64_t>{0});
}

void test_cycle()
{
    const Graph graph{
        {{1, 2}},
        {{2, 2}},
        {{0, 2}, {3, 1}},
        {}
    };

    assert(shortest_path(graph, 0, 3) == std::optional<std::uint64_t>{5});
}

void test_duplicate_edges()
{
    const Graph graph{
        {{1, 10}, {1, 2}},
        {}
    };

    assert(shortest_path(graph, 0, 1) == std::optional<std::uint64_t>{2});
}

void test_zero_cost_edges()
{
    const Graph graph{
        {{1, 0}},
        {{2, 0}},
        {}
    };

    assert(shortest_path(graph, 0, 2) == std::optional<std::uint64_t>{0});
}

void test_invalid_source()
{
    const Graph graph{{}};

    bool threw = false;

    try {
        (void)shortest_path(graph, 1, 0);
    } catch (const std::out_of_range&) {
        threw = true;
    }

    assert(threw);
}

void test_invalid_target()
{
    const Graph graph{{}};

    bool threw = false;

    try {
        (void)shortest_path(graph, 0, 1);
    } catch (const std::out_of_range&) {
        threw = true;
    }

    assert(threw);
}

void test_invalid_edge_destination()
{
    const Graph graph{
        {{5, 1}},
        {}
    };

    bool threw = false;

    try {
        (void)shortest_path(graph, 0, 1);
    } catch (const std::out_of_range&) {
        threw = true;
    }

    assert(threw);
}

} // namespace

int main()
{
    test_simple_path();
    test_direct_edge_is_shorter();
    test_unreachable();
    test_source_equals_target();
    test_cycle();
    test_duplicate_edges();
    test_zero_cost_edges();
    test_invalid_source();
    test_invalid_target();
    test_invalid_edge_destination();
}
