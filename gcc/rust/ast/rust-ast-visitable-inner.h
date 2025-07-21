// Copyright (C) 2025 Free Software Foundation, Inc.

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

// This header is used to generate visitors for AST nodes

// The following is a description of the various x macros
//   used in this file. This file attempts to use these x macros
//   in a fairly well defined way, so that consumers of this file
//   can be creative in their definitions of these macros.
//
// You must define:
//   VISIT_DEF (T, parameter_name, ...)
//     can assume T is a type
//     can assume varargs parameter consists only of
//       comma separated "visit statement" macros
//     can assume varargs parameter is not empty
//     expected to define a visitor which binds a node
//       as parameter_name, iteratively applying the visit
//       statement macros given as varargs
//   VISIT_DEF_STUB (T)
//     can assume T is a type
//     expected to define a no-op visitor
//   VISIT_UNESCAPE (vs)
//     can assume vs is a "visit statement"
//     only present inside the body of a VISIT_ESCAPE
//     used to reenter "visit statement" mode
//     must expand to a single statement (including any semicolon)
//
// As well as the following "visit statement" macros:
//
//   VISIT_INNER_ATTRS (node)
//     node could be any expression
//     expected to visit node's inner attributes
//   VISIT_OUTER_ATTRS (node)
//     node could be any expression
//     expected to visit node's outer attributes
//   VISIT (node)
//     node could be any expression
//     expected to visit node
//   VISIT_COND (cond, vs)
//     cond could be any expression
//     can assume vs is a "visit statement" macro
//     expected to run vs only if cond is true
//   VISIT_VEC (vec)
//     vec could be any expression
//     expected to iteratively visit entries in vec
//   VISIT_FN_PARAMS (fn)
//     used as an alternative to VISIT_VEC (fn.get_function_params ())
//     makes hooking the visit of function parameters easier
//   VISIT_ESCAPE (...)
//     run C++ code provided in varargs variable
//     escape hatch for *cough* InlineAsm *cough*
//     reentry to "visit statement* macros using
//       VISIT_UNESCAPE
//
// In the special case of VISIT_DEF ignoring its varargs
//   parameter, "visit statement" macros don't have to be
//   defined.
//
// If VISIT_ESCAPE ignores its argument or is left undefined
//   (see prior paragraph) VISIT_UNESCAPE doesn't have to
//   be defined.

// clang doesn't format this very well
// TODO: use https://clang.llvm.org/docs/ClangFormatStyleOptions.html#macros
//       when we move to clang-format-17
// clang-format off

VISIT_DEF (Crate, crate,
  VISIT_INNER_ATTRS (crate),
  VISIT_VEC (crate.items)
)

VISIT_DEF_STUB (Token)

VISIT_DEF (DelimTokenTree, delim_tok_tree,
  VISIT_VEC (delim_tok_tree.get_token_trees ())
)

VISIT_DEF (AttrInputMetaItemContainer, input,
  VISIT_VEC (input.get_items ())
)

VISIT_DEF (IdentifierExpr, ident_expr,
  VISIT_OUTER_ATTRS (ident_expr)
)

VISIT_DEF_STUB (Lifetime)

VISIT_DEF (LifetimeParam, lifetime_param,
  VISIT_OUTER_ATTRS (lifetime_param),
  VISIT (lifetime_param.get_lifetime ()),
  VISIT_VEC (lifetime_param.get_lifetime_bounds ())
)

VISIT_DEF (ConstGenericParam, const_param,
  VISIT_OUTER_ATTRS (const_param),
  VISIT_COND (const_param.has_type (), VISIT (const_param.get_type ())),
  VISIT_COND (const_param.has_default_value (), VISIT (const_param.get_default_value_unchecked ()))
)

VISIT_DEF (PathInExpression, path,
  VISIT_OUTER_ATTRS (path),
  VISIT_COND (!path.is_lang_item (),
    VISIT_VEC (path.get_segments ()))
)

VISIT_DEF_STUB (TypePathSegment)

VISIT_DEF (GenericArgsBinding, binding,
  VISIT (binding.get_type ())
)

VISIT_DEF (TypePathSegmentGeneric, segment,
  VISIT_COND (segment.has_generic_args (),
    VISIT (segment.get_generic_args ()))
)

