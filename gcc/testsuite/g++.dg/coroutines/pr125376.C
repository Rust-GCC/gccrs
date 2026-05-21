// PR c++/125376
// { dg-do run { target c++26 } }

#include <coroutine>

struct task {
  struct promise_type {
    task get_return_object () noexcept { return {}; }
    std::suspend_never initial_suspend () noexcept { return {}; }
    std::suspend_never final_suspend ()   noexcept { return {}; }
    void return_void () noexcept {}
    void unhandled_exception () {}
  };
};

struct A {
  unsigned value = 42;
  A () {}
  ~A () { if (value != 42) __builtin_abort (); }
};

struct B {
  unsigned value = 43;
  B () {}
  ~B () { if (value != 43) __builtin_abort (); }
};

task
foo ()
{
  auto _ = A {};
  auto _ = B {};
  co_return;
}

int
main ()
{
  foo ();
}
