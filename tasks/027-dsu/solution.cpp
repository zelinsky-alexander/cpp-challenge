#include <cassert>
#include <cstddef>
#include <stdexcept>

class DisjointSet {
public:
    explicit DisjointSet(std::size_t count);

    std::size_t find(std::size_t value);
    bool unite(std::size_t a, std::size_t b);
    bool connected(std::size_t a, std::size_t b);
    std::size_t components() const noexcept;
};

int main()
{
    DisjointSet dsu(5);
    assert(dsu.components() == 5);
    assert(!dsu.connected(0, 1));

    assert(dsu.unite(0, 1));
    assert(dsu.connected(0, 1));
    assert(dsu.components() == 4);
    assert(!dsu.unite(0, 1));

    assert(dsu.unite(1, 2));
    assert(dsu.connected(0, 2));
    assert(dsu.components() == 3);

    assert(dsu.unite(3, 4));
    assert(dsu.components() == 2);
    assert(dsu.unite(2, 4));
    assert(dsu.components() == 1);

    bool threw = false;
    try { static_cast<void>(dsu.find(5)); } catch (const std::out_of_range&) { threw = true; }
    assert(threw);

    return 0;
}
