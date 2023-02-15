/* Generic partial redundancy elimination with lazy code motion support.
   Copyright (C) 1998-2023 Free Software Foundation, Inc.

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

/* These routines are meant to be used by various optimization
   passes which can be modeled as lazy code motion problems.
   Including, but not limited to:

	* Traditional partial redundancy elimination.

	* Placement of caller/caller register save/restores.

	* Load/store motion.

	* Copy motion.

	* Conversion of flat register files to a stacked register
	model.

	* Dead load/store elimination.

  These routines accept as input:

	* Basic block information (number of blocks, lists of
	predecessors and successors).  Note the granularity
	does not need to be basic block, they could be statements
	or functions.

	* Bitmaps of local properties (computed, transparent and
	anticipatable expressions).

  The output of these routines is bitmap of redundant computations
  and a bitmap of optimal placement points.  */


#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "cfganal.h"
#include "lcm.h"

/* Edge based LCM routines.  */
static void compute_antinout_edge (sbitmap *, sbitmap *, sbitmap *, sbitmap *);
static void compute_earliest (struct edge_list *, int, sbitmap *, sbitmap *,
			      sbitmap *, sbitmap *, sbitmap *);
static void compute_laterin (struct edge_list *, sbitmap *, sbitmap *,
			     sbitmap *, sbitmap *);
static void compute_insert_delete (struct edge_list *edge_list, sbitmap *,
				   sbitmap *, sbitmap *, sbitmap *, sbitmap *);

/* Edge based LCM routines on a reverse flowgraph.  */
static void compute_farthest (struct edge_list *, int, sbitmap *, sbitmap *,
			      sbitmap*, sbitmap *, sbitmap *);
static void compute_nearerout (struct edge_list *, sbitmap *, sbitmap *,
			       sbitmap *, sbitmap *);
static void compute_rev_insert_delete (struct edge_list *edge_list, sbitmap *,
				       sbitmap *, sbitmap *, sbitmap *,
				       sbitmap *);

/* Edge based lcm routines.  */

/* Compute expression anticipatability at entrance and exit of each block.
   This is done based on the flow graph, and not on the pred-succ lists.
   Other than that, its pretty much identical to compute_antinout.  */

static void
compute_antinout_edge (sbitmap *antloc, sbitmap *transp, sbitmap *antin,
		       sbitmap *antout)
{
  basic_block bb;
  edge e;
  basic_block *worklist, *qin, *qout, *qend;
  unsigned int qlen;
  edge_iterator ei;

  /* Allocate a worklist array/queue.  Entries are only added to the
     list if they were not already on the list.  So the size is
     bounded by the number of basic blocks.  */
  qin = qout = worklist = XNEWVEC (basic_block, n_basic_blocks_for_fn (cfun));

  /* We want a maximal solution, so make an optimistic initialization of
     ANTIN.  */
  bitmap_vector_ones (antin, last_basic_block_for_fn (cfun));

  /* Put every block on the worklist; this is necessary because of the
     optimistic initialization of ANTIN above.  */
  int *postorder = XNEWVEC (int, n_basic_blocks_for_fn (cfun));
  int postorder_num = post_order_compute (postorder, false, false);
  for (int i = 0; i < postorder_num; ++i)
    {
      bb = BASIC_BLOCK_FOR_FN (cfun, postorder[i]);
      *qin++ = bb;
      bb->aux = bb;
    }
  free (postorder);

  qin = worklist;
  qend = &worklist[n_basic_blocks_for_fn (cfun) - NUM_FIXED_BLOCKS];
  qlen = n_basic_blocks_for_fn (cfun) - NUM_FIXED_BLOCKS;

  /* Mark blocks which are predecessors of the exit block so that we
     can easily identify them below.  */
  FOR_EACH_EDGE (e, ei, EXIT_BLOCK_PTR_FOR_FN (cfun)->preds)
    e->src->aux = EXIT_BLOCK_PTR_FOR_FN (cfun);

  /* Iterate until the worklist is empty.  */
  while (qlen)
    {
      /* Take the first entry off the worklist.  */
      bb = *qout++;
      qlen--;

      if (qout >= qend)
	qout = worklist;

      if (bb->aux == EXIT_BLOCK_PTR_FOR_FN (cfun))
	/* Do not clear the aux field for blocks which are predecessors of
	   the EXIT block.  That way we never add then to the worklist
	   again.  */
	bitmap_clear (antout[bb->index]);
      else
	{
	  /* Clear the aux field of this block so that it can be added to
	     the worklist again if necessary.  */
	  bb->aux = NULL;
	  bitmap_intersection_of_succs (antout[bb->index], antin, bb);
	}

      if (bitmap_or_and (antin[bb->index], antloc[bb->index],
				   transp[bb->index], antout[bb->index]))
	/* If the in state of this block changed, then we need
	   to add the predecessors of this block to the worklist
	   if they are not already on the worklist.  */
	FOR_EACH_EDGE (e, ei, bb->preds)
	  if (!e->src->aux && e->src != ENTRY_BLOCK_PTR_FOR_FN (cfun))
	    {
	      *qin++ = e->src;
	      e->src->aux = e;
	      qlen++;
	      if (qin >= qend)
		qin = worklist;
	    }
    }

  clear_aux_for_edges ();
  clear_aux_for_blocks ();
  free (worklist);
}

