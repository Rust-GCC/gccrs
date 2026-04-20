// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

using info = decltype(^^void);

template<typename>
void foo () { }
template<typename>
void foo2 () { }
void bar () { }
namespace N { }
namespace M = N;
template<typename T>
T vt{};
template<typename T>
T vt2{};
template<typename T>
concept C = true;
static int i;
enum E { X };
struct S { static int si; };
template<typename T>
struct D { static T di; };
template<typename T>
using Z = D<T>;

template<info R> void fn1 () { typename [:R:]::X x; }	// { dg-error "expected a reflection of a class, namespace, or enumeration" }
// { dg-message "but .foo<int>. is a function" "" { target *-*-* } 7 }
template<info R> void fn2 () { typename [:R:]::X x; }   // { dg-error "expected a reflection of a class, namespace, or enumeration" }
// { dg-message "but .foo2. is a function template" "" { target *-*-* } 9 }
template<info R> void fn3 () { typename [:R:]::X x; }   // { dg-error "expected a reflection of a class, namespace, or enumeration" }
// { dg-message "but .bar. is a function" "" { target *-*-* } 10 }
template<info R> void fn4 () { typename [:R:]::X x; }   // { dg-error "expected a reflection of a class, namespace, or enumeration" }
// { dg-message "but .vt<int>. is a variable" "" { target *-*-* } 14 }
template<info R> void fn5 () { typename [:R:]::X x; }   // { dg-error "expected a reflection of a class, namespace, or enumeration" }
// { dg-message "but .vt2<T>. is a variable template" "" { target *-*-* } 16 }
template<info R> void fn6 () { typename [:R:]::X x; }   // { dg-error "expected a reflection of a class, namespace, or enumeration" }
// { dg-message "but .C. is a concept" "" { target *-*-* } 18 }
template<info R> void fn7 () { typename [:R:]::X x; }   // { dg-error "expected a reflection of a class, namespace, or enumeration" }
// { dg-message "but .i. is a variable" "" { target *-*-* } 19 }
template<info R> void fn8 () { typename [:R:]::X x; }   // { dg-error "expected a reflection of a class, namespace, or enumeration" }
// { dg-message "but .X. is an enumerator" "" { target *-*-* } 20 }
template<info R> void fn9 () { typename [:R:]::X x; }   // { dg-error "expected a reflection of a class, namespace, or enumeration" }
// { dg-message "but .S::si. is a variable" "" { target *-*-* } 21 }
template<info R> void fn10 () { typename [:R:]::X x; }  // { dg-error "expected a reflection of a class, namespace, or enumeration" }
// { dg-message "but .D<int>::di. is a variable" "" { target *-*-* } 23 }
template<info R> void fn11 () { typename [:R:]::X x; }  // { dg-error "expected a reflection of a class, namespace, or enumeration" }
// { dg-message "but .Z. is an alias template" "" { target *-*-* } 25 }

template void fn1<^^foo<int>>(); // { dg-message "required from here" }
template void fn2<^^foo2>();	 // { dg-message "required from here" }
template void fn3<^^bar>();	 // { dg-message "required from here" }
template void fn4<^^vt<int>>();  // { dg-message "required from here" }
template void fn5<^^vt2>();	 // { dg-message "required from here" }
template void fn6<^^C>();	 // { dg-message "required from here" }
template void fn7<^^i>();	 // { dg-message "required from here" }
template void fn8<^^X>();	 // { dg-message "required from here" }
template void fn9<^^S::si>();	 // { dg-message "required from here" }
template void fn10<^^D<int>::di>(); // { dg-message "required from here" }
template void fn11<^^Z>();	 // { dg-message "required from here" }
