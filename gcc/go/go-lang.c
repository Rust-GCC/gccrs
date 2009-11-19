/* go-lang.c -- Go frontend gcc interface.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#include "config.h"
#include "system.h"
#include "ansidecl.h"
#include "coretypes.h"
#include "opts.h"
#include "tree.h"
#include "gimple.h"
#include "ggc.h"
#include "toplev.h"
#include "debug.h"
#include "options.h"
#include "flags.h"
#include "convert.h"
#include "diagnostic.h"
#include "langhooks.h"
#include "langhooks-def.h"
#include "target.h"

#include <mpfr.h>

#include "go-c.h"

/* Language-dependent contents of a type.  */

struct GTY(()) lang_type
{
  char dummy;
};

/* Language-dependent contents of a decl.  */

struct GTY(()) lang_decl
{
  char dummy;
};

/* Language-dependent contents of an identifier.  This must include a
   tree_identifier.  */

struct GTY(()) lang_identifier
{
  struct tree_identifier common;
};

/* The resulting tree type.  */

union GTY((desc ("TREE_CODE (&%h.generic) == IDENTIFIER_NODE"),
	   chain_next ("(union lang_tree_node *) TREE_CHAIN (&%h.generic)")))
lang_tree_node
{
  union tree_node GTY((tag ("0"),
		       desc ("tree_node_structure (&%h)"))) generic;
  struct lang_identifier GTY((tag ("1"))) identifier;
};

/* We don't use language_function.  */

struct GTY(()) language_function
{
  int dummy;
};

/* Language hooks.  */

static bool
go_langhook_init (void)
{
  build_common_tree_nodes (false, false);

  /* The sizetype may be "unsigned long" or "unsigned long long".  */
  if (TYPE_MODE (long_unsigned_type_node) == ptr_mode)
    size_type_node = long_unsigned_type_node;
  else if (TYPE_MODE (long_long_unsigned_type_node) == ptr_mode)
    size_type_node = long_long_unsigned_type_node;
  else
    size_type_node = long_unsigned_type_node;
  set_sizetype(size_type_node);

  build_common_tree_nodes_2 (0);

  /* We must create the gogo IR after calling build_common_tree_nodes
     (because Gogo::define_builtin_function_trees refers indirectly
     to, e.g., unsigned_char_type_node) but before calling
     build_common_builtin_nodes (because it calls, indirectly,
     go_type_for_size).  */
  go_create_gogo ();

  build_common_builtin_nodes ();

  /* I don't know why this is not done by any of the above.  */
  void_list_node = build_tree_list (NULL_TREE, void_type_node);

  return true;
}

/* Initialize before parsing options.  */

static unsigned int
go_langhook_init_options (unsigned int argc ATTRIBUTE_UNUSED,
			  const char** argv ATTRIBUTE_UNUSED)
{
  /* Go says that signed overflow is precisely defined.  */
  flag_wrapv = 1;

  /* We default to using strict aliasing, since Go pointers are safe.
     This is turned off for code that imports the "unsafe" package,
     because using unsafe.pointer violates C style aliasing
     requirements.  */
  flag_strict_aliasing = 1;

  /* The builtin math functions should not set errno.  */
  flag_errno_math = 0;

  /* We default to always showing a column number.  */
  flag_show_column = 1;

  /* The default precision for floating point numbers.  This is used
     for floating point constants with abstract type.  This may
     eventually be controllable by a command line option.  */
  mpfr_set_default_prec (128);

  /* We turn on stack splitting if we can.  */
  if (targetm.supports_split_stack())
    flag_split_stack = 1;

  return CL_Go;
}

/* Handle Go specific options.  Return 0 if we didn't do anything.  */

static int
go_langhook_handle_option (size_t scode, const char *arg,
			   int value ATTRIBUTE_UNUSED)
{
  enum opt_code code = (enum opt_code) scode;
  int ret = 1;

  switch (code)
    {
    case OPT_I:
    case OPT_L:
      /* For the compiler, we currently handle -I and -L exactly the
	 same way: they give us a directory to search for import
	 statements.  */
      go_add_search_path (arg);
      break;

    case OPT_fgo_dump_:
      ret = go_enable_dump (arg);
      break;

    default:
      /* Just return 1 to indicate that the option is valid.  */
      break;
    }

  return ret;
}

/* Run after parsing options.  */

static bool
go_langhook_post_options (const char **pfilename ATTRIBUTE_UNUSED)
{
  gcc_assert (num_in_fnames > 0);

  if (flag_excess_precision_cmdline == EXCESS_PRECISION_DEFAULT)
    flag_excess_precision_cmdline = EXCESS_PRECISION_STANDARD;

  /* Returning false means that the backend should be used.  */
  return false;
}

static void
go_langhook_parse_file (int set_yy_debug ATTRIBUTE_UNUSED)
{
  go_parse_input_files (in_fnames, num_in_fnames);
}

static tree
go_langhook_type_for_size (unsigned int bits, int unsignedp)
{
  return go_type_for_size (bits, unsignedp);
}

