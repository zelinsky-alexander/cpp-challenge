#include <cassert>
#include <functional>
#include <iostream>
#include <optional>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <cassert>

// Solution: Kahn’s topological-sort algorithm with a min-heap

std::optional<std::vector<std::string>> buildOrder(
    const std::vector<std::string>& targets,
    const std::vector<std::pair<std::string, std::string>>& dependencies)
{
    // adjacency[prerequisite] contains targets that depend on it.
    std::unordered_map<std::string, std::unordered_set<std::string>> adjacency;

    // Number of prerequisites that each target still has.
    std::unordered_map<std::string, std::size_t> in_degree;

    // First build the set of valid targets and detect duplicate names.
    for (const auto& target : targets) {
        const auto [it, inserted] = in_degree.emplace(target, 0);
		//std::cout << 
        if (!inserted) {
            return std::nullopt;
        }

        adjacency.emplace(target, std::unordered_set<std::string>{});
    }

    // A pair {target, prerequisite} means:
    //
    // prerequisite ---> target
    //
    // Duplicate edges are ignored by the unordered_set.
    for (const auto& [target, prerequisite] : dependencies) {
        if (!in_degree.contains(target) ||
            !in_degree.contains(prerequisite)) {
            return std::nullopt;
        }

        auto& dependents = adjacency[prerequisite];

        const auto [it, inserted] = dependents.insert(target);

        if (inserted) {
            ++in_degree[target];
        }
    }

    // Min-heap: among all currently buildable targets,
    // always select the lexicographically smallest one.
    std::priority_queue<
        std::string,
        std::vector<std::string>,
        std::greater<>
    > ready;

    for (const auto& target : targets) {
        if (in_degree[target] == 0) {
            ready.push(target);
        }
    }

    std::vector<std::string> result;
    result.reserve(targets.size());

    while (!ready.empty()) {
        std::string current = ready.top();
        ready.pop();

        result.push_back(current);

        for (const auto& dependent : adjacency[current]) {
            auto& degree = in_degree[dependent];

            --degree;

            if (degree == 0) {
                ready.push(dependent);
            }
        }
    }

    // If some targets could never become buildable, they belong
    // to a dependency cycle.
    if (result.size() != targets.size()) {
        return std::nullopt;
    }

    return result;
}

int main()
{
    {
        const std::vector<std::string> targets;
        const std::vector<std::pair<std::string, std::string>> dependencies;

        const auto result = buildOrder(targets, dependencies);

        assert(result.has_value());
        assert(result->empty());
    }

    {
        const std::vector<std::string> targets{"app"};
        const std::vector<std::pair<std::string, std::string>> dependencies;

        const auto result = buildOrder(targets, dependencies);

        assert(result == std::optional<std::vector<std::string>>{
            {"app"}
        });
    }

    {
        const std::vector<std::string> targets{"c", "a", "b"};
        const std::vector<std::pair<std::string, std::string>> dependencies;

        const auto result = buildOrder(targets, dependencies);

        assert((result == std::optional<std::vector<std::string>>{
			{"a", "b", "c"}
		}));
    }

    {
        const std::vector<std::string> targets{
            "app", "core", "net", "tests"
        };

        const std::vector<std::pair<std::string, std::string>> dependencies{
            {"app", "core"},
            {"app", "net"},
            {"tests", "core"}
        };

        const auto result = buildOrder(targets, dependencies);

        assert((result == std::optional<std::vector<std::string>>{{"core", "net", "app", "tests"}}));
    }

    {
        const std::vector<std::string> targets{
            "a", "b", "c", "d"
        };

        const std::vector<std::pair<std::string, std::string>> dependencies{
            {"b", "a"},
            {"c", "b"},
            {"d", "c"}
        };

        const auto result = buildOrder(targets, dependencies);

        assert((result == std::optional<std::vector<std::string>>{{"a", "b", "c", "d"}}));
    }

    {
        // Duplicate targets are invalid.
        const std::vector<std::string> targets{"a", "b", "a"};

        const auto result = buildOrder(targets, {});

        assert(!result.has_value());
    }

    {
        // Duplicate dependency must count only once.
        const std::vector<std::string> targets{"a", "b"};

        const std::vector<std::pair<std::string, std::string>> dependencies{
            {"b", "a"},
            {"b", "a"}
        };

        const auto result = buildOrder(targets, dependencies);

        assert((result == std::optional<std::vector<std::string>>{{"a", "b"}}));
    }

    {
        // Unknown target.
        const std::vector<std::string> targets{"a", "b"};

        const std::vector<std::pair<std::string, std::string>> dependencies{
            {"c", "a"}
        };

        assert(!buildOrder(targets, dependencies).has_value());
    }

    {
        // Unknown prerequisite.
        const std::vector<std::string> targets{"a", "b"};

        const std::vector<std::pair<std::string, std::string>> dependencies{
            {"b", "x"}
        };

        assert(!buildOrder(targets, dependencies).has_value());
    }

    {
        // Self-cycle.
        const std::vector<std::string> targets{"a"};

        const std::vector<std::pair<std::string, std::string>> dependencies{
            {"a", "a"}
        };

        assert(!buildOrder(targets, dependencies).has_value());
    }

    {
        // Longer cycle:
        //
        // a -> b -> c -> a
        const std::vector<std::string> targets{"a", "b", "c"};

        const std::vector<std::pair<std::string, std::string>> dependencies{
            {"b", "a"},
            {"c", "b"},
            {"a", "c"}
        };

        assert(!buildOrder(targets, dependencies).has_value());
    }

    std::cout << "All tests passed\n";
}