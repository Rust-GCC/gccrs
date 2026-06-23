// PR c++/125768
// { dg-additional-options -fmodules }

export module api;
export struct S { };
export template <class T> struct A { };
export using a_s = A<S>;
