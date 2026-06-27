/* PR target/125992 */
/* { dg-do run { target s390_useable_hw } } */
/* { dg-options "-O2 -march=z13" } */

long a, b, c, d[4];

[[gnu::noipa]] long
foo ()
{
  return d[c++];
}

[[gnu::noipa]] void
bar ()
{
  long e = 0;
  for (;;)
    {
      e += a;
      a = foo ();
      if (b + a > e)
	return;
      if (a == 0)
	break;
      if (a > 0)
	b += a;
    }
}

int
main ()
{
  a = 16;
  b = -64;
  c = 0;
  d[0] = 64;
  d[1] = 128;
  bar ();
  if (b != 0)
    __builtin_abort ();
}
