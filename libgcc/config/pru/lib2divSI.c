/* SI mode divide routines for libgcc for PRU
   Copyright (C) 2014-2023 Free Software Foundation, Inc.
   Based on lib2divSI.c from MSP430 port.

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

typedef		  int sint32_type   __attribute__ ((mode (SI)));
typedef unsigned  int uint32_type   __attribute__ ((mode (SI)));
typedef		  int sint16_type   __attribute__ ((mode (HI)));
typedef unsigned  int uint16_type   __attribute__ ((mode (HI)));
typedef		  int sint08_type   __attribute__ ((mode (QI)));
typedef unsigned  int uint08_type   __attribute__ ((mode (QI)));
typedef		  int word_type     __attribute__ ((mode (__word__)));

#define C3B(a,b,c) a##b##c
#define C3(a,b,c) C3B(a,b,c)

/* PRU ABI dictates the library function names.  */
#define __gnu_divsi3 __pruabi_divi
#define __gnu_modsi3 __pruabi_remi
#define __gnu_udivsi3 __pruabi_divu
#define __gnu_umodsi3 __pruabi_remu

#define UINT_TYPE	uint32_type
#define SINT_TYPE	sint32_type
#define BITS_MINUS_1	31
#define NAME_MODE	si

#include "pru-divmod.h"
