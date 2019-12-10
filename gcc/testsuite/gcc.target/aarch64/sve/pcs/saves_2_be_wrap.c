/* { dg-do compile } */
/* { dg-options "-O -mbig-endian -fshrink-wrap -fno-stack-clash-protection -g" } */
/* { dg-final { check-function-bodies "**" "" } } */

void standard_callee (void);
__attribute__((aarch64_vector_pcs)) void vpcs_callee (void);

/*
** calls_standard:
**	stp	x29, x30, \[sp, -16\]!
**	mov	x29, sp
**	addvl	sp, sp, #-17
**	str	p4, \[sp\]
**	str	p5, \[sp, #1, mul vl\]
**	str	p6, \[sp, #2, mul vl\]
**	str	p7, \[sp, #3, mul vl\]
**	str	p8, \[sp, #4, mul vl\]
**	str	p9, \[sp, #5, mul vl\]
**	str	p10, \[sp, #6, mul vl\]
**	str	p11, \[sp, #7, mul vl\]
**	ptrue	p0\.b, all
**	st1d	z8\.d, p0, \[sp, #1, mul vl\]
**	st1d	z9\.d, p0, \[sp, #2, mul vl\]
**	st1d	z10\.d, p0, \[sp, #3, mul vl\]
**	st1d	z11\.d, p0, \[sp, #4, mul vl\]
**	st1d	z12\.d, p0, \[sp, #5, mul vl\]
**	st1d	z13\.d, p0, \[sp, #6, mul vl\]
**	st1d	z14\.d, p0, \[sp, #7, mul vl\]
**	addvl	x11, sp, #16
**	st1d	z15\.d, p0, \[x11, #-8, mul vl\]
**	str	z16, \[sp, #9, mul vl\]
**	str	z17, \[sp, #10, mul vl\]
**	str	z18, \[sp, #11, mul vl\]
**	str	z19, \[sp, #12, mul vl\]
**	str	z20, \[sp, #13, mul vl\]
**	str	z21, \[sp, #14, mul vl\]
**	str	z22, \[sp, #15, mul vl\]
**	str	z23, \[sp, #16, mul vl\]
**	bl	standard_callee
**	ptrue	p0\.b, all
**	ld1d	z8\.d, p0/z, \[sp, #1, mul vl\]
**	ld1d	z9\.d, p0/z, \[sp, #2, mul vl\]
**	ld1d	z10\.d, p0/z, \[sp, #3, mul vl\]
**	ld1d	z11\.d, p0/z, \[sp, #4, mul vl\]
**	ld1d	z12\.d, p0/z, \[sp, #5, mul vl\]
**	ld1d	z13\.d, p0/z, \[sp, #6, mul vl\]
**	ld1d	z14\.d, p0/z, \[sp, #7, mul vl\]
**	addvl	x11, sp, #16
**	ld1d	z15\.d, p0/z, \[x11, #-8, mul vl\]
**	ldr	z16, \[sp, #9, mul vl\]
**	ldr	z17, \[sp, #10, mul vl\]
**	ldr	z18, \[sp, #11, mul vl\]
**	ldr	z19, \[sp, #12, mul vl\]
**	ldr	z20, \[sp, #13, mul vl\]
**	ldr	z21, \[sp, #14, mul vl\]
**	ldr	z22, \[sp, #15, mul vl\]
**	ldr	z23, \[sp, #16, mul vl\]
**	ldr	p4, \[sp\]
**	ldr	p5, \[sp, #1, mul vl\]
**	ldr	p6, \[sp, #2, mul vl\]
**	ldr	p7, \[sp, #3, mul vl\]
**	ldr	p8, \[sp, #4, mul vl\]
**	ldr	p9, \[sp, #5, mul vl\]
**	ldr	p10, \[sp, #6, mul vl\]
**	ldr	p11, \[sp, #7, mul vl\]
**	addvl	sp, sp, #17
**	ldp	x29, x30, \[sp\], 16
**	ret
*/
void calls_standard (__SVInt8_t x) { standard_callee (); }

