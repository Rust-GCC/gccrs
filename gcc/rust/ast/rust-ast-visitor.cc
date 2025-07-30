// Copyright (C) 2020-2025 Free Software Foundation, Inc.

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

#include "rust-ast-visitor.h"
#include "rust-ast-full-decls.h"
#include "rust-ast.h"
#include "rust-path.h"
#include "rust-token.h"
#include "rust-expr.h"
#include "rust-macro.h"
#include "rust-pattern.h"
#include "rust-stmt.h"
#include "rust-type.h"

namespace Rust {
namespace AST {

void
DefaultASTVisitor::visit_function_params (AST::Function &function)
{
  for (auto &param : function.get_function_params ())
    visit (param);
}

#define VISIT_DEF(T, param_name, ...)                                          \
  void DefaultASTVisitor::visit (T &param_name) { __VA_ARGS__; }
#define VISIT_DEF_STUB(T)                                                      \
  void DefaultASTVisitor::visit (T &) {}
#define VISIT_UNESCAPE(vs)                                                     \
  {                                                                            \
    vs;                                                                        \
  }
#define VISIT_INNER_ATTRS(node) visit_inner_attrs (node)
#define VISIT_OUTER_ATTRS(node) visit_outer_attrs (node)
#define VISIT(node) visit (node)
#define VISIT_COND(cond, vs) ((cond) ? vs : (void) 0)
#define VISIT_ESCAPE(...) (([&, this] () { __VA_ARGS__ }) ())
#define VISIT_VEC(vec)                                                         \
  VISIT_ESCAPE (for (auto &visit_vec_internal                                  \
		     : (vec)) visit (visit_vec_internal);)
#define VISIT_FN_PARAMS(fn) visit_function_params (fn)

#include "rust-ast-visitable-inner.h"

#undef VISIT_DEF
#undef VISIT_DEF_STUB
#undef VISIT_UNESCAPE
#undef VISIT_INNER_ATTRS
#undef VISIT_OUTER_ATTRS
#undef VISIT
#undef VISIT_COND
#undef VISIT_ESCAPE
#undef VISIT_VEC
#undef VISIT_FN_PARAMS

void
ContextualASTVisitor::visit (AST::Crate &crate)
{
  ctx.enter (Kind::CRATE);
  DefaultASTVisitor::visit (crate);
  ctx.exit ();
}

void
ContextualASTVisitor::visit (AST::InherentImpl &impl)
{
  ctx.enter (Kind::INHERENT_IMPL);
  DefaultASTVisitor::visit (impl);
  ctx.exit ();
}

void
ContextualASTVisitor::visit (AST::TraitImpl &impl)
{
  ctx.enter (Kind::TRAIT_IMPL);
  DefaultASTVisitor::visit (impl);
  ctx.exit ();
}

void
ContextualASTVisitor::visit (AST::Trait &trait)
{
  ctx.enter (Kind::TRAIT);
  DefaultASTVisitor::visit (trait);
  ctx.exit ();
}

} // namespace AST
} // namespace Rust
