#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <optional>
#include <queue>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

class ExpiringStore {
public:
    void put(std::string key,
             std::string value,
             std::uint64_t expires_at)
    {
        // TODO:
        // 1. Insert or replace the current entry in entries_.
        // 2. Give each replacement a new generation value.
        // 3. Add a corresponding expiration record to expirations_.
        // 4. Preserve enough metadata to recognize stale heap records later.
        (void)key;
        (void)value;
        (void)expires_at;
    }

    [[nodiscard]] std::optional<std::string> get(
        std::string_view key,
        std::uint64_t now)
    {
        // TODO:
        // 1. Purge all expiration records due at or before now.
        // 2. Look up key without copying it unnecessarily, if your chosen
        //    hash-table configuration supports heterogeneous lookup.
        // 3. Return a value copy only when the current entry is still live.
        (void)key;
        (void)now;
        return std::nullopt;
    }

    [[nodiscard]] std::size_t size(std::uint64_t now)
    {
        // TODO: purge expired current entries and return the live count.
        (void)now;
        return 0U;
    }

private:
    struct Entry {
        std::string value;
        std::uint64_t expires_at{};
        std::uint64_t generation{};
    };

    struct Expiration {
        std::uint64_t expires_at{};
        std::uint64_t generation{};
        std::string key;
    };

    struct ExpiresLater {
        [[nodiscard]] bool operator()(const Expiration& lhs,
                                      const Expiration& rhs) const noexcept
        {
            // A priority_queue places the element considered "largest" at
            // the top. Reverse timestamp order to model a min-heap.
            if (lhs.expires_at != rhs.expires_at) {
                return lhs.expires_at > rhs.expires_at;
            }
            return lhs.generation > rhs.generation;
        }
    };

    void purgeExpired(std::uint64_t now)
    {
        // TODO:
        // Repeatedly inspect the earliest expiration record. Remove the map
        // entry only when the record still describes that entry's current
        // generation and expiration time. Discard stale records harmlessly.
        (void)now;
    }

    std::unordered_map<std::string, Entry> entries_;
    std::priority_queue<Expiration,
                        std::vector<Expiration>,
                        ExpiresLater>
        expirations_;
    std::uint64_t next_generation_{1U};
};

int main()
{
    ExpiringStore store;

    assert(store.size(0) == 0U);
    assert(!store.get("missing", 0).has_value());

    store.put("token", "alpha", 10);
    assert(store.size(0) == 1U);
    assert(store.get("token", 9) == std::optional<std::string>{"alpha"});
    assert(!store.get("token", 10).has_value());
    assert(store.size(10) == 0U);

    store.put("token", "old", 20);
    store.put("token", "new", 100);
    assert(store.size(20) == 1U);
    assert(store.get("token", 20) == std::optional<std::string>{"new"});
    assert(store.get("token", 99) == std::optional<std::string>{"new"});
    assert(!store.get("token", 100).has_value());

    store.put("early", "first", 200);
    store.put("early", "replacement", 150);
    assert(store.get("early", 149) ==
           std::optional<std::string>{"replacement"});
    assert(!store.get("early", 150).has_value());

    store.put("a", "one", 300);
    store.put("b", "two", 300);
    store.put("c", "three", 400);
    assert(store.size(300) == 1U);
    assert(store.get("c", 300) == std::optional<std::string>{"three"});

    store.put("", "", 500);
    assert(store.get("", 499) == std::optional<std::string>{""});

    store.put("reuse", "expired", 600);
    assert(!store.get("reuse", 600).has_value());
    store.put("reuse", "live-again", 700);
    assert(store.get("reuse", 650) ==
           std::optional<std::string>{"live-again"});

    assert(store.size(1'000'000) == 0U);

    std::cout << "All assertions passed\n";
    return 0;
}