/* Compute the earliest vector for edge based lcm.  */

static void
compute_earliest (struct edge_list *edge_list, int n_exprs, sbitmap *antin,
		  sbitmap *antout, sbitmap *avout, sbitmap *kill,
		  sbitmap *earliest)
{
  int x, num_edges;
  basic_block pred, succ;

  num_edges = NUM_EDGES (edge_list);

  auto_sbitmap difference (n_exprs), temp_bitmap (n_exprs);
  for (x = 0; x < num_edges; x++)
    {
      pred = INDEX_EDGE_PRED_BB (edge_list, x);
      succ = INDEX_EDGE_SUCC_BB (edge_list, x);
      if (pred == ENTRY_BLOCK_PTR_FOR_FN (cfun))
	bitmap_copy (earliest[x], antin[succ->index]);
      else
	{
	  if (succ == EXIT_BLOCK_PTR_FOR_FN (cfun))
	    bitmap_clear (earliest[x]);
	  else
	    {
	      bitmap_and_compl (difference, antin[succ->index],
				  avout[pred->index]);
	      bitmap_not (temp_bitmap, antout[pred->index]);
	      bitmap_and_or (earliest[x], difference,
				    kill[pred->index], temp_bitmap);
	    }
	}
    }
}

/* later(p,s) is dependent on the calculation of laterin(p).
   laterin(p) is dependent on the calculation of later(p2,p).

     laterin(ENTRY) is defined as all 0's
     later(ENTRY, succs(ENTRY)) are defined using laterin(ENTRY)
     laterin(succs(ENTRY)) is defined by later(ENTRY, succs(ENTRY)).

   If we progress in this manner, starting with all basic blocks
   in the work list, anytime we change later(bb), we need to add
   succs(bb) to the worklist if they are not already on the worklist.

   Boundary conditions:

     We prime the worklist all the normal basic blocks.   The ENTRY block can
     never be added to the worklist since it is never the successor of any
     block.  We explicitly prevent the EXIT block from being added to the
     worklist.

     We optimistically initialize LATER.  That is the only time this routine
     will compute LATER for an edge out of the entry block since the entry
     block is never on the worklist.  Thus, LATERIN is neither used nor
     computed for the ENTRY block.

     Since the EXIT block is never added to the worklist, we will neither
     use nor compute LATERIN for the exit block.  Edges which reach the
     EXIT block are handled in the normal fashion inside the loop.  However,
     the insertion/deletion computation needs LATERIN(EXIT), so we have
     to compute it.  */

