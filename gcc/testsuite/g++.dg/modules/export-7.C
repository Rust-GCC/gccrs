// PR c++/124781
// { dg-do compile { target c++20 } }
// { dg-additional-options "-fmodules" }
// { dg-module-cmi foo }

export module foo;
export using xx = decltype([] {
  using yy = int;
  int abc = 123;
}());
