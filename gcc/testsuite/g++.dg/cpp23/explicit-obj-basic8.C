// PR c++/125330
// { dg-do compile { target c++23 } }

struct T {
  template<typename Self>
  void foo(this Self const &x) requires requires { x.bar(); }
    { x.bar(); }
};
