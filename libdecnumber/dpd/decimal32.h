/* Decimal 32-bit format module header for the decNumber C Library.
   Copyright (C) 2005-2023 Free Software Foundation, Inc.
   Contributed by IBM Corporation.  Author Mike Cowlishaw.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 3, or (at your option) any later
   version.

   GCC is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

Under Section 7 of GPL version 3, you are granted additional
permissions described in the GCC Runtime Library Exception, version
3.1, as published by the Free Software Foundation.

You should have received a copy of the GNU General Public License and
a copy of the GCC Runtime Library Exception along with this program;
see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
<http://www.gnu.org/licenses/>.  */

/* ------------------------------------------------------------------ */
/* Decimal 32-bit format module header				      */
/* ------------------------------------------------------------------ */

#if !defined(DECIMAL32)
  #define DECIMAL32
  #define DEC32NAME	"decimal32"		      /* Short name   */
  #define DEC32FULLNAME "Decimal 32-bit Number"       /* Verbose name */
  #define DEC32AUTHOR	"Mike Cowlishaw"	      /* Who to blame */

  /* parameters for decimal32s */
  #define DECIMAL32_Bytes  4		/* length		      */
  #define DECIMAL32_Pmax   7		/* maximum precision (digits) */
  #define DECIMAL32_Emax   96		/* maximum adjusted exponent  */
  #define DECIMAL32_Emin  -95		/* minimum adjusted exponent  */
  #define DECIMAL32_Bias   101		/* bias for the exponent      */
  #define DECIMAL32_String 15		/* maximum string length, +1  */
  #define DECIMAL32_EconL  6		/* exp. continuation length   */
  /* highest biased exponent (Elimit-1) 			      */
  #define DECIMAL32_Ehigh  (DECIMAL32_Emax+DECIMAL32_Bias-DECIMAL32_Pmax+1)

  /* check enough digits, if pre-defined			      */
  #if defined(DECNUMDIGITS)
    #if (DECNUMDIGITS<DECIMAL32_Pmax)
      #error decimal32.h needs pre-defined DECNUMDIGITS>=7 for safe use
    #endif
  #endif

  #ifndef DECNUMDIGITS
    #define DECNUMDIGITS DECIMAL32_Pmax /* size if not already defined*/
  #endif
  #ifndef DECNUMBER
    #include "decNumber.h"		/* context and number library */
  #endif

  /* Decimal 32-bit type, accessible by bytes */
  typedef struct {
    uint8_t bytes[DECIMAL32_Bytes];	/* decimal32: 1, 5, 6, 20 bits*/
    } decimal32;

  /* special values [top byte excluding sign bit; last two bits are   */
  /* don't-care for Infinity on input, last bit don't-care for NaN]   */
  #if !defined(DECIMAL_NaN)
    #define DECIMAL_NaN     0x7c	/* 0 11111 00 NaN	      */
    #define DECIMAL_sNaN    0x7e	/* 0 11111 10 sNaN	      */
    #define DECIMAL_Inf     0x78	/* 0 11110 00 Infinity	      */
  #endif

  /* ---------------------------------------------------------------- */
  /* Routines							      */
  /* ---------------------------------------------------------------- */

#include "decimal32Symbols.h"

  #ifdef __cplusplus
  extern "C" {
  #endif

  /* String conversions 					      */
  decimal32 * decimal32FromString(decimal32 *, const char *, decContext *);
  char * decimal32ToString(const decimal32 *, char *);
  char * decimal32ToEngString(const decimal32 *, char *);

  /* decNumber conversions					      */
  decimal32 * decimal32FromNumber(decimal32 *, const decNumber *,
				  decContext *);
  decNumber * decimal32ToNumber(const decimal32 *, decNumber *);

  /* Format-dependent utilities 				      */
  uint32_t    decimal32IsCanonical(const decimal32 *);
  decimal32 * decimal32Canonical(decimal32 *, const decimal32 *);

  #ifdef __cplusplus
  }
  #endif

#endif