static void
compute_laterin (struct edge_list *edge_list, sbitmap *earliest,
		 sbitmap *antloc, sbitmap *later, sbitmap *laterin)
{
  int num_edges, i;
  edge e;
  basic_block *worklist, *qin, *qout, *qend, bb;
  unsigned int qlen;
  edge_iterator ei;

  num_edges = NUM_EDGES (edge_list);

  /* Allocate a worklist array/queue.  Entries are only added to the
     list if they were not already on the list.  So the size is
     bounded by the number of basic blocks.  */
  qin = qout = worklist
    = XNEWVEC (basic_block, n_basic_blocks_for_fn (cfun));

  /* Initialize a mapping from each edge to its index.  */
  for (i = 0; i < num_edges; i++)
    INDEX_EDGE (edge_list, i)->aux = (void *) (size_t) i;

  /* We want a maximal solution, so initially consider LATER true for
     all edges.  This allows propagation through a loop since the incoming
     loop edge will have LATER set, so if all the other incoming edges
     to the loop are set, then LATERIN will be set for the head of the
     loop.

     If the optimistic setting of LATER on that edge was incorrect (for
     example the expression is ANTLOC in a block within the loop) then
     this algorithm will detect it when we process the block at the head
     of the optimistic edge.  That will requeue the affected blocks.  */
  bitmap_vector_ones (later, num_edges);

  /* Note that even though we want an optimistic setting of LATER, we
     do not want to be overly optimistic.  Consider an outgoing edge from
     the entry block.  That edge should always have a LATER value the
     same as EARLIEST for that edge.  */
  FOR_EACH_EDGE (e, ei, ENTRY_BLOCK_PTR_FOR_FN (cfun)->succs)
    bitmap_copy (later[(size_t) e->aux], earliest[(size_t) e->aux]);

  /* Add all the blocks to the worklist.  This prevents an early exit from
     the loop given our optimistic initialization of LATER above.  */
  auto_vec<int, 20> postorder;
  inverted_post_order_compute (&postorder);
  for (unsigned int i = 0; i < postorder.length (); ++i)
    {
      bb = BASIC_BLOCK_FOR_FN (cfun, postorder[i]);
      if (bb == EXIT_BLOCK_PTR_FOR_FN (cfun)
	  || bb == ENTRY_BLOCK_PTR_FOR_FN (cfun))
	continue;
      *qin++ = bb;
      bb->aux = bb;
    }

  /* Note that we do not use the last allocated element for our queue,
     as EXIT_BLOCK is never inserted into it. */
  qin = worklist;
  qend = &worklist[n_basic_blocks_for_fn (cfun) - NUM_FIXED_BLOCKS];
  qlen = n_basic_blocks_for_fn (cfun) - NUM_FIXED_BLOCKS;

  /* Iterate until the worklist is empty.  */
  while (qlen)
    {
      /* Take the first entry off the worklist.  */
      bb = *qout++;
      bb->aux = NULL;
      qlen--;
      if (qout >= qend)
	qout = worklist;

      /* Compute the intersection of LATERIN for each incoming edge to B.  */
      bitmap_ones (laterin[bb->index]);
      FOR_EACH_EDGE (e, ei, bb->preds)
	bitmap_and (laterin[bb->index], laterin[bb->index],
		    later[(size_t)e->aux]);

      /* Calculate LATER for all outgoing edges.  */
      FOR_EACH_EDGE (e, ei, bb->succs)
	if (bitmap_ior_and_compl (later[(size_t) e->aux],
				  earliest[(size_t) e->aux],
				  laterin[bb->index],
				  antloc[bb->index])
	    /* If LATER for an outgoing edge was changed, then we need
	       to add the target of the outgoing edge to the worklist.  */
	    && e->dest != EXIT_BLOCK_PTR_FOR_FN (cfun) && e->dest->aux == 0)
	  {
	    *qin++ = e->dest;
	    e->dest->aux = e;
	    qlen++;
	    if (qin >= qend)
	      qin = worklist;
	  }
    }

  /* Computation of insertion and deletion points requires computing LATERIN
     for the EXIT block.  We allocated an extra entry in the LATERIN array
     for just this purpose.  */
  bitmap_ones (laterin[last_basic_block_for_fn (cfun)]);
  FOR_EACH_EDGE (e, ei, EXIT_BLOCK_PTR_FOR_FN (cfun)->preds)
    bitmap_and (laterin[last_basic_block_for_fn (cfun)],
		laterin[last_basic_block_for_fn (cfun)],
		later[(size_t) e->aux]);

  clear_aux_for_edges ();
  free (worklist);
}

