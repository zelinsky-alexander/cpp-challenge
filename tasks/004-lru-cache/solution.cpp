#include <cassert>
#include <cstddef>
#include <iostream>
#include <list>
#include <optional>
#include <unordered_map>

class LruCache {
public:
    explicit LruCache(std::size_t capacity)
        : capacity_(capacity)
    {
    }

    void put(int key, int value)
    {
        // TODO:
        // 1. Handle capacity == 0.
        // 2. If key already exists, update it and mark it most recently used.
        // 3. Otherwise insert one new entry.
        // 4. Evict the least recently used entry when over capacity.
        (void)key;
        (void)value;
    }

    std::optional<int> get(int key)
    {
        // TODO:
        // 1. Find the key in average O(1).
        // 2. Return std::nullopt when missing.
        // 3. Mark a found entry as most recently used without copying it.
        (void)key;
        return std::nullopt;
    }

    [[nodiscard]] std::size_t size() const noexcept
    {
        // TODO: Return the number of currently stored entries.
        return 0;
    }

private:
    struct Entry {
        int key;
        int value;
    };

    using RecencyList = std::list<Entry>;
    using EntryIterator = RecencyList::iterator;

    // TODO: Choose and document which end is most recently used.
    RecencyList entries_;

    // Each mapped iterator must always point to the matching live node in entries_.
    std::unordered_map<int, EntryIterator> by_key_;

    std::size_t capacity_;
};

int main()
{
    {
        LruCache cache(2);

        assert(cache.size() == 0);
        assert(!cache.get(99).has_value());

        cache.put(1, 10);
        cache.put(2, 20);

        assert(cache.size() == 2);
        assert(cache.get(1) == std::optional<int>{10});

        cache.put(3, 30); // Key 2 should be evicted because key 1 was just read.

        assert(!cache.get(2).has_value());
        assert(cache.get(3) == std::optional<int>{30});
        assert(cache.get(1) == std::optional<int>{10});
    }

    {
        LruCache cache(2);

        cache.put(1, 10);
        cache.put(2, 20);
        cache.put(1, 11); // Updating must also refresh recency.
        cache.put(3, 30); // Key 2 should now be evicted.

        assert(cache.size() == 2);
        assert(cache.get(1) == std::optional<int>{11});
        assert(!cache.get(2).has_value());
        assert(cache.get(3) == std::optional<int>{30});
    }

    {
        LruCache cache(1);

        cache.put(-1, -10);
        assert(cache.get(-1) == std::optional<int>{-10});

        cache.put(7, 70);
        assert(!cache.get(-1).has_value());
        assert(cache.get(7) == std::optional<int>{70});
        assert(cache.size() == 1);
    }

    {
        LruCache cache(0);

        cache.put(1, 10);
        assert(cache.size() == 0);
        assert(!cache.get(1).has_value());
    }

    std::cout << "All tests passed\n";
}
