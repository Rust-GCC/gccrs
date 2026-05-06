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

template<info R> void fn1 () { int n = typename [:R:](42); } // { dg-error "expected a reflection of a type" }
// { dg-message "but .foo<int>. is a function" "" { target *-*-* } 7 }
template<info R> void fn2 () { int n = typename [:R:](42); } // { dg-error "expected a reflection of a type" }
// { dg-message "but .foo2. is a function template" "" { target *-*-* } 9 }
template<info R> void fn3 () { int n = typename [:R:](42); } // { dg-error "expected a reflection of a type" }
// { dg-message "but .N. is a namespace" "" { target *-*-* } 11 }
template<info R> void fn4 () { int n = typename [:R:](42); } // { dg-error "expected a reflection of a type" }
// { dg-message "but .M. is a namespace" "" { target *-*-* } 12 }
template<info R> void fn5 () { int n = typename [:R:](42); } // { dg-error "expected a reflection of a type" }
// { dg-message "but .bar. is a function" "" { target *-*-* } 10 }
template<info R> void fn6 () { int n = typename [:R:](42); } // { dg-error "expected a reflection of a type" }
// { dg-message "but .vt<int>. is a variable" "" { target *-*-* } 14 }
template<info R> void fn7 () { int n = typename [:R:](42); } // { dg-error "expected a reflection of a type" }
// { dg-message "but .vt2<T>. is a variable template" "" { target *-*-* } 16 }
template<info R> void fn8 () { int n = typename [:R:](42); } // { dg-error "expected a reflection of a type" }
// { dg-message "but .C. is a concept" "" { target *-*-* } 18 }
template<info R> void fn9 () { int n = typename [:R:](42); } // { dg-error "expected a reflection of a type" }
// { dg-message "but .i. is a variable" "" { target *-*-* } 19 }
template<info R> void fn10 () { int n = typename [:R:](42); } // { dg-error "expected a reflection of a type" }
// { dg-message "but .X. is an enumerator" "" { target *-*-* } 20 }
template<info R> void fn11 () { int n = typename [:R:](42); } // { dg-error "expected a reflection of a type" }
// { dg-message "but .S::si. is a variable" "" { target *-*-* } 21 }
template<info R> void fn12 () { int n = typename [:R:](42); } // { dg-error "expected a reflection of a type" }
// { dg-message "but .D<int>::di. is a variable" "" { target *-*-* } 23 }
template<info R> void fn13 () { int n = typename [:R:](42); } // { dg-error "class template argument deduction failed|no matching" }

template void fn1<^^foo<int>>();  // { dg-message "required from here" }
template void fn2<^^foo2>();	  // { dg-message "required from here" }
template void fn3<^^N>();	  // { dg-message "required from here" }
template void fn4<^^M>();	  // { dg-message "required from here" }
template void fn5<^^bar>();	  // { dg-message "required from here" }
template void fn6<^^vt<int>>();	  // { dg-message "required from here" }
template void fn7<^^vt2>();	  // { dg-message "required from here" }
template void fn8<^^C>();	  // { dg-message "required from here" }
template void fn9<^^i>();	  // { dg-message "required from here" }
template void fn10<^^X>();	  // { dg-message "required from here" }
template void fn11<^^S::si>();	  // { dg-message "required from here" }
template void fn12<^^D<int>::di>(); // { dg-message "required from here" }
template void fn13<^^Z>();	  // { dg-message "required from here" }