/* Compute the insertion and deletion points for edge based LCM.  */

static void
compute_insert_delete (struct edge_list *edge_list, sbitmap *antloc,
		       sbitmap *later, sbitmap *laterin, sbitmap *insert,
		       sbitmap *del)
{
  int x;
  basic_block bb;

  FOR_EACH_BB_FN (bb, cfun)
    bitmap_and_compl (del[bb->index], antloc[bb->index],
			laterin[bb->index]);

  for (x = 0; x < NUM_EDGES (edge_list); x++)
    {
      basic_block b = INDEX_EDGE_SUCC_BB (edge_list, x);

      if (b == EXIT_BLOCK_PTR_FOR_FN (cfun))
	bitmap_and_compl (insert[x], later[x],
			  laterin[last_basic_block_for_fn (cfun)]);
      else
	bitmap_and_compl (insert[x], later[x], laterin[b->index]);
    }
}

/* Given local properties TRANSP, ANTLOC, AVLOC, KILL return the insert and
   delete vectors for edge based LCM  and return the AVIN, AVOUT bitmap.
   map the insert vector to what edge an expression should be inserted on.  */

struct edge_list *
pre_edge_lcm_avs (int n_exprs, sbitmap *transp,
	      sbitmap *avloc, sbitmap *antloc, sbitmap *kill,
	      sbitmap *avin, sbitmap *avout,
	      sbitmap **insert, sbitmap **del)
{
  sbitmap *antin, *antout, *earliest;
  sbitmap *later, *laterin;
  struct edge_list *edge_list;
  int num_edges;

  edge_list = create_edge_list ();
  num_edges = NUM_EDGES (edge_list);

#ifdef LCM_DEBUG_INFO
  if (dump_file)
    {
      fprintf (dump_file, "Edge List:\n");
      verify_edge_list (dump_file, edge_list);
      print_edge_list (dump_file, edge_list);
      dump_bitmap_vector (dump_file, "transp", "", transp,
			  last_basic_block_for_fn (cfun));
      dump_bitmap_vector (dump_file, "antloc", "", antloc,
			  last_basic_block_for_fn (cfun));
      dump_bitmap_vector (dump_file, "avloc", "", avloc,
			  last_basic_block_for_fn (cfun));
      dump_bitmap_vector (dump_file, "kill", "", kill,
			  last_basic_block_for_fn (cfun));
    }
#endif

  /* Compute global availability.  */
  compute_available (avloc, kill, avout, avin);

  /* Compute global anticipatability.  */
  antin = sbitmap_vector_alloc (last_basic_block_for_fn (cfun), n_exprs);
  antout = sbitmap_vector_alloc (last_basic_block_for_fn (cfun), n_exprs);
  compute_antinout_edge (antloc, transp, antin, antout);

#ifdef LCM_DEBUG_INFO
  if (dump_file)
    {
      dump_bitmap_vector (dump_file, "antin", "", antin,
			  last_basic_block_for_fn (cfun));
      dump_bitmap_vector (dump_file, "antout", "", antout,
			  last_basic_block_for_fn (cfun));
    }
#endif

  /* Compute earliestness.  */
  earliest = sbitmap_vector_alloc (num_edges, n_exprs);
  compute_earliest (edge_list, n_exprs, antin, antout, avout, kill, earliest);

#ifdef LCM_DEBUG_INFO
  if (dump_file)
    dump_bitmap_vector (dump_file, "earliest", "", earliest, num_edges);
#endif

  sbitmap_vector_free (antout);
  sbitmap_vector_free (antin);

  later = sbitmap_vector_alloc (num_edges, n_exprs);

  /* Allocate an extra element for the exit block in the laterin vector.  */
  laterin = sbitmap_vector_alloc (last_basic_block_for_fn (cfun) + 1,
				  n_exprs);
  compute_laterin (edge_list, earliest, antloc, later, laterin);

#ifdef LCM_DEBUG_INFO
  if (dump_file)
    {
      dump_bitmap_vector (dump_file, "laterin", "", laterin,
			  last_basic_block_for_fn (cfun) + 1);
      dump_bitmap_vector (dump_file, "later", "", later, num_edges);
    }
#endif

  sbitmap_vector_free (earliest);

  *insert = sbitmap_vector_alloc (num_edges, n_exprs);
  *del = sbitmap_vector_alloc (last_basic_block_for_fn (cfun), n_exprs);
  bitmap_vector_clear (*insert, num_edges);
  bitmap_vector_clear (*del, last_basic_block_for_fn (cfun));
  compute_insert_delete (edge_list, antloc, later, laterin, *insert, *del);

  sbitmap_vector_free (laterin);
  sbitmap_vector_free (later);

#ifdef LCM_DEBUG_INFO
  if (dump_file)
    {
      dump_bitmap_vector (dump_file, "pre_insert_map", "", *insert, num_edges);
      dump_bitmap_vector (dump_file, "pre_delete_map", "", *del,
			  last_basic_block_for_fn (cfun));
    }
#endif

  return edge_list;
}

