/* Support routine for SME.
   Copyright (C) 2026 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 3, or (at your
   option) any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   Under Section 7 of GPL version 3, you are granted additional
   permissions described in the GCC Runtime Library Exception, version
   3.1, as published by the Free Software Foundation.

   You should have received a copy of the GNU General Public License and
   a copy of the GCC Runtime Library Exception along with this program;
   see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
   <http://www.gnu.org/licenses/>.  */

#include "aarch64-asm.h"

.arch armv8-a

#define result			x0
#define src_in			x0
#define chr_in			w1
#define chr_repeated	x1
#define count_in		x2

#define src				x3
#define zeroones		x4
#define	data1			x5
#define	tmp1			x6
#define tmp2			x7
#define found1			x8
#define src_end			x9

#define REP8_01 0x0101010101010101
#define REP8_7f 0x7f7f7f7f7f7f7f7f

/* This function implements the hacker's delight algorithm for searching
   matching values. We find them in words by performing an XOR against the
   repeated searched char. Then, x being the xor result, we calculate
   (x - 0x0101010101010101) & ~(x | 0x7f7f7f7f7f7f7f7f). Words with a matching
   byte will produce a non-zero result.

   __arm_sc_mem* functions must be able to run in streaming mode and can thus
   not make use of most AdvSIMD instructions. */

variant_pcs (__arm_sc_memchr)

ENTRY (__arm_sc_memchr)
	cbz	count_in, L(no_match)

	/* Replicate the needle byte across the rest of the word.  */
	and	chr_in, chr_in, 255
	orr	chr_in, chr_in, chr_in, lsl 8
	orr	chr_in, chr_in, chr_in, lsl 16
	orr	chr_repeated, chr_repeated, chr_repeated, lsl 32
	mov	zeroones, REP8_01

	bic	src, src_in, 7
	ldr	data1, [src], 8
#ifdef __AARCH64EB__
	rev	data1, data1
#endif
	/* Skip unused bytes before src_in.  */
	lsl	tmp1, src_in, 3
	lsr	tmp2, zeroones, tmp1
	lsr	data1, data1, tmp1

	eor	data1, data1, chr_repeated
	sub	tmp1, data1, tmp2
	orr	tmp2, data1, REP8_7f
	bic	found1, tmp1, tmp2
	cbz	found1, L(loop_start)

    /* Calculate the offset and return it if offset < count_in. */
	rbit	found1, found1
	clz	tmp1, found1
	cmp	count_in, tmp1, lsr 3
	add	result, src_in, tmp1, lsr 3
	csel	result, result, xzr, hi
	ret

	.p2align 4

L(no_match):
	mov	result, 0
	ret

L(loop_start):
	/* Deal with count_in being so large that src_end comes before src_in due
	   to wraparound. When this happens, set src_end to SIZE_MAX so that we do
	   not reject results based on the address.  */
	adds	src_end, src_in, count_in
	csinv	src_end, src_end, xzr, cc

	.p2align 4
L(loop):
	cmp	src, src_end
	b.hs	L(no_match)
	ldr	data1, [src], 8
#ifdef __AARCH64EB__
	rev	data1, data1
#endif
	eor	data1, data1, chr_repeated
	sub	tmp1, data1, zeroones
	orr	tmp2, data1, REP8_7f
	bics	xzr, tmp1, tmp2

	b.eq	L(loop)

	/* Found a match - return the address if it is before src_end.  */
	bic	found1, tmp1, tmp2
	sub	src, src, 8
	rbit	found1, found1
	clz	tmp1, found1
	add	result, src, tmp1, lsr 3
	cmp	src_end, result
	csel	result, result, xzr, hi
	ret
END (__arm_sc_memchr)
