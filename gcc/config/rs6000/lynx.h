/* Definitions for Rs6000 running LynxOS.
   Copyright (C) 1995-2023 Free Software Foundation, Inc.
   Contributed by David Henkel-Wallace, Cygnus Support (gumby@cygnus.com)
   Rewritten by Adam Nemet, LynuxWorks Inc.

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

/* Undefine the definition to enable the LynxOS default from the
   top-level lynx.h.  */

#undef SUBTARGET_EXTRA_SPECS

/* Get rid off the spec definitions from rs6000/sysv4.h.  */

#undef CPP_SPEC
#define CPP_SPEC \
"%{msoft-float: -D_SOFT_FLOAT} \
 %(cpp_cpu) \
 %(cpp_os_lynx)"

/* LynxOS only supports big-endian on PPC so we override the
   definition from sysv4.h.  Since the LynxOS 4.0 compiler was set to
   return every structure in memory regardless of their size we have
   to emulate the same behavior here with disabling the SVR4 structure
   returning.  */

#undef CC1_SPEC
#define CC1_SPEC \
"%{G*} %{mno-sdata:-msdata=none} \
 %{maltivec:-mabi=altivec} \
 -maix-struct-return"

#undef ASM_SPEC
#define ASM_SPEC \
"%(asm_cpu) \
 %{,assembler|,assembler-with-cpp: %{mregnames} %{mno-regnames}}"

#undef STARTFILE_SPEC
#undef ENDFILE_SPEC
#undef LIB_SPEC
#undef LINK_SPEC
#define LINK_SPEC \
"%{!msdata=none:%{G*}} %{msdata=none:-G0} \
 %(link_os_lynx)"

/* Override the definition from sysv4.h.  */

#undef TARGET_OS_CPP_BUILTINS
#define TARGET_OS_CPP_BUILTINS()		\
  do						\
    {						\
      builtin_define ("__BIG_ENDIAN__");	\
      builtin_define ("__powerpc__");		\
      builtin_assert ("cpu=powerpc");		\
      builtin_assert ("machine=powerpc");	\
      builtin_define ("__PPC__");		\
    }						\
  while (0)

/* LynxOS does not do anything with .fixup plus let's not create
   writable section for linkonce.r and linkonce.t.  */

#undef RELOCATABLE_NEEDS_FIXUP

/* Override these from rs6000.h with the generic definition.  */

#undef SIZE_TYPE
#undef ASM_OUTPUT_ALIGN

/* The file rs6000.cc defines TARGET_HAVE_TLS unconditionally to the
   value of HAVE_AS_TLS.  HAVE_AS_TLS is true as gas support for TLS
   is detected by configure.  Override the definition to false.  */

#undef HAVE_AS_TLS
#define HAVE_AS_TLS 0

/* Use standard DWARF numbering for DWARF debugging information.  */
#define RS6000_USE_DWARF_NUMBERING

#ifdef CRT_BEGIN
/* This function is part of crtbegin*.o which is at the beginning of
   the link and is called from .fini which is usually toward the end
   of the executable.  Make it longcall so that we don't limit the
   text size of the executables to 32M.  */

static void __do_global_dtors_aux (void) __attribute__ ((longcall));
#endif	/* CRT_BEGIN */

#ifdef CRT_END
/* Similarly here.  This function resides in crtend*.o which is toward
   to end of the link and is called from .init which is at the
   beginning.  */

static void __do_global_ctors_aux (void) __attribute__ ((longcall));
#endif	/* CRT_END */
