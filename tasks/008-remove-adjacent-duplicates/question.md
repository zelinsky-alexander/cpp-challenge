# Task 008 — Remove Adjacent Duplicates In Place

## Goal

Implement:

```cpp
void removeAdjacentDuplicates(std::string& text);
```

Remove consecutive duplicate characters from `text` in place, preserving the first character from each consecutive group.

## Examples

```text
"aaabbcdddde" -> "abcde"
"a"           -> "a"
""            -> ""
"abcdef"      -> "abcdef"
"aaaa"        -> "a"
"aabbaa"      -> "aba"
```

## Requirements

- Use C++20.
- Modify the original string in place.
- Do not create another string or character container.
- Preserve non-adjacent duplicate characters.
- Target time complexity: `O(n)`.
- Target extra-space complexity: `O(1)`.

## Build and run

From the repository root:

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -g \
  tasks/008-remove-adjacent-duplicates/solution.cpp \
  -o /tmp/cpp-challenge-008

/tmp/cpp-challenge-008
```
