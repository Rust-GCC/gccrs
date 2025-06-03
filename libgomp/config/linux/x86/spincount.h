/* Copyright (C) 2023-2025 Free Software Foundation, Inc.
   Contributed by Jun Zhang <jun.zhang@intel.com>.

   This file is part of the GNU Offloading and Multi Processing Library
   (libgomp).

   Libgomp is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   Libgomp is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
   more details.

   Under Section 7 of GPL version 3, you are granted additional
   permissions described in the GCC Runtime Library Exception, version
   3.1, as published by the Free Software Foundation.

   You should have received a copy of the GNU General Public License and
   a copy of the GCC Runtime Library Exception along with this program;
   see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
   <http://www.gnu.org/licenses/>.  */

#ifndef GOMP_SPINCOUNT_H
#define GOMP_SPINCOUNT_H 1

#ifdef __x86_64__
#include "cpuid.h"
#include "libgomp.h"

/* Only for x86 hybrid platform  */
static inline void do_adjust_default_spincount (void)
{
  unsigned int eax, ebx, ecx, edx;
  if (__get_cpuid_count (7, 0, &eax, &ebx, &ecx, &edx)
      /* CPUID.07H.0H:EDX[15] Hybrid.  */
      && ((edx >> 15) & 1))
    gomp_spin_count_var = 1LL;
}
#else

static inline void do_adjust_default_spincount (void)
{
}
#endif

#endif /* GOMP_SPINCOUNT_H */