VISIT_DEF (TypePathFunction, tpf,
  VISIT_VEC (tpf.get_params ()),
  VISIT_COND (tpf.has_return_type (),
    VISIT (tpf.get_return_type ()))
)

VISIT_DEF_STUB (PathIdentSegment)

VISIT_DEF (TypePathSegmentFunction, segment,
  VISIT (segment.get_type_path_function ()),
  VISIT (segment.get_ident_segment ())
)

VISIT_DEF (GenericArgs, args,
  VISIT_VEC (args.get_lifetime_args ()),
  VISIT_VEC (args.get_generic_args ()),
  VISIT_VEC (args.get_binding_args ())
)

VISIT_DEF (PathExprSegment, segment,
  VISIT (segment.get_ident_segment ()),
  VISIT_COND (segment.has_generic_args (),
    VISIT (segment.get_generic_args ()))
)

VISIT_DEF (TypePath, path,
  VISIT_VEC (path.get_segments ())
)

VISIT_DEF (QualifiedPathInExpression, path,
  VISIT_OUTER_ATTRS (path),
  VISIT (path.get_qualified_path_type ()),
  VISIT_VEC (path.get_segments ())
)

VISIT_DEF (QualifiedPathType, path,
  VISIT (path.get_type ()),
  VISIT_COND (path.has_as_clause (), VISIT (path.get_as_type_path ()))
)

VISIT_DEF (QualifiedPathInType, path,
  VISIT (path.get_qualified_path_type ()),
  VISIT (path.get_associated_segment ()),
  VISIT_VEC (path.get_segments ())
)

VISIT_DEF (LiteralExpr, expr,
  VISIT_OUTER_ATTRS (expr)
)

VISIT_DEF (AttrInputLiteral, attr_input,
  VISIT (attr_input.get_literal ())
)

VISIT_DEF (AttrInputMacro, attr_input,
  VISIT (attr_input.get_macro ())
)

VISIT_DEF (MetaItemLitExpr, meta_item,
  VISIT (meta_item.get_literal ())
)

VISIT_DEF_STUB (SimplePathSegment)

VISIT_DEF (SimplePath, path,
  VISIT_VEC (path.get_segments ())
)

VISIT_DEF (MetaItemPathLit, meta_item,
  VISIT (meta_item.get_path ()),
  VISIT (meta_item.get_literal ())
)

VISIT_DEF (BorrowExpr, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT (expr.get_borrowed_expr ())
)

VISIT_DEF (DereferenceExpr, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT (expr.get_dereferenced_expr ())
)

VISIT_DEF (ErrorPropagationExpr, expr,
  VISIT_OUTER_ATTRS (expr)
  // TODO: visit more child nodes
)

VISIT_DEF (NegationExpr, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT (expr.get_negated_expr ())
)

VISIT_DEF (ArithmeticOrLogicalExpr, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT (expr.get_left_expr ()),
  VISIT (expr.get_right_expr ())
)

VISIT_DEF (ComparisonExpr, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT (expr.get_left_expr ()),
  VISIT (expr.get_right_expr ())
)

VISIT_DEF (LazyBooleanExpr, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT (expr.get_left_expr ()),
  VISIT (expr.get_right_expr ())
)

VISIT_DEF (TypeCastExpr, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT (expr.get_casted_expr ()),
  VISIT (expr.get_type_to_cast_to ())
)

VISIT_DEF (AssignmentExpr, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT (expr.get_left_expr ()),
  VISIT (expr.get_right_expr ())
)

VISIT_DEF (CompoundAssignmentExpr, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT (expr.get_left_expr ()),
  VISIT (expr.get_right_expr ())
)

VISIT_DEF (GroupedExpr, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT_INNER_ATTRS (expr),
  VISIT (expr.get_expr_in_parens ())
)

VISIT_DEF (ArrayElemsValues, elems,
  VISIT_VEC (elems.get_values ())
)

VISIT_DEF (ArrayElemsCopied, elems,
  VISIT (elems.get_elem_to_copy ()),
  VISIT (elems.get_num_copies ())
)

VISIT_DEF (ArrayExpr, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT_INNER_ATTRS (expr),
  VISIT (expr.get_array_elems ())
)

