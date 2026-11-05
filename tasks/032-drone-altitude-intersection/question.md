# Task 032 — Drone Altitude Interval Intersection

## Interview scenario

A Ground Control Station receives permitted altitude ranges for the zones along a drone's route. Each zone specifies an inclusive minimum and maximum altitude, measured in meters.

Example:

| Zone | Minimum | Maximum |
| --- | ---: | ---: |
| A | 50 | 120 |
| B | 80 | 130 |
| C | 70 | 200 |

Find **one altitude allowed in every zone**, or report that none exists. This is a constraint problem, not a request to average the zone heights.

## Interface

```cpp
struct AltitudeRange {
    int minimum;
    int maximum;
};

std::optional<AltitudeRange> intersect_ranges(
    std::span<const AltitudeRange> zones);

std::optional<int> choose_altitude(
    std::span<const AltitudeRange> zones);
```

Return the full common range from `intersect_ranges`. For `choose_altitude`, return the integer midpoint of that common range, or `std::nullopt` when no valid altitude exists.

## Requirements

- Use C++20 and the standard library only.
- Treat interval boundaries as inclusive; an intersection of one altitude is valid.
- Return `std::nullopt` for an empty list or any malformed interval (`minimum > maximum`).
- Avoid integer overflow when selecting the midpoint.
- Solve in O(n) time and O(1) extra space, without sorting.
- Test overlapping, touching, disjoint, empty, malformed, and extreme-bound cases.

## Follow-up

Could you process one new constraint at a time without storing all earlier constraints? What changes if previously supplied constraints may be removed?

## Build and run

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -g \
  tasks/032-drone-altitude-intersection/solution.cpp -o /tmp/cpp-challenge-032
/tmp/cpp-challenge-032
```
