// PR c++/123975
// { dg-do run { target c++20 } }
#include <coroutine>

#if DEBUG
#define PRINTF(...) __builtin_printf (__VA_ARGS__)
#else
#define PRINTF(...)
#endif

struct task {
  bool await_ready() const { return false; }
  void await_suspend(auto &&h);
  void await_resume() {}

  struct promise_type {
    task get_return_object() { return task{}; }
    void return_void() {}
    void unhandled_exception() {}
    auto initial_suspend() noexcept { return std::suspend_never{}; }
    auto final_suspend() noexcept { return std::suspend_never{}; }

    promise_type();
    promise_type(const promise_type& p)
    { PRINTF ("%p: promise copied from %p\n", this, &p); }
    ~promise_type() { PRINTF ("%p: promise destroyed\n", this); }
  };
};

const task::promise_type *first;
task::promise_type::promise_type()
{
  if (!first) first = this;
  PRINTF ("%p: promise constructed\n", this);
}
void task::await_suspend (auto &&h)
{
  auto &p = h.promise();
  PRINTF ("suspend %p\n", &h.promise());
  if (&p != first) __builtin_abort ();
}

task f() { co_return; }

task g() {
  co_await f();
}

int main()
{
  g();
}
