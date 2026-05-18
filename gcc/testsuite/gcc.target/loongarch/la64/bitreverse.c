/* { dg-do compile } */
/* { dg-options "-O2" } */
/* { dg-final { check-function-bodies "**" "" "" } } */

/*
** t1:
**	bitrev\.d	\$r4,\$r4
**	jr	\$r1
*/
long
t1 (long x)
{
  return __builtin_bitreverse64 (x);
}

/*
** t2:
**	bitrev\.w	\$r4,\$r4
**	jr	\$r1
*/
int
t2 (int x)
{
  return __builtin_bitreverse32 (x);
}

/*
** t3:
**	bitrev\.d	(\$r[0-9]+),\$r4
**	srai\.d	\$r4,\1,48
**	jr	\$r1
*/
short
t3 (short x)
{
  return __builtin_bitreverse16 (x);
}

/*
** t4:
**	bitrev\.4b	(\$r[0-9]+),\$r4
**	ext\.w\.b	\$r4,\1
**	jr	\$r1
*/
char
t4 (char x)
{
  return __builtin_bitreverse8 (x);
}

/*
** t5:
**	bitrev\.d	(\$r[0-9]+),\$r4
**	srli\.d	\$r4,\1,48
**	jr	\$r1
*/
unsigned short
t5 (short x)
{
  return __builtin_bitreverse16 (x);
}

/*
** t6:
**	bitrev\.4b	(\$r[0-9]+),\$r4
**	bstrpick\.w	\$r4,\1,7,0
**	jr	\$r1
*/
unsigned char
t6 (char x)
{
  return __builtin_bitreverse8 (x);
}
