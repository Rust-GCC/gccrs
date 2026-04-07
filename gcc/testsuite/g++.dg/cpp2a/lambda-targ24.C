// PR c++/123700
// { dg-do compile { target c++20 } }

template<class> constexpr int zero = 0;
template<auto> struct Type;

int outer(auto) {
  using inner = Type<[](auto) {}>;
  return [](auto) { return zero<inner>; }(0);
}

int main() { outer(0.0); }
