/* Power ISA 3.0 atomic memory operation include file.
   Copyright (C) 2017-2026 Free Software Foundation, Inc.
   Contributed by Michael Meissner <meissner@linux.vnet.ibm.com>.

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

#ifndef _AMO_H
#define _AMO_H

#if !defined(_ARCH_PWR9) || !defined(_ARCH_PPC64)
#error "The atomic memory operations require Power 64-bit ISA 3.0"

#else
#include <stdint.h>

/* Enumeration of the LWAT/LDAT sub-opcodes.  */
enum _AMO_LD {
  _AMO_LD_ADD		= 0x00,		/* Fetch and Add.  */
  _AMO_LD_XOR		= 0x01,		/* Fetch and Xor.  */
  _AMO_LD_IOR		= 0x02,		/* Fetch and Ior.  */
  _AMO_LD_AND		= 0x03,		/* Fetch and And.  */
  _AMO_LD_UMAX		= 0x04,		/* Fetch and Unsigned Maximum.  */
  _AMO_LD_SMAX		= 0x05,		/* Fetch and Signed Maximum.  */
  _AMO_LD_UMIN		= 0x06,		/* Fetch and Unsigned Minimum.  */
  _AMO_LD_SMIN		= 0x07,		/* Fetch and Signed Minimum.  */
  _AMO_LD_SWAP		= 0x08,		/* Swap.  */
  _AMO_LD_CS_NE 	= 0x10,         /* Compare and Swap Not Equal.  */
#ifdef _ARCH_FUTURE
  _AMO_LD_CS_EQ 	= 0x11,         /* Compare and Swap Equal.  */
#endif
  _AMO_LD_INC_BOUNDED	= 0x18,		/* Fetch and Increment Bounded.  */
  _AMO_LD_INC_EQUAL	= 0x19,		/* Fetch and Increment Equal.  */
  _AMO_LD_DEC_BOUNDED	= 0x1C		/* Fetch and Decrement Bounded.  */
};

/* Implementation of the simple LWAT/LDAT operations that take one register and
   modify one word or double-word of memory and return the value that was
   previously in the memory location.

   The LWAT/LDAT opcode requires the address to be a single register, and that
   points to a suitably aligned memory location.  Asm volatile is used to
   prevent the optimizer from moving the operation.  */

#define _AMO_LD_SIMPLE(NAME, TYPE, OPCODE, FC)				\
static __inline__ TYPE							\
NAME (TYPE *_PTR, TYPE _VALUE)						\
{									\
  unsigned __int128 _TMP;						\
  TYPE _RET;								\
  __asm__ volatile ("mr %L1,%3\n"					\
		    "\t" OPCODE " %1,%P0,%4\n"				\
		    "\tmr %2,%1\n"					\
		    : "+Q" (_PTR[0]), "=&r" (_TMP), "=r" (_RET)		\
		    : "r" (_VALUE), "n" (FC));				\
  return _RET;								\
}

/* Implementation of the LWAT/LDAT operations that take two input registers
   and modify one word or double-word of memory and return the value that was
   previously in the memory location.  The destination and two source
   registers are encoded with only one register number, so we need three
   consecutive GPR registers and there is no C/C++ type that will give
   us that, so we have to use register asm variables to achieve that.

   The LWAT/LDAT opcode requires the address to be a single register,
   and that points to a suitably aligned memory location. Load atomic
   instructions have side effects, so the asm is marked as volatile.  */

#define _AMO_LD_CMPSWP(NAME, TYPE, OPCODE, FC)				\
static __inline__ TYPE							\
NAME (TYPE *_ADDR, TYPE _COND, TYPE _VALUE)				\
{									\
  register TYPE _ret asm ("r8");					\
  register TYPE _cond asm ("r9") = _COND;				\
  register TYPE _value asm ("r10") = _VALUE;				\
  __asm__ volatile (OPCODE " %0,%P1,%4\n"				\
			: "=r" (_ret), "+Q" (*_ADDR)			\
			: "r" (_cond), "r" (_value), "n" (FC));		\
  return _ret;								\
}

