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
static rdot dot_pass_typeifyExprNode (rdot);

static rdot impl_master = NULL_DOT;

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
      || (RDOT_TYPE (node) == D_STRUCT_METHOD)
      || (RDOT_TYPE (node) == D_STRUCT_TYPE))
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

static
void dot_pass_dataFlowBlock_retvals (rdot suite, std::vector<rdot> * retval)
{
  rdot next;
  for (next = suite; next != NULL_DOT; next = RDOT_CHAIN (next))
    {
      if (RDOT_T_FIELD (next) == D_D_EXPR)
	{
	  if (DOT_RETVAL (next))
	    retval->push_back (next);
	}
      else
	{
	  switch (RDOT_TYPE (next))
	    {
	    case D_STRUCT_WHILE:
	      dot_pass_dataFlowBlock_retvals (RDOT_rhs_TT (next), retval);
	      break;

	    case D_STRUCT_IF:
	      {
		rdot ifblock = RDOT_lhs_TT (next);
		rdot elseblock = RDOT_rhs_TT (next);
		dot_pass_dataFlowBlock_retvals (RDOT_rhs_TT (ifblock), retval);
		if (elseblock != NULL_DOT)
		  dot_pass_dataFlowBlock_retvals (RDOT_lhs_TT (elseblock), retval);
	      }
	      break;

	    default:
	      error ("unable to figure out what to do with [%s]",
		     RDOT_OPCODE_STR (next));
	      break;
	    }
	}
    }
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
std::vector<rdot> * dot_pass_getReferences (rdot vDecl, rdot suite)
{
  std::vector<rdot> * retval = new std::vector<rdot>;
  rdot node;
  for (node = suite; node != NULL_DOT; node = RDOT_CHAIN (node))
    {
      switch (RDOT_TYPE (node))
	{
	case D_PRIMITIVE:
	case D_CALL_EXPR:
	case D_ATTRIB_REF:
	  break;

	case D_MODIFY_EXPR:
	  {
	    rdot ref = dot_pass_dataFlow_getRef (node, vDecl);
	    if (ref != NULL_DOT)
	      retval->push_back (ref);
	  }
	  break;

	case D_STRUCT_WHILE:
	  {
	    rdot wsuite = RDOT_rhs_TT (node);
	    std::vector<rdot> * refs = dot_pass_getReferences (vDecl, wsuite);

	    // append to the list
	    std::vector<rdot>::iterator it;
	    for (it = refs->begin (); it != refs->end (); ++it)
		retval->push_back (*it);
	    delete refs;
	  }
	  break;

	case D_STRUCT_IF:
	  {
	    rdot ifb = RDOT_lhs_TT (node);
	    rdot elb = RDOT_rhs_TT (node);

	    rdot ifsuite = RDOT_rhs_TT (ifb);
	    std::vector<rdot> * refs = dot_pass_getReferences (vDecl, ifsuite);

	    // append to the list
	    std::vector<rdot>::iterator it;
	    for (it = refs->begin (); it != refs->end (); ++it)
		retval->push_back (*it);
	    delete refs;

	    if (elb != NULL_DOT)
	      {
		refs = dot_pass_getReferences (vDecl, RDOT_lhs_TT (elb));
		for (it = refs->begin (); it != refs->end (); ++it)
		  retval->push_back (*it);
		delete refs;
	      }
	  }

	default:
	  break;
	}
    }
  return retval;
}

/**
 * WARN:
 *   This really needs more analysis on why, if you have : infered something has:
 *   infer, int, string
 *   infer, int, infer. Does it mean its an int probably.
 *   Currently it drops any possible type to be an int
 **/
static
rdot dot_pass_inferTheType (std::vector<rdot> * refs, const char * id)
{
  rdot retval = NULL_DOT;
  rdot _retval = rdot_build_decl1 (RTYPE_INFER, NULL_DOT);
  gcc_assert (refs->size () > 0);

  std::vector<rdot> possible_types;
  std::vector<rdot>::iterator it;
  for (it = refs->begin (); it != refs->end (); ++it)
    {
      rdot pos = dot_pass_typeifyExprNode (*it);
      if (RDOT_TYPE (pos) != RTYPE_INFER)
	possible_types.push_back (pos);
    }

  if (possible_types.size () > 0)
    {
      bool first = true;
      std::vector<rdot>::iterator pit;
      for (pit = possible_types.begin (); pit != possible_types.end (); ++pit)
	{
	  if (first == true)
	    {
	      retval = *pit;
	      first = false;
	    }
	  rdot itnode = *pit;
	  if (RDOT_TYPE (retval) != RDOT_TYPE (itnode))
	    {
	      // FIXME:
	      // this will need work what if its int vs float... does float win.
	      error ("Ambigious types found for [%s] -> [%s] OR [%s]",
		     id, RDOT_OPCODE_STR (retval), RDOT_OPCODE_STR (*pit));
	      retval = _retval;
	      break;
	    }
	}
    }
  else
    retval = _retval;
  return retval;
}

static
rdot dot_pass_typeifyPrimitive (rdot node)
{
  rdot retval = rdot_build_decl1 (RTYPE_INFER, NULL_DOT);
  gcc_assert (RDOT_TYPE (node) == D_PRIMITIVE);

  switch (node->opa.tc->T == D_T_INTEGER)
    {
    case D_T_INTEGER:
      RDOT_TYPE (retval) = RTYPE_INT;
      break;

    default:
      error ("Unable to figure out type for this primitive [%s]!",
             RDOT_CODE_STR (node->opa.tc->T));
      break;
    }
  return retval;
}

static
rdot dot_pass_typeifyExprNode (rdot node)
{
  rdot retval = rdot_build_decl1 (RTYPE_INFER, NULL_DOT);
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
              retval = RDOT_rhs_TT (lookup);
            else
              error ("unable to find declaration of [%s] in current scope",
                     RDOT_IDENTIFIER_POINTER (node));
          }
        else
          error ("unable to find declaration of [%s] in current scope",
                 RDOT_IDENTIFIER_POINTER (node));
      }
      break;

    case D_ATTRIB_REF:
      {
	rdot lhs = RDOT_lhs_TT (node);
	rdot rhs = RDOT_rhs_TT (node);

	rdot tlh = dot_pass_typeifyExprNode (lhs);
	if (RDOT_TYPE (tlh) == RTYPE_INFER)
	  error ("Unable to infer the type of [%s]", RDOT_OPCODE_STR (lhs));
	else
	  {
	    if (RDOT_TYPE (tlh) != RTYPE_USER_STRUCT)
	      error ("Unable to access the atribute on non struct type [%s]",
		     RDOT_OPCODE_STR (tlh));
	    else
	      {
		rdot mtype = RDOT_lhs_TT (tlh);
		gcc_assert (mtype != NULL_DOT);
		switch (RDOT_TYPE (rhs))
		  {
		  case D_IDENTIFIER:
		    {
		      const char * aid = RDOT_IDENTIFIER_POINTER (rhs);
		      bool found = false;

		      rdot fields;
		      for (fields = RDOT_rhs_TT (mtype); fields != NULL_DOT;
			   fields = RDOT_CHAIN (fields))
			{
			  rdot rid = RDOT_lhs_TT (fields);
			  const char * pid = RDOT_IDENTIFIER_POINTER (rid);
			  if (strcmp (pid, aid) == 0)
			    {
			      found = true;
			      break;
			    }
			}
		      if (!found)
			error ("field [%s] is not found in [%s] [%s]",
			       aid, RDOT_OPCODE_STR (tlh),
			       RDOT_IDENTIFIER_POINTER (RDOT_lhs_TT (mtype)));
		      else
			retval = RDOT_rhs_TT (fields);
		    }
		    break;

		  case D_CALL_EXPR:
		    {
		      rdot callid = RDOT_lhs_TT (rhs);
		      const char * aid = RDOT_IDENTIFIER_POINTER (callid);
		      bool found = false;

		      rdot fields;
		      for (fields = RDOT_rhs_TT (RDOT_FIELD (mtype));
			   fields != NULL_DOT;
			   fields = RDOT_CHAIN (fields))
			{
			  gcc_assert (RDOT_TYPE (fields) == D_STRUCT_METHOD);
			  rdot rmid = RDOT_FIELD (fields);
			  const char * mid = RDOT_IDENTIFIER_POINTER (rmid);
			  if (strcmp (mid, aid) == 0)
			    {
			      found = true;
			      break;
			    }
			}

		      if (!found)
			error ("type [%s] doesn't implement any method named [%s]",
			       RDOT_IDENTIFIER_POINTER (RDOT_lhs_TT (mtype)), aid);
		      else
			retval = RDOT_FIELD2 (fields);
		    }
		    break;

		  default:
		    error ("Unable to infer [%s].[%s]",
			   RDOT_OPCODE_STR (lhs),
			   RDOT_OPCODE_STR (rhs));
		    break;
		  }
	      }
	  }
      }
      break;

    case D_STRUCT_INIT:
      {
	// lets make sure this type exists!
	const char * slookup = RDOT_IDENTIFIER_POINTER (RDOT_lhs_TT (node));
	rdot lookup = dot_pass_dataFlow_lookup (slookup);
	if (lookup != NULL_DOT)
	  {
	    if (RDOT_TYPE (lookup) == D_STRUCT_TYPE)
	      {
		retval = rdot_build_decl1 (RTYPE_USER_STRUCT, lookup);
		RDOT_FIELD (node) = lookup;
	      }
	    else
	      error ("unable to determine type of [%s] struct initilization, "
		     "[%s] was found in this scope",
		     slookup, RDOT_OPCODE_STR (lookup));
	  }
	else
	  error ("[%s] does not name a type in scope", slookup);
      }
      break;

    case D_CALL_EXPR:
      {
        const char * callid = RDOT_IDENTIFIER_POINTER (RDOT_lhs_TT (node));
        rdot lookup = dot_pass_dataFlow_lookup (callid);
        if (lookup != NULL_DOT)
          {
            if (RDOT_TYPE (lookup) == D_STRUCT_METHOD)
              retval = RDOT_FIELD2 (lookup);
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

	rdot lt = dot_pass_typeifyExprNode (lhs);
	rdot rt = dot_pass_typeifyExprNode (rhs);

	if (RDOT_TYPE (lt) == RTYPE_INFER
	    || RDOT_TYPE (rt) == RTYPE_INFER)
	  ; // we cannot say what the type is...
        else
	  {
	    if (RDOT_TYPE (lt) != RDOT_TYPE (rt))
	      error ("unable to coerce types [%s] and [%s]",
		     RDOT_OPCODE_STR (lt),
		     RDOT_OPCODE_STR (rt));
	    else
	      retval = lt;
	  }
      }
      break;

    default:
      error ("Unable to figure out the type of this [%s]",
	     RDOT_OPCODE_STR (node));
      break;
    }
  return retval;
}

