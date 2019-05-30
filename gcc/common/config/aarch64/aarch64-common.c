/* Common hooks for AArch64.
   Copyright (C) 2012-2019 Free Software Foundation, Inc.
   Contributed by ARM Ltd.

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

#include "config.h"
#define INCLUDE_STRING
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "memmodel.h"
#include "tm_p.h"
#include "common/common-target.h"
#include "common/common-target-def.h"
#include "opts.h"
#include "flags.h"
#include "diagnostic.h"
#include "params.h"

#ifdef  TARGET_BIG_ENDIAN_DEFAULT
#undef  TARGET_DEFAULT_TARGET_FLAGS
#define TARGET_DEFAULT_TARGET_FLAGS (MASK_BIG_END)
#endif

#undef  TARGET_HANDLE_OPTION
#define TARGET_HANDLE_OPTION aarch64_handle_option

#undef	TARGET_OPTION_OPTIMIZATION_TABLE
#define TARGET_OPTION_OPTIMIZATION_TABLE aarch_option_optimization_table
#undef TARGET_OPTION_DEFAULT_PARAMS
#define TARGET_OPTION_DEFAULT_PARAMS aarch64_option_default_params
#undef TARGET_OPTION_VALIDATE_PARAM
#define TARGET_OPTION_VALIDATE_PARAM aarch64_option_validate_param
#undef TARGET_OPTION_INIT_STRUCT
#define TARGET_OPTION_INIT_STRUCT aarch64_option_init_struct

/* Set default optimization options.  */
static const struct default_options aarch_option_optimization_table[] =
  {
    /* Enable section anchors by default at -O1 or higher.  */
    { OPT_LEVELS_1_PLUS, OPT_fsection_anchors, NULL, 1 },
    /* Disable fomit-frame-pointer by default.  */
    { OPT_LEVELS_ALL, OPT_fomit_frame_pointer, NULL, 0 },
    /* Enable -fsched-pressure by default when optimizing.  */
    { OPT_LEVELS_1_PLUS, OPT_fsched_pressure, NULL, 1 },
    /* Enable redundant extension instructions removal at -O2 and higher.  */
    { OPT_LEVELS_2_PLUS, OPT_free, NULL, 1 },
#if (TARGET_DEFAULT_ASYNC_UNWIND_TABLES == 1)
    { OPT_LEVELS_ALL, OPT_fasynchronous_unwind_tables, NULL, 1 },
    { OPT_LEVELS_ALL, OPT_funwind_tables, NULL, 1},
#endif
    { OPT_LEVELS_NONE, 0, NULL, 0 }
  };

/* Implement target validation TARGET_OPTION_DEFAULT_PARAM.  */

static bool
aarch64_option_validate_param (const int value, const int param)
{
  /* Check that both parameters are the same.  */
  if (param == (int) PARAM_STACK_CLASH_PROTECTION_GUARD_SIZE)
    {
      if (value != 12 && value != 16)
	{
	  error ("only values 12 (4 KB) and 16 (64 KB) are supported for guard "
		 "size.  Given value %d (%llu KB) is out of range",
		 value, (1ULL << value) / 1024ULL);
	  return false;
	}
    }

  return true;
}

/* Implement TARGET_OPTION_DEFAULT_PARAMS.  */

static void
aarch64_option_default_params (void)
{
  /* We assume the guard page is 64k.  */
  int index = (int) PARAM_STACK_CLASH_PROTECTION_GUARD_SIZE;
  set_default_param_value (PARAM_STACK_CLASH_PROTECTION_GUARD_SIZE,
			   DEFAULT_STK_CLASH_GUARD_SIZE == 0
			     ? 16 : DEFAULT_STK_CLASH_GUARD_SIZE);

  int guard_size
    = default_param_value (PARAM_STACK_CLASH_PROTECTION_GUARD_SIZE);

  /* Set the interval parameter to be the same as the guard size.  This way the
     mid-end code does the right thing for us.  */
  set_default_param_value (PARAM_STACK_CLASH_PROTECTION_PROBE_INTERVAL,
			   guard_size);

  /* Validate the options.  */
  aarch64_option_validate_param (guard_size, index);
}

