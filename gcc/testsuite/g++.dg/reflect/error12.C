// PR c++/123823
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

constexpr int i = 42;
constexpr auto r = ^^i;
constexpr bool b0 = template [:r:] < 0 > 0;  // { dg-error "not a template" }
constexpr bool b1 = template [:r:] < 0 < 0;  // { dg-error "not a template|expected" }
constexpr bool b2 = template [:r:] < 43;     // { dg-error "not a template|expected" }
constexpr bool b3 = template [:r:] <= 43;    // { dg-error "expected a reflection of a function template" }
// { dg-message "but .i. is a variable" "" { target *-*-* } 5 }
constexpr bool b4 = template [:r:] > 41;     // { dg-error "expected a reflection of a function template" }
constexpr bool b5 = template [:r:] >= 41;    // { dg-error "expected a reflection of a function template" }
constexpr bool b6 = template [:r:] == 42;    // { dg-error "expected a reflection of a function template" }
constexpr bool b7 = template [:r:] != 41;    // { dg-error "expected a reflection of a function template" }

template<bool> struct S { };
S<template [:r:] < 43> s1;    // { dg-error "not a template|invalid" }
S<template [:r:] <= 43> s2;   // { dg-error "expected a reflection|invalid" }
S<(template [:r:] > 41)> s3;  // { dg-error "expected a reflection|invalid" }
S<template [:r:] >= 41> s4;   // { dg-error "expected a reflection|invalid" }
S<template [:r:] == 42> s5;   // { dg-error "expected a reflection|invalid" }
S<template[:r:] != 41> s6;    // { dg-error "expected a reflection|invalid" }
