#include <cassert>
#include <cstddef>
#include <iostream>
#include <list>
#include <optional>
#include <unordered_map>

/*
 * entries_ owns every cache entry and stores them in recency order:
 *
 *     front = most recently used
 *     back  = least recently used
 *
 * byKey_ maps each key to the corresponding live node in entries_.
 *
 * Invariants:
 *
 * 1. Every cached key appears exactly once in entries_.
 * 2. Every cached key appears exactly once in byKey_.
 * 3. Each iterator stored in byKey_ points to the matching live node
 *    in entries_.
 * 4. entries_.size() == byKey_.size().
 * 5. entries_.size() never exceeds capacity_.
 *
 * std::list::splice moves an existing node without copying it and without
 * invalidating iterators to that node. Therefore, when an entry becomes the
 * most recently used, its iterator stored in byKey_ remains valid.
 *
 * During eviction, the map entry is erased before the list node is destroyed.
 * This prevents byKey_ from temporarily containing a dangling iterator.
 *
 * std::unordered_map lookup, insertion, and erase are average O(1).
 * std::list insertion, removal, and splice are O(1).
 * Therefore, get() and put() are average O(1).
 */

class LruCache {
public:
    explicit LruCache(std::size_t capacity)
        : capacity_(capacity)
    {
    }

    void put(int key, int value)
    {
        if (capacity_ == 0) {
            return;
        }

        const auto found = byKey_.find(key);

        if (found != byKey_.end()) {
            // Update the existing node.
            found->second->value = value;

            // Move the same node to the MRU position.
            entries_.splice(
                entries_.begin(),
                entries_,
                found->second);

            return;
        }

        // A new entry requires one free slot.
        if (entries_.size() == capacity_) {
            const int leastRecentlyUsedKey = entries_.back().key;

            // Remove the lookup entry before destroying its list node.
            byKey_.erase(leastRecentlyUsedKey);
            entries_.pop_back();
        }

        entries_.push_front(Entry{key, value});
        byKey_.emplace(key, entries_.begin());
    }

    std::optional<int> get(int key)
    {
        const auto found = byKey_.find(key);

        if (found == byKey_.end()) {
            return std::nullopt;
        }

        // Move the existing node to the MRU position.
        // The stored iterator remains valid after splice.
        entries_.splice(
            entries_.begin(),
            entries_,
            found->second);

        return found->second->value;
    }

    [[nodiscard]] std::size_t size() const noexcept
    {
        return entries_.size();
    }

private:
    struct Entry {
        int key;
        int value;
    };

    using RecencyList = std::list<Entry>;
    using EntryIterator = RecencyList::iterator;

    RecencyList entries_;
    std::unordered_map<int, EntryIterator> byKey_;
    std::size_t capacity_;
};

namespace {

void runTests()
{
    {
        LruCache cache(2);

        assert(cache.size() == 0);
        assert(!cache.get(99).has_value());

        cache.put(1, 10);
        cache.put(2, 20);

        assert(cache.size() == 2);
        assert(cache.get(1) == std::optional<int>{10});

        // Recency before insertion: [1, 2].
        // Key 2 is the LRU and should be evicted.
        cache.put(3, 30);

        assert(!cache.get(2).has_value());
        assert(cache.get(3) == std::optional<int>{30});
        assert(cache.get(1) == std::optional<int>{10});
        assert(cache.size() == 2);
    }

    {
        LruCache cache(2);

        cache.put(1, 10);
        cache.put(2, 20);

        // Updating key 1 also makes it the MRU.
        cache.put(1, 11);

        // Key 2 is now the LRU.
        cache.put(3, 30);

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

    {
        LruCache cache(3);

        cache.put(1, 10);
        cache.put(2, 20);
        cache.put(3, 30);

        // Repeatedly accessing the current MRU must remain safe.
        assert(cache.get(3) == std::optional<int>{30});
        assert(cache.get(3) == std::optional<int>{30});

        cache.put(4, 40);

        // Key 1 remained the LRU.
        assert(!cache.get(1).has_value());
        assert(cache.get(2) == std::optional<int>{20});
        assert(cache.get(3) == std::optional<int>{30});
        assert(cache.get(4) == std::optional<int>{40});
    }

    {
        LruCache cache(2);

        for (int key = 0; key < 100; ++key) {
            cache.put(key, key * 10);
            assert(cache.size() <= 2);
        }

        assert(!cache.get(97).has_value());
        assert(cache.get(98) == std::optional<int>{980});
        assert(cache.get(99) == std::optional<int>{990});
    }
}

} // namespace

int main()
{
    runTests();
    std::cout << "All tests passed\n";
    return 0;
}