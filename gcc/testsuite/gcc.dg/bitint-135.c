/* C2Y N3705: bit-precise enum.  */
/* { dg-do run { target bitint } } */
/* { dg-options "-std=c2y" } */

#if __BITINT_MAXWIDTH__ >= 257
enum A : unsigned _BitInt(7) { A0 = 0, A1 = 7 };
enum B : unsigned _BitInt(79) { B0 = 0, B1 = 255 };
enum C : _BitInt(25) { C0 = -8, C1 = 7 };
enum D : signed _BitInt(182) { D0 = -61822874253726891040447382843909510144wb, D1 = 333265406300494622897688995893630121068wb };
enum E : _BitInt(__BITINT_MAXWIDTH__) { E0 = -1, E1 = 1 };
struct F { enum A a : 3; enum B b : 67; enum C c : 4; enum D d : 129; enum E e : 257; } f;
#endif

[[gnu::noipa]] void
foo ()
{
  if (f.a != A1
      || f.b != B1
      || f.c != C1
      || f.d != D1
      || f.e != E1)
    __builtin_abort ();
  f.a = A0;
  f.b = B0;
  f.c = C0;
  f.d = D0;
  f.e = E0;
}

int
main ()
{
#if __BITINT_MAXWIDTH__ >= 257
  f.a = A1;
  f.b = B1;
  f.c = C1;
  f.d = D1;
  f.e = E1;
  foo ();
  if (f.a != A0
      || f.b != B0
      || f.c != C0
      || f.d != D0
      || f.e != E0)
    __builtin_abort ();
#endif
}
