/* Output routines for graphical representation.
   Copyright (C) 1998-2023 Free Software Foundation, Inc.
   Contributed by Ulrich Drepper <drepper@cygnus.com>, 1998.
   Rewritten for DOT output by Steven Bosscher, 2012.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "cfghooks.h"
#include "pretty-print.h"
#include "diagnostic-core.h" /* for fatal_error */
#include "cfganal.h"
#include "cfgloop.h"
#include "graph.h"
#include "dumpfile.h"

/* DOT files with the .dot extension are recognized as document templates
   by a well-known piece of word processing software out of Redmond, WA.
   Therefore some recommend using the .gv extension instead.  Obstinately
   ignore that recommendation...  */
static const char *const graph_ext = ".dot";

/* Open a file with MODE for dumping our graph to.
   Return the file pointer.  */
static FILE *
open_graph_file (const char *base, const char *mode)
{
  size_t namelen = strlen (base);
  size_t extlen = strlen (graph_ext) + 1;
  char *buf = XALLOCAVEC (char, namelen + extlen);
  FILE *fp;

  memcpy (buf, base, namelen);
  memcpy (buf + namelen, graph_ext, extlen);

  fp = fopen (buf, mode);
  if (fp == NULL)
    fatal_error (input_location, "cannot open %s: %m", buf);

  return fp;
}

/* Disable warnings about quoting issues in the pp_xxx calls below
   that (intentionally) don't follow GCC diagnostic conventions.  */
#if __GNUC__ >= 10
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wformat-diag"
#endif

/* Draw a basic block BB belonging to the function with FUNCDEF_NO
   as its unique number.  */
static void
draw_cfg_node (pretty_printer *pp, int funcdef_no, basic_block bb)
{
  const char *shape;
  const char *fillcolor;

  if (bb->index == ENTRY_BLOCK || bb->index == EXIT_BLOCK)
    {
      shape = "Mdiamond";
      fillcolor = "white";
    }
  else
    {
      shape = "record";
      fillcolor =
	BB_PARTITION (bb) == BB_HOT_PARTITION ? "lightpink"
	: BB_PARTITION (bb) == BB_COLD_PARTITION ? "lightblue"
	: "lightgrey";
    }

  pp_printf (pp,
	     "\tfn_%d_basic_block_%d "
	     "[shape=%s,style=filled,fillcolor=%s,label=\"",
	     funcdef_no, bb->index, shape, fillcolor);

  if (bb->index == ENTRY_BLOCK)
    pp_string (pp, "ENTRY");
  else if (bb->index == EXIT_BLOCK)
    pp_string (pp, "EXIT");
  else
    {
      pp_left_brace (pp);
      pp_write_text_to_stream (pp);
      dump_bb_for_graph (pp, bb);
      pp_right_brace (pp);
    }

  pp_string (pp, "\"];\n\n");
  pp_flush (pp);
}

/* Draw all successor edges of a basic block BB belonging to the function
   with FUNCDEF_NO as its unique number.  */
static void
draw_cfg_node_succ_edges (pretty_printer *pp, int funcdef_no, basic_block bb)
{
  edge e;
  edge_iterator ei;
  FOR_EACH_EDGE (e, ei, bb->succs)
    {
      const char *style = "\"solid,bold\"";
      const char *color = "black";
      int weight = 10;

      if (e->flags & EDGE_FAKE)
	{
	  style = "dotted";
	  color = "green";
	  weight = 0;
	}
      else if (e->flags & EDGE_DFS_BACK)
	{
	  style = "\"dotted,bold\"";
	  color = "blue";
	  weight = 10;
	}
      else if (e->flags & EDGE_FALLTHRU)
	weight = 100;
      else if (e->flags & EDGE_TRUE_VALUE)
	color = "forestgreen";
      else if (e->flags & EDGE_FALSE_VALUE)
	color = "darkorange";

      if (e->flags & EDGE_ABNORMAL)
	color = "red";

      pp_printf (pp,
		 "\tfn_%d_basic_block_%d:s -> fn_%d_basic_block_%d:n "
		 "[style=%s,color=%s,weight=%d,constraint=%s",
		 funcdef_no, e->src->index,
		 funcdef_no, e->dest->index,
		 style, color, weight,
		 (e->flags & (EDGE_FAKE | EDGE_DFS_BACK)) ? "false" : "true");
      if (e->probability.initialized_p ())
        pp_printf (pp, ",label=\"[%i%%]\"",
		   e->probability.to_reg_br_prob_base ()
		   * 100 / REG_BR_PROB_BASE);
      pp_printf (pp, "];\n");
    }
  pp_flush (pp);
}

