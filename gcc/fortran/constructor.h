/* Array and structure constructors
   Copyright (C) 2009-2023 Free Software Foundation, Inc.

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

#ifndef GFC_CONSTRUCTOR_H
#define GFC_CONSTRUCTOR_H

/* Get a new constructor structure.  */
gfc_constructor *gfc_constructor_get (void);

/* Copy a constructor structure.  */
gfc_constructor_base gfc_constructor_copy (gfc_constructor_base base);


/* Free a gfc_constructor structure.  */
void gfc_constructor_free (gfc_constructor_base base);


/* Given an constructor structure, append the expression node onto
   the constructor. Returns the constructor node appended.  */
gfc_constructor *gfc_constructor_append (gfc_constructor_base *base,
					 gfc_constructor *c);

gfc_constructor *gfc_constructor_append_expr (gfc_constructor_base *base,
					      gfc_expr *e, locus *where);


/* Given an constructor structure, place the expression node at position.
   Returns the constructor node inserted.  */
gfc_constructor *gfc_constructor_insert (gfc_constructor_base *base,
					 gfc_constructor *c, int n);

gfc_constructor *gfc_constructor_insert_expr (gfc_constructor_base *base,
					      gfc_expr *e, locus *where,
					      int n);

/* Given an array constructor expression and an element number (starting
   at zero), return a pointer to the array element.  NULL is returned if
   the size of the array has been exceeded. The expression node returned
   remains a part of the array and should not be freed.  */

gfc_constructor *gfc_constructor_lookup (gfc_constructor_base base, int n);

/* Convenience function. Same as ...
     gfc_constructor *c = gfc_constructor_lookup (base, n);
     gfc_expr *e = c ? c->expr : NULL;
*/
gfc_expr *gfc_constructor_lookup_expr (gfc_constructor_base base, int n);

/* Get the first constructor node in the constructure structure.
   Returns NULL if there is no such expression.  */
gfc_constructor *gfc_constructor_first (gfc_constructor_base base);

/* Get the next constructor node in the constructure structure.
   Returns NULL if there is no next expression.  */
gfc_constructor *gfc_constructor_next (gfc_constructor *ctor);

/* Remove the gfc_constructor node from the splay tree.  */
void gfc_constructor_remove (gfc_constructor *);

/* Return first constructor node after offset.  */
gfc_constructor *gfc_constructor_lookup_next (gfc_constructor_base, int);

#endif /* GFC_CONSTRUCTOR_H */
