// Copyright (C) 2020-2022 Free Software Foundation, Inc.

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

#include "rust-tyty-call.h"
#include "rust-hir-type-check-expr.h"

namespace Rust {
namespace TyTy {

BaseType *
TypeCheckCallExpr::go (BaseType *ref, HIR::CallExpr &call,
		       TyTy::VariantDef &variant,
		       Resolver::TypeCheckContext *context)
{
  TypeCheckCallExpr checker (call, variant, context);
  ref->accept_vis (checker);
  return checker.resolved;
}

void
TypeCheckCallExpr::visit (InferType &)
{
  gcc_unreachable ();
}

void
TypeCheckCallExpr::visit (TupleType &)
{
  gcc_unreachable ();
}

void
TypeCheckCallExpr::visit (ArrayType &)
{
  gcc_unreachable ();
}

void
TypeCheckCallExpr::visit (SliceType &)
{
  gcc_unreachable ();
}

void
TypeCheckCallExpr::visit (BoolType &)
{
  gcc_unreachable ();
}

void
TypeCheckCallExpr::visit (IntType &)
{
  gcc_unreachable ();
}

void
TypeCheckCallExpr::visit (UintType &)
{
  gcc_unreachable ();
}

void
TypeCheckCallExpr::visit (FloatType &)
{
  gcc_unreachable ();
}

void
TypeCheckCallExpr::visit (USizeType &)
{
  gcc_unreachable ();
}

void
TypeCheckCallExpr::visit (ISizeType &)
{
  gcc_unreachable ();
}

void
TypeCheckCallExpr::visit (ErrorType &)
{
  gcc_unreachable ();
}

void
TypeCheckCallExpr::visit (CharType &)
{
  gcc_unreachable ();
}

void
TypeCheckCallExpr::visit (ReferenceType &)
{
  gcc_unreachable ();
}

void
TypeCheckCallExpr::visit (PointerType &)
{
  gcc_unreachable ();
}

void
TypeCheckCallExpr::visit (ParamType &)
{
  gcc_unreachable ();
}

void
TypeCheckCallExpr::visit (StrType &)
{
  gcc_unreachable ();
}

void
TypeCheckCallExpr::visit (NeverType &)
{
  gcc_unreachable ();
}

void
TypeCheckCallExpr::visit (PlaceholderType &)
{
  gcc_unreachable ();
}

void
TypeCheckCallExpr::visit (ProjectionType &)
{
  gcc_unreachable ();
}

void
TypeCheckCallExpr::visit (DynamicObjectType &)
{
  gcc_unreachable ();
}

void
TypeCheckCallExpr::visit (ClosureType &type)
{
  gcc_unreachable ();
}

void
TypeCheckCallExpr::visit (ADTType &type)
{
  rust_assert (!variant.is_error ());
  if (variant.get_variant_type () != TyTy::VariantDef::VariantType::TUPLE)
    {
      rust_error_at (
	call.get_locus (),
	"expected function, tuple struct or tuple variant, found struct %<%s%>",
	type.get_name ().c_str ());
      return;
    }

  if (call.num_params () != variant.num_fields ())
    {
      rust_error_at (call.get_locus (),
		     "unexpected number of arguments %lu expected %lu",
		     (unsigned long) call.num_params (),
		     (unsigned long) variant.num_fields ());
      return;
    }

  size_t i = 0;
  for (auto &argument : call.get_arguments ())
    {
      StructFieldType *field = variant.get_field_at_index (i);
      BaseType *field_tyty = field->get_field_type ();

      BaseType *arg = Resolver::TypeCheckExpr::Resolve (argument.get ());
      if (arg->get_kind () == TyTy::TypeKind::ERROR)
	{
	  rust_error_at (argument->get_locus (),
			 "failed to resolve argument type");
	  return;
	}

      auto res = Resolver::TypeCheckBase::coercion_site (
	argument->get_mappings ().get_hirid (), field_tyty, arg,
	argument->get_locus ());
      if (res->get_kind () == TyTy::TypeKind::ERROR)
	{
	  return;
	}

      delete res;
      i++;
    }

  if (i != call.num_params ())
    {
      rust_error_at (call.get_locus (),
		     "unexpected number of arguments %lu expected %lu",
		     (unsigned long) i, (unsigned long) call.num_params ());
      return;
    }

  resolved = type.clone ();
}

void
TypeCheckCallExpr::visit (FnType &type)
{
  type.monomorphize ();
  if (call.num_params () != type.num_params ())
    {
      if (type.is_varadic ())
	{
	  if (call.num_params () < type.num_params ())
	    {
	      rust_error_at (call.get_locus (),
			     "unexpected number of arguments %lu expected %lu",
			     (unsigned long) call.num_params (),
			     (unsigned long) type.num_params ());
	      return;
	    }
	}
      else
	{
	  rust_error_at (call.get_locus (),
			 "unexpected number of arguments %lu expected %lu",
			 (unsigned long) call.num_params (),
			 (unsigned long) type.num_params ());
	  return;
	}
    }

  size_t i = 0;
  for (auto &argument : call.get_arguments ())
    {
      auto argument_expr_tyty
	= Resolver::TypeCheckExpr::Resolve (argument.get ());
      if (argument_expr_tyty->get_kind () == TyTy::TypeKind::ERROR)
	{
	  rust_error_at (
	    argument->get_locus (),
	    "failed to resolve type for argument expr in CallExpr");
	  return;
	}

      // it might be a varadic function
      if (i < type.num_params ())
	{
	  auto fnparam = type.param_at (i);
	  auto resolved_argument_type = Resolver::TypeCheckBase::coercion_site (
	    argument->get_mappings ().get_hirid (), fnparam.second,
	    argument_expr_tyty, argument->get_locus ());
	  if (resolved_argument_type->get_kind () == TyTy::TypeKind::ERROR)
	    {
	      rust_error_at (argument->get_locus (),
			     "Type Resolution failure on parameter");
	      return;
	    }
	}

      i++;
    }

  if (i < call.num_params ())
    {
      rust_error_at (call.get_locus (),
		     "unexpected number of arguments %lu expected %lu",
		     (unsigned long) i, (unsigned long) call.num_params ());
      return;
    }

  type.monomorphize ();
  resolved = type.get_return_type ()->clone ();
}

void
TypeCheckCallExpr::visit (FnPtr &type)
{
  if (call.num_params () != type.num_params ())
    {
      rust_error_at (call.get_locus (),
		     "unexpected number of arguments %lu expected %lu",
		     (unsigned long) call.num_params (),
		     (unsigned long) type.num_params ());
      return;
    }

  size_t i = 0;
  for (auto &argument : call.get_arguments ())
    {
      auto fnparam = type.param_at (i);
      auto argument_expr_tyty
	= Resolver::TypeCheckExpr::Resolve (argument.get ());
      if (argument_expr_tyty->get_kind () == TyTy::TypeKind::ERROR)
	{
	  rust_error_at (
	    argument->get_locus (),
	    "failed to resolve type for argument expr in CallExpr");
	  return;
	}

      auto resolved_argument_type = Resolver::TypeCheckBase::coercion_site (
	argument->get_mappings ().get_hirid (), fnparam, argument_expr_tyty,
	argument->get_locus ());
      if (resolved_argument_type->get_kind () == TyTy::TypeKind::ERROR)
	{
	  rust_error_at (argument->get_locus (),
			 "Type Resolution failure on parameter");
	  return;
	}

      i++;
    }

  if (i != call.num_params ())
    {
      rust_error_at (call.get_locus (),
		     "unexpected number of arguments %lu expected %lu",
		     (unsigned long) i, (unsigned long) call.num_params ());
      return;
    }

  resolved = type.get_return_type ()->monomorphized_clone ();
}

// method call checker

// Resolve the Method parameters and return back the return type
BaseType *
TypeCheckMethodCallExpr::go (BaseType *ref, HIR::MethodCallExpr &call,
			     TyTy::BaseType *adjusted_self,
			     Resolver::TypeCheckContext *context)
{
  TypeCheckMethodCallExpr checker (call, adjusted_self, context);
  ref->accept_vis (checker);
  return checker.resolved;
}

void
TypeCheckMethodCallExpr::visit (InferType &)
{
  gcc_unreachable ();
}

void
TypeCheckMethodCallExpr::visit (TupleType &)
{
  gcc_unreachable ();
}

void
TypeCheckMethodCallExpr::visit (ArrayType &)
{
  gcc_unreachable ();
}

void
TypeCheckMethodCallExpr::visit (SliceType &)
{
  gcc_unreachable ();
}

void
TypeCheckMethodCallExpr::visit (BoolType &)
{
  gcc_unreachable ();
}

void
TypeCheckMethodCallExpr::visit (IntType &)
{
  gcc_unreachable ();
}

void
TypeCheckMethodCallExpr::visit (UintType &)
{
  gcc_unreachable ();
}

void
TypeCheckMethodCallExpr::visit (FloatType &)
{
  gcc_unreachable ();
}

void
TypeCheckMethodCallExpr::visit (USizeType &)
{
  gcc_unreachable ();
}

void
TypeCheckMethodCallExpr::visit (ISizeType &)
{
  gcc_unreachable ();
}

void
TypeCheckMethodCallExpr::visit (ErrorType &)
{
  gcc_unreachable ();
}

void
TypeCheckMethodCallExpr::visit (ADTType &)
{
  gcc_unreachable ();
};

void
TypeCheckMethodCallExpr::visit (CharType &)
{
  gcc_unreachable ();
}

void
TypeCheckMethodCallExpr::visit (ReferenceType &)
{
  gcc_unreachable ();
}

void
TypeCheckMethodCallExpr::visit (PointerType &)
{
  gcc_unreachable ();
}

void
TypeCheckMethodCallExpr::visit (ParamType &)
{
  gcc_unreachable ();
}

void
TypeCheckMethodCallExpr::visit (StrType &)
{
  gcc_unreachable ();
}

void
TypeCheckMethodCallExpr::visit (NeverType &)
{
  gcc_unreachable ();
}

void
TypeCheckMethodCallExpr::visit (PlaceholderType &)
{
  gcc_unreachable ();
}

void
TypeCheckMethodCallExpr::visit (ProjectionType &)
{
  gcc_unreachable ();
}

void
TypeCheckMethodCallExpr::visit (DynamicObjectType &)
{
  gcc_unreachable ();
}

// FIXME
void
TypeCheckMethodCallExpr::visit (FnPtr &type)
{
  gcc_unreachable ();
}

void
TypeCheckMethodCallExpr::visit (ClosureType &type)
{
  gcc_unreachable ();
}

void
TypeCheckMethodCallExpr::visit (FnType &type)
{
  type.get_self_type ()->unify (adjusted_self);

  // +1 for the receiver self
  size_t num_args_to_call = call.num_params () + 1;
  if (num_args_to_call != type.num_params ())
    {
      rust_error_at (call.get_locus (),
		     "unexpected number of arguments %lu expected %lu",
		     (unsigned long) call.num_params (),
		     (unsigned long) type.num_params ());
      return;
    }

  size_t i = 1;
  for (auto &argument : call.get_arguments ())
    {
      auto fnparam = type.param_at (i);
      auto argument_expr_tyty
	= Resolver::TypeCheckExpr::Resolve (argument.get ());
      if (argument_expr_tyty->get_kind () == TyTy::TypeKind::ERROR)
	{
	  rust_error_at (
	    argument->get_locus (),
	    "failed to resolve type for argument expr in CallExpr");
	  return;
	}

      auto resolved_argument_type = Resolver::TypeCheckBase::coercion_site (
	argument->get_mappings ().get_hirid (), fnparam.second,
	argument_expr_tyty, argument->get_locus ());
      if (resolved_argument_type->get_kind () == TyTy::TypeKind::ERROR)
	{
	  rust_error_at (argument->get_locus (),
			 "Type Resolution failure on parameter");
	  return;
	}

      i++;
    }

  if (i != num_args_to_call)
    {
      rust_error_at (call.get_locus (),
		     "unexpected number of arguments %lu expected %lu",
		     (unsigned long) i, (unsigned long) call.num_params ());
      return;
    }

  type.monomorphize ();

  resolved = type.get_return_type ()->monomorphized_clone ();
}

} // namespace TyTy
} // namespace Rust