/*
** calls_vpcs:
**	stp	x29, x30, \[sp, -16\]!
**	mov	x29, sp
**	addvl	sp, sp, #-17
**	str	p4, \[sp\]
**	str	p5, \[sp, #1, mul vl\]
**	str	p6, \[sp, #2, mul vl\]
**	str	p7, \[sp, #3, mul vl\]
**	str	p8, \[sp, #4, mul vl\]
**	str	p9, \[sp, #5, mul vl\]
**	str	p10, \[sp, #6, mul vl\]
**	str	p11, \[sp, #7, mul vl\]
**	ptrue	p0\.b, all
**	st1d	z8\.d, p0, \[sp, #1, mul vl\]
**	st1d	z9\.d, p0, \[sp, #2, mul vl\]
**	st1d	z10\.d, p0, \[sp, #3, mul vl\]
**	st1d	z11\.d, p0, \[sp, #4, mul vl\]
**	st1d	z12\.d, p0, \[sp, #5, mul vl\]
**	st1d	z13\.d, p0, \[sp, #6, mul vl\]
**	st1d	z14\.d, p0, \[sp, #7, mul vl\]
**	addvl	x11, sp, #16
**	st1d	z15\.d, p0, \[x11, #-8, mul vl\]
**	str	z16, \[sp, #9, mul vl\]
**	str	z17, \[sp, #10, mul vl\]
**	str	z18, \[sp, #11, mul vl\]
**	str	z19, \[sp, #12, mul vl\]
**	str	z20, \[sp, #13, mul vl\]
**	str	z21, \[sp, #14, mul vl\]
**	str	z22, \[sp, #15, mul vl\]
**	str	z23, \[sp, #16, mul vl\]
**	bl	vpcs_callee
**	ptrue	p0\.b, all
**	ld1d	z8\.d, p0/z, \[sp, #1, mul vl\]
**	ld1d	z9\.d, p0/z, \[sp, #2, mul vl\]
**	ld1d	z10\.d, p0/z, \[sp, #3, mul vl\]
**	ld1d	z11\.d, p0/z, \[sp, #4, mul vl\]
**	ld1d	z12\.d, p0/z, \[sp, #5, mul vl\]
**	ld1d	z13\.d, p0/z, \[sp, #6, mul vl\]
**	ld1d	z14\.d, p0/z, \[sp, #7, mul vl\]
**	addvl	x11, sp, #16
**	ld1d	z15\.d, p0/z, \[x11, #-8, mul vl\]
**	ldr	z16, \[sp, #9, mul vl\]
**	ldr	z17, \[sp, #10, mul vl\]
**	ldr	z18, \[sp, #11, mul vl\]
**	ldr	z19, \[sp, #12, mul vl\]
**	ldr	z20, \[sp, #13, mul vl\]
**	ldr	z21, \[sp, #14, mul vl\]
**	ldr	z22, \[sp, #15, mul vl\]
**	ldr	z23, \[sp, #16, mul vl\]
**	ldr	p4, \[sp\]
**	ldr	p5, \[sp, #1, mul vl\]
**	ldr	p6, \[sp, #2, mul vl\]
**	ldr	p7, \[sp, #3, mul vl\]
**	ldr	p8, \[sp, #4, mul vl\]
**	ldr	p9, \[sp, #5, mul vl\]
**	ldr	p10, \[sp, #6, mul vl\]
**	ldr	p11, \[sp, #7, mul vl\]
**	addvl	sp, sp, #17
**	ldp	x29, x30, \[sp\], 16
**	ret
*/
void calls_vpcs (__SVInt8_t x) { vpcs_callee (); }

/*
** calls_standard_ptr:
**	stp	x29, x30, \[sp, -16\]!
**	mov	x29, sp
**	addvl	sp, sp, #-17
**	str	p4, \[sp\]
**	str	p5, \[sp, #1, mul vl\]
**	str	p6, \[sp, #2, mul vl\]
**	str	p7, \[sp, #3, mul vl\]
**	str	p8, \[sp, #4, mul vl\]
**	str	p9, \[sp, #5, mul vl\]
**	str	p10, \[sp, #6, mul vl\]
**	str	p11, \[sp, #7, mul vl\]
**	ptrue	p0\.b, all
**	st1d	z8\.d, p0, \[sp, #1, mul vl\]
**	st1d	z9\.d, p0, \[sp, #2, mul vl\]
**	st1d	z10\.d, p0, \[sp, #3, mul vl\]
**	st1d	z11\.d, p0, \[sp, #4, mul vl\]
**	st1d	z12\.d, p0, \[sp, #5, mul vl\]
**	st1d	z13\.d, p0, \[sp, #6, mul vl\]
**	st1d	z14\.d, p0, \[sp, #7, mul vl\]
**	addvl	x11, sp, #16
**	st1d	z15\.d, p0, \[x11, #-8, mul vl\]
**	str	z16, \[sp, #9, mul vl\]
**	str	z17, \[sp, #10, mul vl\]
**	str	z18, \[sp, #11, mul vl\]
**	str	z19, \[sp, #12, mul vl\]
**	str	z20, \[sp, #13, mul vl\]
**	str	z21, \[sp, #14, mul vl\]
**	str	z22, \[sp, #15, mul vl\]
**	str	z23, \[sp, #16, mul vl\]
**	blr	x0
**	ptrue	p0\.b, all
**	ld1d	z8\.d, p0/z, \[sp, #1, mul vl\]
**	ld1d	z9\.d, p0/z, \[sp, #2, mul vl\]
**	ld1d	z10\.d, p0/z, \[sp, #3, mul vl\]
**	ld1d	z11\.d, p0/z, \[sp, #4, mul vl\]
**	ld1d	z12\.d, p0/z, \[sp, #5, mul vl\]
**	ld1d	z13\.d, p0/z, \[sp, #6, mul vl\]
**	ld1d	z14\.d, p0/z, \[sp, #7, mul vl\]
**	addvl	x11, sp, #16
**	ld1d	z15\.d, p0/z, \[x11, #-8, mul vl\]
**	ldr	z16, \[sp, #9, mul vl\]
**	ldr	z17, \[sp, #10, mul vl\]
**	ldr	z18, \[sp, #11, mul vl\]
**	ldr	z19, \[sp, #12, mul vl\]
**	ldr	z20, \[sp, #13, mul vl\]
**	ldr	z21, \[sp, #14, mul vl\]
**	ldr	z22, \[sp, #15, mul vl\]
**	ldr	z23, \[sp, #16, mul vl\]
**	ldr	p4, \[sp\]
**	ldr	p5, \[sp, #1, mul vl\]
**	ldr	p6, \[sp, #2, mul vl\]
**	ldr	p7, \[sp, #3, mul vl\]
**	ldr	p8, \[sp, #4, mul vl\]
**	ldr	p9, \[sp, #5, mul vl\]
**	ldr	p10, \[sp, #6, mul vl\]
**	ldr	p11, \[sp, #7, mul vl\]
**	addvl	sp, sp, #17
**	ldp	x29, x30, \[sp\], 16
**	ret
*/
void
calls_standard_ptr (__SVInt8_t x, void (*fn) (void))
{
  fn ();
}

