/* { dg-do compile } */
/* { dg-options "-O2 -march=armv9.5-a" } */
/* { dg-final { check-function-bodies "**" "" "" } } */

#define vect16 __attribute__((vector_size(16)))

vect16 char fff(char _292, char _145, char _231)
{
    return (vect16 char) {_292, _145, _145, _231, _292, _145, _145, _231, _292, _145, _145, _231, _292, _145, _145, _231};
}

/*
** fff:
**	dup	v31\.8b, w1
**	fmov	w1, s31
**	bfi	w1, w0, 0, 8
**	bfi	w1, w2, 24, 8
**	dup	v0\.4s, w1
**	ret
*/
