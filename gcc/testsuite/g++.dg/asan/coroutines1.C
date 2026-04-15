// PR c++/121961
// { dg-additional-options "-fsanitize=address" }
// { dg-do run { target c++20 } }

#include <coroutine>

struct Result
{
  int Value;
};

template<typename... Args>
struct std::coroutine_traits<Result, Args...>
{
  struct promise_type;

  struct Wrapper
  {
    std::coroutine_handle<promise_type> Handle_;
    bool Own_ = false;

    ~Wrapper ()
    {
      if (Own_)
	Handle_.destroy ();
    }

    operator Result ()
    {
      Own_ = true;
      return { Handle_.promise ().Value_ };
    }
  };

  struct promise_type
  {
    int Value_ = 0;

    std::suspend_never initial_suspend () noexcept { return {}; }
    std::suspend_always final_suspend () noexcept { return {}; }
    void unhandled_exception () {}

    void return_value (int v) { Value_ = v; }

    Wrapper get_return_object ()
    {
      return { std::coroutine_handle<promise_type>::from_promise (*this) };
    }
  };
};

struct Awaitable
{
  int V_;
  bool await_ready () const noexcept { return true; }
  void await_suspend (std::coroutine_handle<>) const noexcept {}
  int await_resume () const noexcept { return V_; }
};

Result simple ()
{
  auto a = co_await Awaitable { 3 };
  auto b = co_await Awaitable { 5 };
  co_return a * b;
}

int main ()
{
  auto r = simple ();
  return r.Value == 15 ? 0 : 1;
}
