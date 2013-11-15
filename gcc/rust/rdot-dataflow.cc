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

static std::vector<std::map<std::string, rdot> *> context;

static
void dot_pass_dataFlow_pushCtx (void)
{
    std::map<std::string, rdot> * ctx = new std::map<std::string, rdot>;
    context.push_back (ctx);
}

static
void dot_pass_dataFlow_popCtx (void)
{
    std::map<std::string, rdot> * ctx = context.back ();
    context.pop_back ();
    delete ctx;
}

static
rdot dot_pass_dataFlow_lookup (const char * id)
{
    rdot retval = NULL_DOT;
    std::vector<std::map<std::string, rdot> *>::reverse_iterator it;
    for (it = context.rbegin (); it != context.rend (); ++it)
    {
	std::map<std::string, rdot> * ctx = *it;
	if (ctx->count (std::string (id)))
	{
	    retval = (*ctx)[std::string (id)];
	    break;
	}
    }
    return retval;
}

static
bool dot_pass_dataFlow_pushDecl (rdot node, const char * id)
{
    rdot check = dot_pass_dataFlow_lookup (id);
    if (check != NULL_DOT)
    {
	error ("DataFlow duplicate declaration [%s]\n", id);
	return true;
    }

    bool retval = false;
    if ((RDOT_TYPE (node) == D_VAR_DECL)
	|| (RDOT_TYPE (node) == D_STRUCT_METHOD))
    {
	std::map<std::string, rdot> * ctx = context.back ();
	(*ctx) [std::string (id)] = node;
    }
    else
    {
	error ("Invalid dataflow declaration pushing to context [%s]\n",
	       RDOT_OPCODE_STR (node));
	retval = true;
    }
    return retval;
}

static void dot_pass_dataFlowToplevel (rdot);
static void dot_pass_dataFlowFunction (rdot);
static void dot_pass_dataFlowBlock (rdot);

static
bool verifyType (rdot node)
{
    bool retval = false;
    switch (RDOT_TYPE (node))
    {
    case RTYPE_BOOL:
    case RTYPE_INT:
    case RTYPE_FLOAT:
    case RTYPE_UINT:
	retval = true;
	break;
	
    default:
	break;
    }
    return retval;
}

static
rdot dot_pass_dataFlow_getDecl (rdot node)
{
    rdot retval = NULL_DOT;
    if (RDOT_T_FIELD (node) == D_D_EXPR)
    {
	// only if its a modify expr
	if (RDOT_TYPE (node) == D_MODIFY_EXPR)
	{
	    rdot decl = RDOT_lhs_TT (node);
	    if (RDOT_TYPE (decl) == D_VAR_DECL)
		retval = decl;
	}
	else if (RDOT_TYPE (node) == D_VAR_DECL)
	    retval = node;
    }
    return retval;
}

/*
  Takes a var_decl and returns the rhs of assignment to try and infer a type on.
  eg1 :
  let x;
  x = 1 + 2
  will return the NULL on the decl and RDOT (1+2) on the expression

  eg2:
  let x = 1;
  ruturns RDOT (1)
 */
static
rdot dot_pass_dataFlow_getRef (rdot decl, rdot var_decl)
{
    rdot retval = NULL_DOT;
    if (RDOT_TYPE (decl) == D_MODIFY_EXPR)
    {
	rdot lhs = RDOT_lhs_TT (decl);
	rdot rhs = RDOT_rhs_TT (decl);

	switch (RDOT_TYPE (lhs))
	{
	case D_VAR_DECL:
	{
	    if (var_decl == lhs)
		retval = rhs;
	}
	break;

	case D_IDENTIFIER:
	{
	    const char * vid = RDOT_IDENTIFIER_POINTER (RDOT_lhs_TT (var_decl));
	    const char * cid = RDOT_IDENTIFIER_POINTER (lhs);
	    // we found a reference assignment...
	    if (strcmp (vid, cid) == 0)
		retval = rhs;
	}
	break;

	default:
	    break;
	}
    }
    return retval;
}

static
opcode_t dot_pass_typeifyPrimitive (rdot node)
{
    opcode_t retval = RTYPE_INFER;
    gcc_assert (RDOT_TYPE (node) == D_PRIMITIVE);

    switch (node->opa.tc->T == D_T_INTEGER)
    {
    case D_T_INTEGER:
	retval = RTYPE_INT;
	break;

    default:
	error ("Unable to figure out type for this primitive [%s]!",
	       RDOT_CODE_STR (node->opa.tc->T));
	break;
    }
    return retval;
}

