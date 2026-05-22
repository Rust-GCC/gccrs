// P3074R7 - trivial unions (was std::uninitialized<T>)
// { dg-do compile { target c++20 } }

union U { int a, b; };
template<U u> class X {};
constexpr U make() { U u; return u; }
void f(X<make()>) {}
