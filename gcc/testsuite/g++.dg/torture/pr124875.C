// { dg-do run }

#include <memory>  // unique_ptr
#include <utility> // std::move

namespace {

template <typename T, unsigned N>
struct SmallVector {
  alignas(T) char InlineElts[sizeof(T) * N];

  unsigned Size = 0;
  //[[gnu::optimize(0)]]
  T* begin() { return (T*)this->InlineElts; }
  //[[gnu::optimize(0)]]
  T* end() { return begin() + this->Size; }

  [[gnu::optimize(0)]]
  ~SmallVector() {
    this->destroy_range(this->begin(), this->end());
  }

  [[gnu::optimize(0)]]
  static void destroy_range(T *S, T *E) {
    while (S != E) {
      --E;
      E->~T();
    }
  }

  //[[gnu::optimize(0)]]
  void push_back(T &&Elt) {
    ::new ((void *)this->end()) T(std::move(Elt));
    this->Size += 1;
  }
};

////////////////////////

template <class T> struct Expected {
  using E = std::unique_ptr<int>;

  [[gnu::optimize(0)]]
  explicit Expected(T &&Val)
  {
    HasError = false;
    new (&t) T(std::forward<T>(Val));
  }

  [[gnu::optimize(0)]]
  explicit Expected(Expected &&Other) {
    HasError = Other.HasError;

    if (!HasError)
      new (&t) T(std::move(Other.t));
    else
      new (&e) E(std::move(Other.e));
  }

  Expected &operator=(Expected &&Other) = delete;

  //[[gnu::optimize(0)]]
  ~Expected() {
    if (!HasError)
      t.~T();
    else
      e.~E();
  }

  [[gnu::optimize(0)]]
  T& get() {
    return t;
  }

  union {
    T t;
    E e;
  };
  bool HasError;
};

struct EA {
  bool destroyed;

  [[gnu::optimize(0)]]
  EA() : destroyed(false) {}
  [[gnu::optimize(0)]]
  // catch double desruction
  ~EA() { if (destroyed) __builtin_abort();  destroyed = true; }
};

[[gnu::optimize(0)]]
static std::unique_ptr<EA> mkEA() {
  return std::make_unique<EA>();
}

//__attribute__((optimize(0)))
[[gnu::noinline]]
static void parseCallExpr_() {
  SmallVector<std::unique_ptr<EA>, 4> Args;
  Expected<std::unique_ptr<EA>> Arg =
      Expected<std::unique_ptr<EA>>(mkEA());

  Args.push_back(std::move(Arg.get()));
}

}

[[gnu::optimize(0)]]
int main() {
  parseCallExpr_();
}
