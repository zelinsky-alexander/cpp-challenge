# Task 012 — Expiring Key-Value Store: Explanation

## Goal

We need an in-memory key-value store where every value has an absolute expiration time.

The public interface is:

```cpp
void put(std::string key,
         std::string value,
         std::uint64_t expires_at);

std::optional<std::string> get(
    std::string_view key,
    std::uint64_t now);

std::size_t size(std::uint64_t now);
```

An entry is expired when:

```text
expires_at <= now
```

The important part is that we should not scan the whole store every time we call `get()` or `size()`.

---

## Why one data structure is not enough

We need two different operations to be efficient:

1. Find a key quickly.
2. Find the next entry that expires quickly.

A hash table is good for key lookup:

```text
key -> current entry
```

A min-heap is good for expiration ordering:

```text
earliest expiration on top
```

So the implementation uses both.

---

## The hash table

The current value of every key is stored in:

```cpp
std::unordered_map<std::string, Entry> entries_;
```

Each entry contains:

```cpp
struct Entry {
    std::string value;
    std::uint64_t expires_at{};
    std::uint64_t generation{};
};
```

For example:

```text
"token" -> {
    value = "abc",
    expires_at = 100,
    generation = 7
}
```

The hash table gives average O(1) lookup.

---

## The expiration heap

Every call to `put()` also adds an expiration record to a priority queue.

The record is:

```cpp
struct Expiration {
    std::uint64_t expires_at{};
    std::uint64_t generation{};
    std::string key;
};
```

The priority queue is ordered so that the earliest expiration is on top.

Conceptually:

```text
top
 |
 v
expires at 10
expires at 25
expires at 40
expires at 100
```

This lets cleanup process only entries that are actually due.

---

## Why generations are needed

The difficult case is replacing an existing key.

Example:

```text
put("token", "old", 10)
put("token", "new", 100)
```

The heap now contains two expiration records:

```text
("token", expires 10)
("token", expires 100)
```

But the hash table contains only the newest value:

```text
"token" -> "new", expires 100
```

When time reaches 10, the old heap record must not delete the new value.

That is why every `put()` gets a unique generation number.

For example:

```text
old entry:
generation = 1

new entry:
generation = 2
```

The old heap record still says:

```text
generation = 1
```

but the current map entry says:

```text
generation = 2
```

They do not match, so the old heap record is stale and is ignored.

---

## put()

The implementation starts by creating a new generation:

```cpp
const std::uint64_t generation = next_generation_++;
```

Then it inserts or replaces the map entry:

```cpp
entries_[key] = Entry{
    std::move(value),
    expires_at,
    generation
};
```

After that, it pushes a matching expiration record:

```cpp
expirations_.push(Expiration{
    expires_at,
    generation,
    std::move(key)
});
```

If the key already existed, the hash-table entry is replaced.

The old heap record is intentionally left in the heap.

This is called lazy deletion.

---

## Why leave stale heap records?

Removing an arbitrary old record from a priority queue is inconvenient.

Instead, we leave it there.

Later, when it reaches the top, we check whether it still represents the current entry.

If not, we simply discard it.

This keeps the implementation simple.

---

## purgeExpired()

Both `get()` and `size()` first call:

```cpp
purgeExpired(now);
```

Cleanup continues while the earliest heap record is due:

```cpp
while (!expirations_.empty() &&
       expirations_.top().expires_at <= now) {
    ...
}
```

The `<=` is important.

If:

```text
expires_at = 5
now = 5
```

the entry is already expired.

---

## Checking whether a heap record is current

The earliest expiration record is removed from the heap:

```cpp
const Expiration expiration = expirations_.top();
expirations_.pop();
```

Then we find the key in the hash table:

```cpp
const auto it = entries_.find(expiration.key);
```

We erase the map entry only when the heap record still matches the current map entry:

```cpp
if (it != entries_.end() &&
    it->second.generation == expiration.generation &&
    it->second.expires_at == expiration.expires_at) {
    entries_.erase(it);
}
```

If the generation is different, the heap record belongs to an older version of the key.

It is simply discarded.

---

## Replacement example

Consider:

```text
put("token", "old", 10)
put("token", "new", 100)
size(10)
```

After the first put:

```text
map:
token -> old, expires 10, generation 1

heap:
10, generation 1, token
```

After the second put:

```text
map:
token -> new, expires 100, generation 2

heap:
10, generation 1, token
100, generation 2, token
```

