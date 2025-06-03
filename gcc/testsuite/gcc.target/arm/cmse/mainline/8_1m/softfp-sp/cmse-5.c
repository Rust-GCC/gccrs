/* { dg-do compile } */
/* { dg-options "-mcmse -mfloat-abi=softfp -mfpu=fpv5-sp-d16" }  */
/* { dg-skip-if "Incompatible float ABI" { *-*-* } { "-mfloat-abi=*" } { "-mfloat-abi=softfp" } } */
/* { dg-skip-if "Skip these if testing double precision" {*-*-*} {"-mfpu=fpv[4-5]-d16"} {""} } */
/* { dg-final { check-function-bodies "**" "" "" } } */

#include "../../../cmse-5.x"

/*
** __acle_se_foo:
**	vstr	FPCXTNS, \[sp, #-4\]!
** ...
**	vscclrm	\{s0-s15, VPR\}
**	clrm	\{r1, r2, r3, ip, APSR\}
**	vldr	FPCXTNS, \[sp\], #4
**	bxns	lr
*/

/* { dg-final { scan-assembler-not "mov\tr0, lr" } } */
