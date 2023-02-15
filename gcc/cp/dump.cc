/* Tree-dumping functionality for intermediate representation.
   Copyright (C) 1999-2023 Free Software Foundation, Inc.
   Written by Mark Mitchell <mark@codesourcery.com>

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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "cp-tree.h"
#include "tree-dump.h"

/* Dump a representation of the accessibility information associated
   with T.  */

static void
dump_access (dump_info_p di, tree t)
{
  if (TREE_PROTECTED(t))
    dump_string_field (di, "accs", "prot");
  else if (TREE_PRIVATE(t))
    dump_string_field (di, "accs", "priv");
  else
    dump_string_field (di, "accs", "pub");
}

/* Dump information common to statements from STMT.  */

static void
dump_stmt (dump_info_p di, const_tree t)
{
  if (EXPR_HAS_LOCATION (t))
    dump_int (di, "line", EXPR_LINENO (t));
}

bool
cp_dump_tree (void* dump_info, tree t)
{
  enum tree_code code;
  dump_info_p di = (dump_info_p) dump_info;

  /* Figure out what kind of node this is.  */
  code = TREE_CODE (t);

  if (DECL_P (t))
    {
      if (DECL_LANG_SPECIFIC (t) && DECL_LANGUAGE (t) != lang_cplusplus)
	dump_string_field (di, "lang", language_to_string (DECL_LANGUAGE (t)));
    }

  switch (code)
    {
    case IDENTIFIER_NODE:
      if (IDENTIFIER_ANY_OP_P (t))
	{
	  dump_string_field (di, "note", "operator");
	  return true;
	}
      else if (IDENTIFIER_CONV_OP_P (t))
	{
	  dump_child ("tynm", TREE_TYPE (t));
	  return true;
	}
      break;

    case OFFSET_TYPE:
      dump_string_field (di, "note", "ptrmem");
      dump_child ("ptd", TYPE_PTRMEM_POINTED_TO_TYPE (t));
      dump_child ("cls", TYPE_PTRMEM_CLASS_TYPE (t));
      return true;

    case RECORD_TYPE:
      if (TYPE_PTRMEMFUNC_P (t))
	{
	  dump_string_field (di, "note", "ptrmem");
	  dump_child ("ptd", TYPE_PTRMEM_POINTED_TO_TYPE (t));
	  dump_child ("cls", TYPE_PTRMEM_CLASS_TYPE (t));
	  return true;
	}
      /* Fall through.  */

    case UNION_TYPE:
      /* Is it a type used as a base? */
      if (TYPE_CONTEXT (t) && TREE_CODE (TYPE_CONTEXT (t)) == TREE_CODE (t)
	  && CLASSTYPE_AS_BASE (TYPE_CONTEXT (t)) == t)
	{
	  dump_child ("bfld", TYPE_CONTEXT (t));
	  return true;
	}

      if (! MAYBE_CLASS_TYPE_P (t))
	break;

      dump_child ("vfld", TYPE_VFIELD (t));
      if (CLASSTYPE_TEMPLATE_SPECIALIZATION(t))
	dump_string(di, "spec");

      if (!dump_flag (di, TDF_SLIM, t) && TYPE_BINFO (t))
	{
	  int i;
	  tree binfo;
	  tree base_binfo;

	  for (binfo = TYPE_BINFO (t), i = 0;
	       BINFO_BASE_ITERATE (binfo, i, base_binfo); ++i)
	    {
	      dump_child ("base", BINFO_TYPE (base_binfo));
	      if (BINFO_VIRTUAL_P (base_binfo))
		dump_string_field (di, "spec", "virt");
	      dump_access (di, base_binfo);
	    }
	}
      break;

    case FIELD_DECL:
      dump_access (di, t);
      if (DECL_MUTABLE_P (t))
	dump_string_field (di, "spec", "mutable");
      break;

    case VAR_DECL:
      if (TREE_CODE (CP_DECL_CONTEXT (t)) == RECORD_TYPE)
	dump_access (di, t);
      if (TREE_STATIC (t) && !TREE_PUBLIC (t))
	dump_string_field (di, "link", "static");
      break;

    case FUNCTION_DECL:
      if (!DECL_THUNK_P (t))
	{
	  if (DECL_OVERLOADED_OPERATOR_P (t))
	    dump_string_field (di, "note", "operator");
	  if (DECL_FUNCTION_MEMBER_P (t))
	    {
	      dump_string_field (di, "note", "member");
	      dump_access (di, t);
	    }
	  if (DECL_PURE_VIRTUAL_P (t))
	    dump_string_field (di, "spec", "pure");
	  if (DECL_VIRTUAL_P (t))
	    dump_string_field (di, "spec", "virt");
	  if (DECL_CONSTRUCTOR_P (t))
	    dump_string_field (di, "note", "constructor");
	  if (DECL_DESTRUCTOR_P (t))
	    dump_string_field (di, "note", "destructor");
	  if (DECL_CONV_FN_P (t))
	    dump_string_field (di, "note", "conversion");
	  if (DECL_GLOBAL_CTOR_P (t))
	    dump_string_field (di, "note", "global init");
	  if (DECL_GLOBAL_DTOR_P (t))
	    dump_string_field (di, "note", "global fini");
	  if (DECL_FRIEND_PSEUDO_TEMPLATE_INSTANTIATION (t))
	    dump_string_field (di, "note", "pseudo tmpl");
	}
      else
	{
	  tree virt = THUNK_VIRTUAL_OFFSET (t);

	  dump_string_field (di, "note", "thunk");
	  if (DECL_THIS_THUNK_P (t))
	    dump_string_field (di, "note", "this adjusting");
	  else
	    {
	      dump_string_field (di, "note", "result adjusting");
	      if (virt)
		virt = BINFO_VPTR_FIELD (virt);
	    }
	  dump_int (di, "fixd", THUNK_FIXED_OFFSET (t));
	  if (virt)
	    dump_int (di, "virt", tree_to_shwi (virt));
	  dump_child ("fn", DECL_INITIAL (t));
	}
      break;

    case NAMESPACE_DECL:
      if (DECL_NAMESPACE_ALIAS (t))
	dump_child ("alis", DECL_NAMESPACE_ALIAS (t));
      else if (!dump_flag (di, TDF_SLIM, t))
	dump_child ("dcls", cp_namespace_decls (t));
      break;

    case TEMPLATE_DECL:
      dump_child ("rslt", DECL_TEMPLATE_RESULT (t));
      dump_child ("inst", DECL_TEMPLATE_INSTANTIATIONS (t));
      dump_child ("spcs", DECL_TEMPLATE_SPECIALIZATIONS (t));
      dump_child ("prms", DECL_TEMPLATE_PARMS (t));
      break;

    case OVERLOAD:
      dump_child ("name", OVL_NAME (t));
      if (!dump_flag (di, TDF_SLIM, t))
	for (lkp_iterator iter (t); iter; ++iter)
	  dump_child ("chld", *iter);
      break;

    case TRY_BLOCK:
      dump_stmt (di, t);
      if (CLEANUP_P (t))
	dump_string_field (di, "note", "cleanup");
      dump_child ("body", TRY_STMTS (t));
      dump_child ("hdlr", TRY_HANDLERS (t));
      break;

    case EH_SPEC_BLOCK:
      dump_stmt (di, t);
      dump_child ("body", EH_SPEC_STMTS (t));
      dump_child ("raises", EH_SPEC_RAISES (t));
      break;

    case PTRMEM_CST:
      dump_child ("clas", PTRMEM_CST_CLASS (t));
      dump_child ("mbr", PTRMEM_CST_MEMBER (t));
      break;

    case THROW_EXPR:
      /* These nodes are unary, but do not have code class `1'.  */
      dump_child ("op 0", TREE_OPERAND (t, 0));
      break;

    case AGGR_INIT_EXPR:
      {
	int i = 0;
	tree arg;
	aggr_init_expr_arg_iterator iter;
	dump_int (di, "ctor", AGGR_INIT_VIA_CTOR_P (t));
	dump_child ("fn", AGGR_INIT_EXPR_FN (t));
	FOR_EACH_AGGR_INIT_EXPR_ARG (arg, iter, t)
	  {
	    char buffer[32];
	    sprintf (buffer, "%u", i);
	    dump_child (buffer, arg);
	    i++;
	  }
	dump_child ("decl", AGGR_INIT_EXPR_SLOT (t));
      }
      break;

    case HANDLER:
      dump_stmt (di, t);
      dump_child ("parm", HANDLER_PARMS (t));
      dump_child ("body", HANDLER_BODY (t));
      break;

    case MUST_NOT_THROW_EXPR:
      dump_stmt (di, t);
      dump_child ("body", TREE_OPERAND (t, 0));
      dump_child ("cond", MUST_NOT_THROW_COND (t));
      break;

    case USING_STMT:
      dump_stmt (di, t);
      dump_child ("nmsp", USING_STMT_NAMESPACE (t));
      break;

    case CLEANUP_STMT:
      dump_stmt (di, t);
      dump_child ("decl", CLEANUP_DECL (t));
      dump_child ("expr", CLEANUP_EXPR (t));
      dump_child ("body", CLEANUP_BODY (t));
      break;

    case IF_STMT:
      dump_stmt (di, t);
      dump_child ("cond", IF_COND (t));
      dump_child ("then", THEN_CLAUSE (t));
      dump_child ("else", ELSE_CLAUSE (t));
      break;

    case RANGE_FOR_STMT:
      dump_stmt (di, t);
      dump_child ("init", RANGE_FOR_INIT_STMT (t));
      dump_child ("decl", RANGE_FOR_DECL (t));
      dump_child ("expr", RANGE_FOR_EXPR (t));
      dump_child ("body", RANGE_FOR_BODY (t));
      break;

    case STMT_EXPR:
      dump_child ("stmt", STMT_EXPR_STMT (t));
      break;

    case EXPR_STMT:
      dump_stmt (di, t);
      dump_child ("expr", EXPR_STMT_EXPR (t));
      break;

    case OMP_DEPOBJ:
      dump_stmt (di, t);
      dump_child ("depobj", OMP_DEPOBJ_DEPOBJ (t));
      dump_child ("clauses", OMP_DEPOBJ_CLAUSES (t));
      break;

    default:
      break;
    }

  return c_dump_tree (di, t);
}
