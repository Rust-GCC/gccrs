/* IPA handling of references.
   Copyright (C) 2004-2023 Free Software Foundation, Inc.
   Contributed by Kenneth Zadeck <zadeck@naturalbridge.com>

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

#ifndef GCC_IPA_REFERENCE_H
#define GCC_IPA_REFERENCE_H

/* In ipa-reference.cc  */
bitmap ipa_reference_get_read_global (struct cgraph_node *fn);
bitmap ipa_reference_get_written_global (struct cgraph_node *fn);
void ipa_reference_cc_finalize (void);
int ipa_reference_var_uid (tree t);

#endif  /* GCC_IPA_REFERENCE_H  */

