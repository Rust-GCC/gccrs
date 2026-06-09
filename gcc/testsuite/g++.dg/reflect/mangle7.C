// PR c++/125498
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>

using namespace std::meta;

template<info>
void
xyzzy ()
{
}

struct S {
  void fn1 (const S &)
  {
    xyzzy<parameters_of (^^S::fn1)[0]> ();
  }
  void fn2 (this S)
  {
    xyzzy<parameters_of (^^S::fn2)[0]> ();
  }
  static void fn3 (int)
  {
    xyzzy<parameters_of (^^S::fn3)[0]> ();
  }
};

struct V : virtual S {
  void fn1 (const V &)
  {
    xyzzy<parameters_of (^^V::fn1)[0]> ();
  }
  void fn2 (this V)
  {
    xyzzy<parameters_of (^^V::fn2)[0]> ();
  }
  static void fn3 (int)
  {
    xyzzy<parameters_of (^^V::fn3)[0]> ();
  }
};

void
g (int)
{
  xyzzy<parameters_of (^^g)[0]> ();
  S s;
  s.fn1 (s);
  s.fn2 ();
  S::fn3 (42);
  V v;
  v.fn1 (v);
  v.fn2 ();
  V::fn3 (42);
}

// { dg-final { scan-assembler "_Z5xyzzyILDmpa_N1S3fn1ERKS0_EEvv" } }
// { dg-final { scan-assembler "_Z5xyzzyILDmpa_NH1S3fn2ES0_EEvv" } }
// { dg-final { scan-assembler "_Z5xyzzyILDmpa_N1S3fn3EiEEvv" } }
// { dg-final { scan-assembler "_Z5xyzzyILDmpa_N1V3fn1ERKS0_EEvv" } }
// { dg-final { scan-assembler "_Z5xyzzyILDmpa_NH1V3fn2ES0_EEvv" } }
// { dg-final { scan-assembler "_Z5xyzzyILDmpa_N1V3fn3EiEEvv" } }
// { dg-final { scan-assembler "_Z5xyzzyILDmpa_1giEEvv" } }