VISIT_DEF (ArrayIndexExpr, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT (expr.get_array_expr ()),
  VISIT (expr.get_index_expr ())
)

VISIT_DEF (TupleExpr, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT_INNER_ATTRS (expr),
  VISIT_VEC (expr.get_tuple_elems ())
)

VISIT_DEF (TupleIndexExpr, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT (expr.get_tuple_expr ())
)

VISIT_DEF (StructExprStruct, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT_INNER_ATTRS (expr),
  VISIT (expr.get_struct_name ())
)

VISIT_DEF_STUB (StructExprFieldIdentifier)

VISIT_DEF (StructExprFieldIdentifierValue, field,
  VISIT (field.get_value ())
)

VISIT_DEF (StructExprFieldIndexValue, field,
  VISIT (field.get_value ())
)

VISIT_DEF (StructBase, base,
  VISIT (base.get_base_struct ())
)

VISIT_DEF (StructExprStructFields, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT_INNER_ATTRS (expr),
  VISIT (expr.get_struct_name ()),
  VISIT_COND (expr.has_struct_base (), VISIT (expr.get_struct_base ())),
  VISIT_VEC (expr.get_fields ())
)

VISIT_DEF (StructExprStructBase, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT_INNER_ATTRS (expr),
  VISIT (expr.get_struct_name ()),
  VISIT (expr.get_struct_base ())
)

VISIT_DEF (CallExpr, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT (expr.get_function_expr ()),
  VISIT_VEC (expr.get_params ())
)

VISIT_DEF (MethodCallExpr, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT (expr.get_receiver_expr ()),
  VISIT (expr.get_method_name ()),
  VISIT_VEC (expr.get_params ())
)

VISIT_DEF (FieldAccessExpr, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT (expr.get_receiver_expr ())
)

VISIT_DEF (ClosureExprInner, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT_VEC (expr.get_params ()),
  VISIT (expr.get_definition_expr ())
)

VISIT_DEF (BlockExpr, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT_INNER_ATTRS (expr),
  VISIT_COND (expr.has_label (),
    VISIT (expr.get_label ())),
  VISIT_VEC (expr.get_statements ()),
  VISIT_COND (expr.has_tail_expr (),
    VISIT (expr.get_tail_expr ()))
)

VISIT_DEF (ConstBlock, expr,
  VISIT (expr.get_const_expr ())
)

VISIT_DEF (AnonConst, expr,
  VISIT_COND (!expr.is_deferred (), VISIT (expr.get_inner_expr ()))
)

VISIT_DEF (ClosureExprInnerTyped, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT_VEC (expr.get_params ()),
  VISIT (expr.get_return_type ()),
  VISIT (expr.get_definition_expr ())
)

VISIT_DEF (ClosureParam, param,
  VISIT_OUTER_ATTRS (param),
  VISIT (param.get_pattern ()),
  VISIT_COND (param.has_type_given (),
    VISIT (param.get_type ()))
)

VISIT_DEF (ContinueExpr, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT_COND (expr.has_label (),
    VISIT (expr.get_label_unchecked ()))
)

VISIT_DEF (BreakExpr, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT_COND (expr.has_label (),
    VISIT (expr.get_label_unchecked ())),
  VISIT_COND (expr.has_break_expr (),
    VISIT (expr.get_break_expr ()))
)

VISIT_DEF (RangeFromToExpr, expr,
  VISIT (expr.get_from_expr ()),
  VISIT (expr.get_to_expr ())
)

VISIT_DEF (RangeFromExpr, expr,
  VISIT (expr.get_from_expr ())
)

VISIT_DEF (RangeToExpr, expr,
  VISIT (expr.get_to_expr ())
)

VISIT_DEF_STUB (RangeFullExpr)

VISIT_DEF (RangeFromToInclExpr, expr,
  VISIT (expr.get_from_expr ()),
  VISIT (expr.get_to_expr ())
)

VISIT_DEF (RangeToInclExpr, expr,
  VISIT (expr.get_to_expr ())
)

VISIT_DEF (ReturnExpr, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT_COND (expr.has_returned_expr (),
    VISIT (expr.get_returned_expr ()))
)

VISIT_DEF (TryExpr, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT (expr.get_block_expr ())
)