static
void dot_pass_dataFlowBlock_ (rdot suite, std::vector<rdot> * decls)
{
  rdot node;
  for (node = suite; node != NULL_DOT; node = RDOT_CHAIN (node))
    {
      if (RDOT_T_FIELD (node) == D_D_EXPR)
        {
          const char * id = NULL;
          rdot decl = dot_pass_dataFlow_getDecl (node);
          if (decl != NULL_DOT)
            {
	      gcc_assert (RDOT_TYPE (decl) == D_VAR_DECL);
	      id = RDOT_IDENTIFIER_POINTER (RDOT_lhs_TT (decl));
	      decls->push_back (decl);

              // push it into the current context...
              rdot lookup = dot_pass_dataFlow_lookup (id);
              if (lookup != NULL_DOT)
                error ("Duplicate declaration of [%s] to [%s]",
                       id, RDOT_OPCODE_STR (lookup));
              else
                gcc_assert (!dot_pass_dataFlow_pushDecl (decl, id));
            }
        }
      else
	{
	  switch (RDOT_TYPE (node))
	    {
	    case D_STRUCT_IF:
	      {
		rdot sif = RDOT_lhs_TT (node);
		rdot ses = RDOT_rhs_TT (node);

		dot_pass_dataFlowBlock_ (RDOT_rhs_TT (sif), decls);
		if (ses != NULL_DOT)
		  dot_pass_dataFlowBlock_ (RDOT_lhs_TT (ses), decls);
	      }
	      break;

	    case D_STRUCT_WHILE:
	      dot_pass_dataFlowBlock_ (RDOT_rhs_TT (node), decls);
	      break;

            case D_STRUCT_LOOP:
              dot_pass_dataFlowBlock_ (RDOT_lhs_TT (node), decls);
              break;

            case C_BREAK_STMT:
            case C_CONT_STMT:
              break;

	    default:
	      error ("Unhandled data flow in block on [%s]\n", RDOT_OPCODE_STR (node));
	      break;
	    }
	}
    }
}

