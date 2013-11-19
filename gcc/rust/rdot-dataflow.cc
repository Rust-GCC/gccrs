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
	case D_MODIFY_EXPR:
	  {
	    rdot ref = dot_pass_dataFlow_getRef (node, vDecl);
	    if (ref != NULL_DOT)
	      retval->push_back (ref);
	  }
	  break;

	default:
	  fatal_error ("Unable to dataflow [%s] into [%s]",
		       __FUNCTION__, RDOT_OPCODE_STR (node));
	  break;
	}
    }
  return retval;
}

static
rdot dot_pass_inferTheType (std::vector<rdot> * refs, const char * id)
{
  rdot retval = rdot_build_decl1 (RTYPE_INFER, NULL_DOT);
  gcc_assert (refs->size () > 0);

  std::vector<rdot> possible_types;
  std::vector<rdot>::iterator it;
  for (it = refs->begin (); it != refs->end (); ++it)
      possible_types.push_back (dot_pass_typeifyExprNode (*it));

  bool found = false;
  bool first = true;
  std::vector<rdot>::iterator pit;
  for (pit = possible_types.begin (); pit != possible_types.end (); ++pit)
    {
      if (first == true)
	{
	  retval = *pit;
	  first = false;
	  found = true;
	}

      if (retval != *pit)
	{
	  error ("Ambigious types found for [%s] -> [%s] OR [%s]",
		 id, RDOT_OPCODE_STR (retval), RDOT_OPCODE_STR (*pit));
	  found = false;
	}
    }
  if (RDOT_TYPE (retval) == RTYPE_INFER)
    error ("Was unable to infer the type for [%s]", id);

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
	  error ("unable to determine type of [%s], struct defintion "
		 "doesn't exist in scrope", slookup);
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

        if (RDOT_TYPE (lt) != RDOT_TYPE (rt))
          error ("unable to coerce types [%s] and [%s]",
                 RDOT_OPCODE_STR (lt),
                 RDOT_OPCODE_STR (rt));
        else
          retval = lt;
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

    case D_STRUCT_TYPE:
      {
	const char * id = RDOT_IDENTIFIER_POINTER (RDOT_lhs_TT (node));
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