static
opcode_t dot_pass_typeifyExprNode (rdot node)
{
    opcode_t retval = RTYPE_INFER;

    switch (RDOT_TYPE (node))
    {
    case D_PRIMITIVE:
	retval = dot_pass_typeifyPrimitive (node);
	break;

    case D_IDENTIFIER:
    {
	rdot lookup = dot_pass_dataFlow_lookup (RDOT_IDENTIFIER_POINTER (node));
	if (lookup != NULL_DOT)
	{
	    /* i think it can only be var_decls being refernced
	       here there are no global variables outside of cal_exprs
	       but thats differnet ast node
	    */
	    if (RDOT_TYPE (lookup) == D_VAR_DECL)
		retval = RDOT_TYPE (RDOT_rhs_TT (lookup));
	    else
		error ("unable to find declaration of [%s] in current scope",
		       RDOT_IDENTIFIER_POINTER (node));
	}
	else
	    error ("unable to find declaration of [%s] in current scope",
		   RDOT_IDENTIFIER_POINTER (node));
    }
    break;

    case D_CALL_EXPR:
    {
	const char * callid = RDOT_IDENTIFIER_POINTER (RDOT_lhs_TT (node));
	rdot lookup = dot_pass_dataFlow_lookup (callid);
	if (lookup != NULL_DOT)
	{
	    if (RDOT_TYPE (lookup) == D_STRUCT_METHOD)
		retval = RDOT_TYPE (RDOT_FIELD2 (lookup));
	    else
		error ("unable to find declaration of [%s] in current scope",
		       callid);
	}
	else
	    error ("unable to find declaration of [%s] in current scope",
		   callid);
    }
    break;

    case D_ADD_EXPR:
    case D_MINUS_EXPR:
    case D_MULT_EXPR:
    case D_DIVD_EXPR:
    {
	rdot lhs = RDOT_lhs_TT (node);
	rdot rhs = RDOT_rhs_TT (node);
	opcode_t lt = dot_pass_typeifyExprNode (lhs);
	opcode_t rt = dot_pass_typeifyExprNode (rhs);

	if (lt != rt)
	    error ("unable to coerce types [%s] and [%s]",
		   RDOT_CODE_STR (lt),
		   RDOT_CODE_STR (rt));
	else
	    retval = lt;
    }
    break;

    default:
	fatal_error ("something very wrong!\n");
	break;
    }
    return retval;
}

