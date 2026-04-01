/* { dg-do compile } */
/* { dg-options "-O -w -mno-sse2 -mstringop-strategy=loop --param=hot-bb-frequency-fraction=0" } */
typedef char U __attribute__((__vector_size__ (64)));
typedef __int128 V __attribute__((__vector_size__ (64)));
typedef short W __attribute__((__vector_size__ (64)));

U u;
V v;
W w;

V
foo (int i, __int128 y)
{
  w = (W){};
  u -= (U)(W){1, i, i};
  return i + y + (V) u + v;
}