VISIT_DEF (BoxExpr, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT (expr.get_boxed_expr ())
)

VISIT_DEF (UnsafeBlockExpr, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT (expr.get_block_expr ())
)

VISIT_DEF (LoopLabel, label,
  VISIT (label.get_lifetime ())
)

VISIT_DEF (LoopExpr, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT_COND (expr.has_loop_label (),
    VISIT (expr.get_loop_label ())),
  VISIT (expr.get_loop_block ())
)

VISIT_DEF (WhileLoopExpr, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT_COND (expr.has_loop_label (),
    VISIT (expr.get_loop_label ())),
  VISIT (expr.get_predicate_expr ()),
  VISIT (expr.get_loop_block ())
)

VISIT_DEF (WhileLetLoopExpr, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT_VEC (expr.get_patterns ()),
  VISIT_COND (expr.has_loop_label (),
    VISIT (expr.get_loop_label ())),
  VISIT (expr.get_scrutinee_expr ()),
  VISIT (expr.get_loop_block ())
)

VISIT_DEF (ForLoopExpr, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT (expr.get_pattern ()),
  VISIT (expr.get_iterator_expr ()),
  VISIT_COND (expr.has_loop_label (),
    VISIT (expr.get_loop_label ())),
  VISIT (expr.get_loop_block ())
)

VISIT_DEF (IfExpr, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT (expr.get_condition_expr ()),
  VISIT (expr.get_if_block ())
)

VISIT_DEF (IfExprConseqElse, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT (expr.get_condition_expr ()),
  VISIT (expr.get_if_block ()),
  VISIT (expr.get_else_block ())
)

VISIT_DEF (IfLetExpr, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT_VEC (expr.get_patterns ()),
  VISIT (expr.get_value_expr ()),
  VISIT (expr.get_if_block ())
)

VISIT_DEF (IfLetExprConseqElse, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT_VEC (expr.get_patterns ()),
  VISIT (expr.get_value_expr ()),
  VISIT (expr.get_if_block ()),
  VISIT (expr.get_else_block ())
)

VISIT_DEF (MatchArm, arm,
  VISIT_OUTER_ATTRS (arm),
  VISIT_VEC (arm.get_patterns ()),
  VISIT_COND (arm.has_match_arm_guard (),
    VISIT (arm.get_guard_expr ()))
)

VISIT_DEF (MatchCase, arm,
  VISIT (arm.get_arm ()),
  VISIT (arm.get_expr ())
)

VISIT_DEF (MatchExpr, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT_INNER_ATTRS (expr),
  VISIT (expr.get_scrutinee_expr ()),
  VISIT_VEC (expr.get_match_cases ())
)

VISIT_DEF (AwaitExpr, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT (expr.get_awaited_expr ())
)

VISIT_DEF (AsyncBlockExpr, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT (expr.get_block_expr ())
)

VISIT_DEF (InlineAsm, expr,
  VISIT_OUTER_ATTRS (expr),
  VISIT_ESCAPE ({
  using RegisterType = AST::InlineAsmOperand::RegisterType;
  for (auto &operand : expr.get_operands ())
    {
      switch (operand.get_register_type ())
	{
	case RegisterType::In:
	  {
	    VISIT_UNESCAPE (VISIT (operand.get_in ().expr))
	    break;
	  }
	case RegisterType::Out:
	  {
	    VISIT_UNESCAPE (VISIT (operand.get_out ().expr))
	    break;
	  }
	case RegisterType::InOut:
	  {
	    VISIT_UNESCAPE (VISIT (operand.get_in_out ().expr))
	    break;
	  }
	case RegisterType::SplitInOut:
	  {
	    auto split = operand.get_split_in_out ();
	    VISIT_UNESCAPE (VISIT (split.in_expr))
	    VISIT_UNESCAPE (VISIT (split.out_expr))
	    break;
	  }
	case RegisterType::Const:
	  {
	    VISIT_UNESCAPE (VISIT (operand.get_const ().anon_const.get_inner_expr ()))
	    break;
	  }
	case RegisterType::Sym:
	  {
	    VISIT_UNESCAPE (VISIT (operand.get_sym ().expr))
	    break;
	  }
	case RegisterType::Label:
	  {
	    VISIT_UNESCAPE (VISIT (operand.get_label ().expr))
	    break;
	  }
	}
    }
  })
)