/* Wrapper to allocate avin/avout and call pre_edge_lcm_avs.  */

struct edge_list *
pre_edge_lcm (int n_exprs, sbitmap *transp,
	      sbitmap *avloc, sbitmap *antloc, sbitmap *kill,
	      sbitmap **insert, sbitmap **del)
{
  struct edge_list *edge_list;
  sbitmap *avin, *avout;

  avin = sbitmap_vector_alloc (last_basic_block_for_fn (cfun), n_exprs);
  avout = sbitmap_vector_alloc (last_basic_block_for_fn (cfun), n_exprs);

  edge_list = pre_edge_lcm_avs (n_exprs, transp, avloc, antloc, kill,
				 avin, avout, insert, del);

  sbitmap_vector_free (avout);
  sbitmap_vector_free (avin);

  return edge_list;
}

/* Compute the AVIN and AVOUT vectors from the AVLOC and KILL vectors.
   Return the number of passes we performed to iterate to a solution.  */

void
compute_available (sbitmap *avloc, sbitmap *kill, sbitmap *avout,
		   sbitmap *avin)
{
  edge e;
  basic_block *worklist, *qin, *qout, *qend, bb;
  unsigned int qlen;
  edge_iterator ei;

  /* Allocate a worklist array/queue.  Entries are only added to the
     list if they were not already on the list.  So the size is
     bounded by the number of basic blocks.  */
  qin = qout = worklist =
    XNEWVEC (basic_block, n_basic_blocks_for_fn (cfun) - NUM_FIXED_BLOCKS);

  /* We want a maximal solution.  */
  bitmap_vector_ones (avout, last_basic_block_for_fn (cfun));

  /* Put every block on the worklist; this is necessary because of the
     optimistic initialization of AVOUT above.  Use inverted postorder
     to make the dataflow problem require less iterations.  */
  auto_vec<int, 20> postorder;
  inverted_post_order_compute (&postorder);
  for (unsigned int i = 0; i < postorder.length (); ++i)
    {
      bb = BASIC_BLOCK_FOR_FN (cfun, postorder[i]);
      if (bb == EXIT_BLOCK_PTR_FOR_FN (cfun)
	  || bb == ENTRY_BLOCK_PTR_FOR_FN (cfun))
	continue;
      *qin++ = bb;
      bb->aux = bb;
    }

  qin = worklist;
  qend = &worklist[n_basic_blocks_for_fn (cfun) - NUM_FIXED_BLOCKS];
  qlen = n_basic_blocks_for_fn (cfun) - NUM_FIXED_BLOCKS;

  /* Mark blocks which are successors of the entry block so that we
     can easily identify them below.  */
  FOR_EACH_EDGE (e, ei, ENTRY_BLOCK_PTR_FOR_FN (cfun)->succs)
    e->dest->aux = ENTRY_BLOCK_PTR_FOR_FN (cfun);

  /* Iterate until the worklist is empty.  */
  while (qlen)
    {
      /* Take the first entry off the worklist.  */
      bb = *qout++;
      qlen--;

      if (qout >= qend)
	qout = worklist;

      /* If one of the predecessor blocks is the ENTRY block, then the
	 intersection of avouts is the null set.  We can identify such blocks
	 by the special value in the AUX field in the block structure.  */
      if (bb->aux == ENTRY_BLOCK_PTR_FOR_FN (cfun))
	/* Do not clear the aux field for blocks which are successors of the
	   ENTRY block.  That way we never add then to the worklist again.  */
	bitmap_clear (avin[bb->index]);
      else
	{
	  /* Clear the aux field of this block so that it can be added to
	     the worklist again if necessary.  */
	  bb->aux = NULL;
	  bitmap_intersection_of_preds (avin[bb->index], avout, bb);
	}

      if (bitmap_ior_and_compl (avout[bb->index], avloc[bb->index],
				    avin[bb->index], kill[bb->index]))
	/* If the out state of this block changed, then we need
	   to add the successors of this block to the worklist
	   if they are not already on the worklist.  */
	FOR_EACH_EDGE (e, ei, bb->succs)
	  if (!e->dest->aux && e->dest != EXIT_BLOCK_PTR_FOR_FN (cfun))
	    {
	      *qin++ = e->dest;
	      e->dest->aux = e;
	      qlen++;

	      if (qin >= qend)
		qin = worklist;
	    }
    }

  clear_aux_for_edges ();
  clear_aux_for_blocks ();
  free (worklist);
}

