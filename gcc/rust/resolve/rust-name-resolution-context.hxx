// Copyright (C) 2026 Free Software Foundation, Inc.

// This file is part of GCC.

// GCC is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 3, or (at your option) any later
// version.

// GCC is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.

// You should have received a copy of the GNU General Public License
// along with GCC; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

#include "rust-name-resolution-context.h"

/**
 * Split the actual path resolution logic in its own file because it's a lot,
 * and it could get even worse for certain edge cases.
 */

namespace Rust {
namespace Resolver2_0 {

template <Namespace N>
tl::optional<Rib::Definition>
NameResolutionContext::resolve_path (
  ForeverStack<N> &stack, const ResolutionPath &path, ResolutionMode mode,
  std::function<void (Usage, Definition)> insert_segment_resolution,
  std::vector<Error> &collect_errors)
{
  std::reference_wrapper<typename ForeverStack<N>::Node> starting_point
    = stack.cursor ();

  return NameResolutionContext::resolve_path (stack, path, mode,
					      insert_segment_resolution,
					      collect_errors, starting_point);
}

template <Namespace N>
tl::optional<Rib::Definition>
NameResolutionContext::resolve_path (
  ForeverStack<N> &stack, const ResolutionPath &path, ResolutionMode mode,
  std::function<void (Usage, Definition)> insert_segment_resolution,
  std::vector<Error> &collect_errors, NodeId starting_point_id)

{
  auto starting_point = stack.dfs_node (stack.root, starting_point_id);

  // We may have a prelude, but haven't visited it yet and thus it's not in
  // our nodes
  if (!starting_point)
    return tl::nullopt;

  return NameResolutionContext::resolve_path (stack, path, mode,
					      insert_segment_resolution,
					      collect_errors, *starting_point);
}

template <Namespace N>
tl::optional<Rib::Definition>
NameResolutionContext::resolve_path (
  ForeverStack<N> &stack, const ResolutionPath &path, ResolutionMode mode,
  std::function<void (Usage, Definition)> insert_segment_resolution,
  std::vector<Error> &collect_errors,
  std::reference_wrapper<typename ForeverStack<N>::Node> starting_point)
{
  bool can_descend = true;

  rust_debug ("resolving %s", path.as_string ().c_str ());

  if (auto lang_item = path.get_lang_prefix ())
    {
      NodeId seg_id
	= Analysis::Mappings::get ().get_lang_item_node (lang_item->first);

      insert_segment_resolution (Usage (lang_item->second),
				 Definition (seg_id));

      if (path.get_segments ().empty ())
	return Rib::Definition::NonShadowable (seg_id);

      auto new_start = stack.dfs_node (stack.root, seg_id);
      rust_assert (new_start.has_value ());
      starting_point = new_start.value ();

      can_descend = false;
    }
  else
    {
      switch (mode)
	{
	case ResolutionMode::Normal:
	  break; // default
	case ResolutionMode::FromRoot:
	  starting_point = stack.root;
	  break;
	case ResolutionMode::FromExtern:
	  starting_point = stack.extern_prelude;
	  break;
	default:
	  rust_unreachable ();
	}
    }

  if (path.get_segments ().empty ())
    return Rib::Definition::NonShadowable (starting_point.get ().id);

  auto &segments = path.get_segments ();

  // if there's only one segment, we just use `get`
  if (can_descend && segments.size () == 1)
    {
      auto &seg = segments.front ();

      tl::optional<Rib::Definition> res
	= stack.get (starting_point.get (), seg.name);

      if (!res)
	res = stack.get_lang_prelude (seg.name);

      if (N == Namespace::Types && !res)
	{
	  if (seg.is_crate_path_seg ())
	    {
	      insert_segment_resolution (Usage (seg.node_id),
					 Definition (stack.root.id));
	      // TODO: does NonShadowable matter?
	      return Rib::Definition::NonShadowable (stack.root.id);
	    }
	  else if (seg.is_lower_self_seg ())
	    {
	      NodeId id = stack.find_closest_module (starting_point.get ()).id;
	      insert_segment_resolution (Usage (seg.node_id), Definition (id));
	      // TODO: does NonShadowable matter?
	      return Rib::Definition::NonShadowable (id);
	    }
	  else if (seg.is_super_path_seg ())
	    {
	      auto &closest_module
		= stack.find_closest_module (starting_point.get ());
	      if (closest_module.is_root ())
		{
		  rust_error_at (seg.locus, ErrorCode::E0433,
				 "too many leading %<super%> keywords");
		  return tl::nullopt;
		}

	      NodeId id
		= stack.find_closest_module (closest_module.parent.value ()).id;
	      insert_segment_resolution (Usage (seg.node_id), Definition (id));
	      // TODO: does NonShadowable matter?
	      return Rib::Definition::NonShadowable (id);
	    }
	  else
	    {
	      // HACK: check for a module after we check the language prelude
	      for (auto &kv :
		   stack.find_closest_module (starting_point.get ()).children)
		{
		  auto &link = kv.first;

		  if (link.path.map_or (
			[&seg] (Identifier path) {
			  auto &path_str = path.as_string ();
			  return path_str == seg.name;
			},
			false))
		    {
		      insert_segment_resolution (Usage (seg.node_id),
						 Definition (kv.second.id));
		      return Rib::Definition::NonShadowable (kv.second.id);
		    }
		}
	    }
	}

      if (res && !res->is_ambiguous ())
	insert_segment_resolution (Usage (seg.node_id),
				   Definition (res->get_node_id ()));
      return res;
    }

  auto iterator = segments.begin ();
  if (can_descend)
    {
      if (auto res = stack.find_starting_point (segments, starting_point,
						insert_segment_resolution,
						collect_errors))
	iterator = *res;
      else
	return tl::nullopt;
    }

  return stack
    .resolve_segments (starting_point.get (), segments, iterator,
		       insert_segment_resolution, collect_errors)
    .and_then ([&segments, &insert_segment_resolution,
		&stack] (typename ForeverStack<N>::Node &final_node)
		 -> tl::optional<Rib::Definition> {
      // leave resolution within impl blocks to type checker
      if (final_node.rib.kind == Rib::Kind::TraitOrImpl)
	return tl::nullopt;

      auto &seg = segments.back ();
      std::string seg_name = seg.name;

      tl::optional<Rib::Definition> res
	= stack.resolve_final_segment (final_node, seg_name,
				       seg.is_lower_self_seg ());
      // Ok we didn't find it in the rib, Lets try the prelude...
      if (!res)
	res = stack.get_lang_prelude (seg_name);

      if (N == Namespace::Types && !res)
	{
	  // HACK: check for a module after we check the language prelude
	  for (auto &kv : final_node.children)
	    {
	      auto &link = kv.first;

	      if (link.path.map_or (
		    [&seg_name] (Identifier path) {
		      auto &path_str = path.as_string ();
		      return path_str == seg_name;
		    },
		    false))
		{
		  insert_segment_resolution (Usage (seg.node_id),
					     Definition (kv.second.id));
		  return Rib::Definition::NonShadowable (kv.second.id);
		}
	    }
	}

      if (res && !res->is_ambiguous ())
	insert_segment_resolution (Usage (seg.node_id),
				   Definition (res->get_node_id ()));

      return res;
    });
}

} // namespace Resolver2_0
} // namespace Rust
