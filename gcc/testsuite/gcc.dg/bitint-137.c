/* { dg-do compile { target bitint575 } } */
/* { dg-options "-O2" } */

unsigned _BitInt(575) x, y;
signed _BitInt(575) z;
unsigned _BitInt(568) v, w;

int
foo ()
{
  return __builtin_clzg (x, 575);
}

int
bar ()
{
  return __builtin_ctzg (x, 575);
}

int
baz ()
{
  return __builtin_clrsbg (z);
}

int
qux ()
{
  return __builtin_ffsg (z);
}

int
corge ()
{
  return __builtin_parityg (x);
}

int
garply ()
{
  return __builtin_popcountg (x);
}

void
fred ()
{
  v = __builtin_bswapg (v);
}

void
grault ()
{
  w = __builtin_bswapg (v);
}

void
waldo ()
{
  x = __builtin_bitreverseg (x);
}

void
xyzzy ()
{
  y = __builtin_bitreverseg (x);
}
