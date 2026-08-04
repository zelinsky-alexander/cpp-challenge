#include <cassert>
#include <cstddef>
#include <iostream>
#include <list>
#include <optional>
#include <unordered_map>

using namespace std;

class LruCache {
public:
    explicit LruCache(std::size_t capacity)
        : capacity_(capacity)
    {
    }

    void put(int key, int value)
    {
        // 1. If capacity == 0 - go away
        // 2. If key already exists, update it and mark it most recently used.
        // 3. Otherwise insert one new entry.
        // 4. Evict the least recently used entry when over capacity.

		if (capacity_ == 0) {
			cout << "Zero capacity, go away\n";
			return;
		}
		
		auto it = by_key_.find(key);
		if (it == by_key_.end()) {
			
			if (entries_.size() == capacity_) {
				Entry lru = entries_.back();
				entries_.pop_back();
				by_key_.erase(lru.key);
				cout << "Cache is full, evict LRU " << lru.key << endl;
			}
			
			cout << key << " not found, put it in cache as MRU\n";
			entries_.push_front(Entry(key, value));
			by_key_[key] = entries_.begin();
		} 
		else 
		{
			cout << key << " found, update its value and move to MRU\n";
			entries_.erase(it->second);
			entries_.push_front(Entry(key, value));
			by_key_[key] = entries_.begin();
		}
    }

    std::optional<int> get(int key)
    {
		auto it = by_key_.find(key);
		if (it == by_key_.end()) {
			return std::nullopt;
		}
		else 
		{
			int value = it->second->value;
			entries_.erase(it->second);
			entries_.push_front(Entry(key, value));
			by_key_[key] = entries_.begin();			
			return value;
		}
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

    // Front of RecencyList entries_ is most recently used.
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