static tree
go_langhook_type_for_mode (enum machine_mode mode, int unsignedp)
{
  return go_type_for_mode (mode, unsignedp);
}

/* Record a builtin function.  We just ignore builtin functions.  */

static tree
go_langhook_builtin_function (tree decl)
{
  return decl;
}

static int
go_langhook_global_bindings_p (void)
{
  return current_function_decl == NULL ? 1 : 0;
}

/* Push a declaration into the current binding level.  We can't
   usefully implement this since we don't want to convert from tree
   back to one of our internal data structures.  I think the only way
   this is used is to record a decl which is to be returned by
   getdecls, and we could implement it for that purpose if
   necessary.  */

static tree
go_langhook_pushdecl (tree decl ATTRIBUTE_UNUSED)
{
  gcc_unreachable ();
}

/* This hook is used to get the current list of declarations as trees.
   We don't support that; instead we use the write_globals hook.  This
   can't simply crash because it is called by -gstabs.  */

static tree
go_langhook_getdecls (void)
{
  return NULL;
}

/* Write out globals.  */

static void
go_langhook_write_globals (void)
{
  go_write_globals ();
}

/* Go specific gimplification.  We need to gimplify
   CALL_EXPR_STATIC_CHAIN, because the gimplifier doesn't handle
   it.  */

static int
go_langhook_gimplify_expr (tree *expr_p, gimple_seq *pre_p, gimple_seq *post_p)
{
  if (TREE_CODE (*expr_p) == CALL_EXPR
      && CALL_EXPR_STATIC_CHAIN (*expr_p) != NULL_TREE)
    gimplify_expr (&CALL_EXPR_STATIC_CHAIN (*expr_p), pre_p, post_p,
		   is_gimple_val, fb_rvalue);
  return GS_UNHANDLED;
}

/* Functions called directly by the generic backend.  */

tree
convert (tree type, tree expr)
{
  if (type == error_mark_node
      || expr == error_mark_node
      || TREE_TYPE (expr) == error_mark_node)
    return error_mark_node;

  if (type == TREE_TYPE (expr))
    return expr;

  if (TYPE_MAIN_VARIANT (type) == TYPE_MAIN_VARIANT (TREE_TYPE (expr)))
    return fold_convert (type, expr);

  switch (TREE_CODE (type))
    {
    case VOID_TYPE:
    case BOOLEAN_TYPE:
      return fold_convert (type, expr);
    case INTEGER_TYPE:
      return fold (convert_to_integer (type, expr));
    case POINTER_TYPE:
      return fold (convert_to_pointer (type, expr));
    case REAL_TYPE:
      return fold (convert_to_real (type, expr));
    default:
      break;
    }

  gcc_unreachable ();
}

/* FIXME: This is a hack to preserve trees that we create from the
   garbage collector.  */

static GTY(()) tree go_gc_root;

void
go_preserve_from_gc(tree t)
{
  go_gc_root = tree_cons (NULL_TREE, t, go_gc_root);
}

#undef LANG_HOOKS_NAME
#undef LANG_HOOKS_INIT
#undef LANG_HOOKS_INIT_OPTIONS
#undef LANG_HOOKS_HANDLE_OPTION
#undef LANG_HOOKS_POST_OPTIONS
#undef LANG_HOOKS_PARSE_FILE
#undef LANG_HOOKS_TYPE_FOR_MODE
#undef LANG_HOOKS_TYPE_FOR_SIZE
#undef LANG_HOOKS_BUILTIN_FUNCTION
#undef LANG_HOOKS_GLOBAL_BINDINGS_P
#undef LANG_HOOKS_PUSHDECL
#undef LANG_HOOKS_GETDECLS
#undef LANG_HOOKS_WRITE_GLOBALS
#undef LANG_HOOKS_GIMPLIFY_EXPR

#define LANG_HOOKS_NAME			"GNU Go"
#define LANG_HOOKS_INIT			go_langhook_init
#define LANG_HOOKS_INIT_OPTIONS		go_langhook_init_options
#define LANG_HOOKS_HANDLE_OPTION	go_langhook_handle_option
#define LANG_HOOKS_POST_OPTIONS		go_langhook_post_options
#define LANG_HOOKS_PARSE_FILE		go_langhook_parse_file
#define LANG_HOOKS_TYPE_FOR_MODE	go_langhook_type_for_mode
#define LANG_HOOKS_TYPE_FOR_SIZE	go_langhook_type_for_size
#define LANG_HOOKS_BUILTIN_FUNCTION	go_langhook_builtin_function
#define LANG_HOOKS_GLOBAL_BINDINGS_P	go_langhook_global_bindings_p
#define LANG_HOOKS_PUSHDECL		go_langhook_pushdecl
#define LANG_HOOKS_GETDECLS		go_langhook_getdecls
#define LANG_HOOKS_WRITE_GLOBALS	go_langhook_write_globals
#define LANG_HOOKS_GIMPLIFY_EXPR	go_langhook_gimplify_expr

struct lang_hooks lang_hooks = LANG_HOOKS_INITIALIZER;

#include "gt-go-go-lang.h"
#include "gtype-go.h"
