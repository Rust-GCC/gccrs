// CWG 3123
// { dg-do run { target c++26 } }

#include <tuple>

int
main ()
{
  long l = 0;
  std::tuple <int, long, unsigned> t = { 1, 2L, 3U };
  template for (auto &&x : t)
    l += x;
  if (l != 6L)
    __builtin_abort ();
}
