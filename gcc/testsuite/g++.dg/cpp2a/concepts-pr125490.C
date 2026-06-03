// PR c++/125490
// { dg-do compile { target c++20 } }

void f();

template<class T>
concept C = f() == 42; // { dg-error "invalid operands" }

template<class T>
concept D = requires { requires f() == 42; }; // { dg-error "invalid operands" }
