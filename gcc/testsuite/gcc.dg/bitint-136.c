/* C2Y N3705: bit-precise enum.  */
/* { dg-do compile { target bitint } } */
/* { dg-options "-std=c2y" } */

#define expr_has_type(e, t) _Generic (e, default : 0, t : 1)

#if __BITINT_MAXWIDTH__ >= 257
enum A : unsigned _BitInt(7) { A0 = 0, A1 = 7 };
enum B : unsigned _BitInt(79) { B0 = 0, B1 = 255 };
enum C : _BitInt(25) { C0 = -8, C1 = 7 };
enum D : signed _BitInt(182) { D0 = -61822874253726891040447382843909510144wb, D1 = 333265406300494622897688995893630121068wb };
enum E : _BitInt(__BITINT_MAXWIDTH__) { E0 = -1, E1 = 1 };
struct F { enum A a : 3; enum B b : 67; enum C c : 4; enum D d : 129; enum E e : 257; } f;
#endif

int
main ()
{
#if __BITINT_MAXWIDTH__ >= 257
  static_assert (expr_has_type (+f.a, unsigned _BitInt(7)));
  static_assert (expr_has_type (f.a + 0wb, unsigned _BitInt(7)));
  static_assert (expr_has_type (f.a + f.a, unsigned _BitInt(7)));
  static_assert (expr_has_type (+f.b, unsigned _BitInt(79)));
  static_assert (expr_has_type (f.b + 0wb, unsigned _BitInt(79)));
  static_assert (expr_has_type (f.b + f.b, unsigned _BitInt(79)));
  static_assert (expr_has_type (+f.c, _BitInt(25)));
  static_assert (expr_has_type (f.c + 0wb, _BitInt(25)));
  static_assert (expr_has_type (f.c + f.c, _BitInt(25)));
  static_assert (expr_has_type (+f.d, signed _BitInt(182)));
  static_assert (expr_has_type (f.d + 0wb, signed _BitInt(182)));
  static_assert (expr_has_type (f.d + f.d, signed _BitInt(182)));
  static_assert (expr_has_type (+f.e, signed _BitInt(__BITINT_MAXWIDTH__)));
  static_assert (expr_has_type (f.e + 0wb, signed _BitInt(__BITINT_MAXWIDTH__)));
  static_assert (expr_has_type (f.e + f.e, signed _BitInt(__BITINT_MAXWIDTH__)));
#endif
}