#define _AMO_LD_INCREMENT(NAME, TYPE, OPCODE, FC)			\
static __inline__ TYPE							\
NAME (TYPE *_ADDR)							\
{									\
  TYPE _RET;								\
  __asm__ volatile (OPCODE " %0,%P1,%3\n"				\
			: "=r" (_RET), "+Q" (_ADDR[0])			\
			: "Q" (_ADDR[1]), "n" (FC));			\
  return _RET;								\
}

#define _AMO_LD_DECREMENT(NAME, TYPE, OPCODE, FC)			\
static __inline__ TYPE							\
NAME (TYPE *_ADDR)							\
{									\
  TYPE _RET;								\
  __asm__ volatile (OPCODE " %0,%P1,%3\n"				\
			: "=r" (_RET), "+Q" (_ADDR[1])			\
			: "Q" (_ADDR[0]), "n" (FC));			\
  return _RET;								\
}

_AMO_LD_SIMPLE (amo_lwat_add,   uint32_t, "lwat", _AMO_LD_ADD)
_AMO_LD_SIMPLE (amo_lwat_xor,   uint32_t, "lwat", _AMO_LD_XOR)
_AMO_LD_SIMPLE (amo_lwat_ior,   uint32_t, "lwat", _AMO_LD_IOR)
_AMO_LD_SIMPLE (amo_lwat_and,   uint32_t, "lwat", _AMO_LD_AND)
_AMO_LD_SIMPLE (amo_lwat_umax,  uint32_t, "lwat", _AMO_LD_UMAX)
_AMO_LD_SIMPLE (amo_lwat_umin,  uint32_t, "lwat", _AMO_LD_UMIN)
_AMO_LD_SIMPLE (amo_lwat_swap,  uint32_t, "lwat", _AMO_LD_SWAP)
_AMO_LD_CMPSWP    (amo_lwat_cas_neq,     uint32_t, "lwat", _AMO_LD_CS_NE)
_AMO_LD_INCREMENT (amo_lwat_inc_eq,      uint32_t, "lwat", _AMO_LD_INC_EQUAL)
_AMO_LD_INCREMENT (amo_lwat_inc_bounded, uint32_t, "lwat", _AMO_LD_INC_BOUNDED)
_AMO_LD_DECREMENT (amo_lwat_dec_bounded, uint32_t, "lwat", _AMO_LD_DEC_BOUNDED)

_AMO_LD_SIMPLE (amo_lwat_sadd,  int32_t,  "lwat", _AMO_LD_ADD)
_AMO_LD_SIMPLE (amo_lwat_smax,  int32_t,  "lwat", _AMO_LD_SMAX)
_AMO_LD_SIMPLE (amo_lwat_smin,  int32_t,  "lwat", _AMO_LD_SMIN)
_AMO_LD_SIMPLE (amo_lwat_sswap, int32_t,  "lwat", _AMO_LD_SWAP)
_AMO_LD_CMPSWP    (amo_lwat_scas_neq,     int32_t, "lwat", _AMO_LD_CS_NE)
_AMO_LD_INCREMENT (amo_lwat_sinc_eq,      int32_t, "lwat", _AMO_LD_INC_EQUAL)
_AMO_LD_INCREMENT (amo_lwat_sinc_bounded, int32_t, "lwat", _AMO_LD_INC_BOUNDED)
_AMO_LD_DECREMENT (amo_lwat_sdec_bounded, int32_t, "lwat", _AMO_LD_DEC_BOUNDED)

_AMO_LD_SIMPLE (amo_ldat_add,   uint64_t, "ldat", _AMO_LD_ADD)
_AMO_LD_SIMPLE (amo_ldat_xor,   uint64_t, "ldat", _AMO_LD_XOR)
_AMO_LD_SIMPLE (amo_ldat_ior,   uint64_t, "ldat", _AMO_LD_IOR)
_AMO_LD_SIMPLE (amo_ldat_and,   uint64_t, "ldat", _AMO_LD_AND)
_AMO_LD_SIMPLE (amo_ldat_umax,  uint64_t, "ldat", _AMO_LD_UMAX)
_AMO_LD_SIMPLE (amo_ldat_umin,  uint64_t, "ldat", _AMO_LD_UMIN)
_AMO_LD_SIMPLE (amo_ldat_swap,  uint64_t, "ldat", _AMO_LD_SWAP)
_AMO_LD_CMPSWP    (amo_ldat_cas_neq,     uint64_t, "ldat", _AMO_LD_CS_NE)
_AMO_LD_INCREMENT (amo_ldat_inc_eq,      uint64_t, "ldat", _AMO_LD_INC_EQUAL)
_AMO_LD_INCREMENT (amo_ldat_inc_bounded, uint64_t, "ldat", _AMO_LD_INC_BOUNDED)
_AMO_LD_DECREMENT (amo_ldat_dec_bounded, uint64_t, "ldat", _AMO_LD_DEC_BOUNDED)

