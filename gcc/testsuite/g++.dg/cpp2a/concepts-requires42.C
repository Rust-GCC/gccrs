// PR c++/125317
// { dg-do compile { target c++20 } }

template<bool = requires { 0 > 0; }> int x;
