/* { dg-do assemble { target aarch64_asm_sve2p2_ok } } */
/* { dg-do compile { target { ! aarch64_asm_sve2p2_ok } } } */
/* { dg-skip-if "" { *-*-* } { "" } { "-DSTREAMING_COMPATIBLE" } } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

#pragma GCC target "+sme2p2"

#include "../../../sve/acle/asm/compact_u64.c"

/*
** compact_u64_tied1:
**	compact	z0\.d, p0, z0\.d
**	ret
*/

/*
** compact_u64_untied:
**	compact	z0\.d, p0, z1\.d
**	ret
*/
