// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <type_traits>
#include <testsuite_tr1.h>

void test01()
{
  using std::is_scalar;
  using namespace __gnu_test;

  static_assert(test_category<is_scalar, decltype (^^::)>(true), "");
}