/*
** calls_vpcs_ptr:
**	stp	x29, x30, \[sp, -16\]!
**	mov	x29, sp
**	addvl	sp, sp, #-17
**	str	p4, \[sp\]
**	str	p5, \[sp, #1, mul vl\]
**	str	p6, \[sp, #2, mul vl\]
**	str	p7, \[sp, #3, mul vl\]
**	str	p8, \[sp, #4, mul vl\]
**	str	p9, \[sp, #5, mul vl\]
**	str	p10, \[sp, #6, mul vl\]
**	str	p11, \[sp, #7, mul vl\]
**	ptrue	p0\.b, all
**	st1d	z8\.d, p0, \[sp, #1, mul vl\]
**	st1d	z9\.d, p0, \[sp, #2, mul vl\]
**	st1d	z10\.d, p0, \[sp, #3, mul vl\]
**	st1d	z11\.d, p0, \[sp, #4, mul vl\]
**	st1d	z12\.d, p0, \[sp, #5, mul vl\]
**	st1d	z13\.d, p0, \[sp, #6, mul vl\]
**	st1d	z14\.d, p0, \[sp, #7, mul vl\]
**	addvl	x11, sp, #16
**	st1d	z15\.d, p0, \[x11, #-8, mul vl\]
**	str	z16, \[sp, #9, mul vl\]
**	str	z17, \[sp, #10, mul vl\]
**	str	z18, \[sp, #11, mul vl\]
**	str	z19, \[sp, #12, mul vl\]
**	str	z20, \[sp, #13, mul vl\]
**	str	z21, \[sp, #14, mul vl\]
**	str	z22, \[sp, #15, mul vl\]
**	str	z23, \[sp, #16, mul vl\]
**	blr	x0
**	ptrue	p0\.b, all
**	ld1d	z8\.d, p0/z, \[sp, #1, mul vl\]
**	ld1d	z9\.d, p0/z, \[sp, #2, mul vl\]
**	ld1d	z10\.d, p0/z, \[sp, #3, mul vl\]
**	ld1d	z11\.d, p0/z, \[sp, #4, mul vl\]
**	ld1d	z12\.d, p0/z, \[sp, #5, mul vl\]
**	ld1d	z13\.d, p0/z, \[sp, #6, mul vl\]
**	ld1d	z14\.d, p0/z, \[sp, #7, mul vl\]
**	addvl	x11, sp, #16
**	ld1d	z15\.d, p0/z, \[x11, #-8, mul vl\]
**	ldr	z16, \[sp, #9, mul vl\]
**	ldr	z17, \[sp, #10, mul vl\]
**	ldr	z18, \[sp, #11, mul vl\]
**	ldr	z19, \[sp, #12, mul vl\]
**	ldr	z20, \[sp, #13, mul vl\]
**	ldr	z21, \[sp, #14, mul vl\]
**	ldr	z22, \[sp, #15, mul vl\]
**	ldr	z23, \[sp, #16, mul vl\]
**	ldr	p4, \[sp\]
**	ldr	p5, \[sp, #1, mul vl\]
**	ldr	p6, \[sp, #2, mul vl\]
**	ldr	p7, \[sp, #3, mul vl\]
**	ldr	p8, \[sp, #4, mul vl\]
**	ldr	p9, \[sp, #5, mul vl\]
**	ldr	p10, \[sp, #6, mul vl\]
**	ldr	p11, \[sp, #7, mul vl\]
**	addvl	sp, sp, #17
**	ldp	x29, x30, \[sp\], 16
**	ret
*/
void
calls_vpcs_ptr (__SVInt8_t x,
		void (*__attribute__((aarch64_vector_pcs)) fn) (void))
{
  fn ();
}
