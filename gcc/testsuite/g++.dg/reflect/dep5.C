// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

struct S {
  static constexpr int x = 42;
  using type = int;
  static void fn (int) { }
  template<typename T>
  static void tfn () { }
  template<typename T>
  static T var;
};

template<typename T>
void
f ()
{
  static_assert ([: ^^T :]::x == 42);
  typename [: ^^T :]::type a = 42;
  [: ^^T :]::fn (42);
  [: ^^T :]::template tfn<([: ^^T :])>();  // { dg-error "expected a reflection of an expression" }
  // { dg-message "but .S. is a type" "" { target *-*-* } 4 }
  auto x = [: ^^T :]::template var<([: ^^T :])>;  // { dg-error "expected a reflection of an expression" }
}

void
g ()
{
  f<S>();
}
