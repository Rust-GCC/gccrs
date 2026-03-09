// PR c++/121500
// { dg-do run { target c++23 } }

struct A {
  A() = default;
  A(const A& other) : n(other.n) { }
  int n = 42;
};

int main() {
  A a;
  auto l = [a](this auto self) {
    return a.n == 42;
  };
  if (!l())
    __builtin_abort();
}