static
void dot_pass_dataFlowBlock (rdot suite)
{
  std::vector<rdot> block_decls;
  dot_pass_dataFlowBlock_ (suite, &block_decls);

  // now we have all defined block declarations now need to get all
  // references in their use if they have an undefined type.
  std::vector<rdot>::iterator it;
  for (it = block_decls.begin (); it != block_decls.end (); ++it)
    {
      rdot decl = *it;
      rdot idecl = RDOT_lhs_TT (decl);
      const char * ident = RDOT_IDENTIFIER_POINTER (idecl);

      if (RDOT_TYPE (RDOT_rhs_TT (decl)) == RTYPE_INFER)
	{
	  std::vector<rdot> * refs = dot_pass_getReferences (decl, suite);
	  if (refs->size () == 0)
	    {
	      error ("Unable to infer type of [%s] it looks to "
		     "be unused in this scope", ident);
	      continue;
	    }
	  RDOT_rhs_TT (decl) = dot_pass_inferTheType (refs, ident);
	  if (RDOT_TYPE (RDOT_rhs_TT (decl)) == RTYPE_INFER)
	    error ("Compiler was unable to infer the type for [%s]", ident);
	  delete refs;
	}
    }
}

static
void dot_pass_dataFlowFunction (rdot node)
{
  const char * method_id = RDOT_IDENTIFIER_POINTER (RDOT_FIELD (node));
  rdot type = RDOT_FIELD2 (node);

  // rust does not infer function types empty types are default void
  // and parameters are synatically required to be typed
  dot_pass_dataFlow_pushCtx ();

  // fill up the parameters here
  rdot params;
  for (params = RDOT_lhs_TT (node); params != NULL_DOT;
       params = RDOT_CHAIN (params))
    {
      rdot pident = RDOT_lhs_TT (params);
      rdot ptype = RDOT_rhs_TT (params);

      const char * cpid = RDOT_IDENTIFIER_POINTER (pident);
      if (strcmp (cpid, "self") == 0)
	{
	  gcc_assert (impl_master != NULL_DOT);
	  rdot stid = RDOT_lhs_TT (impl_master);
	  ptype = rdot_build_decl1 (RTYPE_USER_STRUCT, stid);
	  RDOT_MEM_MODIFIER (ptype) = ALLOC_AUTO;
	}
      rdot vpdecl = rdot_build_varDecl (ptype, RDOT_qual (params), pident);
      bool chk = dot_pass_dataFlow_pushDecl (vpdecl,
					     RDOT_IDENTIFIER_POINTER (pident));
      gcc_assert (chk == false);
    }

  rdot suite = RDOT_rhs_TT (node);
  dot_pass_dataFlowBlock (suite);

  // now check the return type is correct
  /* make sure its a valid type! */
  if (type != NULL)
    {
      bool verify = verifyType (type);
      if (verify == false)
        error ("unable to verify return type of %s [%s]\n",
               method_id, RDOT_OPCODE_STR (type));
      else
	{
	  std::vector<rdot> retvals;
	  dot_pass_dataFlowBlock_retvals (suite, &retvals);

	  if (retvals.size () == 0)
	    error ("Function [%s] does not seem to return anything!", method_id);
	  else
	    {
	      rdot retype = dot_pass_inferTheType (&retvals, method_id);
	      if (RDOT_TYPE (retype) == RTYPE_INFER)
		error ("gcc rust was unable to verify the return type of [%s]",
		       method_id);
	    }
	}
    }
  dot_pass_dataFlow_popCtx ();
}

