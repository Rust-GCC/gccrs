/* Definitions of target machine for GNU compiler,
   for Alpha NetBSD systems.
   Copyright (C) 1998-2023 Free Software Foundation, Inc.

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

#define TARGET_OS_CPP_BUILTINS()		\
    do {					\
	NETBSD_OS_CPP_BUILTINS_ELF();		\
    } while (0)


/* NetBSD doesn't use the LANGUAGE* built-ins.  */
#undef SUBTARGET_LANGUAGE_CPP_BUILTINS
#define SUBTARGET_LANGUAGE_CPP_BUILTINS()	/* nothing */


/* Show that we need a GP when profiling.  */
#undef TARGET_PROFILING_NEEDS_GP
#define TARGET_PROFILING_NEEDS_GP 1


/* Provide a CPP_SPEC appropriate for NetBSD/alpha.  We use
   this to pull in CPP specs that all NetBSD configurations need.  */

#undef CPP_SPEC
#define CPP_SPEC NETBSD_CPP_SPEC

#undef EXTRA_SPECS
#define EXTRA_SPECS			\
  { "netbsd_link_spec", NETBSD_LINK_SPEC_ELF },	\
  { "netbsd_entry_point", NETBSD_ENTRY_POINT },	\
  { "netbsd_endfile_spec", NETBSD_ENDFILE_SPEC },


/* Provide a LINK_SPEC appropriate for a NetBSD/alpha ELF target.  */

#undef LINK_SPEC
#define LINK_SPEC \
  "%{G*} %{relax:-relax} \
   %{O*:-O3} %{!O*:-O1} \
   %(netbsd_link_spec)"

#define NETBSD_ENTRY_POINT "__start"


/* Provide an ENDFILE_SPEC appropriate for NetBSD/alpha ELF.  Here we
   add crtend.o, which provides part of the support for getting
   C++ file-scope static objects deconstructed after exiting "main".

   We also need to handle the GCC option `-ffast-math'.  */

#undef ENDFILE_SPEC
#define ENDFILE_SPEC		\
  "%{Ofast|ffast-math|funsafe-math-optimizations:crtfm%O%s} \
   %(netbsd_endfile_spec)"

#define HAVE_ENABLE_EXECUTE_STACK
