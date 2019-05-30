/* Copyright (C) 1988-2019 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#ifndef GCC_I386_BUILTINS_H
#define GCC_I386_BUILTINS_H

/* The following file contains several enumerations and data structures
   built from the definitions in i386-builtin-types.def.  */

#include "i386-builtin-types.inc"

/* Codes for all the SSE/MMX builtins.  Builtins not mentioned in any
   bdesc_* arrays below should come first, then builtins for each bdesc_*
   array in ascending order, so that we can use direct array accesses.  */
enum ix86_builtins
{
  IX86_BUILTIN_MASKMOVQ,
  IX86_BUILTIN_LDMXCSR,
  IX86_BUILTIN_STMXCSR,
  IX86_BUILTIN_MASKMOVDQU,
  IX86_BUILTIN_PSLLDQ128,
  IX86_BUILTIN_CLFLUSH,
  IX86_BUILTIN_MONITOR,
  IX86_BUILTIN_MWAIT,
  IX86_BUILTIN_UMONITOR,
  IX86_BUILTIN_UMWAIT,
  IX86_BUILTIN_TPAUSE,
  IX86_BUILTIN_CLZERO,
  IX86_BUILTIN_CLDEMOTE,
  IX86_BUILTIN_VEC_INIT_V2SI,
  IX86_BUILTIN_VEC_INIT_V4HI,
  IX86_BUILTIN_VEC_INIT_V8QI,
  IX86_BUILTIN_VEC_EXT_V2DF,
  IX86_BUILTIN_VEC_EXT_V2DI,
  IX86_BUILTIN_VEC_EXT_V4SF,
  IX86_BUILTIN_VEC_EXT_V4SI,
  IX86_BUILTIN_VEC_EXT_V8HI,
  IX86_BUILTIN_VEC_EXT_V2SI,
  IX86_BUILTIN_VEC_EXT_V4HI,
  IX86_BUILTIN_VEC_EXT_V16QI,
  IX86_BUILTIN_VEC_SET_V2DI,
  IX86_BUILTIN_VEC_SET_V4SF,
  IX86_BUILTIN_VEC_SET_V4SI,
  IX86_BUILTIN_VEC_SET_V8HI,
  IX86_BUILTIN_VEC_SET_V4HI,
  IX86_BUILTIN_VEC_SET_V16QI,
  IX86_BUILTIN_GATHERSIV2DF,
  IX86_BUILTIN_GATHERSIV4DF,
  IX86_BUILTIN_GATHERDIV2DF,
  IX86_BUILTIN_GATHERDIV4DF,
  IX86_BUILTIN_GATHERSIV4SF,
  IX86_BUILTIN_GATHERSIV8SF,
  IX86_BUILTIN_GATHERDIV4SF,
  IX86_BUILTIN_GATHERDIV8SF,
  IX86_BUILTIN_GATHERSIV2DI,
  IX86_BUILTIN_GATHERSIV4DI,
  IX86_BUILTIN_GATHERDIV2DI,
  IX86_BUILTIN_GATHERDIV4DI,
  IX86_BUILTIN_GATHERSIV4SI,
  IX86_BUILTIN_GATHERSIV8SI,
  IX86_BUILTIN_GATHERDIV4SI,
  IX86_BUILTIN_GATHERDIV8SI,
  IX86_BUILTIN_GATHER3SIV8SF,
  IX86_BUILTIN_GATHER3SIV4SF,
  IX86_BUILTIN_GATHER3SIV4DF,
  IX86_BUILTIN_GATHER3SIV2DF,
  IX86_BUILTIN_GATHER3DIV8SF,
  IX86_BUILTIN_GATHER3DIV4SF,
  IX86_BUILTIN_GATHER3DIV4DF,
  IX86_BUILTIN_GATHER3DIV2DF,
  IX86_BUILTIN_GATHER3SIV8SI,
  IX86_BUILTIN_GATHER3SIV4SI,
  IX86_BUILTIN_GATHER3SIV4DI,
  IX86_BUILTIN_GATHER3SIV2DI,
  IX86_BUILTIN_GATHER3DIV8SI,
  IX86_BUILTIN_GATHER3DIV4SI,
  IX86_BUILTIN_GATHER3DIV4DI,
  IX86_BUILTIN_GATHER3DIV2DI,
  IX86_BUILTIN_SCATTERSIV8SF,
  IX86_BUILTIN_SCATTERSIV4SF,
  IX86_BUILTIN_SCATTERSIV4DF,
  IX86_BUILTIN_SCATTERSIV2DF,
  IX86_BUILTIN_SCATTERDIV8SF,
  IX86_BUILTIN_SCATTERDIV4SF,
  IX86_BUILTIN_SCATTERDIV4DF,
  IX86_BUILTIN_SCATTERDIV2DF,
  IX86_BUILTIN_SCATTERSIV8SI,
  IX86_BUILTIN_SCATTERSIV4SI,
  IX86_BUILTIN_SCATTERSIV4DI,
  IX86_BUILTIN_SCATTERSIV2DI,
  IX86_BUILTIN_SCATTERDIV8SI,
  IX86_BUILTIN_SCATTERDIV4SI,
  IX86_BUILTIN_SCATTERDIV4DI,
  IX86_BUILTIN_SCATTERDIV2DI,
  /* Alternate 4 and 8 element gather/scatter for the vectorizer
     where all operands are 32-byte or 64-byte wide respectively.  */
  IX86_BUILTIN_GATHERALTSIV4DF,
  IX86_BUILTIN_GATHERALTDIV8SF,
  IX86_BUILTIN_GATHERALTSIV4DI,
  IX86_BUILTIN_GATHERALTDIV8SI,
  IX86_BUILTIN_GATHER3ALTDIV16SF,
  IX86_BUILTIN_GATHER3ALTDIV16SI,
  IX86_BUILTIN_GATHER3ALTSIV4DF,
  IX86_BUILTIN_GATHER3ALTDIV8SF,
  IX86_BUILTIN_GATHER3ALTSIV4DI,
  IX86_BUILTIN_GATHER3ALTDIV8SI,
  IX86_BUILTIN_GATHER3ALTSIV8DF,
  IX86_BUILTIN_GATHER3ALTSIV8DI,
  IX86_BUILTIN_GATHER3DIV16SF,
  IX86_BUILTIN_GATHER3DIV16SI,
  IX86_BUILTIN_GATHER3DIV8DF,
  IX86_BUILTIN_GATHER3DIV8DI,
  IX86_BUILTIN_GATHER3SIV16SF,
  IX86_BUILTIN_GATHER3SIV16SI,
  IX86_BUILTIN_GATHER3SIV8DF,
  IX86_BUILTIN_GATHER3SIV8DI,
  IX86_BUILTIN_SCATTERALTSIV8DF,
  IX86_BUILTIN_SCATTERALTDIV16SF,
  IX86_BUILTIN_SCATTERALTSIV8DI,
  IX86_BUILTIN_SCATTERALTDIV16SI,
  IX86_BUILTIN_SCATTERALTSIV4DF,
  IX86_BUILTIN_SCATTERALTDIV8SF,
  IX86_BUILTIN_SCATTERALTSIV4DI,
  IX86_BUILTIN_SCATTERALTDIV8SI,
  IX86_BUILTIN_SCATTERALTSIV2DF,
  IX86_BUILTIN_SCATTERALTDIV4SF,
  IX86_BUILTIN_SCATTERALTSIV2DI,
  IX86_BUILTIN_SCATTERALTDIV4SI,
  IX86_BUILTIN_SCATTERDIV16SF,
  IX86_BUILTIN_SCATTERDIV16SI,
  IX86_BUILTIN_SCATTERDIV8DF,
  IX86_BUILTIN_SCATTERDIV8DI,
  IX86_BUILTIN_SCATTERSIV16SF,
  IX86_BUILTIN_SCATTERSIV16SI,
  IX86_BUILTIN_SCATTERSIV8DF,
  IX86_BUILTIN_SCATTERSIV8DI,
  IX86_BUILTIN_GATHERPFQPD,
  IX86_BUILTIN_GATHERPFDPS,
  IX86_BUILTIN_GATHERPFDPD,
  IX86_BUILTIN_GATHERPFQPS,
  IX86_BUILTIN_SCATTERPFDPD,
  IX86_BUILTIN_SCATTERPFDPS,
  IX86_BUILTIN_SCATTERPFQPD,
  IX86_BUILTIN_SCATTERPFQPS,
  IX86_BUILTIN_CLWB,
  IX86_BUILTIN_CLFLUSHOPT,
  IX86_BUILTIN_INFQ,
  IX86_BUILTIN_HUGE_VALQ,
  IX86_BUILTIN_NANQ,
  IX86_BUILTIN_NANSQ,
  IX86_BUILTIN_XABORT,
  IX86_BUILTIN_ADDCARRYX32,
  IX86_BUILTIN_ADDCARRYX64,
  IX86_BUILTIN_SBB32,
  IX86_BUILTIN_SBB64,
  IX86_BUILTIN_RDRAND16_STEP,
  IX86_BUILTIN_RDRAND32_STEP,
  IX86_BUILTIN_RDRAND64_STEP,
  IX86_BUILTIN_RDSEED16_STEP,
  IX86_BUILTIN_RDSEED32_STEP,
  IX86_BUILTIN_RDSEED64_STEP,
  IX86_BUILTIN_MONITORX,
  IX86_BUILTIN_MWAITX,
  IX86_BUILTIN_CFSTRING,
  IX86_BUILTIN_CPU_INIT,
  IX86_BUILTIN_CPU_IS,
  IX86_BUILTIN_CPU_SUPPORTS,
  IX86_BUILTIN_READ_FLAGS,
  IX86_BUILTIN_WRITE_FLAGS,

