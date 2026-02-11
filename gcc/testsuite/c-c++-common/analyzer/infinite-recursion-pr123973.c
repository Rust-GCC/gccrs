#define Z 8

void fun(int x, int y)
{
  if (x + y > Z)
    fun(x, Z - x); /* { dg-bogus "infinite recursion" } */
}
