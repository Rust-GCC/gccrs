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

static bool _no_infer = false;

#define RDOT_PREFIX_PRE      "pre-rdot"
#define RDOT_PREFIX_POST     "post-rdot"

static const char * typeStrings [] = {
    "bool",
    "int",
    "float",
    "unsigned_int",
    "__infer_me",
    "void"
};

static const char * typeStringNode (rdot node)
{
    const char * retval = typeStrings [5];
    if (node != NULL_DOT)
    {
	switch (RDOT_TYPE (node))
	{
	case RTYPE_BOOL:
	    retval = typeStrings [0];
	    break;

	case RTYPE_INT:
	    retval = typeStrings [1];
	    break;

	case RTYPE_FLOAT:
	    retval = typeStrings [2];
	    break;

	case RTYPE_UINT:
	    retval = typeStrings [3];
	    break;

	case RTYPE_INFER:
	    if (_no_infer) {
		retval = xstrdup ("The Compiler Failed to infer this type!");
		error ("gcc-rust has failed to infer a type and cannot continue");
	    }
	    else
		retval = typeStrings [4];
	    break;

	default:
	    break;
	}
    }
    return retval;
}

static void dot_pass_dump_node (FILE *, rdot, size_t);
static void dot_pass_dump_method (FILE *, rdot, size_t);

static void dot_pass_dumpPrimitive (FILE *, rdot);
static void dot_pass_dumpExprNode (FILE *, rdot);
static void dot_pass_dump_expr (FILE *, rdot);

static bool first = true;

static
void dot_pass_dump_method (FILE * fd, rdot node, size_t indents)
{
  size_t i;
  for (i = 0; i < indents; ++i)
    fprintf (fd, "  ");

  const char * method_id = RDOT_IDENTIFIER_POINTER (RDOT_FIELD (node));
  const char * rtype = typeStringNode (RDOT_FIELD2 (node));

  fprintf (fd, "fn %s ( void ) -> %s {\n", method_id, rtype);

  rdot suite = RDOT_rhs_TT (node);
  do {
      dot_pass_dump_node (fd, suite, indents + 1);
      fprintf (fd, "\n");
  }
  while ((suite = RDOT_CHAIN (suite)));

  for (i = 0; i < indents; ++i)
      fprintf (fd, "  ");
  fprintf (fd, "}\n");
}

static
void dot_pass_dumpPrimitive (FILE * fd, rdot node)
{
  /* Handle other primitive literal types here ... */
  switch (RDOT_lhs_TC (node)->T)
    {
    case D_T_INTEGER:
	fprintf (fd, "%i", RDOT_lhs_TC (node)->o.integer);
	break;

    case D_T_STRING:
	fprintf (fd, "\"%s\"", RDOT_lhs_TC (node)->o.string);
	break;

    default:
	fatal_error ("Something very wrong!\n");
	break;
    }
}

static
void dot_pass_dumpExprNode (FILE * fd, rdot node)
{
  /* print expr tree ... */
  switch (RDOT_TYPE (node))
    {
    case D_PRIMITIVE:
	dot_pass_dumpPrimitive (fd, node);
	break;

    case D_IDENTIFIER:
	fprintf (fd, "%s", RDOT_IDENTIFIER_POINTER (node));
	break;

    case D_CALL_EXPR:
    {
	rdot id = RDOT_lhs_TT (node);
	dot_pass_dump_expr (fd, id);
	fprintf (fd, " (");

	rdot p;
	for (p = RDOT_rhs_TT (node); p != NULL_DOT; p = RDOT_CHAIN (p))
	    dot_pass_dump_expr (fd, p);
	fprintf (fd, ")");
    }
    break;

    case D_VAR_DECL:
    {
	const char * mut;
	if (RDOT_qual (node) == FINAL)
	    mut = "_final_";
	else
	    mut = "_mut_";

	fprintf (fd, "let [%s] ", mut);
	dot_pass_dumpExprNode (fd, RDOT_lhs_TT (node));
	fprintf (fd, " -> [%s]", typeStringNode (RDOT_rhs_TT (node)));
    }
    break;

    default:
	fatal_error ("unhandled dumpExprNode!\n");
	break;
    }
}

static
void dot_pass_dump_expr (FILE * fd, rdot node)
{
    if (DOT_RETVAL (node)) {
	fprintf (fd, "_rust_retval:\n");
    }

    switch (RDOT_TYPE (node))
    {
    case D_PRIMITIVE:
    case D_IDENTIFIER:
    case D_CALL_EXPR:
	dot_pass_dumpExprNode (fd, node);
	break;
	
    default:
    {
	/* print expr tree ... */
	rdot lhs = RDOT_lhs_TT (node);
	rdot rhs = RDOT_rhs_TT (node);
	
	dot_pass_dumpExprNode (fd, lhs);
	switch (RDOT_TYPE (node))
	{
	case D_MODIFY_EXPR:
	    fprintf (fd, " = ");
	    break;

	case D_ADD_EXPR:
	    fprintf (fd, " + ");
	    break;

	case D_MINUS_EXPR:
	    fprintf (fd, " - ");
	    break;

	case D_MULT_EXPR:
	    fprintf (fd, " * ");
	    break;

	case D_LESS_EXPR:
	    fprintf (fd, " < ");
	    break;
	    
	case D_GREATER_EXPR:
	    fprintf (fd, " > ");
	    break;

	case D_EQ_EQ_EXPR:
	    fprintf (fd, " == ");
	    break;

	case D_NOT_EQ_EXPR:
	    fprintf (fd, " != ");
	    break;

	default:
	    fatal_error ("unhandled dump!\n");
	    break;
	}
	dot_pass_dumpExprNode (fd, rhs);
    }
    break;
    }
}

static
void dot_pass_dump_node (FILE * fd, rdot node, size_t indents)
{
    if (RDOT_T_FIELD (node) ==  D_D_EXPR)
    {
	size_t i;
	for (i = 0; i < indents; ++i)
	    fprintf (fd, "    ");
	dot_pass_dump_expr (fd, node);
	fprintf (fd, ";");
    }
    else
    {
	switch (RDOT_TYPE (node))
	{
	case D_PRIMITIVE:
	    dot_pass_dump_expr (fd, node);
	    break;

	case D_STRUCT_METHOD:
	    dot_pass_dump_method (fd, node, indents);
	    break;
	    
	default:
	    break;
	}
    }
}

vec<rdot,va_gc> * dot_pass_PrettyPrint (vec<rdot,va_gc> * decls)
{
    if (GRS_OPT_dump_dot)
    {
	size_t bsize = 128;
	char * outfile =  (char *) alloca (bsize);
	gcc_assert (outfile);
	memset (outfile, 0, bsize);

	strncpy (outfile, GRS_current_infile, strlen (GRS_current_infile) - 2);
	if (first == true)
	{
	    strncat (outfile, RDOT_PREFIX_PRE, sizeof (RDOT_PREFIX_PRE));
	    first = false;
	}
	else
	{
	    strncat (outfile, RDOT_PREFIX_POST, sizeof (RDOT_PREFIX_POST));
	    _no_infer = true;
	}

	FILE * fd = fopen (outfile, "w");
	if (!fd)
	{
	    error ("Unable to open %s for write\n", outfile);
	    goto exit;
	}

	rdot idtx = NULL_DOT;
	size_t i;
	for (i = 0; decls->iterate (i, &idtx); ++i)
	{
	    dot_pass_dump_node (fd, idtx, 0);
	    fprintf (fd, "\n");
	}

	fclose (fd);
    }
exit:
    return decls;
}
