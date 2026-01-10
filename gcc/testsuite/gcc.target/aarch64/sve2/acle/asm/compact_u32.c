/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-skip-if "" { *-*-* } { "" } { "-DSTREAMING_COMPATIBLE" } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sme2p2"

#include "../../../sve/acle/asm/compact_u32.c"

/*
** compact_u32_tied1:
**	compact	z0\.s, p0, z0\.s
**	ret
*/

/*
** compact_u32_untied:
**	compact	z0\.s, p0, z1\.s
**	ret
*/
