// P3074R7 - trivial unions (was std::uninitialized<T>)
// { dg-do compile { target c++11 } }

#include <type_traits>

// These two were incorrectly deleted.
union A { int a; const int b; };
static_assert (std::is_default_constructible <A>::value, "");
static_assert (std::is_trivially_default_constructible <A>::value, "");
static_assert (std::is_destructible <A>::value, "");
static_assert (std::is_trivially_destructible <A>::value, "");
struct B { int a; union { int b; const int c; }; };
static_assert (std::is_default_constructible <B>::value, "");
static_assert (std::is_trivially_default_constructible <B>::value, "");
static_assert (std::is_destructible <B>::value, "");
static_assert (std::is_trivially_destructible <B>::value, "");
// C::C() is incorrectly not deleted in C++11 to 23, but in C++26 it should
// not be deleted.
union C { const int a = 42; const long b; ~C (); };
#if __cpp_trivial_union >= 202502L
static_assert (std::is_default_constructible <C>::value, "");
static_assert (!std::is_trivially_default_constructible <C>::value, "");
static_assert (std::is_destructible <C>::value, "");
static_assert (!std::is_trivially_destructible <C>::value, "");
#endif
struct D { D () = delete; D (int); ~D () = default; };
union E { D a = 42; D b; ~E (); };
static_assert (std::is_default_constructible <E>::value, "");
static_assert (std::is_destructible <E>::value, "");
struct F { int a; union { D b = 42; D c; }; };
static_assert (std::is_default_constructible <F>::value, "");
static_assert (std::is_destructible <F>::value, "");
struct G { G (); ~G (); };
union I { int a; const int b; ~I (); };
static_assert (std::is_default_constructible <I>::value, "");
static_assert (!std::is_trivially_default_constructible <I>::value, "");
static_assert (std::is_destructible <I>::value, "");
static_assert (!std::is_trivially_destructible <I>::value, "");
union J { D a; int b; };
#if __cpp_trivial_union >= 202502L
static_assert (std::is_default_constructible <J>::value, "");
static_assert (std::is_trivially_default_constructible <J>::value, "");
#else
static_assert (!std::is_default_constructible <J>::value, "");
static_assert (!std::is_trivially_default_constructible <J>::value, "");
#endif
static_assert (std::is_destructible <J>::value, "");
static_assert (std::is_trivially_destructible <J>::value, "");
union K { G a; int b; };
#if __cpp_trivial_union >= 202502L
static_assert (std::is_default_constructible <K>::value, "");
static_assert (std::is_trivially_default_constructible <K>::value, "");
static_assert (std::is_destructible <K>::value, "");
static_assert (std::is_trivially_destructible <K>::value, "");
#else
static_assert (!std::is_default_constructible <K>::value, "");
static_assert (!std::is_trivially_default_constructible <K>::value, "");
static_assert (!std::is_destructible <K>::value, "");
static_assert (!std::is_trivially_destructible <K>::value, "");
#endif
struct L { int a; union { G b; int c; }; };
#if __cpp_trivial_union >= 202502L
static_assert (std::is_default_constructible <L>::value, "");
static_assert (std::is_trivially_default_constructible <L>::value, "");
static_assert (std::is_destructible <L>::value, "");
static_assert (std::is_trivially_destructible <L>::value, "");
#else
static_assert (!std::is_default_constructible <L>::value, "");
static_assert (!std::is_trivially_default_constructible <L>::value, "");
static_assert (!std::is_destructible <L>::value, "");
static_assert (!std::is_trivially_destructible <L>::value, "");
#endif
union M { M (); int a; int b; };
static_assert (std::is_default_constructible <M>::value, "");
static_assert (!std::is_trivially_default_constructible <M>::value, "");
static_assert (std::is_destructible <M>::value, "");
static_assert (std::is_trivially_destructible <M>::value, "");
union N { N (); int a; G b; };
static_assert (!std::is_default_constructible <N>::value, "");
static_assert (!std::is_trivially_default_constructible <N>::value, "");
static_assert (!std::is_destructible <N>::value, "");
static_assert (!std::is_trivially_destructible <N>::value, "");
struct O { O (); union { int a; int b; }; };
static_assert (std::is_default_constructible <O>::value, "");
static_assert (!std::is_trivially_default_constructible <O>::value, "");
static_assert (std::is_destructible <O>::value, "");
static_assert (std::is_trivially_destructible <O>::value, "");
struct P { P (); union { int a; G b; }; };
static_assert (!std::is_default_constructible <P>::value, "");
static_assert (!std::is_trivially_default_constructible <P>::value, "");
static_assert (!std::is_destructible <P>::value, "");
static_assert (!std::is_trivially_destructible <P>::value, "");
struct Q { Q (int); union { int a; G b; }; };
static_assert (!std::is_default_constructible <Q>::value, "");
static_assert (!std::is_trivially_default_constructible <Q>::value, "");
static_assert (!std::is_destructible <Q>::value, "");
static_assert (!std::is_trivially_destructible <Q>::value, "");
struct R { R () = default; R (int); union { int a; G b; }; };
static_assert (!std::is_default_constructible <R>::value, "");
static_assert (!std::is_trivially_default_constructible <R>::value, "");
static_assert (!std::is_destructible <R>::value, "");
static_assert (!std::is_trivially_destructible <R>::value, "");
struct S { S (int); ~S (); };
union T { T (); int a; int b = 42; };
static_assert (std::is_default_constructible <T>::value, "");
static_assert (!std::is_trivially_default_constructible <T>::value, "");
static_assert (std::is_destructible <T>::value, "");
static_assert (std::is_trivially_destructible <T>::value, "");
union U { U (); int a; S b = 42; };
static_assert (!std::is_default_constructible <U>::value, "");
static_assert (!std::is_trivially_default_constructible <U>::value, "");
static_assert (!std::is_destructible <U>::value, "");
static_assert (!std::is_trivially_destructible <U>::value, "");
struct V { V (); union { int a; int b = 42; }; };
static_assert (std::is_default_constructible <V>::value, "");
static_assert (!std::is_trivially_default_constructible <V>::value, "");
static_assert (std::is_destructible <V>::value, "");
static_assert (std::is_trivially_destructible <V>::value, "");
struct W { W (); union { int a; S b = 42; }; };
static_assert (!std::is_default_constructible <W>::value, "");
static_assert (!std::is_trivially_default_constructible <W>::value, "");
static_assert (!std::is_destructible <W>::value, "");
static_assert (!std::is_trivially_destructible <W>::value, "");
struct X { X (int); union { int a; S b = 42; }; };
static_assert (!std::is_default_constructible <X>::value, "");
static_assert (!std::is_trivially_default_constructible <X>::value, "");
static_assert (!std::is_destructible <X>::value, "");
static_assert (!std::is_trivially_destructible <X>::value, "");
struct Y { Y () = default; Y (int); union { int a; S b = 42; }; };
static_assert (!std::is_default_constructible <Y>::value, "");
static_assert (!std::is_trivially_default_constructible <Y>::value, "");
static_assert (!std::is_destructible <Y>::value, "");
static_assert (!std::is_trivially_destructible <Y>::value, "");
