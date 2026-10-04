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
        const std::uint64_t generation = next_generation_++;

        entries_[key] = Entry{
            std::move(value),
            expires_at,
            generation
        };

        expirations_.push(Expiration{
            expires_at,
            generation,
            std::move(key)
        });
    }

    [[nodiscard]] std::optional<std::string> get(
        std::string_view key,
        std::uint64_t now)
    {
        purgeExpired(now);

        const auto it = entries_.find(std::string{key});
        if (it == entries_.end()) {
            return std::nullopt;
        }

        return it->second.value;
    }

    [[nodiscard]] std::size_t size(std::uint64_t now)
    {
        purgeExpired(now);
        return entries_.size();
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
            if (lhs.expires_at != rhs.expires_at) {
                return lhs.expires_at > rhs.expires_at;
            }
            return lhs.generation > rhs.generation;
        }
    };

    void purgeExpired(std::uint64_t now)
    {
        while (!expirations_.empty() &&
               expirations_.top().expires_at <= now) {
            const Expiration expiration = expirations_.top();
            expirations_.pop();

            const auto it = entries_.find(expiration.key);

            if (it != entries_.end() &&
                it->second.generation == expiration.generation &&
                it->second.expires_at == expiration.expires_at) {
                entries_.erase(it);
            }
        }
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
