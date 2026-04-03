// { dg-do compile { target c++26 } }

#include <type_traits>
#include <testsuite_tr1.h>

void test01()
{
  using std::is_structural;
  using namespace __gnu_test;
  struct A { constexpr A () : a (42) {} mutable int a; };
  struct B { constexpr B () {} A b; };
  struct C : public A { constexpr C () {} };
  struct D {};
  struct E : protected D { constexpr E () {} };
  struct F { constexpr F () : f (42) {} int f; };
  struct G { F a; int b[2]; constexpr G () : b{0,1} {} };

  static_assert(test_category<is_structural, int>(true), "");
  static_assert(test_category<is_structural, float>(true), "");
  static_assert(test_category<is_structural, const float>(true), "");
  static_assert(test_category<is_structural, EnumType>(true), "");
  static_assert(test_category<is_structural, int*>(true), "");
  static_assert(test_category<is_structural, int(*)(int)>(true), "");
  static_assert(test_category<is_structural, int (ClassType::*)>(true), "");
  static_assert(test_category<is_structural, int (ClassType::*) (int)>(true), "");
  static_assert(test_category<is_structural, std::nullptr_t>(true), "");
#if __cpp_impl_reflection >= 202506L
  static_assert(test_category<is_structural, decltype(^^::)>(true), "");
#endif
  static_assert(test_category<is_structural, int&>(true), "");
  static_assert(test_category<is_structural, NLType&>(true), "");
  static_assert(test_category<is_structural, PODType>(true), "");
  static_assert(test_category<is_structural, const LType>(true), "");
  static_assert(test_category<is_structural, LTypeDerived>(true), "");
  static_assert(test_category<is_structural, D>(true), "");
  static_assert(test_category<is_structural, F>(true), "");
  static_assert(test_category<is_structural, G>(true), "");

  static_assert(test_category<is_structural, int&&>(false), "");
  static_assert(test_category<is_structural, int[2]>(false), "");
  static_assert(test_category<is_structural, TType>(false), "");
  static_assert(test_category<is_structural, NType>(false), "");
  static_assert(test_category<is_structural, SLType>(false), "");
  static_assert(test_category<is_structural, LType[5]>(false), "");
  static_assert(test_category<is_structural, NLType>(false), "");
  static_assert(test_category<is_structural, NLType[5]>(false), "");
  static_assert(test_category<is_structural, A>(false), "");
  static_assert(test_category<is_structural, const B>(false), "");
  static_assert(test_category<is_structural, C>(false), "");
  static_assert(test_category<is_structural, E>(false), "");
}