/* Compute the farthest vector for edge based lcm.  */

static void
compute_farthest (struct edge_list *edge_list, int n_exprs,
		  sbitmap *st_avout, sbitmap *st_avin, sbitmap *st_antin,
		  sbitmap *kill, sbitmap *farthest)
{
  int x, num_edges;
  basic_block pred, succ;

  num_edges = NUM_EDGES (edge_list);

  auto_sbitmap difference (n_exprs), temp_bitmap (n_exprs);
  for (x = 0; x < num_edges; x++)
    {
      pred = INDEX_EDGE_PRED_BB (edge_list, x);
      succ = INDEX_EDGE_SUCC_BB (edge_list, x);
      if (succ == EXIT_BLOCK_PTR_FOR_FN (cfun))
	bitmap_copy (farthest[x], st_avout[pred->index]);
      else
	{
	  if (pred == ENTRY_BLOCK_PTR_FOR_FN (cfun))
	    bitmap_clear (farthest[x]);
	  else
	    {
	      bitmap_and_compl (difference, st_avout[pred->index],
				  st_antin[succ->index]);
	      bitmap_not (temp_bitmap, st_avin[succ->index]);
	      bitmap_and_or (farthest[x], difference,
				    kill[succ->index], temp_bitmap);
	    }
	}
    }
}

/* Compute nearer and nearerout vectors for edge based lcm.

   This is the mirror of compute_laterin, additional comments on the
   implementation can be found before compute_laterin.  */