VISIT_DEF (LlvmInlineAsm, expr,
  VISIT_ESCAPE (
    for (auto &output : expr.get_outputs ())
      VISIT_UNESCAPE (VISIT (output.expr))
    for (auto &input : expr.get_inputs ())
      VISIT_UNESCAPE (VISIT (input.expr))
  )
)

VISIT_DEF (TypeParam, param,
  VISIT_OUTER_ATTRS (param),
  VISIT_VEC (param.get_type_param_bounds ()),
  VISIT_COND (param.has_type (),
    VISIT (param.get_type ()))
)

VISIT_DEF (LifetimeWhereClauseItem, item,
  VISIT (item.get_lifetime ()),
  VISIT_VEC (item.get_lifetime_bounds ())
)

VISIT_DEF (TypeBoundWhereClauseItem, item,
  VISIT_VEC (item.get_for_lifetimes ()),
  VISIT (item.get_type ()),
  VISIT_VEC (item.get_type_param_bounds ())
)

VISIT_DEF (Visibility, vis,
  VISIT_COND (vis.has_path (),
    VISIT (vis.get_path ()))
)

VISIT_DEF_STUB (FunctionQualifiers)

VISIT_DEF (WhereClause, where,
  VISIT_VEC (where.get_items ())
)

VISIT_DEF (FunctionParam, param,
  VISIT_OUTER_ATTRS (param),
  VISIT_COND (param.has_name (),
    VISIT (param.get_pattern ())),
  VISIT (param.get_type ())
)

VISIT_DEF (SelfParam, param,
  VISIT_OUTER_ATTRS (param),
  VISIT_COND (param.has_lifetime (), VISIT (param.get_lifetime ())),
  VISIT_COND (param.has_type (), VISIT (param.get_type ()))
)

VISIT_DEF (Module, module,
  VISIT_OUTER_ATTRS (module),
  VISIT (module.get_visibility ()),
  VISIT_INNER_ATTRS (module),
  VISIT_VEC (module.get_items ())
)

VISIT_DEF (ExternCrate, crate,
  VISIT_OUTER_ATTRS (crate),
  VISIT (crate.get_visibility ())
)

VISIT_DEF (UseTreeGlob, use_tree,
  VISIT (use_tree.get_path ())
)

VISIT_DEF (UseTreeList, use_tree,
  VISIT (use_tree.get_path ())
)

VISIT_DEF (UseTreeRebind, use_tree,
  VISIT (use_tree.get_path ())
)

VISIT_DEF (UseDeclaration, use_decl,
  VISIT (use_decl.get_visibility ()),
  VISIT (use_decl.get_tree ())
)

VISIT_DEF (Function, function,
  VISIT_OUTER_ATTRS (function),
  VISIT (function.get_visibility ()),
  VISIT (function.get_qualifiers ()),
  VISIT_VEC (function.get_generic_params ()),
  VISIT_FN_PARAMS (function),
  VISIT_COND (function.has_return_type (), VISIT (function.get_return_type ())),
  VISIT_COND (function.has_where_clause (), VISIT (function.get_where_clause ())),
  VISIT_COND (function.has_body (), VISIT (*function.get_definition ()))
)

VISIT_DEF (TypeAlias, type_alias,
  VISIT_OUTER_ATTRS (type_alias),
  VISIT (type_alias.get_visibility ()),
  VISIT_VEC (type_alias.get_generic_params ()),
  VISIT_COND (type_alias.has_where_clause (), VISIT (type_alias.get_where_clause ())),
  VISIT (type_alias.get_type_aliased ())
)

VISIT_DEF (StructField, field,
  VISIT_OUTER_ATTRS (field),
  VISIT (field.get_visibility ()),
  VISIT (field.get_field_type ())
)

VISIT_DEF (StructStruct, struct_item,
  VISIT_OUTER_ATTRS (struct_item),
  VISIT (struct_item.get_visibility ()),
  VISIT_VEC (struct_item.get_generic_params ()),
  VISIT_COND (struct_item.has_where_clause (), VISIT (struct_item.get_where_clause ())),
  VISIT_VEC (struct_item.get_fields ())
)

