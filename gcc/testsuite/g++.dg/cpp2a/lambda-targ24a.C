// PR c++/123700
// { dg-do compile { target c++20 } }
// A version of lambda-targ24.C where zero is a class instead of variable
// template.

template<class> struct zero { };
template<auto> struct Type;

auto outer(auto) {
  using inner = Type<[](auto) {}>;
  return [](auto) { return zero<inner>{}; }(0);
}

int main() { outer(0.0); }
