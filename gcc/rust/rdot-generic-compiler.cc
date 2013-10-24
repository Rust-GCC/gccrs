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

static std::vector<std::map<std::string, tree> *> context;

static tree dot_pass_genFndecl_Basic (location_t, const char *, tree);

static
tree dot_pass_genFndecl_Basic (location_t loc, const char * ident, tree fntype)
{
    tree fndecl = build_decl (loc, FUNCTION_DECL,
                              get_identifier (ident), fntype);
    TREE_STATIC (fndecl) = 0;
    TREE_USED (fndecl) = 1;
    TREE_PUBLIC (fndecl) = 1;

    tree argslist = NULL_TREE;
    DECL_ARGUMENTS (fndecl) = argslist;

    tree resdecl = build_decl (BUILTINS_LOCATION, RESULT_DECL,
                               NULL_TREE, TREE_TYPE (fntype));
    DECL_CONTEXT (resdecl) = fndecl;
    DECL_ARTIFICIAL (resdecl) = true;
    DECL_IGNORED_P (resdecl) = true;
    DECL_RESULT (fndecl) = resdecl;

    if (DECL_STRUCT_FUNCTION (fndecl) == NULL)
        push_struct_function (fndecl);
    else
        push_cfun (DECL_STRUCT_FUNCTION (fndecl));
    return fndecl;
}

static
tree dot_pass_generateCString (const char * str)
{
    tree index_type = build_index_type (size_int (strlen (str)));
    tree const_char_type = build_qualified_type (char_type_node, TYPE_QUAL_CONST);
    tree string_type = build_array_type (const_char_type, index_type);
    string_type = build_variant_type_copy (string_type);

    TYPE_STRING_FLAG (string_type) = 1;
    tree string_val = build_string (strlen (str), str);
    TREE_TYPE (string_val) = string_type;

    return string_val;
}

static
tree dot_pass_lookupCTU (const char * id)
{
    tree retval = error_mark_node;

    std::vector<std::map<std::string, tree> *>::reverse_iterator it;
    for (it = context.rbegin (); it != context.rend (); ++it)
    {
	std::map<std::string, tree> * ctx = *it;
	if (ctx->count (std::string (id)))
	{
	    retval = (*ctx)[std::string (id)];
	    break;
	}
    }

    return retval;
}

static
tree dot_pass_genScalar (rdot decl)
{
    tree retval = error_mark_node;
    gcc_assert (RDOT_TYPE (decl) == D_PRIMITIVE);
    gcc_assert (RDOT_lhs_T (decl) == D_TD_COM);

    switch (RDOT_lhs_TC (decl)->T)
    {
    case D_T_INTEGER:
	retval = build_int_cst (integer_type_node, RDOT_lhs_TC (decl)->o.integer);
	break;
	
    case D_T_STRING:
    {
	tree stringVal = dot_pass_generateCString (RDOT_lhs_TC (decl)->o.string);
	retval = build_fold_addr_expr (stringVal);
    }
    break;

    default:
	error ("invalid scalar type %i!\n", RDOT_lhs_TC (decl)->T);
	break;
    }
    return retval;
}

static
tree dot_pass_genifyCall (tree mfndecl, vec<tree,va_gc> * arguments)
{
    tree retval = error_mark_node;
    if (TREE_CODE (mfndecl) == FUNCTION_DECL)
    {
	retval = build_call_expr_loc_vec (UNKNOWN_LOCATION,
					  mfndecl, arguments);
    }
    return retval;
}

