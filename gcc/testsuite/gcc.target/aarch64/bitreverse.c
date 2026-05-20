/* { dg-do compile } */
/* { dg-options "-O2" } */

/*
** br8:
**	and	w0, w0, 255
**	rbit	w0, w0
**	lsr	w0, w0, 24
**	ret
*/
[[gnu::noipa]] unsigned char
br8 (unsigned char x)
{
  return __builtin_bitreverse8 (x);
}

/*
** br16:
**	and	w0, w0, 65535
**	rbit	w0, w0
**	lsr	w0, w0, 16
**	ret
*/
[[gnu::noipa]] unsigned short
br16 (unsigned short x)
{
  return __builtin_bitreverse16 (x);
}

/*
** br32:
**	rbit	w0, w0
**	ret
*/
[[gnu::noipa]] unsigned int
br32 (unsigned int x)
{
  return __builtin_bitreverse32 (x);
}

/*
** br64:
**	rbit	x0, x0
**	ret
*/
[[gnu::noipa]] unsigned long long
br64 (unsigned long long x)
{
  return __builtin_bitreverse64 (x);
}
