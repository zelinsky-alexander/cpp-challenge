# Task 032 — Explanation: Intersection of Altitude Constraints

## Why the average is not the answer

Averages describe a typical value; they do not guarantee compliance with every constraint. For instance, the intervals `[0, 100]`, `[0, 100]`, and `[0, 1]` have midpoints `50`, `50`, and `0.5`. Their average is `33.5`, but that violates the last interval.

We need the **intersection**: every acceptable altitude must be at least *each* minimum and at most *each* maximum.

## Algorithm

Given ranges `[min_i, max_i]`:

1. Set `lower` to the maximum of all `min_i` values.
2. Set `upper` to the minimum of all `max_i` values.
3. If `lower > upper`, there is no shared altitude.
4. Otherwise every altitude in `[lower, upper]` is valid. Return the midpoint when asked for one representative altitude.

For the interview example:

```text
A  [50, 120]
B  [80, 130]
C  [70, 200]

lower = max(50, 80, 70)    = 80
upper = min(120, 130, 200) = 120
common interval            = [80, 120]
selected altitude          = 100 m
```

The selected midpoint is maximally distant from the nearer interval boundary in this simplified model. For integers, `std::midpoint` (C++20, `<numeric>`) avoids overflow and rounds toward the first argument when the exact midpoint lies between two integers.

## Invariant and correctness

After processing the first `k` zones, `[lower, upper]` equals the intersection of those `k` intervals, as long as the intersection is nonempty.

Adding another zone can only increase `lower` or decrease `upper`. Therefore an empty intersection cannot become nonempty later, and early termination is correct.

For inclusive bounds, `lower == upper` is a valid one-altitude intersection.

## Complexity

- Time: **O(n)**, one pass over `n` zones.
- Extra space: **O(1)**, independent of the number of zones.
- No sorting is needed.

## Streaming extension

When constraints only arrive and are never removed, keep just the two current boundaries:

```cpp
lower = std::max(lower, new_range.minimum);
upper = std::min(upper, new_range.maximum);
```

That is O(1) work per arrival. Handle the first interval as initialization, and reject malformed ranges. If the intersection becomes empty, later additions cannot repair it.

If zones can be removed or updated, those two values alone are insufficient to reconstruct a previous intersection; retain the ranges and recompute, or maintain a suitable structure that tracks boundary extrema.

## Real-world caveat

This exercise assumes one constant altitude is required for all zones. Actual drone flight planning also depends on terrain, vertical safety margins, obstacles, dynamic restrictions, and whether changing altitude between zones is permitted.

This code is an interview exercise, not flight-control software.