static
tree dot_pass_lowerExpr (rdot dot, tree * block)
{
    tree retval = error_mark_node;
    switch (RDOT_TYPE (dot))
    {
    case D_PRIMITIVE:
        retval = dot_pass_genScalar (dot);
        break;

    case D_CALL_EXPR:
    {
	const char * fnid = RDOT_IDENTIFIER_POINTER (RDOT_lhs_TT (dot));

	rdot ptr;
	vec<tree,va_gc> * arguments;
	vec_alloc (arguments, 0);
	for (ptr = RDOT_rhs_TT (dot); ptr != NULL_DOT;
	     ptr = RDOT_CHAIN (ptr))
	    vec_safe_push (arguments, dot_pass_lowerExpr (ptr, block));

	/* lookup the function prototype */
	tree lookup = dot_pass_lookupCTU (fnid);
	if (lookup != error_mark_node)
	    retval = dot_pass_genifyCall (lookup, arguments);
	else
	{
	    error ("Unable to find callable %s\n", fnid);
	    retval = error_mark_node;
	}
    }
    break;

    default:
    {
        switch (RDOT_TYPE (dot))
        {
        default:
            error ("unhandled binary operation type %i!\n", RDOT_TYPE (dot));
            break;
        }
    }
    break;
    }

    return retval;
}

static
tree dot_pass_genifyTopFndecl (rdot node)
{
    const char * method_id = RDOT_IDENTIFIER_POINTER (RDOT_FIELD (node));
    tree fntype = build_function_type_list (void_type_node, NULL_TREE);
    tree fndecl = dot_pass_genFndecl_Basic (UNKNOWN_LOCATION, method_id, fntype);
    current_function_decl = fndecl;

    tree block = alloc_stmt_list ();

    rdot suite;
    for (suite = RDOT_rhs_TT (node); suite != NULL_DOT;
         suite = RDOT_CHAIN (suite))
    {
        if (RDOT_T_FIELD (suite) ==  D_D_EXPR)
        {
	    append_to_statement_list (dot_pass_lowerExpr (suite, &block), &block);
        }
        else
        {
            switch (RDOT_TYPE (suite))
            {
            default:
                error ("Unhandled symbol %i\n", RDOT_TYPE (suite));
                break;
            }
        }
    }
    tree bind = NULL_TREE;
    tree bl = build_block (DECL_RESULT (fndecl), NULL_TREE, fndecl, NULL_TREE);
    DECL_INITIAL (fndecl) = bl;
    TREE_USED (bl) = 1;

    bind = build3 (BIND_EXPR, void_type_node, BLOCK_VARS (bl),
                   NULL_TREE, bl);
    TREE_SIDE_EFFECTS (bind) = 1;
    /* Finalize the main function */
    BIND_EXPR_BODY (bind) = block;
    block = bind;
    DECL_SAVED_TREE (fndecl) = block;

    gimplify_function_tree (fndecl);
    cgraph_finalize_function (fndecl, false);

    pop_cfun ();

    return fndecl;
}

static
tree dot_pass_genifyTopNode (rdot node)
{
    tree retval = error_mark_node;
    switch (RDOT_TYPE (node))
    {
    case D_STRUCT_METHOD:
        retval = dot_pass_genifyTopFndecl (node);
        break;

    default:
        error ("Unhandled Toplevel declaration of type %i\n", RDOT_TYPE (node));
        break;
    }
    return retval;
}

static
void dot_pass_setupContext (void)
{
    std::map<std::string, tree> * lgrs = new std::map<std::string, tree>();
    rs_fill_runtime_decls (lgrs);
    context.push_back (lgrs);
}

static
void dot_pass_pushContext (void)
{
    std::map<std::string, tree> * nctx = new std::map<std::string, tree>;
    context.push_back (nctx);
}

static
void dot_pass_popContext (void)
{
    if (context.size () > 0)
    {
	std::map<std::string, tree> * popd = context.back ();
	context.pop_back ();
	delete popd;
    }
}

vec<tree,va_gc> * dot_pass_Genericify (vec<rdot,va_gc> * decls)
{
    vec<tree,va_gc> * retval;
    vec_alloc (retval, 0);

    dot_pass_setupContext ();

    size_t i;
    rdot idtx = NULL_DOT;
    for (i = 0; decls->iterate (i, &idtx); ++i)
    {
        tree item = dot_pass_genifyTopNode (idtx);
	vec_safe_push (retval, item);
    }
    return retval;
}
