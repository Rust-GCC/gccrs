// { dg-do run { target c++11 } }

#include <vector>
#include <testsuite_hooks.h>

void
test_friend()
{
  std::vector<bool> v{true, false};
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

void
test_static()
{
#ifndef _GLIBCXX_DEBUG
  std::vector<bool> v{true, false};
  std::vector<bool>::swap(v[0], v[1]); // { dg-warning "deprecated" "::swap" { target { c++26 && { ! debug_mode } } } }
  VERIFY( v[0] == false );
  VERIFY( v[1] == true );
#endif
}

int
main()
{
  test_friend();
  test_static();
}
