/* m2rts.h provides a C interface to M2RTS.mod.

Copyright (C) 2019-2022 Free Software Foundation, Inc.
Contributed by Gaius Mulley <gaius.mulley@southwales.ac.uk>.

This file is part of GNU Modula-2.

GNU Modula-2 is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GNU Modula-2 is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

Under Section 7 of GPL version 3, you are granted additional
permissions described in the GCC Runtime Library Exception, version
3.1, as published by the Free Software Foundation.

You should have received a copy of the GNU General Public License and
a copy of the GCC Runtime Library Exception along with this program;
see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
<http://www.gnu.org/licenses/>.  */


typedef void (*proc_con) (int, char **, char **);
typedef void (*proc_dep) (void);

extern "C" void M2RTS_RequestDependant (const char *modulename, const char *dependancy);
extern "C" void M2RTS_RegisterModule (const char *modulename,
				      proc_con init, proc_con fini, proc_dep dependencies);
extern "C" void _M2_M2RTS_init (void);

extern "C" void M2RTS_ConstructModules (const char *,
					int argc, char *argv[], char *envp[]);
extern "C" void M2RTS_Terminate (void);
extern "C" void M2RTS_DeconstructModules (void);

extern "C" void M2RTS_HaltC (const char *filename, int line,
			     const char *functionname, const char *desc)
                             __attribute__ ((noreturn));
