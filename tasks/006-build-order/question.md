# Task 006 — Build Order with Cycle Detection

## Goal

Implement a function that computes a valid build order for a set of targets and their dependencies:

```cpp
std::optional<std::vector<std::string>> buildOrder(
    const std::vector<std::string>& targets,
    const std::vector<std::pair<std::string, std::string>>& dependencies);
```

Each dependency pair `{target, prerequisite}` means that `prerequisite` must be built before `target`.

Return:

- a vector containing every target exactly once in a valid build order, or
- `std::nullopt` when the dependency graph contains a cycle or references an unknown target.

This models a small Linux build-system dependency resolver.

## Requirements

- Use C++20.
- Target Linux.
- Do not modify the input containers.
- Reject duplicate names in `targets` by returning `std::nullopt`.
- Reject dependencies that reference names absent from `targets`.
- Ignore duplicate dependency edges; they must not incorrectly increase the effective in-degree.
- Return every target exactly once, including targets with no dependencies.
- Detect self-dependencies and longer cycles.
- When several targets are simultaneously buildable, choose the lexicographically smallest one so the result is deterministic.
- Aim for `O((V + E) log V)` time and `O(V + E)` additional space.

## Examples

```text
targets:      app, core, net, tests
dependencies: app -> core
              app -> net
              tests -> core

valid result: core, net, app, tests
```

The exact order is determined by repeatedly selecting the lexicographically smallest target whose prerequisites are already satisfied.

## Concepts being assessed

- Directed-graph representation.
- In-degree bookkeeping.
- Topological ordering.
- Cycle detection.
- Duplicate-edge handling.
- Deterministic output using the STL.
- Clear treatment of invalid input.
- Complexity reasoning relevant to build systems and dependency managers.

## Edge cases

Cover at least:

- Empty target list.
- One target with no dependencies.
- Multiple independent targets.
- A simple dependency chain.
- A target with multiple prerequisites.
- Duplicate target names.
- Duplicate dependency edges.
- An unknown target or prerequisite.
- A self-cycle.
- A cycle involving several targets.

## Build and run

From the repository root:

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -g \
  tasks/006-build-order/solution.cpp \
  -o /tmp/cpp-challenge-006

/tmp/cpp-challenge-006
```

The scaffold compiles, but assertions intentionally fail until the TODO is implemented.

Optional sanitizer build:

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion \
  -g -fsanitize=address,undefined -fno-omit-frame-pointer \
  tasks/006-build-order/solution.cpp \
  -o /tmp/cpp-challenge-006-san

/tmp/cpp-challenge-006-san
```