VISIT_DEF (TupleField, field,
  VISIT_OUTER_ATTRS (field),
  VISIT (field.get_visibility ()),
  VISIT (field.get_field_type ())
)

VISIT_DEF (TupleStruct, tuple_struct,
  VISIT_OUTER_ATTRS (tuple_struct),
  VISIT (tuple_struct.get_visibility ()),
  VISIT_VEC (tuple_struct.get_generic_params ()),
  VISIT_COND (tuple_struct.has_where_clause (), VISIT (tuple_struct.get_where_clause ())),
  VISIT_VEC (tuple_struct.get_fields ())
)

VISIT_DEF (EnumItem, item,
  VISIT_OUTER_ATTRS (item),
  VISIT (item.get_visibility ())
)

VISIT_DEF (EnumItemTuple, item,
  VISIT_OUTER_ATTRS (item),
  VISIT (item.get_visibility ()),
  VISIT_VEC (item.get_tuple_fields ())
)

VISIT_DEF (EnumItemStruct, item,
  VISIT_OUTER_ATTRS (item),
  VISIT (item.get_visibility ()),
  VISIT_VEC (item.get_struct_fields ())
)

VISIT_DEF (EnumItemDiscriminant, item,
  VISIT_OUTER_ATTRS (item),
  VISIT (item.get_visibility ()),
  VISIT (item.get_expr ())
)

VISIT_DEF (Enum, enum_item,
  VISIT_OUTER_ATTRS (enum_item),
  VISIT (enum_item.get_visibility ()),
  VISIT_VEC (enum_item.get_generic_params ()),
  VISIT_COND (enum_item.has_where_clause (), VISIT (enum_item.get_where_clause ())),
  VISIT_VEC (enum_item.get_variants ())
)

VISIT_DEF (Union, union_item,
  VISIT_OUTER_ATTRS (union_item),
  VISIT (union_item.get_visibility ()),
  VISIT_VEC (union_item.get_generic_params ()),
  VISIT_COND (union_item.has_where_clause (), VISIT (union_item.get_where_clause ())),
  VISIT_VEC (union_item.get_variants ())
)

VISIT_DEF (ConstantItem, const_item,
  VISIT_OUTER_ATTRS (const_item),
  VISIT (const_item.get_visibility ()),
  VISIT (const_item.get_type ()),
  VISIT_COND (const_item.has_expr (), VISIT (const_item.get_expr ()))
)

VISIT_DEF (StaticItem, static_item,
  VISIT_OUTER_ATTRS (static_item),
  VISIT (static_item.get_visibility ()),
  VISIT (static_item.get_type ()),
  VISIT (static_item.get_expr ())
)

VISIT_DEF (TraitItemConst, item,
  VISIT_OUTER_ATTRS (item),
  VISIT (item.get_type ()),
  VISIT_COND (item.has_expr (), VISIT (item.get_expr ()))
)

VISIT_DEF (TraitItemType, item,
  VISIT_OUTER_ATTRS (item),
  VISIT_VEC (item.get_type_param_bounds ())
)

VISIT_DEF (Trait, trait,
  VISIT_OUTER_ATTRS (trait),
  VISIT (trait.get_visibility ()),
  VISIT_INNER_ATTRS (trait),
  VISIT (trait.get_implicit_self ()),
  VISIT_VEC (trait.get_generic_params ()),
  VISIT_COND (trait.has_where_clause (), VISIT (trait.get_where_clause ())),
  VISIT_VEC (trait.get_type_param_bounds ()),
  VISIT_VEC (trait.get_trait_items ())
)

VISIT_DEF (InherentImpl, impl,
  VISIT_OUTER_ATTRS (impl),
  VISIT (impl.get_visibility ()),
  VISIT_VEC (impl.get_generic_params ()),
  VISIT_COND (impl.has_where_clause (), VISIT (impl.get_where_clause ())),
  VISIT (impl.get_type ()),
  VISIT_INNER_ATTRS (impl),
  VISIT_VEC (impl.get_impl_items ())
)

