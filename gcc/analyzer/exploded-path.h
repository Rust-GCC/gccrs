/* Paths within an exploded_graph.
   Copyright (C) 2019-2026 Free Software Foundation, Inc.
   Contributed by David Malcolm <dmalcolm@redhat.com>.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#ifndef GCC_ANALYZER_EXPLODED_PATH_H
#define GCC_ANALYZER_EXPLODED_PATH_H

#include "analyzer/exploded-graph.h"

namespace ana {

/* A path within an exploded_graph: a sequence of edges.  */

class exploded_path
{
public:
  exploded_path () : m_edges () {}
  exploded_path (const exploded_path &other);

  unsigned length () const { return m_edges.length (); }

  bool find_stmt_backwards (const gimple *search_stmt,
			    int *out_idx) const;

  exploded_node *get_final_enode () const;

  void dump_to_pp (pretty_printer *pp,
		   const extrinsic_state *ext_state) const;
  void dump (FILE *fp, const extrinsic_state *ext_state) const;
  void dump (const extrinsic_state *ext_state = nullptr) const;
  void dump_to_file (const char *filename,
		     const extrinsic_state &ext_state) const;

  bool feasible_p (logger *logger, std::unique_ptr<feasibility_problem> *out,
		    engine *eng, const exploded_graph *eg) const;

  auto_vec<const exploded_edge *> m_edges;
};

/* Finding the shortest exploded_path within an exploded_graph.  */

typedef shortest_paths<eg_traits, exploded_path> shortest_exploded_paths;

} // namespace ana

#endif /* GCC_ANALYZER_EXPLODED_PATH_H */
