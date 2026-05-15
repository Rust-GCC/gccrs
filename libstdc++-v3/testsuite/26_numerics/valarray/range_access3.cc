// { dg-do run { target c++26 } }

#include <valarray>
#include <testsuite_hooks.h>

#ifndef __cpp_lib_valarray
# error "Feature-test macro for text_encoding missing in <initializer_list>"
#elif __cpp_lib_valarray != 202511L
# error "Feature-test macro for text_encoding has wrong value in <initializer_list>"
#endif

void
test01()
{
  std::valarray<double> va(3);
  va[0] = 1.0; va[1] = 2.0; va[2] = 3.0;

  typename std::valarray<double>::iterator it = va.begin();
  VERIFY( it != va.end() );
  VERIFY( *it++ == 1.0 );
  VERIFY( *it++ == 2.0 );
  VERIFY( *it++ == 3.0 );
  VERIFY( it == va.end() );

  const std::valarray<double>& cva = va;
  typename std::valarray<double>::const_iterator cit = cva.begin();
  VERIFY( cit != va.end() );
  VERIFY( *cit++ == 1.0 );
  VERIFY( *cit++ == 2.0 );
  VERIFY( *cit++ == 3.0 );
  VERIFY( cit == cva.end() );
}

void
test02()
{
  std::valarray<double> va;
  VERIFY( va.begin() == va.end() );
  const std::valarray<double>& cva = va;
  VERIFY( cva.begin() == cva.end() );
}

int main()
{
  test01();
  test02();
}
