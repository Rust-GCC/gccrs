// PR c++/124735
// { dg-additional-options "-fmodules" }

struct S {
  auto foo();
};

auto S::foo() { return 123; }
auto pfn = &S::foo;

import "auto-8_a.H";

int main() {
  return S{}.foo() != 123;
}
