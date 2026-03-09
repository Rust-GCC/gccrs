// PR c++/123641
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }
// Test that we skip STAT_TYPE_HIDDEN_P.

#include <meta>
using namespace std::meta;

namespace N {
  void B ();
  struct A {
    int m;
    friend class B;
  };
}
constexpr access_context uctx = access_context::unchecked ();
static_assert (members_of (^^N, uctx).size () == 2);