static void
compute_nearerout (struct edge_list *edge_list, sbitmap *farthest,
		   sbitmap *st_avloc, sbitmap *nearer, sbitmap *nearerout)
{
  int num_edges, i;
  edge e;
  basic_block *worklist, *tos, bb;
  edge_iterator ei;

  num_edges = NUM_EDGES (edge_list);

  /* Allocate a worklist array/queue.  Entries are only added to the
     list if they were not already on the list.  So the size is
     bounded by the number of basic blocks.  */
  tos = worklist = XNEWVEC (basic_block, n_basic_blocks_for_fn (cfun) + 1);

  /* Initialize NEARER for each edge and build a mapping from an edge to
     its index.  */
  for (i = 0; i < num_edges; i++)
    INDEX_EDGE (edge_list, i)->aux = (void *) (size_t) i;

  /* We want a maximal solution.  */
  bitmap_vector_ones (nearer, num_edges);

  /* Note that even though we want an optimistic setting of NEARER, we
     do not want to be overly optimistic.  Consider an incoming edge to
     the exit block.  That edge should always have a NEARER value the
     same as FARTHEST for that edge.  */
  FOR_EACH_EDGE (e, ei, EXIT_BLOCK_PTR_FOR_FN (cfun)->preds)
    bitmap_copy (nearer[(size_t)e->aux], farthest[(size_t)e->aux]);

  /* Add all the blocks to the worklist.  This prevents an early exit
     from the loop given our optimistic initialization of NEARER.  */
  FOR_EACH_BB_FN (bb, cfun)
    {
      *tos++ = bb;
      bb->aux = bb;
    }

  /* Iterate until the worklist is empty.  */
  while (tos != worklist)
    {
      /* Take the first entry off the worklist.  */
      bb = *--tos;
      bb->aux = NULL;

      /* Compute the intersection of NEARER for each outgoing edge from B.  */
      bitmap_ones (nearerout[bb->index]);
      FOR_EACH_EDGE (e, ei, bb->succs)
	bitmap_and (nearerout[bb->index], nearerout[bb->index],
			 nearer[(size_t) e->aux]);

      /* Calculate NEARER for all incoming edges.  */
      FOR_EACH_EDGE (e, ei, bb->preds)
	if (bitmap_ior_and_compl (nearer[(size_t) e->aux],
				      farthest[(size_t) e->aux],
				      nearerout[e->dest->index],
				      st_avloc[e->dest->index])
	    /* If NEARER for an incoming edge was changed, then we need
	       to add the source of the incoming edge to the worklist.  */
	    && e->src != ENTRY_BLOCK_PTR_FOR_FN (cfun) && e->src->aux == 0)
	  {
	    *tos++ = e->src;
	    e->src->aux = e;
	  }
    }

  /* Computation of insertion and deletion points requires computing NEAREROUT
     for the ENTRY block.  We allocated an extra entry in the NEAREROUT array
     for just this purpose.  */
  bitmap_ones (nearerout[last_basic_block_for_fn (cfun)]);
  FOR_EACH_EDGE (e, ei, ENTRY_BLOCK_PTR_FOR_FN (cfun)->succs)
    bitmap_and (nearerout[last_basic_block_for_fn (cfun)],
		     nearerout[last_basic_block_for_fn (cfun)],
		     nearer[(size_t) e->aux]);

  clear_aux_for_edges ();
  free (tos);
}

/* Compute the insertion and deletion points for edge based LCM.  */

static void
compute_rev_insert_delete (struct edge_list *edge_list, sbitmap *st_avloc,
			   sbitmap *nearer, sbitmap *nearerout,
			   sbitmap *insert, sbitmap *del)
{
  int x;
  basic_block bb;

  FOR_EACH_BB_FN (bb, cfun)
    bitmap_and_compl (del[bb->index], st_avloc[bb->index],
			nearerout[bb->index]);

  for (x = 0; x < NUM_EDGES (edge_list); x++)
    {
      basic_block b = INDEX_EDGE_PRED_BB (edge_list, x);
      if (b == ENTRY_BLOCK_PTR_FOR_FN (cfun))
	bitmap_and_compl (insert[x], nearer[x],
			  nearerout[last_basic_block_for_fn (cfun)]);
      else
	bitmap_and_compl (insert[x], nearer[x], nearerout[b->index]);
    }
}

/* Given local properties TRANSP, ST_AVLOC, ST_ANTLOC, KILL return the
   insert and delete vectors for edge based reverse LCM.  Returns an
   edgelist which is used to map the insert vector to what edge
   an expression should be inserted on.  */