_AMO_LD_SIMPLE (amo_ldat_sadd,  int64_t,  "ldat", _AMO_LD_ADD)
_AMO_LD_SIMPLE (amo_ldat_smax,  int64_t,  "ldat", _AMO_LD_SMAX)
_AMO_LD_SIMPLE (amo_ldat_smin,  int64_t,  "ldat", _AMO_LD_SMIN)
_AMO_LD_SIMPLE (amo_ldat_sswap, int64_t,  "ldat", _AMO_LD_SWAP)
_AMO_LD_CMPSWP    (amo_ldat_scas_neq,     int64_t, "ldat", _AMO_LD_CS_NE)
_AMO_LD_INCREMENT (amo_ldat_sinc_eq,      int64_t, "ldat", _AMO_LD_INC_EQUAL)
_AMO_LD_INCREMENT (amo_ldat_sinc_bounded, int64_t, "ldat", _AMO_LD_INC_BOUNDED)
_AMO_LD_DECREMENT (amo_ldat_sdec_bounded, int64_t, "ldat", _AMO_LD_DEC_BOUNDED)

/* Future specific compare-and-swap equal operations.  */
#ifdef _ARCH_FUTURE
_AMO_LD_CMPSWP (amo_lwat_cas_eq,  uint32_t, "lwat", _AMO_LD_CS_EQ)
_AMO_LD_CMPSWP (amo_lwat_scas_eq, int32_t,  "lwat", _AMO_LD_CS_EQ)
_AMO_LD_CMPSWP (amo_ldat_cas_eq,  uint64_t, "ldat", _AMO_LD_CS_EQ)
_AMO_LD_CMPSWP (amo_ldat_scas_eq, int64_t,  "ldat", _AMO_LD_CS_EQ)
#else /* ! _ARCH_FUTURE */
/* Dummy declarations with GCC error attribute: Triggers error on use.  */
#define _AMO_ERR_CMPSWP(NAME, TYPE)					 \
extern TYPE								 \
NAME (TYPE *_ADDR, TYPE _COND, TYPE _VALUE)				 \
  __attribute__ ((error (#NAME " requires ISA<future>; not available on" \
			       " ISA 3.1 or earlier")));
_AMO_ERR_CMPSWP (amo_lwat_cas_eq,  uint32_t)
_AMO_ERR_CMPSWP (amo_lwat_scas_eq, int32_t)
_AMO_ERR_CMPSWP (amo_ldat_cas_eq,  uint64_t)
_AMO_ERR_CMPSWP (amo_ldat_scas_eq, int64_t)
#undef _AMO_ERR_CMPSWP
#endif

/* Enumeration of the STWAT/STDAT sub-opcodes.  */
enum _AMO_ST {
  _AMO_ST_ADD		= 0x00,		/* Store Add.  */
  _AMO_ST_XOR		= 0x01,		/* Store Xor.  */
  _AMO_ST_IOR		= 0x02,		/* Store Ior.  */
  _AMO_ST_AND		= 0x03,		/* Store And.  */
  _AMO_ST_UMAX		= 0x04,		/* Store Unsigned Maximum.  */
  _AMO_ST_SMAX		= 0x05,		/* Store Signed Maximum.  */
  _AMO_ST_UMIN		= 0x06,		/* Store Unsigned Minimum.  */
  _AMO_ST_SMIN		= 0x07,		/* Store Signed Minimum.  */
  _AMO_ST_TWIN		= 0x18		/* Store Twin.  */
};