static
void dot_pass_dataFlowBlock (rdot suite)
{
    dot_pass_dataFlow_pushCtx ();   

    std::vector<rdot> block_decls;
    rdot node;
    for (node = suite; node != NULL_DOT; node = RDOT_CHAIN (node))
    {
	if (RDOT_T_FIELD (node) == D_D_EXPR)
	{
	    const char * id = NULL;
	    rdot decl = dot_pass_dataFlow_getDecl (node);
	    if (decl != NULL_DOT)
	    {
		switch (RDOT_TYPE (decl))
		{
		case D_VAR_DECL:
		{
		    id = RDOT_IDENTIFIER_POINTER (RDOT_lhs_TT (decl));
		    block_decls.push_back (decl);
		}
		break;

		default:
		    fatal_error ("This shouldnt happen [%s]\n",
				 RDOT_OPCODE_STR (decl));
		    break;
		}
		// push it into the current context...
		rdot lookup = dot_pass_dataFlow_lookup (id);
		if (lookup != NULL_DOT)
		    error ("Duplicate declaration of [%s] to [%s]",
			   id, RDOT_OPCODE_STR (lookup));
		else
		    gcc_assert (!dot_pass_dataFlow_pushDecl (decl, id));
	    }
	}
	else {
	    switch (RDOT_TYPE (node))
	    {
	    default:
		error ("Unhandled data flow in block on [%s]\n", RDOT_OPCODE_STR (node));
		break;
	    }
	}
    }

    // now we have all defined block declarations now need to get all
    // references in their use if they have an undefined type.
    std::vector<rdot>::iterator it;
    for (it = block_decls.begin (); it != block_decls.end (); ++it)
    {
	// if is an infered type we need to get all referecnes to its usage
	// in the block to try and figure it out _if_ we can.
	rdot decl = *it;
	gcc_assert (RDOT_TYPE (decl) == D_VAR_DECL);
	// const char * id = RDOT_IDENTIFIER_POINTER (RDOT_lhs_TT (decl));

	if (RDOT_TYPE (RDOT_rhs_TT (decl)) == RTYPE_INFER)
	{
	    std::vector<rdot> flow;	    
	    for (node = suite; node != NULL_DOT; node = RDOT_CHAIN (node))
	    {
		if (RDOT_T_FIELD (node) == D_D_EXPR)
		{
		    rdot ref = dot_pass_dataFlow_getRef (node, decl);
		    if (ref != NULL_DOT)
			flow.push_back (ref);
		}
		else {
		    switch (RDOT_TYPE (node))
		    {
		    default:
			error ("Unhandled data flow on [%s]\n",
			       RDOT_OPCODE_STR (node));
			break;
		    }
		}
	    }

	    if (flow.size () == 0)
	    {
		error ("Unable to infer type on [%s] as it looks to be unused in this scope",
		       RDOT_IDENTIFIER_POINTER (RDOT_lhs_TT (decl)));
	    }
	    else
	    {
		// now go through all the assignments
		std::vector<opcode_t> possible_types;
		std::vector<rdot>::iterator fit;
		for (fit = flow.begin (); fit != flow.end (); ++fit)
		{
		    opcode_t ot =  dot_pass_typeifyExprNode (*fit);
		    if (ot != RTYPE_INFER)
			possible_types.push_back (ot);
		}
		bool found = false;
		bool first = true;
		opcode_t type = RTYPE_INFER;
		
		std::vector<opcode_t>::iterator pit;
		for (pit = possible_types.begin (); pit != possible_types.end (); ++pit)
		{
		    if (first == true) {
			type = *pit;
			first = false;
			found = true;
		    }

		    if (type != *pit) {
			error ("Ambigious types found for [%s] -> [%s] OR [%s]",
			       RDOT_IDENTIFIER_POINTER (RDOT_lhs_TT (decl)),
			       RDOT_CODE_STR (type),
			       RDOT_CODE_STR (*pit));
			found = false;
		    }
		}

		if (found == false)
		    error ("Compiler was unable to infer the type for [%s]",
			   RDOT_IDENTIFIER_POINTER (RDOT_lhs_TT (decl)));
		else
		{
		    gcc_assert (type != RTYPE_INFER);
		    RDOT_TYPE (RDOT_rhs_TT (decl)) = type;
		}
	    }
	}
    }
    
    dot_pass_dataFlow_popCtx ();
}

static
void dot_pass_dataFlowFunction (rdot node)
{
    const char * method_id = RDOT_IDENTIFIER_POINTER (RDOT_FIELD (node));
    rdot type = RDOT_FIELD2 (node);

    /* make sure its a valid type! */
    if (type != NULL)
    {
	bool verify = verifyType (type);
	if (verify == false)
	    error ("unable to verify return type of %s [%s]\n",
		   method_id, RDOT_OPCODE_STR (type));
    }
    // rust does not infer function types empty types are default void
    // and parameters are synatically required to be typed

    rdot suite = RDOT_rhs_TT (node);
    dot_pass_dataFlowBlock (suite);
}

static
void dot_pass_dataFlowToplevel (rdot node)
{
    switch (RDOT_TYPE (node))
    {
    case D_STRUCT_METHOD:
    {
	dot_pass_dataFlowFunction (node);
	const char * id = RDOT_IDENTIFIER_POINTER (RDOT_FIELD (node));
	if (dot_pass_dataFlow_lookup (id))
	    error ("Duplicate declaration against this function [%s]\n", id);
	else
	    dot_pass_dataFlow_pushDecl (node, id);
    }
    break;

    default:
	error ("Unable to dataflow %s\n", RDOT_OPCODE_STR (node));
	break;
    }
}

vec<rdot,va_gc> * dot_pass_inferTypes (vec<rdot,va_gc> * decls)
{
    dot_pass_dataFlow_pushCtx ();

    rdot idtx = NULL_DOT;
    size_t i;
    for (i = 0; decls->iterate (i, &idtx); ++i)
	dot_pass_dataFlowToplevel (idtx);

    dot_pass_dataFlow_popCtx ();
    return decls;
}
