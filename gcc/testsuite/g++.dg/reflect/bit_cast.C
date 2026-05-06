// PR c++/124096
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <bit>

consteval void
f ()
{
  auto a = ^^int;
  auto *b = &a;
  void *c = nullptr;
  (void) std::bit_cast<void *>(b);  // { dg-message "from here" }
  (void) std::bit_cast<decltype(^^int) *>(c); // { dg-message "from here" }
  __builtin_bit_cast (void *, b); // { dg-error ".__builtin_bit_cast. cannot be used with consteval-only types" }
  __builtin_bit_cast (decltype(^^int) *, c);  // { dg-error ".__builtin_bit_cast. cannot be used with consteval-only types" }
}

// { dg-error ".__builtin_bit_cast. cannot be used with consteval-only types" "" { target *-*-* } 0 }