/* Implementation of the simple STWAT/STDAT operations that take one register
   and modify one word or double-word of memory.  No value is returned.

   The STWAT/STDAT opcode requires the address to be a single register, and
   that points to a suitably aligned memory location.  Asm volatile is used to
   prevent the optimizer from moving the operation.  */

#define _AMO_ST_SIMPLE(NAME, TYPE, OPCODE, FC)				\
static __inline__ void							\
NAME (TYPE *_PTR, TYPE _VALUE)						\
{									\
  __asm__ volatile (OPCODE " %1,%P0,%2"					\
		    : "+Q" (_PTR[0])					\
		    : "r" (_VALUE), "n" (FC));				\
  return;								\
}

/* Implementation of the STWAT/STDAT store twin operation that takes
   one register and modifies two words or double-words of memory.
   No value is returned.

   The STWAT/STDAT opcode requires the address to be a single register
   that points to a suitably aligned memory location. Store atomic
   instructions have side effects, so the asm is marked as volatile.  */

#define _AMO_ST_TWIN(NAME, TYPE, OPCODE, FC)				\
static __inline__ void							\
NAME (TYPE *_ADDR, TYPE _VALUE)						\
{									\
  __asm__ volatile (OPCODE " %2,%P0,%3"					\
		    : "+Q" (_ADDR[0]), "+Q" (_ADDR[1])			\
		    : "r" (_VALUE),  "n" (FC));				\
  return;								\
}

_AMO_ST_SIMPLE (amo_stwat_add,  uint32_t, "stwat", _AMO_ST_ADD)
_AMO_ST_SIMPLE (amo_stwat_xor,  uint32_t, "stwat", _AMO_ST_XOR)
_AMO_ST_SIMPLE (amo_stwat_ior,  uint32_t, "stwat", _AMO_ST_IOR)
_AMO_ST_SIMPLE (amo_stwat_and,  uint32_t, "stwat", _AMO_ST_AND)
_AMO_ST_SIMPLE (amo_stwat_umax, uint32_t, "stwat", _AMO_ST_UMAX)
_AMO_ST_SIMPLE (amo_stwat_umin, uint32_t, "stwat", _AMO_ST_UMIN)
_AMO_ST_TWIN (amo_stwat_twin, uint32_t, "stwat", _AMO_ST_TWIN)

_AMO_ST_SIMPLE (amo_stwat_sadd, int32_t,  "stwat", _AMO_ST_ADD)
_AMO_ST_SIMPLE (amo_stwat_smax, int32_t,  "stwat", _AMO_ST_SMAX)
_AMO_ST_SIMPLE (amo_stwat_smin, int32_t,  "stwat", _AMO_ST_SMIN)
_AMO_ST_TWIN (amo_stwat_stwin, int32_t, "stwat", _AMO_ST_TWIN)

_AMO_ST_SIMPLE (amo_stdat_add,  uint64_t, "stdat", _AMO_ST_ADD)
_AMO_ST_SIMPLE (amo_stdat_xor,  uint64_t, "stdat", _AMO_ST_XOR)
_AMO_ST_SIMPLE (amo_stdat_ior,  uint64_t, "stdat", _AMO_ST_IOR)
_AMO_ST_SIMPLE (amo_stdat_and,  uint64_t, "stdat", _AMO_ST_AND)
_AMO_ST_SIMPLE (amo_stdat_umax, uint64_t, "stdat", _AMO_ST_UMAX)
_AMO_ST_SIMPLE (amo_stdat_umin, uint64_t, "stdat", _AMO_ST_UMIN)
_AMO_ST_TWIN (amo_stdat_twin, uint64_t, "stdat", _AMO_ST_TWIN)

_AMO_ST_SIMPLE (amo_stdat_sadd, int64_t,  "stdat", _AMO_ST_ADD)
_AMO_ST_SIMPLE (amo_stdat_smax, int64_t,  "stdat", _AMO_ST_SMAX)
_AMO_ST_SIMPLE (amo_stdat_smin, int64_t,  "stdat", _AMO_ST_SMIN)
_AMO_ST_TWIN (amo_stdat_stwin, int64_t, "stdat", _AMO_ST_TWIN)
#endif	/* _ARCH_PWR9 && _ARCH_PPC64.  */
#endif	/* _POWERPC_AMO_H.  */
