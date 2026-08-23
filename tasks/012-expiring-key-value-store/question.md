# Task 012 — Expiring Key-Value Store

## Goal

Implement an in-memory key-value store whose entries expire at an absolute timestamp:

```cpp
class ExpiringStore {
public:
    void put(std::string key,
             std::string value,
             std::uint64_t expires_at);

    std::optional<std::string> get(
        std::string_view key,
        std::uint64_t now);

    std::size_t size(std::uint64_t now);
};
```

The store is intended for a Linux service that keeps short-lived metadata such as session information, DNS-like records, or cached discovery results.

An entry is expired when `expires_at <= now`.

## Requirements

- Use C++20 and target Linux.
- `put()` inserts a new entry or replaces the value and expiration time of an existing key.
- `get()` returns the current value when the key exists and has not expired; otherwise it returns `std::nullopt`.
- `size()` returns the number of currently live entries.
- Both `get()` and `size()` must remove entries that are expired as of `now`.
- Updating a key must not allow an older expiration record to delete the newer value.
- Empty keys and empty values are valid.
- Timestamps are supplied by the caller; do not read the system clock.
- Aim for:
  - average `O(1)` lookup and replacement;
  - amortized `O(log n)` insertion and expiration processing;
  - `O(n)` additional space, excluding stale heap records awaiting cleanup.
- Do not scan every stored entry on each `get()` or `size()` call.
- Thread safety is not required, but be ready to explain what synchronization would be needed in a concurrent version.

## Suggested direction

Consider combining:

- a hash table for current values and expiration metadata;
- a min-heap ordered by expiration time;
- a generation or version value that distinguishes the current entry from stale heap records created by earlier updates.

You are not required to use this exact representation, but your complexity should remain comparable.

## Semantics to define carefully

Given:

```text
put("token", "old", 10)
put("token", "new", 100)
size(10)
```

The old heap record may now be due, but the replacement value must remain alive until timestamp `100`.

Similarly:

```text
put("a", "value", 5)
get("a", 5)
```

must return no value because expiration is inclusive.

## Concepts being assessed

- Coordinating multiple data structures while preserving invariants.
- Hash-table lookup and priority-queue expiration.
- Lazy deletion and stale-record detection.
- Replacement semantics.
- `std::optional`, `std::string_view`, and move-aware interfaces.
- Complexity and memory-growth reasoning.
- API design suitable for deterministic testing.

## Edge cases

Cover at least:

- An empty store.
- A live entry.
- An entry queried exactly at its expiration time.
- Multiple entries expiring at the same timestamp.
- Replacing a key with a later expiration.
- Replacing a key with an earlier expiration.
- Reusing a key after its previous value expired.
- Empty key and empty value.
- Calls where `now` moves forward by a large amount.

You may assume calls use nondecreasing `now` values. Explain how you would change the contract or implementation if time were allowed to move backward.

## Build and run

From the repository root:

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -g \
  tasks/007-expiring-key-value-store/solution.cpp \
  -o /tmp/cpp-challenge-007

/tmp/cpp-challenge-007
```

The scaffold compiles, but assertions intentionally fail until the TODOs are implemented.

Optional sanitizer build:

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion \
  -g -fsanitize=address,undefined -fno-omit-frame-pointer \
  tasks/007-expiring-key-value-store/solution.cpp \
  -o /tmp/cpp-challenge-007-san

/tmp/cpp-challenge-007-san
```
