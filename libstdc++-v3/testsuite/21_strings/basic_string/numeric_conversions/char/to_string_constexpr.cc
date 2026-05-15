// { dg-do compile { target c++26 } }
// { dg-require-effective-target cxx11_abi }

#include <string>
#include <type_traits>
#include <testsuite_hooks.h>

template<typename T>
constexpr void
test()
{
  using namespace std;
  string res;
  T value;

  value = 0;
  res = to_string(value);
  VERIFY( res == "0" );

  value = 1;
  res = to_string(value);
  VERIFY( res == "1" );

  value = 10;
  res = to_string(value);
  VERIFY( res == "10" );
  
  value = 3000;
  res = to_string(value);
  VERIFY( res == "3000" );

  value = 32767;
  res = to_string(value);
  VERIFY( res == "32767" );

  if (is_unsigned_v<T>)
    return;

  value = -1;
  res = to_string(value);
  VERIFY( res == "-1" );
  
  value = -40;
  res = to_string(value);
  VERIFY( res == "-40" );
  
  value = -32768;
  res = to_string(value);
  VERIFY( res == "-32768" );
}

constexpr bool
test_all()
{
  test<int>();
  test<unsigned int>();
  test<long>();
  test<unsigned long>();
  test<long long>();
  test<unsigned long long>();
  return true;
}

static_assert(test_all());
