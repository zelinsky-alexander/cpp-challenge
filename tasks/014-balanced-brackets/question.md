# Task 014 — Balanced Brackets

## Goal

Implement:

```cpp
bool hasBalancedBrackets(std::string_view text);
```

The input contains only the bracket characters:

```text
( ) [ ] { }
```

Return `true` when every opening bracket is closed by the correct matching bracket in the correct order. Otherwise return `false`.

## Examples

```text
""         -> true
"()"       -> true
"([]){}"   -> true
"([{}])"   -> true
"("        -> false
")"        -> false
"([)]"     -> false
"((()))"   -> true
"{[}]"     -> false
```

## Requirements

- Use C++20.
- Do not modify the input.
- Handle all three bracket types: `()`, `[]`, `{}`.
- Return `false` for a closing bracket that has no matching opening bracket.
- Return `false` when opening brackets remain unmatched at the end.
- Target time complexity: `O(n)`.
- Target extra-space complexity: `O(n)` worst case.
- Do not use recursion.

## Build and run

From the repository root:

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -g \
  tasks/014-balanced-brackets/solution.cpp \
  -o /tmp/cpp-challenge-014

/tmp/cpp-challenge-014
```