/* Implement TARGET_HANDLE_OPTION.
   This function handles the target specific options for CPU/target selection.

   -mcpu=CPU is shorthand for -march=ARCH_FOR_CPU, -mtune=CPU.
   If either of -march or -mtune is given, they override their
   respective component of -mcpu.  This logic is implemented
   in config/aarch64/aarch64.c:aarch64_override_options.  */

bool
aarch64_handle_option (struct gcc_options *opts,
		       struct gcc_options *opts_set ATTRIBUTE_UNUSED,
		       const struct cl_decoded_option *decoded,
		       location_t loc ATTRIBUTE_UNUSED)
{
  size_t code = decoded->opt_index;
  const char *arg = decoded->arg;
  int val = decoded->value;

  switch (code)
    {
    case OPT_march_:
      opts->x_aarch64_arch_string = arg;
      return true;

    case OPT_mcpu_:
      opts->x_aarch64_cpu_string = arg;
      return true;

    case OPT_mtune_:
      opts->x_aarch64_tune_string = arg;
      return true;

    case OPT_mgeneral_regs_only:
      opts->x_target_flags |= MASK_GENERAL_REGS_ONLY;
      return true;

    case OPT_mfix_cortex_a53_835769:
      opts->x_aarch64_fix_a53_err835769 = val;
      return true;

    case OPT_mstrict_align:
      if (val)
	opts->x_target_flags |= MASK_STRICT_ALIGN;
      else
	opts->x_target_flags &= ~MASK_STRICT_ALIGN;
      return true;

    case OPT_momit_leaf_frame_pointer:
      opts->x_flag_omit_leaf_frame_pointer = val;
      return true;

    default:
      return true;
    }
}

/* An ISA extension in the co-processor and main instruction set space.  */
struct aarch64_option_extension
{
  const char *const name;
  const uint64_t flag_canonical;
  const uint64_t flags_on;
  const uint64_t flags_off;
  const bool is_synthetic;
};

/* ISA extensions in AArch64.  */
static const struct aarch64_option_extension all_extensions[] =
{
#define AARCH64_OPT_EXTENSION(NAME, FLAG_CANONICAL, FLAGS_ON, FLAGS_OFF, \
			      SYNTHETIC, Z) \
  {NAME, FLAG_CANONICAL, FLAGS_ON, FLAGS_OFF, SYNTHETIC},
#include "config/aarch64/aarch64-option-extensions.def"
  {NULL, 0, 0, 0, false}
};

/* A copy of the ISA extensions list for AArch64 sorted by the popcount of
   bits and extension turned on.  Cached for efficiency.  */
static struct aarch64_option_extension all_extensions_by_on[] =
{
#define AARCH64_OPT_EXTENSION(NAME, FLAG_CANONICAL, FLAGS_ON, FLAGS_OFF, \
			      SYNTHETIC, Z) \
  {NAME, FLAG_CANONICAL, FLAGS_ON, FLAGS_OFF, SYNTHETIC},
#include "config/aarch64/aarch64-option-extensions.def"
  {NULL, 0, 0, 0, false}
};

struct processor_name_to_arch
{
  const std::string processor_name;
  const enum aarch64_arch arch;
  const uint64_t flags;
};

struct arch_to_arch_name
{
  const enum aarch64_arch arch;
  const std::string arch_name;
  const uint64_t flags;
};

/* Map processor names to the architecture revision they implement and
   the default set of architectural feature flags they support.  */