struct edge_list *
pre_edge_rev_lcm (int n_exprs, sbitmap *transp,
		  sbitmap *st_avloc, sbitmap *st_antloc, sbitmap *kill,
		  sbitmap **insert, sbitmap **del)
{
  sbitmap *st_antin, *st_antout;
  sbitmap *st_avout, *st_avin, *farthest;
  sbitmap *nearer, *nearerout;
  struct edge_list *edge_list;
  int num_edges;

  edge_list = create_edge_list ();
  num_edges = NUM_EDGES (edge_list);

  st_antin = sbitmap_vector_alloc (last_basic_block_for_fn (cfun), n_exprs);
  st_antout = sbitmap_vector_alloc (last_basic_block_for_fn (cfun), n_exprs);
  bitmap_vector_clear (st_antin, last_basic_block_for_fn (cfun));
  bitmap_vector_clear (st_antout, last_basic_block_for_fn (cfun));
  compute_antinout_edge (st_antloc, transp, st_antin, st_antout);

  /* Compute global anticipatability.  */
  st_avout = sbitmap_vector_alloc (last_basic_block_for_fn (cfun), n_exprs);
  st_avin = sbitmap_vector_alloc (last_basic_block_for_fn (cfun), n_exprs);
  compute_available (st_avloc, kill, st_avout, st_avin);

#ifdef LCM_DEBUG_INFO
  if (dump_file)
    {
      fprintf (dump_file, "Edge List:\n");
      verify_edge_list (dump_file, edge_list);
      print_edge_list (dump_file, edge_list);
      dump_bitmap_vector (dump_file, "transp", "", transp,
			  last_basic_block_for_fn (cfun));
      dump_bitmap_vector (dump_file, "st_avloc", "", st_avloc,
			  last_basic_block_for_fn (cfun));
      dump_bitmap_vector (dump_file, "st_antloc", "", st_antloc,
			  last_basic_block_for_fn (cfun));
      dump_bitmap_vector (dump_file, "st_antin", "", st_antin,
			  last_basic_block_for_fn (cfun));
      dump_bitmap_vector (dump_file, "st_antout", "", st_antout,
			  last_basic_block_for_fn (cfun));
      dump_bitmap_vector (dump_file, "st_kill", "", kill,
			  last_basic_block_for_fn (cfun));
    }
#endif

#ifdef LCM_DEBUG_INFO
  if (dump_file)
    {
      dump_bitmap_vector (dump_file, "st_avout", "", st_avout, last_basic_block_for_fn (cfun));
      dump_bitmap_vector (dump_file, "st_avin", "", st_avin, last_basic_block_for_fn (cfun));
    }
#endif

  /* Compute farthestness.  */
  farthest = sbitmap_vector_alloc (num_edges, n_exprs);
  compute_farthest (edge_list, n_exprs, st_avout, st_avin, st_antin,
		    kill, farthest);

#ifdef LCM_DEBUG_INFO
  if (dump_file)
    dump_bitmap_vector (dump_file, "farthest", "", farthest, num_edges);
#endif

  sbitmap_vector_free (st_antin);
  sbitmap_vector_free (st_antout);

  sbitmap_vector_free (st_avin);
  sbitmap_vector_free (st_avout);

  nearer = sbitmap_vector_alloc (num_edges, n_exprs);

  /* Allocate an extra element for the entry block.  */
  nearerout = sbitmap_vector_alloc (last_basic_block_for_fn (cfun) + 1,
				    n_exprs);
  compute_nearerout (edge_list, farthest, st_avloc, nearer, nearerout);

#ifdef LCM_DEBUG_INFO
  if (dump_file)
    {
      dump_bitmap_vector (dump_file, "nearerout", "", nearerout,
			   last_basic_block_for_fn (cfun) + 1);
      dump_bitmap_vector (dump_file, "nearer", "", nearer, num_edges);
    }
#endif

  sbitmap_vector_free (farthest);

  *insert = sbitmap_vector_alloc (num_edges, n_exprs);
  *del = sbitmap_vector_alloc (last_basic_block_for_fn (cfun), n_exprs);
  compute_rev_insert_delete (edge_list, st_avloc, nearer, nearerout,
			     *insert, *del);

  sbitmap_vector_free (nearerout);
  sbitmap_vector_free (nearer);

#ifdef LCM_DEBUG_INFO
  if (dump_file)
    {
      dump_bitmap_vector (dump_file, "pre_insert_map", "", *insert, num_edges);
      dump_bitmap_vector (dump_file, "pre_delete_map", "", *del,
			   last_basic_block_for_fn (cfun));
    }
#endif
  return edge_list;
}

