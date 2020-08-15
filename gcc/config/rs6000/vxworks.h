/* Definitions of target machine for GNU compiler.  Vxworks PowerPC version.
   Copyright (C) 1996-2019 Free Software Foundation, Inc.
   Contributed by CodeSourcery, LLC.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

/* Note to future editors: VxWorks is mostly an EABI target.  We do
   not use rs6000/eabi.h because we would have to override most of
   it anyway.  However, if you change that file, consider making
   analogous changes here too.  */

/* CPP predefined macros.  */

#undef TARGET_OS_CPP_BUILTINS
#define TARGET_OS_CPP_BUILTINS()		\
  do						\
    {						\
      builtin_define ("__ppc");			\
      builtin_define ("__PPC__");		\
      builtin_define ("__EABI__");		\
      builtin_define ("__ELF__");		\
      if (!TARGET_SOFT_FLOAT)			\
	builtin_define ("__hardfp");		\
						\
      /* C89 namespace violation! */		\
      builtin_define ("CPU_FAMILY=PPC");	\
        					\
      VXWORKS_OS_CPP_BUILTINS ();		\
    }		\
  while (0)

#ifdef TARGET_RUST_OS_INFO
# error "TARGET_RUST_OS_INFO already defined in vxworks.h (rs6000) - c++ undefines it and redefines it."
#endif
#define TARGET_RUST_OS_INFO()			\
  do {			\
    VXWORKS_TARGET_RUST_OS_INFO ();			\
  } while (0)

/* Only big endian PPC is supported by VxWorks.  */
#undef BYTES_BIG_ENDIAN
#define BYTES_BIG_ENDIAN 1
#undef WORDS_BIG_ENDIAN
#define WORDS_BIG_ENDIAN 1

/* We have to kill off the entire specs set created by rs6000/sysv4.h
   and substitute our own set.  The top level vxworks.h has done some
   of this for us.  */

#undef SUBTARGET_EXTRA_SPECS
#undef CPP_SPEC
#undef CC1_SPEC
#undef ASM_SPEC

#define SUBTARGET_EXTRA_SPECS /* none needed */

/* FIXME: The only reason we allow no -mcpu switch at all is because
   config-ml.in insists on a "." multilib. */
#define CPP_SPEC \
"%{!DCPU=*:		  \
   %{mcpu=403 : -DCPU=PPC403  ; \
     mcpu=405 : -DCPU=PPC405  ; \
     mcpu=440 : -DCPU=PPC440  ; \
     mcpu=464 : -DCPU=PPC464  ; \
     mcpu=476 : -DCPU=PPC476  ; \
     mcpu=603 : -DCPU=PPC603  ; \
     mcpu=604 : -DCPU=PPC604  ; \
     mcpu=860 : -DCPU=PPC860  ; \
     mcpu=8540: -DCPU=PPC85XX ; \
     mcpu=8548: -DCPU=PPC85XX ; \
              : -DCPU=PPC604  }}" \
VXWORKS_ADDITIONAL_CPP_SPEC

#define CC1_SPEC						\
"%{G*} %{mno-sdata:-msdata=none} %{msdata:-msdata=default}	\
 %{mlittle|mlittle-endian:-mstrict-align}"

#define ASM_SPEC \
"%(asm_cpu) \
 %{,assembler|,assembler-with-cpp: %{mregnames} %{mno-regnames}} \
 %{mrelocatable} %{mrelocatable-lib} %{" FPIC_SPEC ":-K PIC} -mbig"

#undef  LIB_SPEC
#define LIB_SPEC VXWORKS_LIB_SPEC

/* For RTPs, leverage linker relaxation.  This helps programs referring
   to, typically, kernel services too far away for short calls.  This is more
   precise than -mlongcall and can be overriden with -Wl,--no-relax.  */
#define VXWORKS_RELAX_LINK_SPEC "%{mrtp:--relax}"

#undef  LINK_SPEC
#define LINK_SPEC VXWORKS_LINK_SPEC " " VXWORKS_RELAX_LINK_SPEC

#undef  STARTFILE_SPEC
#define STARTFILE_SPEC VXWORKS_STARTFILE_SPEC
#undef  ENDFILE_SPEC
#define ENDFILE_SPEC VXWORKS_ENDFILE_SPEC

/* There is no default multilib.  */
#undef MULTILIB_DEFAULTS

#undef TARGET_DEFAULT
#define TARGET_DEFAULT (MASK_EABI | MASK_STRICT_ALIGN)

#undef PROCESSOR_DEFAULT
#define PROCESSOR_DEFAULT PROCESSOR_PPC604

/* Nor sdata, for kernel mode.  We use this in
   SUBSUBTARGET_INITIALIZE_OPTIONS, after rs6000_rtp has been initialized.  */
#undef SDATA_DEFAULT_SIZE
#define SDATA_DEFAULT_SIZE (TARGET_VXWORKS_RTP ? 8 : 0)

/* Enforce 16-byte alignment for the stack pointer, to permit general
   compliance with e.g. Altivec instructions requirements.  Make sure
   this isn't overruled by the EABI constraints.  */

#undef  STACK_BOUNDARY
#define STACK_BOUNDARY (16*BITS_PER_UNIT)

#undef  PREFERRED_STACK_BOUNDARY
#define PREFERRED_STACK_BOUNDARY STACK_BOUNDARY

#undef  ABI_STACK_BOUNDARY

#undef RS6000_STARTING_FRAME_OFFSET
#define RS6000_STARTING_FRAME_OFFSET					\
  (cfun->calls_alloca							\
   ? RS6000_ALIGN (crtl->outgoing_args_size + RS6000_SAVE_AREA, 16)	\
   : (RS6000_ALIGN (crtl->outgoing_args_size, 16) + RS6000_SAVE_AREA))

#undef STACK_DYNAMIC_OFFSET
#define STACK_DYNAMIC_OFFSET(FUNDECL)					\
   RS6000_ALIGN (crtl->outgoing_args_size.to_constant ()		\
		 + STACK_POINTER_OFFSET, 16)

#undef SUBSUBTARGET_OVERRIDE_OPTIONS
#define SUBSUBTARGET_OVERRIDE_OPTIONS		\
  do {						\
  if (!global_options_set.x_g_switch_value)	\
    g_switch_value = SDATA_DEFAULT_SIZE;	\
  VXWORKS_OVERRIDE_OPTIONS;			\
  } while (0)

/* No _mcount profiling on VxWorks.  */
#undef FUNCTION_PROFILER
#define FUNCTION_PROFILER(FILE,LABELNO) VXWORKS_FUNCTION_PROFILER(FILE,LABELNO)

/* Define this to be nonzero if static stack checking is supported.  */
#define STACK_CHECK_STATIC_BUILTIN 1

/* This platform supports the probing method of stack checking (RTP mode).
   8K is reserved in the stack to propagate exceptions in case of overflow.  */
#define STACK_CHECK_PROTECT 8192
