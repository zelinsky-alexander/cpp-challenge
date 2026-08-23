# C++ Challenge

A daily C++20 interview-practice repository focused on reaching consistent senior-level performance in algorithms, data structures, modern C++, concurrency, Linux, networking, performance, and systems design.

## Daily workflow

Each task lives in its own numbered folder:

```text
tasks/
  001-task-name/
    question.md
    solution.cpp
```

For every task:

1. Read `question.md` without looking up a complete solution.
2. State the intended algorithm, invariant, and complexity before coding.
3. Implement the answer in `solution.cpp`.
4. Compile and run it with a C++20 Linux compiler.
5. Debug failures and record important conclusions in the task files after review.

## Build a task

From the repository root:

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -g tasks/001-minimum-subarray/solution.cpp -o /tmp/cpp-challenge-001
/tmp/cpp-challenge-001
```

Sanitizer build:

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion \
  -g -fsanitize=address,undefined -fno-omit-frame-pointer \
  tasks/001-minimum-subarray/solution.cpp \
  -o /tmp/cpp-challenge-001-san

/tmp/cpp-challenge-001-san
```

## Answer checklist

Before considering a task complete, verify:

- The algorithm matches all stated constraints.
- The invariant can be explained clearly.
- Time and extra-space complexities include preprocessing.
- Empty, minimal, duplicate, and boundary inputs are covered.
- Integer overflow, ownership, iterator invalidation, and unsigned-index risks were considered where relevant.
- The implementation compiles cleanly under C++20 with warnings enabled.

## Progress goal

The target is not memorizing isolated solutions. It is recognizing common patterns quickly and producing a correct, explainable, testable solution with consistent interview-quality reasoning.