static
void dot_pass_dataFlowToplevel (rdot node)
{
  switch (RDOT_TYPE (node))
    {
    case D_STRUCT_METHOD:
      dot_pass_dataFlowFunction (node);
      break;

      /* need to dataflow the layout to check the types eventually */
    case D_STRUCT_TYPE:
      break;

    case D_STRUCT_IMPL:
      {
	// look up to make sure the impl name is available...
	const char * implid = RDOT_IDENTIFIER_POINTER (RDOT_lhs_TT (node));
	rdot lookup = dot_pass_dataFlow_lookup (implid);
	if (lookup == NULL_DOT)
	  error ("impl [%s] does not reference a type in scope", implid);
	else
	  {
	    if (RDOT_TYPE (lookup) != D_STRUCT_TYPE)
	      error ("impl [%s] does not reference a struct points to [%s]",
		     implid, RDOT_OPCODE_STR (lookup));
	    else
	      {
		RDOT_FIELD (lookup) = node;
		impl_master = lookup;
		rdot next;
		for (next = RDOT_rhs_TT (node); next != NULL_DOT;
		     next = RDOT_CHAIN (next))
		    dot_pass_dataFlowFunction (next);
		impl_master = NULL_DOT;
	      }
	  }
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
    {
      rdot node = idtx;
      switch (RDOT_TYPE (node))
	{
	case D_STRUCT_METHOD:
	  {
	    const char * id = RDOT_IDENTIFIER_POINTER (RDOT_FIELD (node));
	    if (dot_pass_dataFlow_lookup (id))
	      error ("Duplicate declaration against this function [%s]\n", id);
	    else
	      dot_pass_dataFlow_pushDecl (node, id);
	  }
	  break;

	case D_STRUCT_TYPE:
	  {
	    const char * id = RDOT_IDENTIFIER_POINTER (RDOT_lhs_TT (node));
	    if (dot_pass_dataFlow_lookup (id))
	      error ("Duplicate declaration against this type [%s]\n", id);
	    else
	      dot_pass_dataFlow_pushDecl (node, id);
	  }
	  break;

	case D_STRUCT_IMPL:
	  {
	    // look up to make sure the impl name is available...
	    const char * implid = RDOT_IDENTIFIER_POINTER (RDOT_lhs_TT (node));
	    rdot lookup = dot_pass_dataFlow_lookup (implid);
	    if (lookup == NULL_DOT)
	      error ("impl [%s] does not reference a type in scope", implid);
	    else
	      {
		if (RDOT_TYPE (lookup) != D_STRUCT_TYPE)
		  error ("impl [%s] does not reference a struct points to [%s]",
			 implid, RDOT_OPCODE_STR (lookup));
		else
		  RDOT_FIELD (lookup) = node;
	      }
	  }
	  break;

	default:
	  break;
	}
    }

  for (i = 0; decls->iterate (i, &idtx); ++i)
    dot_pass_dataFlowToplevel (idtx);

  dot_pass_dataFlow_popCtx ();
  return decls;
}
