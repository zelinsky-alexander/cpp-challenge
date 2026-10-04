# Task 011 — Shortest Path: Explanation

## Goal

We need to find the minimum total cost from one vertex to another in a weighted directed graph.

The graph type is:

```cpp
struct Edge {
    std::size_t to;
    std::uint32_t weight;
};

using Graph = std::vector<std::vector<Edge>>;
```

Each index in the outer vector is a vertex.

For example:

```text
graph[0] = { {1, 4}, {2, 10} }
```

means:

```text
0 -> 1 with cost 4
0 -> 2 with cost 10
```

The weights are unsigned, so there are no negative edges.

That makes Dijkstra's algorithm a good fit.

---

## Main idea

For every vertex, keep the best distance found so far.

At the beginning:

```text
source distance = 0
all other distances = infinity
```

Then repeatedly process the vertex with the smallest known distance.

If going through that vertex gives us a cheaper path to one of its neighbors, update the neighbor's distance.

This is called relaxation.

---

## Example

Suppose the graph is:

```text
0 --4--> 1 --3--> 2
 \                 ^
  \------10--------/
```

We want the shortest path from 0 to 2.

Initially:

```text
distance[0] = 0
distance[1] = infinity
distance[2] = infinity
```

From vertex 0:

```text
0 -> 1 costs 4
0 -> 2 costs 10
```

So now:

```text
distance[1] = 4
distance[2] = 10
```

The next closest vertex is 1.

Going through vertex 1 gives:

```text
0 -> 1 -> 2
cost = 4 + 3 = 7
```

That is better than 10, so:

```text
distance[2] = 7
```

The final answer is:

```text
7
```

---

## Distance array

The implementation starts with:

```cpp
const auto infinity =
    std::numeric_limits<std::uint64_t>::max();

std::vector<std::uint64_t> distance(
    graph.size(),
    infinity);

distance[source] = 0;
```

Using `std::uint64_t` for accumulated distance is required because many `std::uint32_t` edge weights may be added together.

---

## Why use a priority queue?

We always want to process the vertex with the smallest known distance.

A min-priority queue is ideal for this.

The queue stores:

```text
(distance, vertex)
```

The type is:

```cpp
using QueueItem =
    std::pair<std::uint64_t, std::size_t>;
```

and:

```cpp
std::priority_queue<
    QueueItem,
    std::vector<QueueItem>,
    std::greater<>
> queue;
```

Normally `std::priority_queue` is a max-heap.

Using `std::greater<>` turns it into a min-heap.

So the smallest distance is always on top.

---

## Starting the algorithm

The source has distance zero:

```cpp
queue.push({0, source});
```

Then process vertices until the queue is empty:

```cpp
while (!queue.empty()) {
    ...
}
```

---

## Removing the closest vertex

We take the smallest item:

```cpp
const auto [current_distance, vertex] = queue.top();
queue.pop();
```

This gives us the next vertex to examine.

---

## Why can the same vertex appear more than once?

Suppose we first discover:

```text
distance[2] = 10
```

so we push:

```text
(10, 2)
```

Later we find a better path:

```text
distance[2] = 7
```

and push:

```text
(7, 2)
```

Now the priority queue contains both entries.

Instead of trying to remove the old one, the implementation simply ignores stale entries:

```cpp
if (current_distance != distance[vertex]) {
    continue;
}
```

When `(10, 2)` eventually comes out of the queue, we already know:

```text
distance[2] = 7
```

so the old value 10 is ignored.

This is a common and simple Dijkstra implementation.

---

## Relaxing edges

For every outgoing edge:

```cpp
for (const Edge& edge : graph[vertex]) {
    ...
}
```

we calculate the cost of reaching the neighbor through the current vertex:

```cpp
const std::uint64_t new_distance =
    current_distance +
    static_cast<std::uint64_t>(edge.weight);
```

Then compare it with the best known distance:

```cpp
if (new_distance < distance[edge.to]) {
    distance[edge.to] = new_distance;
    queue.push({new_distance, edge.to});
}
```

This is the key step in Dijkstra's algorithm.

---

## Early return

When the target vertex is removed from the priority queue with its current best distance:

```cpp
if (vertex == target) {
    return current_distance;
}
```

we can return immediately.

Because all edge weights are non-negative, no later route can produce a smaller distance to that target.

---

## Unreachable target

If the queue becomes empty before reaching the target:

```cpp
return std::nullopt;
```

That means there is no path from source to target.

---

## Source equals target

The shortest path from a vertex to itself costs zero:

```cpp
if (source == target) {
    return 0;
}
```

No graph traversal is needed.

---

## Invalid indices

The function first checks:

```cpp
if (source >= graph.size() ||
    target >= graph.size()) {
    throw std::out_of_range("invalid vertex index");
}
```

An edge destination is also checked before it is used:

```cpp
if (edge.to >= graph.size()) {
    throw std::out_of_range(
        "invalid edge destination");
}
```

This avoids indexing outside the graph vector.

---

## Cycles

Cycles do not cause an infinite loop.

For example:

```text
0 -> 1 -> 2 -> 0
```

A vertex is only updated when a strictly shorter distance is found:

```cpp
if (new_distance < distance[edge.to])
```

With non-negative weights, repeatedly following a cycle cannot keep producing smaller and smaller distances.

---

## Duplicate edges

Duplicate edges are also fine.

Example:

```text
0 -> 1 cost 10
0 -> 1 cost 2
```

The first edge may set:

```text
distance[1] = 10
```

Then the second edge improves it:

```text
distance[1] = 2
```

The priority queue may contain both values, but the stale-entry check ignores the old one.

---

## Zero-cost edges

Zero-cost edges work normally:

```text
0 -> 1 cost 0
1 -> 2 cost 0
```

The shortest distance from 0 to 2 is:

```text
0
```

Dijkstra requires non-negative edges, and zero is allowed.

---

## Complexity

Let:

```text
V = number of vertices
E = number of edges
```

Each useful distance update pushes an item into the priority queue.

Priority-queue operations cost:

```text
O(log V)
```

Overall complexity is approximately:

```text
O((V + E) log V)
```

The distance vector uses:

```text
O(V)
```

extra space, while the priority queue may also hold pending entries.

---

## Core algorithm

In simplified pseudocode:

```text
validate source and target

if source == target:
    return 0

distance[source] = 0
all others = infinity

push source into min-heap

while heap is not empty:
    take vertex with smallest distance

    if entry is stale:
        skip it

    if vertex is target:
        return its distance

    for each outgoing edge:
        new distance =
            current distance + edge weight

        if new distance is better:
            save it
            push neighbor into heap

return no path
```

---

## Important interview point

The central idea is:

> Always expand the not-yet-processed vertex with the smallest known distance.

Because all weights are non-negative, once the target comes out of the min-heap with its best current distance, that distance is final.

If negative edge weights were allowed, this algorithm would not be correct and a different algorithm such as Bellman-Ford would be needed.
