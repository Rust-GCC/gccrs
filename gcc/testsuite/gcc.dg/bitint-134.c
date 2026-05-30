/* C2Y N3705: bit-precise enum.  */
/* { dg-do compile { target bitint } } */
/* { dg-options "-std=c2y -Wall" } */

enum E : unsigned _BitInt(1) { E1 = 0, E2 = 1 };
#if __BITINT_MAXWITH__ >= 977
enum F : _BitInt(976) { F1 = 1wb, F2 = 300670311812537087458442068171242480163042891902139164259644606340061247186864125993521170465570759574674133007538491713720385607199458367860097993945704706551423706008678936855195978990583102392208311054636025123775462104768336555710742464822135886690195218326330916446029087852132145116425130wb, F3 = ~(unsigned _BitInt(975)) 0 };
#endif

int
foo (enum E x)
{
  switch (x)
    {
    case E1:
      return 1;
    case E2:
      return 2;
    }
}

#if __BITINT_MAXWITH__ >= 977
_BitInt(977)
bar (enum F x)
{
  return x + 1;
}

enum F
baz (enum F x)
{
  switch (x)
    {
    case F1:
      return F2;
    case F2:
      return F3;
    default:
      return F1;
    }
}
#endif
