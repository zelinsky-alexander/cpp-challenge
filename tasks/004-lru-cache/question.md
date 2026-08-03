# Task 004 — Fixed-Capacity LRU Cache

## Goal

Implement an integer key/value least-recently-used cache:

```cpp
class LruCache {
public:
    explicit LruCache(std::size_t capacity);

    void put(int key, int value);
    std::optional<int> get(int key);

    [[nodiscard]] std::size_t size() const noexcept;
};
```

The cache stores at most `capacity` entries.

- `get(key)` returns the stored value when the key exists and marks that entry as the most recently used.
- `get(key)` returns `std::nullopt` when the key does not exist.
- `put(key, value)` inserts a new entry as the most recently used.
- Updating an existing key changes its value and marks it as the most recently used.
- When insertion exceeds the capacity, evict the least recently used entry.
- A cache with capacity `0` never stores entries.

## Example

```text
capacity = 2

put(1, 10)          cache: [1]
put(2, 20)          cache: [2, 1]
get(1) -> 10        cache: [1, 2]
put(3, 30)          evicts key 2; cache: [3, 1]
get(2) -> missing
put(1, 11)          cache: [1, 3]
get(1) -> 11
```

The order shown above is most-recently-used to least-recently-used.

## Requirements

- Use C++20.
- Target average complexity: `O(1)` for both `get` and `put`.
- Use standard-library containers.
- Do not scan all entries to find the least recently used key.
- Do not store duplicate nodes for the same key.
- Keep the lookup structure and recency structure consistent after every operation.
- Avoid unnecessary copying when moving an existing entry to the most-recently-used position.

## Intended data structures

A typical design combines:

- a linked sequence that owns entries in recency order;
- a hash table that maps each key to its position in that sequence.

Before coding, determine:

1. Which end represents the most recently used entry.
2. Which end represents the least recently used entry.
3. What the hash table should store as its mapped value.
4. How to move an existing entry without erasing and recreating it.
5. Which structure must be updated first during eviction so no dangling iterator remains.

## Invariants to explain in `solution.cpp`

Add brief comments describing:

1. The relationship between the linked sequence and hash table.
2. Why each key appears exactly once.
3. Why every hash-table iterator always refers to a live sequence node.
4. Why `get` and `put` are average `O(1)`.
5. Which operations could invalidate iterators and why your chosen operations are safe.

## Edge cases

Consider:

- Capacity `0`.
- Capacity `1`.
- Reading a missing key.
- Updating an existing key.
- Repeatedly reading the current most recently used key.
- Eviction immediately after an update.
- Negative keys and values.
- A long sequence of insertions with many evictions.

## Build and run

From the repository root:

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -g \
  tasks/004-lru-cache/solution.cpp \
  -o /tmp/cpp-challenge-004

/tmp/cpp-challenge-004
```

The initial scaffold compiles, but assertions intentionally fail until the TODOs are implemented.

Sanitizer build:

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion \
  -g -fsanitize=address,undefined -fno-omit-frame-pointer \
  tasks/004-lru-cache/solution.cpp \
  -o /tmp/cpp-challenge-004-san

/tmp/cpp-challenge-004-san
```
