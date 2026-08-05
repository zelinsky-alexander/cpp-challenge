#include <algorithm>
#include <cassert>
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>

using Dependency = std::pair<std::string, std::string>;

std::optional<std::vector<std::string>> buildOrder(
    const std::vector<std::string>& targets,
    const std::vector<Dependency>& dependencies)
{
    // TODO:
    // 1. Validate that target names are unique.
    // 2. Build a directed graph from prerequisite to dependent target.
    // 3. Reject dependency endpoints that are not known targets.
    // 4. Ensure duplicate edges are counted only once.
    // 5. Track each target's in-degree.
    // 6. Repeatedly select the lexicographically smallest zero-in-degree target.
    // 7. Return std::nullopt if not every target can be emitted.
    (void)targets;
    (void)dependencies;
    return std::nullopt;
}

int main()
{
    using Result = std::optional<std::vector<std::string>>;

    assert(buildOrder({}, {}) == Result{std::vector<std::string>{}});

    assert(buildOrder({"core"}, {}) ==
           Result{std::vector<std::string>{"core"}});

    assert(buildOrder({"zlib", "app", "core"}, {}) ==
           Result{std::vector<std::string>{"app", "core", "zlib"}});

    assert(buildOrder(
               {"app", "core", "net", "tests"},
               {{"app", "core"}, {"app", "net"}, {"tests", "core"}}) ==
           Result{std::vector<std::string>{"core", "net", "app", "tests"}});

    assert(buildOrder(
               {"package", "compile", "link", "test"},
               {{"compile", "package"},
                {"link", "compile"},
                {"test", "link"}}) ==
           Result{std::vector<std::string>{
               "package", "compile", "link", "test"}});

    assert(buildOrder(
               {"app", "core"},
               {{"app", "core"}, {"app", "core"}}) ==
           Result{std::vector<std::string>{"core", "app"}});

    assert(!buildOrder({"app", "app"}, {}).has_value());
    assert(!buildOrder({"app"}, {{"app", "missing"}}).has_value());
    assert(!buildOrder({"app"}, {{"missing", "app"}}).has_value());
    assert(!buildOrder({"app"}, {{"app", "app"}}).has_value());

    assert(!buildOrder(
                {"a", "b", "c"},
                {{"a", "b"}, {"b", "c"}, {"c", "a"}})
                .has_value());

    std::cout << "All assertions passed\n";
    return 0;
}
