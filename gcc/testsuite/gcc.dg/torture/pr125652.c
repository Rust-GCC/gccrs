/* { dg-do run } */
/* { dg-require-effective-target int32plus } */

#include <stdint.h>

int a, b, c, d;

void e(int64_t f, int64_t g)
{
  int64_t h = 1, *i, **j;
  while (1)
    {
      i = &h;
      j = &i;
      if (h <= h + f - ~h)
        break;
      *i += g;
    }
  c *= 2;
  d = a >> b + d;
}

int f123()
{
  e(-2147483627, 4611686018427387904);
  return 0;
}

int main()
{
  return f123();
}
