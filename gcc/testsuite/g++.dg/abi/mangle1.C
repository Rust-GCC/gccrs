// Test for mangling of simple testcase involving construction vtables.

// For C++26, the ctor is constant evaluated and so construction vtables
// aren't needed.
// { dg-do compile { target c++23_down } }
// { dg-options "-fno-inline -fabi-compat-version=0" }

struct A {
  virtual void f () { }
};

struct B: public virtual A { };
struct C: public B { };

C c;

// { dg-final { scan-assembler "\n_?_ZN1A1fEv\[: \t\n\]" } }
// { dg-final { scan-assembler "\n_?_ZN1AC2Ev\[: \t\n\]" } }
// { dg-final { scan-assembler "\n_?_ZN1BC2Ev\[: \t\n\]" } }
// { dg-final { scan-assembler "\n_?_ZN1CC1Ev\[: \t\n\]" } }
// { dg-final { scan-assembler "\n_?_ZTC1C0_1B\[: \t\n\]" } }
// { dg-final { scan-assembler "\n_?_ZTI1A\[: \t\n\]" } }
// { dg-final { scan-assembler "\n_?_ZTI1B\[: \t\n\]" } }
// { dg-final { scan-assembler "\n_?_ZTI1C\[: \t\n\]" } }
// { dg-final { scan-assembler "\n_?_ZTS1A\[: \t\n\]" } }
// { dg-final { scan-assembler "\n_?_ZTS1B\[: \t\n\]" } }
// { dg-final { scan-assembler "\n_?_ZTS1C\[: \t\n\]" } }
// { dg-final { scan-assembler "\n_?_ZTT1C\[: \t\n\]" } }
// { dg-final { scan-assembler "\n_?_ZTV1A\[: \t\n\]" } }
// { dg-final { scan-assembler "\n_?_ZTV1C\[: \t\n\]" } }
