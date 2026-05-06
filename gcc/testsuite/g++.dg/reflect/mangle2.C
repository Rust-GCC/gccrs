// PR c++/123237
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>

template<typename T>
using A0 = [:^^T:];

template<typename T>
using A1 = [:parent_of (^^T):]::B;

namespace N {
  using B = int;
  struct S {};
}
namespace D {
  using B = long;
  struct S {};
}

template <typename T>
using A2 = decltype (T ().[:std::meta::nonstatic_data_members_of (^^T, std::meta::access_context::unchecked ())[0]:]);

struct B
{
  long a;
  int b;
};

struct C
{
  int c;
  long d;
};


void
g ()
{
  []<typename T = int> requires(requires { A0<T> {}; }) {}();
  []<typename T = N::S> requires (requires { A1<T> {}; }) {} ();
  []<typename T = C> requires (requires { A2<T> {}; }) {} ();
}

// { dg-final { scan-assembler "_ZZ1gvENKUlTyQrqXtlDSLDmtyT_EEEEvE_clIiEEDav" } }
// { dg-final { scan-assembler "_ZZ1gvENKUlTyQrqXtlNDSclL_ZNSt4meta9parent_ofEDmELDmtyT_EEE1BEEEvE0_clIN1N1SEEEDav" } }
// { dg-final { scan-assembler "_ZZ1gvENKUlTyQrqXtlDtdtcvT__EDScldtclL_ZNSt4meta25nonstatic_data_members_ofEDmNS0_14access_contextEELDmtyS_EclL_ZNS1_9uncheckedEvEEEL_ZNSt6vectorIDmSaIDmEEixE\[jmy]ELi0EEEEEEvE1_clI1CEEDav" } }
