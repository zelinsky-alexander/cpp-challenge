#include <cassert>
#include <cerrno>
#include <fcntl.h>
#include <type_traits>
#include <unistd.h>
#include <utility>

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

static_assert(!std::is_copy_constructible_v<UniqueFd>);
static_assert(!std::is_copy_assignable_v<UniqueFd>);
static_assert(std::is_nothrow_move_constructible_v<UniqueFd>);
static_assert(std::is_nothrow_move_assignable_v<UniqueFd>);

int main()
{
    UniqueFd empty;
    assert(empty.get() == -1);
    assert(!empty);

    int fd = ::open("/dev/null", O_RDONLY);
    assert(fd >= 0);

    UniqueFd owner(fd);
    assert(owner.get() == fd);
    assert(owner);

    UniqueFd moved(std::move(owner));
    assert(!owner);
    assert(moved.get() == fd);

    int released = moved.release();
    assert(released == fd);
    assert(!moved);
    assert(::fcntl(released, F_GETFD) != -1);
    assert(::close(released) == 0);

    int fd2 = ::open("/dev/null", O_RDONLY);
    assert(fd2 >= 0);
    UniqueFd resetOwner(fd2);
    resetOwner.reset();
    assert(!resetOwner);
    errno = 0;
    assert(::fcntl(fd2, F_GETFD) == -1);
    assert(errno == EBADF);

    return 0;
}
