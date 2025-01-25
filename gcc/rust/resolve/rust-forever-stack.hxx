// Copyright (C) 2020-2024 Free Software Foundation, Inc.

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

#include "expected.h"
#include "rust-ast.h"
#include "rust-diagnostics.h"
#include "rust-forever-stack.h"
#include "rust-rib.h"
#include "optional.h"

namespace Rust {
namespace Resolver2_0 {

template <Namespace N>
void
ForeverStack<N>::push (Rib::Kind rib_kind, NodeId id,
		       tl::optional<Identifier> path)
{
  update_cursor (cursor ().insert_child (id, std::move (path), rib_kind));
}

template <Namespace N>
void
ForeverStack<N>::pop ()
{
  rust_assert (!cursor ().is_root ());

  rust_debug ("popping link");

  for (const auto &kv : cursor ().get_rib (N).get_values ())
    rust_debug ("current_rib: k: %s, v: %s", kv.first.c_str (),
		kv.second.to_string ().c_str ());

  if (cursor ().get_parent ().has_value ())
    {
      auto &parent = cursor ().get_parent ().value ();
      auto &rib_values = parent.get_rib (N).get_values ();
      for (const auto &kv : rib_values)
	rust_debug ("new cursor: k: %s, v: %s", kv.first.c_str (),
		    kv.second.to_string ().c_str ());
    }

  update_cursor (cursor ().get_parent ().value ());
}

template <Namespace N>
tl::expected<NodeId, DuplicateNameError>
ForeverStack<N>::insert (Identifier name, NodeId node)
{
  // Handle Macros and Labels - where we are allowed to shadow
  // existing definitions
  if (N == Namespace::Labels || N == Namespace::Macros)
    return cursor ().insert_shadowable (name, node, N);
  else
    return cursor ().insert (name, node, N);
}

template <Namespace N>
tl::expected<NodeId, DuplicateNameError>
ForeverStack<N>::insert_shadowable (Identifier name, NodeId node)
{
  return cursor ().insert_shadowable (name, node, N);
}

template <Namespace N>
tl::expected<NodeId, DuplicateNameError>
ForeverStack<N>::insert_globbed (Identifier name, NodeId node)
{
  return cursor ().insert_globbed (name, node, N);
}

template <Namespace N>
tl::expected<NodeId, DuplicateNameError>
ForeverStack<N>::insert_at_root (Identifier name, NodeId node)
{
  // inserting in the root of the crate is never a shadowing operation, even for
  // macros
  return base.get ().get_root ().insert_shadowable (name, node, N);
}

template <Namespace N>
Rib &
ForeverStack<N>::peek ()
{
  return cursor ().get_rib (N);
}

template <Namespace N>
const Rib &
ForeverStack<N>::peek () const
{
  return cursor ().get_rib (N);
}

template <Namespace N>
void
ForeverStack<N>::reverse_iter (std::function<KeepGoing (Node &)> lambda)
{
  cursor ().reverse_iter (std::move (lambda));
}

template <Namespace N>
void
ForeverStack<N>::reverse_iter (
  std::function<KeepGoing (const Node &)> lambda) const
{
  cursor ().reverse_iter (std::move (lambda));
}

template <Namespace N>
void
ForeverStack<N>::reverse_iter (Node &start,
			       std::function<KeepGoing (Node &)> lambda)
{
  start.reverse_iter (std::move (lambda));
}

template <Namespace N>
void
ForeverStack<N>::reverse_iter (
  const Node &start, std::function<KeepGoing (const Node &)> lambda) const
{
  start.reverse_iter (std::move (lambda));
}

template <Namespace N>
typename ForeverStack<N>::Node &
ForeverStack<N>::cursor ()
{
  return cursor_reference;
}

template <Namespace N>
const typename ForeverStack<N>::Node &
ForeverStack<N>::cursor () const
{
  return cursor_reference;
}

template <Namespace N>
void
ForeverStack<N>::update_cursor (Node &new_cursor)
{
  cursor_reference = new_cursor;
}

template <Namespace N>
tl::optional<Rib::Definition>
ForeverStack<N>::get (const Identifier &name)
{
  tl::optional<Rib::Definition> resolved_definition = tl::nullopt;

  // TODO: Can we improve the API? have `reverse_iter` return an optional?
  reverse_iter ([&resolved_definition, &name] (Node &current) {
    auto candidate = current.get_rib (N).get (name.as_string ());

    return candidate.map_or (
      [&resolved_definition] (Rib::Definition found) {
	// for most namespaces, we do not need to care about various ribs - they
	// are available from all contexts if defined in the current scope, or
	// an outermore one. so if we do have a candidate, we can return it
	// directly and stop iterating
	resolved_definition = found;

	return KeepGoing::No;
      },
      // if there was no candidate, we keep iterating
      KeepGoing::Yes);
  });

  return resolved_definition;
}

template <>
tl::optional<Rib::Definition> inline ForeverStack<Namespace::Labels>::get (
  const Identifier &name)
{
  tl::optional<Rib::Definition> resolved_definition = tl::nullopt;

  reverse_iter ([&resolved_definition, &name] (Node &current) {
    // looking up for labels cannot go through function ribs
    // TODO: What other ribs?
    if (current.get_rib (Namespace::Labels).kind == Rib::Kind::Function)
      return KeepGoing::No;

    auto candidate
      = current.get_rib (Namespace::Labels).get (name.as_string ());

    // FIXME: Factor this in a function with the generic `get`
    return candidate.map_or (
      [&resolved_definition] (Rib::Definition found) {
	resolved_definition = found;

	return KeepGoing::No;
      },
      KeepGoing::Yes);
  });

  return resolved_definition;
}

/* Check if an iterator points to the last element */
template <typename I, typename C>
static bool
is_last (const I &iterator, const C &collection)
{
  return iterator + 1 == collection.end ();
}

/* Check if an iterator points to the start of the collection */
template <typename I, typename C>
static bool
is_start (const I &iterator, const C &collection)
{
  return iterator == collection.begin ();
}

template <Namespace N>
typename ForeverStack<N>::Node &
ForeverStack<N>::find_closest_module (Node &starting_point)
{
  return starting_point.find_closest_module ();
}

/* If a the given condition is met, emit an error about misused leading path
 * segments */
template <typename S>
static inline bool
check_leading_kw_at_start (const S &segment, bool condition)
{
  if (condition)
    rust_error_at (
      segment.get_locus (), ErrorCode::E0433,
      "leading path segment %qs can only be used at the beginning of a path",
      segment.as_string ().c_str ());

  return condition;
}

// we first need to handle the "starting" segments - `super`, `self` or
// `crate`. we don't need to do anything for `self` and can just skip it. for
// `crate`, we need to go back to the root of the current stack. for each
// `super` segment, we go back to the cursor's parent until we reach the
// correct one or the root.
template <Namespace N>
template <typename S>
tl::optional<typename std::vector<S>::const_iterator>
ForeverStack<N>::find_starting_point (
  const std::vector<S> &segments, std::reference_wrapper<Node> &starting_point)
{
  auto iterator = segments.begin ();

  // If we need to do path segment resolution, then we start
  // at the closest module. In order to resolve something like `foo::bar!()`, we
  // need to get back to the surrounding module, and look for a child module
  // named `foo`.
  if (segments.size () > 1)
    starting_point = find_closest_module (starting_point);

  for (; !is_last (iterator, segments); iterator++)
    {
      auto &seg = *iterator;
      auto is_self_or_crate
	= seg.is_crate_path_seg () || seg.is_lower_self_seg ();

      // if we're after the first path segment and meet `self` or `crate`, it's
      // an error - we should only be seeing `super` keywords at this point
      if (check_leading_kw_at_start (seg, !is_start (iterator, segments)
					    && is_self_or_crate))
	return tl::nullopt;

      if (seg.is_crate_path_seg ())
	{
	  starting_point = base.get ().get_root ();
	  iterator++;
	  break;
	}
      if (seg.is_lower_self_seg ())
	{
	  // do nothing and exit
	  iterator++;
	  break;
	}
      if (seg.is_super_path_seg ())
	{
	  if (starting_point.get ().is_root ())
	    {
	      rust_error_at (seg.get_locus (), ErrorCode::E0433,
			     "too many leading %<super%> keywords");
	      return tl::nullopt;
	    }

	  starting_point = find_closest_module (
	    starting_point.get ().get_parent ().value ());
	  continue;
	}

      // now we've gone through the allowed `crate`, `self` or leading `super`
      // segments. we can start resolving each segment itself.
      // if we do see another leading segment, then we can error out.
      break;
    }

  return iterator;
}

template <Namespace N>
template <typename S>
tl::optional<typename ForeverStack<N>::Node &>
ForeverStack<N>::resolve_segments (
  Node &starting_point, const std::vector<S> &segments,
  typename std::vector<S>::const_iterator iterator)
{
  auto *current_node = &starting_point;
  for (; !is_last (iterator, segments); iterator++)
    {
      auto &seg = *iterator;
      auto str = seg.as_string ();
      rust_debug ("[ARTHUR]: resolving segment part: %s", str.c_str ());

      // check that we don't encounter *any* leading keywords afterwards
      if (check_leading_kw_at_start (seg, seg.is_crate_path_seg ()
					    || seg.is_super_path_seg ()
					    || seg.is_lower_self_seg ()))
	return tl::nullopt;

      auto child = current_node->get_child (
	Identifier (seg.as_string (), seg.get_locus ()));

      if (!child.has_value ())
	{
	  rust_error_at (seg.get_locus (), ErrorCode::E0433,
			 "failed to resolve path segment %qs", str.c_str ());
	  return tl::nullopt;
	}

      current_node = &child.value ();
    }

  return *current_node;
}

template <Namespace N>
template <typename S>
tl::optional<Rib::Definition>
ForeverStack<N>::resolve_path (const std::vector<S> &segments)
{
  // TODO: What to do if segments.empty() ?

  // if there's only one segment, we just use `get`
  if (segments.size () == 1)
    return get (segments.back ().as_string ());

  std::reference_wrapper<Node> starting_point = cursor ();

  return find_starting_point (segments, starting_point)
    .and_then ([this, &segments, &starting_point] (
		 typename std::vector<S>::const_iterator iterator) {
      return resolve_segments (starting_point.get (), segments, iterator);
    })
    .and_then ([&segments] (Node final_node) {
      return final_node.get_rib (N).get (segments.back ().as_string ());
    });
}

template <Namespace N>
tl::optional<typename ForeverStack<N>::DfsResult>
ForeverStack<N>::dfs (ForeverStack<N>::Node &starting_point, NodeId to_find)
{
  auto values = starting_point.get_rib (N).get_values ();

  for (auto &kv : values)
    {
      for (auto id : kv.second.ids_shadowable)
	if (id == to_find)
	  return {{starting_point, kv.first}};
      for (auto id : kv.second.ids_non_shadowable)
	if (id == to_find)
	  return {{starting_point, kv.first}};
      for (auto id : kv.second.ids_globbed)
	if (id == to_find)
	  return {{starting_point, kv.first}};
    }

  tl::optional<ForeverStackStore::DfsResult> ret;
  starting_point.child_iter (
    [this, &ret, to_find] (NodeId id, tl::optional<const Identifier &> path,
			   Node &child) {
      auto candidate = dfs (child, to_find);

      if (candidate.has_value ())
	{
	  ret.emplace (std::move (candidate.value ()));
	  return KeepGoing::No;
	}
      else
	{
	  return KeepGoing::Yes;
	}
    });

  return ret;
}

template <Namespace N>
tl::optional<typename ForeverStack<N>::ConstDfsResult>
ForeverStack<N>::dfs (const ForeverStack<N>::Node &starting_point,
		      NodeId to_find) const
{
  auto values = starting_point.get_rib (N).get_values ();

  for (auto &kv : values)
    {
      for (auto id : kv.second.ids_shadowable)
	if (id == to_find)
	  return {{starting_point, kv.first}};
      for (auto id : kv.second.ids_non_shadowable)
	if (id == to_find)
	  return {{starting_point, kv.first}};
      for (auto id : kv.second.ids_globbed)
	if (id == to_find)
	  return {{starting_point, kv.first}};
    }

  tl::optional<ForeverStackStore::ConstDfsResult> ret;
  starting_point.child_iter (
    [this, &ret, to_find] (NodeId id, tl::optional<const Identifier &> path,
			   const Node &child) {
      auto candidate = dfs (child, to_find);

      if (candidate.has_value ())
	{
	  ret.emplace (std::move (candidate.value ()));
	  return KeepGoing::No;
	}
      else
	{
	  return KeepGoing::Yes;
	}
    });

  return ret;
}

template <Namespace N>
tl::optional<Resolver::CanonicalPath>
ForeverStack<N>::to_canonical_path (NodeId id) const
{
  // find the id in the current forever stack, starting from the root,
  // performing either a BFS or DFS once the Node containing the ID is found, go
  // back up to the root (parent().parent().parent()...) accumulate link
  // segments reverse them that's your canonical path

  return dfs (base.get ().get_root (), id)
    .map ([this, id] (ConstDfsResult tuple) {
      auto containing_node = tuple.first;
      auto name = tuple.second;

      auto segments = std::vector<Resolver::CanonicalPath> ();

      reverse_iter (containing_node, [&segments] (const Node &current) {
	if (current.is_root ())
	  return KeepGoing::No;

	auto link_path = current.get_parent_path ();
	if (link_path.has_value ())
	  segments.emplace (segments.begin (),
			    Resolver::CanonicalPath::new_seg (
			      current.get_id (), link_path->as_string ()));

	return KeepGoing::Yes;
      });

      auto path = Resolver::CanonicalPath::create_empty ();
      for (const auto &segment : segments)
	path = path.append (segment);

      // Finally, append the name
      path = path.append (Resolver::CanonicalPath::new_seg (id, name));

      return path;
    });
}

template <Namespace N>
tl::optional<Rib &>
ForeverStack<N>::dfs_rib (ForeverStack<N>::Node &starting_point, NodeId to_find)
{
  return dfs_node (starting_point, to_find).map ([] (Node &x) -> Rib & {
    return x.get_rib (N);
  });
}

template <Namespace N>
tl::optional<const Rib &>
ForeverStack<N>::dfs_rib (const ForeverStack<N>::Node &starting_point,
			  NodeId to_find) const
{
  return dfs_node (starting_point, to_find).map ([] (Node &x) -> Rib & {
    return x.get_rib (N);
  });
}

template <Namespace N>
tl::optional<typename ForeverStack<N>::Node &>
ForeverStack<N>::dfs_node (ForeverStack<N>::Node &starting_point,
			   NodeId to_find)
{
  return starting_point.dfs_node (to_find);
}

template <Namespace N>
tl::optional<const typename ForeverStack<N>::Node &>
ForeverStack<N>::dfs_node (const ForeverStack<N>::Node &starting_point,
			   NodeId to_find) const
{
  return starting_point.dfs_node (to_find);
}

template <Namespace N>
tl::optional<Rib &>
ForeverStack<N>::to_rib (NodeId rib_id)
{
  return dfs_rib (base.get ().get_root (), rib_id);
}

template <Namespace N>
tl::optional<const Rib &>
ForeverStack<N>::to_rib (NodeId rib_id) const
{
  return dfs_rib (base.get ().get_root (), rib_id);
}

template <Namespace N>
void
ForeverStack<N>::stream_rib (std::stringstream &stream, const Rib &rib,
			     const std::string &next,
			     const std::string &next_next)
{
  if (rib.get_values ().empty ())
    {
      stream << next << "rib: {},\n";
      return;
    }

  stream << next << "rib: {\n";

  for (const auto &kv : rib.get_values ())
    stream << next_next << kv.first << ": " << kv.second.to_string () << "\n";

  stream << next << "},\n";
}

template <Namespace N>
void
ForeverStack<N>::stream_node (std::stringstream &stream, unsigned indentation,
			      const ForeverStack<N>::Node &node)
{
  auto indent = std::string (indentation, ' ');
  auto next = std::string (indentation + 4, ' ');
  auto next_next = std::string (indentation + 8, ' ');

  stream << indent << "Node {\n"
	 << next << "is_root: " << (node.is_root () ? "true" : "false") << ",\n"
	 << next << "is_leaf: " << (node.is_leaf () ? "true" : "false")
	 << ",\n";

  stream_rib (stream, node.get_rib (N), next, next_next);

  stream << indent << "}\n";

  node.child_iter ([this, &stream, &indent,
		    indentation] (NodeId id,
				  tl::optional<const Identifier &> path,
				  const Node &child) {
    stream << indent << "Link (" << id << ", "
	   << (path.has_value () ? path.value ().as_string () : "<anon>")
	   << "):\n";

    stream_node (stream, indentation + 4, child);

    stream << '\n';

    return KeepGoing::Yes;
  });
}

template <Namespace N>
std::string
ForeverStack<N>::as_debug_string ()
{
  std::stringstream stream;

  stream_node (stream, 0, base.get ().get_root ());

  return stream.str ();
}

template <Namespace N>
bool
ForeverStack<N>::is_module_descendant (NodeId parent, NodeId child) const
{
  return dfs_node (dfs_node (base.get ().get_root (), parent).value (), child)
    .has_value ();
}

// FIXME: Can we add selftests?

} // namespace Resolver2_0
} // namespace Rust
