// { dg-do run { target c++11 } }

#include <bitset>
#include <testsuite_hooks.h>

void
test_friend()
{
  std::bitset<2> v("01");
  swap(v[0], v[1]);
  VERIFY( v[0] == false );
  VERIFY( v[1] == true );

  bool b = true;
  swap(v[0], b);
  VERIFY( v[0] == true );
  VERIFY( b == false );

  swap(b, v[1]);
  VERIFY( v[1] == false );
  VERIFY( b == true );
}

int
main()
{
  test_friend();
}