At time 10, cleanup pops:

```text
10, generation 1, token
```

But the map says:

```text
generation 2
```

So the record is stale.

The new value remains alive.

---

## Replacing with an earlier expiration

This also works:

```text
put("early", "first", 200)
put("early", "replacement", 150)
```

The map contains the second value with a newer generation.

At time 150, the newer heap record is processed and deletes the entry.

Later, when the old expiration at 200 reaches the top, the key is already gone, so that stale record is simply discarded.

---

## get()

The implementation first purges expired entries:

```cpp
purgeExpired(now);
```

Then performs a hash-table lookup:

```cpp
const auto it = entries_.find(std::string{key});
```

If the key does not exist:

```cpp
return std::nullopt;
```

Otherwise:

```cpp
return it->second.value;
```

Because cleanup already ran, any entry remaining in the map is live for the supplied `now`.

---

## Why convert string_view to string?

The public API accepts:

```cpp
std::string_view
```

which avoids forcing the caller to construct a `std::string`.

This simple implementation uses a normal:

```cpp
std::unordered_map<std::string, Entry>
```

so lookup uses:

```cpp
entries_.find(std::string{key});
```

That creates a temporary string.

A more advanced version could define transparent hash and equality functions so that the unordered map can be searched directly with `std::string_view`.

For this exercise, the temporary string keeps the code simpler.

---

## size()

`size()` is very small:

```cpp
purgeExpired(now);
return entries_.size();
```

After cleanup, the hash table contains only live entries.

So no full scan is necessary.

---

## Empty keys and values

These are valid:

```cpp
store.put("", "", 500);
```

`std::string` and `std::unordered_map` handle empty strings normally, so no special case is needed.

---

## Reusing a key after expiration

Example:

```text
put("reuse", "expired", 600)
get("reuse", 600)
put("reuse", "live-again", 700)
```

At time 600, the old entry is removed.

The later `put()` simply creates a new entry with a new generation.

So the same key can be reused safely.

---

## Complexity

### put()

Hash-table replacement is average:

```text
O(1)
```

Pushing into the heap is:

```text
O(log n)
```

So `put()` is approximately:

```text
O(log n)
```

### get()

The map lookup is average:

```text
O(1)
```

It may also clean expired heap records.

Each heap record is pushed once and popped at most once, so cleanup cost is amortized across calls.

### size()

It performs expiration cleanup and then returns:

```cpp
entries_.size()
```

It does not scan all live entries.

---

## Memory behavior

The hash table contains only the current version of each live key.

The heap may temporarily contain stale records from older replacements.

For example, repeatedly updating the same key can produce several old heap entries.

Those stale records disappear when their expiration times become due and cleanup pops them.

This is the tradeoff of lazy deletion.

---

## Why nondecreasing now helps

The task allows us to assume that calls use nondecreasing `now` values.

That means once an expiration record is due and removed, time will never later move back before that expiration.

This makes destructive cleanup natural.

If time were allowed to move backward, the API contract would need to change.

Once an expired entry has physically been erased, the store cannot reconstruct it just because a later call supplies an older timestamp.

Possible alternatives would be:

- require monotonic time as this task does;
- never physically delete historical versions;
- define expiration relative to the greatest observed time instead of each individual call.

For a normal cache or service, monotonic logical time is the simplest contract.

---

## Thread safety

This implementation is not thread-safe.

A simple concurrent version could protect:

```text
entries_
expirations_
next_generation_
```

with one `std::mutex`.

Every `put()`, `get()`, and `size()` call would lock that mutex while accessing the shared state.

Using one mutex is not the highest-performance design, but it is the simplest correct synchronization strategy.

---

## Core algorithm

In simplified pseudocode:

```text
put(key, value, expiration):
    generation = next generation

    map[key] =
        value, expiration, generation

    push into min-heap:
        expiration, generation, key


purge(now):
    while earliest heap expiration <= now:
        record = pop heap

        find record.key in map

        if current map entry has
           same generation and expiration:
            erase it

        otherwise:
            record is stale
            ignore it


get(key, now):
    purge(now)

    if key not in map:
        return no value

    return value


size(now):
    purge(now)
    return map size
```

---

## Important interview point

The main idea is not just using a hash table or a heap.

The important part is coordinating both structures correctly.

The invariant is:

> The hash table contains the current version of each key, while the heap may contain both current and stale expiration records.

The generation number lets us distinguish those two cases safely.
