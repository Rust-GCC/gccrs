// PR c++/124735
// { dg-additional-options "-fmodule-header" }
// { dg-module-cmi {} }

struct S {
  auto foo();
};
