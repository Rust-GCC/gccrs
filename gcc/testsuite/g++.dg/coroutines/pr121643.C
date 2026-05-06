// PR c++/121643

#include <coroutine>

struct Detached {
  struct promise_type {
    Detached get_return_object() const noexcept { return {}; }
    std::suspend_never initial_suspend() const noexcept { return {}; }
    void return_void() const noexcept {}
    void unhandled_exception() const noexcept { }
    std::suspend_never final_suspend() const noexcept { return {}; }
  };
};

struct Awaiter {
  bool await_ready() noexcept { return true; }
  void
  await_suspend(std::coroutine_handle<> h,
                int sloc = 1)
    noexcept
  {}
  void await_resume() noexcept {}
};

Detached
coroutine()
{
  co_await Awaiter{};
}
