#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <limits>
#include <numeric>
#include <optional>
#include <span>
#include <vector>

struct AltitudeRange {
    int minimum;
    int maximum;
};

// Return the common permitted range, if every zone can be satisfied.
std::optional<AltitudeRange> intersect_ranges(std::span<const AltitudeRange> zones)
{
    if (zones.empty()) {
        return std::nullopt;
    }

    int lower = zones.front().minimum;
    int upper = zones.front().maximum;

    for (const auto& zone : zones) {
        if (zone.minimum > zone.maximum) {
            return std::nullopt; // Malformed input.
        }

        lower = std::max(lower, zone.minimum);
        upper = std::min(upper, zone.maximum);

        if (lower > upper) {
            return std::nullopt; // No altitude satisfies every zone.
        }
    }

    return AltitudeRange{lower, upper};
}

std::optional<int> choose_altitude(std::span<const AltitudeRange> zones)
{
    const auto common = intersect_ranges(zones);
    if (!common) {
        return std::nullopt;
    }

    // C++20 midpoint avoids overflow from adding the two bounds.
    return std::midpoint(common->minimum, common->maximum);
}

int main()
{
    const std::vector<AltitudeRange> example{{50, 120}, {80, 130}, {70, 200}};
    const auto common = intersect_ranges(example);
    assert(common.has_value());
    assert(common->minimum == 80);
    assert(common->maximum == 120);
    assert(choose_altitude(example) == 100);

    const std::array<AltitudeRange, 1> single{{{45, 95}}};
    assert(choose_altitude(single) == 70);

    // Closed intervals can intersect at exactly one altitude.
    const std::array<AltitudeRange, 2> touching{{{50, 80}, {80, 150}}};
    assert(choose_altitude(touching) == 80);

    const std::array<AltitudeRange, 2> disjoint{{{50, 70}, {80, 120}}};
    assert(!intersect_ranges(disjoint));
    assert(!choose_altitude(disjoint));

    const std::array<AltitudeRange, 0> empty{};
    assert(!choose_altitude(empty));

    const std::array<AltitudeRange, 2> invalid{{{50, 120}, {130, 80}}};
    assert(!choose_altitude(invalid));

    // An average of zone midpoints would be 33.5, violating [0, 1].
    const std::array<AltitudeRange, 3> counterexample{{{0, 100}, {0, 100}, {0, 1}}};
    assert(choose_altitude(counterexample) == 0);

    // Check that extreme integer boundaries do not overflow.
    const std::array<AltitudeRange, 1> extremes{{
        {std::numeric_limits<int>::min(), std::numeric_limits<int>::max()}
    }};
    assert(choose_altitude(extremes) == -1);

    std::cout << "Task 032: all drone-altitude tests passed\n";
}
