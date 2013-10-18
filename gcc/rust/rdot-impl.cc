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

rdot rdot_alloc (void)
{
    rdot retval = (struct grs_tree_dot *)
	xmalloc (sizeof (struct grs_tree_dot));
    gcc_assert (retval);
    memset (retval, 0, sizeof (struct grs_tree_dot));
    return retval;
}

rdot_tree_common * rdot_cm_alloc (void)
{
    rdot_tree_common * retval = (struct grs_rdot_tree_common *)
	xmalloc (sizeof (struct grs_rdot_tree_common *));
    gcc_assert (retval);
    memset (retval, 0, sizeof (struct grs_rdot_tree_common *));
    return retval;
}

rdot rdot_build_decl1 (opcode_t o, rdot t1)
{
  rdot decl = RDOT_alloc;

  RDOT_TYPE(decl) = o;
  RDOT_T_FIELD(decl) = D_TD_NULL;
  RDOT_FIELD(decl) = NULL_DOT;

  decl->opaT = D_TD_DOT;
  decl->opa.t = t1;
  decl->opbT = D_TD_NULL;

  RDOT_CHAIN(decl) = NULL_DOT;

  return decl;
}

rdot rdot_build_decl2 (opcode_t o,
				  rdot t1,
				  rdot t2)
{
  rdot decl = RDOT_alloc;

  RDOT_TYPE (decl) = o;
  if ((o == D_MODIFY_EXPR)
      || (o == D_ADD_EXPR)
      || (o == D_MINUS_EXPR)
      || (o == D_MULT_EXPR)
      || (o == D_DIVD_EXPR)
      || (o == D_CALL_EXPR)
      || (o == D_EQ_EQ_EXPR)
      || (o == D_LESS_EXPR)
      || (o == D_LESS_EQ_EXPR)
      || (o == D_GREATER_EXPR)
      || (o == D_GREATER_EQ_EXPR)
      || (o == D_NOT_EQ_EXPR)
      )
    RDOT_T_FIELD(decl) = D_D_EXPR;
  else
    RDOT_T_FIELD(decl) = D_TD_NULL;

  RDOT_FIELD (decl) = NULL_DOT;

  decl->opaT = D_TD_DOT;
  decl->opa.t = t1;
  decl->opbT = D_TD_DOT;
  decl->opb.t = t2;

  RDOT_CHAIN(decl) = NULL_DOT;

  return decl;
}

rdot rdot_build_integer (const int i)
{
  rdot decl = RDOT_alloc;
  RDOT_TYPE(decl) = D_PRIMITIVE;

  RDOT_FIELD(decl) = NULL_DOT;
  RDOT_T_FIELD(decl) = D_TD_NULL;

  decl->opaT = D_TD_COM;
  decl->opa.tc = RDOT_CM_alloc;

  decl->opa.tc->T = D_T_INTEGER;
  decl->opa.tc->o.integer = i;

  decl->opbT = D_TD_NULL;
  RDOT_CHAIN(decl) = NULL_DOT;

  return decl;
}

rdot rdot_build_string (const char * s)
{
  rdot decl = RDOT_alloc;
  RDOT_TYPE (decl) = D_PRIMITIVE;

  RDOT_FIELD (decl) = NULL_DOT;
  RDOT_T_FIELD (decl) = D_TD_NULL;

  decl->opaT = D_TD_COM;
  decl->opa.tc = RDOT_CM_alloc;

  decl->opa.tc->T = D_T_STRING;
  decl->opa.tc->o.string = xstrdup (s);

  decl->opbT = D_TD_NULL;
  RDOT_CHAIN (decl) = NULL_DOT;

  return decl;
}

rdot rdot_build_identifier (const char * s)
{
  rdot decl = RDOT_alloc;

  RDOT_TYPE(decl) = D_IDENTIFIER;
  RDOT_FIELD(decl) = NULL_DOT;
  RDOT_T_FIELD(decl) = D_TD_NULL;

  decl->opaT = D_TD_COM;
  decl->opa.tc = RDOT_CM_alloc;
  decl->opa.tc->T = D_T_STRING;
  decl->opa.tc->o.string = (char *) s;

  decl->opbT = D_TD_NULL;

  RDOT_CHAIN(decl) = NULL_DOT;

  return decl;
}
