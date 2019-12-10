/* Subroutines used to generate function calls and handle built-in
   instructions on IBM RS/6000.
   Copyright (C) 1991-2019 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 3, or (at your
   option) any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

#define IN_TARGET_CODE 1

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "rtl.h"
#include "tree.h"
#include "memmodel.h"
#include "gimple.h"
#include "cfghooks.h"
#include "cfgloop.h"
#include "df.h"
#include "tm_p.h"
#include "stringpool.h"
#include "expmed.h"
#include "optabs.h"
#include "regs.h"
#include "ira.h"
#include "recog.h"
#include "cgraph.h"
#include "diagnostic-core.h"
#include "insn-attr.h"
#include "flags.h"
#include "alias.h"
#include "fold-const.h"
#include "attribs.h"
#include "stor-layout.h"
#include "calls.h"
#include "print-tree.h"
#include "varasm.h"
#include "explow.h"
#include "expr.h"
#include "output.h"
#include "common/common-target.h"
#include "langhooks.h"
#include "gimplify.h"
#include "gimple-fold.h"
#include "gimple-iterator.h"
#include "gimple-ssa.h"
#include "builtins.h"
#include "tree-vector-builder.h"
#if TARGET_XCOFF
#include "xcoffout.h"  /* get declarations of xcoff_*_section_name */
#endif
#include "ppc-auxv.h"
#include "tree-ssa-propagate.h"
#include "tree-vrp.h"
#include "tree-ssanames.h"
#include "targhooks.h"

#include "rs6000-internal.h"

#if TARGET_MACHO
#include "gstab.h"  /* for N_SLINE */
#include "dbxout.h" /* dbxout_ */
#endif

#ifndef TARGET_PROFILE_KERNEL
#define TARGET_PROFILE_KERNEL 0
#endif

#ifdef HAVE_AS_GNU_ATTRIBUTE
# ifndef HAVE_LD_PPC_GNU_ATTR_LONG_DOUBLE
# define HAVE_LD_PPC_GNU_ATTR_LONG_DOUBLE 0
# endif
#endif

#ifndef TARGET_NO_PROTOTYPE
#define TARGET_NO_PROTOTYPE 0
#endif

struct builtin_description
{
  const HOST_WIDE_INT mask;
  const enum insn_code icode;
  const char *const name;
  const enum rs6000_builtins code;
};

/* Used by __builtin_cpu_is(), mapping from PLATFORM names to values.  */
static const struct
{
  const char *cpu;
  unsigned int cpuid;
} cpu_is_info[] = {
  { "power9",	   PPC_PLATFORM_POWER9 },
  { "power8",	   PPC_PLATFORM_POWER8 },
  { "power7",	   PPC_PLATFORM_POWER7 },
  { "power6x",	   PPC_PLATFORM_POWER6X },
  { "power6",	   PPC_PLATFORM_POWER6 },
  { "power5+",	   PPC_PLATFORM_POWER5_PLUS },
  { "power5",	   PPC_PLATFORM_POWER5 },
  { "ppc970",	   PPC_PLATFORM_PPC970 },
  { "power4",	   PPC_PLATFORM_POWER4 },
  { "ppca2",	   PPC_PLATFORM_PPCA2 },
  { "ppc476",	   PPC_PLATFORM_PPC476 },
  { "ppc464",	   PPC_PLATFORM_PPC464 },
  { "ppc440",	   PPC_PLATFORM_PPC440 },
  { "ppc405",	   PPC_PLATFORM_PPC405 },
  { "ppc-cell-be", PPC_PLATFORM_CELL_BE }
};

/* Used by __builtin_cpu_supports(), mapping from HWCAP names to masks.  */
static const struct
{
  const char *hwcap;
  int mask;
  unsigned int id;
} cpu_supports_info[] = {
  /* AT_HWCAP masks.  */
  { "4xxmac",		PPC_FEATURE_HAS_4xxMAC,		0 },
  { "altivec",		PPC_FEATURE_HAS_ALTIVEC,	0 },
  { "arch_2_05",	PPC_FEATURE_ARCH_2_05,		0 },
  { "arch_2_06",	PPC_FEATURE_ARCH_2_06,		0 },
  { "archpmu",		PPC_FEATURE_PERFMON_COMPAT,	0 },
  { "booke",		PPC_FEATURE_BOOKE,		0 },
  { "cellbe",		PPC_FEATURE_CELL_BE,		0 },
  { "dfp",		PPC_FEATURE_HAS_DFP,		0 },
  { "efpdouble",	PPC_FEATURE_HAS_EFP_DOUBLE,	0 },
  { "efpsingle",	PPC_FEATURE_HAS_EFP_SINGLE,	0 },
  { "fpu",		PPC_FEATURE_HAS_FPU,		0 },
  { "ic_snoop",		PPC_FEATURE_ICACHE_SNOOP,	0 },
  { "mmu",		PPC_FEATURE_HAS_MMU,		0 },
  { "notb",		PPC_FEATURE_NO_TB,		0 },
  { "pa6t",		PPC_FEATURE_PA6T,		0 },
  { "power4",		PPC_FEATURE_POWER4,		0 },
  { "power5",		PPC_FEATURE_POWER5,		0 },
  { "power5+",		PPC_FEATURE_POWER5_PLUS,	0 },
  { "power6x",		PPC_FEATURE_POWER6_EXT,		0 },
  { "ppc32",		PPC_FEATURE_32,			0 },
  { "ppc601",		PPC_FEATURE_601_INSTR,		0 },
  { "ppc64",		PPC_FEATURE_64,			0 },
  { "ppcle",		PPC_FEATURE_PPC_LE,		0 },
  { "smt",		PPC_FEATURE_SMT,		0 },
  { "spe",		PPC_FEATURE_HAS_SPE,		0 },
  { "true_le",		PPC_FEATURE_TRUE_LE,		0 },
  { "ucache",		PPC_FEATURE_UNIFIED_CACHE,	0 },
  { "vsx",		PPC_FEATURE_HAS_VSX,		0 },

  /* AT_HWCAP2 masks.  */
  { "arch_2_07",	PPC_FEATURE2_ARCH_2_07,		1 },
  { "dscr",		PPC_FEATURE2_HAS_DSCR,		1 },
  { "ebb",		PPC_FEATURE2_HAS_EBB,		1 },
  { "htm",		PPC_FEATURE2_HAS_HTM,		1 },
  { "htm-nosc",		PPC_FEATURE2_HTM_NOSC,		1 },
  { "htm-no-suspend",	PPC_FEATURE2_HTM_NO_SUSPEND,	1 },
  { "isel",		PPC_FEATURE2_HAS_ISEL,		1 },
  { "tar",		PPC_FEATURE2_HAS_TAR,		1 },
  { "vcrypto",		PPC_FEATURE2_HAS_VEC_CRYPTO,	1 },
  { "arch_3_00",	PPC_FEATURE2_ARCH_3_00,		1 },
  { "ieee128",		PPC_FEATURE2_HAS_IEEE128,	1 },
  { "darn",		PPC_FEATURE2_DARN,		1 },
  { "scv",		PPC_FEATURE2_SCV,		1 }
};

static void altivec_init_builtins (void);
static tree builtin_function_type (machine_mode, machine_mode,
				   machine_mode, machine_mode,
				   enum rs6000_builtins, const char *name);
static void rs6000_common_init_builtins (void);
static void htm_init_builtins (void);


/* Hash table to keep track of the argument types for builtin functions.  */

struct GTY((for_user)) builtin_hash_struct
{
  tree type;
  machine_mode mode[4];	/* return value + 3 arguments.  */
  unsigned char uns_p[4];	/* and whether the types are unsigned.  */
};

struct builtin_hasher : ggc_ptr_hash<builtin_hash_struct>
{
  static hashval_t hash (builtin_hash_struct *);
  static bool equal (builtin_hash_struct *, builtin_hash_struct *);
};

static GTY (()) hash_table<builtin_hasher> *builtin_hash_table;

/* Hash function for builtin functions with up to 3 arguments and a return
   type.  */
hashval_t
builtin_hasher::hash (builtin_hash_struct *bh)
{
  unsigned ret = 0;
  int i;

  for (i = 0; i < 4; i++)
    {
      ret = (ret * (unsigned)MAX_MACHINE_MODE) + ((unsigned)bh->mode[i]);
      ret = (ret * 2) + bh->uns_p[i];
    }

  return ret;
}

/* Compare builtin hash entries H1 and H2 for equivalence.  */
bool
builtin_hasher::equal (builtin_hash_struct *p1, builtin_hash_struct *p2)
{
  return ((p1->mode[0] == p2->mode[0])
	  && (p1->mode[1] == p2->mode[1])
	  && (p1->mode[2] == p2->mode[2])
	  && (p1->mode[3] == p2->mode[3])
	  && (p1->uns_p[0] == p2->uns_p[0])
	  && (p1->uns_p[1] == p2->uns_p[1])
	  && (p1->uns_p[2] == p2->uns_p[2])
	  && (p1->uns_p[3] == p2->uns_p[3]));
}


/* Table that classifies rs6000 builtin functions (pure, const, etc.).  */
#undef RS6000_BUILTIN_0
#undef RS6000_BUILTIN_1
#undef RS6000_BUILTIN_2
#undef RS6000_BUILTIN_3
#undef RS6000_BUILTIN_A
#undef RS6000_BUILTIN_D
#undef RS6000_BUILTIN_H
#undef RS6000_BUILTIN_P
#undef RS6000_BUILTIN_X

#define RS6000_BUILTIN_0(ENUM, NAME, MASK, ATTR, ICODE) \
  { NAME, ICODE, MASK, ATTR },

#define RS6000_BUILTIN_1(ENUM, NAME, MASK, ATTR, ICODE) \
  { NAME, ICODE, MASK, ATTR },

#define RS6000_BUILTIN_2(ENUM, NAME, MASK, ATTR, ICODE)  \
  { NAME, ICODE, MASK, ATTR },

#define RS6000_BUILTIN_3(ENUM, NAME, MASK, ATTR, ICODE)  \
  { NAME, ICODE, MASK, ATTR },

#define RS6000_BUILTIN_A(ENUM, NAME, MASK, ATTR, ICODE)  \
  { NAME, ICODE, MASK, ATTR },

#define RS6000_BUILTIN_D(ENUM, NAME, MASK, ATTR, ICODE)  \
  { NAME, ICODE, MASK, ATTR },

#define RS6000_BUILTIN_H(ENUM, NAME, MASK, ATTR, ICODE)  \
  { NAME, ICODE, MASK, ATTR },

#define RS6000_BUILTIN_P(ENUM, NAME, MASK, ATTR, ICODE)  \
  { NAME, ICODE, MASK, ATTR },

#define RS6000_BUILTIN_X(ENUM, NAME, MASK, ATTR, ICODE)  \
  { NAME, ICODE, MASK, ATTR },

struct rs6000_builtin_info_type {
  const char *name;
  const enum insn_code icode;
  const HOST_WIDE_INT mask;
  const unsigned attr;
};

const struct rs6000_builtin_info_type rs6000_builtin_info[] =
{
#include "rs6000-builtin.def"
};

#undef RS6000_BUILTIN_0
#undef RS6000_BUILTIN_1
#undef RS6000_BUILTIN_2
#undef RS6000_BUILTIN_3
#undef RS6000_BUILTIN_A
#undef RS6000_BUILTIN_D
#undef RS6000_BUILTIN_H
#undef RS6000_BUILTIN_P
#undef RS6000_BUILTIN_X


/* Nonzero if we can use a floating-point register to pass this arg.  */
#define USE_FP_FOR_ARG_P(CUM,MODE)		\
  (SCALAR_FLOAT_MODE_NOT_VECTOR_P (MODE)		\
   && (CUM)->fregno <= FP_ARG_MAX_REG		\
   && TARGET_HARD_FLOAT)

/* Nonzero if we can use an AltiVec register to pass this arg.  */
#define USE_ALTIVEC_FOR_ARG_P(CUM,MODE,NAMED)			\
  (ALTIVEC_OR_VSX_VECTOR_MODE (MODE)				\
   && (CUM)->vregno <= ALTIVEC_ARG_MAX_REG			\
   && TARGET_ALTIVEC_ABI					\
   && (NAMED))

/* Walk down the type tree of TYPE counting consecutive base elements.
   If *MODEP is VOIDmode, then set it to the first valid floating point
   or vector type.  If a non-floating point or vector type is found, or
   if a floating point or vector type that doesn't match a non-VOIDmode
   *MODEP is found, then return -1, otherwise return the count in the
   sub-tree.  */

static int
rs6000_aggregate_candidate (const_tree type, machine_mode *modep)
{
  machine_mode mode;
  HOST_WIDE_INT size;

  switch (TREE_CODE (type))
    {
    case REAL_TYPE:
      mode = TYPE_MODE (type);
      if (!SCALAR_FLOAT_MODE_P (mode))
	return -1;

      if (*modep == VOIDmode)
	*modep = mode;

      if (*modep == mode)
	return 1;

      break;

    case COMPLEX_TYPE:
      mode = TYPE_MODE (TREE_TYPE (type));
      if (!SCALAR_FLOAT_MODE_P (mode))
	return -1;

      if (*modep == VOIDmode)
	*modep = mode;

      if (*modep == mode)
	return 2;

      break;

    case VECTOR_TYPE:
      if (!TARGET_ALTIVEC_ABI || !TARGET_ALTIVEC)
	return -1;

      /* Use V4SImode as representative of all 128-bit vector types.  */
      size = int_size_in_bytes (type);
      switch (size)
	{
	case 16:
	  mode = V4SImode;
	  break;
	default:
	  return -1;
	}

      if (*modep == VOIDmode)
	*modep = mode;

      /* Vector modes are considered to be opaque: two vectors are
	 equivalent for the purposes of being homogeneous aggregates
	 if they are the same size.  */
      if (*modep == mode)
	return 1;

      break;

    case ARRAY_TYPE:
      {
	int count;
	tree index = TYPE_DOMAIN (type);

	/* Can't handle incomplete types nor sizes that are not
	   fixed.  */
	if (!COMPLETE_TYPE_P (type)
	    || TREE_CODE (TYPE_SIZE (type)) != INTEGER_CST)
	  return -1;

	count = rs6000_aggregate_candidate (TREE_TYPE (type), modep);
	if (count == -1
	    || !index
	    || !TYPE_MAX_VALUE (index)
	    || !tree_fits_uhwi_p (TYPE_MAX_VALUE (index))
	    || !TYPE_MIN_VALUE (index)
	    || !tree_fits_uhwi_p (TYPE_MIN_VALUE (index))
	    || count < 0)
	  return -1;

	count *= (1 + tree_to_uhwi (TYPE_MAX_VALUE (index))
		      - tree_to_uhwi (TYPE_MIN_VALUE (index)));

	/* There must be no padding.  */
	if (wi::to_wide (TYPE_SIZE (type))
	    != count * GET_MODE_BITSIZE (*modep))
	  return -1;

	return count;
      }

    case RECORD_TYPE:
      {
	int count = 0;
	int sub_count;
	tree field;

	/* Can't handle incomplete types nor sizes that are not
	   fixed.  */
	if (!COMPLETE_TYPE_P (type)
	    || TREE_CODE (TYPE_SIZE (type)) != INTEGER_CST)
	  return -1;

	for (field = TYPE_FIELDS (type); field; field = TREE_CHAIN (field))
	  {
	    if (TREE_CODE (field) != FIELD_DECL)
	      continue;

	    sub_count = rs6000_aggregate_candidate (TREE_TYPE (field), modep);
	    if (sub_count < 0)
	      return -1;
	    count += sub_count;
	  }

	/* There must be no padding.  */
	if (wi::to_wide (TYPE_SIZE (type))
	    != count * GET_MODE_BITSIZE (*modep))
	  return -1;

	return count;
      }

    case UNION_TYPE:
    case QUAL_UNION_TYPE:
      {
	/* These aren't very interesting except in a degenerate case.  */
	int count = 0;
	int sub_count;
	tree field;

	/* Can't handle incomplete types nor sizes that are not
	   fixed.  */
	if (!COMPLETE_TYPE_P (type)
	    || TREE_CODE (TYPE_SIZE (type)) != INTEGER_CST)
	  return -1;

	for (field = TYPE_FIELDS (type); field; field = TREE_CHAIN (field))
	  {
	    if (TREE_CODE (field) != FIELD_DECL)
	      continue;

	    sub_count = rs6000_aggregate_candidate (TREE_TYPE (field), modep);
	    if (sub_count < 0)
	      return -1;
	    count = count > sub_count ? count : sub_count;
	  }

	/* There must be no padding.  */
	if (wi::to_wide (TYPE_SIZE (type))
	    != count * GET_MODE_BITSIZE (*modep))
	  return -1;

	return count;
      }

    default:
      break;
    }

  return -1;
}

/* If an argument, whose type is described by TYPE and MODE, is a homogeneous
   float or vector aggregate that shall be passed in FP/vector registers
   according to the ELFv2 ABI, return the homogeneous element mode in
   *ELT_MODE and the number of elements in *N_ELTS, and return TRUE.

   Otherwise, set *ELT_MODE to MODE and *N_ELTS to 1, and return FALSE.  */

bool
rs6000_discover_homogeneous_aggregate (machine_mode mode, const_tree type,
				       machine_mode *elt_mode,
				       int *n_elts)
{
  /* Note that we do not accept complex types at the top level as
     homogeneous aggregates; these types are handled via the
     targetm.calls.split_complex_arg mechanism.  Complex types
     can be elements of homogeneous aggregates, however.  */
  if (TARGET_HARD_FLOAT && DEFAULT_ABI == ABI_ELFv2 && type
      && AGGREGATE_TYPE_P (type))
    {
      machine_mode field_mode = VOIDmode;
      int field_count = rs6000_aggregate_candidate (type, &field_mode);

      if (field_count > 0)
	{
	  int reg_size = ALTIVEC_OR_VSX_VECTOR_MODE (field_mode) ? 16 : 8;
	  int field_size = ROUND_UP (GET_MODE_SIZE (field_mode), reg_size);

	  /* The ELFv2 ABI allows homogeneous aggregates to occupy
	     up to AGGR_ARG_NUM_REG registers.  */
	  if (field_count * field_size <= AGGR_ARG_NUM_REG * reg_size)
	    {
	      if (elt_mode)
		*elt_mode = field_mode;
	      if (n_elts)
		*n_elts = field_count;
	      return true;
	    }
	}
    }

  if (elt_mode)
    *elt_mode = mode;
  if (n_elts)
    *n_elts = 1;
  return false;
}

/* Return a nonzero value to say to return the function value in
   memory, just as large structures are always returned.  TYPE will be
   the data type of the value, and FNTYPE will be the type of the
   function doing the returning, or @code{NULL} for libcalls.

   The AIX ABI for the RS/6000 specifies that all structures are
   returned in memory.  The Darwin ABI does the same.
   
   For the Darwin 64 Bit ABI, a function result can be returned in
   registers or in memory, depending on the size of the return data
   type.  If it is returned in registers, the value occupies the same
   registers as it would if it were the first and only function
   argument.  Otherwise, the function places its result in memory at
   the location pointed to by GPR3.
   
   The SVR4 ABI specifies that structures <= 8 bytes are returned in r3/r4, 
   but a draft put them in memory, and GCC used to implement the draft
   instead of the final standard.  Therefore, aix_struct_return
   controls this instead of DEFAULT_ABI; V.4 targets needing backward
   compatibility can change DRAFT_V4_STRUCT_RET to override the
   default, and -m switches get the final word.  See
   rs6000_option_override_internal for more details.

   The PPC32 SVR4 ABI uses IEEE double extended for long double, if 128-bit
   long double support is enabled.  These values are returned in memory.

   int_size_in_bytes returns -1 for variable size objects, which go in
   memory always.  The cast to unsigned makes -1 > 8.  */

bool
rs6000_return_in_memory (const_tree type, const_tree fntype ATTRIBUTE_UNUSED)
{
  /* For the Darwin64 ABI, test if we can fit the return value in regs.  */
  if (TARGET_MACHO
      && rs6000_darwin64_abi
      && TREE_CODE (type) == RECORD_TYPE
      && int_size_in_bytes (type) > 0)
    {
      CUMULATIVE_ARGS valcum;
      rtx valret;

      valcum.words = 0;
      valcum.fregno = FP_ARG_MIN_REG;
      valcum.vregno = ALTIVEC_ARG_MIN_REG;
      /* Do a trial code generation as if this were going to be passed
	 as an argument; if any part goes in memory, we return NULL.  */
      valret = rs6000_darwin64_record_arg (&valcum, type, true, true);
      if (valret)
	return false;
      /* Otherwise fall through to more conventional ABI rules.  */
    }

  /* The ELFv2 ABI returns homogeneous VFP aggregates in registers */
  if (rs6000_discover_homogeneous_aggregate (TYPE_MODE (type), type,
					     NULL, NULL))
    return false;

  /* The ELFv2 ABI returns aggregates up to 16B in registers */
  if (DEFAULT_ABI == ABI_ELFv2 && AGGREGATE_TYPE_P (type)
      && (unsigned HOST_WIDE_INT) int_size_in_bytes (type) <= 16)
    return false;

  if (AGGREGATE_TYPE_P (type)
      && (aix_struct_return
	  || (unsigned HOST_WIDE_INT) int_size_in_bytes (type) > 8))
    return true;

  /* Allow -maltivec -mabi=no-altivec without warning.  Altivec vector
     modes only exist for GCC vector types if -maltivec.  */
  if (TARGET_32BIT && !TARGET_ALTIVEC_ABI
      && ALTIVEC_VECTOR_MODE (TYPE_MODE (type)))
    return false;

  /* Return synthetic vectors in memory.  */
  if (TREE_CODE (type) == VECTOR_TYPE
      && int_size_in_bytes (type) > (TARGET_ALTIVEC_ABI ? 16 : 8))
    {
      static bool warned_for_return_big_vectors = false;
      if (!warned_for_return_big_vectors)
	{
	  warning (OPT_Wpsabi, "GCC vector returned by reference: "
		   "non-standard ABI extension with no compatibility "
		   "guarantee");
	  warned_for_return_big_vectors = true;
	}
      return true;
    }

  if (DEFAULT_ABI == ABI_V4 && TARGET_IEEEQUAD
      && FLOAT128_IEEE_P (TYPE_MODE (type)))
    return true;

  return false;
}

/* Specify whether values returned in registers should be at the most
   significant end of a register.  We want aggregates returned by
   value to match the way aggregates are passed to functions.  */

bool
rs6000_return_in_msb (const_tree valtype)
{
  return (DEFAULT_ABI == ABI_ELFv2
	  && BYTES_BIG_ENDIAN
	  && AGGREGATE_TYPE_P (valtype)
	  && (rs6000_function_arg_padding (TYPE_MODE (valtype), valtype)
	      == PAD_UPWARD));
}

#ifdef HAVE_AS_GNU_ATTRIBUTE
/* Return TRUE if a call to function FNDECL may be one that
   potentially affects the function calling ABI of the object file.  */

static bool
call_ABI_of_interest (tree fndecl)
{
  if (rs6000_gnu_attr && symtab->state == EXPANSION)
    {
      struct cgraph_node *c_node;

      /* Libcalls are always interesting.  */
      if (fndecl == NULL_TREE)
	return true;

      /* Any call to an external function is interesting.  */
      if (DECL_EXTERNAL (fndecl))
	return true;

      /* Interesting functions that we are emitting in this object file.  */
      c_node = cgraph_node::get (fndecl);
      c_node = c_node->ultimate_alias_target ();
      return !c_node->only_called_directly_p ();
    }
  return false;
}
#endif

/* Initialize a variable CUM of type CUMULATIVE_ARGS
   for a call to a function whose data type is FNTYPE.
   For a library call, FNTYPE is 0 and RETURN_MODE the return value mode.

   For incoming args we set the number of arguments in the prototype large
   so we never return a PARALLEL.  */

void
init_cumulative_args (CUMULATIVE_ARGS *cum, tree fntype,
		      rtx libname ATTRIBUTE_UNUSED, int incoming,
		      int libcall, int n_named_args,
		      tree fndecl,
		      machine_mode return_mode ATTRIBUTE_UNUSED)
{
  static CUMULATIVE_ARGS zero_cumulative;

  *cum = zero_cumulative;
  cum->words = 0;
  cum->fregno = FP_ARG_MIN_REG;
  cum->vregno = ALTIVEC_ARG_MIN_REG;
  cum->prototype = (fntype && prototype_p (fntype));
  cum->call_cookie = ((DEFAULT_ABI == ABI_V4 && libcall)
		      ? CALL_LIBCALL : CALL_NORMAL);
  cum->sysv_gregno = GP_ARG_MIN_REG;
  cum->stdarg = stdarg_p (fntype);
  cum->libcall = libcall;

  cum->nargs_prototype = 0;
  if (incoming || cum->prototype)
    cum->nargs_prototype = n_named_args;

  /* Check for a longcall attribute.  */
  if ((!fntype && rs6000_default_long_calls)
      || (fntype
	  && lookup_attribute ("longcall", TYPE_ATTRIBUTES (fntype))
	  && !lookup_attribute ("shortcall", TYPE_ATTRIBUTES (fntype))))
    cum->call_cookie |= CALL_LONG;
  else if (DEFAULT_ABI != ABI_DARWIN)
    {
      bool is_local = (fndecl
		       && !DECL_EXTERNAL (fndecl)
		       && !DECL_WEAK (fndecl)
		       && (*targetm.binds_local_p) (fndecl));
      if (is_local)
	;
      else if (flag_plt)
	{
	  if (fntype
	      && lookup_attribute ("noplt", TYPE_ATTRIBUTES (fntype)))
	    cum->call_cookie |= CALL_LONG;
	}
      else
	{
	  if (!(fntype
		&& lookup_attribute ("plt", TYPE_ATTRIBUTES (fntype))))
	    cum->call_cookie |= CALL_LONG;
	}
    }

  if (TARGET_DEBUG_ARG)
    {
      fprintf (stderr, "\ninit_cumulative_args:");
      if (fntype)
	{
	  tree ret_type = TREE_TYPE (fntype);
	  fprintf (stderr, " ret code = %s,",
		   get_tree_code_name (TREE_CODE (ret_type)));
	}

      if (cum->call_cookie & CALL_LONG)
	fprintf (stderr, " longcall,");

      fprintf (stderr, " proto = %d, nargs = %d\n",
	       cum->prototype, cum->nargs_prototype);
    }

#ifdef HAVE_AS_GNU_ATTRIBUTE
  if (TARGET_ELF && (TARGET_64BIT || DEFAULT_ABI == ABI_V4))
    {
      cum->escapes = call_ABI_of_interest (fndecl);
      if (cum->escapes)
	{
	  tree return_type;

	  if (fntype)
	    {
	      return_type = TREE_TYPE (fntype);
	      return_mode = TYPE_MODE (return_type);
	    }
	  else
	    return_type = lang_hooks.types.type_for_mode (return_mode, 0);

	  if (return_type != NULL)
	    {
	      if (TREE_CODE (return_type) == RECORD_TYPE
		  && TYPE_TRANSPARENT_AGGR (return_type))
		{
		  return_type = TREE_TYPE (first_field (return_type));
		  return_mode = TYPE_MODE (return_type);
		}
	      if (AGGREGATE_TYPE_P (return_type)
		  && ((unsigned HOST_WIDE_INT) int_size_in_bytes (return_type)
		      <= 8))
		rs6000_returns_struct = true;
	    }
	  if (SCALAR_FLOAT_MODE_P (return_mode))
	    {
	      rs6000_passes_float = true;
	      if ((HAVE_LD_PPC_GNU_ATTR_LONG_DOUBLE || TARGET_64BIT)
		  && (FLOAT128_IBM_P (return_mode)
		      || FLOAT128_IEEE_P (return_mode)
		      || (return_type != NULL
			  && (TYPE_MAIN_VARIANT (return_type)
			      == long_double_type_node))))
		rs6000_passes_long_double = true;

	      /* Note if we passed or return a IEEE 128-bit type.  We changed
		 the mangling for these types, and we may need to make an alias
		 with the old mangling.  */
	      if (FLOAT128_IEEE_P (return_mode))
		rs6000_passes_ieee128 = true;
	    }
	  if (ALTIVEC_OR_VSX_VECTOR_MODE (return_mode))
	    rs6000_passes_vector = true;
	}
    }
#endif

  if (fntype
      && !TARGET_ALTIVEC
      && TARGET_ALTIVEC_ABI
      && ALTIVEC_VECTOR_MODE (TYPE_MODE (TREE_TYPE (fntype))))
    {
      error ("cannot return value in vector register because"
	     " altivec instructions are disabled, use %qs"
	     " to enable them", "-maltivec");
    }
}


/* On rs6000, function arguments are promoted, as are function return
   values.  */

machine_mode
rs6000_promote_function_mode (const_tree type ATTRIBUTE_UNUSED,
			      machine_mode mode,
			      int *punsignedp ATTRIBUTE_UNUSED,
			      const_tree, int)
{
  PROMOTE_MODE (mode, *punsignedp, type);

  return mode;
}

/* Return true if TYPE must be passed on the stack and not in registers.  */

bool
rs6000_must_pass_in_stack (const function_arg_info &arg)
{
  if (DEFAULT_ABI == ABI_AIX || DEFAULT_ABI == ABI_ELFv2 || TARGET_64BIT)
    return must_pass_in_stack_var_size (arg);
  else
    return must_pass_in_stack_var_size_or_pad (arg);
}

static inline bool
is_complex_IBM_long_double (machine_mode mode)
{
  return mode == ICmode || (mode == TCmode && FLOAT128_IBM_P (TCmode));
}

/* Whether ABI_V4 passes MODE args to a function in floating point
   registers.  */

static bool
abi_v4_pass_in_fpr (machine_mode mode, bool named)
{
  if (!TARGET_HARD_FLOAT)
    return false;
  if (mode == DFmode)
    return true;
  if (mode == SFmode && named)
    return true;
  /* ABI_V4 passes complex IBM long double in 8 gprs.
     Stupid, but we can't change the ABI now.  */
  if (is_complex_IBM_long_double (mode))
    return false;
  if (FLOAT128_2REG_P (mode))
    return true;
  if (DECIMAL_FLOAT_MODE_P (mode))
    return true;
  return false;
}

/* Implement TARGET_FUNCTION_ARG_PADDING.

   For the AIX ABI structs are always stored left shifted in their
   argument slot.  */

pad_direction
rs6000_function_arg_padding (machine_mode mode, const_tree type)
{
#ifndef AGGREGATE_PADDING_FIXED
#define AGGREGATE_PADDING_FIXED 0
#endif
#ifndef AGGREGATES_PAD_UPWARD_ALWAYS
#define AGGREGATES_PAD_UPWARD_ALWAYS 0
#endif

  if (!AGGREGATE_PADDING_FIXED)
    {
      /* GCC used to pass structures of the same size as integer types as
	 if they were in fact integers, ignoring TARGET_FUNCTION_ARG_PADDING.
	 i.e. Structures of size 1 or 2 (or 4 when TARGET_64BIT) were
	 passed padded downward, except that -mstrict-align further
	 muddied the water in that multi-component structures of 2 and 4
	 bytes in size were passed padded upward.

	 The following arranges for best compatibility with previous
	 versions of gcc, but removes the -mstrict-align dependency.  */
      if (BYTES_BIG_ENDIAN)
	{
	  HOST_WIDE_INT size = 0;

	  if (mode == BLKmode)
	    {
	      if (type && TREE_CODE (TYPE_SIZE (type)) == INTEGER_CST)
		size = int_size_in_bytes (type);
	    }
	  else
	    size = GET_MODE_SIZE (mode);

	  if (size == 1 || size == 2 || size == 4)
	    return PAD_DOWNWARD;
	}
      return PAD_UPWARD;
    }

  if (AGGREGATES_PAD_UPWARD_ALWAYS)
    {
      if (type != 0 && AGGREGATE_TYPE_P (type))
	return PAD_UPWARD;
    }

  /* Fall back to the default.  */
  return default_function_arg_padding (mode, type);
}

/* If defined, a C expression that gives the alignment boundary, in bits,
   of an argument with the specified mode and type.  If it is not defined,
   PARM_BOUNDARY is used for all arguments.

   V.4 wants long longs and doubles to be double word aligned.  Just
   testing the mode size is a boneheaded way to do this as it means
   that other types such as complex int are also double word aligned.
   However, we're stuck with this because changing the ABI might break
   existing library interfaces.

   Quadword align Altivec/VSX vectors.
   Quadword align large synthetic vector types.   */

unsigned int
rs6000_function_arg_boundary (machine_mode mode, const_tree type)
{
  machine_mode elt_mode;
  int n_elts;

  rs6000_discover_homogeneous_aggregate (mode, type, &elt_mode, &n_elts);

  if (DEFAULT_ABI == ABI_V4
      && (GET_MODE_SIZE (mode) == 8
	  || (TARGET_HARD_FLOAT
	      && !is_complex_IBM_long_double (mode)
	      && FLOAT128_2REG_P (mode))))
    return 64;
  else if (FLOAT128_VECTOR_P (mode))
    return 128;
  else if (type && TREE_CODE (type) == VECTOR_TYPE
	   && int_size_in_bytes (type) >= 8
	   && int_size_in_bytes (type) < 16)
    return 64;
  else if (ALTIVEC_OR_VSX_VECTOR_MODE (elt_mode)
	   || (type && TREE_CODE (type) == VECTOR_TYPE
	       && int_size_in_bytes (type) >= 16))
    return 128;

  /* Aggregate types that need > 8 byte alignment are quadword-aligned
     in the parameter area in the ELFv2 ABI, and in the AIX ABI unless
     -mcompat-align-parm is used.  */
  if (((DEFAULT_ABI == ABI_AIX && !rs6000_compat_align_parm)
       || DEFAULT_ABI == ABI_ELFv2)
      && type && TYPE_ALIGN (type) > 64)
    {
      /* "Aggregate" means any AGGREGATE_TYPE except for single-element
         or homogeneous float/vector aggregates here.  We already handled
         vector aggregates above, but still need to check for float here. */
      bool aggregate_p = (AGGREGATE_TYPE_P (type)
			  && !SCALAR_FLOAT_MODE_P (elt_mode));

      /* We used to check for BLKmode instead of the above aggregate type
	 check.  Warn when this results in any difference to the ABI.  */
      if (aggregate_p != (mode == BLKmode))
	{
	  static bool warned;
	  if (!warned && warn_psabi)
	    {
	      warned = true;
	      inform (input_location,
		      "the ABI of passing aggregates with %d-byte alignment"
		      " has changed in GCC 5",
		      (int) TYPE_ALIGN (type) / BITS_PER_UNIT);
	    }
	}

      if (aggregate_p)
	return 128;
    }

  /* Similar for the Darwin64 ABI.  Note that for historical reasons we
     implement the "aggregate type" check as a BLKmode check here; this
     means certain aggregate types are in fact not aligned.  */
  if (TARGET_MACHO && rs6000_darwin64_abi
      && mode == BLKmode
      && type && TYPE_ALIGN (type) > 64)
    return 128;

  return PARM_BOUNDARY;
}

/* The offset in words to the start of the parameter save area.  */

static unsigned int
rs6000_parm_offset (void)
{
  return (DEFAULT_ABI == ABI_V4 ? 2
	  : DEFAULT_ABI == ABI_ELFv2 ? 4
	  : 6);
}

/* For a function parm of MODE and TYPE, return the starting word in
   the parameter area.  NWORDS of the parameter area are already used.  */

static unsigned int
rs6000_parm_start (machine_mode mode, const_tree type,
		   unsigned int nwords)
{
  unsigned int align;

  align = rs6000_function_arg_boundary (mode, type) / PARM_BOUNDARY - 1;
  return nwords + (-(rs6000_parm_offset () + nwords) & align);
}

/* Compute the size (in words) of a function argument.  */

static unsigned long
rs6000_arg_size (machine_mode mode, const_tree type)
{
  unsigned long size;

  if (mode != BLKmode)
    size = GET_MODE_SIZE (mode);
  else
    size = int_size_in_bytes (type);

  if (TARGET_32BIT)
    return (size + 3) >> 2;
  else
    return (size + 7) >> 3;
}

/* Use this to flush pending int fields.  */

static void
rs6000_darwin64_record_arg_advance_flush (CUMULATIVE_ARGS *cum,
					  HOST_WIDE_INT bitpos, int final)
{
  unsigned int startbit, endbit;
  int intregs, intoffset;

  /* Handle the situations where a float is taking up the first half
     of the GPR, and the other half is empty (typically due to
     alignment restrictions). We can detect this by a 8-byte-aligned
     int field, or by seeing that this is the final flush for this
     argument. Count the word and continue on.  */
  if (cum->floats_in_gpr == 1
      && (cum->intoffset % 64 == 0
	  || (cum->intoffset == -1 && final)))
    {
      cum->words++;
      cum->floats_in_gpr = 0;
    }

  if (cum->intoffset == -1)
    return;

  intoffset = cum->intoffset;
  cum->intoffset = -1;
  cum->floats_in_gpr = 0;

  if (intoffset % BITS_PER_WORD != 0)
    {
      unsigned int bits = BITS_PER_WORD - intoffset % BITS_PER_WORD;
      if (!int_mode_for_size (bits, 0).exists ())
	{
	  /* We couldn't find an appropriate mode, which happens,
	     e.g., in packed structs when there are 3 bytes to load.
	     Back intoffset back to the beginning of the word in this
	     case.  */
	  intoffset = ROUND_DOWN (intoffset, BITS_PER_WORD);
	}
    }

  startbit = ROUND_DOWN (intoffset, BITS_PER_WORD);
  endbit = ROUND_UP (bitpos, BITS_PER_WORD);
  intregs = (endbit - startbit) / BITS_PER_WORD;
  cum->words += intregs;
  /* words should be unsigned. */
  if ((unsigned)cum->words < (endbit/BITS_PER_WORD))
    {
      int pad = (endbit/BITS_PER_WORD) - cum->words;
      cum->words += pad;
    }
}

/* The darwin64 ABI calls for us to recurse down through structs,
   looking for elements passed in registers.  Unfortunately, we have
   to track int register count here also because of misalignments
   in powerpc alignment mode.  */

static void
rs6000_darwin64_record_arg_advance_recurse (CUMULATIVE_ARGS *cum,
					    const_tree type,
					    HOST_WIDE_INT startbitpos)
{
  tree f;

  for (f = TYPE_FIELDS (type); f ; f = DECL_CHAIN (f))
    if (TREE_CODE (f) == FIELD_DECL)
      {
	HOST_WIDE_INT bitpos = startbitpos;
	tree ftype = TREE_TYPE (f);
	machine_mode mode;
	if (ftype == error_mark_node)
	  continue;
	mode = TYPE_MODE (ftype);

	if (DECL_SIZE (f) != 0
	    && tree_fits_uhwi_p (bit_position (f)))
	  bitpos += int_bit_position (f);

	/* ??? FIXME: else assume zero offset.  */

	if (TREE_CODE (ftype) == RECORD_TYPE)
	  rs6000_darwin64_record_arg_advance_recurse (cum, ftype, bitpos);
	else if (USE_FP_FOR_ARG_P (cum, mode))
	  {
	    unsigned n_fpregs = (GET_MODE_SIZE (mode) + 7) >> 3;
	    rs6000_darwin64_record_arg_advance_flush (cum, bitpos, 0);
	    cum->fregno += n_fpregs;
	    /* Single-precision floats present a special problem for
	       us, because they are smaller than an 8-byte GPR, and so
	       the structure-packing rules combined with the standard
	       varargs behavior mean that we want to pack float/float
	       and float/int combinations into a single register's
	       space. This is complicated by the arg advance flushing,
	       which works on arbitrarily large groups of int-type
	       fields.  */
	    if (mode == SFmode)
	      {
		if (cum->floats_in_gpr == 1)
		  {
		    /* Two floats in a word; count the word and reset
		       the float count.  */
		    cum->words++;
		    cum->floats_in_gpr = 0;
		  }
		else if (bitpos % 64 == 0)
		  {
		    /* A float at the beginning of an 8-byte word;
		       count it and put off adjusting cum->words until
		       we see if a arg advance flush is going to do it
		       for us.  */
		    cum->floats_in_gpr++;
		  }
		else
		  {
		    /* The float is at the end of a word, preceded
		       by integer fields, so the arg advance flush
		       just above has already set cum->words and
		       everything is taken care of.  */
		  }
	      }
	    else
	      cum->words += n_fpregs;
	  }
	else if (USE_ALTIVEC_FOR_ARG_P (cum, mode, 1))
	  {
	    rs6000_darwin64_record_arg_advance_flush (cum, bitpos, 0);
	    cum->vregno++;
	    cum->words += 2;
	  }
	else if (cum->intoffset == -1)
	  cum->intoffset = bitpos;
      }
}

/* Check for an item that needs to be considered specially under the darwin 64
   bit ABI.  These are record types where the mode is BLK or the structure is
   8 bytes in size.  */
int
rs6000_darwin64_struct_check_p (machine_mode mode, const_tree type)
{
  return rs6000_darwin64_abi
	 && ((mode == BLKmode 
	      && TREE_CODE (type) == RECORD_TYPE 
	      && int_size_in_bytes (type) > 0)
	  || (type && TREE_CODE (type) == RECORD_TYPE 
	      && int_size_in_bytes (type) == 8)) ? 1 : 0;
}

/* Update the data in CUM to advance over an argument
   of mode MODE and data type TYPE.
   (TYPE is null for libcalls where that information may not be available.)

   Note that for args passed by reference, function_arg will be called
   with MODE and TYPE set to that of the pointer to the arg, not the arg
   itself.  */

static void
rs6000_function_arg_advance_1 (CUMULATIVE_ARGS *cum, machine_mode mode,
			       const_tree type, bool named, int depth)
{
  machine_mode elt_mode;
  int n_elts;

  rs6000_discover_homogeneous_aggregate (mode, type, &elt_mode, &n_elts);

  /* Only tick off an argument if we're not recursing.  */
  if (depth == 0)
    cum->nargs_prototype--;

#ifdef HAVE_AS_GNU_ATTRIBUTE
  if (TARGET_ELF && (TARGET_64BIT || DEFAULT_ABI == ABI_V4)
      && cum->escapes)
    {
      if (SCALAR_FLOAT_MODE_P (mode))
	{
	  rs6000_passes_float = true;
	  if ((HAVE_LD_PPC_GNU_ATTR_LONG_DOUBLE || TARGET_64BIT)
	      && (FLOAT128_IBM_P (mode)
		  || FLOAT128_IEEE_P (mode)
		  || (type != NULL
		      && TYPE_MAIN_VARIANT (type) == long_double_type_node)))
	    rs6000_passes_long_double = true;

	  /* Note if we passed or return a IEEE 128-bit type.  We changed the
	     mangling for these types, and we may need to make an alias with
	     the old mangling.  */
	  if (FLOAT128_IEEE_P (mode))
	    rs6000_passes_ieee128 = true;
	}
      if (named && ALTIVEC_OR_VSX_VECTOR_MODE (mode))
	rs6000_passes_vector = true;
    }
#endif

  if (TARGET_ALTIVEC_ABI
      && (ALTIVEC_OR_VSX_VECTOR_MODE (elt_mode)
	  || (type && TREE_CODE (type) == VECTOR_TYPE
	      && int_size_in_bytes (type) == 16)))
    {
      bool stack = false;

      if (USE_ALTIVEC_FOR_ARG_P (cum, elt_mode, named))
	{
	  cum->vregno += n_elts;

	  if (!TARGET_ALTIVEC)
	    error ("cannot pass argument in vector register because"
		   " altivec instructions are disabled, use %qs"
		   " to enable them", "-maltivec");

	  /* PowerPC64 Linux and AIX allocate GPRs for a vector argument
	     even if it is going to be passed in a vector register.
	     Darwin does the same for variable-argument functions.  */
	  if (((DEFAULT_ABI == ABI_AIX || DEFAULT_ABI == ABI_ELFv2)
	       && TARGET_64BIT)
	      || (cum->stdarg && DEFAULT_ABI != ABI_V4))
	    stack = true;
	}
      else
	stack = true;

      if (stack)
	{
	  int align;

	  /* Vector parameters must be 16-byte aligned.  In 32-bit
	     mode this means we need to take into account the offset
	     to the parameter save area.  In 64-bit mode, they just
	     have to start on an even word, since the parameter save
	     area is 16-byte aligned.  */
	  if (TARGET_32BIT)
	    align = -(rs6000_parm_offset () + cum->words) & 3;
	  else
	    align = cum->words & 1;
	  cum->words += align + rs6000_arg_size (mode, type);

	  if (TARGET_DEBUG_ARG)
	    {
	      fprintf (stderr, "function_adv: words = %2d, align=%d, ",
		       cum->words, align);
	      fprintf (stderr, "nargs = %4d, proto = %d, mode = %4s\n",
		       cum->nargs_prototype, cum->prototype,
		       GET_MODE_NAME (mode));
	    }
	}
    }
  else if (TARGET_MACHO && rs6000_darwin64_struct_check_p (mode, type))
    {
      int size = int_size_in_bytes (type);
      /* Variable sized types have size == -1 and are
	 treated as if consisting entirely of ints.
	 Pad to 16 byte boundary if needed.  */
      if (TYPE_ALIGN (type) >= 2 * BITS_PER_WORD
	  && (cum->words % 2) != 0)
	cum->words++;
      /* For varargs, we can just go up by the size of the struct. */
      if (!named)
	cum->words += (size + 7) / 8;
      else
	{
	  /* It is tempting to say int register count just goes up by
	     sizeof(type)/8, but this is wrong in a case such as
	     { int; double; int; } [powerpc alignment].  We have to
	     grovel through the fields for these too.  */
	  cum->intoffset = 0;
	  cum->floats_in_gpr = 0;
	  rs6000_darwin64_record_arg_advance_recurse (cum, type, 0);
	  rs6000_darwin64_record_arg_advance_flush (cum,
						    size * BITS_PER_UNIT, 1);
	}
	  if (TARGET_DEBUG_ARG)
	    {
	      fprintf (stderr, "function_adv: words = %2d, align=%d, size=%d",
		       cum->words, TYPE_ALIGN (type), size);
	      fprintf (stderr, 
	           "nargs = %4d, proto = %d, mode = %4s (darwin64 abi)\n",
		       cum->nargs_prototype, cum->prototype,
		       GET_MODE_NAME (mode));
	    }
    }
  else if (DEFAULT_ABI == ABI_V4)
    {
      if (abi_v4_pass_in_fpr (mode, named))
	{
	  /* _Decimal128 must use an even/odd register pair.  This assumes
	     that the register number is odd when fregno is odd.  */
	  if (mode == TDmode && (cum->fregno % 2) == 1)
	    cum->fregno++;

	  if (cum->fregno + (FLOAT128_2REG_P (mode) ? 1 : 0)
	      <= FP_ARG_V4_MAX_REG)
	    cum->fregno += (GET_MODE_SIZE (mode) + 7) >> 3;
	  else
	    {
	      cum->fregno = FP_ARG_V4_MAX_REG + 1;
	      if (mode == DFmode || FLOAT128_IBM_P (mode)
		  || mode == DDmode || mode == TDmode)
		cum->words += cum->words & 1;
	      cum->words += rs6000_arg_size (mode, type);
	    }
	}
      else
	{
	  int n_words = rs6000_arg_size (mode, type);
	  int gregno = cum->sysv_gregno;

	  /* Long long is put in (r3,r4), (r5,r6), (r7,r8) or (r9,r10).
	     As does any other 2 word item such as complex int due to a
	     historical mistake.  */
	  if (n_words == 2)
	    gregno += (1 - gregno) & 1;

	  /* Multi-reg args are not split between registers and stack.  */
	  if (gregno + n_words - 1 > GP_ARG_MAX_REG)
	    {
	      /* Long long is aligned on the stack.  So are other 2 word
		 items such as complex int due to a historical mistake.  */
	      if (n_words == 2)
		cum->words += cum->words & 1;
	      cum->words += n_words;
	    }

	  /* Note: continuing to accumulate gregno past when we've started
	     spilling to the stack indicates the fact that we've started
	     spilling to the stack to expand_builtin_saveregs.  */
	  cum->sysv_gregno = gregno + n_words;
	}

      if (TARGET_DEBUG_ARG)
	{
	  fprintf (stderr, "function_adv: words = %2d, fregno = %2d, ",
		   cum->words, cum->fregno);
	  fprintf (stderr, "gregno = %2d, nargs = %4d, proto = %d, ",
		   cum->sysv_gregno, cum->nargs_prototype, cum->prototype);
	  fprintf (stderr, "mode = %4s, named = %d\n",
		   GET_MODE_NAME (mode), named);
	}
    }
  else
    {
      int n_words = rs6000_arg_size (mode, type);
      int start_words = cum->words;
      int align_words = rs6000_parm_start (mode, type, start_words);

      cum->words = align_words + n_words;

      if (SCALAR_FLOAT_MODE_P (elt_mode) && TARGET_HARD_FLOAT)
	{
	  /* _Decimal128 must be passed in an even/odd float register pair.
	     This assumes that the register number is odd when fregno is
	     odd.  */
	  if (elt_mode == TDmode && (cum->fregno % 2) == 1)
	    cum->fregno++;
	  cum->fregno += n_elts * ((GET_MODE_SIZE (elt_mode) + 7) >> 3);
	}

      if (TARGET_DEBUG_ARG)
	{
	  fprintf (stderr, "function_adv: words = %2d, fregno = %2d, ",
		   cum->words, cum->fregno);
	  fprintf (stderr, "nargs = %4d, proto = %d, mode = %4s, ",
		   cum->nargs_prototype, cum->prototype, GET_MODE_NAME (mode));
	  fprintf (stderr, "named = %d, align = %d, depth = %d\n",
		   named, align_words - start_words, depth);
	}
    }
}

void
rs6000_function_arg_advance (cumulative_args_t cum,
			     const function_arg_info &arg)
{
  rs6000_function_arg_advance_1 (get_cumulative_args (cum),
				 arg.mode, arg.type, arg.named, 0);
}

/* A subroutine of rs6000_darwin64_record_arg.  Assign the bits of the
   structure between cum->intoffset and bitpos to integer registers.  */

static void
rs6000_darwin64_record_arg_flush (CUMULATIVE_ARGS *cum,
				  HOST_WIDE_INT bitpos, rtx rvec[], int *k)
{
  machine_mode mode;
  unsigned int regno;
  unsigned int startbit, endbit;
  int this_regno, intregs, intoffset;
  rtx reg;

  if (cum->intoffset == -1)
    return;

  intoffset = cum->intoffset;
  cum->intoffset = -1;

  /* If this is the trailing part of a word, try to only load that
     much into the register.  Otherwise load the whole register.  Note
     that in the latter case we may pick up unwanted bits.  It's not a
     problem at the moment but may wish to revisit.  */

  if (intoffset % BITS_PER_WORD != 0)
    {
      unsigned int bits = BITS_PER_WORD - intoffset % BITS_PER_WORD;
      if (!int_mode_for_size (bits, 0).exists (&mode))
	{
	  /* We couldn't find an appropriate mode, which happens,
	     e.g., in packed structs when there are 3 bytes to load.
	     Back intoffset back to the beginning of the word in this
	     case.  */
	  intoffset = ROUND_DOWN (intoffset, BITS_PER_WORD);
	  mode = word_mode;
	}
    }
  else
    mode = word_mode;

  startbit = ROUND_DOWN (intoffset, BITS_PER_WORD);
  endbit = ROUND_UP (bitpos, BITS_PER_WORD);
  intregs = (endbit - startbit) / BITS_PER_WORD;
  this_regno = cum->words + intoffset / BITS_PER_WORD;

  if (intregs > 0 && intregs > GP_ARG_NUM_REG - this_regno)
    cum->use_stack = 1;

  intregs = MIN (intregs, GP_ARG_NUM_REG - this_regno);
  if (intregs <= 0)
    return;

  intoffset /= BITS_PER_UNIT;
  do
    {
      regno = GP_ARG_MIN_REG + this_regno;
      reg = gen_rtx_REG (mode, regno);
      rvec[(*k)++] =
	gen_rtx_EXPR_LIST (VOIDmode, reg, GEN_INT (intoffset));

      this_regno += 1;
      intoffset = (intoffset | (UNITS_PER_WORD-1)) + 1;
      mode = word_mode;
      intregs -= 1;
    }
  while (intregs > 0);
}

/* Recursive workhorse for the following.  */

static void
rs6000_darwin64_record_arg_recurse (CUMULATIVE_ARGS *cum, const_tree type,
				    HOST_WIDE_INT startbitpos, rtx rvec[],
				    int *k)
{
  tree f;

  for (f = TYPE_FIELDS (type); f ; f = DECL_CHAIN (f))
    if (TREE_CODE (f) == FIELD_DECL)
      {
	HOST_WIDE_INT bitpos = startbitpos;
	tree ftype = TREE_TYPE (f);
	machine_mode mode;
	if (ftype == error_mark_node)
	  continue;
	mode = TYPE_MODE (ftype);

	if (DECL_SIZE (f) != 0
	    && tree_fits_uhwi_p (bit_position (f)))
	  bitpos += int_bit_position (f);

	/* ??? FIXME: else assume zero offset.  */

	if (TREE_CODE (ftype) == RECORD_TYPE)
	  rs6000_darwin64_record_arg_recurse (cum, ftype, bitpos, rvec, k);
	else if (cum->named && USE_FP_FOR_ARG_P (cum, mode))
	  {
	    unsigned n_fpreg = (GET_MODE_SIZE (mode) + 7) >> 3;
#if 0
	    switch (mode)
	      {
	      case E_SCmode: mode = SFmode; break;
	      case E_DCmode: mode = DFmode; break;
	      case E_TCmode: mode = TFmode; break;
	      default: break;
	      }
#endif
	    rs6000_darwin64_record_arg_flush (cum, bitpos, rvec, k);
	    if (cum->fregno + n_fpreg > FP_ARG_MAX_REG + 1)
	      {
		gcc_assert (cum->fregno == FP_ARG_MAX_REG
			    && (mode == TFmode || mode == TDmode));
		/* Long double or _Decimal128 split over regs and memory.  */
		mode = DECIMAL_FLOAT_MODE_P (mode) ? DDmode : DFmode;
		cum->use_stack=1;
	      }
	    rvec[(*k)++]
	      = gen_rtx_EXPR_LIST (VOIDmode,
				   gen_rtx_REG (mode, cum->fregno++),
				   GEN_INT (bitpos / BITS_PER_UNIT));
	    if (FLOAT128_2REG_P (mode))
	      cum->fregno++;
	  }
	else if (cum->named && USE_ALTIVEC_FOR_ARG_P (cum, mode, 1))
	  {
	    rs6000_darwin64_record_arg_flush (cum, bitpos, rvec, k);
	    rvec[(*k)++]
	      = gen_rtx_EXPR_LIST (VOIDmode,
				   gen_rtx_REG (mode, cum->vregno++),
				   GEN_INT (bitpos / BITS_PER_UNIT));
	  }
	else if (cum->intoffset == -1)
	  cum->intoffset = bitpos;
      }
}

/* For the darwin64 ABI, we want to construct a PARALLEL consisting of
   the register(s) to be used for each field and subfield of a struct
   being passed by value, along with the offset of where the
   register's value may be found in the block.  FP fields go in FP
   register, vector fields go in vector registers, and everything
   else goes in int registers, packed as in memory.

   This code is also used for function return values.  RETVAL indicates
   whether this is the case.

   Much of this is taken from the SPARC V9 port, which has a similar
   calling convention.  */

rtx
rs6000_darwin64_record_arg (CUMULATIVE_ARGS *orig_cum, const_tree type,
			    bool named, bool retval)
{
  rtx rvec[FIRST_PSEUDO_REGISTER];
  int k = 1, kbase = 1;
  HOST_WIDE_INT typesize = int_size_in_bytes (type);
  /* This is a copy; modifications are not visible to our caller.  */
  CUMULATIVE_ARGS copy_cum = *orig_cum;
  CUMULATIVE_ARGS *cum = &copy_cum;

  /* Pad to 16 byte boundary if needed.  */
  if (!retval && TYPE_ALIGN (type) >= 2 * BITS_PER_WORD
      && (cum->words % 2) != 0)
    cum->words++;

  cum->intoffset = 0;
  cum->use_stack = 0;
  cum->named = named;

  /* Put entries into rvec[] for individual FP and vector fields, and
     for the chunks of memory that go in int regs.  Note we start at
     element 1; 0 is reserved for an indication of using memory, and
     may or may not be filled in below. */
  rs6000_darwin64_record_arg_recurse (cum, type, /* startbit pos= */ 0, rvec, &k);
  rs6000_darwin64_record_arg_flush (cum, typesize * BITS_PER_UNIT, rvec, &k);

  /* If any part of the struct went on the stack put all of it there.
     This hack is because the generic code for
     FUNCTION_ARG_PARTIAL_NREGS cannot handle cases where the register
     parts of the struct are not at the beginning.  */
  if (cum->use_stack)
    {
      if (retval)
	return NULL_RTX;    /* doesn't go in registers at all */
      kbase = 0;
      rvec[0] = gen_rtx_EXPR_LIST (VOIDmode, NULL_RTX, const0_rtx);
    }
  if (k > 1 || cum->use_stack)
    return gen_rtx_PARALLEL (BLKmode, gen_rtvec_v (k - kbase, &rvec[kbase]));
  else
    return NULL_RTX;
}

/* Determine where to place an argument in 64-bit mode with 32-bit ABI.  */

static rtx
rs6000_mixed_function_arg (machine_mode mode, const_tree type,
			   int align_words)
{
  int n_units;
  int i, k;
  rtx rvec[GP_ARG_NUM_REG + 1];

  if (align_words >= GP_ARG_NUM_REG)
    return NULL_RTX;

  n_units = rs6000_arg_size (mode, type);

  /* Optimize the simple case where the arg fits in one gpr, except in
     the case of BLKmode due to assign_parms assuming that registers are
     BITS_PER_WORD wide.  */
  if (n_units == 0
      || (n_units == 1 && mode != BLKmode))
    return gen_rtx_REG (mode, GP_ARG_MIN_REG + align_words);

  k = 0;
  if (align_words + n_units > GP_ARG_NUM_REG)
    /* Not all of the arg fits in gprs.  Say that it goes in memory too,
       using a magic NULL_RTX component.
       This is not strictly correct.  Only some of the arg belongs in
       memory, not all of it.  However, the normal scheme using
       function_arg_partial_nregs can result in unusual subregs, eg.
       (subreg:SI (reg:DF) 4), which are not handled well.  The code to
       store the whole arg to memory is often more efficient than code
       to store pieces, and we know that space is available in the right
       place for the whole arg.  */
    rvec[k++] = gen_rtx_EXPR_LIST (VOIDmode, NULL_RTX, const0_rtx);

  i = 0;
  do
    {
      rtx r = gen_rtx_REG (SImode, GP_ARG_MIN_REG + align_words);
      rtx off = GEN_INT (i++ * 4);
      rvec[k++] = gen_rtx_EXPR_LIST (VOIDmode, r, off);
    }
  while (++align_words < GP_ARG_NUM_REG && --n_units != 0);

  return gen_rtx_PARALLEL (mode, gen_rtvec_v (k, rvec));
}

/* We have an argument of MODE and TYPE that goes into FPRs or VRs,
   but must also be copied into the parameter save area starting at
   offset ALIGN_WORDS.  Fill in RVEC with the elements corresponding
   to the GPRs and/or memory.  Return the number of elements used.  */

static int
rs6000_psave_function_arg (machine_mode mode, const_tree type,
			   int align_words, rtx *rvec)
{
  int k = 0;

  if (align_words < GP_ARG_NUM_REG)
    {
      int n_words = rs6000_arg_size (mode, type);

      if (align_words + n_words > GP_ARG_NUM_REG
	  || mode == BLKmode
	  || (TARGET_32BIT && TARGET_POWERPC64))
	{
	  /* If this is partially on the stack, then we only
	     include the portion actually in registers here.  */
	  machine_mode rmode = TARGET_32BIT ? SImode : DImode;
	  int i = 0;

	  if (align_words + n_words > GP_ARG_NUM_REG)
	    {
	      /* Not all of the arg fits in gprs.  Say that it goes in memory
		 too, using a magic NULL_RTX component.  Also see comment in
		 rs6000_mixed_function_arg for why the normal
		 function_arg_partial_nregs scheme doesn't work in this case. */
	      rvec[k++] = gen_rtx_EXPR_LIST (VOIDmode, NULL_RTX, const0_rtx);
	    }

	  do
	    {
	      rtx r = gen_rtx_REG (rmode, GP_ARG_MIN_REG + align_words);
	      rtx off = GEN_INT (i++ * GET_MODE_SIZE (rmode));
	      rvec[k++] = gen_rtx_EXPR_LIST (VOIDmode, r, off);
	    }
	  while (++align_words < GP_ARG_NUM_REG && --n_words != 0);
	}
      else
	{
	  /* The whole arg fits in gprs.  */
	  rtx r = gen_rtx_REG (mode, GP_ARG_MIN_REG + align_words);
	  rvec[k++] = gen_rtx_EXPR_LIST (VOIDmode, r, const0_rtx);
	}
    }
  else
    {
      /* It's entirely in memory.  */
      rvec[k++] = gen_rtx_EXPR_LIST (VOIDmode, NULL_RTX, const0_rtx);
    }

  return k;
}

/* RVEC is a vector of K components of an argument of mode MODE.
   Construct the final function_arg return value from it.  */

static rtx
rs6000_finish_function_arg (machine_mode mode, rtx *rvec, int k)
{
  gcc_assert (k >= 1);

  /* Avoid returning a PARALLEL in the trivial cases.  */
  if (k == 1)
    {
      if (XEXP (rvec[0], 0) == NULL_RTX)
	return NULL_RTX;

      if (GET_MODE (XEXP (rvec[0], 0)) == mode)
	return XEXP (rvec[0], 0);
    }

  return gen_rtx_PARALLEL (mode, gen_rtvec_v (k, rvec));
}

/* Determine where to put an argument to a function.
   Value is zero to push the argument on the stack,
   or a hard register in which to store the argument.

   CUM is a variable of type CUMULATIVE_ARGS which gives info about
    the preceding args and about the function being called.  It is
    not modified in this routine.
   ARG is a description of the argument.

   On RS/6000 the first eight words of non-FP are normally in registers
   and the rest are pushed.  Under AIX, the first 13 FP args are in registers.
   Under V.4, the first 8 FP args are in registers.

   If this is floating-point and no prototype is specified, we use
   both an FP and integer register (or possibly FP reg and stack).  Library
   functions (when CALL_LIBCALL is set) always have the proper types for args,
   so we can pass the FP value just in one register.  emit_library_function
   doesn't support PARALLEL anyway.

   Note that for args passed by reference, function_arg will be called
   with ARG describing the pointer to the arg, not the arg itself.  */

rtx
rs6000_function_arg (cumulative_args_t cum_v, const function_arg_info &arg)
{
  CUMULATIVE_ARGS *cum = get_cumulative_args (cum_v);
  tree type = arg.type;
  machine_mode mode = arg.mode;
  bool named = arg.named;
  enum rs6000_abi abi = DEFAULT_ABI;
  machine_mode elt_mode;
  int n_elts;

  /* Return a marker to indicate whether CR1 needs to set or clear the
     bit that V.4 uses to say fp args were passed in registers.
     Assume that we don't need the marker for software floating point,
     or compiler generated library calls.  */
  if (arg.end_marker_p ())
    {
      if (abi == ABI_V4
	  && (cum->call_cookie & CALL_LIBCALL) == 0
	  && (cum->stdarg
	      || (cum->nargs_prototype < 0
		  && (cum->prototype || TARGET_NO_PROTOTYPE)))
	  && TARGET_HARD_FLOAT)
	return GEN_INT (cum->call_cookie
			| ((cum->fregno == FP_ARG_MIN_REG)
			   ? CALL_V4_SET_FP_ARGS
			   : CALL_V4_CLEAR_FP_ARGS));

      return GEN_INT (cum->call_cookie & ~CALL_LIBCALL);
    }

  rs6000_discover_homogeneous_aggregate (mode, type, &elt_mode, &n_elts);

  if (TARGET_MACHO && rs6000_darwin64_struct_check_p (mode, type))
    {
      rtx rslt = rs6000_darwin64_record_arg (cum, type, named, /*retval= */false);
      if (rslt != NULL_RTX)
	return rslt;
      /* Else fall through to usual handling.  */
    }

  if (USE_ALTIVEC_FOR_ARG_P (cum, elt_mode, named))
    {
      rtx rvec[GP_ARG_NUM_REG + AGGR_ARG_NUM_REG + 1];
      rtx r, off;
      int i, k = 0;

      /* Do we also need to pass this argument in the parameter save area?
	 Library support functions for IEEE 128-bit are assumed to not need the
	 value passed both in GPRs and in vector registers.  */
      if (TARGET_64BIT && !cum->prototype
	  && (!cum->libcall || !FLOAT128_VECTOR_P (elt_mode)))
	{
	  int align_words = ROUND_UP (cum->words, 2);
	  k = rs6000_psave_function_arg (mode, type, align_words, rvec);
	}

      /* Describe where this argument goes in the vector registers.  */
      for (i = 0; i < n_elts && cum->vregno + i <= ALTIVEC_ARG_MAX_REG; i++)
	{
	  r = gen_rtx_REG (elt_mode, cum->vregno + i);
	  off = GEN_INT (i * GET_MODE_SIZE (elt_mode));
	  rvec[k++] =  gen_rtx_EXPR_LIST (VOIDmode, r, off);
	}

      return rs6000_finish_function_arg (mode, rvec, k);
    }
  else if (TARGET_ALTIVEC_ABI
	   && (ALTIVEC_OR_VSX_VECTOR_MODE (mode)
	       || (type && TREE_CODE (type) == VECTOR_TYPE
		   && int_size_in_bytes (type) == 16)))
    {
      if (named || abi == ABI_V4)
	return NULL_RTX;
      else
	{
	  /* Vector parameters to varargs functions under AIX or Darwin
	     get passed in memory and possibly also in GPRs.  */
	  int align, align_words, n_words;
	  machine_mode part_mode;

	  /* Vector parameters must be 16-byte aligned.  In 32-bit
	     mode this means we need to take into account the offset
	     to the parameter save area.  In 64-bit mode, they just
	     have to start on an even word, since the parameter save
	     area is 16-byte aligned.  */
	  if (TARGET_32BIT)
	    align = -(rs6000_parm_offset () + cum->words) & 3;
	  else
	    align = cum->words & 1;
	  align_words = cum->words + align;

	  /* Out of registers?  Memory, then.  */
	  if (align_words >= GP_ARG_NUM_REG)
	    return NULL_RTX;

	  if (TARGET_32BIT && TARGET_POWERPC64)
	    return rs6000_mixed_function_arg (mode, type, align_words);

	  /* The vector value goes in GPRs.  Only the part of the
	     value in GPRs is reported here.  */
	  part_mode = mode;
	  n_words = rs6000_arg_size (mode, type);
	  if (align_words + n_words > GP_ARG_NUM_REG)
	    /* Fortunately, there are only two possibilities, the value
	       is either wholly in GPRs or half in GPRs and half not.  */
	    part_mode = DImode;

	  return gen_rtx_REG (part_mode, GP_ARG_MIN_REG + align_words);
	}
    }

  else if (abi == ABI_V4)
    {
      if (abi_v4_pass_in_fpr (mode, named))
	{
	  /* _Decimal128 must use an even/odd register pair.  This assumes
	     that the register number is odd when fregno is odd.  */
	  if (mode == TDmode && (cum->fregno % 2) == 1)
	    cum->fregno++;

	  if (cum->fregno + (FLOAT128_2REG_P (mode) ? 1 : 0)
	      <= FP_ARG_V4_MAX_REG)
	    return gen_rtx_REG (mode, cum->fregno);
	  else
	    return NULL_RTX;
	}
      else
	{
	  int n_words = rs6000_arg_size (mode, type);
	  int gregno = cum->sysv_gregno;

	  /* Long long is put in (r3,r4), (r5,r6), (r7,r8) or (r9,r10).
	     As does any other 2 word item such as complex int due to a
	     historical mistake.  */
	  if (n_words == 2)
	    gregno += (1 - gregno) & 1;

	  /* Multi-reg args are not split between registers and stack.  */
	  if (gregno + n_words - 1 > GP_ARG_MAX_REG)
	    return NULL_RTX;

	  if (TARGET_32BIT && TARGET_POWERPC64)
	    return rs6000_mixed_function_arg (mode, type,
					      gregno - GP_ARG_MIN_REG);
	  return gen_rtx_REG (mode, gregno);
	}
    }
  else
    {
      int align_words = rs6000_parm_start (mode, type, cum->words);

      /* _Decimal128 must be passed in an even/odd float register pair.
	 This assumes that the register number is odd when fregno is odd.  */
      if (elt_mode == TDmode && (cum->fregno % 2) == 1)
	cum->fregno++;

      if (USE_FP_FOR_ARG_P (cum, elt_mode)
	  && !(TARGET_AIX && !TARGET_ELF
	       && type != NULL && AGGREGATE_TYPE_P (type)))
	{
	  rtx rvec[GP_ARG_NUM_REG + AGGR_ARG_NUM_REG + 1];
	  rtx r, off;
	  int i, k = 0;
	  unsigned long n_fpreg = (GET_MODE_SIZE (elt_mode) + 7) >> 3;
	  int fpr_words;

	  /* Do we also need to pass this argument in the parameter
	     save area?  */
	  if (type && (cum->nargs_prototype <= 0
		       || ((DEFAULT_ABI == ABI_AIX || DEFAULT_ABI == ABI_ELFv2)
			   && TARGET_XL_COMPAT
			   && align_words >= GP_ARG_NUM_REG)))
	    k = rs6000_psave_function_arg (mode, type, align_words, rvec);

	  /* Describe where this argument goes in the fprs.  */
	  for (i = 0; i < n_elts
		      && cum->fregno + i * n_fpreg <= FP_ARG_MAX_REG; i++)
	    {
	      /* Check if the argument is split over registers and memory.
		 This can only ever happen for long double or _Decimal128;
		 complex types are handled via split_complex_arg.  */
	      machine_mode fmode = elt_mode;
	      if (cum->fregno + (i + 1) * n_fpreg > FP_ARG_MAX_REG + 1)
		{
		  gcc_assert (FLOAT128_2REG_P (fmode));
		  fmode = DECIMAL_FLOAT_MODE_P (fmode) ? DDmode : DFmode;
		}

	      r = gen_rtx_REG (fmode, cum->fregno + i * n_fpreg);
	      off = GEN_INT (i * GET_MODE_SIZE (elt_mode));
	      rvec[k++] = gen_rtx_EXPR_LIST (VOIDmode, r, off);
	    }

	  /* If there were not enough FPRs to hold the argument, the rest
	     usually goes into memory.  However, if the current position
	     is still within the register parameter area, a portion may
	     actually have to go into GPRs.

	     Note that it may happen that the portion of the argument
	     passed in the first "half" of the first GPR was already
	     passed in the last FPR as well.

	     For unnamed arguments, we already set up GPRs to cover the
	     whole argument in rs6000_psave_function_arg, so there is
	     nothing further to do at this point.  */
	  fpr_words = (i * GET_MODE_SIZE (elt_mode)) / (TARGET_32BIT ? 4 : 8);
	  if (i < n_elts && align_words + fpr_words < GP_ARG_NUM_REG
	      && cum->nargs_prototype > 0)
            {
	      static bool warned;

	      machine_mode rmode = TARGET_32BIT ? SImode : DImode;
	      int n_words = rs6000_arg_size (mode, type);

	      align_words += fpr_words;
	      n_words -= fpr_words;

	      do
		{
		  r = gen_rtx_REG (rmode, GP_ARG_MIN_REG + align_words);
		  off = GEN_INT (fpr_words++ * GET_MODE_SIZE (rmode));
		  rvec[k++] = gen_rtx_EXPR_LIST (VOIDmode, r, off);
		}
	      while (++align_words < GP_ARG_NUM_REG && --n_words != 0);

	      if (!warned && warn_psabi)
		{
		  warned = true;
		  inform (input_location,
			  "the ABI of passing homogeneous %<float%> aggregates"
			  " has changed in GCC 5");
		}
	    }

	  return rs6000_finish_function_arg (mode, rvec, k);
	}
      else if (align_words < GP_ARG_NUM_REG)
	{
	  if (TARGET_32BIT && TARGET_POWERPC64)
	    return rs6000_mixed_function_arg (mode, type, align_words);

	  return gen_rtx_REG (mode, GP_ARG_MIN_REG + align_words);
	}
      else
	return NULL_RTX;
    }
}

/* For an arg passed partly in registers and partly in memory, this is
   the number of bytes passed in registers.  For args passed entirely in
   registers or entirely in memory, zero.  When an arg is described by a
   PARALLEL, perhaps using more than one register type, this function
   returns the number of bytes used by the first element of the PARALLEL.  */

int
rs6000_arg_partial_bytes (cumulative_args_t cum_v,
			  const function_arg_info &arg)
{
  CUMULATIVE_ARGS *cum = get_cumulative_args (cum_v);
  bool passed_in_gprs = true;
  int ret = 0;
  int align_words;
  machine_mode elt_mode;
  int n_elts;

  rs6000_discover_homogeneous_aggregate (arg.mode, arg.type,
					 &elt_mode, &n_elts);

  if (DEFAULT_ABI == ABI_V4)
    return 0;

  if (USE_ALTIVEC_FOR_ARG_P (cum, elt_mode, arg.named))
    {
      /* If we are passing this arg in the fixed parameter save area (gprs or
         memory) as well as VRs, we do not use the partial bytes mechanism;
         instead, rs6000_function_arg will return a PARALLEL including a memory
         element as necessary.  Library support functions for IEEE 128-bit are
         assumed to not need the value passed both in GPRs and in vector
         registers.  */
      if (TARGET_64BIT && !cum->prototype
	  && (!cum->libcall || !FLOAT128_VECTOR_P (elt_mode)))
	return 0;

      /* Otherwise, we pass in VRs only.  Check for partial copies.  */
      passed_in_gprs = false;
      if (cum->vregno + n_elts > ALTIVEC_ARG_MAX_REG + 1)
	ret = (ALTIVEC_ARG_MAX_REG + 1 - cum->vregno) * 16;
    }

  /* In this complicated case we just disable the partial_nregs code.  */
  if (TARGET_MACHO && rs6000_darwin64_struct_check_p (arg.mode, arg.type))
    return 0;

  align_words = rs6000_parm_start (arg.mode, arg.type, cum->words);

  if (USE_FP_FOR_ARG_P (cum, elt_mode)
      && !(TARGET_AIX && !TARGET_ELF && arg.aggregate_type_p ()))
    {
      unsigned long n_fpreg = (GET_MODE_SIZE (elt_mode) + 7) >> 3;

      /* If we are passing this arg in the fixed parameter save area
         (gprs or memory) as well as FPRs, we do not use the partial
	 bytes mechanism; instead, rs6000_function_arg will return a
	 PARALLEL including a memory element as necessary.  */
      if (arg.type
	  && (cum->nargs_prototype <= 0
	      || ((DEFAULT_ABI == ABI_AIX || DEFAULT_ABI == ABI_ELFv2)
		  && TARGET_XL_COMPAT
		  && align_words >= GP_ARG_NUM_REG)))
	return 0;

      /* Otherwise, we pass in FPRs only.  Check for partial copies.  */
      passed_in_gprs = false;
      if (cum->fregno + n_elts * n_fpreg > FP_ARG_MAX_REG + 1)
	{
	  /* Compute number of bytes / words passed in FPRs.  If there
	     is still space available in the register parameter area
	     *after* that amount, a part of the argument will be passed
	     in GPRs.  In that case, the total amount passed in any
	     registers is equal to the amount that would have been passed
	     in GPRs if everything were passed there, so we fall back to
	     the GPR code below to compute the appropriate value.  */
	  int fpr = ((FP_ARG_MAX_REG + 1 - cum->fregno)
		     * MIN (8, GET_MODE_SIZE (elt_mode)));
	  int fpr_words = fpr / (TARGET_32BIT ? 4 : 8);

	  if (align_words + fpr_words < GP_ARG_NUM_REG)
	    passed_in_gprs = true;
	  else
	    ret = fpr;
	}
    }

  if (passed_in_gprs
      && align_words < GP_ARG_NUM_REG
      && GP_ARG_NUM_REG < align_words + rs6000_arg_size (arg.mode, arg.type))
    ret = (GP_ARG_NUM_REG - align_words) * (TARGET_32BIT ? 4 : 8);

  if (ret != 0 && TARGET_DEBUG_ARG)
    fprintf (stderr, "rs6000_arg_partial_bytes: %d\n", ret);

  return ret;
}

/* A C expression that indicates when an argument must be passed by
   reference.  If nonzero for an argument, a copy of that argument is
   made in memory and a pointer to the argument is passed instead of
   the argument itself.  The pointer is passed in whatever way is
   appropriate for passing a pointer to that type.

   Under V.4, aggregates and long double are passed by reference.

   As an extension to all 32-bit ABIs, AltiVec vectors are passed by
   reference unless the AltiVec vector extension ABI is in force.

   As an extension to all ABIs, variable sized types are passed by
   reference.  */

bool
rs6000_pass_by_reference (cumulative_args_t, const function_arg_info &arg)
{
  if (!arg.type)
    return 0;

  if (DEFAULT_ABI == ABI_V4 && TARGET_IEEEQUAD
      && FLOAT128_IEEE_P (TYPE_MODE (arg.type)))
    {
      if (TARGET_DEBUG_ARG)
	fprintf (stderr, "function_arg_pass_by_reference: V4 IEEE 128-bit\n");
      return 1;
    }

  if (DEFAULT_ABI == ABI_V4 && AGGREGATE_TYPE_P (arg.type))
    {
      if (TARGET_DEBUG_ARG)
	fprintf (stderr, "function_arg_pass_by_reference: V4 aggregate\n");
      return 1;
    }

  if (int_size_in_bytes (arg.type) < 0)
    {
      if (TARGET_DEBUG_ARG)
	fprintf (stderr, "function_arg_pass_by_reference: variable size\n");
      return 1;
    }

  /* Allow -maltivec -mabi=no-altivec without warning.  Altivec vector
     modes only exist for GCC vector types if -maltivec.  */
  if (TARGET_32BIT && !TARGET_ALTIVEC_ABI && ALTIVEC_VECTOR_MODE (arg.mode))
    {
      if (TARGET_DEBUG_ARG)
	fprintf (stderr, "function_arg_pass_by_reference: AltiVec\n");
      return 1;
    }

  /* Pass synthetic vectors in memory.  */
  if (TREE_CODE (arg.type) == VECTOR_TYPE
      && int_size_in_bytes (arg.type) > (TARGET_ALTIVEC_ABI ? 16 : 8))
    {
      static bool warned_for_pass_big_vectors = false;
      if (TARGET_DEBUG_ARG)
	fprintf (stderr, "function_arg_pass_by_reference: synthetic vector\n");
      if (!warned_for_pass_big_vectors)
	{
	  warning (OPT_Wpsabi, "GCC vector passed by reference: "
		   "non-standard ABI extension with no compatibility "
		   "guarantee");
	  warned_for_pass_big_vectors = true;
	}
      return 1;
    }

  return 0;
}

/* Process parameter of type TYPE after ARGS_SO_FAR parameters were
   already processes.  Return true if the parameter must be passed
   (fully or partially) on the stack.  */

static bool
rs6000_parm_needs_stack (cumulative_args_t args_so_far, tree type)
{
  int unsignedp;
  rtx entry_parm;

  /* Catch errors.  */
  if (type == NULL || type == error_mark_node)
    return true;

  /* Handle types with no storage requirement.  */
  if (TYPE_MODE (type) == VOIDmode)
    return false;

  /* Handle complex types.  */
  if (TREE_CODE (type) == COMPLEX_TYPE)
    return (rs6000_parm_needs_stack (args_so_far, TREE_TYPE (type))
	    || rs6000_parm_needs_stack (args_so_far, TREE_TYPE (type)));

  /* Handle transparent aggregates.  */
  if ((TREE_CODE (type) == UNION_TYPE || TREE_CODE (type) == RECORD_TYPE)
      && TYPE_TRANSPARENT_AGGR (type))
    type = TREE_TYPE (first_field (type));

  /* See if this arg was passed by invisible reference.  */
  function_arg_info arg (type, /*named=*/true);
  apply_pass_by_reference_rules (get_cumulative_args (args_so_far), arg);

  /* Find mode as it is passed by the ABI.  */
  unsignedp = TYPE_UNSIGNED (type);
  arg.mode = promote_mode (arg.type, arg.mode, &unsignedp);

  /* If we must pass in stack, we need a stack.  */
  if (rs6000_must_pass_in_stack (arg))
    return true;

  /* If there is no incoming register, we need a stack.  */
  entry_parm = rs6000_function_arg (args_so_far, arg);
  if (entry_parm == NULL)
    return true;

  /* Likewise if we need to pass both in registers and on the stack.  */
  if (GET_CODE (entry_parm) == PARALLEL
      && XEXP (XVECEXP (entry_parm, 0, 0), 0) == NULL_RTX)
    return true;

  /* Also true if we're partially in registers and partially not.  */
  if (rs6000_arg_partial_bytes (args_so_far, arg) != 0)
    return true;

  /* Update info on where next arg arrives in registers.  */
  rs6000_function_arg_advance (args_so_far, arg);
  return false;
}

/* Return true if FUN has no prototype, has a variable argument
   list, or passes any parameter in memory.  */

static bool
rs6000_function_parms_need_stack (tree fun, bool incoming)
{
  tree fntype, result;
  CUMULATIVE_ARGS args_so_far_v;
  cumulative_args_t args_so_far;

  if (!fun)
    /* Must be a libcall, all of which only use reg parms.  */
    return false;

  fntype = fun;
  if (!TYPE_P (fun))
    fntype = TREE_TYPE (fun);

  /* Varargs functions need the parameter save area.  */
  if ((!incoming && !prototype_p (fntype)) || stdarg_p (fntype))
    return true;

  INIT_CUMULATIVE_INCOMING_ARGS (args_so_far_v, fntype, NULL_RTX);
  args_so_far = pack_cumulative_args (&args_so_far_v);

  /* When incoming, we will have been passed the function decl.
     It is necessary to use the decl to handle K&R style functions,
     where TYPE_ARG_TYPES may not be available.  */
  if (incoming)
    {
      gcc_assert (DECL_P (fun));
      result = DECL_RESULT (fun);
    }
  else
    result = TREE_TYPE (fntype);

  if (result && aggregate_value_p (result, fntype))
    {
      if (!TYPE_P (result))
	result = TREE_TYPE (result);
      result = build_pointer_type (result);
      rs6000_parm_needs_stack (args_so_far, result);
    }

  if (incoming)
    {
      tree parm;

      for (parm = DECL_ARGUMENTS (fun);
	   parm && parm != void_list_node;
	   parm = TREE_CHAIN (parm))
	if (rs6000_parm_needs_stack (args_so_far, TREE_TYPE (parm)))
	  return true;
    }
  else
    {
      function_args_iterator args_iter;
      tree arg_type;

      FOREACH_FUNCTION_ARGS (fntype, arg_type, args_iter)
	if (rs6000_parm_needs_stack (args_so_far, arg_type))
	  return true;
    }

  return false;
}

/* Return the size of the REG_PARM_STACK_SPACE are for FUN.  This is
   usually a constant depending on the ABI.  However, in the ELFv2 ABI
   the register parameter area is optional when calling a function that
   has a prototype is scope, has no variable argument list, and passes
   all parameters in registers.  */

int
rs6000_reg_parm_stack_space (tree fun, bool incoming)
{
  int reg_parm_stack_space;

  switch (DEFAULT_ABI)
    {
    default:
      reg_parm_stack_space = 0;
      break;

    case ABI_AIX:
    case ABI_DARWIN:
      reg_parm_stack_space = TARGET_64BIT ? 64 : 32;
      break;

    case ABI_ELFv2:
      /* ??? Recomputing this every time is a bit expensive.  Is there
	 a place to cache this information?  */
      if (rs6000_function_parms_need_stack (fun, incoming))
	reg_parm_stack_space = TARGET_64BIT ? 64 : 32;
      else
	reg_parm_stack_space = 0;
      break;
    }

  return reg_parm_stack_space;
}

static void
rs6000_move_block_from_reg (int regno, rtx x, int nregs)
{
  int i;
  machine_mode reg_mode = TARGET_32BIT ? SImode : DImode;

  if (nregs == 0)
    return;

  for (i = 0; i < nregs; i++)
    {
      rtx tem = adjust_address_nv (x, reg_mode, i * GET_MODE_SIZE (reg_mode));
      if (reload_completed)
	{
	  if (! strict_memory_address_p (reg_mode, XEXP (tem, 0)))
	    tem = NULL_RTX;
	  else
	    tem = simplify_gen_subreg (reg_mode, x, BLKmode,
				       i * GET_MODE_SIZE (reg_mode));
	}
      else
	tem = replace_equiv_address (tem, XEXP (tem, 0));

      gcc_assert (tem);

      emit_move_insn (tem, gen_rtx_REG (reg_mode, regno + i));
    }
}

/* Perform any needed actions needed for a function that is receiving a
   variable number of arguments.

   CUM is as above.

   ARG is the last named argument.

   PRETEND_SIZE is a variable that should be set to the amount of stack
   that must be pushed by the prolog to pretend that our caller pushed
   it.

   Normally, this macro will push all remaining incoming registers on the
   stack and set PRETEND_SIZE to the length of the registers pushed.  */

void
setup_incoming_varargs (cumulative_args_t cum,
			const function_arg_info &arg,
			int *pretend_size ATTRIBUTE_UNUSED, int no_rtl)
{
  CUMULATIVE_ARGS next_cum;
  int reg_size = TARGET_32BIT ? 4 : 8;
  rtx save_area = NULL_RTX, mem;
  int first_reg_offset;
  alias_set_type set;

  /* Skip the last named argument.  */
  next_cum = *get_cumulative_args (cum);
  rs6000_function_arg_advance_1 (&next_cum, arg.mode, arg.type, arg.named, 0);

  if (DEFAULT_ABI == ABI_V4)
    {
      first_reg_offset = next_cum.sysv_gregno - GP_ARG_MIN_REG;

      if (! no_rtl)
	{
	  int gpr_reg_num = 0, gpr_size = 0, fpr_size = 0;
	  HOST_WIDE_INT offset = 0;

	  /* Try to optimize the size of the varargs save area.
	     The ABI requires that ap.reg_save_area is doubleword
	     aligned, but we don't need to allocate space for all
	     the bytes, only those to which we actually will save
	     anything.  */
	  if (cfun->va_list_gpr_size && first_reg_offset < GP_ARG_NUM_REG)
	    gpr_reg_num = GP_ARG_NUM_REG - first_reg_offset;
	  if (TARGET_HARD_FLOAT
	      && next_cum.fregno <= FP_ARG_V4_MAX_REG
	      && cfun->va_list_fpr_size)
	    {
	      if (gpr_reg_num)
		fpr_size = (next_cum.fregno - FP_ARG_MIN_REG)
			   * UNITS_PER_FP_WORD;
	      if (cfun->va_list_fpr_size
		  < FP_ARG_V4_MAX_REG + 1 - next_cum.fregno)
		fpr_size += cfun->va_list_fpr_size * UNITS_PER_FP_WORD;
	      else
		fpr_size += (FP_ARG_V4_MAX_REG + 1 - next_cum.fregno)
			    * UNITS_PER_FP_WORD;
	    }
	  if (gpr_reg_num)
	    {
	      offset = -((first_reg_offset * reg_size) & ~7);
	      if (!fpr_size && gpr_reg_num > cfun->va_list_gpr_size)
		{
		  gpr_reg_num = cfun->va_list_gpr_size;
		  if (reg_size == 4 && (first_reg_offset & 1))
		    gpr_reg_num++;
		}
	      gpr_size = (gpr_reg_num * reg_size + 7) & ~7;
	    }
	  else if (fpr_size)
	    offset = - (int) (next_cum.fregno - FP_ARG_MIN_REG)
		       * UNITS_PER_FP_WORD
		     - (int) (GP_ARG_NUM_REG * reg_size);

	  if (gpr_size + fpr_size)
	    {
	      rtx reg_save_area
		= assign_stack_local (BLKmode, gpr_size + fpr_size, 64);
	      gcc_assert (MEM_P (reg_save_area));
	      reg_save_area = XEXP (reg_save_area, 0);
	      if (GET_CODE (reg_save_area) == PLUS)
		{
		  gcc_assert (XEXP (reg_save_area, 0)
			      == virtual_stack_vars_rtx);
		  gcc_assert (CONST_INT_P (XEXP (reg_save_area, 1)));
		  offset += INTVAL (XEXP (reg_save_area, 1));
		}
	      else
		gcc_assert (reg_save_area == virtual_stack_vars_rtx);
	    }

	  cfun->machine->varargs_save_offset = offset;
	  save_area = plus_constant (Pmode, virtual_stack_vars_rtx, offset);
	}
    }
  else
    {
      first_reg_offset = next_cum.words;
      save_area = crtl->args.internal_arg_pointer;

      if (targetm.calls.must_pass_in_stack (arg))
	first_reg_offset += rs6000_arg_size (TYPE_MODE (arg.type), arg.type);
    }

  set = get_varargs_alias_set ();
  if (! no_rtl && first_reg_offset < GP_ARG_NUM_REG
      && cfun->va_list_gpr_size)
    {
      int n_gpr, nregs = GP_ARG_NUM_REG - first_reg_offset;

      if (va_list_gpr_counter_field)
	/* V4 va_list_gpr_size counts number of registers needed.  */
	n_gpr = cfun->va_list_gpr_size;
      else
	/* char * va_list instead counts number of bytes needed.  */
	n_gpr = (cfun->va_list_gpr_size + reg_size - 1) / reg_size;

      if (nregs > n_gpr)
	nregs = n_gpr;

      mem = gen_rtx_MEM (BLKmode,
			 plus_constant (Pmode, save_area,
					first_reg_offset * reg_size));
      MEM_NOTRAP_P (mem) = 1;
      set_mem_alias_set (mem, set);
      set_mem_align (mem, BITS_PER_WORD);

      rs6000_move_block_from_reg (GP_ARG_MIN_REG + first_reg_offset, mem,
				  nregs);
    }

  /* Save FP registers if needed.  */
  if (DEFAULT_ABI == ABI_V4
      && TARGET_HARD_FLOAT
      && ! no_rtl
      && next_cum.fregno <= FP_ARG_V4_MAX_REG
      && cfun->va_list_fpr_size)
    {
      int fregno = next_cum.fregno, nregs;
      rtx cr1 = gen_rtx_REG (CCmode, CR1_REGNO);
      rtx lab = gen_label_rtx ();
      int off = (GP_ARG_NUM_REG * reg_size) + ((fregno - FP_ARG_MIN_REG)
					       * UNITS_PER_FP_WORD);

      emit_jump_insn
	(gen_rtx_SET (pc_rtx,
		      gen_rtx_IF_THEN_ELSE (VOIDmode,
					    gen_rtx_NE (VOIDmode, cr1,
							const0_rtx),
					    gen_rtx_LABEL_REF (VOIDmode, lab),
					    pc_rtx)));

      for (nregs = 0;
	   fregno <= FP_ARG_V4_MAX_REG && nregs < cfun->va_list_fpr_size;
	   fregno++, off += UNITS_PER_FP_WORD, nregs++)
	{
	  mem = gen_rtx_MEM (TARGET_HARD_FLOAT ? DFmode : SFmode,
                             plus_constant (Pmode, save_area, off));
  	  MEM_NOTRAP_P (mem) = 1;
  	  set_mem_alias_set (mem, set);
	  set_mem_align (mem, GET_MODE_ALIGNMENT (
			 TARGET_HARD_FLOAT ? DFmode : SFmode));
	  emit_move_insn (mem, gen_rtx_REG (
                          TARGET_HARD_FLOAT ? DFmode : SFmode, fregno));
	}

      emit_label (lab);
    }
}

/* Create the va_list data type.  */

tree
rs6000_build_builtin_va_list (void)
{
  tree f_gpr, f_fpr, f_res, f_ovf, f_sav, record, type_decl;

  /* For AIX, prefer 'char *' because that's what the system
     header files like.  */
  if (DEFAULT_ABI != ABI_V4)
    return build_pointer_type (char_type_node);

  record = (*lang_hooks.types.make_type) (RECORD_TYPE);
  type_decl = build_decl (BUILTINS_LOCATION, TYPE_DECL,
      			  get_identifier ("__va_list_tag"), record);

  f_gpr = build_decl (BUILTINS_LOCATION, FIELD_DECL, get_identifier ("gpr"),
		      unsigned_char_type_node);
  f_fpr = build_decl (BUILTINS_LOCATION, FIELD_DECL, get_identifier ("fpr"),
		      unsigned_char_type_node);
  /* Give the two bytes of padding a name, so that -Wpadded won't warn on
     every user file.  */
  f_res = build_decl (BUILTINS_LOCATION, FIELD_DECL,
      		      get_identifier ("reserved"), short_unsigned_type_node);
  f_ovf = build_decl (BUILTINS_LOCATION, FIELD_DECL,
      		      get_identifier ("overflow_arg_area"),
		      ptr_type_node);
  f_sav = build_decl (BUILTINS_LOCATION, FIELD_DECL,
      		      get_identifier ("reg_save_area"),
		      ptr_type_node);

  va_list_gpr_counter_field = f_gpr;
  va_list_fpr_counter_field = f_fpr;

  DECL_FIELD_CONTEXT (f_gpr) = record;
  DECL_FIELD_CONTEXT (f_fpr) = record;
  DECL_FIELD_CONTEXT (f_res) = record;
  DECL_FIELD_CONTEXT (f_ovf) = record;
  DECL_FIELD_CONTEXT (f_sav) = record;

  TYPE_STUB_DECL (record) = type_decl;
  TYPE_NAME (record) = type_decl;
  TYPE_FIELDS (record) = f_gpr;
  DECL_CHAIN (f_gpr) = f_fpr;
  DECL_CHAIN (f_fpr) = f_res;
  DECL_CHAIN (f_res) = f_ovf;
  DECL_CHAIN (f_ovf) = f_sav;

  layout_type (record);

  /* The correct type is an array type of one element.  */
  return build_array_type (record, build_index_type (size_zero_node));
}

/* Implement va_start.  */

void
rs6000_va_start (tree valist, rtx nextarg)
{
  HOST_WIDE_INT words, n_gpr, n_fpr;
  tree f_gpr, f_fpr, f_res, f_ovf, f_sav;
  tree gpr, fpr, ovf, sav, t;

  /* Only SVR4 needs something special.  */
  if (DEFAULT_ABI != ABI_V4)
    {
      std_expand_builtin_va_start (valist, nextarg);
      return;
    }

  f_gpr = TYPE_FIELDS (TREE_TYPE (va_list_type_node));
  f_fpr = DECL_CHAIN (f_gpr);
  f_res = DECL_CHAIN (f_fpr);
  f_ovf = DECL_CHAIN (f_res);
  f_sav = DECL_CHAIN (f_ovf);

  valist = build_simple_mem_ref (valist);
  gpr = build3 (COMPONENT_REF, TREE_TYPE (f_gpr), valist, f_gpr, NULL_TREE);
  fpr = build3 (COMPONENT_REF, TREE_TYPE (f_fpr), unshare_expr (valist),
		f_fpr, NULL_TREE);
  ovf = build3 (COMPONENT_REF, TREE_TYPE (f_ovf), unshare_expr (valist),
		f_ovf, NULL_TREE);
  sav = build3 (COMPONENT_REF, TREE_TYPE (f_sav), unshare_expr (valist),
		f_sav, NULL_TREE);

  /* Count number of gp and fp argument registers used.  */
  words = crtl->args.info.words;
  n_gpr = MIN (crtl->args.info.sysv_gregno - GP_ARG_MIN_REG,
	       GP_ARG_NUM_REG);
  n_fpr = MIN (crtl->args.info.fregno - FP_ARG_MIN_REG,
	       FP_ARG_NUM_REG);

  if (TARGET_DEBUG_ARG)
    fprintf (stderr, "va_start: words = " HOST_WIDE_INT_PRINT_DEC", n_gpr = "
	     HOST_WIDE_INT_PRINT_DEC", n_fpr = " HOST_WIDE_INT_PRINT_DEC"\n",
	     words, n_gpr, n_fpr);

  if (cfun->va_list_gpr_size)
    {
      t = build2 (MODIFY_EXPR, TREE_TYPE (gpr), gpr,
		  build_int_cst (NULL_TREE, n_gpr));
      TREE_SIDE_EFFECTS (t) = 1;
      expand_expr (t, const0_rtx, VOIDmode, EXPAND_NORMAL);
    }

  if (cfun->va_list_fpr_size)
    {
      t = build2 (MODIFY_EXPR, TREE_TYPE (fpr), fpr,
		  build_int_cst (NULL_TREE, n_fpr));
      TREE_SIDE_EFFECTS (t) = 1;
      expand_expr (t, const0_rtx, VOIDmode, EXPAND_NORMAL);

#ifdef HAVE_AS_GNU_ATTRIBUTE
      if (call_ABI_of_interest (cfun->decl))
	rs6000_passes_float = true;
#endif
    }

  /* Find the overflow area.  */
  t = make_tree (TREE_TYPE (ovf), crtl->args.internal_arg_pointer);
  if (words != 0)
    t = fold_build_pointer_plus_hwi (t, words * MIN_UNITS_PER_WORD);
  t = build2 (MODIFY_EXPR, TREE_TYPE (ovf), ovf, t);
  TREE_SIDE_EFFECTS (t) = 1;
  expand_expr (t, const0_rtx, VOIDmode, EXPAND_NORMAL);

  /* If there were no va_arg invocations, don't set up the register
     save area.  */
  if (!cfun->va_list_gpr_size
      && !cfun->va_list_fpr_size
      && n_gpr < GP_ARG_NUM_REG
      && n_fpr < FP_ARG_V4_MAX_REG)
    return;

  /* Find the register save area.  */
  t = make_tree (TREE_TYPE (sav), virtual_stack_vars_rtx);
  if (cfun->machine->varargs_save_offset)
    t = fold_build_pointer_plus_hwi (t, cfun->machine->varargs_save_offset);
  t = build2 (MODIFY_EXPR, TREE_TYPE (sav), sav, t);
  TREE_SIDE_EFFECTS (t) = 1;
  expand_expr (t, const0_rtx, VOIDmode, EXPAND_NORMAL);
}

/* Implement va_arg.  */

tree
rs6000_gimplify_va_arg (tree valist, tree type, gimple_seq *pre_p,
			gimple_seq *post_p)
{
  tree f_gpr, f_fpr, f_res, f_ovf, f_sav;
  tree gpr, fpr, ovf, sav, reg, t, u;
  int size, rsize, n_reg, sav_ofs, sav_scale;
  tree lab_false, lab_over, addr;
  int align;
  tree ptrtype = build_pointer_type_for_mode (type, ptr_mode, true);
  int regalign = 0;
  gimple *stmt;

  if (pass_va_arg_by_reference (type))
    {
      t = rs6000_gimplify_va_arg (valist, ptrtype, pre_p, post_p);
      return build_va_arg_indirect_ref (t);
    }

  /* We need to deal with the fact that the darwin ppc64 ABI is defined by an
     earlier version of gcc, with the property that it always applied alignment
     adjustments to the va-args (even for zero-sized types).  The cheapest way
     to deal with this is to replicate the effect of the part of 
     std_gimplify_va_arg_expr that carries out the align adjust, for the case 
     of relevance.  
     We don't need to check for pass-by-reference because of the test above.
     We can return a simplifed answer, since we know there's no offset to add.  */

  if (((TARGET_MACHO
        && rs6000_darwin64_abi)
       || DEFAULT_ABI == ABI_ELFv2
       || (DEFAULT_ABI == ABI_AIX && !rs6000_compat_align_parm))
      && integer_zerop (TYPE_SIZE (type)))
    {
      unsigned HOST_WIDE_INT align, boundary;
      tree valist_tmp = get_initialized_tmp_var (valist, pre_p, NULL);
      align = PARM_BOUNDARY / BITS_PER_UNIT;
      boundary = rs6000_function_arg_boundary (TYPE_MODE (type), type);
      if (boundary > MAX_SUPPORTED_STACK_ALIGNMENT)
	boundary = MAX_SUPPORTED_STACK_ALIGNMENT;
      boundary /= BITS_PER_UNIT;
      if (boundary > align)
	{
	  tree t ;
	  /* This updates arg ptr by the amount that would be necessary
	     to align the zero-sized (but not zero-alignment) item.  */
	  t = build2 (MODIFY_EXPR, TREE_TYPE (valist), valist_tmp,
		      fold_build_pointer_plus_hwi (valist_tmp, boundary - 1));
	  gimplify_and_add (t, pre_p);

	  t = fold_convert (sizetype, valist_tmp);
	  t = build2 (MODIFY_EXPR, TREE_TYPE (valist), valist_tmp,
		  fold_convert (TREE_TYPE (valist),
				fold_build2 (BIT_AND_EXPR, sizetype, t,
					     size_int (-boundary))));
	  t = build2 (MODIFY_EXPR, TREE_TYPE (valist), valist, t);
	  gimplify_and_add (t, pre_p);
	}
      /* Since it is zero-sized there's no increment for the item itself. */
      valist_tmp = fold_convert (build_pointer_type (type), valist_tmp);
      return build_va_arg_indirect_ref (valist_tmp);
    }

  if (DEFAULT_ABI != ABI_V4)
    {
      if (targetm.calls.split_complex_arg && TREE_CODE (type) == COMPLEX_TYPE)
	{
	  tree elem_type = TREE_TYPE (type);
	  machine_mode elem_mode = TYPE_MODE (elem_type);
	  int elem_size = GET_MODE_SIZE (elem_mode);

	  if (elem_size < UNITS_PER_WORD)
	    {
	      tree real_part, imag_part;
	      gimple_seq post = NULL;

	      real_part = rs6000_gimplify_va_arg (valist, elem_type, pre_p,
						  &post);
	      /* Copy the value into a temporary, lest the formal temporary
		 be reused out from under us.  */
	      real_part = get_initialized_tmp_var (real_part, pre_p, &post);
	      gimple_seq_add_seq (pre_p, post);

	      imag_part = rs6000_gimplify_va_arg (valist, elem_type, pre_p,
						  post_p);

	      return build2 (COMPLEX_EXPR, type, real_part, imag_part);
	    }
	}

      return std_gimplify_va_arg_expr (valist, type, pre_p, post_p);
    }

  f_gpr = TYPE_FIELDS (TREE_TYPE (va_list_type_node));
  f_fpr = DECL_CHAIN (f_gpr);
  f_res = DECL_CHAIN (f_fpr);
  f_ovf = DECL_CHAIN (f_res);
  f_sav = DECL_CHAIN (f_ovf);

  gpr = build3 (COMPONENT_REF, TREE_TYPE (f_gpr), valist, f_gpr, NULL_TREE);
  fpr = build3 (COMPONENT_REF, TREE_TYPE (f_fpr), unshare_expr (valist),
		f_fpr, NULL_TREE);
  ovf = build3 (COMPONENT_REF, TREE_TYPE (f_ovf), unshare_expr (valist),
		f_ovf, NULL_TREE);
  sav = build3 (COMPONENT_REF, TREE_TYPE (f_sav), unshare_expr (valist),
		f_sav, NULL_TREE);

  size = int_size_in_bytes (type);
  rsize = (size + 3) / 4;
  int pad = 4 * rsize - size;
  align = 1;

  machine_mode mode = TYPE_MODE (type);
  if (abi_v4_pass_in_fpr (mode, false))
    {
      /* FP args go in FP registers, if present.  */
      reg = fpr;
      n_reg = (size + 7) / 8;
      sav_ofs = (TARGET_HARD_FLOAT ? 8 : 4) * 4;
      sav_scale = (TARGET_HARD_FLOAT ? 8 : 4);
      if (mode != SFmode && mode != SDmode)
	align = 8;
    }
  else
    {
      /* Otherwise into GP registers.  */
      reg = gpr;
      n_reg = rsize;
      sav_ofs = 0;
      sav_scale = 4;
      if (n_reg == 2)
	align = 8;
    }

  /* Pull the value out of the saved registers....  */

  lab_over = NULL;
  addr = create_tmp_var (ptr_type_node, "addr");

  /*  AltiVec vectors never go in registers when -mabi=altivec.  */
  if (TARGET_ALTIVEC_ABI && ALTIVEC_VECTOR_MODE (mode))
    align = 16;
  else
    {
      lab_false = create_artificial_label (input_location);
      lab_over = create_artificial_label (input_location);

      /* Long long is aligned in the registers.  As are any other 2 gpr
	 item such as complex int due to a historical mistake.  */
      u = reg;
      if (n_reg == 2 && reg == gpr)
	{
	  regalign = 1;
	  u = build2 (BIT_AND_EXPR, TREE_TYPE (reg), unshare_expr (reg),
		     build_int_cst (TREE_TYPE (reg), n_reg - 1));
	  u = build2 (POSTINCREMENT_EXPR, TREE_TYPE (reg),
		      unshare_expr (reg), u);
	}
      /* _Decimal128 is passed in even/odd fpr pairs; the stored
	 reg number is 0 for f1, so we want to make it odd.  */
      else if (reg == fpr && mode == TDmode)
	{
	  t = build2 (BIT_IOR_EXPR, TREE_TYPE (reg), unshare_expr (reg),
		      build_int_cst (TREE_TYPE (reg), 1));
	  u = build2 (MODIFY_EXPR, void_type_node, unshare_expr (reg), t);
	}

      t = fold_convert (TREE_TYPE (reg), size_int (8 - n_reg + 1));
      t = build2 (GE_EXPR, boolean_type_node, u, t);
      u = build1 (GOTO_EXPR, void_type_node, lab_false);
      t = build3 (COND_EXPR, void_type_node, t, u, NULL_TREE);
      gimplify_and_add (t, pre_p);

      t = sav;
      if (sav_ofs)
	t = fold_build_pointer_plus_hwi (sav, sav_ofs);

      u = build2 (POSTINCREMENT_EXPR, TREE_TYPE (reg), unshare_expr (reg),
		  build_int_cst (TREE_TYPE (reg), n_reg));
      u = fold_convert (sizetype, u);
      u = build2 (MULT_EXPR, sizetype, u, size_int (sav_scale));
      t = fold_build_pointer_plus (t, u);

      /* _Decimal32 varargs are located in the second word of the 64-bit
	 FP register for 32-bit binaries.  */
      if (TARGET_32BIT && TARGET_HARD_FLOAT && mode == SDmode)
	t = fold_build_pointer_plus_hwi (t, size);

      /* Args are passed right-aligned.  */
      if (BYTES_BIG_ENDIAN)
	t = fold_build_pointer_plus_hwi (t, pad);

      gimplify_assign (addr, t, pre_p);

      gimple_seq_add_stmt (pre_p, gimple_build_goto (lab_over));

      stmt = gimple_build_label (lab_false);
      gimple_seq_add_stmt (pre_p, stmt);

      if ((n_reg == 2 && !regalign) || n_reg > 2)
	{
	  /* Ensure that we don't find any more args in regs.
	     Alignment has taken care of for special cases.  */
	  gimplify_assign (reg, build_int_cst (TREE_TYPE (reg), 8), pre_p);
	}
    }

  /* ... otherwise out of the overflow area.  */

  /* Care for on-stack alignment if needed.  */
  t = ovf;
  if (align != 1)
    {
      t = fold_build_pointer_plus_hwi (t, align - 1);
      t = build2 (BIT_AND_EXPR, TREE_TYPE (t), t,
		  build_int_cst (TREE_TYPE (t), -align));
    }

  /* Args are passed right-aligned.  */
  if (BYTES_BIG_ENDIAN)
    t = fold_build_pointer_plus_hwi (t, pad);

  gimplify_expr (&t, pre_p, NULL, is_gimple_val, fb_rvalue);

  gimplify_assign (unshare_expr (addr), t, pre_p);

  t = fold_build_pointer_plus_hwi (t, size);
  gimplify_assign (unshare_expr (ovf), t, pre_p);

  if (lab_over)
    {
      stmt = gimple_build_label (lab_over);
      gimple_seq_add_stmt (pre_p, stmt);
    }

  if (STRICT_ALIGNMENT
      && (TYPE_ALIGN (type)
	  > (unsigned) BITS_PER_UNIT * (align < 4 ? 4 : align)))
    {
      /* The value (of type complex double, for example) may not be
	 aligned in memory in the saved registers, so copy via a
	 temporary.  (This is the same code as used for SPARC.)  */
      tree tmp = create_tmp_var (type, "va_arg_tmp");
      tree dest_addr = build_fold_addr_expr (tmp);

      tree copy = build_call_expr (builtin_decl_implicit (BUILT_IN_MEMCPY),
				   3, dest_addr, addr, size_int (rsize * 4));
      TREE_ADDRESSABLE (tmp) = 1;

      gimplify_and_add (copy, pre_p);
      addr = dest_addr;
    }

  addr = fold_convert (ptrtype, addr);
  return build_va_arg_indirect_ref (addr);
}

/* Builtins.  */

static void
def_builtin (const char *name, tree type, enum rs6000_builtins code)
{
  tree t;
  unsigned classify = rs6000_builtin_info[(int)code].attr;
  const char *attr_string = "";

  gcc_assert (name != NULL);
  gcc_assert (IN_RANGE ((int)code, 0, (int)RS6000_BUILTIN_COUNT));

  if (rs6000_builtin_decls[(int)code])
    fatal_error (input_location,
		 "internal error: builtin function %qs already processed",
		 name);

  rs6000_builtin_decls[(int)code] = t =
    add_builtin_function (name, type, (int)code, BUILT_IN_MD, NULL, NULL_TREE);

  /* Set any special attributes.  */
  if ((classify & RS6000_BTC_CONST) != 0)
    {
      /* const function, function only depends on the inputs.  */
      TREE_READONLY (t) = 1;
      TREE_NOTHROW (t) = 1;
      attr_string = ", const";
    }
  else if ((classify & RS6000_BTC_PURE) != 0)
    {
      /* pure function, function can read global memory, but does not set any
	 external state.  */
      DECL_PURE_P (t) = 1;
      TREE_NOTHROW (t) = 1;
      attr_string = ", pure";
    }
  else if ((classify & RS6000_BTC_FP) != 0)
    {
      /* Function is a math function.  If rounding mode is on, then treat the
	 function as not reading global memory, but it can have arbitrary side
	 effects.  If it is off, then assume the function is a const function.
	 This mimics the ATTR_MATHFN_FPROUNDING attribute in
	 builtin-attribute.def that is used for the math functions. */
      TREE_NOTHROW (t) = 1;
      if (flag_rounding_math)
	{
	  DECL_PURE_P (t) = 1;
	  DECL_IS_NOVOPS (t) = 1;
	  attr_string = ", fp, pure";
	}
      else
	{
	  TREE_READONLY (t) = 1;
	  attr_string = ", fp, const";
	}
    }
  else if ((classify & RS6000_BTC_ATTR_MASK) != 0)
    gcc_unreachable ();

  if (TARGET_DEBUG_BUILTIN)
    fprintf (stderr, "rs6000_builtin, code = %4d, %s%s\n",
	     (int)code, name, attr_string);
}

/* Simple ternary operations: VECd = foo (VECa, VECb, VECc).  */

#undef RS6000_BUILTIN_0
#undef RS6000_BUILTIN_1
#undef RS6000_BUILTIN_2
#undef RS6000_BUILTIN_3
#undef RS6000_BUILTIN_A
#undef RS6000_BUILTIN_D
#undef RS6000_BUILTIN_H
#undef RS6000_BUILTIN_P
#undef RS6000_BUILTIN_X

#define RS6000_BUILTIN_0(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_1(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_2(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_3(ENUM, NAME, MASK, ATTR, ICODE) \
  { MASK, ICODE, NAME, ENUM },

#define RS6000_BUILTIN_A(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_D(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_H(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_P(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_X(ENUM, NAME, MASK, ATTR, ICODE)

static const struct builtin_description bdesc_3arg[] =
{
#include "rs6000-builtin.def"
};

/* DST operations: void foo (void *, const int, const char).  */

#undef RS6000_BUILTIN_0
#undef RS6000_BUILTIN_1
#undef RS6000_BUILTIN_2
#undef RS6000_BUILTIN_3
#undef RS6000_BUILTIN_A
#undef RS6000_BUILTIN_D
#undef RS6000_BUILTIN_H
#undef RS6000_BUILTIN_P
#undef RS6000_BUILTIN_X

#define RS6000_BUILTIN_0(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_1(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_2(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_3(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_A(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_D(ENUM, NAME, MASK, ATTR, ICODE) \
  { MASK, ICODE, NAME, ENUM },

#define RS6000_BUILTIN_H(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_P(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_X(ENUM, NAME, MASK, ATTR, ICODE)

static const struct builtin_description bdesc_dst[] =
{
#include "rs6000-builtin.def"
};

/* Simple binary operations: VECc = foo (VECa, VECb).  */

#undef RS6000_BUILTIN_0
#undef RS6000_BUILTIN_1
#undef RS6000_BUILTIN_2
#undef RS6000_BUILTIN_3
#undef RS6000_BUILTIN_A
#undef RS6000_BUILTIN_D
#undef RS6000_BUILTIN_H
#undef RS6000_BUILTIN_P
#undef RS6000_BUILTIN_X

#define RS6000_BUILTIN_0(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_1(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_2(ENUM, NAME, MASK, ATTR, ICODE) \
  { MASK, ICODE, NAME, ENUM },

#define RS6000_BUILTIN_3(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_A(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_D(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_H(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_P(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_X(ENUM, NAME, MASK, ATTR, ICODE)

static const struct builtin_description bdesc_2arg[] =
{
#include "rs6000-builtin.def"
};

#undef RS6000_BUILTIN_0
#undef RS6000_BUILTIN_1
#undef RS6000_BUILTIN_2
#undef RS6000_BUILTIN_3
#undef RS6000_BUILTIN_A
#undef RS6000_BUILTIN_D
#undef RS6000_BUILTIN_H
#undef RS6000_BUILTIN_P
#undef RS6000_BUILTIN_X

#define RS6000_BUILTIN_0(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_1(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_2(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_3(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_A(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_D(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_H(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_P(ENUM, NAME, MASK, ATTR, ICODE) \
  { MASK, ICODE, NAME, ENUM },

#define RS6000_BUILTIN_X(ENUM, NAME, MASK, ATTR, ICODE)

/* AltiVec predicates.  */

static const struct builtin_description bdesc_altivec_preds[] =
{
#include "rs6000-builtin.def"
};

/* ABS* operations.  */

#undef RS6000_BUILTIN_0
#undef RS6000_BUILTIN_1
#undef RS6000_BUILTIN_2
#undef RS6000_BUILTIN_3
#undef RS6000_BUILTIN_A
#undef RS6000_BUILTIN_D
#undef RS6000_BUILTIN_H
#undef RS6000_BUILTIN_P
#undef RS6000_BUILTIN_X

#define RS6000_BUILTIN_0(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_1(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_2(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_3(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_A(ENUM, NAME, MASK, ATTR, ICODE) \
  { MASK, ICODE, NAME, ENUM },

#define RS6000_BUILTIN_D(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_H(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_P(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_X(ENUM, NAME, MASK, ATTR, ICODE)

static const struct builtin_description bdesc_abs[] =
{
#include "rs6000-builtin.def"
};

/* Simple unary operations: VECb = foo (unsigned literal) or VECb =
   foo (VECa).  */

#undef RS6000_BUILTIN_0
#undef RS6000_BUILTIN_1
#undef RS6000_BUILTIN_2
#undef RS6000_BUILTIN_3
#undef RS6000_BUILTIN_A
#undef RS6000_BUILTIN_D
#undef RS6000_BUILTIN_H
#undef RS6000_BUILTIN_P
#undef RS6000_BUILTIN_X

#define RS6000_BUILTIN_0(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_1(ENUM, NAME, MASK, ATTR, ICODE) \
  { MASK, ICODE, NAME, ENUM },

#define RS6000_BUILTIN_2(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_3(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_A(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_D(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_H(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_P(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_X(ENUM, NAME, MASK, ATTR, ICODE)

static const struct builtin_description bdesc_1arg[] =
{
#include "rs6000-builtin.def"
};

/* Simple no-argument operations: result = __builtin_darn_32 () */

#undef RS6000_BUILTIN_0
#undef RS6000_BUILTIN_1
#undef RS6000_BUILTIN_2
#undef RS6000_BUILTIN_3
#undef RS6000_BUILTIN_A
#undef RS6000_BUILTIN_D
#undef RS6000_BUILTIN_H
#undef RS6000_BUILTIN_P
#undef RS6000_BUILTIN_X

#define RS6000_BUILTIN_0(ENUM, NAME, MASK, ATTR, ICODE) \
  { MASK, ICODE, NAME, ENUM },

#define RS6000_BUILTIN_1(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_2(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_3(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_A(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_D(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_H(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_P(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_X(ENUM, NAME, MASK, ATTR, ICODE)

static const struct builtin_description bdesc_0arg[] =
{
#include "rs6000-builtin.def"
};

/* HTM builtins.  */
#undef RS6000_BUILTIN_0
#undef RS6000_BUILTIN_1
#undef RS6000_BUILTIN_2
#undef RS6000_BUILTIN_3
#undef RS6000_BUILTIN_A
#undef RS6000_BUILTIN_D
#undef RS6000_BUILTIN_H
#undef RS6000_BUILTIN_P
#undef RS6000_BUILTIN_X

#define RS6000_BUILTIN_0(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_1(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_2(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_3(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_A(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_D(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_H(ENUM, NAME, MASK, ATTR, ICODE) \
  { MASK, ICODE, NAME, ENUM },

#define RS6000_BUILTIN_P(ENUM, NAME, MASK, ATTR, ICODE)
#define RS6000_BUILTIN_X(ENUM, NAME, MASK, ATTR, ICODE)

static const struct builtin_description bdesc_htm[] =
{
#include "rs6000-builtin.def"
};

#undef RS6000_BUILTIN_0
#undef RS6000_BUILTIN_1
#undef RS6000_BUILTIN_2
#undef RS6000_BUILTIN_3
#undef RS6000_BUILTIN_A
#undef RS6000_BUILTIN_D
#undef RS6000_BUILTIN_H
#undef RS6000_BUILTIN_P

/* Return true if a builtin function is overloaded.  */
bool
rs6000_overloaded_builtin_p (enum rs6000_builtins fncode)
{
  return (rs6000_builtin_info[(int)fncode].attr & RS6000_BTC_OVERLOADED) != 0;
}

const char *
rs6000_overloaded_builtin_name (enum rs6000_builtins fncode)
{
  return rs6000_builtin_info[(int)fncode].name;
}

/* Expand an expression EXP that calls a builtin without arguments.  */
static rtx
rs6000_expand_zeroop_builtin (enum insn_code icode, rtx target)
{
  rtx pat;
  machine_mode tmode = insn_data[icode].operand[0].mode;

  if (icode == CODE_FOR_nothing)
    /* Builtin not supported on this processor.  */
    return 0;

  if (icode == CODE_FOR_rs6000_mffsl
      && rs6000_isa_flags & OPTION_MASK_SOFT_FLOAT)
    {
      error ("%<__builtin_mffsl%> not supported with %<-msoft-float%>");
      return const0_rtx;
    }

  if (target == 0
      || GET_MODE (target) != tmode
      || ! (*insn_data[icode].operand[0].predicate) (target, tmode))
    target = gen_reg_rtx (tmode);

  pat = GEN_FCN (icode) (target);
  if (! pat)
    return 0;
  emit_insn (pat);

  return target;
}


static rtx
rs6000_expand_mtfsf_builtin (enum insn_code icode, tree exp)
{
  rtx pat;
  tree arg0 = CALL_EXPR_ARG (exp, 0);
  tree arg1 = CALL_EXPR_ARG (exp, 1);
  rtx op0 = expand_normal (arg0);
  rtx op1 = expand_normal (arg1);
  machine_mode mode0 = insn_data[icode].operand[0].mode;
  machine_mode mode1 = insn_data[icode].operand[1].mode;

  if (icode == CODE_FOR_nothing)
    /* Builtin not supported on this processor.  */
    return 0;

  /* If we got invalid arguments bail out before generating bad rtl.  */
  if (arg0 == error_mark_node || arg1 == error_mark_node)
    return const0_rtx;

  if (!CONST_INT_P (op0)
      || INTVAL (op0) > 255
      || INTVAL (op0) < 0)
    {
      error ("argument 1 must be an 8-bit field value");
      return const0_rtx;
    }

  if (! (*insn_data[icode].operand[0].predicate) (op0, mode0))
    op0 = copy_to_mode_reg (mode0, op0);

  if (! (*insn_data[icode].operand[1].predicate) (op1, mode1))
    op1 = copy_to_mode_reg (mode1, op1);

  pat = GEN_FCN (icode) (op0, op1);
  if (!pat)
    return const0_rtx;
  emit_insn (pat);

  return NULL_RTX;
}

static rtx
rs6000_expand_mtfsb_builtin (enum insn_code icode, tree exp)
{
  rtx pat;
  tree arg0 = CALL_EXPR_ARG (exp, 0);
  rtx op0 = expand_normal (arg0);

  if (icode == CODE_FOR_nothing)
    /* Builtin not supported on this processor.  */
    return 0;

  if (rs6000_isa_flags & OPTION_MASK_SOFT_FLOAT)
    {
      error ("%<__builtin_mtfsb0%> and %<__builtin_mtfsb1%> not supported with "
	     "%<-msoft-float%>");
      return const0_rtx;
    }

  /* If we got invalid arguments bail out before generating bad rtl.  */
  if (arg0 == error_mark_node)
    return const0_rtx;

  /* Only allow bit numbers 0 to 31.  */
  if (!u5bit_cint_operand (op0, VOIDmode))
    {
       error ("Argument must be a constant between 0 and 31.");
       return const0_rtx;
     }

  pat = GEN_FCN (icode) (op0);
  if (!pat)
    return const0_rtx;
  emit_insn (pat);

  return NULL_RTX;
}

static rtx
rs6000_expand_set_fpscr_rn_builtin (enum insn_code icode, tree exp)
{
  rtx pat;
  tree arg0 = CALL_EXPR_ARG (exp, 0);
  rtx op0 = expand_normal (arg0);
  machine_mode mode0 = insn_data[icode].operand[0].mode;

  if (icode == CODE_FOR_nothing)
    /* Builtin not supported on this processor.  */
    return 0;

  if (rs6000_isa_flags & OPTION_MASK_SOFT_FLOAT)
    {
      error ("%<__builtin_set_fpscr_rn%> not supported with %<-msoft-float%>");
      return const0_rtx;
    }

  /* If we got invalid arguments bail out before generating bad rtl.  */
  if (arg0 == error_mark_node)
    return const0_rtx;

  /* If the argument is a constant, check the range. Argument can only be a
     2-bit value.  Unfortunately, can't check the range of the value at
     compile time if the argument is a variable.  The least significant two
     bits of the argument, regardless of type, are used to set the rounding
     mode.  All other bits are ignored.  */
  if (CONST_INT_P (op0) && !const_0_to_3_operand(op0, VOIDmode))
    {
      error ("Argument must be a value between 0 and 3.");
      return const0_rtx;
    }

  if (! (*insn_data[icode].operand[0].predicate) (op0, mode0))
    op0 = copy_to_mode_reg (mode0, op0);

  pat = GEN_FCN (icode) (op0);
  if (!pat)
    return const0_rtx;
  emit_insn (pat);

  return NULL_RTX;
}
static rtx
rs6000_expand_set_fpscr_drn_builtin (enum insn_code icode, tree exp)
{
  rtx pat;
  tree arg0 = CALL_EXPR_ARG (exp, 0);
  rtx op0 = expand_normal (arg0);
  machine_mode mode0 = insn_data[icode].operand[0].mode;

  if (TARGET_32BIT)
    /* Builtin not supported in 32-bit mode.  */
    fatal_error (input_location,
		 "%<__builtin_set_fpscr_drn%> is not supported "
		 "in 32-bit mode");

  if (rs6000_isa_flags & OPTION_MASK_SOFT_FLOAT)
    {
      error ("%<__builtin_set_fpscr_drn%> not supported with %<-msoft-float%>");
      return const0_rtx;
    }

  if (icode == CODE_FOR_nothing)
    /* Builtin not supported on this processor.  */
    return 0;

  /* If we got invalid arguments bail out before generating bad rtl.  */
  if (arg0 == error_mark_node)
    return const0_rtx;

  /* If the argument is a constant, check the range. Agrument can only be a
     3-bit value.  Unfortunately, can't check the range of the value at
     compile time if the argument is a variable. The least significant two
     bits of the argument, regardless of type, are used to set the rounding
     mode.  All other bits are ignored.  */
  if (CONST_INT_P (op0) && !const_0_to_7_operand(op0, VOIDmode))
   {
      error ("Argument must be a value between 0 and 7.");
      return const0_rtx;
    }

  if (! (*insn_data[icode].operand[0].predicate) (op0, mode0))
    op0 = copy_to_mode_reg (mode0, op0);

  pat = GEN_FCN (icode) (op0);
  if (! pat)
    return const0_rtx;
  emit_insn (pat);

  return NULL_RTX;
}

static rtx
rs6000_expand_unop_builtin (enum insn_code icode, tree exp, rtx target)
{
  rtx pat;
  tree arg0 = CALL_EXPR_ARG (exp, 0);
  rtx op0 = expand_normal (arg0);
  machine_mode tmode = insn_data[icode].operand[0].mode;
  machine_mode mode0 = insn_data[icode].operand[1].mode;

  if (icode == CODE_FOR_nothing)
    /* Builtin not supported on this processor.  */
    return 0;

  /* If we got invalid arguments bail out before generating bad rtl.  */
  if (arg0 == error_mark_node)
    return const0_rtx;

  if (icode == CODE_FOR_altivec_vspltisb
      || icode == CODE_FOR_altivec_vspltish
      || icode == CODE_FOR_altivec_vspltisw)
    {
      /* Only allow 5-bit *signed* literals.  */
      if (!CONST_INT_P (op0)
	  || INTVAL (op0) > 15
	  || INTVAL (op0) < -16)
	{
	  error ("argument 1 must be a 5-bit signed literal");
	  return CONST0_RTX (tmode);
	}
    }

  if (target == 0
      || GET_MODE (target) != tmode
      || ! (*insn_data[icode].operand[0].predicate) (target, tmode))
    target = gen_reg_rtx (tmode);

  if (! (*insn_data[icode].operand[1].predicate) (op0, mode0))
    op0 = copy_to_mode_reg (mode0, op0);

  pat = GEN_FCN (icode) (target, op0);
  if (! pat)
    return 0;
  emit_insn (pat);

  return target;
}

static rtx
altivec_expand_abs_builtin (enum insn_code icode, tree exp, rtx target)
{
  rtx pat, scratch1, scratch2;
  tree arg0 = CALL_EXPR_ARG (exp, 0);
  rtx op0 = expand_normal (arg0);
  machine_mode tmode = insn_data[icode].operand[0].mode;
  machine_mode mode0 = insn_data[icode].operand[1].mode;

  /* If we have invalid arguments, bail out before generating bad rtl.  */
  if (arg0 == error_mark_node)
    return const0_rtx;

  if (target == 0
      || GET_MODE (target) != tmode
      || ! (*insn_data[icode].operand[0].predicate) (target, tmode))
    target = gen_reg_rtx (tmode);

  if (! (*insn_data[icode].operand[1].predicate) (op0, mode0))
    op0 = copy_to_mode_reg (mode0, op0);

  scratch1 = gen_reg_rtx (mode0);
  scratch2 = gen_reg_rtx (mode0);

  pat = GEN_FCN (icode) (target, op0, scratch1, scratch2);
  if (! pat)
    return 0;
  emit_insn (pat);

  return target;
}

static rtx
rs6000_expand_binop_builtin (enum insn_code icode, tree exp, rtx target)
{
  rtx pat;
  tree arg0 = CALL_EXPR_ARG (exp, 0);
  tree arg1 = CALL_EXPR_ARG (exp, 1);
  rtx op0 = expand_normal (arg0);
  rtx op1 = expand_normal (arg1);
  machine_mode tmode = insn_data[icode].operand[0].mode;
  machine_mode mode0 = insn_data[icode].operand[1].mode;
  machine_mode mode1 = insn_data[icode].operand[2].mode;

  if (icode == CODE_FOR_nothing)
    /* Builtin not supported on this processor.  */
    return 0;

  /* If we got invalid arguments bail out before generating bad rtl.  */
  if (arg0 == error_mark_node || arg1 == error_mark_node)
    return const0_rtx;

  if (icode == CODE_FOR_unpackv1ti
	   || icode == CODE_FOR_unpackkf
	   || icode == CODE_FOR_unpacktf
	   || icode == CODE_FOR_unpackif
	   || icode == CODE_FOR_unpacktd)
    {
      /* Only allow 1-bit unsigned literals. */
      STRIP_NOPS (arg1);
      if (TREE_CODE (arg1) != INTEGER_CST
	  || !IN_RANGE (TREE_INT_CST_LOW (arg1), 0, 1))
	{
	  error ("argument 2 must be a 1-bit unsigned literal");
	  return CONST0_RTX (tmode);
	}
    }
  else if (icode == CODE_FOR_altivec_vspltw)
    {
      /* Only allow 2-bit unsigned literals.  */
      STRIP_NOPS (arg1);
      if (TREE_CODE (arg1) != INTEGER_CST
	  || TREE_INT_CST_LOW (arg1) & ~3)
	{
	  error ("argument 2 must be a 2-bit unsigned literal");
	  return CONST0_RTX (tmode);
	}
    }
  else if (icode == CODE_FOR_altivec_vsplth)
    {
      /* Only allow 3-bit unsigned literals.  */
      STRIP_NOPS (arg1);
      if (TREE_CODE (arg1) != INTEGER_CST
	  || TREE_INT_CST_LOW (arg1) & ~7)
	{
	  error ("argument 2 must be a 3-bit unsigned literal");
	  return CONST0_RTX (tmode);
	}
    }
  else if (icode == CODE_FOR_altivec_vspltb)
    {
      /* Only allow 4-bit unsigned literals.  */
      STRIP_NOPS (arg1);
      if (TREE_CODE (arg1) != INTEGER_CST
	  || TREE_INT_CST_LOW (arg1) & ~15)
	{
	  error ("argument 2 must be a 4-bit unsigned literal");
	  return CONST0_RTX (tmode);
	}
    }
  else if (icode == CODE_FOR_altivec_vcfux
      || icode == CODE_FOR_altivec_vcfsx
      || icode == CODE_FOR_altivec_vctsxs
      || icode == CODE_FOR_altivec_vctuxs)
    {
      /* Only allow 5-bit unsigned literals.  */
      STRIP_NOPS (arg1);
      if (TREE_CODE (arg1) != INTEGER_CST
	  || TREE_INT_CST_LOW (arg1) & ~0x1f)
	{
	  error ("argument 2 must be a 5-bit unsigned literal");
	  return CONST0_RTX (tmode);
	}
    }
  else if (icode == CODE_FOR_dfptstsfi_eq_dd
      || icode == CODE_FOR_dfptstsfi_lt_dd
      || icode == CODE_FOR_dfptstsfi_gt_dd
      || icode == CODE_FOR_dfptstsfi_unordered_dd
      || icode == CODE_FOR_dfptstsfi_eq_td
      || icode == CODE_FOR_dfptstsfi_lt_td
      || icode == CODE_FOR_dfptstsfi_gt_td
      || icode == CODE_FOR_dfptstsfi_unordered_td)
    {
      /* Only allow 6-bit unsigned literals.  */
      STRIP_NOPS (arg0);
      if (TREE_CODE (arg0) != INTEGER_CST
	  || !IN_RANGE (TREE_INT_CST_LOW (arg0), 0, 63))
	{
	  error ("argument 1 must be a 6-bit unsigned literal");
	  return CONST0_RTX (tmode);
	}
    }
  else if (icode == CODE_FOR_xststdcqp_kf
	   || icode == CODE_FOR_xststdcqp_tf
	   || icode == CODE_FOR_xststdcdp
	   || icode == CODE_FOR_xststdcsp
	   || icode == CODE_FOR_xvtstdcdp
	   || icode == CODE_FOR_xvtstdcsp)
    {
      /* Only allow 7-bit unsigned literals. */
      STRIP_NOPS (arg1);
      if (TREE_CODE (arg1) != INTEGER_CST
	  || !IN_RANGE (TREE_INT_CST_LOW (arg1), 0, 127))
	{
	  error ("argument 2 must be a 7-bit unsigned literal");
	  return CONST0_RTX (tmode);
	}
    }

  if (target == 0
      || GET_MODE (target) != tmode
      || ! (*insn_data[icode].operand[0].predicate) (target, tmode))
    target = gen_reg_rtx (tmode);

  if (! (*insn_data[icode].operand[1].predicate) (op0, mode0))
    op0 = copy_to_mode_reg (mode0, op0);
  if (! (*insn_data[icode].operand[2].predicate) (op1, mode1))
    op1 = copy_to_mode_reg (mode1, op1);

  pat = GEN_FCN (icode) (target, op0, op1);
  if (! pat)
    return 0;
  emit_insn (pat);

  return target;
}

static rtx
altivec_expand_predicate_builtin (enum insn_code icode, tree exp, rtx target)
{
  rtx pat, scratch;
  tree cr6_form = CALL_EXPR_ARG (exp, 0);
  tree arg0 = CALL_EXPR_ARG (exp, 1);
  tree arg1 = CALL_EXPR_ARG (exp, 2);
  rtx op0 = expand_normal (arg0);
  rtx op1 = expand_normal (arg1);
  machine_mode tmode = SImode;
  machine_mode mode0 = insn_data[icode].operand[1].mode;
  machine_mode mode1 = insn_data[icode].operand[2].mode;
  int cr6_form_int;

  if (TREE_CODE (cr6_form) != INTEGER_CST)
    {
      error ("argument 1 of %qs must be a constant",
	     "__builtin_altivec_predicate");
      return const0_rtx;
    }
  else
    cr6_form_int = TREE_INT_CST_LOW (cr6_form);

  gcc_assert (mode0 == mode1);

  /* If we have invalid arguments, bail out before generating bad rtl.  */
  if (arg0 == error_mark_node || arg1 == error_mark_node)
    return const0_rtx;

  if (target == 0
      || GET_MODE (target) != tmode
      || ! (*insn_data[icode].operand[0].predicate) (target, tmode))
    target = gen_reg_rtx (tmode);

  if (! (*insn_data[icode].operand[1].predicate) (op0, mode0))
    op0 = copy_to_mode_reg (mode0, op0);
  if (! (*insn_data[icode].operand[2].predicate) (op1, mode1))
    op1 = copy_to_mode_reg (mode1, op1);

  /* Note that for many of the relevant operations (e.g. cmpne or
     cmpeq) with float or double operands, it makes more sense for the
     mode of the allocated scratch register to select a vector of
     integer.  But the choice to copy the mode of operand 0 was made
     long ago and there are no plans to change it.  */
  scratch = gen_reg_rtx (mode0);

  pat = GEN_FCN (icode) (scratch, op0, op1);
  if (! pat)
    return 0;
  emit_insn (pat);

  /* The vec_any* and vec_all* predicates use the same opcodes for two
     different operations, but the bits in CR6 will be different
     depending on what information we want.  So we have to play tricks
     with CR6 to get the right bits out.

     If you think this is disgusting, look at the specs for the
     AltiVec predicates.  */

  switch (cr6_form_int)
    {
    case 0:
      emit_insn (gen_cr6_test_for_zero (target));
      break;
    case 1:
      emit_insn (gen_cr6_test_for_zero_reverse (target));
      break;
    case 2:
      emit_insn (gen_cr6_test_for_lt (target));
      break;
    case 3:
      emit_insn (gen_cr6_test_for_lt_reverse (target));
      break;
    default:
      error ("argument 1 of %qs is out of range",
	     "__builtin_altivec_predicate");
      break;
    }

  return target;
}

rtx
swap_endian_selector_for_mode (machine_mode mode)
{
  unsigned int swap1[16] = {15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};
  unsigned int swap2[16] = {7,6,5,4,3,2,1,0,15,14,13,12,11,10,9,8};
  unsigned int swap4[16] = {3,2,1,0,7,6,5,4,11,10,9,8,15,14,13,12};
  unsigned int swap8[16] = {1,0,3,2,5,4,7,6,9,8,11,10,13,12,15,14};

  unsigned int *swaparray, i;
  rtx perm[16];

  switch (mode)
    {
    case E_V1TImode:
      swaparray = swap1;
      break;
    case E_V2DFmode:
    case E_V2DImode:
      swaparray = swap2;
      break;
    case E_V4SFmode:
    case E_V4SImode:
      swaparray = swap4;
      break;
    case E_V8HImode:
      swaparray = swap8;
      break;
    default:
      gcc_unreachable ();
    }

  for (i = 0; i < 16; ++i)
    perm[i] = GEN_INT (swaparray[i]);

  return force_reg (V16QImode, gen_rtx_CONST_VECTOR (V16QImode,
						     gen_rtvec_v (16, perm)));
}

static rtx
altivec_expand_lv_builtin (enum insn_code icode, tree exp, rtx target, bool blk)
{
  rtx pat, addr;
  tree arg0 = CALL_EXPR_ARG (exp, 0);
  tree arg1 = CALL_EXPR_ARG (exp, 1);
  machine_mode tmode = insn_data[icode].operand[0].mode;
  machine_mode mode0 = Pmode;
  machine_mode mode1 = Pmode;
  rtx op0 = expand_normal (arg0);
  rtx op1 = expand_normal (arg1);

  if (icode == CODE_FOR_nothing)
    /* Builtin not supported on this processor.  */
    return 0;

  /* If we got invalid arguments bail out before generating bad rtl.  */
  if (arg0 == error_mark_node || arg1 == error_mark_node)
    return const0_rtx;

  if (target == 0
      || GET_MODE (target) != tmode
      || ! (*insn_data[icode].operand[0].predicate) (target, tmode))
    target = gen_reg_rtx (tmode);

  op1 = copy_to_mode_reg (mode1, op1);

  /* For LVX, express the RTL accurately by ANDing the address with -16.
     LVXL and LVE*X expand to use UNSPECs to hide their special behavior,
     so the raw address is fine.  */
  if (icode == CODE_FOR_altivec_lvx_v1ti
      || icode == CODE_FOR_altivec_lvx_v2df
      || icode == CODE_FOR_altivec_lvx_v2di
      || icode == CODE_FOR_altivec_lvx_v4sf
      || icode == CODE_FOR_altivec_lvx_v4si
      || icode == CODE_FOR_altivec_lvx_v8hi
      || icode == CODE_FOR_altivec_lvx_v16qi)
    {
      rtx rawaddr;
      if (op0 == const0_rtx)
	rawaddr = op1;
      else
	{
	  op0 = copy_to_mode_reg (mode0, op0);
	  rawaddr = gen_rtx_PLUS (Pmode, op1, op0);
	}
      addr = gen_rtx_AND (Pmode, rawaddr, gen_rtx_CONST_INT (Pmode, -16));
      addr = gen_rtx_MEM (blk ? BLKmode : tmode, addr);

      emit_insn (gen_rtx_SET (target, addr));
    }
  else
    {
      if (op0 == const0_rtx)
	addr = gen_rtx_MEM (blk ? BLKmode : tmode, op1);
      else
	{
	  op0 = copy_to_mode_reg (mode0, op0);
	  addr = gen_rtx_MEM (blk ? BLKmode : tmode,
			      gen_rtx_PLUS (Pmode, op1, op0));
	}

      pat = GEN_FCN (icode) (target, addr);
      if (! pat)
	return 0;
      emit_insn (pat);
    }

  return target;
}

static rtx
altivec_expand_stxvl_builtin (enum insn_code icode, tree exp)
{
  rtx pat;
  tree arg0 = CALL_EXPR_ARG (exp, 0);
  tree arg1 = CALL_EXPR_ARG (exp, 1);
  tree arg2 = CALL_EXPR_ARG (exp, 2);
  rtx op0 = expand_normal (arg0);
  rtx op1 = expand_normal (arg1);
  rtx op2 = expand_normal (arg2);
  machine_mode mode0 = insn_data[icode].operand[0].mode;
  machine_mode mode1 = insn_data[icode].operand[1].mode;
  machine_mode mode2 = insn_data[icode].operand[2].mode;

  if (icode == CODE_FOR_nothing)
    /* Builtin not supported on this processor.  */
    return NULL_RTX;

  /* If we got invalid arguments bail out before generating bad rtl.  */
  if (arg0 == error_mark_node
      || arg1 == error_mark_node
      || arg2 == error_mark_node)
    return NULL_RTX;

  if (! (*insn_data[icode].operand[1].predicate) (op0, mode0))
    op0 = copy_to_mode_reg (mode0, op0);
  if (! (*insn_data[icode].operand[2].predicate) (op1, mode1))
    op1 = copy_to_mode_reg (mode1, op1);
  if (! (*insn_data[icode].operand[3].predicate) (op2, mode2))
    op2 = copy_to_mode_reg (mode2, op2);

  pat = GEN_FCN (icode) (op0, op1, op2);
  if (pat)
    emit_insn (pat);

  return NULL_RTX;
}

static rtx
altivec_expand_stv_builtin (enum insn_code icode, tree exp)
{
  tree arg0 = CALL_EXPR_ARG (exp, 0);
  tree arg1 = CALL_EXPR_ARG (exp, 1);
  tree arg2 = CALL_EXPR_ARG (exp, 2);
  rtx op0 = expand_normal (arg0);
  rtx op1 = expand_normal (arg1);
  rtx op2 = expand_normal (arg2);
  rtx pat, addr, rawaddr;
  machine_mode tmode = insn_data[icode].operand[0].mode;
  machine_mode smode = insn_data[icode].operand[1].mode;
  machine_mode mode1 = Pmode;
  machine_mode mode2 = Pmode;

  /* Invalid arguments.  Bail before doing anything stoopid!  */
  if (arg0 == error_mark_node
      || arg1 == error_mark_node
      || arg2 == error_mark_node)
    return const0_rtx;

  op2 = copy_to_mode_reg (mode2, op2);

  /* For STVX, express the RTL accurately by ANDing the address with -16.
     STVXL and STVE*X expand to use UNSPECs to hide their special behavior,
     so the raw address is fine.  */
  if (icode == CODE_FOR_altivec_stvx_v2df
      || icode == CODE_FOR_altivec_stvx_v2di
      || icode == CODE_FOR_altivec_stvx_v4sf
      || icode == CODE_FOR_altivec_stvx_v4si
      || icode == CODE_FOR_altivec_stvx_v8hi
      || icode == CODE_FOR_altivec_stvx_v16qi)
    {
      if (op1 == const0_rtx)
	rawaddr = op2;
      else
	{
	  op1 = copy_to_mode_reg (mode1, op1);
	  rawaddr = gen_rtx_PLUS (Pmode, op2, op1);
	}

      addr = gen_rtx_AND (Pmode, rawaddr, gen_rtx_CONST_INT (Pmode, -16));
      addr = gen_rtx_MEM (tmode, addr);

      op0 = copy_to_mode_reg (tmode, op0);

      emit_insn (gen_rtx_SET (addr, op0));
    }
  else
    {
      if (! (*insn_data[icode].operand[1].predicate) (op0, smode))
	op0 = copy_to_mode_reg (smode, op0);

      if (op1 == const0_rtx)
	addr = gen_rtx_MEM (tmode, op2);
      else
	{
	  op1 = copy_to_mode_reg (mode1, op1);
	  addr = gen_rtx_MEM (tmode, gen_rtx_PLUS (Pmode, op2, op1));
	}

      pat = GEN_FCN (icode) (addr, op0);
      if (pat)
	emit_insn (pat);
    }

  return NULL_RTX;
}

/* Return the appropriate SPR number associated with the given builtin.  */
static inline HOST_WIDE_INT
htm_spr_num (enum rs6000_builtins code)
{
  if (code == HTM_BUILTIN_GET_TFHAR
      || code == HTM_BUILTIN_SET_TFHAR)
    return TFHAR_SPR;
  else if (code == HTM_BUILTIN_GET_TFIAR
	   || code == HTM_BUILTIN_SET_TFIAR)
    return TFIAR_SPR;
  else if (code == HTM_BUILTIN_GET_TEXASR
	   || code == HTM_BUILTIN_SET_TEXASR)
    return TEXASR_SPR;
  gcc_assert (code == HTM_BUILTIN_GET_TEXASRU
	      || code == HTM_BUILTIN_SET_TEXASRU);
  return TEXASRU_SPR;
}

/* Return the correct ICODE value depending on whether we are
   setting or reading the HTM SPRs.  */
static inline enum insn_code
rs6000_htm_spr_icode (bool nonvoid)
{
  if (nonvoid)
    return (TARGET_POWERPC64) ? CODE_FOR_htm_mfspr_di : CODE_FOR_htm_mfspr_si;
  else
    return (TARGET_POWERPC64) ? CODE_FOR_htm_mtspr_di : CODE_FOR_htm_mtspr_si;
}

/* Expand the HTM builtin in EXP and store the result in TARGET.
   Store true in *EXPANDEDP if we found a builtin to expand.  */
static rtx
htm_expand_builtin (tree exp, rtx target, bool * expandedp)
{
  tree fndecl = TREE_OPERAND (CALL_EXPR_FN (exp), 0);
  bool nonvoid = TREE_TYPE (TREE_TYPE (fndecl)) != void_type_node;
  enum rs6000_builtins fcode
    = (enum rs6000_builtins) DECL_MD_FUNCTION_CODE (fndecl);
  const struct builtin_description *d;
  size_t i;

  *expandedp = true;

  if (!TARGET_POWERPC64
      && (fcode == HTM_BUILTIN_TABORTDC
	  || fcode == HTM_BUILTIN_TABORTDCI))
    {
      size_t uns_fcode = (size_t)fcode;
      const char *name = rs6000_builtin_info[uns_fcode].name;
      error ("builtin %qs is only valid in 64-bit mode", name);
      return const0_rtx;
    }

  /* Expand the HTM builtins.  */
  d = bdesc_htm;
  for (i = 0; i < ARRAY_SIZE (bdesc_htm); i++, d++)
    if (d->code == fcode)
      {
	rtx op[MAX_HTM_OPERANDS], pat;
	int nopnds = 0;
	tree arg;
	call_expr_arg_iterator iter;
	unsigned attr = rs6000_builtin_info[fcode].attr;
	enum insn_code icode = d->icode;
	const struct insn_operand_data *insn_op;
	bool uses_spr = (attr & RS6000_BTC_SPR);
	rtx cr = NULL_RTX;

	if (uses_spr)
	  icode = rs6000_htm_spr_icode (nonvoid);
	insn_op = &insn_data[icode].operand[0];

	if (nonvoid)
	  {
	    machine_mode tmode = (uses_spr) ? insn_op->mode : E_SImode;
	    if (!target
		|| GET_MODE (target) != tmode
		|| (uses_spr && !(*insn_op->predicate) (target, tmode)))
	      target = gen_reg_rtx (tmode);
	    if (uses_spr)
	      op[nopnds++] = target;
	  }

	FOR_EACH_CALL_EXPR_ARG (arg, iter, exp)
	{
	  if (arg == error_mark_node || nopnds >= MAX_HTM_OPERANDS)
	    return const0_rtx;

	  insn_op = &insn_data[icode].operand[nopnds];

	  op[nopnds] = expand_normal (arg);

	  if (!(*insn_op->predicate) (op[nopnds], insn_op->mode))
	    {
	      if (!strcmp (insn_op->constraint, "n"))
		{
		  int arg_num = (nonvoid) ? nopnds : nopnds + 1;
		  if (!CONST_INT_P (op[nopnds]))
		    error ("argument %d must be an unsigned literal", arg_num);
		  else
		    error ("argument %d is an unsigned literal that is "
			   "out of range", arg_num);
		  return const0_rtx;
		}
	      op[nopnds] = copy_to_mode_reg (insn_op->mode, op[nopnds]);
	    }

	  nopnds++;
	}

	/* Handle the builtins for extended mnemonics.  These accept
	   no arguments, but map to builtins that take arguments.  */
	switch (fcode)
	  {
	  case HTM_BUILTIN_TENDALL:  /* Alias for: tend. 1  */
	  case HTM_BUILTIN_TRESUME:  /* Alias for: tsr. 1  */
	    op[nopnds++] = GEN_INT (1);
	    if (flag_checking)
	      attr |= RS6000_BTC_UNARY;
	    break;
	  case HTM_BUILTIN_TSUSPEND: /* Alias for: tsr. 0  */
	    op[nopnds++] = GEN_INT (0);
	    if (flag_checking)
	      attr |= RS6000_BTC_UNARY;
	    break;
	  default:
	    break;
	  }

	/* If this builtin accesses SPRs, then pass in the appropriate
	   SPR number and SPR regno as the last two operands.  */
	if (uses_spr)
	  {
	    machine_mode mode = (TARGET_POWERPC64) ? DImode : SImode;
	    op[nopnds++] = gen_rtx_CONST_INT (mode, htm_spr_num (fcode));
	  }
	/* If this builtin accesses a CR, then pass in a scratch
	   CR as the last operand.  */
	else if (attr & RS6000_BTC_CR)
	  { cr = gen_reg_rtx (CCmode);
	    op[nopnds++] = cr;
	  }

	if (flag_checking)
	  {
	    int expected_nopnds = 0;
	    if ((attr & RS6000_BTC_TYPE_MASK) == RS6000_BTC_UNARY)
	      expected_nopnds = 1;
	    else if ((attr & RS6000_BTC_TYPE_MASK) == RS6000_BTC_BINARY)
	      expected_nopnds = 2;
	    else if ((attr & RS6000_BTC_TYPE_MASK) == RS6000_BTC_TERNARY)
	      expected_nopnds = 3;
	    if (!(attr & RS6000_BTC_VOID))
	      expected_nopnds += 1;
	    if (uses_spr)
	      expected_nopnds += 1;

	    gcc_assert (nopnds == expected_nopnds
			&& nopnds <= MAX_HTM_OPERANDS);
	  }

	switch (nopnds)
	  {
	  case 1:
	    pat = GEN_FCN (icode) (op[0]);
	    break;
	  case 2:
	    pat = GEN_FCN (icode) (op[0], op[1]);
	    break;
	  case 3:
	    pat = GEN_FCN (icode) (op[0], op[1], op[2]);
	    break;
	  case 4:
	    pat = GEN_FCN (icode) (op[0], op[1], op[2], op[3]);
	    break;
	  default:
	    gcc_unreachable ();
	  }
	if (!pat)
	  return NULL_RTX;
	emit_insn (pat);

	if (attr & RS6000_BTC_CR)
	  {
	    if (fcode == HTM_BUILTIN_TBEGIN)
	      {
		/* Emit code to set TARGET to true or false depending on
		   whether the tbegin. instruction successfully or failed
		   to start a transaction.  We do this by placing the 1's
		   complement of CR's EQ bit into TARGET.  */
		rtx scratch = gen_reg_rtx (SImode);
		emit_insn (gen_rtx_SET (scratch,
					gen_rtx_EQ (SImode, cr,
						     const0_rtx)));
		emit_insn (gen_rtx_SET (target,
					gen_rtx_XOR (SImode, scratch,
						     GEN_INT (1))));
	      }
	    else
	      {
		/* Emit code to copy the 4-bit condition register field
		   CR into the least significant end of register TARGET.  */
		rtx scratch1 = gen_reg_rtx (SImode);
		rtx scratch2 = gen_reg_rtx (SImode);
		rtx subreg = simplify_gen_subreg (CCmode, scratch1, SImode, 0);
		emit_insn (gen_movcc (subreg, cr));
		emit_insn (gen_lshrsi3 (scratch2, scratch1, GEN_INT (28)));
		emit_insn (gen_andsi3 (target, scratch2, GEN_INT (0xf)));
	      }
	  }

	if (nonvoid)
	  return target;
	return const0_rtx;
      }

  *expandedp = false;
  return NULL_RTX;
}

/* Expand the CPU builtin in FCODE and store the result in TARGET.  */

static rtx
cpu_expand_builtin (enum rs6000_builtins fcode, tree exp ATTRIBUTE_UNUSED,
		    rtx target)
{
  /* __builtin_cpu_init () is a nop, so expand to nothing.  */
  if (fcode == RS6000_BUILTIN_CPU_INIT)
    return const0_rtx;

  if (target == 0 || GET_MODE (target) != SImode)
    target = gen_reg_rtx (SImode);

#ifdef TARGET_LIBC_PROVIDES_HWCAP_IN_TCB
  tree arg = TREE_OPERAND (CALL_EXPR_ARG (exp, 0), 0);
  /* Target clones creates an ARRAY_REF instead of STRING_CST, convert it back
     to a STRING_CST.  */
  if (TREE_CODE (arg) == ARRAY_REF
      && TREE_CODE (TREE_OPERAND (arg, 0)) == STRING_CST
      && TREE_CODE (TREE_OPERAND (arg, 1)) == INTEGER_CST
      && compare_tree_int (TREE_OPERAND (arg, 1), 0) == 0)
    arg = TREE_OPERAND (arg, 0);

  if (TREE_CODE (arg) != STRING_CST)
    {
      error ("builtin %qs only accepts a string argument",
	     rs6000_builtin_info[(size_t) fcode].name);
      return const0_rtx;
    }

  if (fcode == RS6000_BUILTIN_CPU_IS)
    {
      const char *cpu = TREE_STRING_POINTER (arg);
      rtx cpuid = NULL_RTX;
      for (size_t i = 0; i < ARRAY_SIZE (cpu_is_info); i++)
	if (strcmp (cpu, cpu_is_info[i].cpu) == 0)
	  {
	    /* The CPUID value in the TCB is offset by _DL_FIRST_PLATFORM.  */
	    cpuid = GEN_INT (cpu_is_info[i].cpuid + _DL_FIRST_PLATFORM);
	    break;
	  }
      if (cpuid == NULL_RTX)
	{
	  /* Invalid CPU argument.  */
	  error ("cpu %qs is an invalid argument to builtin %qs",
		 cpu, rs6000_builtin_info[(size_t) fcode].name);
	  return const0_rtx;
	}

      rtx platform = gen_reg_rtx (SImode);
      rtx tcbmem = gen_const_mem (SImode,
				  gen_rtx_PLUS (Pmode,
						gen_rtx_REG (Pmode, TLS_REGNUM),
						GEN_INT (TCB_PLATFORM_OFFSET)));
      emit_move_insn (platform, tcbmem);
      emit_insn (gen_eqsi3 (target, platform, cpuid));
    }
  else if (fcode == RS6000_BUILTIN_CPU_SUPPORTS)
    {
      const char *hwcap = TREE_STRING_POINTER (arg);
      rtx mask = NULL_RTX;
      int hwcap_offset;
      for (size_t i = 0; i < ARRAY_SIZE (cpu_supports_info); i++)
	if (strcmp (hwcap, cpu_supports_info[i].hwcap) == 0)
	  {
	    mask = GEN_INT (cpu_supports_info[i].mask);
	    hwcap_offset = TCB_HWCAP_OFFSET (cpu_supports_info[i].id);
	    break;
	  }
      if (mask == NULL_RTX)
	{
	  /* Invalid HWCAP argument.  */
	  error ("%s %qs is an invalid argument to builtin %qs",
		 "hwcap", hwcap, rs6000_builtin_info[(size_t) fcode].name);
	  return const0_rtx;
	}

      rtx tcb_hwcap = gen_reg_rtx (SImode);
      rtx tcbmem = gen_const_mem (SImode,
				  gen_rtx_PLUS (Pmode,
						gen_rtx_REG (Pmode, TLS_REGNUM),
						GEN_INT (hwcap_offset)));
      emit_move_insn (tcb_hwcap, tcbmem);
      rtx scratch1 = gen_reg_rtx (SImode);
      emit_insn (gen_rtx_SET (scratch1, gen_rtx_AND (SImode, tcb_hwcap, mask)));
      rtx scratch2 = gen_reg_rtx (SImode);
      emit_insn (gen_eqsi3 (scratch2, scratch1, const0_rtx));
      emit_insn (gen_rtx_SET (target, gen_rtx_XOR (SImode, scratch2, const1_rtx)));
    }
  else
    gcc_unreachable ();

  /* Record that we have expanded a CPU builtin, so that we can later
     emit a reference to the special symbol exported by LIBC to ensure we
     do not link against an old LIBC that doesn't support this feature.  */
  cpu_builtin_p = true;

#else
  warning (0, "builtin %qs needs GLIBC (2.23 and newer) that exports hardware "
	   "capability bits", rs6000_builtin_info[(size_t) fcode].name);
  
  /* For old LIBCs, always return FALSE.  */
  emit_move_insn (target, GEN_INT (0));
#endif /* TARGET_LIBC_PROVIDES_HWCAP_IN_TCB */

  return target;
}

static rtx
rs6000_expand_ternop_builtin (enum insn_code icode, tree exp, rtx target)
{
  rtx pat;
  tree arg0 = CALL_EXPR_ARG (exp, 0);
  tree arg1 = CALL_EXPR_ARG (exp, 1);
  tree arg2 = CALL_EXPR_ARG (exp, 2);
  rtx op0 = expand_normal (arg0);
  rtx op1 = expand_normal (arg1);
  rtx op2 = expand_normal (arg2);
  machine_mode tmode = insn_data[icode].operand[0].mode;
  machine_mode mode0 = insn_data[icode].operand[1].mode;
  machine_mode mode1 = insn_data[icode].operand[2].mode;
  machine_mode mode2 = insn_data[icode].operand[3].mode;

  if (icode == CODE_FOR_nothing)
    /* Builtin not supported on this processor.  */
    return 0;

  /* If we got invalid arguments bail out before generating bad rtl.  */
  if (arg0 == error_mark_node
      || arg1 == error_mark_node
      || arg2 == error_mark_node)
    return const0_rtx;

  /* Check and prepare argument depending on the instruction code.

     Note that a switch statement instead of the sequence of tests
     would be incorrect as many of the CODE_FOR values could be
     CODE_FOR_nothing and that would yield multiple alternatives
     with identical values.  We'd never reach here at runtime in
     this case.  */
  if (icode == CODE_FOR_altivec_vsldoi_v4sf
      || icode == CODE_FOR_altivec_vsldoi_v2df
      || icode == CODE_FOR_altivec_vsldoi_v4si
      || icode == CODE_FOR_altivec_vsldoi_v8hi
      || icode == CODE_FOR_altivec_vsldoi_v16qi)
    {
      /* Only allow 4-bit unsigned literals.  */
      STRIP_NOPS (arg2);
      if (TREE_CODE (arg2) != INTEGER_CST
	  || TREE_INT_CST_LOW (arg2) & ~0xf)
	{
	  error ("argument 3 must be a 4-bit unsigned literal");
	  return CONST0_RTX (tmode);
	}
    }
  else if (icode == CODE_FOR_vsx_xxpermdi_v2df
           || icode == CODE_FOR_vsx_xxpermdi_v2di
           || icode == CODE_FOR_vsx_xxpermdi_v2df_be
           || icode == CODE_FOR_vsx_xxpermdi_v2di_be
           || icode == CODE_FOR_vsx_xxpermdi_v1ti
           || icode == CODE_FOR_vsx_xxpermdi_v4sf
           || icode == CODE_FOR_vsx_xxpermdi_v4si
           || icode == CODE_FOR_vsx_xxpermdi_v8hi
           || icode == CODE_FOR_vsx_xxpermdi_v16qi
           || icode == CODE_FOR_vsx_xxsldwi_v16qi
           || icode == CODE_FOR_vsx_xxsldwi_v8hi
           || icode == CODE_FOR_vsx_xxsldwi_v4si
           || icode == CODE_FOR_vsx_xxsldwi_v4sf
           || icode == CODE_FOR_vsx_xxsldwi_v2di
           || icode == CODE_FOR_vsx_xxsldwi_v2df)
    {
      /* Only allow 2-bit unsigned literals.  */
      STRIP_NOPS (arg2);
      if (TREE_CODE (arg2) != INTEGER_CST
	  || TREE_INT_CST_LOW (arg2) & ~0x3)
	{
	  error ("argument 3 must be a 2-bit unsigned literal");
	  return CONST0_RTX (tmode);
	}
    }
  else if (icode == CODE_FOR_vsx_set_v2df
           || icode == CODE_FOR_vsx_set_v2di
	   || icode == CODE_FOR_bcdadd
	   || icode == CODE_FOR_bcdadd_lt
	   || icode == CODE_FOR_bcdadd_eq
	   || icode == CODE_FOR_bcdadd_gt
	   || icode == CODE_FOR_bcdsub
	   || icode == CODE_FOR_bcdsub_lt
	   || icode == CODE_FOR_bcdsub_eq
	   || icode == CODE_FOR_bcdsub_gt)
    {
      /* Only allow 1-bit unsigned literals.  */
      STRIP_NOPS (arg2);
      if (TREE_CODE (arg2) != INTEGER_CST
	  || TREE_INT_CST_LOW (arg2) & ~0x1)
	{
	  error ("argument 3 must be a 1-bit unsigned literal");
	  return CONST0_RTX (tmode);
	}
    }
  else if (icode == CODE_FOR_dfp_ddedpd_dd
           || icode == CODE_FOR_dfp_ddedpd_td)
    {
      /* Only allow 2-bit unsigned literals where the value is 0 or 2.  */
      STRIP_NOPS (arg0);
      if (TREE_CODE (arg0) != INTEGER_CST
	  || TREE_INT_CST_LOW (arg2) & ~0x3)
	{
	  error ("argument 1 must be 0 or 2");
	  return CONST0_RTX (tmode);
	}
    }
  else if (icode == CODE_FOR_dfp_denbcd_dd
	   || icode == CODE_FOR_dfp_denbcd_td)
    {
      /* Only allow 1-bit unsigned literals.  */
      STRIP_NOPS (arg0);
      if (TREE_CODE (arg0) != INTEGER_CST
	  || TREE_INT_CST_LOW (arg0) & ~0x1)
	{
	  error ("argument 1 must be a 1-bit unsigned literal");
	  return CONST0_RTX (tmode);
	}
    }
  else if (icode == CODE_FOR_dfp_dscli_dd
           || icode == CODE_FOR_dfp_dscli_td
	   || icode == CODE_FOR_dfp_dscri_dd
	   || icode == CODE_FOR_dfp_dscri_td)
    {
      /* Only allow 6-bit unsigned literals.  */
      STRIP_NOPS (arg1);
      if (TREE_CODE (arg1) != INTEGER_CST
	  || TREE_INT_CST_LOW (arg1) & ~0x3f)
	{
	  error ("argument 2 must be a 6-bit unsigned literal");
	  return CONST0_RTX (tmode);
	}
    }
  else if (icode == CODE_FOR_crypto_vshasigmaw
	   || icode == CODE_FOR_crypto_vshasigmad)
    {
      /* Check whether the 2nd and 3rd arguments are integer constants and in
	 range and prepare arguments.  */
      STRIP_NOPS (arg1);
      if (TREE_CODE (arg1) != INTEGER_CST || wi::geu_p (wi::to_wide (arg1), 2))
	{
	  error ("argument 2 must be 0 or 1");
	  return CONST0_RTX (tmode);
	}

      STRIP_NOPS (arg2);
      if (TREE_CODE (arg2) != INTEGER_CST
	  || wi::geu_p (wi::to_wide (arg2), 16))
	{
	  error ("argument 3 must be in the range [0, 15]");
	  return CONST0_RTX (tmode);
	}
    }

  if (target == 0
      || GET_MODE (target) != tmode
      || ! (*insn_data[icode].operand[0].predicate) (target, tmode))
    target = gen_reg_rtx (tmode);

  if (! (*insn_data[icode].operand[1].predicate) (op0, mode0))
    op0 = copy_to_mode_reg (mode0, op0);
  if (! (*insn_data[icode].operand[2].predicate) (op1, mode1))
    op1 = copy_to_mode_reg (mode1, op1);
  if (! (*insn_data[icode].operand[3].predicate) (op2, mode2))
    op2 = copy_to_mode_reg (mode2, op2);

  pat = GEN_FCN (icode) (target, op0, op1, op2);
  if (! pat)
    return 0;
  emit_insn (pat);

  return target;
}


/* Expand the dst builtins.  */
static rtx
altivec_expand_dst_builtin (tree exp, rtx target ATTRIBUTE_UNUSED,
			    bool *expandedp)
{
  tree fndecl = TREE_OPERAND (CALL_EXPR_FN (exp), 0);
  enum rs6000_builtins fcode
    = (enum rs6000_builtins) DECL_MD_FUNCTION_CODE (fndecl);
  tree arg0, arg1, arg2;
  machine_mode mode0, mode1;
  rtx pat, op0, op1, op2;
  const struct builtin_description *d;
  size_t i;

  *expandedp = false;

  /* Handle DST variants.  */
  d = bdesc_dst;
  for (i = 0; i < ARRAY_SIZE (bdesc_dst); i++, d++)
    if (d->code == fcode)
      {
	arg0 = CALL_EXPR_ARG (exp, 0);
	arg1 = CALL_EXPR_ARG (exp, 1);
	arg2 = CALL_EXPR_ARG (exp, 2);
	op0 = expand_normal (arg0);
	op1 = expand_normal (arg1);
	op2 = expand_normal (arg2);
	mode0 = insn_data[d->icode].operand[0].mode;
	mode1 = insn_data[d->icode].operand[1].mode;

	/* Invalid arguments, bail out before generating bad rtl.  */
	if (arg0 == error_mark_node
	    || arg1 == error_mark_node
	    || arg2 == error_mark_node)
	  return const0_rtx;

	*expandedp = true;
	STRIP_NOPS (arg2);
	if (TREE_CODE (arg2) != INTEGER_CST
	    || TREE_INT_CST_LOW (arg2) & ~0x3)
	  {
	    error ("argument to %qs must be a 2-bit unsigned literal", d->name);
	    return const0_rtx;
	  }

	if (! (*insn_data[d->icode].operand[0].predicate) (op0, mode0))
	  op0 = copy_to_mode_reg (Pmode, op0);
	if (! (*insn_data[d->icode].operand[1].predicate) (op1, mode1))
	  op1 = copy_to_mode_reg (mode1, op1);

	pat = GEN_FCN (d->icode) (op0, op1, op2);
	if (pat != 0)
	  emit_insn (pat);

	return NULL_RTX;
      }

  return NULL_RTX;
}

/* Expand vec_init builtin.  */
static rtx
altivec_expand_vec_init_builtin (tree type, tree exp, rtx target)
{
  machine_mode tmode = TYPE_MODE (type);
  machine_mode inner_mode = GET_MODE_INNER (tmode);
  int i, n_elt = GET_MODE_NUNITS (tmode);

  gcc_assert (VECTOR_MODE_P (tmode));
  gcc_assert (n_elt == call_expr_nargs (exp));

  if (!target || !register_operand (target, tmode))
    target = gen_reg_rtx (tmode);

  /* If we have a vector compromised of a single element, such as V1TImode, do
     the initialization directly.  */
  if (n_elt == 1 && GET_MODE_SIZE (tmode) == GET_MODE_SIZE (inner_mode))
    {
      rtx x = expand_normal (CALL_EXPR_ARG (exp, 0));
      emit_move_insn (target, gen_lowpart (tmode, x));
    }
  else
    {
      rtvec v = rtvec_alloc (n_elt);

      for (i = 0; i < n_elt; ++i)
	{
	  rtx x = expand_normal (CALL_EXPR_ARG (exp, i));
	  RTVEC_ELT (v, i) = gen_lowpart (inner_mode, x);
	}

      rs6000_expand_vector_init (target, gen_rtx_PARALLEL (tmode, v));
    }

  return target;
}

/* Return the integer constant in ARG.  Constrain it to be in the range
   of the subparts of VEC_TYPE; issue an error if not.  */

static int
get_element_number (tree vec_type, tree arg)
{
  unsigned HOST_WIDE_INT elt, max = TYPE_VECTOR_SUBPARTS (vec_type) - 1;

  if (!tree_fits_uhwi_p (arg)
      || (elt = tree_to_uhwi (arg), elt > max))
    {
      error ("selector must be an integer constant in the range [0, %wi]", max);
      return 0;
    }

  return elt;
}

/* Expand vec_set builtin.  */
static rtx
altivec_expand_vec_set_builtin (tree exp)
{
  machine_mode tmode, mode1;
  tree arg0, arg1, arg2;
  int elt;
  rtx op0, op1;

  arg0 = CALL_EXPR_ARG (exp, 0);
  arg1 = CALL_EXPR_ARG (exp, 1);
  arg2 = CALL_EXPR_ARG (exp, 2);

  tmode = TYPE_MODE (TREE_TYPE (arg0));
  mode1 = TYPE_MODE (TREE_TYPE (TREE_TYPE (arg0)));
  gcc_assert (VECTOR_MODE_P (tmode));

  op0 = expand_expr (arg0, NULL_RTX, tmode, EXPAND_NORMAL);
  op1 = expand_expr (arg1, NULL_RTX, mode1, EXPAND_NORMAL);
  elt = get_element_number (TREE_TYPE (arg0), arg2);

  if (GET_MODE (op1) != mode1 && GET_MODE (op1) != VOIDmode)
    op1 = convert_modes (mode1, GET_MODE (op1), op1, true);

  op0 = force_reg (tmode, op0);
  op1 = force_reg (mode1, op1);

  rs6000_expand_vector_set (op0, op1, elt);

  return op0;
}

/* Expand vec_ext builtin.  */
static rtx
altivec_expand_vec_ext_builtin (tree exp, rtx target)
{
  machine_mode tmode, mode0;
  tree arg0, arg1;
  rtx op0;
  rtx op1;

  arg0 = CALL_EXPR_ARG (exp, 0);
  arg1 = CALL_EXPR_ARG (exp, 1);

  op0 = expand_normal (arg0);
  op1 = expand_normal (arg1);

  if (TREE_CODE (arg1) == INTEGER_CST)
    {
      unsigned HOST_WIDE_INT elt;
      unsigned HOST_WIDE_INT size = TYPE_VECTOR_SUBPARTS (TREE_TYPE (arg0));
      unsigned int truncated_selector;
      /* Even if !tree_fits_uhwi_p (arg1)), TREE_INT_CST_LOW (arg0)
	 returns low-order bits of INTEGER_CST for modulo indexing.  */
      elt = TREE_INT_CST_LOW (arg1);
      truncated_selector = elt % size;
      op1 = GEN_INT (truncated_selector);
    }

  tmode = TYPE_MODE (TREE_TYPE (TREE_TYPE (arg0)));
  mode0 = TYPE_MODE (TREE_TYPE (arg0));
  gcc_assert (VECTOR_MODE_P (mode0));

  op0 = force_reg (mode0, op0);

  if (optimize || !target || !register_operand (target, tmode))
    target = gen_reg_rtx (tmode);

  rs6000_expand_vector_extract (target, op0, op1);

  return target;
}

/* Expand the builtin in EXP and store the result in TARGET.  Store
   true in *EXPANDEDP if we found a builtin to expand.  */
static rtx
altivec_expand_builtin (tree exp, rtx target, bool *expandedp)
{
  const struct builtin_description *d;
  size_t i;
  enum insn_code icode;
  tree fndecl = TREE_OPERAND (CALL_EXPR_FN (exp), 0);
  tree arg0, arg1, arg2;
  rtx op0, pat;
  machine_mode tmode, mode0;
  enum rs6000_builtins fcode
    = (enum rs6000_builtins) DECL_MD_FUNCTION_CODE (fndecl);

  if (rs6000_overloaded_builtin_p (fcode))
    {
      *expandedp = true;
      error ("unresolved overload for Altivec builtin %qF", fndecl);

      /* Given it is invalid, just generate a normal call.  */
      return expand_call (exp, target, false);
    }

  target = altivec_expand_dst_builtin (exp, target, expandedp);
  if (*expandedp)
    return target;

  *expandedp = true;

  switch (fcode)
    {
    case ALTIVEC_BUILTIN_STVX_V2DF:
      return altivec_expand_stv_builtin (CODE_FOR_altivec_stvx_v2df, exp);
    case ALTIVEC_BUILTIN_STVX_V2DI:
      return altivec_expand_stv_builtin (CODE_FOR_altivec_stvx_v2di, exp);
    case ALTIVEC_BUILTIN_STVX_V4SF:
      return altivec_expand_stv_builtin (CODE_FOR_altivec_stvx_v4sf, exp);
    case ALTIVEC_BUILTIN_STVX:
    case ALTIVEC_BUILTIN_STVX_V4SI:
      return altivec_expand_stv_builtin (CODE_FOR_altivec_stvx_v4si, exp);
    case ALTIVEC_BUILTIN_STVX_V8HI:
      return altivec_expand_stv_builtin (CODE_FOR_altivec_stvx_v8hi, exp);
    case ALTIVEC_BUILTIN_STVX_V16QI:
      return altivec_expand_stv_builtin (CODE_FOR_altivec_stvx_v16qi, exp);
    case ALTIVEC_BUILTIN_STVEBX:
      return altivec_expand_stv_builtin (CODE_FOR_altivec_stvebx, exp);
    case ALTIVEC_BUILTIN_STVEHX:
      return altivec_expand_stv_builtin (CODE_FOR_altivec_stvehx, exp);
    case ALTIVEC_BUILTIN_STVEWX:
      return altivec_expand_stv_builtin (CODE_FOR_altivec_stvewx, exp);
    case ALTIVEC_BUILTIN_STVXL_V2DF:
      return altivec_expand_stv_builtin (CODE_FOR_altivec_stvxl_v2df, exp);
    case ALTIVEC_BUILTIN_STVXL_V2DI:
      return altivec_expand_stv_builtin (CODE_FOR_altivec_stvxl_v2di, exp);
    case ALTIVEC_BUILTIN_STVXL_V4SF:
      return altivec_expand_stv_builtin (CODE_FOR_altivec_stvxl_v4sf, exp);
    case ALTIVEC_BUILTIN_STVXL:
    case ALTIVEC_BUILTIN_STVXL_V4SI:
      return altivec_expand_stv_builtin (CODE_FOR_altivec_stvxl_v4si, exp);
    case ALTIVEC_BUILTIN_STVXL_V8HI:
      return altivec_expand_stv_builtin (CODE_FOR_altivec_stvxl_v8hi, exp);
    case ALTIVEC_BUILTIN_STVXL_V16QI:
      return altivec_expand_stv_builtin (CODE_FOR_altivec_stvxl_v16qi, exp);

    case ALTIVEC_BUILTIN_STVLX:
      return altivec_expand_stv_builtin (CODE_FOR_altivec_stvlx, exp);
    case ALTIVEC_BUILTIN_STVLXL:
      return altivec_expand_stv_builtin (CODE_FOR_altivec_stvlxl, exp);
    case ALTIVEC_BUILTIN_STVRX:
      return altivec_expand_stv_builtin (CODE_FOR_altivec_stvrx, exp);
    case ALTIVEC_BUILTIN_STVRXL:
      return altivec_expand_stv_builtin (CODE_FOR_altivec_stvrxl, exp);

    case P9V_BUILTIN_STXVL:
      return altivec_expand_stxvl_builtin (CODE_FOR_stxvl, exp);

    case P9V_BUILTIN_XST_LEN_R:
      return altivec_expand_stxvl_builtin (CODE_FOR_xst_len_r, exp);

    case VSX_BUILTIN_STXVD2X_V1TI:
      return altivec_expand_stv_builtin (CODE_FOR_vsx_store_v1ti, exp);
    case VSX_BUILTIN_STXVD2X_V2DF:
      return altivec_expand_stv_builtin (CODE_FOR_vsx_store_v2df, exp);
    case VSX_BUILTIN_STXVD2X_V2DI:
      return altivec_expand_stv_builtin (CODE_FOR_vsx_store_v2di, exp);
    case VSX_BUILTIN_STXVW4X_V4SF:
      return altivec_expand_stv_builtin (CODE_FOR_vsx_store_v4sf, exp);
    case VSX_BUILTIN_STXVW4X_V4SI:
      return altivec_expand_stv_builtin (CODE_FOR_vsx_store_v4si, exp);
    case VSX_BUILTIN_STXVW4X_V8HI:
      return altivec_expand_stv_builtin (CODE_FOR_vsx_store_v8hi, exp);
    case VSX_BUILTIN_STXVW4X_V16QI:
      return altivec_expand_stv_builtin (CODE_FOR_vsx_store_v16qi, exp);

    /* For the following on big endian, it's ok to use any appropriate
       unaligned-supporting store, so use a generic expander.  For
       little-endian, the exact element-reversing instruction must
       be used.  */
   case VSX_BUILTIN_ST_ELEMREV_V1TI:
     {
        enum insn_code code = (BYTES_BIG_ENDIAN ? CODE_FOR_vsx_store_v1ti
			       : CODE_FOR_vsx_st_elemrev_v1ti);
        return altivec_expand_stv_builtin (code, exp);
      }
    case VSX_BUILTIN_ST_ELEMREV_V2DF:
      {
	enum insn_code code = (BYTES_BIG_ENDIAN ? CODE_FOR_vsx_store_v2df
			       : CODE_FOR_vsx_st_elemrev_v2df);
	return altivec_expand_stv_builtin (code, exp);
      }
    case VSX_BUILTIN_ST_ELEMREV_V2DI:
      {
	enum insn_code code = (BYTES_BIG_ENDIAN ? CODE_FOR_vsx_store_v2di
			       : CODE_FOR_vsx_st_elemrev_v2di);
	return altivec_expand_stv_builtin (code, exp);
      }
    case VSX_BUILTIN_ST_ELEMREV_V4SF:
      {
	enum insn_code code = (BYTES_BIG_ENDIAN ? CODE_FOR_vsx_store_v4sf
			       : CODE_FOR_vsx_st_elemrev_v4sf);
	return altivec_expand_stv_builtin (code, exp);
      }
    case VSX_BUILTIN_ST_ELEMREV_V4SI:
      {
	enum insn_code code = (BYTES_BIG_ENDIAN ? CODE_FOR_vsx_store_v4si
			       : CODE_FOR_vsx_st_elemrev_v4si);
	return altivec_expand_stv_builtin (code, exp);
      }
    case VSX_BUILTIN_ST_ELEMREV_V8HI:
      {
	enum insn_code code = (BYTES_BIG_ENDIAN ? CODE_FOR_vsx_store_v8hi
			       : CODE_FOR_vsx_st_elemrev_v8hi);
	return altivec_expand_stv_builtin (code, exp);
      }
    case VSX_BUILTIN_ST_ELEMREV_V16QI:
      {
	enum insn_code code = (BYTES_BIG_ENDIAN ? CODE_FOR_vsx_store_v16qi
			       : CODE_FOR_vsx_st_elemrev_v16qi);
	return altivec_expand_stv_builtin (code, exp);
      }

    case ALTIVEC_BUILTIN_MFVSCR:
      icode = CODE_FOR_altivec_mfvscr;
      tmode = insn_data[icode].operand[0].mode;

      if (target == 0
	  || GET_MODE (target) != tmode
	  || ! (*insn_data[icode].operand[0].predicate) (target, tmode))
	target = gen_reg_rtx (tmode);

      pat = GEN_FCN (icode) (target);
      if (! pat)
	return 0;
      emit_insn (pat);
      return target;

    case ALTIVEC_BUILTIN_MTVSCR:
      icode = CODE_FOR_altivec_mtvscr;
      arg0 = CALL_EXPR_ARG (exp, 0);
      op0 = expand_normal (arg0);
      mode0 = insn_data[icode].operand[0].mode;

      /* If we got invalid arguments bail out before generating bad rtl.  */
      if (arg0 == error_mark_node)
	return const0_rtx;

      if (! (*insn_data[icode].operand[0].predicate) (op0, mode0))
	op0 = copy_to_mode_reg (mode0, op0);

      pat = GEN_FCN (icode) (op0);
      if (pat)
	emit_insn (pat);
      return NULL_RTX;

    case ALTIVEC_BUILTIN_DSSALL:
      emit_insn (gen_altivec_dssall ());
      return NULL_RTX;

    case ALTIVEC_BUILTIN_DSS:
      icode = CODE_FOR_altivec_dss;
      arg0 = CALL_EXPR_ARG (exp, 0);
      STRIP_NOPS (arg0);
      op0 = expand_normal (arg0);
      mode0 = insn_data[icode].operand[0].mode;

      /* If we got invalid arguments bail out before generating bad rtl.  */
      if (arg0 == error_mark_node)
	return const0_rtx;

      if (TREE_CODE (arg0) != INTEGER_CST
	  || TREE_INT_CST_LOW (arg0) & ~0x3)
	{
	  error ("argument to %qs must be a 2-bit unsigned literal", "dss");
	  return const0_rtx;
	}

      if (! (*insn_data[icode].operand[0].predicate) (op0, mode0))
	op0 = copy_to_mode_reg (mode0, op0);

      emit_insn (gen_altivec_dss (op0));
      return NULL_RTX;

    case ALTIVEC_BUILTIN_VEC_INIT_V4SI:
    case ALTIVEC_BUILTIN_VEC_INIT_V8HI:
    case ALTIVEC_BUILTIN_VEC_INIT_V16QI:
    case ALTIVEC_BUILTIN_VEC_INIT_V4SF:
    case VSX_BUILTIN_VEC_INIT_V2DF:
    case VSX_BUILTIN_VEC_INIT_V2DI:
    case VSX_BUILTIN_VEC_INIT_V1TI:
      return altivec_expand_vec_init_builtin (TREE_TYPE (exp), exp, target);

    case ALTIVEC_BUILTIN_VEC_SET_V4SI:
    case ALTIVEC_BUILTIN_VEC_SET_V8HI:
    case ALTIVEC_BUILTIN_VEC_SET_V16QI:
    case ALTIVEC_BUILTIN_VEC_SET_V4SF:
    case VSX_BUILTIN_VEC_SET_V2DF:
    case VSX_BUILTIN_VEC_SET_V2DI:
    case VSX_BUILTIN_VEC_SET_V1TI:
      return altivec_expand_vec_set_builtin (exp);

    case ALTIVEC_BUILTIN_VEC_EXT_V4SI:
    case ALTIVEC_BUILTIN_VEC_EXT_V8HI:
    case ALTIVEC_BUILTIN_VEC_EXT_V16QI:
    case ALTIVEC_BUILTIN_VEC_EXT_V4SF:
    case VSX_BUILTIN_VEC_EXT_V2DF:
    case VSX_BUILTIN_VEC_EXT_V2DI:
    case VSX_BUILTIN_VEC_EXT_V1TI:
      return altivec_expand_vec_ext_builtin (exp, target);

    case P9V_BUILTIN_VEC_EXTRACT4B:
      arg1 = CALL_EXPR_ARG (exp, 1);
      STRIP_NOPS (arg1);

      /* Generate a normal call if it is invalid.  */
      if (arg1 == error_mark_node)
	return expand_call (exp, target, false);

      if (TREE_CODE (arg1) != INTEGER_CST || TREE_INT_CST_LOW (arg1) > 12)
	{
	  error ("second argument to %qs must be [0, 12]", "vec_vextract4b");
	  return expand_call (exp, target, false);
	}
      break;

    case P9V_BUILTIN_VEC_INSERT4B:
      arg2 = CALL_EXPR_ARG (exp, 2);
      STRIP_NOPS (arg2);

      /* Generate a normal call if it is invalid.  */
      if (arg2 == error_mark_node)
	return expand_call (exp, target, false);

      if (TREE_CODE (arg2) != INTEGER_CST || TREE_INT_CST_LOW (arg2) > 12)
	{
	  error ("third argument to %qs must be [0, 12]", "vec_vinsert4b");
	  return expand_call (exp, target, false);
	}
      break;

    default:
      break;
      /* Fall through.  */
    }

  /* Expand abs* operations.  */
  d = bdesc_abs;
  for (i = 0; i < ARRAY_SIZE (bdesc_abs); i++, d++)
    if (d->code == fcode)
      return altivec_expand_abs_builtin (d->icode, exp, target);

  /* Expand the AltiVec predicates.  */
  d = bdesc_altivec_preds;
  for (i = 0; i < ARRAY_SIZE (bdesc_altivec_preds); i++, d++)
    if (d->code == fcode)
      return altivec_expand_predicate_builtin (d->icode, exp, target);

  /* LV* are funky.  We initialized them differently.  */
  switch (fcode)
    {
    case ALTIVEC_BUILTIN_LVSL:
      return altivec_expand_lv_builtin (CODE_FOR_altivec_lvsl,
					exp, target, false);
    case ALTIVEC_BUILTIN_LVSR:
      return altivec_expand_lv_builtin (CODE_FOR_altivec_lvsr,
					exp, target, false);
    case ALTIVEC_BUILTIN_LVEBX:
      return altivec_expand_lv_builtin (CODE_FOR_altivec_lvebx,
					exp, target, false);
    case ALTIVEC_BUILTIN_LVEHX:
      return altivec_expand_lv_builtin (CODE_FOR_altivec_lvehx,
					exp, target, false);
    case ALTIVEC_BUILTIN_LVEWX:
      return altivec_expand_lv_builtin (CODE_FOR_altivec_lvewx,
					exp, target, false);
    case ALTIVEC_BUILTIN_LVXL_V2DF:
      return altivec_expand_lv_builtin (CODE_FOR_altivec_lvxl_v2df,
					exp, target, false);
    case ALTIVEC_BUILTIN_LVXL_V2DI:
      return altivec_expand_lv_builtin (CODE_FOR_altivec_lvxl_v2di,
					exp, target, false);
    case ALTIVEC_BUILTIN_LVXL_V4SF:
      return altivec_expand_lv_builtin (CODE_FOR_altivec_lvxl_v4sf,
					exp, target, false);
    case ALTIVEC_BUILTIN_LVXL:
    case ALTIVEC_BUILTIN_LVXL_V4SI:
      return altivec_expand_lv_builtin (CODE_FOR_altivec_lvxl_v4si,
					exp, target, false);
    case ALTIVEC_BUILTIN_LVXL_V8HI:
      return altivec_expand_lv_builtin (CODE_FOR_altivec_lvxl_v8hi,
					exp, target, false);
    case ALTIVEC_BUILTIN_LVXL_V16QI:
      return altivec_expand_lv_builtin (CODE_FOR_altivec_lvxl_v16qi,
					exp, target, false);
    case ALTIVEC_BUILTIN_LVX_V1TI:
      return altivec_expand_lv_builtin (CODE_FOR_altivec_lvx_v1ti,
					exp, target, false);
    case ALTIVEC_BUILTIN_LVX_V2DF:
      return altivec_expand_lv_builtin (CODE_FOR_altivec_lvx_v2df,
					exp, target, false);
    case ALTIVEC_BUILTIN_LVX_V2DI:
      return altivec_expand_lv_builtin (CODE_FOR_altivec_lvx_v2di,
					exp, target, false);
    case ALTIVEC_BUILTIN_LVX_V4SF:
      return altivec_expand_lv_builtin (CODE_FOR_altivec_lvx_v4sf,
					exp, target, false);
    case ALTIVEC_BUILTIN_LVX:
    case ALTIVEC_BUILTIN_LVX_V4SI:
      return altivec_expand_lv_builtin (CODE_FOR_altivec_lvx_v4si,
					exp, target, false);
    case ALTIVEC_BUILTIN_LVX_V8HI:
      return altivec_expand_lv_builtin (CODE_FOR_altivec_lvx_v8hi,
					exp, target, false);
    case ALTIVEC_BUILTIN_LVX_V16QI:
      return altivec_expand_lv_builtin (CODE_FOR_altivec_lvx_v16qi,
					exp, target, false);
    case ALTIVEC_BUILTIN_LVLX:
      return altivec_expand_lv_builtin (CODE_FOR_altivec_lvlx,
					exp, target, true);
    case ALTIVEC_BUILTIN_LVLXL:
      return altivec_expand_lv_builtin (CODE_FOR_altivec_lvlxl,
					exp, target, true);
    case ALTIVEC_BUILTIN_LVRX:
      return altivec_expand_lv_builtin (CODE_FOR_altivec_lvrx,
					exp, target, true);
    case ALTIVEC_BUILTIN_LVRXL:
      return altivec_expand_lv_builtin (CODE_FOR_altivec_lvrxl,
					exp, target, true);
    case VSX_BUILTIN_LXVD2X_V1TI:
      return altivec_expand_lv_builtin (CODE_FOR_vsx_load_v1ti,
					exp, target, false);
    case VSX_BUILTIN_LXVD2X_V2DF:
      return altivec_expand_lv_builtin (CODE_FOR_vsx_load_v2df,
					exp, target, false);
    case VSX_BUILTIN_LXVD2X_V2DI:
      return altivec_expand_lv_builtin (CODE_FOR_vsx_load_v2di,
					exp, target, false);
    case VSX_BUILTIN_LXVW4X_V4SF:
      return altivec_expand_lv_builtin (CODE_FOR_vsx_load_v4sf,
					exp, target, false);
    case VSX_BUILTIN_LXVW4X_V4SI:
      return altivec_expand_lv_builtin (CODE_FOR_vsx_load_v4si,
					exp, target, false);
    case VSX_BUILTIN_LXVW4X_V8HI:
      return altivec_expand_lv_builtin (CODE_FOR_vsx_load_v8hi,
					exp, target, false);
    case VSX_BUILTIN_LXVW4X_V16QI:
      return altivec_expand_lv_builtin (CODE_FOR_vsx_load_v16qi,
					exp, target, false);
    /* For the following on big endian, it's ok to use any appropriate
       unaligned-supporting load, so use a generic expander.  For
       little-endian, the exact element-reversing instruction must
       be used.  */
    case VSX_BUILTIN_LD_ELEMREV_V2DF:
      {
	enum insn_code code = (BYTES_BIG_ENDIAN ? CODE_FOR_vsx_load_v2df
			       : CODE_FOR_vsx_ld_elemrev_v2df);
	return altivec_expand_lv_builtin (code, exp, target, false);
      }
    case VSX_BUILTIN_LD_ELEMREV_V1TI:
      {
	enum insn_code code = (BYTES_BIG_ENDIAN ? CODE_FOR_vsx_load_v1ti
			       : CODE_FOR_vsx_ld_elemrev_v1ti);
	return altivec_expand_lv_builtin (code, exp, target, false);
      }
    case VSX_BUILTIN_LD_ELEMREV_V2DI:
      {
	enum insn_code code = (BYTES_BIG_ENDIAN ? CODE_FOR_vsx_load_v2di
			       : CODE_FOR_vsx_ld_elemrev_v2di);
	return altivec_expand_lv_builtin (code, exp, target, false);
      }
    case VSX_BUILTIN_LD_ELEMREV_V4SF:
      {
	enum insn_code code = (BYTES_BIG_ENDIAN ? CODE_FOR_vsx_load_v4sf
			       : CODE_FOR_vsx_ld_elemrev_v4sf);
	return altivec_expand_lv_builtin (code, exp, target, false);
      }
    case VSX_BUILTIN_LD_ELEMREV_V4SI:
      {
	enum insn_code code = (BYTES_BIG_ENDIAN ? CODE_FOR_vsx_load_v4si
			       : CODE_FOR_vsx_ld_elemrev_v4si);
	return altivec_expand_lv_builtin (code, exp, target, false);
      }
    case VSX_BUILTIN_LD_ELEMREV_V8HI:
      {
	enum insn_code code = (BYTES_BIG_ENDIAN ? CODE_FOR_vsx_load_v8hi
			       : CODE_FOR_vsx_ld_elemrev_v8hi);
	return altivec_expand_lv_builtin (code, exp, target, false);
      }
    case VSX_BUILTIN_LD_ELEMREV_V16QI:
      {
	enum insn_code code = (BYTES_BIG_ENDIAN ? CODE_FOR_vsx_load_v16qi
			       : CODE_FOR_vsx_ld_elemrev_v16qi);
	return altivec_expand_lv_builtin (code, exp, target, false);
      }
      break;
    default:
      break;
      /* Fall through.  */
    }

  *expandedp = false;
  return NULL_RTX;
}

/* Check whether a builtin function is supported in this target
   configuration.  */
bool
rs6000_builtin_is_supported_p (enum rs6000_builtins fncode)
{
  HOST_WIDE_INT fnmask = rs6000_builtin_info[fncode].mask;
  if ((fnmask & rs6000_builtin_mask) != fnmask)
    return false;
  else
    return true;
}

/* Raise an error message for a builtin function that is called without the
   appropriate target options being set.  */

void
rs6000_invalid_builtin (enum rs6000_builtins fncode)
{
  size_t uns_fncode = (size_t) fncode;
  const char *name = rs6000_builtin_info[uns_fncode].name;
  HOST_WIDE_INT fnmask = rs6000_builtin_info[uns_fncode].mask;

  gcc_assert (name != NULL);
  if ((fnmask & RS6000_BTM_CELL) != 0)
    error ("%qs is only valid for the cell processor", name);
  else if ((fnmask & RS6000_BTM_VSX) != 0)
    error ("%qs requires the %qs option", name, "-mvsx");
  else if ((fnmask & RS6000_BTM_HTM) != 0)
    error ("%qs requires the %qs option", name, "-mhtm");
  else if ((fnmask & RS6000_BTM_ALTIVEC) != 0)
    error ("%qs requires the %qs option", name, "-maltivec");
  else if ((fnmask & (RS6000_BTM_DFP | RS6000_BTM_P8_VECTOR))
	   == (RS6000_BTM_DFP | RS6000_BTM_P8_VECTOR))
    error ("%qs requires the %qs and %qs options", name, "-mhard-dfp",
	   "-mpower8-vector");
  else if ((fnmask & RS6000_BTM_DFP) != 0)
    error ("%qs requires the %qs option", name, "-mhard-dfp");
  else if ((fnmask & RS6000_BTM_P8_VECTOR) != 0)
    error ("%qs requires the %qs option", name, "-mpower8-vector");
  else if ((fnmask & (RS6000_BTM_P9_VECTOR | RS6000_BTM_64BIT))
	   == (RS6000_BTM_P9_VECTOR | RS6000_BTM_64BIT))
    error ("%qs requires the %qs and %qs options", name, "-mcpu=power9",
	   "-m64");
  else if ((fnmask & RS6000_BTM_P9_VECTOR) != 0)
    error ("%qs requires the %qs option", name, "-mcpu=power9");
  else if ((fnmask & (RS6000_BTM_P9_MISC | RS6000_BTM_64BIT))
	   == (RS6000_BTM_P9_MISC | RS6000_BTM_64BIT))
    error ("%qs requires the %qs and %qs options", name, "-mcpu=power9",
	   "-m64");
  else if ((fnmask & RS6000_BTM_P9_MISC) == RS6000_BTM_P9_MISC)
    error ("%qs requires the %qs option", name, "-mcpu=power9");
  else if ((fnmask & RS6000_BTM_LDBL128) == RS6000_BTM_LDBL128)
    {
      if (!TARGET_HARD_FLOAT)
	error ("%qs requires the %qs option", name, "-mhard-float");
      else
	error ("%qs requires the %qs option", name,
	       TARGET_IEEEQUAD ? "-mabi=ibmlongdouble" : "-mlong-double-128");
    }
  else if ((fnmask & RS6000_BTM_HARD_FLOAT) != 0)
    error ("%qs requires the %qs option", name, "-mhard-float");
  else if ((fnmask & RS6000_BTM_FLOAT128_HW) != 0)
    error ("%qs requires ISA 3.0 IEEE 128-bit floating point", name);
  else if ((fnmask & RS6000_BTM_FLOAT128) != 0)
    error ("%qs requires the %qs option", name, "%<-mfloat128%>");
  else if ((fnmask & (RS6000_BTM_POPCNTD | RS6000_BTM_POWERPC64))
	   == (RS6000_BTM_POPCNTD | RS6000_BTM_POWERPC64))
    error ("%qs requires the %qs (or newer), and %qs or %qs options",
	   name, "-mcpu=power7", "-m64", "-mpowerpc64");
  else
    error ("%qs is not supported with the current options", name);
}

/* Target hook for early folding of built-ins, shamelessly stolen
   from ia64.c.  */

tree
rs6000_fold_builtin (tree fndecl ATTRIBUTE_UNUSED,
		     int n_args ATTRIBUTE_UNUSED,
		     tree *args ATTRIBUTE_UNUSED,
		     bool ignore ATTRIBUTE_UNUSED)
{
#ifdef SUBTARGET_FOLD_BUILTIN
  return SUBTARGET_FOLD_BUILTIN (fndecl, n_args, args, ignore);
#else
  return NULL_TREE;
#endif
}

/*  Helper function to sort out which built-ins may be valid without having
    a LHS.  */
static bool
rs6000_builtin_valid_without_lhs (enum rs6000_builtins fn_code)
{
  switch (fn_code)
    {
    case ALTIVEC_BUILTIN_STVX_V16QI:
    case ALTIVEC_BUILTIN_STVX_V8HI:
    case ALTIVEC_BUILTIN_STVX_V4SI:
    case ALTIVEC_BUILTIN_STVX_V4SF:
    case ALTIVEC_BUILTIN_STVX_V2DI:
    case ALTIVEC_BUILTIN_STVX_V2DF:
    case VSX_BUILTIN_STXVW4X_V16QI:
    case VSX_BUILTIN_STXVW4X_V8HI:
    case VSX_BUILTIN_STXVW4X_V4SF:
    case VSX_BUILTIN_STXVW4X_V4SI:
    case VSX_BUILTIN_STXVD2X_V2DF:
    case VSX_BUILTIN_STXVD2X_V2DI:
      return true;
    default:
      return false;
    }
}

/* Helper function to handle the gimple folding of a vector compare
   operation.  This sets up true/false vectors, and uses the
   VEC_COND_EXPR operation.
   CODE indicates which comparison is to be made. (EQ, GT, ...).
   TYPE indicates the type of the result.  */
static tree
fold_build_vec_cmp (tree_code code, tree type,
		    tree arg0, tree arg1)
{
  tree cmp_type = truth_type_for (type);
  tree zero_vec = build_zero_cst (type);
  tree minus_one_vec = build_minus_one_cst (type);
  tree cmp = fold_build2 (code, cmp_type, arg0, arg1);
  return fold_build3 (VEC_COND_EXPR, type, cmp, minus_one_vec, zero_vec);
}

/* Helper function to handle the in-between steps for the
   vector compare built-ins.  */
static void
fold_compare_helper (gimple_stmt_iterator *gsi, tree_code code, gimple *stmt)
{
  tree arg0 = gimple_call_arg (stmt, 0);
  tree arg1 = gimple_call_arg (stmt, 1);
  tree lhs = gimple_call_lhs (stmt);
  tree cmp = fold_build_vec_cmp (code, TREE_TYPE (lhs), arg0, arg1);
  gimple *g = gimple_build_assign (lhs, cmp);
  gimple_set_location (g, gimple_location (stmt));
  gsi_replace (gsi, g, true);
}

/* Helper function to map V2DF and V4SF types to their
 integral equivalents (V2DI and V4SI).  */
tree map_to_integral_tree_type (tree input_tree_type)
{
  if (INTEGRAL_TYPE_P (TREE_TYPE (input_tree_type)))
    return input_tree_type;
  else
    {
      if (types_compatible_p (TREE_TYPE (input_tree_type),
			      TREE_TYPE (V2DF_type_node)))
	return V2DI_type_node;
      else if (types_compatible_p (TREE_TYPE (input_tree_type),
				   TREE_TYPE (V4SF_type_node)))
	return V4SI_type_node;
      else
	gcc_unreachable ();
    }
}

/* Helper function to handle the vector merge[hl] built-ins.  The
   implementation difference between h and l versions for this code are in
   the values used when building of the permute vector for high word versus
   low word merge.  The variance is keyed off the use_high parameter.  */
static void
fold_mergehl_helper (gimple_stmt_iterator *gsi, gimple *stmt, int use_high)
{
  tree arg0 = gimple_call_arg (stmt, 0);
  tree arg1 = gimple_call_arg (stmt, 1);
  tree lhs = gimple_call_lhs (stmt);
  tree lhs_type = TREE_TYPE (lhs);
  int n_elts = TYPE_VECTOR_SUBPARTS (lhs_type);
  int midpoint = n_elts / 2;
  int offset = 0;

  if (use_high == 1)
    offset = midpoint;

  /* The permute_type will match the lhs for integral types.  For double and
     float types, the permute type needs to map to the V2 or V4 type that
     matches size.  */
  tree permute_type;
  permute_type = map_to_integral_tree_type (lhs_type);
  tree_vector_builder elts (permute_type, VECTOR_CST_NELTS (arg0), 1);

  for (int i = 0; i < midpoint; i++)
    {
      elts.safe_push (build_int_cst (TREE_TYPE (permute_type),
				     offset + i));
      elts.safe_push (build_int_cst (TREE_TYPE (permute_type),
				     offset + n_elts + i));
    }

  tree permute = elts.build ();

  gimple *g = gimple_build_assign (lhs, VEC_PERM_EXPR, arg0, arg1, permute);
  gimple_set_location (g, gimple_location (stmt));
  gsi_replace (gsi, g, true);
}

/* Helper function to handle the vector merge[eo] built-ins.  */
static void
fold_mergeeo_helper (gimple_stmt_iterator *gsi, gimple *stmt, int use_odd)
{
  tree arg0 = gimple_call_arg (stmt, 0);
  tree arg1 = gimple_call_arg (stmt, 1);
  tree lhs = gimple_call_lhs (stmt);
  tree lhs_type = TREE_TYPE (lhs);
  int n_elts = TYPE_VECTOR_SUBPARTS (lhs_type);

  /* The permute_type will match the lhs for integral types.  For double and
     float types, the permute type needs to map to the V2 or V4 type that
     matches size.  */
  tree permute_type;
  permute_type = map_to_integral_tree_type (lhs_type);

  tree_vector_builder elts (permute_type, VECTOR_CST_NELTS (arg0), 1);

 /* Build the permute vector.  */
  for (int i = 0; i < n_elts / 2; i++)
    {
      elts.safe_push (build_int_cst (TREE_TYPE (permute_type),
				     2*i + use_odd));
      elts.safe_push (build_int_cst (TREE_TYPE (permute_type),
				     2*i + use_odd + n_elts));
    }

  tree permute = elts.build ();

  gimple *g = gimple_build_assign (lhs, VEC_PERM_EXPR, arg0, arg1, permute);
  gimple_set_location (g, gimple_location (stmt));
  gsi_replace (gsi, g, true);
}

/* Fold a machine-dependent built-in in GIMPLE.  (For folding into
   a constant, use rs6000_fold_builtin.)  */

bool
rs6000_gimple_fold_builtin (gimple_stmt_iterator *gsi)
{
  gimple *stmt = gsi_stmt (*gsi);
  tree fndecl = gimple_call_fndecl (stmt);
  gcc_checking_assert (fndecl && DECL_BUILT_IN_CLASS (fndecl) == BUILT_IN_MD);
  enum rs6000_builtins fn_code
    = (enum rs6000_builtins) DECL_MD_FUNCTION_CODE (fndecl);
  tree arg0, arg1, lhs, temp;
  enum tree_code bcode;
  gimple *g;

  size_t uns_fncode = (size_t) fn_code;
  enum insn_code icode = rs6000_builtin_info[uns_fncode].icode;
  const char *fn_name1 = rs6000_builtin_info[uns_fncode].name;
  const char *fn_name2 = (icode != CODE_FOR_nothing)
			  ? get_insn_name ((int) icode)
			  : "nothing";

  if (TARGET_DEBUG_BUILTIN)
      fprintf (stderr, "rs6000_gimple_fold_builtin %d %s %s\n",
	       fn_code, fn_name1, fn_name2);

  if (!rs6000_fold_gimple)
    return false;

  /* Prevent gimple folding for code that does not have a LHS, unless it is
     allowed per the rs6000_builtin_valid_without_lhs helper function.  */
  if (!gimple_call_lhs (stmt) && !rs6000_builtin_valid_without_lhs (fn_code))
    return false;

  /* Don't fold invalid builtins, let rs6000_expand_builtin diagnose it.  */
  HOST_WIDE_INT mask = rs6000_builtin_info[uns_fncode].mask;
  bool func_valid_p = (rs6000_builtin_mask & mask) == mask;
  if (!func_valid_p)
    return false;

  switch (fn_code)
    {
    /* Flavors of vec_add.  We deliberately don't expand
       P8V_BUILTIN_VADDUQM as it gets lowered from V1TImode to
       TImode, resulting in much poorer code generation.  */
    case ALTIVEC_BUILTIN_VADDUBM:
    case ALTIVEC_BUILTIN_VADDUHM:
    case ALTIVEC_BUILTIN_VADDUWM:
    case P8V_BUILTIN_VADDUDM:
    case ALTIVEC_BUILTIN_VADDFP:
    case VSX_BUILTIN_XVADDDP:
      bcode = PLUS_EXPR;
    do_binary:
      arg0 = gimple_call_arg (stmt, 0);
      arg1 = gimple_call_arg (stmt, 1);
      lhs = gimple_call_lhs (stmt);
      if (INTEGRAL_TYPE_P (TREE_TYPE (TREE_TYPE (lhs)))
	  && !TYPE_OVERFLOW_WRAPS (TREE_TYPE (TREE_TYPE (lhs))))
	{
	  /* Ensure the binary operation is performed in a type
	     that wraps if it is integral type.  */
	  gimple_seq stmts = NULL;
	  tree type = unsigned_type_for (TREE_TYPE (lhs));
	  tree uarg0 = gimple_build (&stmts, VIEW_CONVERT_EXPR,
				     type, arg0);
	  tree uarg1 = gimple_build (&stmts, VIEW_CONVERT_EXPR,
				     type, arg1);
	  tree res = gimple_build (&stmts, gimple_location (stmt), bcode,
				   type, uarg0, uarg1);
	  gsi_insert_seq_before (gsi, stmts, GSI_SAME_STMT);
	  g = gimple_build_assign (lhs, VIEW_CONVERT_EXPR,
				   build1 (VIEW_CONVERT_EXPR,
					   TREE_TYPE (lhs), res));
	  gsi_replace (gsi, g, true);
	  return true;
	}
      g = gimple_build_assign (lhs, bcode, arg0, arg1);
      gimple_set_location (g, gimple_location (stmt));
      gsi_replace (gsi, g, true);
      return true;
    /* Flavors of vec_sub.  We deliberately don't expand
       P8V_BUILTIN_VSUBUQM. */
    case ALTIVEC_BUILTIN_VSUBUBM:
    case ALTIVEC_BUILTIN_VSUBUHM:
    case ALTIVEC_BUILTIN_VSUBUWM:
    case P8V_BUILTIN_VSUBUDM:
    case ALTIVEC_BUILTIN_VSUBFP:
    case VSX_BUILTIN_XVSUBDP:
      bcode = MINUS_EXPR;
      goto do_binary;
    case VSX_BUILTIN_XVMULSP:
    case VSX_BUILTIN_XVMULDP:
      arg0 = gimple_call_arg (stmt, 0);
      arg1 = gimple_call_arg (stmt, 1);
      lhs = gimple_call_lhs (stmt);
      g = gimple_build_assign (lhs, MULT_EXPR, arg0, arg1);
      gimple_set_location (g, gimple_location (stmt));
      gsi_replace (gsi, g, true);
      return true;
    /* Even element flavors of vec_mul (signed). */
    case ALTIVEC_BUILTIN_VMULESB:
    case ALTIVEC_BUILTIN_VMULESH:
    case P8V_BUILTIN_VMULESW:
    /* Even element flavors of vec_mul (unsigned).  */
    case ALTIVEC_BUILTIN_VMULEUB:
    case ALTIVEC_BUILTIN_VMULEUH:
    case P8V_BUILTIN_VMULEUW:
      arg0 = gimple_call_arg (stmt, 0);
      arg1 = gimple_call_arg (stmt, 1);
      lhs = gimple_call_lhs (stmt);
      g = gimple_build_assign (lhs, VEC_WIDEN_MULT_EVEN_EXPR, arg0, arg1);
      gimple_set_location (g, gimple_location (stmt));
      gsi_replace (gsi, g, true);
      return true;
    /* Odd element flavors of vec_mul (signed).  */
    case ALTIVEC_BUILTIN_VMULOSB:
    case ALTIVEC_BUILTIN_VMULOSH:
    case P8V_BUILTIN_VMULOSW:
    /* Odd element flavors of vec_mul (unsigned). */
    case ALTIVEC_BUILTIN_VMULOUB:
    case ALTIVEC_BUILTIN_VMULOUH:
    case P8V_BUILTIN_VMULOUW:
      arg0 = gimple_call_arg (stmt, 0);
      arg1 = gimple_call_arg (stmt, 1);
      lhs = gimple_call_lhs (stmt);
      g = gimple_build_assign (lhs, VEC_WIDEN_MULT_ODD_EXPR, arg0, arg1);
      gimple_set_location (g, gimple_location (stmt));
      gsi_replace (gsi, g, true);
      return true;
    /* Flavors of vec_div (Integer).  */
    case VSX_BUILTIN_DIV_V2DI:
    case VSX_BUILTIN_UDIV_V2DI:
      arg0 = gimple_call_arg (stmt, 0);
      arg1 = gimple_call_arg (stmt, 1);
      lhs = gimple_call_lhs (stmt);
      g = gimple_build_assign (lhs, TRUNC_DIV_EXPR, arg0, arg1);
      gimple_set_location (g, gimple_location (stmt));
      gsi_replace (gsi, g, true);
      return true;
    /* Flavors of vec_div (Float).  */
    case VSX_BUILTIN_XVDIVSP:
    case VSX_BUILTIN_XVDIVDP:
      arg0 = gimple_call_arg (stmt, 0);
      arg1 = gimple_call_arg (stmt, 1);
      lhs = gimple_call_lhs (stmt);
      g = gimple_build_assign (lhs, RDIV_EXPR, arg0, arg1);
      gimple_set_location (g, gimple_location (stmt));
      gsi_replace (gsi, g, true);
      return true;
    /* Flavors of vec_and.  */
    case ALTIVEC_BUILTIN_VAND:
      arg0 = gimple_call_arg (stmt, 0);
      arg1 = gimple_call_arg (stmt, 1);
      lhs = gimple_call_lhs (stmt);
      g = gimple_build_assign (lhs, BIT_AND_EXPR, arg0, arg1);
      gimple_set_location (g, gimple_location (stmt));
      gsi_replace (gsi, g, true);
      return true;
    /* Flavors of vec_andc.  */
    case ALTIVEC_BUILTIN_VANDC:
      arg0 = gimple_call_arg (stmt, 0);
      arg1 = gimple_call_arg (stmt, 1);
      lhs = gimple_call_lhs (stmt);
      temp = create_tmp_reg_or_ssa_name (TREE_TYPE (arg1));
      g = gimple_build_assign (temp, BIT_NOT_EXPR, arg1);
      gimple_set_location (g, gimple_location (stmt));
      gsi_insert_before (gsi, g, GSI_SAME_STMT);
      g = gimple_build_assign (lhs, BIT_AND_EXPR, arg0, temp);
      gimple_set_location (g, gimple_location (stmt));
      gsi_replace (gsi, g, true);
      return true;
    /* Flavors of vec_nand.  */
    case P8V_BUILTIN_VEC_NAND:
    case P8V_BUILTIN_NAND_V16QI:
    case P8V_BUILTIN_NAND_V8HI:
    case P8V_BUILTIN_NAND_V4SI:
    case P8V_BUILTIN_NAND_V4SF:
    case P8V_BUILTIN_NAND_V2DF:
    case P8V_BUILTIN_NAND_V2DI:
      arg0 = gimple_call_arg (stmt, 0);
      arg1 = gimple_call_arg (stmt, 1);
      lhs = gimple_call_lhs (stmt);
      temp = create_tmp_reg_or_ssa_name (TREE_TYPE (arg1));
      g = gimple_build_assign (temp, BIT_AND_EXPR, arg0, arg1);
      gimple_set_location (g, gimple_location (stmt));
      gsi_insert_before (gsi, g, GSI_SAME_STMT);
      g = gimple_build_assign (lhs, BIT_NOT_EXPR, temp);
      gimple_set_location (g, gimple_location (stmt));
      gsi_replace (gsi, g, true);
      return true;
    /* Flavors of vec_or.  */
    case ALTIVEC_BUILTIN_VOR:
      arg0 = gimple_call_arg (stmt, 0);
      arg1 = gimple_call_arg (stmt, 1);
      lhs = gimple_call_lhs (stmt);
      g = gimple_build_assign (lhs, BIT_IOR_EXPR, arg0, arg1);
      gimple_set_location (g, gimple_location (stmt));
      gsi_replace (gsi, g, true);
      return true;
    /* flavors of vec_orc.  */
    case P8V_BUILTIN_ORC_V16QI:
    case P8V_BUILTIN_ORC_V8HI:
    case P8V_BUILTIN_ORC_V4SI:
    case P8V_BUILTIN_ORC_V4SF:
    case P8V_BUILTIN_ORC_V2DF:
    case P8V_BUILTIN_ORC_V2DI:
      arg0 = gimple_call_arg (stmt, 0);
      arg1 = gimple_call_arg (stmt, 1);
      lhs = gimple_call_lhs (stmt);
      temp = create_tmp_reg_or_ssa_name (TREE_TYPE (arg1));
      g = gimple_build_assign (temp, BIT_NOT_EXPR, arg1);
      gimple_set_location (g, gimple_location (stmt));
      gsi_insert_before (gsi, g, GSI_SAME_STMT);
      g = gimple_build_assign (lhs, BIT_IOR_EXPR, arg0, temp);
      gimple_set_location (g, gimple_location (stmt));
      gsi_replace (gsi, g, true);
      return true;
    /* Flavors of vec_xor.  */
    case ALTIVEC_BUILTIN_VXOR:
      arg0 = gimple_call_arg (stmt, 0);
      arg1 = gimple_call_arg (stmt, 1);
      lhs = gimple_call_lhs (stmt);
      g = gimple_build_assign (lhs, BIT_XOR_EXPR, arg0, arg1);
      gimple_set_location (g, gimple_location (stmt));
      gsi_replace (gsi, g, true);
      return true;
    /* Flavors of vec_nor.  */
    case ALTIVEC_BUILTIN_VNOR:
      arg0 = gimple_call_arg (stmt, 0);
      arg1 = gimple_call_arg (stmt, 1);
      lhs = gimple_call_lhs (stmt);
      temp = create_tmp_reg_or_ssa_name (TREE_TYPE (arg1));
      g = gimple_build_assign (temp, BIT_IOR_EXPR, arg0, arg1);
      gimple_set_location (g, gimple_location (stmt));
      gsi_insert_before (gsi, g, GSI_SAME_STMT);
      g = gimple_build_assign (lhs, BIT_NOT_EXPR, temp);
      gimple_set_location (g, gimple_location (stmt));
      gsi_replace (gsi, g, true);
      return true;
    /* flavors of vec_abs.  */
    case ALTIVEC_BUILTIN_ABS_V16QI:
    case ALTIVEC_BUILTIN_ABS_V8HI:
    case ALTIVEC_BUILTIN_ABS_V4SI:
    case ALTIVEC_BUILTIN_ABS_V4SF:
    case P8V_BUILTIN_ABS_V2DI:
    case VSX_BUILTIN_XVABSDP:
      arg0 = gimple_call_arg (stmt, 0);
      if (INTEGRAL_TYPE_P (TREE_TYPE (TREE_TYPE (arg0)))
	  && !TYPE_OVERFLOW_WRAPS (TREE_TYPE (TREE_TYPE (arg0))))
	return false;
      lhs = gimple_call_lhs (stmt);
      g = gimple_build_assign (lhs, ABS_EXPR, arg0);
      gimple_set_location (g, gimple_location (stmt));
      gsi_replace (gsi, g, true);
      return true;
    /* flavors of vec_min.  */
    case VSX_BUILTIN_XVMINDP:
    case P8V_BUILTIN_VMINSD:
    case P8V_BUILTIN_VMINUD:
    case ALTIVEC_BUILTIN_VMINSB:
    case ALTIVEC_BUILTIN_VMINSH:
    case ALTIVEC_BUILTIN_VMINSW:
    case ALTIVEC_BUILTIN_VMINUB:
    case ALTIVEC_BUILTIN_VMINUH:
    case ALTIVEC_BUILTIN_VMINUW:
    case ALTIVEC_BUILTIN_VMINFP:
      arg0 = gimple_call_arg (stmt, 0);
      arg1 = gimple_call_arg (stmt, 1);
      lhs = gimple_call_lhs (stmt);
      g = gimple_build_assign (lhs, MIN_EXPR, arg0, arg1);
      gimple_set_location (g, gimple_location (stmt));
      gsi_replace (gsi, g, true);
      return true;
    /* flavors of vec_max.  */
    case VSX_BUILTIN_XVMAXDP:
    case P8V_BUILTIN_VMAXSD:
    case P8V_BUILTIN_VMAXUD:
    case ALTIVEC_BUILTIN_VMAXSB:
    case ALTIVEC_BUILTIN_VMAXSH:
    case ALTIVEC_BUILTIN_VMAXSW:
    case ALTIVEC_BUILTIN_VMAXUB:
    case ALTIVEC_BUILTIN_VMAXUH:
    case ALTIVEC_BUILTIN_VMAXUW:
    case ALTIVEC_BUILTIN_VMAXFP:
      arg0 = gimple_call_arg (stmt, 0);
      arg1 = gimple_call_arg (stmt, 1);
      lhs = gimple_call_lhs (stmt);
      g = gimple_build_assign (lhs, MAX_EXPR, arg0, arg1);
      gimple_set_location (g, gimple_location (stmt));
      gsi_replace (gsi, g, true);
      return true;
    /* Flavors of vec_eqv.  */
    case P8V_BUILTIN_EQV_V16QI:
    case P8V_BUILTIN_EQV_V8HI:
    case P8V_BUILTIN_EQV_V4SI:
    case P8V_BUILTIN_EQV_V4SF:
    case P8V_BUILTIN_EQV_V2DF:
    case P8V_BUILTIN_EQV_V2DI:
      arg0 = gimple_call_arg (stmt, 0);
      arg1 = gimple_call_arg (stmt, 1);
      lhs = gimple_call_lhs (stmt);
      temp = create_tmp_reg_or_ssa_name (TREE_TYPE (arg1));
      g = gimple_build_assign (temp, BIT_XOR_EXPR, arg0, arg1);
      gimple_set_location (g, gimple_location (stmt));
      gsi_insert_before (gsi, g, GSI_SAME_STMT);
      g = gimple_build_assign (lhs, BIT_NOT_EXPR, temp);
      gimple_set_location (g, gimple_location (stmt));
      gsi_replace (gsi, g, true);
      return true;
    /* Flavors of vec_rotate_left.  */
    case ALTIVEC_BUILTIN_VRLB:
    case ALTIVEC_BUILTIN_VRLH:
    case ALTIVEC_BUILTIN_VRLW:
    case P8V_BUILTIN_VRLD:
      arg0 = gimple_call_arg (stmt, 0);
      arg1 = gimple_call_arg (stmt, 1);
      lhs = gimple_call_lhs (stmt);
      g = gimple_build_assign (lhs, LROTATE_EXPR, arg0, arg1);
      gimple_set_location (g, gimple_location (stmt));
      gsi_replace (gsi, g, true);
      return true;
  /* Flavors of vector shift right algebraic.
     vec_sra{b,h,w} -> vsra{b,h,w}.  */
    case ALTIVEC_BUILTIN_VSRAB:
    case ALTIVEC_BUILTIN_VSRAH:
    case ALTIVEC_BUILTIN_VSRAW:
    case P8V_BUILTIN_VSRAD:
      {
	arg0 = gimple_call_arg (stmt, 0);
	arg1 = gimple_call_arg (stmt, 1);
	lhs = gimple_call_lhs (stmt);
	tree arg1_type = TREE_TYPE (arg1);
	tree unsigned_arg1_type = unsigned_type_for (TREE_TYPE (arg1));
	tree unsigned_element_type = unsigned_type_for (TREE_TYPE (arg1_type));
	location_t loc = gimple_location (stmt);
	/* Force arg1 into the range valid matching the arg0 type.  */
	/* Build a vector consisting of the max valid bit-size values.  */
	int n_elts = VECTOR_CST_NELTS (arg1);
	tree element_size = build_int_cst (unsigned_element_type,
					   128 / n_elts);
	tree_vector_builder elts (unsigned_arg1_type, n_elts, 1);
	for (int i = 0; i < n_elts; i++)
	  elts.safe_push (element_size);
	tree modulo_tree = elts.build ();
	/* Modulo the provided shift value against that vector.  */
	gimple_seq stmts = NULL;
	tree unsigned_arg1 = gimple_build (&stmts, VIEW_CONVERT_EXPR,
					   unsigned_arg1_type, arg1);
	tree new_arg1 = gimple_build (&stmts, loc, TRUNC_MOD_EXPR,
				      unsigned_arg1_type, unsigned_arg1,
				      modulo_tree);
	gsi_insert_seq_before (gsi, stmts, GSI_SAME_STMT);
	/* And finally, do the shift.  */
	g = gimple_build_assign (lhs, RSHIFT_EXPR, arg0, new_arg1);
	gimple_set_location (g, loc);
	gsi_replace (gsi, g, true);
	return true;
      }
   /* Flavors of vector shift left.
      builtin_altivec_vsl{b,h,w} -> vsl{b,h,w}.  */
    case ALTIVEC_BUILTIN_VSLB:
    case ALTIVEC_BUILTIN_VSLH:
    case ALTIVEC_BUILTIN_VSLW:
    case P8V_BUILTIN_VSLD:
      {
	location_t loc;
	gimple_seq stmts = NULL;
	arg0 = gimple_call_arg (stmt, 0);
	tree arg0_type = TREE_TYPE (arg0);
	if (INTEGRAL_TYPE_P (TREE_TYPE (arg0_type))
	    && !TYPE_OVERFLOW_WRAPS (TREE_TYPE (arg0_type)))
	  return false;
	arg1 = gimple_call_arg (stmt, 1);
	tree arg1_type = TREE_TYPE (arg1);
	tree unsigned_arg1_type = unsigned_type_for (TREE_TYPE (arg1));
	tree unsigned_element_type = unsigned_type_for (TREE_TYPE (arg1_type));
	loc = gimple_location (stmt);
	lhs = gimple_call_lhs (stmt);
	/* Force arg1 into the range valid matching the arg0 type.  */
	/* Build a vector consisting of the max valid bit-size values.  */
	int n_elts = VECTOR_CST_NELTS (arg1);
	int tree_size_in_bits = TREE_INT_CST_LOW (size_in_bytes (arg1_type))
				* BITS_PER_UNIT;
	tree element_size = build_int_cst (unsigned_element_type,
					   tree_size_in_bits / n_elts);
	tree_vector_builder elts (unsigned_type_for (arg1_type), n_elts, 1);
	for (int i = 0; i < n_elts; i++)
	  elts.safe_push (element_size);
	tree modulo_tree = elts.build ();
	/* Modulo the provided shift value against that vector.  */
	tree unsigned_arg1 = gimple_build (&stmts, VIEW_CONVERT_EXPR,
					   unsigned_arg1_type, arg1);
	tree new_arg1 = gimple_build (&stmts, loc, TRUNC_MOD_EXPR,
				      unsigned_arg1_type, unsigned_arg1,
				      modulo_tree);
	gsi_insert_seq_before (gsi, stmts, GSI_SAME_STMT);
	/* And finally, do the shift.  */
	g = gimple_build_assign (lhs, LSHIFT_EXPR, arg0, new_arg1);
	gimple_set_location (g, gimple_location (stmt));
	gsi_replace (gsi, g, true);
	return true;
      }
    /* Flavors of vector shift right.  */
    case ALTIVEC_BUILTIN_VSRB:
    case ALTIVEC_BUILTIN_VSRH:
    case ALTIVEC_BUILTIN_VSRW:
    case P8V_BUILTIN_VSRD:
      {
	arg0 = gimple_call_arg (stmt, 0);
	arg1 = gimple_call_arg (stmt, 1);
	lhs = gimple_call_lhs (stmt);
	tree arg1_type = TREE_TYPE (arg1);
	tree unsigned_arg1_type = unsigned_type_for (TREE_TYPE (arg1));
	tree unsigned_element_type = unsigned_type_for (TREE_TYPE (arg1_type));
	location_t loc = gimple_location (stmt);
	gimple_seq stmts = NULL;
	/* Convert arg0 to unsigned.  */
	tree arg0_unsigned
	  = gimple_build (&stmts, VIEW_CONVERT_EXPR,
			  unsigned_type_for (TREE_TYPE (arg0)), arg0);
	/* Force arg1 into the range valid matching the arg0 type.  */
	/* Build a vector consisting of the max valid bit-size values.  */
	int n_elts = VECTOR_CST_NELTS (arg1);
	tree element_size = build_int_cst (unsigned_element_type,
					   128 / n_elts);
	tree_vector_builder elts (unsigned_arg1_type, n_elts, 1);
	for (int i = 0; i < n_elts; i++)
	  elts.safe_push (element_size);
	tree modulo_tree = elts.build ();
	/* Modulo the provided shift value against that vector.  */
	tree unsigned_arg1 = gimple_build (&stmts, VIEW_CONVERT_EXPR,
					   unsigned_arg1_type, arg1);
	tree new_arg1 = gimple_build (&stmts, loc, TRUNC_MOD_EXPR,
				      unsigned_arg1_type, unsigned_arg1,
				      modulo_tree);
	/* Do the shift.  */
	tree res
	  = gimple_build (&stmts, RSHIFT_EXPR,
			  TREE_TYPE (arg0_unsigned), arg0_unsigned, new_arg1);
	/* Convert result back to the lhs type.  */
	res = gimple_build (&stmts, VIEW_CONVERT_EXPR, TREE_TYPE (lhs), res);
	gsi_insert_seq_before (gsi, stmts, GSI_SAME_STMT);
	update_call_from_tree (gsi, res);
	return true;
      }
    /* Vector loads.  */
    case ALTIVEC_BUILTIN_LVX_V16QI:
    case ALTIVEC_BUILTIN_LVX_V8HI:
    case ALTIVEC_BUILTIN_LVX_V4SI:
    case ALTIVEC_BUILTIN_LVX_V4SF:
    case ALTIVEC_BUILTIN_LVX_V2DI:
    case ALTIVEC_BUILTIN_LVX_V2DF:
    case ALTIVEC_BUILTIN_LVX_V1TI:
      {
	arg0 = gimple_call_arg (stmt, 0);  // offset
	arg1 = gimple_call_arg (stmt, 1);  // address
	lhs = gimple_call_lhs (stmt);
	location_t loc = gimple_location (stmt);
	/* Since arg1 may be cast to a different type, just use ptr_type_node
	   here instead of trying to enforce TBAA on pointer types.  */
	tree arg1_type = ptr_type_node;
	tree lhs_type = TREE_TYPE (lhs);
	/* POINTER_PLUS_EXPR wants the offset to be of type 'sizetype'.  Create
	   the tree using the value from arg0.  The resulting type will match
	   the type of arg1.  */
	gimple_seq stmts = NULL;
	tree temp_offset = gimple_convert (&stmts, loc, sizetype, arg0);
	tree temp_addr = gimple_build (&stmts, loc, POINTER_PLUS_EXPR,
				       arg1_type, arg1, temp_offset);
	/* Mask off any lower bits from the address.  */
	tree aligned_addr = gimple_build (&stmts, loc, BIT_AND_EXPR,
					  arg1_type, temp_addr,
					  build_int_cst (arg1_type, -16));
	gsi_insert_seq_before (gsi, stmts, GSI_SAME_STMT);
	if (!is_gimple_mem_ref_addr (aligned_addr))
	  {
	    tree t = make_ssa_name (TREE_TYPE (aligned_addr));
	    gimple *g = gimple_build_assign (t, aligned_addr);
	    gsi_insert_before (gsi, g, GSI_SAME_STMT);
	    aligned_addr = t;
	  }
	/* Use the build2 helper to set up the mem_ref.  The MEM_REF could also
	   take an offset, but since we've already incorporated the offset
	   above, here we just pass in a zero.  */
	gimple *g
	  = gimple_build_assign (lhs, build2 (MEM_REF, lhs_type, aligned_addr,
					      build_int_cst (arg1_type, 0)));
	gimple_set_location (g, loc);
	gsi_replace (gsi, g, true);
	return true;
      }
    /* Vector stores.  */
    case ALTIVEC_BUILTIN_STVX_V16QI:
    case ALTIVEC_BUILTIN_STVX_V8HI:
    case ALTIVEC_BUILTIN_STVX_V4SI:
    case ALTIVEC_BUILTIN_STVX_V4SF:
    case ALTIVEC_BUILTIN_STVX_V2DI:
    case ALTIVEC_BUILTIN_STVX_V2DF:
      {
	arg0 = gimple_call_arg (stmt, 0); /* Value to be stored.  */
	arg1 = gimple_call_arg (stmt, 1); /* Offset.  */
	tree arg2 = gimple_call_arg (stmt, 2); /* Store-to address.  */
	location_t loc = gimple_location (stmt);
	tree arg0_type = TREE_TYPE (arg0);
	/* Use ptr_type_node (no TBAA) for the arg2_type.
	   FIXME: (Richard)  "A proper fix would be to transition this type as
	   seen from the frontend to GIMPLE, for example in a similar way we
	   do for MEM_REFs by piggy-backing that on an extra argument, a
	   constant zero pointer of the alias pointer type to use (which would
	   also serve as a type indicator of the store itself).  I'd use a
	   target specific internal function for this (not sure if we can have
	   those target specific, but I guess if it's folded away then that's
	   fine) and get away with the overload set."  */
	tree arg2_type = ptr_type_node;
	/* POINTER_PLUS_EXPR wants the offset to be of type 'sizetype'.  Create
	   the tree using the value from arg0.  The resulting type will match
	   the type of arg2.  */
	gimple_seq stmts = NULL;
	tree temp_offset = gimple_convert (&stmts, loc, sizetype, arg1);
	tree temp_addr = gimple_build (&stmts, loc, POINTER_PLUS_EXPR,
				       arg2_type, arg2, temp_offset);
	/* Mask off any lower bits from the address.  */
	tree aligned_addr = gimple_build (&stmts, loc, BIT_AND_EXPR,
					  arg2_type, temp_addr,
					  build_int_cst (arg2_type, -16));
	gsi_insert_seq_before (gsi, stmts, GSI_SAME_STMT);
	if (!is_gimple_mem_ref_addr (aligned_addr))
	  {
	    tree t = make_ssa_name (TREE_TYPE (aligned_addr));
	    gimple *g = gimple_build_assign (t, aligned_addr);
	    gsi_insert_before (gsi, g, GSI_SAME_STMT);
	    aligned_addr = t;
	  }
	/* The desired gimple result should be similar to:
	   MEM[(__vector floatD.1407 *)_1] = vf1D.2697;  */
	gimple *g
	  = gimple_build_assign (build2 (MEM_REF, arg0_type, aligned_addr,
					 build_int_cst (arg2_type, 0)), arg0);
	gimple_set_location (g, loc);
	gsi_replace (gsi, g, true);
	return true;
      }

    /* unaligned Vector loads.  */
    case VSX_BUILTIN_LXVW4X_V16QI:
    case VSX_BUILTIN_LXVW4X_V8HI:
    case VSX_BUILTIN_LXVW4X_V4SF:
    case VSX_BUILTIN_LXVW4X_V4SI:
    case VSX_BUILTIN_LXVD2X_V2DF:
    case VSX_BUILTIN_LXVD2X_V2DI:
      {
	arg0 = gimple_call_arg (stmt, 0);  // offset
	arg1 = gimple_call_arg (stmt, 1);  // address
	lhs = gimple_call_lhs (stmt);
	location_t loc = gimple_location (stmt);
	/* Since arg1 may be cast to a different type, just use ptr_type_node
	   here instead of trying to enforce TBAA on pointer types.  */
	tree arg1_type = ptr_type_node;
	tree lhs_type = TREE_TYPE (lhs);
	/* In GIMPLE the type of the MEM_REF specifies the alignment.  The
	  required alignment (power) is 4 bytes regardless of data type.  */
	tree align_ltype = build_aligned_type (lhs_type, 4);
	/* POINTER_PLUS_EXPR wants the offset to be of type 'sizetype'.  Create
	   the tree using the value from arg0.  The resulting type will match
	   the type of arg1.  */
	gimple_seq stmts = NULL;
	tree temp_offset = gimple_convert (&stmts, loc, sizetype, arg0);
	tree temp_addr = gimple_build (&stmts, loc, POINTER_PLUS_EXPR,
				       arg1_type, arg1, temp_offset);
	gsi_insert_seq_before (gsi, stmts, GSI_SAME_STMT);
	if (!is_gimple_mem_ref_addr (temp_addr))
	  {
	    tree t = make_ssa_name (TREE_TYPE (temp_addr));
	    gimple *g = gimple_build_assign (t, temp_addr);
	    gsi_insert_before (gsi, g, GSI_SAME_STMT);
	    temp_addr = t;
	  }
	/* Use the build2 helper to set up the mem_ref.  The MEM_REF could also
	   take an offset, but since we've already incorporated the offset
	   above, here we just pass in a zero.  */
	gimple *g;
	g = gimple_build_assign (lhs, build2 (MEM_REF, align_ltype, temp_addr,
					      build_int_cst (arg1_type, 0)));
	gimple_set_location (g, loc);
	gsi_replace (gsi, g, true);
	return true;
      }

    /* unaligned Vector stores.  */
    case VSX_BUILTIN_STXVW4X_V16QI:
    case VSX_BUILTIN_STXVW4X_V8HI:
    case VSX_BUILTIN_STXVW4X_V4SF:
    case VSX_BUILTIN_STXVW4X_V4SI:
    case VSX_BUILTIN_STXVD2X_V2DF:
    case VSX_BUILTIN_STXVD2X_V2DI:
      {
	arg0 = gimple_call_arg (stmt, 0); /* Value to be stored.  */
	arg1 = gimple_call_arg (stmt, 1); /* Offset.  */
	tree arg2 = gimple_call_arg (stmt, 2); /* Store-to address.  */
	location_t loc = gimple_location (stmt);
	tree arg0_type = TREE_TYPE (arg0);
	/* Use ptr_type_node (no TBAA) for the arg2_type.  */
	tree arg2_type = ptr_type_node;
	/* In GIMPLE the type of the MEM_REF specifies the alignment.  The
	   required alignment (power) is 4 bytes regardless of data type.  */
	tree align_stype = build_aligned_type (arg0_type, 4);
	/* POINTER_PLUS_EXPR wants the offset to be of type 'sizetype'.  Create
	   the tree using the value from arg1.  */
	gimple_seq stmts = NULL;
	tree temp_offset = gimple_convert (&stmts, loc, sizetype, arg1);
	tree temp_addr = gimple_build (&stmts, loc, POINTER_PLUS_EXPR,
				       arg2_type, arg2, temp_offset);
	gsi_insert_seq_before (gsi, stmts, GSI_SAME_STMT);
	if (!is_gimple_mem_ref_addr (temp_addr))
	  {
	    tree t = make_ssa_name (TREE_TYPE (temp_addr));
	    gimple *g = gimple_build_assign (t, temp_addr);
	    gsi_insert_before (gsi, g, GSI_SAME_STMT);
	    temp_addr = t;
	  }
	gimple *g;
	g = gimple_build_assign (build2 (MEM_REF, align_stype, temp_addr,
					 build_int_cst (arg2_type, 0)), arg0);
	gimple_set_location (g, loc);
	gsi_replace (gsi, g, true);
	return true;
      }

    /* Vector Fused multiply-add (fma).  */
    case ALTIVEC_BUILTIN_VMADDFP:
    case VSX_BUILTIN_XVMADDDP:
    case ALTIVEC_BUILTIN_VMLADDUHM:
      {
	arg0 = gimple_call_arg (stmt, 0);
	arg1 = gimple_call_arg (stmt, 1);
	tree arg2 = gimple_call_arg (stmt, 2);
	lhs = gimple_call_lhs (stmt);
	gcall *g = gimple_build_call_internal (IFN_FMA, 3, arg0, arg1, arg2);
	gimple_call_set_lhs (g, lhs);
	gimple_call_set_nothrow (g, true);
	gimple_set_location (g, gimple_location (stmt));
	gsi_replace (gsi, g, true);
	return true;
      }

    /* Vector compares; EQ, NE, GE, GT, LE.  */
    case ALTIVEC_BUILTIN_VCMPEQUB:
    case ALTIVEC_BUILTIN_VCMPEQUH:
    case ALTIVEC_BUILTIN_VCMPEQUW:
    case P8V_BUILTIN_VCMPEQUD:
      fold_compare_helper (gsi, EQ_EXPR, stmt);
      return true;

    case P9V_BUILTIN_CMPNEB:
    case P9V_BUILTIN_CMPNEH:
    case P9V_BUILTIN_CMPNEW:
      fold_compare_helper (gsi, NE_EXPR, stmt);
      return true;

    case VSX_BUILTIN_CMPGE_16QI:
    case VSX_BUILTIN_CMPGE_U16QI:
    case VSX_BUILTIN_CMPGE_8HI:
    case VSX_BUILTIN_CMPGE_U8HI:
    case VSX_BUILTIN_CMPGE_4SI:
    case VSX_BUILTIN_CMPGE_U4SI:
    case VSX_BUILTIN_CMPGE_2DI:
    case VSX_BUILTIN_CMPGE_U2DI:
      fold_compare_helper (gsi, GE_EXPR, stmt);
      return true;

    case ALTIVEC_BUILTIN_VCMPGTSB:
    case ALTIVEC_BUILTIN_VCMPGTUB:
    case ALTIVEC_BUILTIN_VCMPGTSH:
    case ALTIVEC_BUILTIN_VCMPGTUH:
    case ALTIVEC_BUILTIN_VCMPGTSW:
    case ALTIVEC_BUILTIN_VCMPGTUW:
    case P8V_BUILTIN_VCMPGTUD:
    case P8V_BUILTIN_VCMPGTSD:
      fold_compare_helper (gsi, GT_EXPR, stmt);
      return true;

    case VSX_BUILTIN_CMPLE_16QI:
    case VSX_BUILTIN_CMPLE_U16QI:
    case VSX_BUILTIN_CMPLE_8HI:
    case VSX_BUILTIN_CMPLE_U8HI:
    case VSX_BUILTIN_CMPLE_4SI:
    case VSX_BUILTIN_CMPLE_U4SI:
    case VSX_BUILTIN_CMPLE_2DI:
    case VSX_BUILTIN_CMPLE_U2DI:
      fold_compare_helper (gsi, LE_EXPR, stmt);
      return true;

    /* flavors of vec_splat_[us]{8,16,32}.  */
    case ALTIVEC_BUILTIN_VSPLTISB:
    case ALTIVEC_BUILTIN_VSPLTISH:
    case ALTIVEC_BUILTIN_VSPLTISW:
      {
	arg0 = gimple_call_arg (stmt, 0);
	lhs = gimple_call_lhs (stmt);

	/* Only fold the vec_splat_*() if the lower bits of arg 0 is a
	   5-bit signed constant in range -16 to +15.  */
	if (TREE_CODE (arg0) != INTEGER_CST
	    || !IN_RANGE (TREE_INT_CST_LOW (arg0), -16, 15))
	  return false;
	gimple_seq stmts = NULL;
	location_t loc = gimple_location (stmt);
	tree splat_value = gimple_convert (&stmts, loc,
					   TREE_TYPE (TREE_TYPE (lhs)), arg0);
	gsi_insert_seq_before (gsi, stmts, GSI_SAME_STMT);
	tree splat_tree = build_vector_from_val (TREE_TYPE (lhs), splat_value);
	g = gimple_build_assign (lhs, splat_tree);
	gimple_set_location (g, gimple_location (stmt));
	gsi_replace (gsi, g, true);
	return true;
      }

    /* Flavors of vec_splat.  */
    /* a = vec_splat (b, 0x3) becomes a = { b[3],b[3],b[3],...};  */
    case ALTIVEC_BUILTIN_VSPLTB:
    case ALTIVEC_BUILTIN_VSPLTH:
    case ALTIVEC_BUILTIN_VSPLTW:
    case VSX_BUILTIN_XXSPLTD_V2DI:
    case VSX_BUILTIN_XXSPLTD_V2DF:
      {
	arg0 = gimple_call_arg (stmt, 0); /* input vector.  */
	arg1 = gimple_call_arg (stmt, 1); /* index into arg0.  */
	/* Only fold the vec_splat_*() if arg1 is both a constant value and
	   is a valid index into the arg0 vector.  */
	unsigned int n_elts = VECTOR_CST_NELTS (arg0);
	if (TREE_CODE (arg1) != INTEGER_CST
	    || TREE_INT_CST_LOW (arg1) > (n_elts -1))
	  return false;
	lhs = gimple_call_lhs (stmt);
	tree lhs_type = TREE_TYPE (lhs);
	tree arg0_type = TREE_TYPE (arg0);
	tree splat;
	if (TREE_CODE (arg0) == VECTOR_CST)
	  splat = VECTOR_CST_ELT (arg0, TREE_INT_CST_LOW (arg1));
	else
	  {
	    /* Determine (in bits) the length and start location of the
	       splat value for a call to the tree_vec_extract helper.  */
	    int splat_elem_size = TREE_INT_CST_LOW (size_in_bytes (arg0_type))
				  * BITS_PER_UNIT / n_elts;
	    int splat_start_bit = TREE_INT_CST_LOW (arg1) * splat_elem_size;
	    tree len = build_int_cst (bitsizetype, splat_elem_size);
	    tree start = build_int_cst (bitsizetype, splat_start_bit);
	    splat = tree_vec_extract (gsi, TREE_TYPE (lhs_type), arg0,
				      len, start);
	  }
	/* And finally, build the new vector.  */
	tree splat_tree = build_vector_from_val (lhs_type, splat);
	g = gimple_build_assign (lhs, splat_tree);
	gimple_set_location (g, gimple_location (stmt));
	gsi_replace (gsi, g, true);
	return true;
      }

    /* vec_mergel (integrals).  */
    case ALTIVEC_BUILTIN_VMRGLH:
    case ALTIVEC_BUILTIN_VMRGLW:
    case VSX_BUILTIN_XXMRGLW_4SI:
    case ALTIVEC_BUILTIN_VMRGLB:
    case VSX_BUILTIN_VEC_MERGEL_V2DI:
    case VSX_BUILTIN_XXMRGLW_4SF:
    case VSX_BUILTIN_VEC_MERGEL_V2DF:
      fold_mergehl_helper (gsi, stmt, 1);
      return true;
    /* vec_mergeh (integrals).  */
    case ALTIVEC_BUILTIN_VMRGHH:
    case ALTIVEC_BUILTIN_VMRGHW:
    case VSX_BUILTIN_XXMRGHW_4SI:
    case ALTIVEC_BUILTIN_VMRGHB:
    case VSX_BUILTIN_VEC_MERGEH_V2DI:
    case VSX_BUILTIN_XXMRGHW_4SF:
    case VSX_BUILTIN_VEC_MERGEH_V2DF:
      fold_mergehl_helper (gsi, stmt, 0);
      return true;

    /* Flavors of vec_mergee.  */
    case P8V_BUILTIN_VMRGEW_V4SI:
    case P8V_BUILTIN_VMRGEW_V2DI:
    case P8V_BUILTIN_VMRGEW_V4SF:
    case P8V_BUILTIN_VMRGEW_V2DF:
      fold_mergeeo_helper (gsi, stmt, 0);
      return true;
    /* Flavors of vec_mergeo.  */
    case P8V_BUILTIN_VMRGOW_V4SI:
    case P8V_BUILTIN_VMRGOW_V2DI:
    case P8V_BUILTIN_VMRGOW_V4SF:
    case P8V_BUILTIN_VMRGOW_V2DF:
      fold_mergeeo_helper (gsi, stmt, 1);
      return true;

    /* d = vec_pack (a, b) */
    case P8V_BUILTIN_VPKUDUM:
    case ALTIVEC_BUILTIN_VPKUHUM:
    case ALTIVEC_BUILTIN_VPKUWUM:
      {
	arg0 = gimple_call_arg (stmt, 0);
	arg1 = gimple_call_arg (stmt, 1);
	lhs = gimple_call_lhs (stmt);
	gimple *g = gimple_build_assign (lhs, VEC_PACK_TRUNC_EXPR, arg0, arg1);
	gimple_set_location (g, gimple_location (stmt));
	gsi_replace (gsi, g, true);
	return true;
      }

    /* d = vec_unpackh (a) */
    /* Note that the UNPACK_{HI,LO}_EXPR used in the gimple_build_assign call
       in this code is sensitive to endian-ness, and needs to be inverted to
       handle both LE and BE targets.  */
    case ALTIVEC_BUILTIN_VUPKHSB:
    case ALTIVEC_BUILTIN_VUPKHSH:
    case P8V_BUILTIN_VUPKHSW:
      {
	arg0 = gimple_call_arg (stmt, 0);
	lhs = gimple_call_lhs (stmt);
	if (BYTES_BIG_ENDIAN)
	  g = gimple_build_assign (lhs, VEC_UNPACK_HI_EXPR, arg0);
	else
	  g = gimple_build_assign (lhs, VEC_UNPACK_LO_EXPR, arg0);
	gimple_set_location (g, gimple_location (stmt));
	gsi_replace (gsi, g, true);
	return true;
      }
    /* d = vec_unpackl (a) */
    case ALTIVEC_BUILTIN_VUPKLSB:
    case ALTIVEC_BUILTIN_VUPKLSH:
    case P8V_BUILTIN_VUPKLSW:
      {
	arg0 = gimple_call_arg (stmt, 0);
	lhs = gimple_call_lhs (stmt);
	if (BYTES_BIG_ENDIAN)
	  g = gimple_build_assign (lhs, VEC_UNPACK_LO_EXPR, arg0);
	else
	  g = gimple_build_assign (lhs, VEC_UNPACK_HI_EXPR, arg0);
	gimple_set_location (g, gimple_location (stmt));
	gsi_replace (gsi, g, true);
	return true;
      }
    /* There is no gimple type corresponding with pixel, so just return.  */
    case ALTIVEC_BUILTIN_VUPKHPX:
    case ALTIVEC_BUILTIN_VUPKLPX:
      return false;

    /* vec_perm.  */
    case ALTIVEC_BUILTIN_VPERM_16QI:
    case ALTIVEC_BUILTIN_VPERM_8HI:
    case ALTIVEC_BUILTIN_VPERM_4SI:
    case ALTIVEC_BUILTIN_VPERM_2DI:
    case ALTIVEC_BUILTIN_VPERM_4SF:
    case ALTIVEC_BUILTIN_VPERM_2DF:
      {
	arg0 = gimple_call_arg (stmt, 0);
	arg1 = gimple_call_arg (stmt, 1);
	tree permute = gimple_call_arg (stmt, 2);
	lhs = gimple_call_lhs (stmt);
	location_t loc = gimple_location (stmt);
	gimple_seq stmts = NULL;
	// convert arg0 and arg1 to match the type of the permute
	// for the VEC_PERM_EXPR operation.
	tree permute_type = (TREE_TYPE (permute));
	tree arg0_ptype = gimple_build (&stmts, loc, VIEW_CONVERT_EXPR,
					permute_type, arg0);
	tree arg1_ptype = gimple_build (&stmts, loc, VIEW_CONVERT_EXPR,
					permute_type, arg1);
	tree lhs_ptype = gimple_build (&stmts, loc, VEC_PERM_EXPR,
				      permute_type, arg0_ptype, arg1_ptype,
				      permute);
	// Convert the result back to the desired lhs type upon completion.
	tree temp = gimple_build (&stmts, loc, VIEW_CONVERT_EXPR,
				  TREE_TYPE (lhs), lhs_ptype);
	gsi_insert_seq_before (gsi, stmts, GSI_SAME_STMT);
	g = gimple_build_assign (lhs, temp);
	gimple_set_location (g, loc);
	gsi_replace (gsi, g, true);
	return true;
      }

    default:
      if (TARGET_DEBUG_BUILTIN)
	fprintf (stderr, "gimple builtin intrinsic not matched:%d %s %s\n",
		 fn_code, fn_name1, fn_name2);
      break;
    }

  return false;
}

/* Expand an expression EXP that calls a built-in function,
   with result going to TARGET if that's convenient
   (and in mode MODE if that's convenient).
   SUBTARGET may be used as the target for computing one of EXP's operands.
   IGNORE is nonzero if the value is to be ignored.  */

rtx
rs6000_expand_builtin (tree exp, rtx target, rtx subtarget ATTRIBUTE_UNUSED,
		       machine_mode mode ATTRIBUTE_UNUSED,
		       int ignore ATTRIBUTE_UNUSED)
{
  tree fndecl = TREE_OPERAND (CALL_EXPR_FN (exp), 0);
  enum rs6000_builtins fcode
    = (enum rs6000_builtins) DECL_MD_FUNCTION_CODE (fndecl);
  size_t uns_fcode = (size_t)fcode;
  const struct builtin_description *d;
  size_t i;
  rtx ret;
  bool success;
  HOST_WIDE_INT mask = rs6000_builtin_info[uns_fcode].mask;
  bool func_valid_p = ((rs6000_builtin_mask & mask) == mask);
  enum insn_code icode = rs6000_builtin_info[uns_fcode].icode;

  /* We have two different modes (KFmode, TFmode) that are the IEEE 128-bit
     floating point type, depending on whether long double is the IBM extended
     double (KFmode) or long double is IEEE 128-bit (TFmode).  It is simpler if
     we only define one variant of the built-in function, and switch the code
     when defining it, rather than defining two built-ins and using the
     overload table in rs6000-c.c to switch between the two.  If we don't have
     the proper assembler, don't do this switch because CODE_FOR_*kf* and
     CODE_FOR_*tf* will be CODE_FOR_nothing.  */
  if (FLOAT128_IEEE_P (TFmode))
    switch (icode)
      {
      default:
	break;

      case CODE_FOR_sqrtkf2_odd:	icode = CODE_FOR_sqrttf2_odd;	break;
      case CODE_FOR_trunckfdf2_odd:	icode = CODE_FOR_trunctfdf2_odd; break;
      case CODE_FOR_addkf3_odd:		icode = CODE_FOR_addtf3_odd;	break;
      case CODE_FOR_subkf3_odd:		icode = CODE_FOR_subtf3_odd;	break;
      case CODE_FOR_mulkf3_odd:		icode = CODE_FOR_multf3_odd;	break;
      case CODE_FOR_divkf3_odd:		icode = CODE_FOR_divtf3_odd;	break;
      case CODE_FOR_fmakf4_odd:		icode = CODE_FOR_fmatf4_odd;	break;
      case CODE_FOR_xsxexpqp_kf:	icode = CODE_FOR_xsxexpqp_tf;	break;
      case CODE_FOR_xsxsigqp_kf:	icode = CODE_FOR_xsxsigqp_tf;	break;
      case CODE_FOR_xststdcnegqp_kf:	icode = CODE_FOR_xststdcnegqp_tf; break;
      case CODE_FOR_xsiexpqp_kf:	icode = CODE_FOR_xsiexpqp_tf;	break;
      case CODE_FOR_xsiexpqpf_kf:	icode = CODE_FOR_xsiexpqpf_tf;	break;
      case CODE_FOR_xststdcqp_kf:	icode = CODE_FOR_xststdcqp_tf;	break;
      }

  if (TARGET_DEBUG_BUILTIN)
    {
      const char *name1 = rs6000_builtin_info[uns_fcode].name;
      const char *name2 = (icode != CODE_FOR_nothing)
			   ? get_insn_name ((int) icode)
			   : "nothing";
      const char *name3;

      switch (rs6000_builtin_info[uns_fcode].attr & RS6000_BTC_TYPE_MASK)
	{
	default:		   name3 = "unknown";	break;
	case RS6000_BTC_SPECIAL:   name3 = "special";	break;
	case RS6000_BTC_UNARY:	   name3 = "unary";	break;
	case RS6000_BTC_BINARY:	   name3 = "binary";	break;
	case RS6000_BTC_TERNARY:   name3 = "ternary";	break;
	case RS6000_BTC_PREDICATE: name3 = "predicate";	break;
	case RS6000_BTC_ABS:	   name3 = "abs";	break;
	case RS6000_BTC_DST:	   name3 = "dst";	break;
	}


      fprintf (stderr,
	       "rs6000_expand_builtin, %s (%d), insn = %s (%d), type=%s%s\n",
	       (name1) ? name1 : "---", fcode,
	       (name2) ? name2 : "---", (int) icode,
	       name3,
	       func_valid_p ? "" : ", not valid");
    }	     

  if (!func_valid_p)
    {
      rs6000_invalid_builtin (fcode);

      /* Given it is invalid, just generate a normal call.  */
      return expand_call (exp, target, ignore);
    }

  switch (fcode)
    {
    case RS6000_BUILTIN_RECIP:
      return rs6000_expand_binop_builtin (CODE_FOR_recipdf3, exp, target);

    case RS6000_BUILTIN_RECIPF:
      return rs6000_expand_binop_builtin (CODE_FOR_recipsf3, exp, target);

    case RS6000_BUILTIN_RSQRTF:
      return rs6000_expand_unop_builtin (CODE_FOR_rsqrtsf2, exp, target);

    case RS6000_BUILTIN_RSQRT:
      return rs6000_expand_unop_builtin (CODE_FOR_rsqrtdf2, exp, target);

    case POWER7_BUILTIN_BPERMD:
      return rs6000_expand_binop_builtin (((TARGET_64BIT)
					   ? CODE_FOR_bpermd_di
					   : CODE_FOR_bpermd_si), exp, target);

    case RS6000_BUILTIN_GET_TB:
      return rs6000_expand_zeroop_builtin (CODE_FOR_rs6000_get_timebase,
					   target);

    case RS6000_BUILTIN_MFTB:
      return rs6000_expand_zeroop_builtin (((TARGET_64BIT)
					    ? CODE_FOR_rs6000_mftb_di
					    : CODE_FOR_rs6000_mftb_si),
					   target);

    case RS6000_BUILTIN_MFFS:
      return rs6000_expand_zeroop_builtin (CODE_FOR_rs6000_mffs, target);

    case RS6000_BUILTIN_MTFSB0:
      return rs6000_expand_mtfsb_builtin (CODE_FOR_rs6000_mtfsb0, exp);

    case RS6000_BUILTIN_MTFSB1:
      return rs6000_expand_mtfsb_builtin (CODE_FOR_rs6000_mtfsb1, exp);

    case RS6000_BUILTIN_SET_FPSCR_RN:
      return rs6000_expand_set_fpscr_rn_builtin (CODE_FOR_rs6000_set_fpscr_rn,
						 exp);

    case RS6000_BUILTIN_SET_FPSCR_DRN:
      return
        rs6000_expand_set_fpscr_drn_builtin (CODE_FOR_rs6000_set_fpscr_drn,
					     exp);

    case RS6000_BUILTIN_MFFSL:
      return rs6000_expand_zeroop_builtin (CODE_FOR_rs6000_mffsl, target);

    case RS6000_BUILTIN_MTFSF:
      return rs6000_expand_mtfsf_builtin (CODE_FOR_rs6000_mtfsf, exp);

    case RS6000_BUILTIN_CPU_INIT:
    case RS6000_BUILTIN_CPU_IS:
    case RS6000_BUILTIN_CPU_SUPPORTS:
      return cpu_expand_builtin (fcode, exp, target);

    case MISC_BUILTIN_SPEC_BARRIER:
      {
	emit_insn (gen_speculation_barrier ());
	return NULL_RTX;
      }

    case ALTIVEC_BUILTIN_MASK_FOR_LOAD:
    case ALTIVEC_BUILTIN_MASK_FOR_STORE:
      {
	int icode2 = (BYTES_BIG_ENDIAN ? (int) CODE_FOR_altivec_lvsr_direct
		     : (int) CODE_FOR_altivec_lvsl_direct);
	machine_mode tmode = insn_data[icode2].operand[0].mode;
	machine_mode mode = insn_data[icode2].operand[1].mode;
	tree arg;
	rtx op, addr, pat;

	gcc_assert (TARGET_ALTIVEC);

	arg = CALL_EXPR_ARG (exp, 0);
	gcc_assert (POINTER_TYPE_P (TREE_TYPE (arg)));
	op = expand_expr (arg, NULL_RTX, Pmode, EXPAND_NORMAL);
	addr = memory_address (mode, op);
	if (fcode == ALTIVEC_BUILTIN_MASK_FOR_STORE)
	  op = addr;
	else
	  {
	    /* For the load case need to negate the address.  */
	    op = gen_reg_rtx (GET_MODE (addr));
	    emit_insn (gen_rtx_SET (op, gen_rtx_NEG (GET_MODE (addr), addr)));
	  }
	op = gen_rtx_MEM (mode, op);

	if (target == 0
	    || GET_MODE (target) != tmode
	    || ! (*insn_data[icode2].operand[0].predicate) (target, tmode))
	  target = gen_reg_rtx (tmode);

	pat = GEN_FCN (icode2) (target, op);
	if (!pat)
	  return 0;
	emit_insn (pat);

	return target;
      }

    case ALTIVEC_BUILTIN_VCFUX:
    case ALTIVEC_BUILTIN_VCFSX:
    case ALTIVEC_BUILTIN_VCTUXS:
    case ALTIVEC_BUILTIN_VCTSXS:
  /* FIXME: There's got to be a nicer way to handle this case than
     constructing a new CALL_EXPR.  */
      if (call_expr_nargs (exp) == 1)
	{
	  exp = build_call_nary (TREE_TYPE (exp), CALL_EXPR_FN (exp),
				 2, CALL_EXPR_ARG (exp, 0), integer_zero_node);
	}
      break;

      /* For the pack and unpack int128 routines, fix up the builtin so it
	 uses the correct IBM128 type.  */
    case MISC_BUILTIN_PACK_IF:
      if (TARGET_LONG_DOUBLE_128 && !TARGET_IEEEQUAD)
	{
	  icode = CODE_FOR_packtf;
	  fcode = MISC_BUILTIN_PACK_TF;
	  uns_fcode = (size_t)fcode;
	}
      break;

    case MISC_BUILTIN_UNPACK_IF:
      if (TARGET_LONG_DOUBLE_128 && !TARGET_IEEEQUAD)
	{
	  icode = CODE_FOR_unpacktf;
	  fcode = MISC_BUILTIN_UNPACK_TF;
	  uns_fcode = (size_t)fcode;
	}
      break;

    default:
      break;
    }

  if (TARGET_ALTIVEC)
    {
      ret = altivec_expand_builtin (exp, target, &success);

      if (success)
	return ret;
    }
  if (TARGET_HTM)
    {
      ret = htm_expand_builtin (exp, target, &success);

      if (success)
	return ret;
    }  

  unsigned attr = rs6000_builtin_info[uns_fcode].attr & RS6000_BTC_TYPE_MASK;
  /* RS6000_BTC_SPECIAL represents no-operand operators.  */
  gcc_assert (attr == RS6000_BTC_UNARY
	      || attr == RS6000_BTC_BINARY
	      || attr == RS6000_BTC_TERNARY
	      || attr == RS6000_BTC_SPECIAL);
  
  /* Handle simple unary operations.  */
  d = bdesc_1arg;
  for (i = 0; i < ARRAY_SIZE (bdesc_1arg); i++, d++)
    if (d->code == fcode)
      return rs6000_expand_unop_builtin (icode, exp, target);

  /* Handle simple binary operations.  */
  d = bdesc_2arg;
  for (i = 0; i < ARRAY_SIZE (bdesc_2arg); i++, d++)
    if (d->code == fcode)
      return rs6000_expand_binop_builtin (icode, exp, target);

  /* Handle simple ternary operations.  */
  d = bdesc_3arg;
  for (i = 0; i < ARRAY_SIZE  (bdesc_3arg); i++, d++)
    if (d->code == fcode)
      return rs6000_expand_ternop_builtin (icode, exp, target);

  /* Handle simple no-argument operations. */
  d = bdesc_0arg;
  for (i = 0; i < ARRAY_SIZE (bdesc_0arg); i++, d++)
    if (d->code == fcode)
      return rs6000_expand_zeroop_builtin (icode, target);

  gcc_unreachable ();
}

/* Create a builtin vector type with a name.  Taking care not to give
   the canonical type a name.  */

static tree
rs6000_vector_type (const char *name, tree elt_type, unsigned num_elts)
{
  tree result = build_vector_type (elt_type, num_elts);

  /* Copy so we don't give the canonical type a name.  */
  result = build_variant_type_copy (result);

  add_builtin_type (name, result);

  return result;
}

void
rs6000_init_builtins (void)
{
  tree tdecl;
  tree ftype;
  machine_mode mode;

  if (TARGET_DEBUG_BUILTIN)
    fprintf (stderr, "rs6000_init_builtins%s%s\n",
	     (TARGET_ALTIVEC)	   ? ", altivec" : "",
	     (TARGET_VSX)	   ? ", vsx"	 : "");

  V2DI_type_node = rs6000_vector_type (TARGET_POWERPC64 ? "__vector long"
				       : "__vector long long",
				       intDI_type_node, 2);
  V2DF_type_node = rs6000_vector_type ("__vector double", double_type_node, 2);
  V4SI_type_node = rs6000_vector_type ("__vector signed int",
				       intSI_type_node, 4);
  V4SF_type_node = rs6000_vector_type ("__vector float", float_type_node, 4);
  V8HI_type_node = rs6000_vector_type ("__vector signed short",
				       intHI_type_node, 8);
  V16QI_type_node = rs6000_vector_type ("__vector signed char",
					intQI_type_node, 16);

  unsigned_V16QI_type_node = rs6000_vector_type ("__vector unsigned char",
					unsigned_intQI_type_node, 16);
  unsigned_V8HI_type_node = rs6000_vector_type ("__vector unsigned short",
				       unsigned_intHI_type_node, 8);
  unsigned_V4SI_type_node = rs6000_vector_type ("__vector unsigned int",
				       unsigned_intSI_type_node, 4);
  unsigned_V2DI_type_node = rs6000_vector_type (TARGET_POWERPC64
				       ? "__vector unsigned long"
				       : "__vector unsigned long long",
				       unsigned_intDI_type_node, 2);

  opaque_V4SI_type_node = build_opaque_vector_type (intSI_type_node, 4);

  const_str_type_node
    = build_pointer_type (build_qualified_type (char_type_node,
						TYPE_QUAL_CONST));

  /* We use V1TI mode as a special container to hold __int128_t items that
     must live in VSX registers.  */
  if (intTI_type_node)
    {
      V1TI_type_node = rs6000_vector_type ("__vector __int128",
					   intTI_type_node, 1);
      unsigned_V1TI_type_node
	= rs6000_vector_type ("__vector unsigned __int128",
			      unsigned_intTI_type_node, 1);
    }

  /* The 'vector bool ...' types must be kept distinct from 'vector unsigned ...'
     types, especially in C++ land.  Similarly, 'vector pixel' is distinct from
     'vector unsigned short'.  */

  bool_char_type_node = build_distinct_type_copy (unsigned_intQI_type_node);
  bool_short_type_node = build_distinct_type_copy (unsigned_intHI_type_node);
  bool_int_type_node = build_distinct_type_copy (unsigned_intSI_type_node);
  bool_long_long_type_node = build_distinct_type_copy (unsigned_intDI_type_node);
  pixel_type_node = build_distinct_type_copy (unsigned_intHI_type_node);

  long_integer_type_internal_node = long_integer_type_node;
  long_unsigned_type_internal_node = long_unsigned_type_node;
  long_long_integer_type_internal_node = long_long_integer_type_node;
  long_long_unsigned_type_internal_node = long_long_unsigned_type_node;
  intQI_type_internal_node = intQI_type_node;
  uintQI_type_internal_node = unsigned_intQI_type_node;
  intHI_type_internal_node = intHI_type_node;
  uintHI_type_internal_node = unsigned_intHI_type_node;
  intSI_type_internal_node = intSI_type_node;
  uintSI_type_internal_node = unsigned_intSI_type_node;
  intDI_type_internal_node = intDI_type_node;
  uintDI_type_internal_node = unsigned_intDI_type_node;
  intTI_type_internal_node = intTI_type_node;
  uintTI_type_internal_node = unsigned_intTI_type_node;
  float_type_internal_node = float_type_node;
  double_type_internal_node = double_type_node;
  long_double_type_internal_node = long_double_type_node;
  dfloat64_type_internal_node = dfloat64_type_node;
  dfloat128_type_internal_node = dfloat128_type_node;
  void_type_internal_node = void_type_node;

  /* 128-bit floating point support.  KFmode is IEEE 128-bit floating point.
     IFmode is the IBM extended 128-bit format that is a pair of doubles.
     TFmode will be either IEEE 128-bit floating point or the IBM double-double
     format that uses a pair of doubles, depending on the switches and
     defaults.

     If we don't support for either 128-bit IBM double double or IEEE 128-bit
     floating point, we need make sure the type is non-zero or else self-test
     fails during bootstrap.

     Always create __ibm128 as a separate type, even if the current long double
     format is IBM extended double.

     For IEEE 128-bit floating point, always create the type __ieee128.  If the
     user used -mfloat128, rs6000-c.c will create a define from __float128 to
     __ieee128.  */
  if (TARGET_FLOAT128_TYPE)
    {
      if (!TARGET_IEEEQUAD && TARGET_LONG_DOUBLE_128)
	ibm128_float_type_node = long_double_type_node;
      else
	{
	  ibm128_float_type_node = make_node (REAL_TYPE);
	  TYPE_PRECISION (ibm128_float_type_node) = 128;
	  SET_TYPE_MODE (ibm128_float_type_node, IFmode);
	  layout_type (ibm128_float_type_node);
	}

      lang_hooks.types.register_builtin_type (ibm128_float_type_node,
					      "__ibm128");

      if (TARGET_IEEEQUAD && TARGET_LONG_DOUBLE_128)
	ieee128_float_type_node = long_double_type_node;
      else
	ieee128_float_type_node = float128_type_node;

      lang_hooks.types.register_builtin_type (ieee128_float_type_node,
					      "__ieee128");
    }

  else
    ieee128_float_type_node = ibm128_float_type_node = long_double_type_node;

  /* Initialize the modes for builtin_function_type, mapping a machine mode to
     tree type node.  */
  builtin_mode_to_type[QImode][0] = integer_type_node;
  builtin_mode_to_type[HImode][0] = integer_type_node;
  builtin_mode_to_type[SImode][0] = intSI_type_node;
  builtin_mode_to_type[SImode][1] = unsigned_intSI_type_node;
  builtin_mode_to_type[DImode][0] = intDI_type_node;
  builtin_mode_to_type[DImode][1] = unsigned_intDI_type_node;
  builtin_mode_to_type[TImode][0] = intTI_type_node;
  builtin_mode_to_type[TImode][1] = unsigned_intTI_type_node;
  builtin_mode_to_type[SFmode][0] = float_type_node;
  builtin_mode_to_type[DFmode][0] = double_type_node;
  builtin_mode_to_type[IFmode][0] = ibm128_float_type_node;
  builtin_mode_to_type[KFmode][0] = ieee128_float_type_node;
  builtin_mode_to_type[TFmode][0] = long_double_type_node;
  builtin_mode_to_type[DDmode][0] = dfloat64_type_node;
  builtin_mode_to_type[TDmode][0] = dfloat128_type_node;
  builtin_mode_to_type[V1TImode][0] = V1TI_type_node;
  builtin_mode_to_type[V1TImode][1] = unsigned_V1TI_type_node;
  builtin_mode_to_type[V2DImode][0] = V2DI_type_node;
  builtin_mode_to_type[V2DImode][1] = unsigned_V2DI_type_node;
  builtin_mode_to_type[V2DFmode][0] = V2DF_type_node;
  builtin_mode_to_type[V4SImode][0] = V4SI_type_node;
  builtin_mode_to_type[V4SImode][1] = unsigned_V4SI_type_node;
  builtin_mode_to_type[V4SFmode][0] = V4SF_type_node;
  builtin_mode_to_type[V8HImode][0] = V8HI_type_node;
  builtin_mode_to_type[V8HImode][1] = unsigned_V8HI_type_node;
  builtin_mode_to_type[V16QImode][0] = V16QI_type_node;
  builtin_mode_to_type[V16QImode][1] = unsigned_V16QI_type_node;

  tdecl = add_builtin_type ("__bool char", bool_char_type_node);
  TYPE_NAME (bool_char_type_node) = tdecl;

  tdecl = add_builtin_type ("__bool short", bool_short_type_node);
  TYPE_NAME (bool_short_type_node) = tdecl;

  tdecl = add_builtin_type ("__bool int", bool_int_type_node);
  TYPE_NAME (bool_int_type_node) = tdecl;

  tdecl = add_builtin_type ("__pixel", pixel_type_node);
  TYPE_NAME (pixel_type_node) = tdecl;

  bool_V16QI_type_node = rs6000_vector_type ("__vector __bool char",
					     bool_char_type_node, 16);
  bool_V8HI_type_node = rs6000_vector_type ("__vector __bool short",
					    bool_short_type_node, 8);
  bool_V4SI_type_node = rs6000_vector_type ("__vector __bool int",
					    bool_int_type_node, 4);
  bool_V2DI_type_node = rs6000_vector_type (TARGET_POWERPC64
					    ? "__vector __bool long"
					    : "__vector __bool long long",
					    bool_long_long_type_node, 2);
  pixel_V8HI_type_node = rs6000_vector_type ("__vector __pixel",
					     pixel_type_node, 8);

  /* Create Altivec and VSX builtins on machines with at least the
     general purpose extensions (970 and newer) to allow the use of
     the target attribute.  */
  if (TARGET_EXTRA_BUILTINS)
    altivec_init_builtins ();
  if (TARGET_HTM)
    htm_init_builtins ();

  if (TARGET_EXTRA_BUILTINS)
    rs6000_common_init_builtins ();

  ftype = builtin_function_type (DFmode, DFmode, DFmode, VOIDmode,
				 RS6000_BUILTIN_RECIP, "__builtin_recipdiv");
  def_builtin ("__builtin_recipdiv", ftype, RS6000_BUILTIN_RECIP);

  ftype = builtin_function_type (SFmode, SFmode, SFmode, VOIDmode,
				 RS6000_BUILTIN_RECIPF, "__builtin_recipdivf");
  def_builtin ("__builtin_recipdivf", ftype, RS6000_BUILTIN_RECIPF);

  ftype = builtin_function_type (DFmode, DFmode, VOIDmode, VOIDmode,
				 RS6000_BUILTIN_RSQRT, "__builtin_rsqrt");
  def_builtin ("__builtin_rsqrt", ftype, RS6000_BUILTIN_RSQRT);

  ftype = builtin_function_type (SFmode, SFmode, VOIDmode, VOIDmode,
				 RS6000_BUILTIN_RSQRTF, "__builtin_rsqrtf");
  def_builtin ("__builtin_rsqrtf", ftype, RS6000_BUILTIN_RSQRTF);

  mode = (TARGET_64BIT) ? DImode : SImode;
  ftype = builtin_function_type (mode, mode, mode, VOIDmode,
				 POWER7_BUILTIN_BPERMD, "__builtin_bpermd");
  def_builtin ("__builtin_bpermd", ftype, POWER7_BUILTIN_BPERMD);

  ftype = build_function_type_list (unsigned_intDI_type_node,
				    NULL_TREE);
  def_builtin ("__builtin_ppc_get_timebase", ftype, RS6000_BUILTIN_GET_TB);

  if (TARGET_64BIT)
    ftype = build_function_type_list (unsigned_intDI_type_node,
				      NULL_TREE);
  else
    ftype = build_function_type_list (unsigned_intSI_type_node,
				      NULL_TREE);
  def_builtin ("__builtin_ppc_mftb", ftype, RS6000_BUILTIN_MFTB);

  ftype = build_function_type_list (double_type_node, NULL_TREE);
  def_builtin ("__builtin_mffs", ftype, RS6000_BUILTIN_MFFS);

  ftype = build_function_type_list (double_type_node, NULL_TREE);
  def_builtin ("__builtin_mffsl", ftype, RS6000_BUILTIN_MFFSL);

  ftype = build_function_type_list (void_type_node,
				    intSI_type_node,
				    NULL_TREE);
  def_builtin ("__builtin_mtfsb0", ftype, RS6000_BUILTIN_MTFSB0);

  ftype = build_function_type_list (void_type_node,
				    intSI_type_node,
				    NULL_TREE);
  def_builtin ("__builtin_mtfsb1", ftype, RS6000_BUILTIN_MTFSB1);

  ftype = build_function_type_list (void_type_node,
				    intDI_type_node,
				    NULL_TREE);
  def_builtin ("__builtin_set_fpscr_rn", ftype, RS6000_BUILTIN_SET_FPSCR_RN);

  ftype = build_function_type_list (void_type_node,
				    intDI_type_node,
				    NULL_TREE);
  def_builtin ("__builtin_set_fpscr_drn", ftype, RS6000_BUILTIN_SET_FPSCR_DRN);

  ftype = build_function_type_list (void_type_node,
				    intSI_type_node, double_type_node,
				    NULL_TREE);
  def_builtin ("__builtin_mtfsf", ftype, RS6000_BUILTIN_MTFSF);

  ftype = build_function_type_list (void_type_node, NULL_TREE);
  def_builtin ("__builtin_cpu_init", ftype, RS6000_BUILTIN_CPU_INIT);
  def_builtin ("__builtin_ppc_speculation_barrier", ftype,
	       MISC_BUILTIN_SPEC_BARRIER);

  ftype = build_function_type_list (bool_int_type_node, const_ptr_type_node,
				    NULL_TREE);
  def_builtin ("__builtin_cpu_is", ftype, RS6000_BUILTIN_CPU_IS);
  def_builtin ("__builtin_cpu_supports", ftype, RS6000_BUILTIN_CPU_SUPPORTS);

  /* AIX libm provides clog as __clog.  */
  if (TARGET_XCOFF &&
      (tdecl = builtin_decl_explicit (BUILT_IN_CLOG)) != NULL_TREE)
    set_user_assembler_name (tdecl, "__clog");

#ifdef SUBTARGET_INIT_BUILTINS
  SUBTARGET_INIT_BUILTINS;
#endif
}

/* Returns the rs6000 builtin decl for CODE.  */

tree
rs6000_builtin_decl (unsigned code, bool initialize_p ATTRIBUTE_UNUSED)
{
  HOST_WIDE_INT fnmask;

  if (code >= RS6000_BUILTIN_COUNT)
    return error_mark_node;

  fnmask = rs6000_builtin_info[code].mask;
  if ((fnmask & rs6000_builtin_mask) != fnmask)
    {
      rs6000_invalid_builtin ((enum rs6000_builtins)code);
      return error_mark_node;
    }

  return rs6000_builtin_decls[code];
}

static void
altivec_init_builtins (void)
{
  const struct builtin_description *d;
  size_t i;
  tree ftype;
  tree decl;
  HOST_WIDE_INT builtin_mask = rs6000_builtin_mask;

  tree pvoid_type_node = build_pointer_type (void_type_node);

  tree pcvoid_type_node
    = build_pointer_type (build_qualified_type (void_type_node,
						TYPE_QUAL_CONST));

  tree int_ftype_opaque
    = build_function_type_list (integer_type_node,
				opaque_V4SI_type_node, NULL_TREE);
  tree opaque_ftype_opaque
    = build_function_type_list (integer_type_node, NULL_TREE);
  tree opaque_ftype_opaque_int
    = build_function_type_list (opaque_V4SI_type_node,
				opaque_V4SI_type_node, integer_type_node, NULL_TREE);
  tree opaque_ftype_opaque_opaque_int
    = build_function_type_list (opaque_V4SI_type_node,
				opaque_V4SI_type_node, opaque_V4SI_type_node,
				integer_type_node, NULL_TREE);
  tree opaque_ftype_opaque_opaque_opaque
    = build_function_type_list (opaque_V4SI_type_node,
				opaque_V4SI_type_node, opaque_V4SI_type_node,
				opaque_V4SI_type_node, NULL_TREE);
  tree opaque_ftype_opaque_opaque
    = build_function_type_list (opaque_V4SI_type_node,
				opaque_V4SI_type_node, opaque_V4SI_type_node,
				NULL_TREE);
  tree int_ftype_int_opaque_opaque
    = build_function_type_list (integer_type_node,
                                integer_type_node, opaque_V4SI_type_node,
                                opaque_V4SI_type_node, NULL_TREE);
  tree int_ftype_int_v4si_v4si
    = build_function_type_list (integer_type_node,
				integer_type_node, V4SI_type_node,
				V4SI_type_node, NULL_TREE);
  tree int_ftype_int_v2di_v2di
    = build_function_type_list (integer_type_node,
				integer_type_node, V2DI_type_node,
				V2DI_type_node, NULL_TREE);
  tree void_ftype_v4si
    = build_function_type_list (void_type_node, V4SI_type_node, NULL_TREE);
  tree v8hi_ftype_void
    = build_function_type_list (V8HI_type_node, NULL_TREE);
  tree void_ftype_void
    = build_function_type_list (void_type_node, NULL_TREE);
  tree void_ftype_int
    = build_function_type_list (void_type_node, integer_type_node, NULL_TREE);

  tree opaque_ftype_long_pcvoid
    = build_function_type_list (opaque_V4SI_type_node,
				long_integer_type_node, pcvoid_type_node,
				NULL_TREE);
  tree v16qi_ftype_long_pcvoid
    = build_function_type_list (V16QI_type_node,
				long_integer_type_node, pcvoid_type_node,
				NULL_TREE);
  tree v8hi_ftype_long_pcvoid
    = build_function_type_list (V8HI_type_node,
				long_integer_type_node, pcvoid_type_node,
				NULL_TREE);
  tree v4si_ftype_long_pcvoid
    = build_function_type_list (V4SI_type_node,
				long_integer_type_node, pcvoid_type_node,
				NULL_TREE);
  tree v4sf_ftype_long_pcvoid
    = build_function_type_list (V4SF_type_node,
				long_integer_type_node, pcvoid_type_node,
				NULL_TREE);
  tree v2df_ftype_long_pcvoid
    = build_function_type_list (V2DF_type_node,
				long_integer_type_node, pcvoid_type_node,
				NULL_TREE);
  tree v2di_ftype_long_pcvoid
    = build_function_type_list (V2DI_type_node,
				long_integer_type_node, pcvoid_type_node,
				NULL_TREE);
  tree v1ti_ftype_long_pcvoid
    = build_function_type_list (V1TI_type_node,
				long_integer_type_node, pcvoid_type_node,
				NULL_TREE);

  tree void_ftype_opaque_long_pvoid
    = build_function_type_list (void_type_node,
				opaque_V4SI_type_node, long_integer_type_node,
				pvoid_type_node, NULL_TREE);
  tree void_ftype_v4si_long_pvoid
    = build_function_type_list (void_type_node,
				V4SI_type_node, long_integer_type_node,
				pvoid_type_node, NULL_TREE);
  tree void_ftype_v16qi_long_pvoid
    = build_function_type_list (void_type_node,
				V16QI_type_node, long_integer_type_node,
				pvoid_type_node, NULL_TREE);

  tree void_ftype_v16qi_pvoid_long
    = build_function_type_list (void_type_node,
				V16QI_type_node, pvoid_type_node,
				long_integer_type_node, NULL_TREE);

  tree void_ftype_v8hi_long_pvoid
    = build_function_type_list (void_type_node,
				V8HI_type_node, long_integer_type_node,
				pvoid_type_node, NULL_TREE);
  tree void_ftype_v4sf_long_pvoid
    = build_function_type_list (void_type_node,
				V4SF_type_node, long_integer_type_node,
				pvoid_type_node, NULL_TREE);
  tree void_ftype_v2df_long_pvoid
    = build_function_type_list (void_type_node,
				V2DF_type_node, long_integer_type_node,
				pvoid_type_node, NULL_TREE);
  tree void_ftype_v1ti_long_pvoid
    = build_function_type_list (void_type_node,
				V1TI_type_node, long_integer_type_node,
				pvoid_type_node, NULL_TREE);
  tree void_ftype_v2di_long_pvoid
    = build_function_type_list (void_type_node,
				V2DI_type_node, long_integer_type_node,
				pvoid_type_node, NULL_TREE);
  tree int_ftype_int_v8hi_v8hi
    = build_function_type_list (integer_type_node,
				integer_type_node, V8HI_type_node,
				V8HI_type_node, NULL_TREE);
  tree int_ftype_int_v16qi_v16qi
    = build_function_type_list (integer_type_node,
				integer_type_node, V16QI_type_node,
				V16QI_type_node, NULL_TREE);
  tree int_ftype_int_v4sf_v4sf
    = build_function_type_list (integer_type_node,
				integer_type_node, V4SF_type_node,
				V4SF_type_node, NULL_TREE);
  tree int_ftype_int_v2df_v2df
    = build_function_type_list (integer_type_node,
				integer_type_node, V2DF_type_node,
				V2DF_type_node, NULL_TREE);
  tree v2di_ftype_v2di
    = build_function_type_list (V2DI_type_node, V2DI_type_node, NULL_TREE);
  tree v4si_ftype_v4si
    = build_function_type_list (V4SI_type_node, V4SI_type_node, NULL_TREE);
  tree v8hi_ftype_v8hi
    = build_function_type_list (V8HI_type_node, V8HI_type_node, NULL_TREE);
  tree v16qi_ftype_v16qi
    = build_function_type_list (V16QI_type_node, V16QI_type_node, NULL_TREE);
  tree v4sf_ftype_v4sf
    = build_function_type_list (V4SF_type_node, V4SF_type_node, NULL_TREE);
  tree v2df_ftype_v2df
    = build_function_type_list (V2DF_type_node, V2DF_type_node, NULL_TREE);
  tree void_ftype_pcvoid_int_int
    = build_function_type_list (void_type_node,
				pcvoid_type_node, integer_type_node,
				integer_type_node, NULL_TREE);

  def_builtin ("__builtin_altivec_mtvscr", void_ftype_v4si, ALTIVEC_BUILTIN_MTVSCR);
  def_builtin ("__builtin_altivec_mfvscr", v8hi_ftype_void, ALTIVEC_BUILTIN_MFVSCR);
  def_builtin ("__builtin_altivec_dssall", void_ftype_void, ALTIVEC_BUILTIN_DSSALL);
  def_builtin ("__builtin_altivec_dss", void_ftype_int, ALTIVEC_BUILTIN_DSS);
  def_builtin ("__builtin_altivec_lvsl", v16qi_ftype_long_pcvoid, ALTIVEC_BUILTIN_LVSL);
  def_builtin ("__builtin_altivec_lvsr", v16qi_ftype_long_pcvoid, ALTIVEC_BUILTIN_LVSR);
  def_builtin ("__builtin_altivec_lvebx", v16qi_ftype_long_pcvoid, ALTIVEC_BUILTIN_LVEBX);
  def_builtin ("__builtin_altivec_lvehx", v8hi_ftype_long_pcvoid, ALTIVEC_BUILTIN_LVEHX);
  def_builtin ("__builtin_altivec_lvewx", v4si_ftype_long_pcvoid, ALTIVEC_BUILTIN_LVEWX);
  def_builtin ("__builtin_altivec_lvxl", v4si_ftype_long_pcvoid, ALTIVEC_BUILTIN_LVXL);
  def_builtin ("__builtin_altivec_lvxl_v2df", v2df_ftype_long_pcvoid,
	       ALTIVEC_BUILTIN_LVXL_V2DF);
  def_builtin ("__builtin_altivec_lvxl_v2di", v2di_ftype_long_pcvoid,
	       ALTIVEC_BUILTIN_LVXL_V2DI);
  def_builtin ("__builtin_altivec_lvxl_v4sf", v4sf_ftype_long_pcvoid,
	       ALTIVEC_BUILTIN_LVXL_V4SF);
  def_builtin ("__builtin_altivec_lvxl_v4si", v4si_ftype_long_pcvoid,
	       ALTIVEC_BUILTIN_LVXL_V4SI);
  def_builtin ("__builtin_altivec_lvxl_v8hi", v8hi_ftype_long_pcvoid,
	       ALTIVEC_BUILTIN_LVXL_V8HI);
  def_builtin ("__builtin_altivec_lvxl_v16qi", v16qi_ftype_long_pcvoid,
	       ALTIVEC_BUILTIN_LVXL_V16QI);
  def_builtin ("__builtin_altivec_lvx", v4si_ftype_long_pcvoid, ALTIVEC_BUILTIN_LVX);
  def_builtin ("__builtin_altivec_lvx_v1ti", v1ti_ftype_long_pcvoid,
	       ALTIVEC_BUILTIN_LVX_V1TI);
  def_builtin ("__builtin_altivec_lvx_v2df", v2df_ftype_long_pcvoid,
	       ALTIVEC_BUILTIN_LVX_V2DF);
  def_builtin ("__builtin_altivec_lvx_v2di", v2di_ftype_long_pcvoid,
	       ALTIVEC_BUILTIN_LVX_V2DI);
  def_builtin ("__builtin_altivec_lvx_v4sf", v4sf_ftype_long_pcvoid,
	       ALTIVEC_BUILTIN_LVX_V4SF);
  def_builtin ("__builtin_altivec_lvx_v4si", v4si_ftype_long_pcvoid,
	       ALTIVEC_BUILTIN_LVX_V4SI);
  def_builtin ("__builtin_altivec_lvx_v8hi", v8hi_ftype_long_pcvoid,
	       ALTIVEC_BUILTIN_LVX_V8HI);
  def_builtin ("__builtin_altivec_lvx_v16qi", v16qi_ftype_long_pcvoid,
	       ALTIVEC_BUILTIN_LVX_V16QI);
  def_builtin ("__builtin_altivec_stvx", void_ftype_v4si_long_pvoid, ALTIVEC_BUILTIN_STVX);
  def_builtin ("__builtin_altivec_stvx_v2df", void_ftype_v2df_long_pvoid,
	       ALTIVEC_BUILTIN_STVX_V2DF);
  def_builtin ("__builtin_altivec_stvx_v2di", void_ftype_v2di_long_pvoid,
	       ALTIVEC_BUILTIN_STVX_V2DI);
  def_builtin ("__builtin_altivec_stvx_v4sf", void_ftype_v4sf_long_pvoid,
	       ALTIVEC_BUILTIN_STVX_V4SF);
  def_builtin ("__builtin_altivec_stvx_v4si", void_ftype_v4si_long_pvoid,
	       ALTIVEC_BUILTIN_STVX_V4SI);
  def_builtin ("__builtin_altivec_stvx_v8hi", void_ftype_v8hi_long_pvoid,
	       ALTIVEC_BUILTIN_STVX_V8HI);
  def_builtin ("__builtin_altivec_stvx_v16qi", void_ftype_v16qi_long_pvoid,
	       ALTIVEC_BUILTIN_STVX_V16QI);
  def_builtin ("__builtin_altivec_stvewx", void_ftype_v4si_long_pvoid, ALTIVEC_BUILTIN_STVEWX);
  def_builtin ("__builtin_altivec_stvxl", void_ftype_v4si_long_pvoid, ALTIVEC_BUILTIN_STVXL);
  def_builtin ("__builtin_altivec_stvxl_v2df", void_ftype_v2df_long_pvoid,
	       ALTIVEC_BUILTIN_STVXL_V2DF);
  def_builtin ("__builtin_altivec_stvxl_v2di", void_ftype_v2di_long_pvoid,
	       ALTIVEC_BUILTIN_STVXL_V2DI);
  def_builtin ("__builtin_altivec_stvxl_v4sf", void_ftype_v4sf_long_pvoid,
	       ALTIVEC_BUILTIN_STVXL_V4SF);
  def_builtin ("__builtin_altivec_stvxl_v4si", void_ftype_v4si_long_pvoid,
	       ALTIVEC_BUILTIN_STVXL_V4SI);
  def_builtin ("__builtin_altivec_stvxl_v8hi", void_ftype_v8hi_long_pvoid,
	       ALTIVEC_BUILTIN_STVXL_V8HI);
  def_builtin ("__builtin_altivec_stvxl_v16qi", void_ftype_v16qi_long_pvoid,
	       ALTIVEC_BUILTIN_STVXL_V16QI);
  def_builtin ("__builtin_altivec_stvebx", void_ftype_v16qi_long_pvoid, ALTIVEC_BUILTIN_STVEBX);
  def_builtin ("__builtin_altivec_stvehx", void_ftype_v8hi_long_pvoid, ALTIVEC_BUILTIN_STVEHX);
  def_builtin ("__builtin_vec_ld", opaque_ftype_long_pcvoid, ALTIVEC_BUILTIN_VEC_LD);
  def_builtin ("__builtin_vec_lde", opaque_ftype_long_pcvoid, ALTIVEC_BUILTIN_VEC_LDE);
  def_builtin ("__builtin_vec_ldl", opaque_ftype_long_pcvoid, ALTIVEC_BUILTIN_VEC_LDL);
  def_builtin ("__builtin_vec_lvsl", v16qi_ftype_long_pcvoid, ALTIVEC_BUILTIN_VEC_LVSL);
  def_builtin ("__builtin_vec_lvsr", v16qi_ftype_long_pcvoid, ALTIVEC_BUILTIN_VEC_LVSR);
  def_builtin ("__builtin_vec_lvebx", v16qi_ftype_long_pcvoid, ALTIVEC_BUILTIN_VEC_LVEBX);
  def_builtin ("__builtin_vec_lvehx", v8hi_ftype_long_pcvoid, ALTIVEC_BUILTIN_VEC_LVEHX);
  def_builtin ("__builtin_vec_lvewx", v4si_ftype_long_pcvoid, ALTIVEC_BUILTIN_VEC_LVEWX);
  def_builtin ("__builtin_vec_st", void_ftype_opaque_long_pvoid, ALTIVEC_BUILTIN_VEC_ST);
  def_builtin ("__builtin_vec_ste", void_ftype_opaque_long_pvoid, ALTIVEC_BUILTIN_VEC_STE);
  def_builtin ("__builtin_vec_stl", void_ftype_opaque_long_pvoid, ALTIVEC_BUILTIN_VEC_STL);
  def_builtin ("__builtin_vec_stvewx", void_ftype_opaque_long_pvoid, ALTIVEC_BUILTIN_VEC_STVEWX);
  def_builtin ("__builtin_vec_stvebx", void_ftype_opaque_long_pvoid, ALTIVEC_BUILTIN_VEC_STVEBX);
  def_builtin ("__builtin_vec_stvehx", void_ftype_opaque_long_pvoid, ALTIVEC_BUILTIN_VEC_STVEHX);

  def_builtin ("__builtin_vsx_lxvd2x_v2df", v2df_ftype_long_pcvoid,
	       VSX_BUILTIN_LXVD2X_V2DF);
  def_builtin ("__builtin_vsx_lxvd2x_v2di", v2di_ftype_long_pcvoid,
	       VSX_BUILTIN_LXVD2X_V2DI);
  def_builtin ("__builtin_vsx_lxvw4x_v4sf", v4sf_ftype_long_pcvoid,
	       VSX_BUILTIN_LXVW4X_V4SF);
  def_builtin ("__builtin_vsx_lxvw4x_v4si", v4si_ftype_long_pcvoid,
	       VSX_BUILTIN_LXVW4X_V4SI);
  def_builtin ("__builtin_vsx_lxvw4x_v8hi", v8hi_ftype_long_pcvoid,
	       VSX_BUILTIN_LXVW4X_V8HI);
  def_builtin ("__builtin_vsx_lxvw4x_v16qi", v16qi_ftype_long_pcvoid,
	       VSX_BUILTIN_LXVW4X_V16QI);
  def_builtin ("__builtin_vsx_stxvd2x_v2df", void_ftype_v2df_long_pvoid,
	       VSX_BUILTIN_STXVD2X_V2DF);
  def_builtin ("__builtin_vsx_stxvd2x_v2di", void_ftype_v2di_long_pvoid,
	       VSX_BUILTIN_STXVD2X_V2DI);
  def_builtin ("__builtin_vsx_stxvw4x_v4sf", void_ftype_v4sf_long_pvoid,
	       VSX_BUILTIN_STXVW4X_V4SF);
  def_builtin ("__builtin_vsx_stxvw4x_v4si", void_ftype_v4si_long_pvoid,
	       VSX_BUILTIN_STXVW4X_V4SI);
  def_builtin ("__builtin_vsx_stxvw4x_v8hi", void_ftype_v8hi_long_pvoid,
	       VSX_BUILTIN_STXVW4X_V8HI);
  def_builtin ("__builtin_vsx_stxvw4x_v16qi", void_ftype_v16qi_long_pvoid,
	       VSX_BUILTIN_STXVW4X_V16QI);

  def_builtin ("__builtin_vsx_ld_elemrev_v2df", v2df_ftype_long_pcvoid,
	       VSX_BUILTIN_LD_ELEMREV_V2DF);
  def_builtin ("__builtin_vsx_ld_elemrev_v2di", v2di_ftype_long_pcvoid,
	       VSX_BUILTIN_LD_ELEMREV_V2DI);
  def_builtin ("__builtin_vsx_ld_elemrev_v4sf", v4sf_ftype_long_pcvoid,
	       VSX_BUILTIN_LD_ELEMREV_V4SF);
  def_builtin ("__builtin_vsx_ld_elemrev_v4si", v4si_ftype_long_pcvoid,
	       VSX_BUILTIN_LD_ELEMREV_V4SI);
  def_builtin ("__builtin_vsx_ld_elemrev_v8hi", v8hi_ftype_long_pcvoid,
	       VSX_BUILTIN_LD_ELEMREV_V8HI);
  def_builtin ("__builtin_vsx_ld_elemrev_v16qi", v16qi_ftype_long_pcvoid,
	       VSX_BUILTIN_LD_ELEMREV_V16QI);
  def_builtin ("__builtin_vsx_st_elemrev_v2df", void_ftype_v2df_long_pvoid,
	       VSX_BUILTIN_ST_ELEMREV_V2DF);
  def_builtin ("__builtin_vsx_st_elemrev_v1ti", void_ftype_v1ti_long_pvoid,
	       VSX_BUILTIN_ST_ELEMREV_V1TI);
  def_builtin ("__builtin_vsx_st_elemrev_v2di", void_ftype_v2di_long_pvoid,
	       VSX_BUILTIN_ST_ELEMREV_V2DI);
  def_builtin ("__builtin_vsx_st_elemrev_v4sf", void_ftype_v4sf_long_pvoid,
	       VSX_BUILTIN_ST_ELEMREV_V4SF);
  def_builtin ("__builtin_vsx_st_elemrev_v4si", void_ftype_v4si_long_pvoid,
	       VSX_BUILTIN_ST_ELEMREV_V4SI);
  def_builtin ("__builtin_vsx_st_elemrev_v8hi", void_ftype_v8hi_long_pvoid,
	       VSX_BUILTIN_ST_ELEMREV_V8HI);
  def_builtin ("__builtin_vsx_st_elemrev_v16qi", void_ftype_v16qi_long_pvoid,
	       VSX_BUILTIN_ST_ELEMREV_V16QI);

  def_builtin ("__builtin_vec_vsx_ld", opaque_ftype_long_pcvoid,
	       VSX_BUILTIN_VEC_LD);
  def_builtin ("__builtin_vec_vsx_st", void_ftype_opaque_long_pvoid,
	       VSX_BUILTIN_VEC_ST);
  def_builtin ("__builtin_vec_xl", opaque_ftype_long_pcvoid,
	       VSX_BUILTIN_VEC_XL);
  def_builtin ("__builtin_vec_xl_be", opaque_ftype_long_pcvoid,
	       VSX_BUILTIN_VEC_XL_BE);
  def_builtin ("__builtin_vec_xst", void_ftype_opaque_long_pvoid,
	       VSX_BUILTIN_VEC_XST);
  def_builtin ("__builtin_vec_xst_be", void_ftype_opaque_long_pvoid,
	       VSX_BUILTIN_VEC_XST_BE);

  def_builtin ("__builtin_vec_step", int_ftype_opaque, ALTIVEC_BUILTIN_VEC_STEP);
  def_builtin ("__builtin_vec_splats", opaque_ftype_opaque, ALTIVEC_BUILTIN_VEC_SPLATS);
  def_builtin ("__builtin_vec_promote", opaque_ftype_opaque, ALTIVEC_BUILTIN_VEC_PROMOTE);

  def_builtin ("__builtin_vec_sld", opaque_ftype_opaque_opaque_int, ALTIVEC_BUILTIN_VEC_SLD);
  def_builtin ("__builtin_vec_splat", opaque_ftype_opaque_int, ALTIVEC_BUILTIN_VEC_SPLAT);
  def_builtin ("__builtin_vec_extract", opaque_ftype_opaque_int, ALTIVEC_BUILTIN_VEC_EXTRACT);
  def_builtin ("__builtin_vec_insert", opaque_ftype_opaque_opaque_int, ALTIVEC_BUILTIN_VEC_INSERT);
  def_builtin ("__builtin_vec_vspltw", opaque_ftype_opaque_int, ALTIVEC_BUILTIN_VEC_VSPLTW);
  def_builtin ("__builtin_vec_vsplth", opaque_ftype_opaque_int, ALTIVEC_BUILTIN_VEC_VSPLTH);
  def_builtin ("__builtin_vec_vspltb", opaque_ftype_opaque_int, ALTIVEC_BUILTIN_VEC_VSPLTB);
  def_builtin ("__builtin_vec_ctf", opaque_ftype_opaque_int, ALTIVEC_BUILTIN_VEC_CTF);
  def_builtin ("__builtin_vec_vcfsx", opaque_ftype_opaque_int, ALTIVEC_BUILTIN_VEC_VCFSX);
  def_builtin ("__builtin_vec_vcfux", opaque_ftype_opaque_int, ALTIVEC_BUILTIN_VEC_VCFUX);
  def_builtin ("__builtin_vec_cts", opaque_ftype_opaque_int, ALTIVEC_BUILTIN_VEC_CTS);
  def_builtin ("__builtin_vec_ctu", opaque_ftype_opaque_int, ALTIVEC_BUILTIN_VEC_CTU);

  def_builtin ("__builtin_vec_adde", opaque_ftype_opaque_opaque_opaque,
		ALTIVEC_BUILTIN_VEC_ADDE);
  def_builtin ("__builtin_vec_addec", opaque_ftype_opaque_opaque_opaque,
		ALTIVEC_BUILTIN_VEC_ADDEC);
  def_builtin ("__builtin_vec_cmpne", opaque_ftype_opaque_opaque,
		ALTIVEC_BUILTIN_VEC_CMPNE);
  def_builtin ("__builtin_vec_mul", opaque_ftype_opaque_opaque,
		ALTIVEC_BUILTIN_VEC_MUL);
  def_builtin ("__builtin_vec_sube", opaque_ftype_opaque_opaque_opaque,
		ALTIVEC_BUILTIN_VEC_SUBE);
  def_builtin ("__builtin_vec_subec", opaque_ftype_opaque_opaque_opaque,
		ALTIVEC_BUILTIN_VEC_SUBEC);

  /* Cell builtins.  */
  def_builtin ("__builtin_altivec_lvlx",  v16qi_ftype_long_pcvoid, ALTIVEC_BUILTIN_LVLX);
  def_builtin ("__builtin_altivec_lvlxl", v16qi_ftype_long_pcvoid, ALTIVEC_BUILTIN_LVLXL);
  def_builtin ("__builtin_altivec_lvrx",  v16qi_ftype_long_pcvoid, ALTIVEC_BUILTIN_LVRX);
  def_builtin ("__builtin_altivec_lvrxl", v16qi_ftype_long_pcvoid, ALTIVEC_BUILTIN_LVRXL);

  def_builtin ("__builtin_vec_lvlx",  v16qi_ftype_long_pcvoid, ALTIVEC_BUILTIN_VEC_LVLX);
  def_builtin ("__builtin_vec_lvlxl", v16qi_ftype_long_pcvoid, ALTIVEC_BUILTIN_VEC_LVLXL);
  def_builtin ("__builtin_vec_lvrx",  v16qi_ftype_long_pcvoid, ALTIVEC_BUILTIN_VEC_LVRX);
  def_builtin ("__builtin_vec_lvrxl", v16qi_ftype_long_pcvoid, ALTIVEC_BUILTIN_VEC_LVRXL);

  def_builtin ("__builtin_altivec_stvlx",  void_ftype_v16qi_long_pvoid, ALTIVEC_BUILTIN_STVLX);
  def_builtin ("__builtin_altivec_stvlxl", void_ftype_v16qi_long_pvoid, ALTIVEC_BUILTIN_STVLXL);
  def_builtin ("__builtin_altivec_stvrx",  void_ftype_v16qi_long_pvoid, ALTIVEC_BUILTIN_STVRX);
  def_builtin ("__builtin_altivec_stvrxl", void_ftype_v16qi_long_pvoid, ALTIVEC_BUILTIN_STVRXL);

  def_builtin ("__builtin_vec_stvlx",  void_ftype_v16qi_long_pvoid, ALTIVEC_BUILTIN_VEC_STVLX);
  def_builtin ("__builtin_vec_stvlxl", void_ftype_v16qi_long_pvoid, ALTIVEC_BUILTIN_VEC_STVLXL);
  def_builtin ("__builtin_vec_stvrx",  void_ftype_v16qi_long_pvoid, ALTIVEC_BUILTIN_VEC_STVRX);
  def_builtin ("__builtin_vec_stvrxl", void_ftype_v16qi_long_pvoid, ALTIVEC_BUILTIN_VEC_STVRXL);

  if (TARGET_P9_VECTOR)
    {
      def_builtin ("__builtin_altivec_stxvl", void_ftype_v16qi_pvoid_long,
		   P9V_BUILTIN_STXVL);
      def_builtin ("__builtin_xst_len_r", void_ftype_v16qi_pvoid_long,
		   P9V_BUILTIN_XST_LEN_R);
    }

  /* Add the DST variants.  */
  d = bdesc_dst;
  for (i = 0; i < ARRAY_SIZE (bdesc_dst); i++, d++)
    {
      HOST_WIDE_INT mask = d->mask;

      /* It is expected that these dst built-in functions may have
	 d->icode equal to CODE_FOR_nothing.  */
      if ((mask & builtin_mask) != mask)
	{
	  if (TARGET_DEBUG_BUILTIN)
	    fprintf (stderr, "altivec_init_builtins, skip dst %s\n",
		     d->name);
	  continue;
	}
      def_builtin (d->name, void_ftype_pcvoid_int_int, d->code);
    }

  /* Initialize the predicates.  */
  d = bdesc_altivec_preds;
  for (i = 0; i < ARRAY_SIZE (bdesc_altivec_preds); i++, d++)
    {
      machine_mode mode1;
      tree type;
      HOST_WIDE_INT mask = d->mask;

      if ((mask & builtin_mask) != mask)
	{
	  if (TARGET_DEBUG_BUILTIN)
	    fprintf (stderr, "altivec_init_builtins, skip predicate %s\n",
		     d->name);
	  continue;
	}

      if (rs6000_overloaded_builtin_p (d->code))
	mode1 = VOIDmode;
      else
	{
	  /* Cannot define builtin if the instruction is disabled.  */
	  gcc_assert (d->icode != CODE_FOR_nothing);
	  mode1 = insn_data[d->icode].operand[1].mode;
	}

      switch (mode1)
	{
	case E_VOIDmode:
	  type = int_ftype_int_opaque_opaque;
	  break;
	case E_V2DImode:
	  type = int_ftype_int_v2di_v2di;
	  break;
	case E_V4SImode:
	  type = int_ftype_int_v4si_v4si;
	  break;
	case E_V8HImode:
	  type = int_ftype_int_v8hi_v8hi;
	  break;
	case E_V16QImode:
	  type = int_ftype_int_v16qi_v16qi;
	  break;
	case E_V4SFmode:
	  type = int_ftype_int_v4sf_v4sf;
	  break;
	case E_V2DFmode:
	  type = int_ftype_int_v2df_v2df;
	  break;
	default:
	  gcc_unreachable ();
	}

      def_builtin (d->name, type, d->code);
    }

  /* Initialize the abs* operators.  */
  d = bdesc_abs;
  for (i = 0; i < ARRAY_SIZE (bdesc_abs); i++, d++)
    {
      machine_mode mode0;
      tree type;
      HOST_WIDE_INT mask = d->mask;

      if ((mask & builtin_mask) != mask)
	{
	  if (TARGET_DEBUG_BUILTIN)
	    fprintf (stderr, "altivec_init_builtins, skip abs %s\n",
		     d->name);
	  continue;
	}

      /* Cannot define builtin if the instruction is disabled.  */
      gcc_assert (d->icode != CODE_FOR_nothing);
      mode0 = insn_data[d->icode].operand[0].mode;

      switch (mode0)
	{
	case E_V2DImode:
	  type = v2di_ftype_v2di;
	  break;
	case E_V4SImode:
	  type = v4si_ftype_v4si;
	  break;
	case E_V8HImode:
	  type = v8hi_ftype_v8hi;
	  break;
	case E_V16QImode:
	  type = v16qi_ftype_v16qi;
	  break;
	case E_V4SFmode:
	  type = v4sf_ftype_v4sf;
	  break;
	case E_V2DFmode:
	  type = v2df_ftype_v2df;
	  break;
	default:
	  gcc_unreachable ();
	}

      def_builtin (d->name, type, d->code);
    }

  /* Initialize target builtin that implements
     targetm.vectorize.builtin_mask_for_load.  */

  decl = add_builtin_function ("__builtin_altivec_mask_for_load",
			       v16qi_ftype_long_pcvoid,
			       ALTIVEC_BUILTIN_MASK_FOR_LOAD,
			       BUILT_IN_MD, NULL, NULL_TREE);
  TREE_READONLY (decl) = 1;
  /* Record the decl. Will be used by rs6000_builtin_mask_for_load.  */
  altivec_builtin_mask_for_load = decl;

  /* Access to the vec_init patterns.  */
  ftype = build_function_type_list (V4SI_type_node, integer_type_node,
				    integer_type_node, integer_type_node,
				    integer_type_node, NULL_TREE);
  def_builtin ("__builtin_vec_init_v4si", ftype, ALTIVEC_BUILTIN_VEC_INIT_V4SI);

  ftype = build_function_type_list (V8HI_type_node, short_integer_type_node,
				    short_integer_type_node,
				    short_integer_type_node,
				    short_integer_type_node,
				    short_integer_type_node,
				    short_integer_type_node,
				    short_integer_type_node,
				    short_integer_type_node, NULL_TREE);
  def_builtin ("__builtin_vec_init_v8hi", ftype, ALTIVEC_BUILTIN_VEC_INIT_V8HI);

  ftype = build_function_type_list (V16QI_type_node, char_type_node,
				    char_type_node, char_type_node,
				    char_type_node, char_type_node,
				    char_type_node, char_type_node,
				    char_type_node, char_type_node,
				    char_type_node, char_type_node,
				    char_type_node, char_type_node,
				    char_type_node, char_type_node,
				    char_type_node, NULL_TREE);
  def_builtin ("__builtin_vec_init_v16qi", ftype,
	       ALTIVEC_BUILTIN_VEC_INIT_V16QI);

  ftype = build_function_type_list (V4SF_type_node, float_type_node,
				    float_type_node, float_type_node,
				    float_type_node, NULL_TREE);
  def_builtin ("__builtin_vec_init_v4sf", ftype, ALTIVEC_BUILTIN_VEC_INIT_V4SF);

  /* VSX builtins.  */
  ftype = build_function_type_list (V2DF_type_node, double_type_node,
				    double_type_node, NULL_TREE);
  def_builtin ("__builtin_vec_init_v2df", ftype, VSX_BUILTIN_VEC_INIT_V2DF);

  ftype = build_function_type_list (V2DI_type_node, intDI_type_node,
				    intDI_type_node, NULL_TREE);
  def_builtin ("__builtin_vec_init_v2di", ftype, VSX_BUILTIN_VEC_INIT_V2DI);

  /* Access to the vec_set patterns.  */
  ftype = build_function_type_list (V4SI_type_node, V4SI_type_node,
				    intSI_type_node,
				    integer_type_node, NULL_TREE);
  def_builtin ("__builtin_vec_set_v4si", ftype, ALTIVEC_BUILTIN_VEC_SET_V4SI);

  ftype = build_function_type_list (V8HI_type_node, V8HI_type_node,
				    intHI_type_node,
				    integer_type_node, NULL_TREE);
  def_builtin ("__builtin_vec_set_v8hi", ftype, ALTIVEC_BUILTIN_VEC_SET_V8HI);

  ftype = build_function_type_list (V16QI_type_node, V16QI_type_node,
				    intQI_type_node,
				    integer_type_node, NULL_TREE);
  def_builtin ("__builtin_vec_set_v16qi", ftype, ALTIVEC_BUILTIN_VEC_SET_V16QI);

  ftype = build_function_type_list (V4SF_type_node, V4SF_type_node,
				    float_type_node,
				    integer_type_node, NULL_TREE);
  def_builtin ("__builtin_vec_set_v4sf", ftype, ALTIVEC_BUILTIN_VEC_SET_V4SF);

  ftype = build_function_type_list (V2DF_type_node, V2DF_type_node,
				    double_type_node,
				    integer_type_node, NULL_TREE);
  def_builtin ("__builtin_vec_set_v2df", ftype, VSX_BUILTIN_VEC_SET_V2DF);

  ftype = build_function_type_list (V2DI_type_node, V2DI_type_node,
				    intDI_type_node,
				    integer_type_node, NULL_TREE);
  def_builtin ("__builtin_vec_set_v2di", ftype, VSX_BUILTIN_VEC_SET_V2DI);

  /* Access to the vec_extract patterns.  */
  ftype = build_function_type_list (intSI_type_node, V4SI_type_node,
				    integer_type_node, NULL_TREE);
  def_builtin ("__builtin_vec_ext_v4si", ftype, ALTIVEC_BUILTIN_VEC_EXT_V4SI);

  ftype = build_function_type_list (intHI_type_node, V8HI_type_node,
				    integer_type_node, NULL_TREE);
  def_builtin ("__builtin_vec_ext_v8hi", ftype, ALTIVEC_BUILTIN_VEC_EXT_V8HI);

  ftype = build_function_type_list (intQI_type_node, V16QI_type_node,
				    integer_type_node, NULL_TREE);
  def_builtin ("__builtin_vec_ext_v16qi", ftype, ALTIVEC_BUILTIN_VEC_EXT_V16QI);

  ftype = build_function_type_list (float_type_node, V4SF_type_node,
				    integer_type_node, NULL_TREE);
  def_builtin ("__builtin_vec_ext_v4sf", ftype, ALTIVEC_BUILTIN_VEC_EXT_V4SF);

  ftype = build_function_type_list (double_type_node, V2DF_type_node,
				    integer_type_node, NULL_TREE);
  def_builtin ("__builtin_vec_ext_v2df", ftype, VSX_BUILTIN_VEC_EXT_V2DF);

  ftype = build_function_type_list (intDI_type_node, V2DI_type_node,
				    integer_type_node, NULL_TREE);
  def_builtin ("__builtin_vec_ext_v2di", ftype, VSX_BUILTIN_VEC_EXT_V2DI);


  if (V1TI_type_node)
    {
      tree v1ti_ftype_long_pcvoid
	= build_function_type_list (V1TI_type_node,
				    long_integer_type_node, pcvoid_type_node,
				    NULL_TREE);
      tree void_ftype_v1ti_long_pvoid
	= build_function_type_list (void_type_node,
				    V1TI_type_node, long_integer_type_node,
				    pvoid_type_node, NULL_TREE);
      def_builtin ("__builtin_vsx_ld_elemrev_v1ti", v1ti_ftype_long_pcvoid,
		   VSX_BUILTIN_LD_ELEMREV_V1TI);
      def_builtin ("__builtin_vsx_lxvd2x_v1ti", v1ti_ftype_long_pcvoid,
		   VSX_BUILTIN_LXVD2X_V1TI);
      def_builtin ("__builtin_vsx_stxvd2x_v1ti", void_ftype_v1ti_long_pvoid,
		   VSX_BUILTIN_STXVD2X_V1TI);
      ftype = build_function_type_list (V1TI_type_node, intTI_type_node,
					NULL_TREE, NULL_TREE);
      def_builtin ("__builtin_vec_init_v1ti", ftype, VSX_BUILTIN_VEC_INIT_V1TI);
      ftype = build_function_type_list (V1TI_type_node, V1TI_type_node,
					intTI_type_node,
					integer_type_node, NULL_TREE);
      def_builtin ("__builtin_vec_set_v1ti", ftype, VSX_BUILTIN_VEC_SET_V1TI);
      ftype = build_function_type_list (intTI_type_node, V1TI_type_node,
					integer_type_node, NULL_TREE);
      def_builtin ("__builtin_vec_ext_v1ti", ftype, VSX_BUILTIN_VEC_EXT_V1TI);
    }

}

static void
htm_init_builtins (void)
{
  HOST_WIDE_INT builtin_mask = rs6000_builtin_mask;
  const struct builtin_description *d;
  size_t i;

  d = bdesc_htm;
  for (i = 0; i < ARRAY_SIZE (bdesc_htm); i++, d++)
    {
      tree op[MAX_HTM_OPERANDS], type;
      HOST_WIDE_INT mask = d->mask;
      unsigned attr = rs6000_builtin_info[d->code].attr;
      bool void_func = (attr & RS6000_BTC_VOID);
      int attr_args = (attr & RS6000_BTC_TYPE_MASK);
      int nopnds = 0;
      tree gpr_type_node;
      tree rettype;
      tree argtype;

      /* It is expected that these htm built-in functions may have
	 d->icode equal to CODE_FOR_nothing.  */

      if (TARGET_32BIT && TARGET_POWERPC64)
	gpr_type_node = long_long_unsigned_type_node;
      else
	gpr_type_node = long_unsigned_type_node;

      if (attr & RS6000_BTC_SPR)
	{
	  rettype = gpr_type_node;
	  argtype = gpr_type_node;
	}
      else if (d->code == HTM_BUILTIN_TABORTDC
	       || d->code == HTM_BUILTIN_TABORTDCI)
	{
	  rettype = unsigned_type_node;
	  argtype = gpr_type_node;
	}
      else
	{
	  rettype = unsigned_type_node;
	  argtype = unsigned_type_node;
	}

      if ((mask & builtin_mask) != mask)
	{
	  if (TARGET_DEBUG_BUILTIN)
	    fprintf (stderr, "htm_builtin, skip binary %s\n", d->name);
	  continue;
	}

      if (d->name == 0)
	{
	  if (TARGET_DEBUG_BUILTIN)
	    fprintf (stderr, "htm_builtin, bdesc_htm[%ld] no name\n",
		     (long unsigned) i);
	  continue;
	}

      op[nopnds++] = (void_func) ? void_type_node : rettype;

      if (attr_args == RS6000_BTC_UNARY)
	op[nopnds++] = argtype;
      else if (attr_args == RS6000_BTC_BINARY)
	{
	  op[nopnds++] = argtype;
	  op[nopnds++] = argtype;
	}
      else if (attr_args == RS6000_BTC_TERNARY)
	{
	  op[nopnds++] = argtype;
	  op[nopnds++] = argtype;
	  op[nopnds++] = argtype;
	}

      switch (nopnds)
	{
	case 1:
	  type = build_function_type_list (op[0], NULL_TREE);
	  break;
	case 2:
	  type = build_function_type_list (op[0], op[1], NULL_TREE);
	  break;
	case 3:
	  type = build_function_type_list (op[0], op[1], op[2], NULL_TREE);
	  break;
	case 4:
	  type = build_function_type_list (op[0], op[1], op[2], op[3],
					   NULL_TREE);
	  break;
	default:
	  gcc_unreachable ();
	}

      def_builtin (d->name, type, d->code);
    }
}

/* Map types for builtin functions with an explicit return type and up to 3
   arguments.  Functions with fewer than 3 arguments use VOIDmode as the type
   of the argument.  */
static tree
builtin_function_type (machine_mode mode_ret, machine_mode mode_arg0,
		       machine_mode mode_arg1, machine_mode mode_arg2,
		       enum rs6000_builtins builtin, const char *name)
{
  struct builtin_hash_struct h;
  struct builtin_hash_struct *h2;
  int num_args = 3;
  int i;
  tree ret_type = NULL_TREE;
  tree arg_type[3] = { NULL_TREE, NULL_TREE, NULL_TREE };

  /* Create builtin_hash_table.  */
  if (builtin_hash_table == NULL)
    builtin_hash_table = hash_table<builtin_hasher>::create_ggc (1500);

  h.type = NULL_TREE;
  h.mode[0] = mode_ret;
  h.mode[1] = mode_arg0;
  h.mode[2] = mode_arg1;
  h.mode[3] = mode_arg2;
  h.uns_p[0] = 0;
  h.uns_p[1] = 0;
  h.uns_p[2] = 0;
  h.uns_p[3] = 0;

  /* If the builtin is a type that produces unsigned results or takes unsigned
     arguments, and it is returned as a decl for the vectorizer (such as
     widening multiplies, permute), make sure the arguments and return value
     are type correct.  */
  switch (builtin)
    {
    /* unsigned 1 argument functions.  */
    case CRYPTO_BUILTIN_VSBOX:
    case CRYPTO_BUILTIN_VSBOX_BE:
    case P8V_BUILTIN_VGBBD:
    case MISC_BUILTIN_CDTBCD:
    case MISC_BUILTIN_CBCDTD:
      h.uns_p[0] = 1;
      h.uns_p[1] = 1;
      break;

    /* unsigned 2 argument functions.  */
    case ALTIVEC_BUILTIN_VMULEUB:
    case ALTIVEC_BUILTIN_VMULEUH:
    case P8V_BUILTIN_VMULEUW:
    case ALTIVEC_BUILTIN_VMULOUB:
    case ALTIVEC_BUILTIN_VMULOUH:
    case P8V_BUILTIN_VMULOUW:
    case CRYPTO_BUILTIN_VCIPHER:
    case CRYPTO_BUILTIN_VCIPHER_BE:
    case CRYPTO_BUILTIN_VCIPHERLAST:
    case CRYPTO_BUILTIN_VCIPHERLAST_BE:
    case CRYPTO_BUILTIN_VNCIPHER:
    case CRYPTO_BUILTIN_VNCIPHER_BE:
    case CRYPTO_BUILTIN_VNCIPHERLAST:
    case CRYPTO_BUILTIN_VNCIPHERLAST_BE:
    case CRYPTO_BUILTIN_VPMSUMB:
    case CRYPTO_BUILTIN_VPMSUMH:
    case CRYPTO_BUILTIN_VPMSUMW:
    case CRYPTO_BUILTIN_VPMSUMD:
    case CRYPTO_BUILTIN_VPMSUM:
    case MISC_BUILTIN_ADDG6S:
    case MISC_BUILTIN_DIVWEU:
    case MISC_BUILTIN_DIVDEU:
    case VSX_BUILTIN_UDIV_V2DI:
    case ALTIVEC_BUILTIN_VMAXUB:
    case ALTIVEC_BUILTIN_VMINUB:
    case ALTIVEC_BUILTIN_VMAXUH:
    case ALTIVEC_BUILTIN_VMINUH:
    case ALTIVEC_BUILTIN_VMAXUW:
    case ALTIVEC_BUILTIN_VMINUW:
    case P8V_BUILTIN_VMAXUD:
    case P8V_BUILTIN_VMINUD:
      h.uns_p[0] = 1;
      h.uns_p[1] = 1;
      h.uns_p[2] = 1;
      break;

    /* unsigned 3 argument functions.  */
    case ALTIVEC_BUILTIN_VPERM_16QI_UNS:
    case ALTIVEC_BUILTIN_VPERM_8HI_UNS:
    case ALTIVEC_BUILTIN_VPERM_4SI_UNS:
    case ALTIVEC_BUILTIN_VPERM_2DI_UNS:
    case ALTIVEC_BUILTIN_VSEL_16QI_UNS:
    case ALTIVEC_BUILTIN_VSEL_8HI_UNS:
    case ALTIVEC_BUILTIN_VSEL_4SI_UNS:
    case ALTIVEC_BUILTIN_VSEL_2DI_UNS:
    case VSX_BUILTIN_VPERM_16QI_UNS:
    case VSX_BUILTIN_VPERM_8HI_UNS:
    case VSX_BUILTIN_VPERM_4SI_UNS:
    case VSX_BUILTIN_VPERM_2DI_UNS:
    case VSX_BUILTIN_XXSEL_16QI_UNS:
    case VSX_BUILTIN_XXSEL_8HI_UNS:
    case VSX_BUILTIN_XXSEL_4SI_UNS:
    case VSX_BUILTIN_XXSEL_2DI_UNS:
    case CRYPTO_BUILTIN_VPERMXOR:
    case CRYPTO_BUILTIN_VPERMXOR_V2DI:
    case CRYPTO_BUILTIN_VPERMXOR_V4SI:
    case CRYPTO_BUILTIN_VPERMXOR_V8HI:
    case CRYPTO_BUILTIN_VPERMXOR_V16QI:
    case CRYPTO_BUILTIN_VSHASIGMAW:
    case CRYPTO_BUILTIN_VSHASIGMAD:
    case CRYPTO_BUILTIN_VSHASIGMA:
      h.uns_p[0] = 1;
      h.uns_p[1] = 1;
      h.uns_p[2] = 1;
      h.uns_p[3] = 1;
      break;

    /* signed permute functions with unsigned char mask.  */
    case ALTIVEC_BUILTIN_VPERM_16QI:
    case ALTIVEC_BUILTIN_VPERM_8HI:
    case ALTIVEC_BUILTIN_VPERM_4SI:
    case ALTIVEC_BUILTIN_VPERM_4SF:
    case ALTIVEC_BUILTIN_VPERM_2DI:
    case ALTIVEC_BUILTIN_VPERM_2DF:
    case VSX_BUILTIN_VPERM_16QI:
    case VSX_BUILTIN_VPERM_8HI:
    case VSX_BUILTIN_VPERM_4SI:
    case VSX_BUILTIN_VPERM_4SF:
    case VSX_BUILTIN_VPERM_2DI:
    case VSX_BUILTIN_VPERM_2DF:
      h.uns_p[3] = 1;
      break;

    /* unsigned args, signed return.  */
    case VSX_BUILTIN_XVCVUXDSP:
    case VSX_BUILTIN_XVCVUXDDP_UNS:
    case ALTIVEC_BUILTIN_UNSFLOAT_V4SI_V4SF:
      h.uns_p[1] = 1;
      break;

    /* signed args, unsigned return.  */
    case VSX_BUILTIN_XVCVDPUXDS_UNS:
    case ALTIVEC_BUILTIN_FIXUNS_V4SF_V4SI:
    case MISC_BUILTIN_UNPACK_TD:
    case MISC_BUILTIN_UNPACK_V1TI:
      h.uns_p[0] = 1;
      break;

    /* unsigned arguments, bool return (compares).  */
    case ALTIVEC_BUILTIN_VCMPEQUB:
    case ALTIVEC_BUILTIN_VCMPEQUH:
    case ALTIVEC_BUILTIN_VCMPEQUW:
    case P8V_BUILTIN_VCMPEQUD:
    case VSX_BUILTIN_CMPGE_U16QI:
    case VSX_BUILTIN_CMPGE_U8HI:
    case VSX_BUILTIN_CMPGE_U4SI:
    case VSX_BUILTIN_CMPGE_U2DI:
    case ALTIVEC_BUILTIN_VCMPGTUB:
    case ALTIVEC_BUILTIN_VCMPGTUH:
    case ALTIVEC_BUILTIN_VCMPGTUW:
    case P8V_BUILTIN_VCMPGTUD:
      h.uns_p[1] = 1;
      h.uns_p[2] = 1;
      break;

    /* unsigned arguments for 128-bit pack instructions.  */
    case MISC_BUILTIN_PACK_TD:
    case MISC_BUILTIN_PACK_V1TI:
      h.uns_p[1] = 1;
      h.uns_p[2] = 1;
      break;

    /* unsigned second arguments (vector shift right).  */
    case ALTIVEC_BUILTIN_VSRB:
    case ALTIVEC_BUILTIN_VSRH:
    case ALTIVEC_BUILTIN_VSRW:
    case P8V_BUILTIN_VSRD:
      h.uns_p[2] = 1;
      break;

    default:
      break;
    }

  /* Figure out how many args are present.  */
  while (num_args > 0 && h.mode[num_args] == VOIDmode)
    num_args--;

  ret_type = builtin_mode_to_type[h.mode[0]][h.uns_p[0]];
  if (!ret_type && h.uns_p[0])
    ret_type = builtin_mode_to_type[h.mode[0]][0];

  if (!ret_type)
    fatal_error (input_location,
		 "internal error: builtin function %qs had an unexpected "
		 "return type %qs", name, GET_MODE_NAME (h.mode[0]));

  for (i = 0; i < (int) ARRAY_SIZE (arg_type); i++)
    arg_type[i] = NULL_TREE;

  for (i = 0; i < num_args; i++)
    {
      int m = (int) h.mode[i+1];
      int uns_p = h.uns_p[i+1];

      arg_type[i] = builtin_mode_to_type[m][uns_p];
      if (!arg_type[i] && uns_p)
	arg_type[i] = builtin_mode_to_type[m][0];

      if (!arg_type[i])
	fatal_error (input_location,
		     "internal error: builtin function %qs, argument %d "
		     "had unexpected argument type %qs", name, i,
		     GET_MODE_NAME (m));
    }

  builtin_hash_struct **found = builtin_hash_table->find_slot (&h, INSERT);
  if (*found == NULL)
    {
      h2 = ggc_alloc<builtin_hash_struct> ();
      *h2 = h;
      *found = h2;

      h2->type = build_function_type_list (ret_type, arg_type[0], arg_type[1],
					   arg_type[2], NULL_TREE);
    }

  return (*found)->type;
}

static void
rs6000_common_init_builtins (void)
{
  const struct builtin_description *d;
  size_t i;

  tree opaque_ftype_opaque = NULL_TREE;
  tree opaque_ftype_opaque_opaque = NULL_TREE;
  tree opaque_ftype_opaque_opaque_opaque = NULL_TREE;
  HOST_WIDE_INT builtin_mask = rs6000_builtin_mask;

  /* Create Altivec and VSX builtins on machines with at least the
     general purpose extensions (970 and newer) to allow the use of
     the target attribute.  */

  if (TARGET_EXTRA_BUILTINS)
    builtin_mask |= RS6000_BTM_COMMON;

  /* Add the ternary operators.  */
  d = bdesc_3arg;
  for (i = 0; i < ARRAY_SIZE (bdesc_3arg); i++, d++)
    {
      tree type;
      HOST_WIDE_INT mask = d->mask;

      if ((mask & builtin_mask) != mask)
	{
	  if (TARGET_DEBUG_BUILTIN)
	    fprintf (stderr, "rs6000_builtin, skip ternary %s\n", d->name);
	  continue;
	}

      if (rs6000_overloaded_builtin_p (d->code))
	{
	  if (! (type = opaque_ftype_opaque_opaque_opaque))
	    type = opaque_ftype_opaque_opaque_opaque
	      = build_function_type_list (opaque_V4SI_type_node,
					  opaque_V4SI_type_node,
					  opaque_V4SI_type_node,
					  opaque_V4SI_type_node,
					  NULL_TREE);
	}
      else
	{
	  enum insn_code icode = d->icode;
	  if (d->name == 0)
	    {
	      if (TARGET_DEBUG_BUILTIN)
		fprintf (stderr, "rs6000_builtin, bdesc_3arg[%ld] no name\n",
			 (long unsigned)i);

	      continue;
	    }

          if (icode == CODE_FOR_nothing)
	    {
	      if (TARGET_DEBUG_BUILTIN)
		fprintf (stderr, "rs6000_builtin, skip ternary %s (no code)\n",
			 d->name);

	      continue;
	    }

	  type = builtin_function_type (insn_data[icode].operand[0].mode,
					insn_data[icode].operand[1].mode,
					insn_data[icode].operand[2].mode,
					insn_data[icode].operand[3].mode,
					d->code, d->name);
	}

      def_builtin (d->name, type, d->code);
    }

  /* Add the binary operators.  */
  d = bdesc_2arg;
  for (i = 0; i < ARRAY_SIZE (bdesc_2arg); i++, d++)
    {
      machine_mode mode0, mode1, mode2;
      tree type;
      HOST_WIDE_INT mask = d->mask;

      if ((mask & builtin_mask) != mask)
	{
	  if (TARGET_DEBUG_BUILTIN)
	    fprintf (stderr, "rs6000_builtin, skip binary %s\n", d->name);
	  continue;
	}

      if (rs6000_overloaded_builtin_p (d->code))
	{
	  if (! (type = opaque_ftype_opaque_opaque))
	    type = opaque_ftype_opaque_opaque
	      = build_function_type_list (opaque_V4SI_type_node,
					  opaque_V4SI_type_node,
					  opaque_V4SI_type_node,
					  NULL_TREE);
	}
      else
	{
	  enum insn_code icode = d->icode;
	  if (d->name == 0)
	    {
	      if (TARGET_DEBUG_BUILTIN)
		fprintf (stderr, "rs6000_builtin, bdesc_2arg[%ld] no name\n",
			 (long unsigned)i);

	      continue;
	    }

          if (icode == CODE_FOR_nothing)
	    {
	      if (TARGET_DEBUG_BUILTIN)
		fprintf (stderr, "rs6000_builtin, skip binary %s (no code)\n",
			 d->name);

	      continue;
	    }

          mode0 = insn_data[icode].operand[0].mode;
          mode1 = insn_data[icode].operand[1].mode;
          mode2 = insn_data[icode].operand[2].mode;

	  type = builtin_function_type (mode0, mode1, mode2, VOIDmode,
					d->code, d->name);
	}

      def_builtin (d->name, type, d->code);
    }

  /* Add the simple unary operators.  */
  d = bdesc_1arg;
  for (i = 0; i < ARRAY_SIZE (bdesc_1arg); i++, d++)
    {
      machine_mode mode0, mode1;
      tree type;
      HOST_WIDE_INT mask = d->mask;

      if ((mask & builtin_mask) != mask)
	{
	  if (TARGET_DEBUG_BUILTIN)
	    fprintf (stderr, "rs6000_builtin, skip unary %s\n", d->name);
	  continue;
	}

      if (rs6000_overloaded_builtin_p (d->code))
	{
	  if (! (type = opaque_ftype_opaque))
	    type = opaque_ftype_opaque
	      = build_function_type_list (opaque_V4SI_type_node,
					  opaque_V4SI_type_node,
					  NULL_TREE);
	}
      else
        {
	  enum insn_code icode = d->icode;
	  if (d->name == 0)
	    {
	      if (TARGET_DEBUG_BUILTIN)
		fprintf (stderr, "rs6000_builtin, bdesc_1arg[%ld] no name\n",
			 (long unsigned)i);

	      continue;
	    }

          if (icode == CODE_FOR_nothing)
	    {
	      if (TARGET_DEBUG_BUILTIN)
		fprintf (stderr, "rs6000_builtin, skip unary %s (no code)\n",
			 d->name);

	      continue;
	    }

          mode0 = insn_data[icode].operand[0].mode;
          mode1 = insn_data[icode].operand[1].mode;

	  type = builtin_function_type (mode0, mode1, VOIDmode, VOIDmode,
					d->code, d->name);
	}

      def_builtin (d->name, type, d->code);
    }

  /* Add the simple no-argument operators.  */
  d = bdesc_0arg;
  for (i = 0; i < ARRAY_SIZE (bdesc_0arg); i++, d++)
    {
      machine_mode mode0;
      tree type;
      HOST_WIDE_INT mask = d->mask;

      if ((mask & builtin_mask) != mask)
	{
	  if (TARGET_DEBUG_BUILTIN)
	    fprintf (stderr, "rs6000_builtin, skip no-argument %s\n", d->name);
	  continue;
	}
      if (rs6000_overloaded_builtin_p (d->code))
	{
	  if (!opaque_ftype_opaque)
	    opaque_ftype_opaque
	      = build_function_type_list (opaque_V4SI_type_node, NULL_TREE);
	  type = opaque_ftype_opaque;
	}
      else
	{
	  enum insn_code icode = d->icode;
	  if (d->name == 0)
	    {
	      if (TARGET_DEBUG_BUILTIN)
		fprintf (stderr, "rs6000_builtin, bdesc_0arg[%lu] no name\n",
			 (long unsigned) i);
	      continue;
	    }
	  if (icode == CODE_FOR_nothing)
	    {
	      if (TARGET_DEBUG_BUILTIN)
		fprintf (stderr,
			 "rs6000_builtin, skip no-argument %s (no code)\n",
			 d->name);
	      continue;
	    }
	  mode0 = insn_data[icode].operand[0].mode;
	  type = builtin_function_type (mode0, VOIDmode, VOIDmode, VOIDmode,
					d->code, d->name);
	}
      def_builtin (d->name, type, d->code);
    }
}

/* Return the internal arg pointer used for function incoming
   arguments.  When -fsplit-stack, the arg pointer is r12 so we need
   to copy it to a pseudo in order for it to be preserved over calls
   and suchlike.  We'd really like to use a pseudo here for the
   internal arg pointer but data-flow analysis is not prepared to
   accept pseudos as live at the beginning of a function.  */

rtx
rs6000_internal_arg_pointer (void)
{
  if (flag_split_stack
     && (lookup_attribute ("no_split_stack", DECL_ATTRIBUTES (cfun->decl))
         == NULL))

    {
      if (cfun->machine->split_stack_arg_pointer == NULL_RTX)
	{
	  rtx pat;

	  cfun->machine->split_stack_arg_pointer = gen_reg_rtx (Pmode);
	  REG_POINTER (cfun->machine->split_stack_arg_pointer) = 1;

	  /* Put the pseudo initialization right after the note at the
	     beginning of the function.  */
	  pat = gen_rtx_SET (cfun->machine->split_stack_arg_pointer,
			     gen_rtx_REG (Pmode, 12));
	  push_topmost_sequence ();
	  emit_insn_after (pat, get_insns ());
	  pop_topmost_sequence ();
	}
      rtx ret = plus_constant (Pmode, cfun->machine->split_stack_arg_pointer,
			       FIRST_PARM_OFFSET (current_function_decl));
      return copy_to_reg (ret);
    }
  return virtual_incoming_args_rtx;
}


/* A C compound statement that outputs the assembler code for a thunk
   function, used to implement C++ virtual function calls with
   multiple inheritance.  The thunk acts as a wrapper around a virtual
   function, adjusting the implicit object parameter before handing
   control off to the real function.

   First, emit code to add the integer DELTA to the location that
   contains the incoming first argument.  Assume that this argument
   contains a pointer, and is the one used to pass the `this' pointer
   in C++.  This is the incoming argument *before* the function
   prologue, e.g. `%o0' on a sparc.  The addition must preserve the
   values of all other incoming arguments.

   After the addition, emit code to jump to FUNCTION, which is a
   `FUNCTION_DECL'.  This is a direct pure jump, not a call, and does
   not touch the return address.  Hence returning from FUNCTION will
   return to whoever called the current `thunk'.

   The effect must be as if FUNCTION had been called directly with the
   adjusted first argument.  This macro is responsible for emitting
   all of the code for a thunk function; output_function_prologue()
   and output_function_epilogue() are not invoked.

   The THUNK_FNDECL is redundant.  (DELTA and FUNCTION have already
   been extracted from it.)  It might possibly be useful on some
   targets, but probably not.

   If you do not define this macro, the target-independent code in the
   C++ frontend will generate a less efficient heavyweight thunk that
   calls FUNCTION instead of jumping to it.  The generic approach does
   not support varargs.  */

void
rs6000_output_mi_thunk (FILE *file, tree thunk_fndecl ATTRIBUTE_UNUSED,
			HOST_WIDE_INT delta, HOST_WIDE_INT vcall_offset,
			tree function)
{
  const char *fnname = IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (thunk_fndecl));
  rtx this_rtx, funexp;
  rtx_insn *insn;

  reload_completed = 1;
  epilogue_completed = 1;

  /* Mark the end of the (empty) prologue.  */
  emit_note (NOTE_INSN_PROLOGUE_END);

  /* Find the "this" pointer.  If the function returns a structure,
     the structure return pointer is in r3.  */
  if (aggregate_value_p (TREE_TYPE (TREE_TYPE (function)), function))
    this_rtx = gen_rtx_REG (Pmode, 4);
  else
    this_rtx = gen_rtx_REG (Pmode, 3);

  /* Apply the constant offset, if required.  */
  if (delta)
    emit_insn (gen_add3_insn (this_rtx, this_rtx, GEN_INT (delta)));

  /* Apply the offset from the vtable, if required.  */
  if (vcall_offset)
    {
      rtx vcall_offset_rtx = GEN_INT (vcall_offset);
      rtx tmp = gen_rtx_REG (Pmode, 12);

      emit_move_insn (tmp, gen_rtx_MEM (Pmode, this_rtx));
      if (((unsigned HOST_WIDE_INT) vcall_offset) + 0x8000 >= 0x10000)
	{
	  emit_insn (gen_add3_insn (tmp, tmp, vcall_offset_rtx));
	  emit_move_insn (tmp, gen_rtx_MEM (Pmode, tmp));
	}
      else
	{
	  rtx loc = gen_rtx_PLUS (Pmode, tmp, vcall_offset_rtx);

	  emit_move_insn (tmp, gen_rtx_MEM (Pmode, loc));
	}
      emit_insn (gen_add3_insn (this_rtx, this_rtx, tmp));
    }

  /* Generate a tail call to the target function.  */
  if (!TREE_USED (function))
    {
      assemble_external (function);
      TREE_USED (function) = 1;
    }
  funexp = XEXP (DECL_RTL (function), 0);
  funexp = gen_rtx_MEM (FUNCTION_MODE, funexp);

  insn = emit_call_insn (gen_sibcall (funexp, const0_rtx, const0_rtx));
  SIBLING_CALL_P (insn) = 1;
  emit_barrier ();

  /* Run just enough of rest_of_compilation to get the insns emitted.
     There's not really enough bulk here to make other passes such as
     instruction scheduling worth while.  */
  insn = get_insns ();
  shorten_branches (insn);
  assemble_start_function (thunk_fndecl, fnname);
  final_start_function (insn, file, 1);
  final (insn, file, 1);
  final_end_function ();
  assemble_end_function (thunk_fndecl, fnname);

  reload_completed = 0;
  epilogue_completed = 0;
}

#include "gt-rs6000-call.h"
