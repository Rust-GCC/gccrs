/* Support routines for Value Range Propagation (VRP).
   Copyright (C) 2016-2023 Free Software Foundation, Inc.

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

#ifndef GCC_TREE_VRP_H
#define GCC_TREE_VRP_H

#include "value-range.h"

extern bool range_int_cst_p (const value_range *);

extern int compare_values (tree, tree);
extern int compare_values_warnv (tree, tree, bool *);
extern int operand_less_p (tree, tree);

void range_fold_unary_expr (value_range *, enum tree_code, tree type,
			    const value_range *, tree op0_type);
void range_fold_binary_expr (value_range *, enum tree_code, tree type,
			     const value_range *, const value_range *);

extern enum value_range_kind intersect_range_with_nonzero_bits
  (enum value_range_kind, wide_int *, wide_int *, const wide_int &, signop);

extern bool find_case_label_range (gswitch *, tree, tree, size_t *, size_t *);
extern tree find_case_label_range (gswitch *, const irange *vr);
extern bool find_case_label_index (gswitch *, size_t, tree, size_t *);
extern bool overflow_comparison_p (tree_code, tree, tree, tree *);
extern void maybe_set_nonzero_bits (edge, tree);

#endif /* GCC_TREE_VRP_H */
