// PR c++/124706
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

using info = decltype(^^::);

template <class T>
struct A
{
  A(T);
};
[:^^A:] a(42);
typename [:^^A:] a2(42);

template<typename T> struct B { T x; };
[:^^B:] b{0};
typename [:^^B:] b2{0};

template<class T, class U>
struct X { X(U) {} };

template<class U>
using Y = X<void, U>;

[:^^Y:] y{1};
typename [:^^Y:] y2{1};

template<info R>
void
g ()
{
  typename [:R:] x(42);
}

void
f ()
{
  g<^^A>();
  g<^^B>();
  g<^^Y>();
}