  /* All the remaining builtins are tracked in bdesc_* arrays in
     i386-builtin.def.  Don't add any IX86_BUILTIN_* enumerators after
     this point.  */
#define BDESC(mask, mask2, icode, name, code, comparison, flag)	\
  code,
#define BDESC_FIRST(kind, kindu, mask, mask2, icode, name, code, comparison, flag) \
  code,									\
  IX86_BUILTIN__BDESC_##kindu##_FIRST = code,
#define BDESC_END(kind, next_kind)

#include "i386-builtin.def"

#undef BDESC
#undef BDESC_FIRST
#undef BDESC_END

  IX86_BUILTIN_MAX,

  IX86_BUILTIN__BDESC_MAX_FIRST = IX86_BUILTIN_MAX,

  /* Now just the aliases for bdesc_* start/end.  */
#define BDESC(mask, mask2, icode, name, code, comparison, flag)
#define BDESC_FIRST(kind, kindu, mask, mask2, icode, name, code, comparison, flag)
#define BDESC_END(kind, next_kind) \
  IX86_BUILTIN__BDESC_##kind##_LAST					    \
    = IX86_BUILTIN__BDESC_##next_kind##_FIRST - 1,

#include "i386-builtin.def"

#undef BDESC
#undef BDESC_FIRST
#undef BDESC_END