static const struct processor_name_to_arch all_cores[] =
{
#define AARCH64_CORE(NAME, X, IDENT, ARCH_IDENT, FLAGS, COSTS, IMP, PART, VARIANT) \
  {NAME, AARCH64_ARCH_##ARCH_IDENT, FLAGS},
#include "config/aarch64/aarch64-cores.def"
  {"generic", AARCH64_ARCH_8A, AARCH64_FL_FOR_ARCH8},
  {"", aarch64_no_arch, 0}
};

/* Map architecture revisions to their string representation.  */
static const struct arch_to_arch_name all_architectures[] =
{
#define AARCH64_ARCH(NAME, CORE, ARCH_IDENT, ARCH, FLAGS) \
  {AARCH64_ARCH_##ARCH_IDENT, NAME, FLAGS},
#include "config/aarch64/aarch64-arches.def"
  {aarch64_no_arch, "", 0}
};

/* Parse the architecture extension string STR and update ISA_FLAGS
   with the architecture features turned on or off.  Return a
   aarch64_parse_opt_result describing the result.
   When the STR string contains an invalid extension,
   a copy of the string is created and stored to INVALID_EXTENSION.  */

enum aarch64_parse_opt_result
aarch64_parse_extension (const char *str, uint64_t *isa_flags,
			 std::string *invalid_extension)
{
  /* The extension string is parsed left to right.  */
  const struct aarch64_option_extension *opt = NULL;

  /* Flag to say whether we are adding or removing an extension.  */
  int adding_ext = -1;

  while (str != NULL && *str != 0)
    {
      const char *ext;
      size_t len;

      str++;
      ext = strchr (str, '+');

      if (ext != NULL)
	len = ext - str;
      else
	len = strlen (str);

      if (len >= 2 && strncmp (str, "no", 2) == 0)
	{
	  adding_ext = 0;
	  len -= 2;
	  str += 2;
	}
      else if (len > 0)
	adding_ext = 1;

      if (len == 0)
	return AARCH64_PARSE_MISSING_ARG;


      /* Scan over the extensions table trying to find an exact match.  */
      for (opt = all_extensions; opt->name != NULL; opt++)
	{
	  if (strlen (opt->name) == len && strncmp (opt->name, str, len) == 0)
	    {
	      /* Add or remove the extension.  */
	      if (adding_ext)
		*isa_flags |= (opt->flags_on | opt->flag_canonical);
	      else
		*isa_flags &= ~(opt->flags_off | opt->flag_canonical);
	      break;
	    }
	}

      if (opt->name == NULL)
	{
	  /* Extension not found in list.  */
	  if (invalid_extension)
	    *invalid_extension = std::string (str, len);
	  return AARCH64_PARSE_INVALID_FEATURE;
	}

      str = ext;
    };

  return AARCH64_PARSE_OK;
}

/* Append all architecture extension candidates to the CANDIDATES vector.  */

void
aarch64_get_all_extension_candidates (auto_vec<const char *> *candidates)
{
  const struct aarch64_option_extension *opt;
  for (opt = all_extensions; opt->name != NULL; opt++)
    candidates->safe_push (opt->name);
}

/* Comparer to sort aarch64's feature extensions by population count. Largest
   first.  */

typedef const struct aarch64_option_extension opt_ext;

int opt_ext_cmp (const void* a, const void* b)
{
  opt_ext *opt_a = (opt_ext *)a;
  opt_ext *opt_b = (opt_ext *)b;

  /* We consider the total set of bits an options turns on to be the union of
     the singleton set containing the option itself and the set of options it
     turns on as a dependency.  As an example +dotprod turns on FL_DOTPROD and
     FL_SIMD.  As such the set of bits represented by this option is
     {FL_DOTPROD, FL_SIMD}. */
  uint64_t total_flags_a = opt_a->flag_canonical & opt_a->flags_on;
  uint64_t total_flags_b = opt_b->flag_canonical & opt_b->flags_on;
  int popcnt_a = popcount_hwi ((HOST_WIDE_INT)total_flags_a);
  int popcnt_b = popcount_hwi ((HOST_WIDE_INT)total_flags_b);
  int order = popcnt_b - popcnt_a;

  /* If they have the same amount of bits set, give it a more
     deterministic ordering by using the value of the bits themselves.  */
  if (order != 0)
    return order;

  if (total_flags_a != total_flags_b)
    return total_flags_a < total_flags_b ? 1 : -1;

  return 0;
}

/* Implement TARGET_OPTION_INIT_STRUCT.  */

static void
aarch64_option_init_struct (struct gcc_options *opts ATTRIBUTE_UNUSED)
{
    /* Sort the extensions based on how many bits they set, order the larger
       counts first.  We sort the list because this makes processing the
       feature bits O(n) instead of O(n^2).  While n is small, the function
       to calculate the feature strings is called on every options push,
       pop and attribute change (arm_neon headers, lto etc all cause this to
       happen quite frequently).  It is a trade-off between time and space and
       so time won.  */
    int n_extensions
      = sizeof (all_extensions) / sizeof (struct aarch64_option_extension);
    qsort (&all_extensions_by_on, n_extensions,
	   sizeof (struct aarch64_option_extension), opt_ext_cmp);
}

/* Checks to see if enough bits from the option OPT are enabled in
   ISA_FLAG_BITS to be able to replace the individual options with the
   canonicalized version of the option.  This is done based on two rules:

   1) Synthetic groups, such as +crypto we only care about the bits that are
      turned on. e.g. +aes+sha2 can be replaced with +crypto.

   2) Options that themselves have a bit, such as +rdma, in this case, all the
      feature bits they turn on must be available and the bit for the option
      itself must be.  In this case it's effectively a reduction rather than a
      grouping. e.g. +fp+simd is not enough to turn on +rdma, for that you would
      need +rdma+fp+simd which is reduced down to +rdma.
*/

static bool
aarch64_contains_opt (uint64_t isa_flag_bits, opt_ext *opt)
{
  uint64_t flags_check
    = opt->is_synthetic ? opt->flags_on : opt->flag_canonical;

  return (isa_flag_bits & flags_check) == flags_check;
}

/* Return a string representation of ISA_FLAGS.  DEFAULT_ARCH_FLAGS
   gives the default set of flags which are implied by whatever -march
   we'd put out.  Our job is to figure out the minimal set of "+" and
   "+no" feature flags to put out, and to put them out grouped such
   that all the "+" flags come before the "+no" flags.  */

std::string
aarch64_get_extension_string_for_isa_flags (uint64_t isa_flags,
					    uint64_t default_arch_flags)
{
  const struct aarch64_option_extension *opt = NULL;
  std::string outstr = "";

  uint64_t isa_flag_bits = isa_flags;

  /* Pass one: Minimize the search space by reducing the set of options
     to the smallest set that still turns on the same features as before in
     conjunction with the bits that are turned on by default for the selected
     architecture.  */
  for (opt = all_extensions_by_on; opt->name != NULL; opt++)
    {
      /* If the bit is on by default, then all the options it turns on are also
	 on by default due to the transitive dependencies.

         If the option is enabled explicitly in the set then we need to emit
	 an option for it.  Since this list is sorted by extensions setting the
	 largest number of featers first, we can be sure that nothing else will
	 ever need to set the bits we already set.  Consider the following
	 situation:

	  Feat1 = A + B + C
	  Feat2 = A + B
	  Feat3 = A + D
	  Feat4 = B + C
	  Feat5 = C

	The following results are expected:

	  A + C = A + Feat5
	  B + C = Feat4
	  Feat4 + A = Feat1
	  Feat2 + Feat5 = Feat1
	  Feat1 + C = Feat1
          Feat3 + Feat4 = Feat1 + D

	This search assumes that all invidual feature bits are use visible,
	in other words the user must be able to do +A, +B, +C and +D.  */
      if (aarch64_contains_opt (isa_flag_bits | default_arch_flags, opt))
      {
	/* We remove all the dependent bits, to prevent them from being turned
	   on twice.  This only works because we assume that all there are
	   individual options to set all bits standalone.  */
	isa_flag_bits &= ~opt->flags_on;
	isa_flag_bits |= opt->flag_canonical;
      }
    }

   /* By toggling bits on and off, we may have set bits on that are already
      enabled by default.  So we mask the default set out so we don't emit an
      option for them.  Instead of checking for this each time during Pass One
      we just mask all default bits away at the end.  */
   isa_flag_bits &= ~default_arch_flags;

   /* We now have the smallest set of features we need to process.  A subsequent
      linear scan of the bits in isa_flag_bits will allow us to print the ext
      names.  However as a special case if CRC was enabled before, always print
      it.  This is required because some CPUs have an incorrect specification
      in older assemblers.  Even though CRC should be the default for these
      cases the -mcpu values won't turn it on.  */
  if (isa_flags & AARCH64_ISA_CRC)
    isa_flag_bits |= AARCH64_ISA_CRC;

  /* Pass Two:
     Print the option names that we're sure we must turn on.  These are only
     optional extension names.  Mandatory ones have already been removed and
     ones we explicitly want off have been too.  */
  for (opt = all_extensions_by_on; opt->name != NULL; opt++)
    {
      if (isa_flag_bits & opt->flag_canonical)
	{
	  outstr += "+";
	  outstr += opt->name;
	}
    }

  /* Pass Three:
     Print out a +no for any mandatory extension that we are
     turning off.  By this point aarch64_parse_extension would have ensured
     that any optional extensions are turned off.  The only things left are
     things that can't be turned off usually, e.g. something that is on by
     default because it's mandatory and we want it off.  For turning off bits
     we don't guarantee the smallest set of flags, but instead just emit all
     options the user has specified.

     The assembler requires all +<opts> to be printed before +no<opts>.  */
  for (opt = all_extensions_by_on; opt->name != NULL; opt++)
    {
      if ((~isa_flags) & opt->flag_canonical
		&& !((~default_arch_flags) & opt->flag_canonical))
	{
	  outstr += "+no";
	  outstr += opt->name;
	}
    }

  return outstr;
}

/* Attempt to rewrite NAME, which has been passed on the command line
   as a -mcpu option to an equivalent -march value.  If we can do so,
   return the new string, otherwise return an error.  */

const char *
aarch64_rewrite_selected_cpu (const char *name)
{
  std::string original_string (name);
  std::string extension_str;
  std::string processor;
  size_t extension_pos = original_string.find_first_of ('+');

  /* Strip and save the extension string.  */
  if (extension_pos != std::string::npos)
    {
      processor = original_string.substr (0, extension_pos);
      extension_str = original_string.substr (extension_pos,
					      std::string::npos);
    }
  else
    {
      /* No extensions.  */
      processor = original_string;
    }

  const struct processor_name_to_arch* p_to_a;
  for (p_to_a = all_cores;
       p_to_a->arch != aarch64_no_arch;
       p_to_a++)
    {
      if (p_to_a->processor_name == processor)
	break;
    }

  const struct arch_to_arch_name* a_to_an;
  for (a_to_an = all_architectures;
       a_to_an->arch != aarch64_no_arch;
       a_to_an++)
    {
      if (a_to_an->arch == p_to_a->arch)
	break;
    }

  /* We couldn't find that proceesor name, or the processor name we
     found does not map to an architecture we understand.  */
  if (p_to_a->arch == aarch64_no_arch
      || a_to_an->arch == aarch64_no_arch)
    fatal_error (input_location, "unknown value %qs for %<-mcpu%>", name);

  uint64_t extensions = p_to_a->flags;
  aarch64_parse_extension (extension_str.c_str (), &extensions, NULL);

  std::string outstr = a_to_an->arch_name
	+ aarch64_get_extension_string_for_isa_flags (extensions,
						      a_to_an->flags);

  /* We are going to memory leak here, nobody elsewhere
     in the callchain is going to clean up after us.  The alternative is
     to allocate a static buffer, and assert that it is big enough for our
     modified string, which seems much worse!  */
  return xstrdup (outstr.c_str ());
}

/* Called by the driver to rewrite a name passed to the -mcpu
   argument in preparation to be passed to the assembler.  The
   names passed from the commend line will be in ARGV, we want
   to use the right-most argument, which should be in
   ARGV[ARGC - 1].  ARGC should always be greater than 0.  */

const char *
aarch64_rewrite_mcpu (int argc, const char **argv)
{
  gcc_assert (argc);
  return aarch64_rewrite_selected_cpu (argv[argc - 1]);
}

struct gcc_targetm_common targetm_common = TARGETM_COMMON_INITIALIZER;

#undef AARCH64_CPU_NAME_LENGTH

