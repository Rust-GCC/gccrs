// { dg-do run { target c++11 } }
// { dg-require-effective-target x86  }
// { dg-require-cstdint "" }

#include <random>
#include <sstream>
#include <testsuite_hooks.h>

template<typename RealType>
void
test_default()
{
  std::stringstream str;
  std::piecewise_linear_distribution<RealType>
     u(1, RealType(0), RealType(1), [](RealType) -> RealType { return 1.0; });
  std::minstd_rand0 rng;

  str << u;
  std::string res = str.str();

  std::piecewise_linear_distribution<RealType> v;
  str >> v;
  VERIFY( u == v );

  if (!std::numeric_limits<RealType>::is_iec559)
    return;
 
  char const* expected = nullptr;
  switch (std::numeric_limits<RealType>::digits)
  {
  case 24: // ieee32
    expected =
      "1 0.000000000e+00 1.000000000e+00"
       " 1.000000000e+00 1.000000000e+00";
    VERIFY( res == expected );
    break;
  case 53: // ieee64
    expected =
      "1 0.00000000000000000e+00 1.00000000000000000e+00"
       " 1.00000000000000000e+00 1.00000000000000000e+00";
    VERIFY( res == expected );
    break;
  case 64: // ieee80 
    expected =
      "1 0.000000000000000000000e+00 1.000000000000000000000e+00"
       " 1.000000000000000000000e+00 1.000000000000000000000e+00";
    VERIFY( res == expected );
    break;
  default:
    break;
  };
}

template<typename RealType>
void
test_custom()
{
  std::stringstream str;
  std::piecewise_linear_distribution<RealType>
     u(3, RealType(0), RealType(1), [](RealType r) -> RealType { return 1.0 + r; });
  std::minstd_rand0 rng;

  str << u;
  std::string res = str.str();

  std::piecewise_constant_distribution<RealType> v;
  str >> v;
  // This does not hold currently
  // VERIFY( u == v );

  if (!std::numeric_limits<RealType>::is_iec559)
    return;

  char const* expected = nullptr;
  switch (std::numeric_limits<RealType>::digits)
  {
  case 24: // ieee32
    expected =
      "3 0.000000000e+00 3.333333433e-01 6.666666865e-01 1.000000000e+00"
	" 7.272727292e-01 9.090909278e-01 1.090909061e+00 1.272727325e+00";
    VERIFY( res == expected );
    break;
  case 53: // ieee64
    expected =
      "3 0.00000000000000000e+00 3.33333333333333315e-01 6.66666666666666630e-01 1.00000000000000000e+00" 
	" 7.27272727272727182e-01 9.09090909090908950e-01 1.09090909090909083e+00 1.27272727272727249e+00";
    VERIFY( res == expected );
    break;
  case 64: // ieee80 
    expected =
      "3 0.000000000000000000000e+00 3.333333333333333333424e-01 6.666666666666666666847e-01 1.000000000000000000000e+00"
	" 7.272727272727271818908e-01 9.090909090909090606303e-01 1.090909090909090828347e+00 1.272727272727272707087e+00";
    VERIFY( res == expected );
    break;
  default:
    break;
  };
}

int main()
{
  test_default<float>();
  test_default<double>();
  test_default<long double>();

#ifdef __x86_64__
  test_custom<float>();
  test_custom<double>();
#endif  
  test_custom<long double>();
  return 0;
}
