/* C2Y N3705: bit-precise enum.  */
/* { dg-do compile { target bitint } } */
/* { dg-options "-std=c2y" } */

#define expr_has_type(e, t) _Generic (e, default : 0, t : 1)

#if __BITINT_MAXWIDTH__ >= 129
enum A : unsigned _BitInt(3) { A0 = 0, A1 = 7 };
enum B : unsigned _BitInt(67) { B0 = 0, B1 = 255 };
enum C : _BitInt(4) { C0 = -8, C1 = 7 };
enum D : signed _BitInt(129) { D0 = -61822874253726891040447382843909510144wb, D1 = 333265406300494622897688995893630121068wb };
enum E : _BitInt(__BITINT_MAXWIDTH__) { E0 = -1, E1 = 1 };
#endif

int
main ()
{
#if __BITINT_MAXWIDTH__ >= 129
  enum A a = A0;
  static_assert (expr_has_type (a, enum A));
  static_assert (expr_has_type (+a, unsigned _BitInt(3)));
  static_assert (expr_has_type (a + 0wb, unsigned _BitInt(3)));
  static_assert (expr_has_type (a + a, unsigned _BitInt(3)));
  enum B b = B0;
  static_assert (expr_has_type (b, enum B));
  static_assert (expr_has_type (+b, unsigned _BitInt(67)));
  static_assert (expr_has_type (b + 0wb, unsigned _BitInt(67)));
  static_assert (expr_has_type (b + b, unsigned _BitInt(67)));
  enum C c = C0;
  static_assert (expr_has_type (c, enum C));
  static_assert (expr_has_type (+c, _BitInt(4)));
  static_assert (expr_has_type (c + 0wb, _BitInt(4)));
  static_assert (expr_has_type (c + c, _BitInt(4)));
  enum D d = D0;
  static_assert (expr_has_type (d, enum D));
  static_assert (expr_has_type (+d, signed _BitInt(129)));
  static_assert (expr_has_type (d + 0wb, signed _BitInt(129)));
  static_assert (expr_has_type (d + d, signed _BitInt(129)));
  enum E e = E0;
  static_assert (expr_has_type (e, enum E));
  static_assert (expr_has_type (+e, signed _BitInt(__BITINT_MAXWIDTH__)));
  static_assert (expr_has_type (e + 0wb, signed _BitInt(__BITINT_MAXWIDTH__)));
  static_assert (expr_has_type (e + e, signed _BitInt(__BITINT_MAXWIDTH__)));
#endif
}
