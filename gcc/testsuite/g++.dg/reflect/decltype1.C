// PR c++/124494
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

typedef signed char int8_t;
namespace std {
  using ::int8_t;
}
auto rr = ^^decltype(std::int8_t); // { dg-error "argument to .decltype. must be an expression" }
