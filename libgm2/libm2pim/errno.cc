/* errno.c provide access to the errno value.

Copyright (C) 2009-2022 Free Software Foundation, Inc.
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

#include <config.h>

#if defined(HAVE_SYS_ERRNO_H)
#include <sys/errno.h>
#endif

#if defined(HAVE_ERRNO_H)
#include <errno.h>
#endif

#include "m2rts.h"

extern "C" int
errno_geterrno (void)
{
#if defined(HAVE_ERRNO_H) || defined(HAVE_SYS_ERRNO_H)
  return errno;
#else
  return -1;
#endif
}

extern "C" void
_M2_errno_init (int, char *[], char *[])
{
}

extern "C" void
_M2_errno_fini (int, char *[], char *[])
{
}

extern "C" void
_M2_errno_dep (void)
{
}

extern "C" void __attribute__((__constructor__))
_M2_errno_ctor (void)
{
  M2RTS_RegisterModule ("errno", _M2_errno_init, _M2_errno_fini,
			_M2_errno_dep);
}
