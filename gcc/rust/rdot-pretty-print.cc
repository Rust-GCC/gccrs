/* This file is part of GCC.

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
<http://www.gnu.org/licenses/>. */

#include "rust.h"

static void dot_pass_dump_node (FILE *, rdot, size_t);

static
void dot_pass_dump_node (FILE * fd, rdot decl, size_t indents)
{
    return;
}

vec<rdot,va_gc> * dot_pass_PrettyPrint (vec<rdot,va_gc> * decls)
{
    if (GRS_OPT_dump_dot)
    {
	const char * dump_file = "gccrs.rdot";
	FILE * fd = fopen (dump_file, "rb");
	if (!fd)
	    error ("Unable to open %s for write\n", dump_file);

	rdot idtx = NULL_DOT;
	size_t i;
	for (i = 0; decls->iterate (i, &idtx); ++i)
	{
	    dot_pass_dump_node (fd, idtx, 0);
	    fprintf (fd, "\n");
	}

	fclose (fd);
    }
    return decls;
}