  /* Just to make sure there is no comma after the last enumerator.  */
  IX86_BUILTIN__BDESC_MAX_LAST = IX86_BUILTIN__BDESC_MAX_FIRST
};

/* Table of all of the builtin functions that are possible with different ISA's
   but are waiting to be built until a function is declared to use that
   ISA.  */
struct builtin_isa {
  HOST_WIDE_INT isa;		/* isa_flags this builtin is defined for */
  HOST_WIDE_INT isa2;		/* additional isa_flags this builtin is defined for */
  const char *name;		/* function name */
  enum ix86_builtin_func_type tcode; /* type to use in the declaration */
  unsigned char const_p:1;	/* true if the declaration is constant */
  unsigned char pure_p:1;	/* true if the declaration has pure attribute */
  bool set_and_not_built_p;
};

/* Bits for builtin_description.flag.  */

/* Set when we don't support the comparison natively, and should
   swap_comparison in order to support it.  */
#define BUILTIN_DESC_SWAP_OPERANDS	1

struct builtin_description
{
  const HOST_WIDE_INT mask;
  const HOST_WIDE_INT mask2;
  const enum insn_code icode;
  const char *const name;
  const enum ix86_builtins code;
  const enum rtx_code comparison;
  const int flag;
};

#define MULTI_ARG_4_DF2_DI_I	V2DF_FTYPE_V2DF_V2DF_V2DI_INT
#define MULTI_ARG_4_DF2_DI_I1	V4DF_FTYPE_V4DF_V4DF_V4DI_INT
#define MULTI_ARG_4_SF2_SI_I	V4SF_FTYPE_V4SF_V4SF_V4SI_INT
#define MULTI_ARG_4_SF2_SI_I1	V8SF_FTYPE_V8SF_V8SF_V8SI_INT
#define MULTI_ARG_3_SF		V4SF_FTYPE_V4SF_V4SF_V4SF
#define MULTI_ARG_3_DF		V2DF_FTYPE_V2DF_V2DF_V2DF
#define MULTI_ARG_3_SF2		V8SF_FTYPE_V8SF_V8SF_V8SF
#define MULTI_ARG_3_DF2		V4DF_FTYPE_V4DF_V4DF_V4DF
#define MULTI_ARG_3_DI		V2DI_FTYPE_V2DI_V2DI_V2DI
#define MULTI_ARG_3_SI		V4SI_FTYPE_V4SI_V4SI_V4SI
#define MULTI_ARG_3_SI_DI	V4SI_FTYPE_V4SI_V4SI_V2DI
#define MULTI_ARG_3_HI		V8HI_FTYPE_V8HI_V8HI_V8HI
#define MULTI_ARG_3_HI_SI	V8HI_FTYPE_V8HI_V8HI_V4SI
#define MULTI_ARG_3_QI		V16QI_FTYPE_V16QI_V16QI_V16QI
#define MULTI_ARG_3_DI2		V4DI_FTYPE_V4DI_V4DI_V4DI
#define MULTI_ARG_3_SI2		V8SI_FTYPE_V8SI_V8SI_V8SI
#define MULTI_ARG_3_HI2		V16HI_FTYPE_V16HI_V16HI_V16HI
#define MULTI_ARG_3_QI2		V32QI_FTYPE_V32QI_V32QI_V32QI
#define MULTI_ARG_2_SF		V4SF_FTYPE_V4SF_V4SF
#define MULTI_ARG_2_DF		V2DF_FTYPE_V2DF_V2DF
#define MULTI_ARG_2_DI		V2DI_FTYPE_V2DI_V2DI
#define MULTI_ARG_2_SI		V4SI_FTYPE_V4SI_V4SI
#define MULTI_ARG_2_HI		V8HI_FTYPE_V8HI_V8HI
#define MULTI_ARG_2_QI		V16QI_FTYPE_V16QI_V16QI
#define MULTI_ARG_2_DI_IMM	V2DI_FTYPE_V2DI_SI
#define MULTI_ARG_2_SI_IMM	V4SI_FTYPE_V4SI_SI
#define MULTI_ARG_2_HI_IMM	V8HI_FTYPE_V8HI_SI
#define MULTI_ARG_2_QI_IMM	V16QI_FTYPE_V16QI_SI
#define MULTI_ARG_2_DI_CMP	V2DI_FTYPE_V2DI_V2DI_CMP
#define MULTI_ARG_2_SI_CMP	V4SI_FTYPE_V4SI_V4SI_CMP
#define MULTI_ARG_2_HI_CMP	V8HI_FTYPE_V8HI_V8HI_CMP
#define MULTI_ARG_2_QI_CMP	V16QI_FTYPE_V16QI_V16QI_CMP
#define MULTI_ARG_2_SF_TF	V4SF_FTYPE_V4SF_V4SF_TF
#define MULTI_ARG_2_DF_TF	V2DF_FTYPE_V2DF_V2DF_TF
#define MULTI_ARG_2_DI_TF	V2DI_FTYPE_V2DI_V2DI_TF
#define MULTI_ARG_2_SI_TF	V4SI_FTYPE_V4SI_V4SI_TF
#define MULTI_ARG_2_HI_TF	V8HI_FTYPE_V8HI_V8HI_TF
#define MULTI_ARG_2_QI_TF	V16QI_FTYPE_V16QI_V16QI_TF
#define MULTI_ARG_1_SF		V4SF_FTYPE_V4SF
#define MULTI_ARG_1_DF		V2DF_FTYPE_V2DF
#define MULTI_ARG_1_SF2		V8SF_FTYPE_V8SF
#define MULTI_ARG_1_DF2		V4DF_FTYPE_V4DF
#define MULTI_ARG_1_DI		V2DI_FTYPE_V2DI
#define MULTI_ARG_1_SI		V4SI_FTYPE_V4SI
#define MULTI_ARG_1_HI		V8HI_FTYPE_V8HI
#define MULTI_ARG_1_QI		V16QI_FTYPE_V16QI
#define MULTI_ARG_1_SI_DI	V2DI_FTYPE_V4SI
#define MULTI_ARG_1_HI_DI	V2DI_FTYPE_V8HI
#define MULTI_ARG_1_HI_SI	V4SI_FTYPE_V8HI
#define MULTI_ARG_1_QI_DI	V2DI_FTYPE_V16QI
#define MULTI_ARG_1_QI_SI	V4SI_FTYPE_V16QI
#define MULTI_ARG_1_QI_HI	V8HI_FTYPE_V16QI

#define BDESC(mask, mask2, icode, name, code, comparison, flag)	\
  { mask, mask2, icode, name, code, comparison, flag },
#define BDESC_FIRST(kind, kindu, mask, mask2, icode, name, code, comparison, flag) \
static const struct builtin_description bdesc_##kind[] =		    \
{									    \
  BDESC (mask, mask2, icode, name, code, comparison, flag)
#define BDESC_END(kind, next_kind) \
};

#include "i386-builtin.def"

extern builtin_isa ix86_builtins_isa[(int) IX86_BUILTIN_MAX];

tree ix86_builtin_vectorized_function (unsigned int fn, tree type_out,
					      tree type_in);
void ix86_init_builtins (void);
tree ix86_vectorize_builtin_gather (const_tree mem_vectype,
					   const_tree index_type, int scale);
tree ix86_builtin_decl (unsigned code, bool);
tree ix86_builtin_reciprocal (tree fndecl);
unsigned int get_builtin_code_for_version (tree decl, tree *predicate_list);
tree fold_builtin_cpu (tree fndecl, tree *args);
tree get_ix86_builtin (enum ix86_builtins c);

#endif  /* GCC_I386_BUILTINS_H */
