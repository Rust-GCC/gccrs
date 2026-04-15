/* PR middle-end/123635 */
/* { dg-do run { target bitint } } */
/* { dg-require-effective-target sync_char_short } */
/* { dg-options "-std=c23 -pedantic-errors" } */
/* { dg-skip-if "" { ! run_expensive_tests }  { "*" } { "-O0" "-O2" } } */
/* { dg-skip-if "" { ! run_expensive_tests } { "-flto" } { "" } } */

#if __BITINT_MAXWIDTH__ >= 513
_BitInt(513) a, b, c, d;
unsigned _BitInt(513) e, f, g, h;
_BitInt(513) i, j, k;
unsigned _BitInt(513) l, m, n;
#endif

#include "../bitintext.h"

#if __BITINT_MAXWIDTH__ >= 513
[[gnu::noipa]] void
f1 (_BitInt(513) q, _BitInt(513) r, _BitInt(513) s,
    unsigned _BitInt(513) t, unsigned _BitInt(513) u, unsigned _BitInt(513) v)
{
  a = q * r;
  BEXTC (a);
  b = r * s;
  BEXTC (b);
  c = q / r;
  BEXTC (c);
  d = q / s;
  BEXTC (d);
  e = t * u;
  BEXTC (e);
  f = u * v;
  BEXTC (f);
  g = t / u;
  BEXTC (g);
  h = t / v;
  BEXTC (h);
}

[[gnu::noipa]] void
f2 (float q, double r, long double s, float t, double u, long double v)
{
  i = q;
  BEXTC (i);
  j = r;
  BEXTC (j);
  k = s;
  BEXTC (k);
  l = t;
  BEXTC (l);
  m = u;
  BEXTC (m);
  n = v;
  BEXTC (n);
}
#endif

int
main ()
{
#if __BITINT_MAXWIDTH__ >= 513
  __builtin_memset (&a, 0x55, sizeof (a));
  __builtin_memset (&b, 0xaa, sizeof (b));
  __builtin_memset (&c, 0x55, sizeof (c));
  __builtin_memset (&d, 0xaa, sizeof (d));
  __builtin_memset (&e, 0x55, sizeof (e));
  __builtin_memset (&f, 0xaa, sizeof (f));
  __builtin_memset (&g, 0x55, sizeof (g));
  __builtin_memset (&h, 0xaa, sizeof (h));
  f1 (-53323980256963787505256507743137477556434962931963225515943461794698643113423wb,
      -10076482373458251489901780456236592759327822657780415144730546867053397315531wb,
      9430367348600775477158545473775377451258484445522540280907903691748059121081wb,
      15046745594550617619205422464231805109110883864578289024439083517871820578179553927615539526791313634437787081814763432804808038115388367331529035246240655uwb,
      20633637828717837096174917874088391607464281656818868213468970773994599068609617673436080725569780340050358299419252926775025136778754971701553110169281uwb,
      5136122090451895036220764749952166060863831396714271041799786889820341177646647112770727950839029515643589984981094121423221389337601736282556974uwb);
  __builtin_memset (&i, 0x55, sizeof (i));
  __builtin_memset (&j, 0xaa, sizeof (j));
  __builtin_memset (&k, 0x55, sizeof (k));
  __builtin_memset (&l, 0xaa, sizeof (l));
  __builtin_memset (&m, 0x55, sizeof (m));
  __builtin_memset (&n, 0xaa, sizeof (n));
  f2 (12345678.5f, -234567891234567.125, 123465987893275.53244532L,
      12345678.5f, 234567891234567.125, 123465987893275.53244532L);
#endif
}