VISIT_DEF (TraitImpl, impl,
  VISIT_OUTER_ATTRS (impl),
  VISIT (impl.get_visibility ()),
  VISIT_VEC (impl.get_generic_params ()),
  VISIT_COND (impl.has_where_clause (), VISIT (impl.get_where_clause ())),
  VISIT (impl.get_type ()),
  VISIT (impl.get_trait_path ()),
  VISIT_INNER_ATTRS (impl),
  VISIT_VEC (impl.get_impl_items ())
)

VISIT_DEF (ExternalTypeItem, item,
  VISIT_OUTER_ATTRS (item),
  VISIT (item.get_visibility ())
)

VISIT_DEF (ExternalStaticItem, item,
  VISIT_OUTER_ATTRS (item),
  VISIT (item.get_visibility ()),
  VISIT (item.get_type ())
)

VISIT_DEF (ExternBlock, block,
  VISIT_OUTER_ATTRS (block),
  VISIT (block.get_visibility ()),
  VISIT_INNER_ATTRS (block),
  VISIT_VEC (block.get_extern_items ())
)

VISIT_DEF_STUB (MacroMatchFragment)

VISIT_DEF (MacroMatchRepetition, match,
  VISIT_VEC (match.get_matches ())
)

VISIT_DEF (MacroMatcher, matcher,
  VISIT_VEC (matcher.get_matches ())
)

VISIT_DEF (MacroTranscriber, transcriber,
  VISIT (transcriber.get_token_tree ())
)

VISIT_DEF (MacroRule, rule,
  VISIT (rule.get_matcher ()),
  VISIT (rule.get_transcriber ())
)

VISIT_DEF (MacroRulesDefinition, rules_def,
  VISIT_OUTER_ATTRS (rules_def),
  VISIT_VEC (rules_def.get_macro_rules ())
)

VISIT_DEF (MacroInvocData, data,
  VISIT (data.get_path ()),
  VISIT (data.get_delim_tok_tree ())
)

VISIT_DEF (MacroInvocation, macro_invoc,
  VISIT_OUTER_ATTRS (macro_invoc),
  VISIT (macro_invoc.get_invoc_data ())
)

VISIT_DEF (MetaItemPath, meta_item,
  VISIT (meta_item.get_path ())
)

VISIT_DEF (MetaItemSeq, meta_item,
  VISIT (meta_item.get_path ()),
  VISIT_VEC (meta_item.get_seq ())
)

VISIT_DEF_STUB (MetaWord)

VISIT_DEF_STUB (MetaNameValueStr)

VISIT_DEF (MetaListPaths, meta_item,
  VISIT_VEC (meta_item.get_paths ())
)

VISIT_DEF (MetaListNameValueStr, meta_item,
  VISIT_VEC (meta_item.get_values ())
)

VISIT_DEF_STUB (LiteralPattern)

VISIT_DEF (IdentifierPattern, pattern,
  VISIT_COND (pattern.has_subpattern (), VISIT (pattern.get_subpattern ()))
)

VISIT_DEF_STUB (WildcardPattern)

VISIT_DEF_STUB (RestPattern)

VISIT_DEF_STUB (RangePatternBoundLiteral)

VISIT_DEF (RangePatternBoundPath, bound,
  VISIT (bound.get_path ())
)

VISIT_DEF (RangePatternBoundQualPath, bound,
  VISIT (bound.get_qualified_path ())
)

VISIT_DEF (RangePattern, pattern,
  VISIT_COND (pattern.get_has_lower_bound (), VISIT (pattern.get_lower_bound ())),
  VISIT_COND (pattern.get_has_upper_bound (), VISIT (pattern.get_upper_bound ()))
)

VISIT_DEF (ReferencePattern, pattern,
  VISIT (pattern.get_referenced_pattern ())
)

VISIT_DEF (StructPatternFieldTuplePat, field,
  VISIT_OUTER_ATTRS (field),
  VISIT (field.get_index_pattern ())
)

VISIT_DEF (StructPatternFieldIdentPat, field,
  VISIT_OUTER_ATTRS (field),
  VISIT (field.get_ident_pattern ())
)

VISIT_DEF (StructPatternFieldIdent, field,
  VISIT_OUTER_ATTRS (field)
)

VISIT_DEF (StructPatternElements, spe,
  VISIT_VEC (spe.get_struct_pattern_fields ()),
  VISIT_VEC (spe.get_etc_outer_attrs ()) // TODO: look into this
)

