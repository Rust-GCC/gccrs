/* { dg-do compile } */
/* { dg-additional-options "-O -msve-vector-bits=256" } */
/* { dg-final { check-function-bodies "**" "" } } */

#include <arm_sve.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
** cntb_pow2:
**	mov	w0, 32
**	ret
*/
uint64_t cntb_pow2 () { return svcntb_pat (SV_POW2); }

/*
** cntb_vl1:
**	mov	w0, 1
**	ret
*/
uint64_t cntb_vl1 () { return svcntb_pat (SV_VL1); }

/*
** cntb_vl2:
**	mov	w0, 2
**	ret
*/
uint64_t cntb_vl2 () { return svcntb_pat (SV_VL2); }

/*
** cntb_vl3:
**	mov	w0, 3
**	ret
*/
uint64_t cntb_vl3 () { return svcntb_pat (SV_VL3); }

/*
** cntb_vl4:
**	mov	w0, 4
**	ret
*/
uint64_t cntb_vl4 () { return svcntb_pat (SV_VL4); }

/*
** cntb_vl5:
**	mov	w0, 5
**	ret
*/
uint64_t cntb_vl5 () { return svcntb_pat (SV_VL5); }

/*
** cntb_vl6:
**	mov	w0, 6
**	ret
*/
uint64_t cntb_vl6 () { return svcntb_pat (SV_VL6); }

/*
** cntb_vl7:
**	mov	w0, 7
**	ret
*/
uint64_t cntb_vl7 () { return svcntb_pat (SV_VL7); }

/*
** cntb_vl8:
**	mov	w0, 8
**	ret
*/
uint64_t cntb_vl8 () { return svcntb_pat (SV_VL8); }

/*
** cntb_vl16:
**	mov	w0, 16
**	ret
*/
uint64_t cntb_vl16 () { return svcntb_pat (SV_VL16); }

/*
** cntb_vl32:
**	mov	w0, 32
**	ret
*/
uint64_t cntb_vl32 () { return svcntb_pat (SV_VL32); }

/*
** cntb_vl64:
**	mov	w0, 0
**	ret
*/
uint64_t cntb_vl64 () { return svcntb_pat (SV_VL64); }

/*
** cntb_vl128:
**	mov	w0, 0
**	ret
*/
uint64_t cntb_vl128 () { return svcntb_pat (SV_VL128); }

/*
** cntb_vl256:
**	mov	w0, 0
**	ret
*/
uint64_t cntb_vl256 () { return svcntb_pat (SV_VL256); }

/*
** cntb_mul3:
**	mov	w0, 30
**	ret
*/
uint64_t cntb_mul3 () { return svcntb_pat (SV_MUL3); }

/*
** cntb_mul4:
**	mov	w0, 32
**	ret
*/
uint64_t cntb_mul4 () { return svcntb_pat (SV_MUL4); }

/*
** cntb_all:
**	mov	w0, 32
**	ret
*/
uint64_t cntb_all () { return svcntb_pat (SV_ALL); }

#ifdef __cplusplus
}
#endif
