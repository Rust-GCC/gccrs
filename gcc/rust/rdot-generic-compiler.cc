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

static std::vector<tree> * dot_pass_popContext (void);
static void dot_pass_pushContext (void);

static tree global_retDecl;
static bool global_retDecl_;

static
tree dot_pass_rustToGccType (opcode_t type, bool consty)
{
    tree retval = error_mark_node;
    switch (type)
    {
    case RTYPE_INT:
	retval = integer_type_node;
	break;

    default:
	error ("Unable to figure out gcc type for [%s]\n",
	       RDOT_CODE_STR (type));
	break;
    }
    if (consty)
	retval = build_qualified_type (retval, TYPE_QUAL_CONST);

    return retval;
}

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
void dot_pass_pushDecl (const char * id, tree decl)
{
    tree test = dot_pass_lookupCTU (id);
    if (test == error_mark_node)
    {
	std::map<std::string, tree> * ctx = context.back ();
	(*ctx) [std::string (id)] = decl;
    }
    else
	error ("duplicate declaration of [%s]\n", id);
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

    case D_MODIFY_EXPR:
    {
	rdot lhs = RDOT_lhs_TT (dot);
	rdot rhs = RDOT_rhs_TT (dot);

	tree decl = dot_pass_lowerExpr (lhs, block);
	tree assignment = dot_pass_lowerExpr (rhs, block);

	tree assign = build2 ( MODIFY_EXPR, TREE_TYPE (decl),
			       decl, assignment);
	retval = assign;
    }
    break;

    case D_VAR_DECL:
    {
	const char * varID = RDOT_IDENTIFIER_POINTER (RDOT_lhs_TT (dot));
	bool consty = false;
	if (RDOT_qual (dot) == FINAL)
	    consty = true;
	tree gcc_type = dot_pass_rustToGccType (RDOT_TYPE (RDOT_rhs_TT (dot)),
						consty);
	tree decl = build_decl (RDOT_LOCATION (dot),
				VAR_DECL, get_identifier (varID),
				gcc_type);
	if (dot_pass_lookupCTU (varID) == error_mark_node)
	    dot_pass_pushDecl (varID, decl);
	retval = decl;
    }
    break;

    default:
	error ("unhandled binary operation type [%s]!\n", RDOT_OPCODE_STR (dot));
	break;
    }

    if (DOT_RETVAL (dot))
    {
	if (global_retDecl != error_mark_node)
	{
	    tree retass = fold_build2_loc (RDOT_LOCATION (dot),
					   MODIFY_EXPR, TREE_TYPE (global_retDecl),
					   global_retDecl, retval);
	    append_to_statement_list (retass, block);
	    global_retDecl_ = true;
	}
    }

    return retval;
}

static
tree dot_pass_genifyTopFndecl (rdot node)
{
    /* doesn't handle arguments yet ... */
    const char * method_id = RDOT_IDENTIFIER_POINTER (RDOT_FIELD (node));

    if (dot_pass_lookupCTU (method_id) != error_mark_node)
    {
	error ("Duplicate declaration of function [%s]\n", method_id);
	return error_mark_node;
    }

    tree rtype = void_type_node;
    if (RDOT_FIELD2 (node))
	rtype = dot_pass_rustToGccType (RDOT_TYPE (RDOT_FIELD2 (node)), false);
    tree fntype = build_function_type_list (rtype, NULL_TREE);

    tree fndecl = dot_pass_genFndecl_Basic (RDOT_LOCATION (node),
					    method_id, fntype);
    dot_pass_pushDecl (method_id, fndecl);

    current_function_decl = fndecl;
    tree block = alloc_stmt_list ();

    dot_pass_pushContext ();

    if (rtype != void_type_node)
    {
	global_retDecl = DECL_RESULT (fndecl);
	global_retDecl_ = false;
    }

    rdot suite;
    for (suite = RDOT_rhs_TT (node); suite != NULL_DOT;
         suite = RDOT_CHAIN (suite))
    {
        if (RDOT_T_FIELD (suite) ==  D_D_EXPR)
	    append_to_statement_list (dot_pass_lowerExpr (suite, &block), &block);
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

    // make sure it returns something!!!
    if (rtype != void_type_node)
    {
	if (global_retDecl_ == false)
	{
	    error ("Function [%s] doesn't seem to return anything!!\n", method_id);
	    return error_mark_node;
	}
	tree returnVal = build1 (RETURN_EXPR, rtype,
				 global_retDecl);
	append_to_statement_list (returnVal, &block);
    }

    DECL_PURE_P (fndecl) = true;

    tree bind = NULL_TREE;
    tree declare_vars = DECL_RESULT (fndecl);

    tree head = declare_vars;
    std::vector<tree> * decl_vars = dot_pass_popContext ();
    std::vector<tree>::iterator it;
    for (it = decl_vars->begin (); it != decl_vars->end (); ++it)
    {
	DECL_CHAIN (head) = *it;
	head = *it;
    }
    delete decl_vars;

    tree bl = make_node (BLOCK);
    BLOCK_SUPERCONTEXT (bl) = fndecl;
    DECL_INITIAL (fndecl) = bl;
    BLOCK_VARS(bl) = declare_vars;
    TREE_USED (bl) = true;

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

    // reset them
    global_retDecl = error_mark_node;
    global_retDecl_ = false;

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
std::vector<tree> * dot_pass_popContext (void)
{
    std::vector<tree> * retval = new std::vector<tree>;
    if (context.size () > 0)
    {
	std::map<std::string, tree> * popd = context.back ();
	context.pop_back ();

	std::map<std::string, tree>::iterator it;
	for (it = popd->begin (); it != popd->end (); ++it)
	    retval->push_back (it->second);

	delete popd;
    }
    return retval;
}

vec<tree,va_gc> * dot_pass_Genericify (vec<rdot,va_gc> * decls)
{
    vec<tree,va_gc> * retval;
    vec_alloc (retval, 0);

    dot_pass_setupContext ();
    dot_pass_pushContext ();

    size_t i;
    rdot idtx = NULL_DOT;
    for (i = 0; decls->iterate (i, &idtx); ++i)
    {
        tree item = dot_pass_genifyTopNode (idtx);
	vec_safe_push (retval, item);
    }

    dot_pass_popContext ();
    return retval;
}
