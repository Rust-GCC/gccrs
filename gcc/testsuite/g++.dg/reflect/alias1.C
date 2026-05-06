// PR c++/124544
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

using alias = int;
template<class> using alias_t = int;

template<auto> struct A { };
static_assert( ! __is_same(A<^^int>, A<^^alias>) );
static_assert( ! __is_same(A<^^int>, A<^^alias_t<int>>) );
static_assert( ! __is_same(A<^^alias>, A<^^alias_t<int>>) );
static_assert( ! __is_same(A<^^alias_t<char>>, A<^^alias_t<int>>) );
static_assert(   __is_same(A<^^alias_t<alias>>, A<^^alias_t<int>>) );

template<class> struct B { };
static_assert(   __is_same(A<^^B<alias>>, A<^^B<int>>) );

template<auto> int V;
static_assert( &V<^^int> != &V<^^alias_t<int>> );
static_assert( &V<^^int> != &V<^^alias> );
static_assert( &V<^^alias_t<int>> != &V<^^alias_t<char>> );

template<auto> void f();
static_assert( &f<^^int> != &f<^^alias_t<int>> );
static_assert( &f<^^int> != &f<^^alias> );
static_assert( &f<^^alias_t<int>> != &f<^^alias_t<char>> );
