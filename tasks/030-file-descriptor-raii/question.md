# Task 030 — File-Descriptor RAII Wrapper

## Goal

Implement an owning RAII wrapper for a POSIX file descriptor:

```cpp
class UniqueFd {
public:
    UniqueFd() noexcept;
    explicit UniqueFd(int fd) noexcept;
    ~UniqueFd();

    UniqueFd(const UniqueFd&) = delete;
    UniqueFd& operator=(const UniqueFd&) = delete;

    UniqueFd(UniqueFd&& other) noexcept;
    UniqueFd& operator=(UniqueFd&& other) noexcept;

    int get() const noexcept;
    explicit operator bool() const noexcept;
    int release() noexcept;
    void reset(int newFd = -1) noexcept;
};
```

The wrapper owns at most one descriptor. A valid owned descriptor must be closed exactly once.

## Requirements

- Use C++20 on Linux/POSIX.
- Treat `-1` as invalid/no descriptor.
- The destructor closes an owned valid descriptor.
- Moving transfers ownership and leaves the source invalid.
- `release()` returns the descriptor without closing it and leaves the wrapper invalid.
- `reset()` closes the currently owned descriptor before taking ownership of `newFd` when appropriate.
- Copy construction and copy assignment must be disabled.
- No dynamic allocation is needed.

## Build and run

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -g \
  tasks/030-file-descriptor-raii/solution.cpp -o /tmp/cpp-challenge-030
/tmp/cpp-challenge-030
```