/* Draw all the basic blocks in the CFG in case loops are not available.
   First compute a topological order of the blocks to get a good ranking of
   the nodes.  Then, if any nodes are not reachable from ENTRY, add them at
   the end.  */

static void
draw_cfg_nodes_no_loops (pretty_printer *pp, struct function *fun)
{
  int *rpo = XNEWVEC (int, n_basic_blocks_for_fn (fun));
  int i, n;

  auto_sbitmap visited (last_basic_block_for_fn (fun));
  bitmap_clear (visited);

  n = pre_and_rev_post_order_compute_fn (fun, NULL, rpo, true);
  for (i = n_basic_blocks_for_fn (fun) - n;
       i < n_basic_blocks_for_fn (fun); i++)
    {
      basic_block bb = BASIC_BLOCK_FOR_FN (fun, rpo[i]);
      draw_cfg_node (pp, fun->funcdef_no, bb);
      bitmap_set_bit (visited, bb->index);
    }
  free (rpo);

  if (n != n_basic_blocks_for_fn (fun))
    {
      /* Some blocks are unreachable.  We still want to dump them.  */
      basic_block bb;
      FOR_ALL_BB_FN (bb, fun)
	if (! bitmap_bit_p (visited, bb->index))
	  draw_cfg_node (pp, fun->funcdef_no, bb);
    }
}

/* Draw all the basic blocks in LOOP.  Print the blocks in breath-first
   order to get a good ranking of the nodes.  This function is recursive:
   It first prints inner loops, then the body of LOOP itself.  */

static void
draw_cfg_nodes_for_loop (pretty_printer *pp, int funcdef_no,
			 class loop *loop)
{
  basic_block *body;
  unsigned int i;
  const char *fillcolors[3] = { "grey88", "grey77", "grey66" };

  if (loop->header != NULL
      && loop->latch != EXIT_BLOCK_PTR_FOR_FN (cfun))
    pp_printf (pp,
	       "\tsubgraph cluster_%d_%d {\n"
	       "\tstyle=\"filled\";\n"
	       "\tcolor=\"darkgreen\";\n"
	       "\tfillcolor=\"%s\";\n"
	       "\tlabel=\"loop %d\";\n"
	       "\tlabeljust=l;\n"
	       "\tpenwidth=2;\n",
	       funcdef_no, loop->num,
	       fillcolors[(loop_depth (loop) - 1) % 3],
	       loop->num);

  for (class loop *inner = loop->inner; inner; inner = inner->next)
    draw_cfg_nodes_for_loop (pp, funcdef_no, inner);

  if (loop->header == NULL)
    return;

  if (loop->latch == EXIT_BLOCK_PTR_FOR_FN (cfun))
    body = get_loop_body (loop);
  else
    body = get_loop_body_in_bfs_order (loop);

  for (i = 0; i < loop->num_nodes; i++)
    {
      basic_block bb = body[i];
      if (bb->loop_father == loop)
	draw_cfg_node (pp, funcdef_no, bb);
    }

  free (body);

  if (loop->latch != EXIT_BLOCK_PTR_FOR_FN (cfun))
    pp_printf (pp, "\t}\n");
}

/* Draw all the basic blocks in the CFG in case the loop tree is available.
   All loop bodys are printed in clusters.  */

static void
draw_cfg_nodes (pretty_printer *pp, struct function *fun)
{
  /* ???  The loop and dominance APIs are dependent on fun == cfun.  */
  if (fun == cfun && loops_for_fn (fun))
    draw_cfg_nodes_for_loop (pp, fun->funcdef_no, get_loop (fun, 0));
  else
    draw_cfg_nodes_no_loops (pp, fun);
}

/* Draw all edges in the CFG.  Retreating edges are drawin as not
   constraining, this makes the layout of the graph better.  */