VISIT_DEF (StructPattern, pattern,
  VISIT (pattern.get_path ()),
  VISIT (pattern.get_struct_pattern_elems ())
)

VISIT_DEF (TupleStructItemsNoRange, tuple_items,
  VISIT_VEC (tuple_items.get_patterns ())
)

VISIT_DEF (TupleStructItemsRange, tuple_items,
  VISIT_VEC (tuple_items.get_lower_patterns ()),
  VISIT_VEC (tuple_items.get_upper_patterns ())
)

VISIT_DEF (TupleStructPattern, pattern,
  VISIT (pattern.get_path ()),
  VISIT (pattern.get_items ())
)

VISIT_DEF (TuplePatternItemsMultiple, tuple_items,
  VISIT_VEC (tuple_items.get_patterns ())
)

VISIT_DEF (TuplePatternItemsRanged, tuple_items,
  VISIT_VEC (tuple_items.get_lower_patterns ()),
  VISIT_VEC (tuple_items.get_upper_patterns ())
)

VISIT_DEF (TuplePattern, pattern,
  VISIT (pattern.get_items ())
)

VISIT_DEF (GroupedPattern, pattern,
  VISIT (pattern.get_pattern_in_parens ())
)

VISIT_DEF (SlicePattern, pattern,
  VISIT_VEC (pattern.get_items ())
)

VISIT_DEF (AltPattern, pattern,
  VISIT_VEC (pattern.get_alts ())
)

VISIT_DEF_STUB (EmptyStmt)

VISIT_DEF (LetStmt, stmt,
  VISIT_OUTER_ATTRS (stmt),
  VISIT (stmt.get_pattern ()),
  VISIT_COND (stmt.has_type (), VISIT (stmt.get_type ())),
  VISIT_COND (stmt.has_init_expr (), VISIT (stmt.get_init_expr ()))
)

VISIT_DEF (ExprStmt, stmt,
  VISIT (stmt.get_expr ())
)

VISIT_DEF (TraitBound, bound,
  VISIT_VEC (bound.get_for_lifetimes ()),
  VISIT (bound.get_type_path ())
)

VISIT_DEF (ImplTraitType, type,
  VISIT_VEC (type.get_type_param_bounds ())
)

VISIT_DEF (TraitObjectType, type,
  VISIT_VEC (type.get_type_param_bounds ())
)

VISIT_DEF (ParenthesisedType, type,
  VISIT (type.get_type_in_parens ())
)

VISIT_DEF (ImplTraitTypeOneBound, type,
  VISIT (type.get_trait_bound ())
)

VISIT_DEF (TraitObjectTypeOneBound, type,
  VISIT (type.get_trait_bound ())
)

VISIT_DEF (TupleType, type,
  VISIT_VEC (type.get_elems ())
)

VISIT_DEF_STUB (NeverType)

VISIT_DEF (RawPointerType, type,
  VISIT (type.get_type_pointed_to ())
)

VISIT_DEF (ReferenceType, type,
  VISIT (type.get_lifetime ()),
  VISIT (type.get_base_type ())
)

VISIT_DEF (ArrayType, type,
  VISIT (type.get_elem_type ()),
  VISIT (type.get_size_expr ())
)

VISIT_DEF (SliceType, type,
  VISIT (type.get_elem_type ())
)

VISIT_DEF_STUB (InferredType)

VISIT_DEF (MaybeNamedParam, param,
  VISIT_OUTER_ATTRS (param),
  VISIT (param.get_type ())
)

VISIT_DEF (BareFunctionType, type,
  VISIT_VEC (type.get_for_lifetimes ()),
  VISIT (type.get_function_qualifiers ()),
  VISIT_VEC (type.get_function_params ()),
  VISIT_COND (type.is_variadic (), VISIT_VEC (type.get_variadic_attr ())), // TODO: look into this
  VISIT_COND (type.has_return_type (), VISIT (type.get_return_type ()))
)

// TODO: visit subnodes?
VISIT_DEF_STUB (FormatArgs)

VISIT_DEF (VariadicParam, param,
  VISIT_COND (param.has_pattern (), VISIT (param.get_pattern ()))
)

// clang-format on