static void
draw_cfg_edges (pretty_printer *pp, struct function *fun)
{
  basic_block bb;

  /* Save EDGE_DFS_BACK flag to dfs_back.  */
  auto_bitmap dfs_back;
  edge e;
  edge_iterator ei;
  unsigned int idx = 0;
  FOR_EACH_BB_FN (bb, fun)
    FOR_EACH_EDGE (e, ei, bb->succs)
      {
	if (e->flags & EDGE_DFS_BACK)
	  bitmap_set_bit (dfs_back, idx);
	idx++;
      }

  mark_dfs_back_edges (fun);
  FOR_ALL_BB_FN (bb, fun)
    draw_cfg_node_succ_edges (pp, fun->funcdef_no, bb);

  /* Restore EDGE_DFS_BACK flag from dfs_back.  */
  idx = 0;
  FOR_EACH_BB_FN (bb, fun)
    FOR_EACH_EDGE (e, ei, bb->succs)
      {
	if (bitmap_bit_p (dfs_back, idx))
	  e->flags |= EDGE_DFS_BACK;
	else
	  e->flags &= ~EDGE_DFS_BACK;
	idx++;
      }

  /* Add an invisible edge from ENTRY to EXIT, to improve the graph layout.  */
  pp_printf (pp,
	     "\tfn_%d_basic_block_%d:s -> fn_%d_basic_block_%d:n "
	     "[style=\"invis\",constraint=true];\n",
	     fun->funcdef_no, ENTRY_BLOCK,
	     fun->funcdef_no, EXIT_BLOCK);
  pp_flush (pp);
}

/* Print a graphical representation of the CFG of function FUN.
   First print all basic blocks.  Draw all edges at the end to get
   subgraphs right for GraphViz, which requires nodes to be defined
   before edges to cluster nodes properly.  */

void DEBUG_FUNCTION
print_graph_cfg (FILE *fp, struct function *fun)
{
  pretty_printer graph_slim_pp;
  graph_slim_pp.buffer->stream = fp;
  pretty_printer *const pp = &graph_slim_pp;
  const char *funcname = function_name (fun);
  pp_printf (pp, "subgraph \"cluster_%s\" {\n"
		 "\tstyle=\"dashed\";\n"
		 "\tcolor=\"black\";\n"
		 "\tlabel=\"%s ()\";\n",
		 funcname, funcname);
  draw_cfg_nodes (pp, fun);
  draw_cfg_edges (pp, fun);
  pp_printf (pp, "}\n");
  pp_flush (pp);
}

/* Overload with additional flag argument.  */

void DEBUG_FUNCTION
print_graph_cfg (FILE *fp, struct function *fun, dump_flags_t flags)
{
  dump_flags_t saved_dump_flags = dump_flags;
  dump_flags = flags;
  print_graph_cfg (fp, fun);
  dump_flags = saved_dump_flags;
}


/* Print a graphical representation of the CFG of function FUN.
   First print all basic blocks.  Draw all edges at the end to get
   subgraphs right for GraphViz, which requires nodes to be defined
   before edges to cluster nodes properly.  */

void
print_graph_cfg (const char *base, struct function *fun)
{
  FILE *fp = open_graph_file (base, "a");
  print_graph_cfg (fp, fun);
  fclose (fp);
}

/* Start the dump of a graph.  */
static void
start_graph_dump (FILE *fp, const char *base)
{
  pretty_printer graph_slim_pp;
  graph_slim_pp.buffer->stream = fp;
  pretty_printer *const pp = &graph_slim_pp;
  pp_string (pp, "digraph \"");
  pp_write_text_to_stream (pp);
  pp_string (pp, base);
  pp_write_text_as_dot_label_to_stream (pp, /*for_record=*/false);
  pp_string (pp, "\" {\n");
  pp_string (pp, "overlap=false;\n");
  pp_flush (pp);
}

/* End the dump of a graph.  */
static void
end_graph_dump (FILE *fp)
{
  fputs ("}\n", fp);
}

/* Similar as clean_dump_file, but this time for graph output files.  */
void
clean_graph_dump_file (const char *base)
{
  FILE *fp = open_graph_file (base, "w");
  start_graph_dump (fp, base);
  fclose (fp);
}


/* Do final work on the graph output file.  */
void
finish_graph_dump_file (const char *base)
{
  FILE *fp = open_graph_file (base, "a");
  end_graph_dump (fp);
  fclose (fp);
}

#if __GNUC__ >= 10
#  pragma GCC diagnostic pop
#endif
