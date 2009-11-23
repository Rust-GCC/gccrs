// expressions.cc -- Go frontend expression handling.

// Copyright 2009 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "go-system.h"

#include <gmp.h>

extern "C"
{
#include "intl.h"
#include "tree.h"
#include "gimple.h"
#include "convert.h"
#include "real.h"
#include "diagnostic.h"
#include "tm_p.h"
}

#include "go-c.h"
#include "gogo.h"
#include "types.h"
#include "export.h"
#include "import.h"
#include "refcount.h"
#include "statements.h"
#include "lex.h"
#include "expressions.h"

// Class Expression.

Expression::Expression(Expression_classification classification,
		       source_location location)
  : classification_(classification), location_(location)
{
}

Expression::~Expression()
{
}

// If this expression has a constant integer value, return it.

bool
Expression::integer_constant_value(bool iota_is_constant, mpz_t val,
				   Type** ptype) const
{
  *ptype = NULL;
  return this->do_integer_constant_value(iota_is_constant, val, ptype);
}

// If this expression has a constant floating point value, return it.

bool
Expression::float_constant_value(mpfr_t val, Type** ptype) const
{
  *ptype = NULL;
  if (this->do_float_constant_value(val, ptype))
    return true;
  mpz_t ival;
  mpz_init(ival);
  Type* t;
  bool ret;
  if (!this->do_integer_constant_value(false, ival, &t))
    ret = false;
  else
    {
      mpfr_set_z(val, ival, GMP_RNDN);
      ret = true;
    }
  mpz_clear(ival);
  return ret;
}

// Traverse the expressions.

int
Expression::traverse(Expression** pexpr, Traverse* traverse)
{
  Expression* expr = *pexpr;
  if ((traverse->traverse_mask() & Traverse::traverse_expressions) != 0)
    {
      int t = traverse->expression(pexpr);
      if (t == TRAVERSE_EXIT)
	return TRAVERSE_EXIT;
      else if (t == TRAVERSE_SKIP_COMPONENTS)
	return TRAVERSE_CONTINUE;
    }
  return expr->do_traverse(traverse);
}

// Traverse subexpressions of this expression.

int
Expression::traverse_subexpressions(Traverse* traverse)
{
  return this->do_traverse(traverse);
}

// Default implementation for do_traverse for child classes.

int
Expression::do_traverse(Traverse*)
{
  return TRAVERSE_CONTINUE;
}

// This virtual function is called by the parser if the value of this
// expression is being discarded.  By default, we warn.  Expressions
// with side effects override.

void
Expression::do_discarding_value()
{
  this->warn_unused_value();
}

// This virtual function is the default behaviour for taking the
// address of an expression.

bool
Expression::do_address_taken(source_location location, bool)
{
  this->report_address_taken_error(location);
  return false;
}

// Report an error taking the address of an expression.

void
Expression::report_address_taken_error(source_location location)
{
  error_at(location, "invalid operand for unary %<&%>");
}

// This is the default implementation of the function to handle
// decrementing the reference count of the old value of an lvalue.
// Any expression which may be an l-value must implement this.

Expression*
Expression::do_being_set(Refcounts*)
{
  gcc_unreachable();
}

// This virtual function is called to export expressions.  This will
// only be used by expressions which may be constant.

void
Expression::do_export(Export*) const
{
  gcc_unreachable();
}

// Warn that the value of the expression is not used.

void
Expression::warn_unused_value()
{
  warning_at(this->location(), OPT_Wunused_value, "value computed is not used");
}

// Note that this expression is an error.  This is called by children
// when they discover an error.

void
Expression::set_is_error()
{
  this->classification_ = EXPRESSION_ERROR;
}

// For children to call to report an error conveniently.

void
Expression::report_error(const char* msg)
{
  error_at(this->location_, msg);
  this->set_is_error();
}

// Set types of variables and constants.  This is implemented by the
// child class.

void
Expression::determine_type(const Type_context* context)
{
  this->do_determine_type(context);
}

// Set types when there is no context.

void
Expression::determine_type_no_context()
{
  Type_context context;
  this->do_determine_type(&context);
}

// Return a tree handling any conversions which must be done during
// assignment.

tree
Expression::convert_for_assignment(Translate_context* context, Type* lhs_type,
				   Type* rhs_type, tree rhs_tree,
				   source_location location)
{
  if (lhs_type == rhs_type)
    return rhs_tree;

  if (lhs_type->is_error_type() || rhs_type->is_error_type())
    return error_mark_node;

  if (lhs_type->is_undefined() || rhs_type->is_undefined())
    {
      // Make sure we report the error.
      lhs_type->base();
      rhs_type->base();
      return error_mark_node;
    }

  if (rhs_tree == error_mark_node || TREE_TYPE(rhs_tree) == error_mark_node)
    return error_mark_node;

  Gogo* gogo = context->gogo();

  tree lhs_type_tree = lhs_type->get_tree(gogo);
  if (lhs_type_tree == error_mark_node)
    return error_mark_node;

  if (lhs_type->interface_type() != NULL)
    {
      Interface_type* lhs_interface_type = lhs_type->interface_type();

      if (rhs_type->interface_type() == NULL)
	{
	  // Assignment of a non-interface to an interface.  Since
	  // RHS_TYPE is a static type, we can fill in the interface
	  // method table at compile time.

	  // When setting an interface to nil, we just use NULL.
	  if (rhs_type->is_nil_type())
	    return fold_convert(lhs_type_tree, null_pointer_node);

	  // This should have been checked already.
	  gcc_assert(lhs_interface_type->implements_interface(rhs_type,
							      NULL));

	  // Get the decl for the type descriptor that we are going to
	  // store in the interface value.
	  tree type_descriptor = rhs_type->type_descriptor(gogo);

	  // Build the interface method table for this interface and
	  // this object type: a list of function pointers for each
	  // interface method.
	  Named_type* rhs_named_type = rhs_type->named_type();
	  bool is_pointer = false;
	  if (rhs_named_type == NULL)
	    {
	      rhs_named_type = rhs_type->deref()->named_type();
	      is_pointer = true;
	    }
	  tree method_table;
	  if (rhs_named_type == NULL)
	    method_table = null_pointer_node;
	  else
	    method_table =
	      rhs_named_type->interface_method_table(gogo,
						     lhs_interface_type);
	  method_table = fold_convert(const_ptr_type_node, method_table);

	  // If we are assigning a pointer to the interface, then the
	  // interface refers to the original value.  If we are
	  // assigning a value to the interface, then the interface
	  // takes ownership of the value.

	  if (rhs_type->points_to() != NULL)
	    {
	      static tree new_interface_pointer_decl;
	      tree call = Gogo::call_builtin(&new_interface_pointer_decl,
					     location,
					     "__go_new_interface_pointer",
					     3,
					     ptr_type_node,
					     TREE_TYPE(type_descriptor),
					     type_descriptor,
					     const_ptr_type_node,
					     method_table,
					     ptr_type_node,
					     rhs_tree);
	      return fold_convert(lhs_type_tree, call);
	    }
	  else
	    {
	      // We need an addressable copy of the value.  We are
	      // going to copy the value into the interface.
	      tree make_tmp;
	      tree object;
	      if (TREE_ADDRESSABLE(TREE_TYPE(rhs_tree)) || DECL_P(rhs_tree))
		{
		  make_tmp = NULL_TREE;
		  object = build_fold_addr_expr(rhs_tree);
		  if (DECL_P(rhs_tree))
		    TREE_ADDRESSABLE(rhs_tree) = 1;
		}
	      else
		{
		  tree tmp;
		  if (current_function_decl != NULL)
		    {
		      tmp = create_tmp_var(TREE_TYPE(rhs_tree),
					   get_name(rhs_tree));
		      DECL_INITIAL(tmp) = rhs_tree;
		      make_tmp = build1(DECL_EXPR, void_type_node, tmp);
		      TREE_ADDRESSABLE(tmp) = 1;
		    }
		  else
		    {
		      tmp = build_decl(location, VAR_DECL,
				       create_tmp_var_name("I"),
				       TREE_TYPE(rhs_tree));
		      DECL_EXTERNAL(tmp) = 0;
		      TREE_PUBLIC(tmp) = 0;
		      TREE_STATIC(tmp) = 1;
		      DECL_ARTIFICIAL(tmp) = 1;
		      if (!TREE_CONSTANT(rhs_tree))
			make_tmp = build2(MODIFY_EXPR, void_type_node,
					  tmp, rhs_tree);
		      else
			{
			  TREE_READONLY(tmp) = 1;
			  TREE_CONSTANT(tmp) = 1;
			  DECL_INITIAL(tmp) = rhs_tree;
			  make_tmp = NULL_TREE;
			}
		      rest_of_decl_compilation(tmp, 1, 0);
		    }
		  object = build_fold_addr_expr(tmp);
		}
	      object = fold_convert(ptr_type_node, object);

	      tree object_size = TYPE_SIZE_UNIT(TREE_TYPE(rhs_tree));

	      static tree new_interface_object_decl;
	      tree call = Gogo::call_builtin(&new_interface_object_decl,
					     location,
					     "__go_new_interface_object",
					     4,
					     ptr_type_node,
					     TREE_TYPE(type_descriptor),
					     type_descriptor,
					     const_ptr_type_node,
					     method_table,
					     sizetype,
					     object_size,
					     ptr_type_node,
					     object);
	      call = fold_convert(lhs_type_tree, call);
	      if (make_tmp == NULL_TREE)
		return call;
	      else
		return build2(COMPOUND_EXPR, lhs_type_tree, make_tmp, call);
	    }
	}
      else
	{
	  // Converting from one interface type to another.  In the
	  // general case this requires runtime examination of the
	  // type method table to match it up with the interface
	  // methods.

	  // FIXME: If the right hand side is a subset of the left
	  // hand side, we don't need the runtime examination of
	  // method names.

	  // FIXME: We could do better with some analysis to determine
	  // the type of the right hand side.

	  // FIXME: What if one interface should be a pointer and the
	  // other should be an object?

	  // Build the general interface method table for the left
	  // hand side interface.
	  tree lhs_type_descriptor = lhs_type->type_descriptor(gogo);

	  gcc_assert(POINTER_TYPE_P(TREE_TYPE(rhs_tree)));

	  static tree convert_interface_decl;
	  tree call = Gogo::call_builtin(&convert_interface_decl,
					 location,
					 "__go_convert_interface",
					 3,
					 ptr_type_node,
					 TREE_TYPE(lhs_type_descriptor),
					 lhs_type_descriptor,
					 ptr_type_node,
					 fold_convert(ptr_type_node, rhs_tree),
					 build_pointer_type(boolean_type_node),
					 null_pointer_node);
	  return fold_convert(lhs_type_tree, call);
	}
    }
  else if (rhs_type->interface_type() != NULL)
    {
      // Converting from an interface type to a non-interface type.
      // If the object associated with the interface has the right
      // type, we get the object.  Otherwise we fail at runtime.
      tree lhs_type_descriptor = lhs_type->type_descriptor(gogo);

      gcc_assert(POINTER_TYPE_P(TREE_TYPE(rhs_tree)));

      if (lhs_type->points_to() != NULL)
	{
	  static tree interface_to_pointer_decl;
	  tree call = Gogo::call_builtin(&interface_to_pointer_decl,
					 location,
					 "__go_interface_to_pointer",
					 2,
					 ptr_type_node,
					 TREE_TYPE(lhs_type_descriptor),
					 lhs_type_descriptor,
					 const_ptr_type_node,
					 fold_convert(const_ptr_type_node,
						      rhs_tree));
	  gcc_assert(POINTER_TYPE_P(lhs_type_tree));
	  return fold_convert(lhs_type_tree, call);
	}
      else
	{
	  tree tmp = create_tmp_var(lhs_type_tree, NULL);
	  DECL_IGNORED_P(tmp) = 0;
	  tree make_tmp = build1(DECL_EXPR, void_type_node, tmp);
	  tree tmpaddr = build_fold_addr_expr(tmp);
	  TREE_ADDRESSABLE(tmp) = 1;

	  tree object_size = DECL_SIZE_UNIT(tmp);

	  static tree interface_to_object_decl;
	  tree call = Gogo::call_builtin(&interface_to_object_decl,
					 location,
					 "__go_interface_to_object",
					 4,
					 void_type_node,
					 ptr_type_node,
					 fold_convert(ptr_type_node,
						      tmpaddr),
					 TREE_TYPE(lhs_type_descriptor),
					 lhs_type_descriptor,
					 sizetype,
					 object_size,
					 ptr_type_node,
					 fold_convert(ptr_type_node,
						      rhs_tree));
	  return build2(COMPOUND_EXPR, lhs_type_tree, make_tmp,
			build2(COMPOUND_EXPR, lhs_type_tree, call, tmp));
	}
    }
  else if (lhs_type->is_open_array_type()
	   && rhs_type->points_to() != NULL
	   && rhs_type->points_to()->array_type() != NULL
	   && !rhs_type->points_to()->is_open_array_type())
    {
      // Conversion from *[N]T to []T.
      gcc_assert(TREE_CODE(lhs_type_tree) == RECORD_TYPE);

      Array_type* rhs_at = rhs_type->points_to()->array_type();

      VEC(constructor_elt,gc)* init = VEC_alloc(constructor_elt, gc, 3);

      tree bad_index = NULL_TREE;
      tree value_pointer;
      bool is_constant;
      if (!INDIRECT_REF_P(rhs_tree)
	  || !TREE_CONSTANT(TREE_OPERAND(rhs_tree, 0))
	  || !rhs_at->length()->is_constant())
	{
	  if (!DECL_P(rhs_tree))
	    rhs_tree = save_expr(rhs_tree);
	  bad_index = build2(EQ_EXPR, boolean_type_node, rhs_tree,
			     fold_convert(TREE_TYPE(rhs_tree),
					  null_pointer_node));
	  value_pointer = rhs_tree;
	  is_constant = false;
	}
      else
	{
	  tree decl;
	  if (DECL_P(TREE_OPERAND(rhs_tree, 0)))
	    value_pointer = rhs_tree;
	  else
	    {
	      decl = build_decl(location, VAR_DECL, create_tmp_var_name("A"),
				TREE_TYPE(TREE_TYPE(rhs_tree)));
	      DECL_EXTERNAL(decl) = 0;
	      TREE_PUBLIC(decl) = 0;
	      TREE_STATIC(decl) = 1;
	      DECL_ARTIFICIAL(decl) = 1;
	      DECL_INITIAL(decl) = build_fold_indirect_ref(rhs_tree);
	      rest_of_decl_compilation(decl, 1, 0);
	      value_pointer = build_fold_addr_expr(decl);
	    }
	  is_constant = true;
	}

      constructor_elt* elt = VEC_quick_push(constructor_elt, init, NULL);
      tree field = TYPE_FIELDS(lhs_type_tree);
      gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)),
			"__values") == 0);
      elt->index = field;
      elt->value = value_pointer;
      if (is_constant)
	gcc_assert(TREE_CONSTANT(elt->value));

      elt = VEC_quick_push(constructor_elt, init, NULL);
      field = TREE_CHAIN(field);
      gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)),
			"__count") == 0);
      elt->index = field;
      elt->value = fold_convert(TREE_TYPE(field),
				rhs_at->length_tree(gogo, error_mark_node));
      gcc_assert(elt->value != error_mark_node);
      if (is_constant)
	gcc_assert(TREE_CONSTANT(elt->value));

      elt = VEC_quick_push(constructor_elt, init, NULL);
      field = TREE_CHAIN(field);
      gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)),
			"__capacity") == 0);
      elt->index = field;
      elt->value = fold_convert(TREE_TYPE(field),
				rhs_at->capacity_tree(gogo, error_mark_node));
      gcc_assert(elt->value != error_mark_node);
      if (is_constant)
	gcc_assert(TREE_CONSTANT(elt->value));

      tree val = build_constructor(lhs_type_tree, init);
      if (is_constant)
	TREE_CONSTANT(val) = 1;

      if (bad_index != NULL_TREE)
	{
	  gcc_assert(!is_constant);

	  // FIXME: Duplicates Array_index_expression::do_get_tree.
	  static tree bad_index_fndecl;
	  tree crash = Gogo::call_builtin(&bad_index_fndecl,
					  location,
					  "__go_bad_index",
					  0,
					  void_type_node);
	  TREE_THIS_VOLATILE(bad_index_fndecl) = 1;
	  val = build2(COMPOUND_EXPR, TREE_TYPE(val),
		       build3(COND_EXPR, void_type_node,
			      bad_index, crash, NULL_TREE),
		       val);
	}

      return val;
    }
  else if (lhs_type->is_open_array_type()
	   && rhs_type->is_nil_type())
    {
      // Assigning nil to an open array.
      gcc_assert(TREE_CODE(lhs_type_tree) == RECORD_TYPE);

      VEC(constructor_elt,gc)* init = VEC_alloc(constructor_elt, gc, 3);

      constructor_elt* elt = VEC_quick_push(constructor_elt, init, NULL);
      tree field = TYPE_FIELDS(lhs_type_tree);
      gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)),
			"__values") == 0);
      elt->index = field;
      elt->value = fold_convert(TREE_TYPE(field), null_pointer_node);

      elt = VEC_quick_push(constructor_elt, init, NULL);
      field = TREE_CHAIN(field);
      gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)),
			"__count") == 0);
      elt->index = field;
      elt->value = fold_convert(TREE_TYPE(field), integer_zero_node);

      elt = VEC_quick_push(constructor_elt, init, NULL);
      field = TREE_CHAIN(field);
      gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)),
			"__capacity") == 0);
      elt->index = field;
      elt->value = fold_convert(TREE_TYPE(field), integer_zero_node);

      tree val = build_constructor(lhs_type_tree, init);
      TREE_CONSTANT(val) = 1;

      return val;
    }
  else if (rhs_type->is_nil_type())
    {
      // The left hand side should be a pointer type at the tree
      // level.
      gcc_assert(POINTER_TYPE_P(lhs_type_tree));
      return fold_convert(lhs_type_tree, null_pointer_node);
    }
  else if (lhs_type_tree == TREE_TYPE(rhs_tree))
    {
      // No conversion is needed.
      return rhs_tree;
    }
  else if (POINTER_TYPE_P(lhs_type_tree)
	   || INTEGRAL_TYPE_P(lhs_type_tree)
	   || SCALAR_FLOAT_TYPE_P(lhs_type_tree))
    return fold_convert_loc(location, lhs_type_tree, rhs_tree);
  else if (TREE_CODE(lhs_type_tree) == RECORD_TYPE
	   && TREE_CODE(TREE_TYPE(rhs_tree)) == RECORD_TYPE)
    {
      // This conversion must be permitted by Go, or we wouldn't have
      // gotten here.
      gcc_assert(int_size_in_bytes(lhs_type_tree)
		 == int_size_in_bytes(TREE_TYPE(rhs_tree)));
      return fold_build1_loc(location, VIEW_CONVERT_EXPR, lhs_type_tree,
			     rhs_tree);
    }
  else
    {
      gcc_assert(useless_type_conversion_p(lhs_type_tree, TREE_TYPE(rhs_tree)));
      return rhs_tree;
    }
}

// Convert an expression to a tree.  This is implemented by the child
// class.  Not that it is not in general safe to call this multiple
// times for a single expression, but that we don't catch such errors.

tree
Expression::get_tree(Translate_context* context)
{
  // The child may have marked this expression as having an error.
  if (this->classification_ == EXPRESSION_ERROR)
    return error_mark_node;

  return this->do_get_tree(context);
}

// Return a tree for VAL in TYPE.

tree
Expression::integer_constant_tree(mpz_t val, tree type)
{
  if (type == error_mark_node)
    return error_mark_node;
  else if (TREE_CODE(type) == INTEGER_TYPE)
    return double_int_to_tree(type,
			      mpz_get_double_int(type, val, true));
  else if (TREE_CODE(type) == REAL_TYPE)
    {
      mpfr_t fval;
      mpfr_init_set_z(fval, val, GMP_RNDN);
      tree ret = Expression::float_constant_tree(fval, type);
      mpfr_clear(fval);
      return ret;
    }
  else
    gcc_unreachable();
}

// Return a tree for VAL in TYPE.

tree
Expression::float_constant_tree(mpfr_t val, tree type)
{
  if (type == error_mark_node)
    return error_mark_node;
  else if (TREE_CODE(type) == INTEGER_TYPE)
    {
      mpz_t ival;
      mpz_init(ival);
      mpfr_get_z(ival, val, GMP_RNDN);
      tree ret = Expression::integer_constant_tree(ival, type);
      mpz_clear(ival);
      return ret;
    }
  else if (TREE_CODE(type) == REAL_TYPE)
    {
      REAL_VALUE_TYPE r1;
      real_from_mpfr(&r1, val, type, GMP_RNDN);
      REAL_VALUE_TYPE r2;
      real_convert(&r2, TYPE_MODE(type), &r1);
      return build_real(type, r2);
    }
  else
    gcc_unreachable();
}

// Error expressions.  This are used to avoid cascading errors.

class Error_expression : public Expression
{
 public:
  Error_expression(source_location location)
    : Expression(EXPRESSION_ERROR, location)
  { }

 protected:
  bool
  do_is_constant() const
  { return true; }

  bool
  do_integer_constant_value(bool, mpz_t val, Type**) const
  {
    mpz_set_ui(val, 0);
    return true;
  }

  bool
  do_float_constant_value(mpfr_t val, Type**) const
  {
    mpfr_set_ui(val, 0, GMP_RNDN);
    return true;
  }

  void
  do_discarding_value()
  { }

  Type*
  do_type()
  { return Type::make_error_type(); }

  void
  do_determine_type(const Type_context*)
  { }

  Expression*
  do_copy()
  { return this; }

  bool
  do_is_value() const
  { return true; }

  bool
  do_address_taken(source_location, bool)
  { return true; }

  Expression*
  do_being_set(Refcounts*)
  { return this; }

  tree
  do_get_tree(Translate_context*)
  { return error_mark_node; }
};

Expression*
Expression::make_error(source_location location)
{
  return new Error_expression(location);
}

// An expression which is really a type.  This is used during parsing.
// It is an error if these survive after lowering.

class
Type_expression : public Expression
{
 public:
  Type_expression(Type* type, source_location location)
    : Expression(EXPRESSION_TYPE, location),
      type_(type)
  { }

 protected:
  int
  do_traverse(Traverse* traverse)
  { return Type::traverse(this->type_, traverse); }

  Type*
  do_type()
  { return this->type_; }

  void
  do_determine_type(const Type_context*)
  { }

  void
  do_check_types(Gogo*)
  { this->report_error(_("invalid use of type")); }

  Expression*
  do_copy()
  { return this; }

  tree
  do_get_tree(Translate_context*)
  { gcc_unreachable(); }

 private:
  // The type which we are representing as an expression.
  Type* type_;
};

Expression*
Expression::make_type(Type* type, source_location location)
{
  return new Type_expression(type, location);
}

// Class Var_expression.

// Lower a variable expression.  Here we just make sure that the
// initialization expression of the variable has been lowered.  This
// ensures that we will be able to determine the type of the variable
// if necessary.

Expression*
Var_expression::do_lower(Gogo* gogo, int)
{
  if (this->variable_->is_variable())
    this->variable_->var_value()->lower_init_expression(gogo);
  return this;
}

// Return the name of the variable.

const std::string&
Var_expression::name() const
{
  return this->variable_->name();
}

// Return the type of a reference to a variable.

Type*
Var_expression::do_type()
{
  if (this->variable_->is_variable())
    return this->variable_->var_value()->type();
  else if (this->variable_->is_result_variable())
    return this->variable_->result_var_value()->type();
  else
    gcc_unreachable();
}

// Something takes the address of this variable.  This means that we
// may want to move the variable onto the heap.

bool
Var_expression::do_address_taken(source_location location, bool escapes)
{
  if (!escapes)
    return true;
  else if (this->variable_->is_variable())
    {
      this->variable_->var_value()->set_address_taken();
      return true;
    }
  else if (this->variable_->is_result_variable())
    {
      // There is no way to make a result variable permanent; it has
      // to disappear after the function returns.
      error_at(location, "may not take address of out parameter");
      return false;
    }
  else
    gcc_unreachable();
}

// This variable is being copied.  We may need to increment the
// reference count.

Expression*
Var_expression::do_being_copied(Refcounts* refcounts, bool for_local)
{
  if (!this->type()->has_refcounted_component())
    return this;

  // FIXME: If this is the last use of this variable, then instead of
  // introducing a copy we can avoid freeing this variable at the end
  // of the function.

  return Expression::make_refcount_adjust(refcounts,
					  REFCOUNT_INCREMENT_COPIED,
					  this, for_local);
}

// This variable is being set.  We may need to decrement the reference
// count of the old value.

Expression*
Var_expression::do_being_set(Refcounts* refcounts)
{
  if (this->variable_->is_variable()
      && this->variable_->var_value()->holds_only_args())
    return this;
  if (!this->type()->has_refcounted_component())
    return this;
  return Expression::make_refcount_decrement_lvalue(refcounts, this);
}

// Get the tree for a reference to a variable.

tree
Var_expression::do_get_tree(Translate_context* context)
{
  return this->variable_->get_tree(context->gogo(), context->function());
}

// Make a reference to a variable in an expression.

Expression*
Expression::make_var_reference(Named_object* var, source_location location)
{
  if (var->is_sink())
    return Expression::make_sink(location);

  // FIXME: Creating a new object for each reference to a variable is
  // wasteful.
  return new Var_expression(var, location);
}

// A sink expression--a use of the blank identifier _.

class Sink_expression : public Expression
{
 public:
  Sink_expression(source_location location)
    : Expression(EXPRESSION_SINK, location),
      type_(NULL), var_(NULL_TREE)
  { }

 protected:
  void
  do_discarding_value()
  { }

  Type*
  do_type();

  void
  do_determine_type(const Type_context*);

  Expression*
  do_copy()
  { return new Sink_expression(this->location()); }

  bool
  do_is_lvalue() const
  { return true; }

  Expression*
  do_being_set(Refcounts*)
  { return this; }

  tree
  do_get_tree(Translate_context*);

 private:
  // The type of this sink variable.
  Type* type_;
  // The temporary variable we generate.
  tree var_;
};

// Return the type of a sink expression.

Type*
Sink_expression::do_type()
{
  if (this->type_ == NULL)
    return Type::make_sink_type();
  return this->type_;
}

// Determine the type of a sink expression.

void
Sink_expression::do_determine_type(const Type_context* context)
{
  if (context->type != NULL)
    this->type_ = context->type;
}

// Return a temporary variable for a sink expression.  This will
// presumably be a write-only variable which the middle-end will drop.

tree
Sink_expression::do_get_tree(Translate_context* context)
{
  if (this->var_ == NULL_TREE)
    {
      gcc_assert(this->type_ != NULL && !this->type_->is_sink_type());
      this->var_ = create_tmp_var(this->type_->get_tree(context->gogo()),
				  "blank");
    }
  return this->var_;
}

// Make a sink expression.

Expression*
Expression::make_sink(source_location location)
{
  return new Sink_expression(location);
}

// Class Func_expression.

// FIXME: Can a function expression appear in a constant expression?
// The value is unchanging.  Initializing a constant to the address of
// a function seems like it could work, though there might be little
// point to it.

// Return the name of the function.

const std::string&
Func_expression::name() const
{
  return this->function_->name();
}

// Traversal.  FIXME: Traversing the closure means that we make an
// entry in the reference count queue for it, but we will never use
// that entry in practice.

int
Func_expression::do_traverse(Traverse* traverse)
{
  return (this->closure_ == NULL
	  ? TRAVERSE_CONTINUE
	  : Expression::traverse(&this->closure_, traverse));
}

// Return the type of a function expression.

Type*
Func_expression::do_type()
{
  if (this->function_->is_function())
    return this->function_->func_value()->type();
  else if (this->function_->is_function_declaration())
    return this->function_->func_declaration_value()->type();
  else
    gcc_unreachable();
}

// Copying a function expression requires copying the closure, if any.

Expression*
Func_expression::do_being_copied(Refcounts* refcounts, bool for_local)
{
  if (this->closure_ != NULL)
    this->closure_ = this->closure_->being_copied(refcounts, for_local);
  return this;
}

// There is nothing to do if we discard a function expression.

Expression*
Func_expression::do_note_decrements(Refcounts*)
{
  return this;
}

// Get the tree for a function expression without evaluating the
// closure.

tree
Func_expression::get_tree_without_closure(Gogo* gogo)
{
  Function_type* fntype;
  if (this->function_->is_function())
    fntype = this->function_->func_value()->type();
  else if (this->function_->is_function_declaration())
    fntype = this->function_->func_declaration_value()->type();
  else
    gcc_unreachable();

  // Builtin functions are handled specially by Call_expression.  We
  // can't take their address.
  if (fntype->is_builtin())
    {
      error_at(this->location(), "invalid use of special builtin function %qs",
	       this->function_->name().c_str());
      return error_mark_node;
    }

  Named_object* no = this->function_;
  tree id = this->function_->get_id(gogo);
  tree fndecl;
  if (no->is_function())
    fndecl = no->func_value()->get_or_make_decl(gogo, no, id);
  else if (no->is_function_declaration())
    fndecl = no->func_declaration_value()->get_or_make_decl(gogo, no, id);
  else
    gcc_unreachable();

  tree ret = build_fold_addr_expr(fndecl);
  if (CAN_HAVE_LOCATION_P(ret))
    SET_EXPR_LOCATION(ret, this->location());
  return ret;
}

// Get the tree for a function expression.  This is used when we take
// the address of a function rather than simply calling it.  If the
// function has a closure, we must use a trampoline.

tree
Func_expression::do_get_tree(Translate_context* context)
{
  Gogo* gogo = context->gogo();

  tree fnaddr = this->get_tree_without_closure(gogo);
  if (fnaddr == error_mark_node)
    return error_mark_node;

  // For a normal non-nested function call, that is all we have to do.
  if (!this->function_->is_function()
      || this->function_->func_value()->enclosing() == NULL)
    {
      gcc_assert(this->closure_ == NULL);
      return fnaddr;
    }

  // For a nested function call, we have to always allocate a
  // trampoline.  If we don't always allocate, then closures will not
  // be reliably distinct.
  Expression* closure = this->closure_;
  tree closure_tree;
  if (closure == NULL)
    closure_tree = null_pointer_node;
  else
    {
      // Get the value of the closure.  This will be a pointer to
      // space allocated on the heap.
      closure_tree = closure->get_tree(context);
      if (closure_tree == error_mark_node)
	return error_mark_node;
      gcc_assert(POINTER_TYPE_P(TREE_TYPE(closure_tree)));
    }

  // Now we need to build some code on the heap.  This code will load
  // the static chain pointer with the closure and then jump to the
  // body of the function.  The normal gcc approach is to build the
  // code on the stack.  Unfortunately we can not do that, as Go
  // permits us to return the function pointer.

  return gogo->make_trampoline(fnaddr, closure_tree, this->location());
}

// Make a reference to a function in an expression.

Expression*
Expression::make_func_reference(Named_object* function, Expression* closure,
				source_location location)
{
  return new Func_expression(function, closure, location);
}

// Class Unknown_expression.

// Return the name of an unknown expression.

const std::string&
Unknown_expression::name() const
{
  return this->named_object_->name();
}

// Lower a reference to an unknown name.

Expression*
Unknown_expression::do_lower(Gogo*, int)
{
  source_location location = this->location();
  Named_object* no = this->named_object_;
  Named_object* real = no->unknown_value()->real_named_object();
  if (real == NULL)
    {
      if (this->is_composite_literal_key_)
	return this;
      const std::string& n(this->named_object_->name());
      error_at(location, "reference to undefined name %qs",
	       Gogo::unpack_hidden_name(n).c_str());
      return Expression::make_error(location);
    }
  switch (real->classification())
    {
    case Named_object::NAMED_OBJECT_CONST:
      return Expression::make_const_reference(real, location);
    case Named_object::NAMED_OBJECT_TYPE:
      return Expression::make_type(real->type_value(), location);
    case Named_object::NAMED_OBJECT_TYPE_DECLARATION:
      if (this->is_composite_literal_key_)
	return this;
      error_at(location, "reference to undefined type %qs",
	       Gogo::unpack_hidden_name(real->name()).c_str());
      return Expression::make_error(location);
    case Named_object::NAMED_OBJECT_VAR:
      return Expression::make_var_reference(real, location);
    case Named_object::NAMED_OBJECT_FUNC:
    case Named_object::NAMED_OBJECT_FUNC_DECLARATION:
      return Expression::make_func_reference(real, NULL, location);
    case Named_object::NAMED_OBJECT_PACKAGE:
      if (this->is_composite_literal_key_)
	return this;
      error_at(location, "unexpected reference to package");
      return Expression::make_error(location);
    default:
      gcc_unreachable();
    }
}

// Make a reference to an unknown name.

Expression*
Expression::make_unknown_reference(Named_object* no, source_location location)
{
  gcc_assert(no->resolve()->is_unknown());
  return new Unknown_expression(no, location);
}

// A boolean expression.

class Boolean_expression : public Expression
{
 public:
  Boolean_expression(bool val, source_location location)
    : Expression(EXPRESSION_BOOLEAN, location),
      val_(val)
  { }

  static Expression*
  do_import(Import*);

 protected:
  bool
  do_is_constant() const
  { return true; }

  Type*
  do_type()
  { return Type::make_boolean_type(); }

  void
  do_determine_type(const Type_context*)
  { }

  Expression*
  do_copy()
  { return this; }

  tree
  do_get_tree(Translate_context*)
  { return this->val_ ? boolean_true_node : boolean_false_node; }

  void
  do_export(Export* exp) const
  { exp->write_c_string(this->val_ ? "true" : "false"); }

 private:
  bool val_;
};

// Import a boolean constant.

Expression*
Boolean_expression::do_import(Import* imp)
{
  if (imp->peek_char() == 't')
    {
      imp->require_c_string("true");
      return Expression::make_boolean(true, imp->location());
    }
  else
    {
      imp->require_c_string("false");
      return Expression::make_boolean(false, imp->location());
    }
}

// Make a boolean expression.

Expression*
Expression::make_boolean(bool val, source_location location)
{
  return new Boolean_expression(val, location);
}

// Class String_expression.

// The type of a string expression.

Type*
String_expression::do_type()
{
  return Type::make_string_type();
}

// Build a string constant.

tree
String_expression::do_get_tree(Translate_context* context)
{
  return context->gogo()->go_string_constant_tree(this->val_);
}

// Export a string expression.

void
String_expression::do_export(Export* exp) const
{
  std::string s;
  s.reserve(this->val_.length() * 4 + 2);
  s += '"';
  for (std::string::const_iterator p = this->val_.begin();
       p != this->val_.end();
       ++p)
    {
      if (*p == '\\' || *p == '"')
	{
	  s += '\\';
	  s += *p;
	}
      else if (*p >= 0x20 && *p < 0x7f)
	s += *p;
      else if (*p == '\n')
	s += "\\n";
      else if (*p == '\t')
	s += "\\t";
      else
	{
	  s += "\\x";
	  unsigned char c = *p;
	  unsigned int dig = c >> 4;
	  s += dig < 10 ? '0' + dig : 'A' + dig - 10;
	  dig = c & 0xf;
	  s += dig < 10 ? '0' + dig : 'A' + dig - 10;
	}
    }
  s += '"';
  exp->write_string(s);
}

// Import a string expression.

Expression*
String_expression::do_import(Import* imp)
{
  imp->require_c_string("\"");
  std::string val;
  while (true)
    {
      int c = imp->get_char();
      if (c == '"' || c == -1)
	break;
      if (c != '\\')
	val += static_cast<char>(c);
      else
	{
	  c = imp->get_char();
	  if (c == '\\' || c == '"')
	    val += static_cast<char>(c);
	  else if (c == 'n')
	    val += '\n';
	  else if (c == 't')
	    val += '\t';
	  else if (c == 'x')
	    {
	      c = imp->get_char();
	      unsigned int vh = c >= '0' && c <= '9' ? c - '0' : c - 'A' + 10;
	      c = imp->get_char();
	      unsigned int vl = c >= '0' && c <= '9' ? c - '0' : c - 'A' + 10;
	      char v = (vh << 4) | vl;
	      val += v;
	    }
	  else
	    {
	      error_at(imp->location(), "bad string constant");
	      return Expression::make_error(imp->location());
	    }
	}
    }
  return Expression::make_string(val, imp->location());
}

// Make a string expression.

Expression*
Expression::make_string(const std::string& val, source_location location)
{
  return new String_expression(val, location);
}

// Make an integer expression.

class Integer_expression : public Expression
{
 public:
  Integer_expression(const mpz_t* val, Type* type, source_location location)
    : Expression(EXPRESSION_INTEGER, location),
      type_(type)
  { mpz_init_set(this->val_, *val); }

  static Expression*
  do_import(Import*);

  // Return whether VAL fits in the type.
  static bool
  check_constant(mpz_t val, Type*, source_location);

 protected:
  bool
  do_is_constant() const
  { return true; }

  bool
  do_integer_constant_value(bool, mpz_t val, Type** ptype) const;

  Type*
  do_type();

  void
  do_determine_type(const Type_context* context);

  void
  do_check_types(Gogo*);

  tree
  do_get_tree(Translate_context*);

  Expression*
  do_copy()
  { return Expression::make_integer(&this->val_, this->type_,
				    this->location()); }

  void
  do_export(Export*) const;

 private:
  // The integer value.
  mpz_t val_;
  // The type so far.
  Type* type_;
};

// Return an integer constant value.

bool
Integer_expression::do_integer_constant_value(bool, mpz_t val,
					      Type** ptype) const
{
  if (this->type_ != NULL)
    *ptype = this->type_;
  mpz_set(val, this->val_);
  return true;
}

// Return the current type.  If we haven't set the type yet, we return
// an abstract integer type.

Type*
Integer_expression::do_type()
{
  if (this->type_ == NULL)
    this->type_ = Type::make_abstract_integer_type();
  return this->type_;
}

// Set the type of the integer value.  Here we may switch from an
// abstract type to a real type.

void
Integer_expression::do_determine_type(const Type_context* context)
{
  if (this->type_ != NULL && !this->type_->is_abstract())
    ;
  else if (context->type != NULL
	   && (context->type->integer_type() != NULL
	       || context->type->float_type() != NULL))
    this->type_ = context->type;
  else if (!context->may_be_abstract)
    this->type_ = Type::lookup_integer_type("int");
}

// Return true if the integer VAL fits in the range of the type TYPE.
// Otherwise give an error and return false.  TYPE may be NULL.

bool
Integer_expression::check_constant(mpz_t val, Type* type,
				   source_location location)
{
  if (type == NULL)
    return true;
  Integer_type* itype = type->integer_type();
  if (itype == NULL || itype->is_abstract())
    return true;

  int bits = mpz_sizeinbase(val, 2);

  if (itype->is_unsigned())
    {
      // For an unsigned type we can only accept a nonnegative number,
      // and we must be able to represent at least BITS.
      if (mpz_sgn(val) >= 0
	  && bits <= itype->bits())
	return true;
    }
  else
    {
      // For a signed type we need an extra bit to indicate the sign.
      // We have to handle the most negative integer specially.
      if (bits + 1 <= itype->bits()
	  || (bits <= itype->bits()
	      && mpz_sgn(val) < 0
	      && (mpz_scan1(val, 0)
		  == static_cast<unsigned long>(itype->bits() - 1))
	      && mpz_scan0(val, itype->bits()) == ULONG_MAX))
	return true;
    }

  error_at(location, "integer constant overflow");
  return false;
}

// Check the type of an integer constant.

void
Integer_expression::do_check_types(Gogo*)
{
  if (this->type_ == NULL)
    return;
  if (!Integer_expression::check_constant(this->val_, this->type_,
					  this->location()))
    this->set_is_error();
}

// Get a tree for an integer constant.

tree
Integer_expression::do_get_tree(Translate_context* context)
{
  Gogo* gogo = context->gogo();
  tree type;
  if (this->type_ != NULL && !this->type_->is_abstract())
    type = this->type_->get_tree(gogo);
  else if (this->type_ != NULL && this->type_->float_type() != NULL)
    {
      // We are converting to an abstract floating point type.
      type = Type::lookup_float_type("float64")->get_tree(gogo);
    }
  else
    {
      // If we still have an abstract type here, then this is being
      // used in a constant expression which didn't get reduced for
      // some reason.  Use a type which will fit the value.  We use <,
      // not <=, because we need an extra bit for the sign bit.
      int bits = mpz_sizeinbase(this->val_, 2);
      if (bits < INT_TYPE_SIZE)
	type = Type::lookup_integer_type("int")->get_tree(gogo);
      else if (bits < 64)
	type = Type::lookup_integer_type("int64")->get_tree(gogo);
      else
	type = long_long_integer_type_node;
    }
  return Expression::integer_constant_tree(this->val_, type);
}

// Export an integer in a constant expression.

void
Integer_expression::do_export(Export* exp) const
{
  char* s = mpz_get_str(NULL, 10, this->val_);
  exp->write_c_string(s);
  free(s);
  // A trailing space lets us reliably identify the end of the number.
  exp->write_c_string(" ");
}

// Import an integer or floating point value.  This handles both
// because they both start with digits.

Expression*
Integer_expression::do_import(Import* imp)
{
  std::string num = imp->read_identifier();
  imp->require_c_string(" ");
  if (num.find('.') == std::string::npos && num.find('E') == std::string::npos)
    {
      mpz_t val;
      if (mpz_init_set_str(val, num.c_str(), 10) != 0)
	{
	  error_at(imp->location(), "bad number in import data: %qs",
		   num.c_str());
	  return Expression::make_error(imp->location());
	}
      Expression* ret = Expression::make_integer(&val, NULL, imp->location());
      mpz_clear(val);
      return ret;
    }
  else
    {
      mpfr_t val;
      if (mpfr_init_set_str(val, num.c_str(), 10, GMP_RNDN) != 0)
	{
	  error_at(imp->location(), "bad number in import data: %qs",
		   num.c_str());
	  return Expression::make_error(imp->location());
	}
      Expression* ret = Expression::make_float(&val, NULL, imp->location());
      mpfr_clear(val);
      return ret;
    }
}

// Build a new integer value.

Expression*
Expression::make_integer(const mpz_t* val, Type* type,
			 source_location location)
{
  return new Integer_expression(val, type, location);
}

// Floats.

class Float_expression : public Expression
{
 public:
  Float_expression(const mpfr_t* val, Type* type, source_location location)
    : Expression(EXPRESSION_FLOAT, location),
      type_(type)
  {
    mpfr_init_set(this->val_, *val, GMP_RNDN);
  }

  // Constrain VAL to fit into TYPE.
  static void
  constrain_float(mpfr_t val, Type* type);

  // Return whether VAL fits in the type.
  static bool
  check_constant(mpfr_t val, Type*, source_location);

 protected:
  bool
  do_is_constant() const
  { return true; }

  bool
  do_float_constant_value(mpfr_t val, Type**) const;

  Type*
  do_type();

  void
  do_determine_type(const Type_context*);

  void
  do_check_types(Gogo*);

  Expression*
  do_copy()
  { return Expression::make_float(&this->val_, this->type_,
				  this->location()); }

  tree
  do_get_tree(Translate_context*);

  void
  do_export(Export*) const;

 private:
  // The floating point value.
  mpfr_t val_;
  // The type so far.
  Type* type_;
};

// Constrain VAL to fit into TYPE.

void
Float_expression::constrain_float(mpfr_t val, Type* type)
{
  Float_type* ftype = type->float_type();
  if (ftype != NULL && !ftype->is_abstract())
    {
      tree type_tree = ftype->type_tree();
      REAL_VALUE_TYPE rvt;
      real_from_mpfr(&rvt, val, type_tree, GMP_RNDN);
      real_convert(&rvt, TYPE_MODE(type_tree), &rvt);
      mpfr_from_real(val, &rvt, GMP_RNDN);
    }
}

// Return a floating point constant value.

bool
Float_expression::do_float_constant_value(mpfr_t val, Type** ptype) const
{
  if (this->type_ != NULL)
    *ptype = this->type_;
  mpfr_set(val, this->val_, GMP_RNDN);
  return true;
}

// Return the current type.  If we haven't set the type yet, we return
// an abstract float type.

Type*
Float_expression::do_type()
{
  if (this->type_ == NULL)
    this->type_ = Type::make_abstract_float_type();
  return this->type_;
}

// Set the type of the float value.  Here we may switch from an
// abstract type to a real type.

void
Float_expression::do_determine_type(const Type_context* context)
{
  if (this->type_ != NULL && !this->type_->is_abstract())
    ;
  else if (context->type != NULL
	   && (context->type->integer_type() != NULL
	       || context->type->float_type() != NULL))
    this->type_ = context->type;
  else if (!context->may_be_abstract)
    this->type_ = Type::lookup_float_type("float");
}

// Return true if the floating point value VAL fits in the range of
// the type TYPE.  Otherwise give an error and return false.  TYPE may
// be NULL.

bool
Float_expression::check_constant(mpfr_t val, Type* type,
				 source_location location)
{
  if (type == NULL)
    return true;
  Float_type* ftype= type->float_type();
  if (ftype == NULL || ftype->is_abstract())
    return true;

  // A NaN or Infinity always fits in the range of the type.
  if (mpfr_nan_p(val) || mpfr_inf_p(val) || mpfr_zero_p(val))
    return true;

  mp_exp_t exp = mpfr_get_exp(val);
  mp_exp_t max_exp;
  switch (ftype->bits())
    {
    case 32:
      max_exp = 128;
      break;
    case 64:
      max_exp = 1024;
      break;
    default:
      gcc_unreachable();
    }
  if (exp > max_exp)
    {
      error_at(location, "floating point constant overflow");
      return false;
    }
  return true;
}

// Check the type of a float value.

void
Float_expression::do_check_types(Gogo*)
{
  if (this->type_ == NULL)
    return;

  if (!Float_expression::check_constant(this->val_, this->type_,
					this->location()))
    this->set_is_error();

  Integer_type* integer_type = this->type_->integer_type();
  if (integer_type != NULL)
    {
      if (!mpfr_integer_p(this->val_))
	this->report_error(_("floating point constant truncated to integer"));
      else
	{
	  gcc_assert(!integer_type->is_abstract());
	  mpz_t ival;
	  mpz_init(ival);
	  mpfr_get_z(ival, this->val_, GMP_RNDN);
	  Integer_expression::check_constant(ival, integer_type,
					     this->location());
	  mpz_clear(ival);
	}
    }
}

// Get a tree for a float constant.

tree
Float_expression::do_get_tree(Translate_context* context)
{
  Gogo* gogo = context->gogo();
  tree type;
  if (this->type_ != NULL && !this->type_->is_abstract())
    type = this->type_->get_tree(gogo);
  else if (this->type_ != NULL && this->type_->integer_type() != NULL)
    {
      // We have an abstract integer type.  We just hope for the best.
      type = Type::lookup_integer_type("int")->get_tree(gogo);
    }
  else
    {
      // If we still have an abstract type here, then this is being
      // used in a constant expression which didn't get reduced.  We
      // just use float64 and hope for the best.
      type = Type::lookup_float_type("float64")->get_tree(gogo);
    }
  return Expression::float_constant_tree(this->val_, type);
}

// Export a floating point number in a constant expression.

void
Float_expression::do_export(Export* exp) const
{
  exp->write_c_string("0.");
  mp_exp_t exponent;
  char* s = mpfr_get_str(NULL, &exponent, 10, 0, this->val_, GMP_RNDN);
  exp->write_c_string(s);
  char buf[30];
  snprintf(buf, sizeof buf, "E%ld", exponent);
  exp->write_c_string(buf);
  // A trailing space lets us reliably identify the end of the number.
  exp->write_c_string(" ");
}

// Make a float expression.

Expression*
Expression::make_float(const mpfr_t* val, Type* type, source_location location)
{
  return new Float_expression(val, type, location);
}

// A reference to a const in an expression.

class Const_expression : public Expression
{
 public:
  Const_expression(Named_object* constant, source_location location)
    : Expression(EXPRESSION_CONST_REFERENCE, location),
      constant_(constant), type_(NULL)
  { }

  const std::string&
  name() const
  { return this->constant_->name(); }

 protected:
  Expression*
  do_lower(Gogo*, int);

  bool
  do_is_constant() const
  { return true; }

  bool
  do_integer_constant_value(bool, mpz_t val, Type**) const;

  bool
  do_float_constant_value(mpfr_t val, Type**) const;

  bool
  do_string_constant_value(std::string* val) const
  { return this->constant_->const_value()->expr()->string_constant_value(val); }

  Type*
  do_type();

  // The type of a const is set by the declaration, not the use.
  void
  do_determine_type(const Type_context*);

  void
  do_check_types(Gogo*);

  Expression*
  do_copy()
  { return this; }

  bool
  do_address_taken(source_location location, bool escapes)
  {
    return this->constant_->const_value()->expr()->address_taken(location,
								 escapes);
  }

  Expression*
  do_being_copied(Refcounts*, bool);

  Expression*
  do_note_decrements(Refcounts*);

  tree
  do_get_tree(Translate_context* context);

  // When exporting a reference to a const as part of a const
  // expression, we export the value.  We ignore the fact that it has
  // a name.
  void
  do_export(Export* exp) const
  { this->constant_->const_value()->expr()->export_expression(exp); }

 private:
  // The constant.
  Named_object* constant_;
  // The type of this reference.  This is used if the constant has an
  // abstract type.
  Type* type_;
};

// Lower a constant expression.  This is where we convert the
// predeclared constant iota into an integer value.

Expression*
Const_expression::do_lower(Gogo* gogo, int iota_value)
{
  if (this->constant_->const_value()->expr()->classification()
      == EXPRESSION_IOTA)
    {
      if (iota_value == -1)
	{
	  error_at(this->location(),
		   "iota is only defined in const declarations");
	  iota_value = 0;
	}
      mpz_t val;
      mpz_init_set_ui(val, static_cast<unsigned long>(iota_value));
      Expression* ret = Expression::make_integer(&val, NULL,
						 this->location());
      mpz_clear(val);
      return ret;
    }

  // Make sure that the constant itself has been lowered.
  gogo->lower_constant(this->constant_);

  return this;
}

// Return an integer constant value.

bool
Const_expression::do_integer_constant_value(bool iota_is_constant, mpz_t val,
					    Type** ptype) const
{
  Type* ctype;
  if (this->type_ != NULL)
    ctype = this->type_;
  else
    ctype = this->constant_->const_value()->type();
  if (ctype != NULL && ctype->integer_type() == NULL)
    return false;

  Expression* e = this->constant_->const_value()->expr();
  Type* t;
  bool r = e->integer_constant_value(iota_is_constant, val, &t);

  if (r
      && ctype != NULL
      && !Integer_expression::check_constant(val, ctype, this->location()))
    return false;

  *ptype = ctype != NULL ? ctype : t;
  return r;
}

// Return a floating point constant value.

bool
Const_expression::do_float_constant_value(mpfr_t val, Type** ptype) const
{
  Type* ctype;
  if (this->type_ != NULL)
    ctype = this->type_;
  else
    ctype = this->constant_->const_value()->type();
  if (ctype != NULL && ctype->float_type() == NULL)
    return false;

  Type* t;
  bool r = this->constant_->const_value()->expr()->float_constant_value(val,
									&t);
  if (r && ctype != NULL)
    {
      if (!Float_expression::check_constant(val, ctype, this->location()))
	return false;
      Float_expression::constrain_float(val, ctype);
    }
  *ptype = ctype != NULL ? ctype : t;
  return r;
}

// Return the type of the const reference.

Type*
Const_expression::do_type()
{
  if (this->type_ != NULL)
    return this->type_;
  Named_constant* nc = this->constant_->const_value();
  Type* ret = nc->type();
  if (ret != NULL)
    return ret;
  // During parsing, a named constant may have a NULL type, but we
  // must not return a NULL type here.
  return nc->expr()->type();
}

// Set the type of the const reference.

void
Const_expression::do_determine_type(const Type_context* context)
{
  Type* ctype = this->constant_->const_value()->type();
  if (ctype != NULL && !ctype->is_abstract())
    this->type_ = ctype;
  else if (context->type != NULL)
    this->type_ = context->type;
  else if (!context->may_be_abstract)
    {
      if (ctype == NULL)
	ctype = this->constant_->const_value()->expr()->type();
      if (ctype->is_abstract())
	ctype = ctype->make_non_abstract_type();
      this->type_ = ctype;
    }
}

// Check types of a const reference.

void
Const_expression::do_check_types(Gogo*)
{
  if (this->type_ == NULL || this->type_->is_abstract())
    return;

  // Check for integer overflow.
  if (this->type_->integer_type() != NULL)
    {
      mpz_t ival;
      mpz_init(ival);
      Type* dummy;
      if (!this->integer_constant_value(true, ival, &dummy))
	{
	  mpfr_t fval;
	  mpfr_init(fval);
	  Expression* cexpr = this->constant_->const_value()->expr();
	  if (cexpr->float_constant_value(fval, &dummy))
	    {
	      if (!mpfr_integer_p(fval))
		this->report_error("floating point constant "
				   "truncated to integer");
	      else
		{
		  mpfr_get_z(ival, fval, GMP_RNDN);
		  Integer_expression::check_constant(ival, this->type_,
						     this->location());
		}
	    }
	  mpfr_clear(fval);
	}
      mpz_clear(ival);
    }
}

// Copying a constant will require incrementing a reference count if
// the constant is a struct or an array with components that require
// reference counts.  If the type itself is reference counted then
// copying it should not require any adustment.

Expression*
Const_expression::do_being_copied(Refcounts* refcounts, bool for_local)
{
  Type* type = this->type();
  if (type->is_refcounted())
    return this;
  if (!type->has_refcounted_component())
    return this;
  return Expression::make_refcount_adjust(refcounts,
					  REFCOUNT_INCREMENT_COPIED,
					  this, for_local);
}

// Referring to a constant never increments a reference count.

Expression*
Const_expression::do_note_decrements(Refcounts*)
{
  return this;
}

// Return a tree for the const reference.

tree
Const_expression::do_get_tree(Translate_context* context)
{
  Gogo* gogo = context->gogo();
  tree type_tree;
  if (this->type_ == NULL)
    type_tree = NULL_TREE;
  else
    {
      type_tree = this->type_->get_tree(gogo);
      if (type_tree == error_mark_node)
	return error_mark_node;
    }

  // If the type has been set for this expression, but the underlying
  // object is an abstract int or float, we try to get the abstract
  // value.  Otherwise we may lose something in the conversion.
  if (this->type_ != NULL
      && this->constant_->const_value()->type()->is_abstract())
    {
      Expression* expr = this->constant_->const_value()->expr();
      mpz_t ival;
      mpz_init(ival);
      Type* t;
      if (expr->integer_constant_value(true, ival, &t))
	{
	  tree ret = Expression::integer_constant_tree(ival, type_tree);
	  mpz_clear(ival);
	  return ret;
	}
      mpz_clear(ival);

      mpfr_t fval;
      mpfr_init(fval);
      if (expr->float_constant_value(fval, &t))
	{
	  tree ret = Expression::float_constant_tree(fval, type_tree);
	  mpfr_clear(fval);
	  return ret;
	}
      mpfr_clear(fval);
    }

  tree const_tree = this->constant_->get_tree(gogo, context->function());
  if (this->type_ == NULL
      || const_tree == error_mark_node
      || TREE_TYPE(const_tree) == error_mark_node)
    return const_tree;

  tree ret;
  if (TYPE_MAIN_VARIANT(type_tree) == TYPE_MAIN_VARIANT(TREE_TYPE(const_tree)))
    ret = fold_convert(type_tree, const_tree);
  else if (TREE_CODE(type_tree) == INTEGER_TYPE)
    ret = fold(convert_to_integer(type_tree, const_tree));
  else if (TREE_CODE(type_tree) == REAL_TYPE)
    ret = fold(convert_to_real(type_tree, const_tree));
  else
    gcc_unreachable();
  return ret;
}

// Make a reference to a constant in an expression.

Expression*
Expression::make_const_reference(Named_object* constant,
				 source_location location)
{
  return new Const_expression(constant, location);
}

// The nil value.

class Nil_expression : public Expression
{
 public:
  Nil_expression(source_location location)
    : Expression(EXPRESSION_NIL, location)
  { }

  static Expression*
  do_import(Import*);

 protected:
  bool
  do_is_constant() const
  { return true; }

  Type*
  do_type()
  { return Type::make_nil_type(); }

  void
  do_determine_type(const Type_context*)
  { }

  Expression*
  do_copy()
  { return this; }

  tree
  do_get_tree(Translate_context*)
  { return null_pointer_node; }

  void
  do_export(Export* exp) const
  { exp->write_c_string("nil"); }
};

// Import a nil expression.

Expression*
Nil_expression::do_import(Import* imp)
{
  imp->require_c_string("nil");
  return Expression::make_nil(imp->location());
}

// Make a nil expression.

Expression*
Expression::make_nil(source_location location)
{
  return new Nil_expression(location);
}

// The value of the predeclared constant iota.  This is little more
// than a marker.  This will be lowered to an integer in
// Const_expression::do_lower, which is where we know the value that
// it should have.

class Iota_expression : public Parser_expression
{
 public:
  Iota_expression(source_location location)
    : Parser_expression(EXPRESSION_IOTA, location)
  { }

 protected:
  Expression*
  do_lower(Gogo*, int)
  { gcc_unreachable(); }

  // There should only ever be one of these.
  Expression*
  do_copy()
  { gcc_unreachable(); }
};

// Make an iota expression.  This is only called for one case: the
// value of the predeclared constant iota.

Expression*
Expression::make_iota()
{
  static Iota_expression iota_expression(UNKNOWN_LOCATION);
  return &iota_expression;
}

// A type conversion expression.

class Type_conversion_expression : public Expression
{
 public:
  Type_conversion_expression(Type* type, Expression* expr,
			     source_location location)
    : Expression(EXPRESSION_CONVERSION, location),
      type_(type), expr_(expr), is_being_copied_(false)
  { }

  // Return the type to which we are converting.
  Type*
  type() const
  { return this->type_; }

  // Return the expression which we are converting.
  Expression*
  expr() const
  { return this->expr_; }

  static Expression*
  do_import(Import*);

 protected:
  int
  do_traverse(Traverse* traverse);

  Expression*
  do_lower(Gogo*, int);

  bool
  do_is_constant() const
  { return this->expr_->is_constant(); }

  bool
  do_integer_constant_value(bool, mpz_t, Type**) const;

  bool
  do_float_constant_value(mpfr_t, Type**) const;

  bool
  do_string_constant_value(std::string*) const;

  Type*
  do_type()
  { return this->type_; }

  void
  do_determine_type(const Type_context*)
  {
    Type_context subcontext(this->type_, true);
    this->expr_->determine_type(&subcontext);
  }

  void
  do_check_types(Gogo*);

  Expression*
  do_copy()
  {
    return new Type_conversion_expression(this->type_, this->expr_->copy(),
					  this->location());
  }

  Expression*
  do_being_copied(Refcounts*, bool);

  Expression*
  do_note_decrements(Refcounts*);

  tree
  do_get_tree(Translate_context* context);

  void
  do_export(Export*) const;

 private:
  // The type to convert to.
  Type* type_;
  // The expression to convert.
  Expression* expr_;
  // Whether this expression is being copied.
  bool is_being_copied_;
};

// Traversal.

int
Type_conversion_expression::do_traverse(Traverse* traverse)
{
  if (Expression::traverse(&this->expr_, traverse) == TRAVERSE_EXIT
      || Type::traverse(this->type_, traverse) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  return TRAVERSE_CONTINUE;
}

// Convert to a constant at lowering time.

Expression*
Type_conversion_expression::do_lower(Gogo*, int)
{
  Type* type = this->type_;
  Expression* val = this->expr_;
  source_location location = this->location();

  if (type->integer_type() != NULL)
    {
      mpz_t ival;
      mpz_init(ival);
      Type* dummy;
      if (val->integer_constant_value(false, ival, &dummy))
	{
	  if (!Integer_expression::check_constant(ival, type, location))
	    mpz_set_ui(ival, 0);
	  return Expression::make_integer(&ival, type, location);
	}

      mpfr_t fval;
      mpfr_init(fval);
      if (val->float_constant_value(fval, &dummy))
	{
	  if (!mpfr_integer_p(fval))
	    {
	      error_at(location,
		       "floating point constant truncated to integer");
	      return Expression::make_error(location);
	    }
	  mpfr_get_z(ival, fval, GMP_RNDN);
	  if (!Integer_expression::check_constant(ival, type, location))
	    mpz_set_ui(ival, 0);
	  return Expression::make_integer(&ival, type, location);
	}
      mpfr_clear(fval);
      mpz_clear(ival);
    }

  if (type->float_type() != NULL)
    {
      mpfr_t fval;
      mpfr_init(fval);
      Type* dummy;
      if (val->float_constant_value(fval, &dummy))
	{
	  if (!Float_expression::check_constant(fval, type, location))
	    mpfr_set_ui(fval, 0, GMP_RNDN);
	  Float_expression::constrain_float(fval, type);
	  return Expression::make_float(&fval, type, location);
	}
      mpfr_clear(fval);
    }

  return this;
}

// Return the constant integer value if there is one.

bool
Type_conversion_expression::do_integer_constant_value(bool iota_is_constant,
						      mpz_t val,
						      Type** ptype) const
{
  if (this->type_->integer_type() == NULL)
    return false;

  mpz_t ival;
  mpz_init(ival);
  Type* dummy;
  if (this->expr_->integer_constant_value(iota_is_constant, ival, &dummy))
    {
      if (!Integer_expression::check_constant(ival, this->type_,
					      this->location()))
	{
	  mpz_clear(ival);
	  return false;
	}
      mpz_set(val, ival);
      mpz_clear(ival);
      *ptype = this->type_;
      return true;
    }
  mpz_clear(ival);

  mpfr_t fval;
  mpfr_init(fval);
  if (this->expr_->float_constant_value(fval, &dummy))
    {
      mpfr_get_z(val, fval, GMP_RNDN);
      mpfr_clear(fval);
      if (!Integer_expression::check_constant(val, this->type_,
					      this->location()))
	return false;
      *ptype = this->type_;
      return true;
    }
  mpfr_clear(fval);

  return false;
}

// Return the constant floating point value if there is one.

bool
Type_conversion_expression::do_float_constant_value(mpfr_t val,
						    Type** ptype) const
{
  if (this->type_->float_type() == NULL)
    return false;

  mpfr_t fval;
  mpfr_init(fval);
  Type* dummy;
  if (this->expr_->float_constant_value(fval, &dummy))
    {
      if (!Float_expression::check_constant(fval, this->type_,
					    this->location()))
	{
	  mpfr_clear(fval);
	  return false;
	}
      mpfr_set(val, fval, GMP_RNDN);
      mpfr_clear(fval);
      Float_expression::constrain_float(val, this->type_);
      *ptype = this->type_;
      return true;
    }
  mpfr_clear(fval);

  mpz_t ival;
  mpz_init(ival);
  if (this->expr_->integer_constant_value(false, ival, &dummy))
    {
      mpfr_set_z(val, ival, GMP_RNDN);
      mpz_clear(ival);
      if (!Float_expression::check_constant(val, this->type_,
					    this->location()))
	return false;
      Float_expression::constrain_float(val, this->type_);
      *ptype = this->type_;
      return true;
    }
  mpz_clear(ival);

  return false;
}

// Return the constant string value if there is one.

bool
Type_conversion_expression::do_string_constant_value(std::string* val) const
{
  if (this->type_->is_string_type()
      && this->expr_->type()->integer_type() != NULL)
    {
      mpz_t ival;
      mpz_init(ival);
      Type* dummy;
      if (this->expr_->integer_constant_value(false, ival, &dummy))
	{
	  unsigned long ulval = mpz_get_ui(ival);
	  if (mpz_cmp_ui(ival, ulval) == 0)
	    {
	      Lex::append_char(ulval, true, val, this->location());
	      mpz_clear(ival);
	      return true;
	    }
	}
      mpz_clear(ival);
    }

  // FIXME: Could handle conversion from const []int here.

  return false;
}

// Check that types are convertible.

void
Type_conversion_expression::do_check_types(Gogo*)
{
  Type* type = this->type_;
  Type* expr_type = this->expr_->type();
  std::string reason;

  if (Type::are_compatible_for_conversion(type, expr_type, &reason))
    return;

  bool ok = false;
  if ((type->integer_type() != NULL || type->float_type() != NULL)
      && (expr_type->integer_type() != NULL
	  || expr_type->float_type() != NULL))
    ok = true;
  else if (type->is_string_type())
    {
      if (expr_type->integer_type() != NULL)
	ok = true;
      else
	{
	  Type* t = expr_type->deref();
	  if (t->array_type() != NULL)
	    {
	      Type* e = t->array_type()->element_type()->forwarded();
	      if (e->integer_type() != NULL
		  && (e == Type::lookup_integer_type("uint8")
		      || e == Type::lookup_integer_type("int")))
		ok = true;
	    }
	}
    }
  else if ((type->is_unsafe_pointer_type()
	    && (expr_type->points_to() != NULL
		|| (expr_type->integer_type() != NULL
		    && (expr_type->integer_type()->bits()
			== static_cast<int>(POINTER_SIZE))
		    && expr_type->integer_type()->is_unsigned())))
	   || (expr_type->is_unsafe_pointer_type()
	       && (type->points_to() != NULL
		   || (type->integer_type() != NULL
		       && (type->integer_type()->bits()
			   == static_cast<int>(POINTER_SIZE))
		       && type->integer_type()->is_unsigned()))))
    {
      // Conversions between unsafe pointers and other pointers or
      // integers of appropriate size are permitted.
      ok = true;
    }

  // FIXME: Conversions between interfaces and pointers are supported.

  if (!ok)
    {
      if (reason.empty())
	this->report_error(_("invalid type conversion"));
      else
	{
	  error_at(this->location(), "invalid type conversion (%s)",
		   reason.c_str());
	  this->set_is_error();
	}
    }
}

// The type conversion is being copied elsewhere.  If we do not call a
// function which creates a new reference, then we need to pass this
// on to the subsidiary expression.

Expression*
Type_conversion_expression::do_being_copied(Refcounts* refcounts,
					    bool for_local)
{
  this->is_being_copied_ = true;
  Type* type = this->type_;
  Type* expr_type = this->expr_->type();
  bool copy_base;
  if (type == expr_type)
    copy_base = true;
  else if (type->interface_type() != NULL
	   || expr_type->interface_type() != NULL)
    copy_base = false;
  else if (type->is_string_type()
	   && (expr_type->integer_type() != NULL
	       || expr_type->deref()->array_type() != NULL))
    copy_base = false;
  else
    copy_base = true;
  if (copy_base && expr_type->has_refcounted_component())
    this->expr_ = this->expr_->being_copied(refcounts, for_local);
  return this;
}

// A type conversion may introduce a reference count.

Expression*
Type_conversion_expression::do_note_decrements(Refcounts* refcounts)
{
  if (this->is_being_copied_)
    return this;
  Type* type = this->type_;
  Type* expr_type = this->expr_->type();
  bool need_decrement;
  if (type == expr_type)
    need_decrement = false;
  else if (type->interface_type() != NULL)
    need_decrement = true;
  else if (expr_type->interface_type() != NULL)
    need_decrement = type->has_refcounted_component();
  else if (type->is_string_type()
	   && (expr_type->integer_type() != NULL
	       || expr_type->deref()->array_type() != NULL))
    need_decrement = true;
  else
    need_decrement = false;
  if (!need_decrement)
    return this;
  return Expression::make_refcount_adjust(refcounts, REFCOUNT_DECREMENT_NEW,
					  this, false);
}

// Get a tree for a type conversion.

tree
Type_conversion_expression::do_get_tree(Translate_context* context)
{
  Gogo* gogo = context->gogo();
  tree type_tree = this->type_->get_tree(gogo);
  tree expr_tree = this->expr_->get_tree(context);

  if (type_tree == error_mark_node
      || expr_tree == error_mark_node
      || TREE_TYPE(expr_tree) == error_mark_node)
    return error_mark_node;

  if (TYPE_MAIN_VARIANT(type_tree) == TYPE_MAIN_VARIANT(TREE_TYPE(expr_tree)))
    return fold_convert(type_tree, expr_tree);

  Type* type = this->type_;
  Type* expr_type = this->expr_->type();
  tree ret;
  if (type->interface_type() != NULL || expr_type->interface_type() != NULL)
    ret = Expression::convert_for_assignment(context, type, expr_type,
					     expr_tree, this->location());
  else if (type->integer_type() != NULL)
    {
      if (expr_type->integer_type() != NULL
	  || expr_type->float_type() != NULL
	  || expr_type->is_unsafe_pointer_type())
	ret = fold(convert_to_integer(type_tree, expr_tree));
      else
	gcc_unreachable();
    }
  else if (type->float_type() != NULL)
    {
      if (expr_type->integer_type() != NULL
	  || expr_type->float_type() != NULL)
	ret = fold(convert_to_real(type_tree, expr_tree));
      else
	gcc_unreachable();
    }
  else if (type->is_string_type()
	   && expr_type->integer_type() != NULL)
    {
      expr_tree = fold_convert(integer_type_node, expr_tree);
      if (host_integerp(expr_tree, 0))
	{
	  HOST_WIDE_INT intval = tree_low_cst(expr_tree, 0);
	  std::string s;
	  Lex::append_char(intval, true, &s, this->location());
	  Expression* se = Expression::make_string(s, this->location());
	  return se->get_tree(context);
	}

      static tree int_to_string_fndecl;
      ret = Gogo::call_builtin(&int_to_string_fndecl,
			       this->location(),
			       "__go_int_to_string",
			       1,
			       type_tree,
			       integer_type_node,
			       fold_convert(integer_type_node, expr_tree));
    }
  else if (type->is_string_type()
	   && (expr_type->array_type() != NULL
	       || (expr_type->points_to() != NULL
		   && expr_type->points_to()->array_type() != NULL)))
    {
      Type* t = expr_type;
      if (t->points_to() != NULL)
	{
	  t = t->points_to();
	  expr_tree = build_fold_indirect_ref(expr_tree);
	}
      if (!DECL_P(expr_tree))
	expr_tree = save_expr(expr_tree);
      Array_type* a = t->array_type();
      Type* e = a->element_type()->forwarded();
      gcc_assert(e->integer_type() != NULL);
      tree valptr = fold_convert(const_ptr_type_node,
				 a->value_pointer_tree(gogo, expr_tree));
      tree len = a->length_tree(gogo, expr_tree);
      if (e->integer_type()->is_unsigned()
	  && e->integer_type()->bits() == 8)
	{
	  static tree byte_array_to_string_fndecl;
	  ret = Gogo::call_builtin(&byte_array_to_string_fndecl,
				   this->location(),
				   "__go_byte_array_to_string",
				   2,
				   type_tree,
				   const_ptr_type_node,
				   valptr,
				   size_type_node,
				   len);
	}
      else
	{
	  gcc_assert(e == Type::lookup_integer_type("int"));
	  static tree int_array_to_string_fndecl;
	  ret = Gogo::call_builtin(&int_array_to_string_fndecl,
				   this->location(),
				   "__go_int_array_to_string",
				   2,
				   type_tree,
				   const_ptr_type_node,
				   valptr,
				   size_type_node,
				   len);
	}
    }
  else if ((type->is_unsafe_pointer_type()
	    && expr_type->points_to() != NULL)
	   || (expr_type->is_unsafe_pointer_type()
	       && type->points_to() != NULL))
    ret = fold_convert(type_tree, expr_tree);
  else if (type->is_unsafe_pointer_type()
	   && expr_type->integer_type() != NULL)
    ret = convert_to_pointer(type_tree, expr_tree);
  else
    ret = Expression::convert_for_assignment(context, type, expr_type,
					     expr_tree, this->location());

  return ret;
}

// Output a type conversion in a constant expression.

void
Type_conversion_expression::do_export(Export* exp) const
{
  exp->write_c_string("convert(");
  exp->write_type(this->type_);
  exp->write_c_string(", ");
  this->expr_->export_expression(exp);
  exp->write_c_string(")");
}

// Import a type conversion or a struct construction.

Expression*
Type_conversion_expression::do_import(Import* imp)
{
  imp->require_c_string("convert(");
  Type* type = imp->read_type();
  imp->require_c_string(", ");
  Expression* val = Expression::import_expression(imp);
  imp->require_c_string(")");
  return Expression::make_cast(type, val, imp->location());
}

// Make a type cast expression.

Expression*
Expression::make_cast(Type* type, Expression* val, source_location location)
{
  if (type->is_error_type() || val->is_error_expression())
    return Expression::make_error(location);
  return new Type_conversion_expression(type, val, location);
}

// Unary expressions.

class Unary_expression : public Expression
{
 public:
  Unary_expression(Operator op, Expression* expr, source_location location)
    : Expression(EXPRESSION_UNARY, location),
      op_(op), escapes_(true), expr_(expr)
  { }

  // Return the operator.
  Operator
  op() const
  { return this->op_; }

  // Return the operand.
  Expression*
  operand() const
  { return this->expr_; }

  // Record that an address expression does not escape.
  void
  set_does_not_escape()
  {
    gcc_assert(this->op_ == OPERATOR_AND);
    this->escapes_ = false;
  }

  // Apply unary opcode OP to UVAL, setting VAL.  Return true if this
  // could be done, false if not.
  static bool
  eval_integer(Operator op, Type* utype, mpz_t uval, mpz_t val,
	       source_location);

  // Apply unary opcode OP to UVAL, setting VAL.  Return true if this
  // could be done, false if not.
  static bool
  eval_float(Operator op, mpfr_t uval, mpfr_t val);

  static Expression*
  do_import(Import*);

 protected:
  int
  do_traverse(Traverse* traverse)
  { return Expression::traverse(&this->expr_, traverse); }

  Expression*
  do_lower(Gogo*, int);

  bool
  do_is_constant() const;

  bool
  do_integer_constant_value(bool, mpz_t, Type**) const;

  bool
  do_float_constant_value(mpfr_t, Type**) const;

  Type*
  do_type();

  void
  do_determine_type(const Type_context*);

  void
  do_check_types(Gogo*);

  Expression*
  do_copy()
  {
    return Expression::make_unary(this->op_, this->expr_->copy(),
				  this->location());
  }

  bool
  do_is_lvalue() const
  { return this->op_ == OPERATOR_MULT; }

  bool
  do_address_taken(source_location, bool);

  Expression*
  do_being_copied(Refcounts*, bool);

  Expression*
  do_note_decrements(Refcounts*);

  Expression*
  do_being_set(Refcounts*);

  tree
  do_get_tree(Translate_context*);

  void
  do_export(Export*) const;

 private:
  // The unary operator to apply.
  Operator op_;
  // Normally true.  False if this is an address expression which does
  // not escape the current function.
  bool escapes_;
  // The operand.
  Expression* expr_;
};

// If we are taking the address of a composite literal, and the
// contents are not constant, then we want to make a heap composite
// instead.

Expression*
Unary_expression::do_lower(Gogo*, int)
{
  source_location loc = this->location();
  Operator op = this->op_;
  Expression* expr = this->expr_;

  if (op == OPERATOR_MULT && expr->is_type_expression())
    return Expression::make_type(Type::make_pointer_type(expr->type()), loc);

  // *&x simplifies to x.  *(*T)(unsafe.Pointer)(&x) does not require
  // *moving x to the heap.  FIXME: Is it worth doing a real escape
  // *analysis here?  This case is found in math/unsafe.go and is
  // *therefore worth special casing.
  if (op == OPERATOR_MULT)
    {
      Expression* e = expr;
      while (e->classification() == EXPRESSION_CONVERSION)
	{
	  Type_conversion_expression* te
	    = static_cast<Type_conversion_expression*>(e);
	  e = te->expr();
	}

      if (e->classification() == EXPRESSION_UNARY)
	{
	  Unary_expression* ue = static_cast<Unary_expression*>(e);
	  if (ue->op_ == OPERATOR_AND)
	    {
	      if (e == expr)
		{
		  // *&x == x.
		  return ue->expr_;
		}
	      ue->set_does_not_escape();
	    }
	}
    }

  if (op == OPERATOR_PLUS || op == OPERATOR_MINUS
      || op == OPERATOR_NOT || op == OPERATOR_XOR)
    {
      Expression* ret = NULL;

      mpz_t eval;
      mpz_init(eval);
      Type* etype;
      if (expr->integer_constant_value(false, eval, &etype))
	{
	  mpz_t val;
	  mpz_init(val);
	  if (Unary_expression::eval_integer(op, etype, eval, val, loc))
	    ret = Expression::make_integer(&val, etype, loc);
	  mpz_clear(val);
	}
      mpz_clear(eval);
      if (ret != NULL)
	return ret;

      if (op == OPERATOR_PLUS || op == OPERATOR_MINUS)
	{
	  mpfr_t fval;
	  mpfr_init(fval);
	  Type* ftype;
	  if (expr->float_constant_value(fval, &ftype))
	    {
	      mpfr_t val;
	      mpfr_init(val);
	      if (Unary_expression::eval_float(op, fval, val))
		ret = Expression::make_float(&val, ftype, loc);
	      mpfr_clear(val);
	    }
	  mpfr_clear(fval);
	  if (ret != NULL)
	    return ret;
	}
    }

  return this;
}

// Return whether a unary expression is a constant.

bool
Unary_expression::do_is_constant() const
{
  if (this->op_ == OPERATOR_MULT)
    {
      // Indirecting through a pointer is only constant if the object
      // to which the expression points is constant, but we currently
      // have no way to determine that.
      return false;
    }
  else if (this->op_ == OPERATOR_AND)
    {
      // Taking the address of a variable is constant if it is a
      // global variable, not constant otherwise.  In other cases
      // taking the address is probably not a constant.
      Var_expression* ve = this->expr_->var_expression();
      if (ve != NULL)
	{
	  Named_object* no = ve->named_object();
	  return no->is_variable() && no->var_value()->is_global();
	}
      return false;
    }
  else
    return this->expr_->is_constant();
}

// Apply unary opcode OP to UVAL, setting VAL.  UTYPE is the type of
// UVAL, if known; it may be NULL.  Return true if this could be done,
// false if not.

bool
Unary_expression::eval_integer(Operator op, Type* utype, mpz_t uval, mpz_t val,
			       source_location location)
{
  switch (op)
    {
    case OPERATOR_PLUS:
      mpz_set(val, uval);
      return true;
    case OPERATOR_MINUS:
      mpz_neg(val, uval);
      return Integer_expression::check_constant(val, utype, location);
    case OPERATOR_NOT:
      mpz_set_ui(val, mpz_cmp_si(uval, 0) == 0 ? 1 : 0);
      return true;
    case OPERATOR_XOR:
      if (utype == NULL
	  || utype->integer_type() == NULL
	  || utype->integer_type()->is_abstract())
	mpz_com(val, uval);
      else
	{
	  // The number of HOST_WIDE_INTs that it takes to represent
	  // UVAL.
	  size_t count = ((mpz_sizeinbase(uval, 2)
			   + HOST_BITS_PER_WIDE_INT
			   - 1)
			  / HOST_BITS_PER_WIDE_INT);

	  unsigned HOST_WIDE_INT* phwi = new unsigned HOST_WIDE_INT[count];
	  memset(phwi, 0, count * sizeof(HOST_WIDE_INT));

	  size_t ecount;
	  mpz_export(phwi, &ecount, -1, sizeof(HOST_WIDE_INT), 0, 0, uval);
	  gcc_assert(ecount <= count);

	  // Trim down to the number of words required by the type.
	  size_t obits = utype->integer_type()->bits();
	  if (!utype->integer_type()->is_unsigned())
	    ++obits;
	  size_t ocount = ((obits + HOST_BITS_PER_WIDE_INT - 1)
			   / HOST_BITS_PER_WIDE_INT);
	  gcc_assert(ocount <= ocount);

	  for (size_t i = 0; i < ocount; ++i)
	    phwi[i] = ~phwi[i];

	  size_t clearbits = ocount * HOST_BITS_PER_WIDE_INT - obits;
	  if (clearbits != 0)
	    phwi[ocount - 1] &= (((unsigned HOST_WIDE_INT) (HOST_WIDE_INT) -1)
				 >> clearbits);

	  mpz_import(val, ocount, -1, sizeof(HOST_WIDE_INT), 0, 0, phwi);

	  delete[] phwi;
	}
      return Integer_expression::check_constant(val, utype, location);
    case OPERATOR_AND:
    case OPERATOR_MULT:
      return false;
    default:
      gcc_unreachable();
    }
}

// Apply unary opcode OP to UVAL, setting VAL.  Return true if this
// could be done, false if not.

bool
Unary_expression::eval_float(Operator op, mpfr_t uval, mpfr_t val)
{
  switch (op)
    {
    case OPERATOR_PLUS:
      mpfr_set(val, uval, GMP_RNDN);
      return true;
    case OPERATOR_MINUS:
      mpfr_neg(val, uval, GMP_RNDN);
      return true;
    case OPERATOR_NOT:
    case OPERATOR_XOR:
    case OPERATOR_AND:
    case OPERATOR_MULT:
      return false;
    default:
      gcc_unreachable();
    }
}

// Return the integral constant value of a unary expression, if it has one.

bool
Unary_expression::do_integer_constant_value(bool iota_is_constant, mpz_t val,
					    Type** ptype) const
{
  mpz_t uval;
  mpz_init(uval);
  bool ret;
  if (!this->expr_->integer_constant_value(iota_is_constant, uval, ptype))
    ret = false;
  else
    ret = Unary_expression::eval_integer(this->op_, *ptype, uval, val,
					 this->location());
  mpz_clear(uval);
  return ret;
}

// Return the floating point constant value of a unary expression, if
// it has one.

bool
Unary_expression::do_float_constant_value(mpfr_t val, Type** ptype) const
{
  mpfr_t uval;
  mpfr_init(uval);
  bool ret;
  if (!this->expr_->float_constant_value(uval, ptype))
    ret = false;
  else
    ret = Unary_expression::eval_float(this->op_, uval, val);
  mpfr_clear(uval);
  return ret;
}

// Return the type of a unary expression.

Type*
Unary_expression::do_type()
{
  switch (this->op_)
    {
    case OPERATOR_PLUS:
    case OPERATOR_MINUS:
    case OPERATOR_NOT:
    case OPERATOR_XOR:
      return this->expr_->type();

    case OPERATOR_AND:
      return Type::make_pointer_type(this->expr_->type());

    case OPERATOR_MULT:
      {
	Type* subtype = this->expr_->type();
	Type* points_to = subtype->points_to();
	if (points_to == NULL)
	  return Type::make_error_type();
	return points_to;
      }

    default:
      gcc_unreachable();
    }
}

// Determine abstract types for a unary expression.

void
Unary_expression::do_determine_type(const Type_context* context)
{
  switch (this->op_)
    {
    case OPERATOR_PLUS:
    case OPERATOR_MINUS:
    case OPERATOR_NOT:
    case OPERATOR_XOR:
      this->expr_->determine_type(context);
      break;

    case OPERATOR_AND:
      // Taking the address of something.
      {
	Type* subtype = (context->type == NULL
			 ? NULL
			 : context->type->points_to());
	Type_context subcontext(subtype, false);
	this->expr_->determine_type(&subcontext);
      }
      break;

    case OPERATOR_MULT:
      // Indirecting through a pointer.
      {
	Type* subtype = (context->type == NULL
			 ? NULL
			 : Type::make_pointer_type(context->type));
	Type_context subcontext(subtype, false);
	this->expr_->determine_type(&subcontext);
      }
      break;

    default:
      gcc_unreachable();
    }
}

// Check types for a unary expression.

void
Unary_expression::do_check_types(Gogo*)
{
  switch (this->op_)
    {
    case OPERATOR_PLUS:
    case OPERATOR_MINUS:
      {
	Type* type = this->expr_->type();
	if (type->integer_type() == NULL
	    && type->float_type() == NULL
	    && !type->is_error_type())
	  this->report_error(_("expected numeric type"));
      }
      break;

    case OPERATOR_NOT:
    case OPERATOR_XOR:
      {
	Type* type = this->expr_->type();
	if (type->integer_type() == NULL
	    && !type->is_boolean_type()
	    && !type->is_error_type())
	  this->report_error(_("expected integer or boolean type"));
      }
      break;

    case OPERATOR_AND:
      if (!this->expr_->address_taken(this->location(), this->escapes_))
	this->set_is_error();
      break;

    case OPERATOR_MULT:
      // Indirecting through a pointer.
      {
	Type* type = this->expr_->type();
	if (type->points_to() == NULL
	    && !type->is_error_type())
	  this->report_error(_("expected pointer"));
      }
      break;

    default:
      gcc_unreachable();
    }
}

// &*p is OK.

bool
Unary_expression::do_address_taken(source_location location, bool)
{
  if (this->op_ == OPERATOR_MULT)
    return true;
  this->report_address_taken_error(location);
  return false;
}

// Copying a unary expression may require incrementing a reference
// count.

Expression*
Unary_expression::do_being_copied(Refcounts* refcounts, bool for_local)
{
  if (!this->type()->has_refcounted_component())
    return this;
  if (this->op_ == OPERATOR_PLUS)
    {
      this->expr_ = this->expr_->being_copied(refcounts, for_local);
      return this;
    }
  else if (this->op_ == OPERATOR_AND && this->is_constant())
    {
      // No need to increment the reference count when the address is
      // a constant.
      return this;
    }
  else
    return Expression::make_refcount_adjust(refcounts,
					    REFCOUNT_INCREMENT_COPIED,
					    this, for_local);
}

// A unary expression does not increment any reference counts, so
// there are no reference counts to decrement afterward.

Expression*
Unary_expression::do_note_decrements(Refcounts*)
{
  return this;
}

// Assigning to *p requires decrementing the reference count of the
// old value.

Expression*
Unary_expression::do_being_set(Refcounts* refcounts)
{
  gcc_assert(this->op_ == OPERATOR_MULT);
  if (!this->type()->has_refcounted_component())
    return this;
  return Expression::make_refcount_decrement_lvalue(refcounts, this);
}

// Get a tree for a unary expression.

tree
Unary_expression::do_get_tree(Translate_context* context)
{
  tree expr = this->expr_->get_tree(context);
  if (expr == error_mark_node)
    return error_mark_node;

  source_location loc = this->location();
  switch (this->op_)
    {
    case OPERATOR_PLUS:
      return expr;

    case OPERATOR_MINUS:
      {
	tree type = TREE_TYPE(expr);
	tree compute_type = excess_precision_type(type);
	if (compute_type != NULL_TREE)
	  expr = convert_to_real(compute_type, expr);
	tree ret = fold_build1_loc(loc, NEGATE_EXPR,
				   (compute_type != NULL_TREE
				    ? compute_type
				    : type),
				   expr);
	if (compute_type != NULL_TREE)
	  ret = convert_to_real(type, ret);
	return ret;
      }

    case OPERATOR_NOT:
      if (TREE_CODE(TREE_TYPE(expr)) == BOOLEAN_TYPE)
	return fold_build1_loc(loc, TRUTH_NOT_EXPR, TREE_TYPE(expr), expr);
      else
	return fold_build2_loc(loc, NE_EXPR, boolean_type_node, expr,
			       build_int_cst(TREE_TYPE(expr), 0));

    case OPERATOR_XOR:
      return fold_build1_loc(loc, BIT_NOT_EXPR, TREE_TYPE(expr), expr);

    case OPERATOR_AND:
      // We should not see a non-constant constructor here; cases
      // where we would see one should have been moved onto the heap
      // at parse time.  Taking the address of a nonconstant
      // constructor will not do what the programmer expects.
      gcc_assert(TREE_CODE(expr) != CONSTRUCTOR || TREE_CONSTANT(expr));

      // Build a decl for a constant constructor.
      if (TREE_CODE(expr) == CONSTRUCTOR && TREE_CONSTANT(expr))
	{
	  tree decl = build_decl(this->location(), VAR_DECL,
				 create_tmp_var_name("C"), TREE_TYPE(expr));
	  DECL_EXTERNAL(decl) = 0;
	  TREE_PUBLIC(decl) = 0;
	  TREE_READONLY(decl) = 1;
	  TREE_CONSTANT(decl) = 1;
	  TREE_STATIC(decl) = 1;
	  DECL_ARTIFICIAL(decl) = 1;
	  DECL_INITIAL(decl) = expr;
	  rest_of_decl_compilation(decl, 1, 0);
	  expr = decl;
	}

      return build_fold_addr_expr_loc(loc, expr);

    case OPERATOR_MULT:
      gcc_assert(POINTER_TYPE_P(TREE_TYPE(expr)));
      return build_fold_indirect_ref_loc(loc, expr);

    default:
      gcc_unreachable();
    }
}

// Export a unary expression.

void
Unary_expression::do_export(Export* exp) const
{
  switch (this->op_)
    {
    case OPERATOR_PLUS:
      exp->write_c_string("+ ");
      break;
    case OPERATOR_MINUS:
      exp->write_c_string("- ");
      break;
    case OPERATOR_NOT:
      exp->write_c_string("! ");
      break;
    case OPERATOR_XOR:
      exp->write_c_string("^ ");
      break;
    case OPERATOR_AND:
    case OPERATOR_MULT:
    default:
      gcc_unreachable();
    }
  this->expr_->export_expression(exp);
}

// Import a unary expression.

Expression*
Unary_expression::do_import(Import* imp)
{
  Operator op;
  switch (imp->get_char())
    {
    case '+':
      op = OPERATOR_PLUS;
      break;
    case '-':
      op = OPERATOR_MINUS;
      break;
    case '!':
      op = OPERATOR_NOT;
      break;
    case '^':
      op = OPERATOR_XOR;
      break;
    default:
      gcc_unreachable();
    }
  imp->require_c_string(" ");
  Expression* expr = Expression::import_expression(imp);
  return Expression::make_unary(op, expr, imp->location());
}

// Make a unary expression.

Expression*
Expression::make_unary(Operator op, Expression* expr, source_location location)
{
  return new Unary_expression(op, expr, location);
}

// Class Binary_expression.

// Traversal.

int
Binary_expression::do_traverse(Traverse* traverse)
{
  int t = Expression::traverse(&this->left_, traverse);
  if (t == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  return Expression::traverse(&this->right_, traverse);
}

// Compare integer constants according to OP.

bool
Binary_expression::compare_integer(Operator op, mpz_t left_val,
				   mpz_t right_val)
{
  int i = mpz_cmp(left_val, right_val);
  switch (op)
    {
    case OPERATOR_EQEQ:
      return i == 0;
    case OPERATOR_NOTEQ:
      return i != 0;
    case OPERATOR_LT:
      return i < 0;
    case OPERATOR_LE:
      return i <= 0;
    case OPERATOR_GT:
      return i > 0;
    case OPERATOR_GE:
      return i >= 0;
    default:
      gcc_unreachable();
    }
}

// Compare floating point constants according to OP.

bool
Binary_expression::compare_float(Operator op, Type* type, mpfr_t left_val,
				 mpfr_t right_val)
{
  int i;
  if (type == NULL)
    i = mpfr_cmp(left_val, right_val);
  else
    {
      mpfr_t lv;
      mpfr_init_set(lv, left_val, GMP_RNDN);
      mpfr_t rv;
      mpfr_init_set(rv, right_val, GMP_RNDN);
      Float_expression::constrain_float(lv, type);
      Float_expression::constrain_float(rv, type);
      i = mpfr_cmp(lv, rv);
      mpfr_clear(lv);
      mpfr_clear(rv);
    }
  switch (op)
    {
    case OPERATOR_EQEQ:
      return i == 0;
    case OPERATOR_NOTEQ:
      return i != 0;
    case OPERATOR_LT:
      return i < 0;
    case OPERATOR_LE:
      return i <= 0;
    case OPERATOR_GT:
      return i > 0;
    case OPERATOR_GE:
      return i >= 0;
    default:
      gcc_unreachable();
    }
}

// Apply binary opcode OP to LEFT_VAL and RIGHT_VAL, setting VAL.
// LEFT_TYPE is the type of LEFT_VAL, RIGHT_TYPE is the type of
// RIGHT_VAL; LEFT_TYPE and/or RIGHT_TYPE may be NULL.  Return true if
// this could be done, false if not.

bool
Binary_expression::eval_integer(Operator op, Type* left_type, mpz_t left_val,
				Type* right_type, mpz_t right_val,
				source_location location, mpz_t val)
{
  bool is_shift_op = false;
  switch (op)
    {
    case OPERATOR_OROR:
    case OPERATOR_ANDAND:
    case OPERATOR_EQEQ:
    case OPERATOR_NOTEQ:
    case OPERATOR_LT:
    case OPERATOR_LE:
    case OPERATOR_GT:
    case OPERATOR_GE:
      // These return boolean values.  We should probably handle them
      // anyhow in case a type conversion is used on the result.
      return false;
    case OPERATOR_PLUS:
      mpz_add(val, left_val, right_val);
      break;
    case OPERATOR_MINUS:
      mpz_sub(val, left_val, right_val);
      break;
    case OPERATOR_OR:
      mpz_ior(val, left_val, right_val);
      break;
    case OPERATOR_XOR:
      mpz_xor(val, left_val, right_val);
      break;
    case OPERATOR_MULT:
      mpz_mul(val, left_val, right_val);
      break;
    case OPERATOR_DIV:
      if (mpz_sgn(right_val) != 0)
	mpz_tdiv_q(val, left_val, right_val);
      else
	{
	  error_at(location, "division by zero");
	  mpz_set_ui(val, 0);
	  return true;
	}
      break;
    case OPERATOR_MOD:
      if (mpz_sgn(right_val) != 0)
	mpz_tdiv_r(val, left_val, right_val);
      else
	{
	  error_at(location, "division by zero");
	  mpz_set_ui(val, 0);
	  return true;
	}
      break;
    case OPERATOR_LSHIFT:
      {
	unsigned long shift = mpz_get_ui(right_val);
	if (mpz_cmp_ui(right_val, shift) != 0)
	  {
	    error_at(location, "shift count overflow");
	    mpz_set_ui(val, 0);
	    return true;
	  }
	mpz_mul_2exp(val, left_val, shift);
	is_shift_op = true;
	break;
      }
      break;
    case OPERATOR_RSHIFT:
      {
	unsigned long shift = mpz_get_ui(right_val);
	if (mpz_cmp_ui(right_val, shift) != 0)
	  {
	    error_at(location, "shift count overflow");
	    mpz_set_ui(val, 0);
	    return true;
	  }
	if (mpz_cmp_ui(left_val, 0) >= 0)
	  mpz_tdiv_q_2exp(val, left_val, shift);
	else
	  mpz_fdiv_q_2exp(val, left_val, shift);
	is_shift_op = true;
	break;
      }
      break;
    case OPERATOR_AND:
      mpz_and(val, left_val, right_val);
      break;
    case OPERATOR_BITCLEAR:
      {
	mpz_t tval;
	mpz_init(tval);
	mpz_com(tval, right_val);
	mpz_and(val, left_val, tval);
	mpz_clear(tval);
      }
      break;
    default:
      gcc_unreachable();
    }

  Type* type = left_type;
  if (!is_shift_op)
    {
      if (type == NULL)
	type = right_type;
      else if (type != right_type && right_type != NULL)
	{
	  if (type->is_abstract())
	    type = right_type;
	  else if (!right_type->is_abstract())
	    {
	      // This look like a type error which should be diagnosed
	      // elsewhere.  Don't do anything here, to avoid an
	      // unhelpful chain of error messages.
	      return true;
	    }
	}
    }

  if (type != NULL && !type->is_abstract())
    {
      // We have to check the operands too, as we have implicitly
      // coerced them to TYPE.
      if ((type != left_type
	   && !Integer_expression::check_constant(left_val, type, location))
	  || (!is_shift_op
	      && type != right_type
	      && !Integer_expression::check_constant(right_val, type,
						     location))
	  || !Integer_expression::check_constant(val, type, location))
	mpz_set_ui(val, 0);
    }

  return true;
}

// Apply binary opcode OP to LEFT_VAL and RIGHT_VAL, setting VAL.
// Return true if this could be done, false if not.

bool
Binary_expression::eval_float(Operator op, Type* left_type, mpfr_t left_val,
			      Type* right_type, mpfr_t right_val,
			      mpfr_t val, source_location location)
{
  switch (op)
    {
    case OPERATOR_OROR:
    case OPERATOR_ANDAND:
    case OPERATOR_EQEQ:
    case OPERATOR_NOTEQ:
    case OPERATOR_LT:
    case OPERATOR_LE:
    case OPERATOR_GT:
    case OPERATOR_GE:
      // These return boolean values.  We should probably handle them
      // anyhow in case a type conversion is used on the result.
      return false;
    case OPERATOR_PLUS:
      mpfr_add(val, left_val, right_val, GMP_RNDN);
      break;
    case OPERATOR_MINUS:
      mpfr_sub(val, left_val, right_val, GMP_RNDN);
      break;
    case OPERATOR_OR:
    case OPERATOR_XOR:
    case OPERATOR_AND:
    case OPERATOR_BITCLEAR:
      return false;
    case OPERATOR_MULT:
      mpfr_mul(val, left_val, right_val, GMP_RNDN);
      break;
    case OPERATOR_DIV:
      if (mpfr_zero_p(right_val))
	error_at(location, "division by zero");
      mpfr_div(val, left_val, right_val, GMP_RNDN);
      break;
    case OPERATOR_MOD:
      return false;
    case OPERATOR_LSHIFT:
    case OPERATOR_RSHIFT:
      return false;
    default:
      gcc_unreachable();
    }

  Type* type = left_type;
  if (type == NULL)
    type = right_type;
  else if (type != right_type && right_type != NULL)
    {
      if (type->is_abstract())
	type = right_type;
      else if (!right_type->is_abstract())
	{
	  // This looks like a type error which should be diagnosed
	  // elsewhere.  Don't do anything here, to avoid an unhelpful
	  // chain of error messages.
	  return true;
	}
    }

  if (type != NULL && !type->is_abstract())
    {
      if ((type != left_type
	   && !Float_expression::check_constant(left_val, type, location))
	  || (type != right_type
	      && !Float_expression::check_constant(right_val, type,
						   location))
	  || !Float_expression::check_constant(val, type, location))
	mpfr_set_ui(val, 0, GMP_RNDN);
    }

  return true;
}

// Lower a binary expression.  We have to evaluate constant
// expressions now, in order t implemented Go's unlimited precision
// constants.

Expression*
Binary_expression::do_lower(Gogo*, int)
{
  source_location location = this->location();
  Operator op = this->op_;
  Expression* left = this->left_;
  Expression* right = this->right_;

  const bool is_comparison = (op == OPERATOR_EQEQ
			      || op == OPERATOR_NOTEQ
			      || op == OPERATOR_LT
			      || op == OPERATOR_LE
			      || op == OPERATOR_GT
			      || op == OPERATOR_GE);

  // Integer constant expressions.
  {
    mpz_t left_val;
    mpz_init(left_val);
    Type* left_type;
    mpz_t right_val;
    mpz_init(right_val);
    Type* right_type;
    if (left->integer_constant_value(false, left_val, &left_type)
	&& right->integer_constant_value(false, right_val, &right_type))
      {
	Expression* ret = NULL;
	if (left_type != right_type
	    && left_type != NULL
	    && right_type != NULL
	    && left_type->base() != right_type->base()
	    && op != OPERATOR_LSHIFT
	    && op != OPERATOR_RSHIFT)
	  {
	    // May be a type error--let it be diagnosed later.
	  }
	else if (is_comparison)
	  {
	    bool b = Binary_expression::compare_integer(op, left_val,
							right_val);
	    ret = Expression::make_cast(Type::lookup_bool_type(),
					Expression::make_boolean(b, location),
					location);
	  }
	else
	  {
	    mpz_t val;
	    mpz_init(val);

	    if (Binary_expression::eval_integer(op, left_type, left_val,
						right_type, right_val,
						location, val))
	      {
		gcc_assert(op != OPERATOR_OROR && op != OPERATOR_ANDAND);
		Type* type;
		if (op == OPERATOR_LSHIFT || op == OPERATOR_RSHIFT)
		  type = left_type;
		else if (left_type == NULL)
		  type = right_type;
		else if (right_type == NULL)
		  type = left_type;
		else if (!left_type->is_abstract()
			 && left_type->named_type() != NULL)
		  type = left_type;
		else if (!right_type->is_abstract()
			 && right_type->named_type() != NULL)
		  type = right_type;
		else if (!left_type->is_abstract())
		  type = left_type;
		else if (!right_type->is_abstract())
		  type = right_type;
		else if (left_type->float_type() != NULL)
		  type = left_type;
		else if (right_type->float_type() != NULL)
		  type = right_type;
		else
		  type = left_type;
		ret = Expression::make_integer(&val, type, location);
	      }

	    mpz_clear(val);
	  }

	if (ret != NULL)
	  {
	    mpz_clear(right_val);
	    mpz_clear(left_val);
	    return ret;
	  }
      }
    mpz_clear(right_val);
    mpz_clear(left_val);
  }

  // Floating point constant expressions.
  {
    mpfr_t left_val;
    mpfr_init(left_val);
    Type* left_type;
    mpfr_t right_val;
    mpfr_init(right_val);
    Type* right_type;
    if (left->float_constant_value(left_val, &left_type)
	&& right->float_constant_value(right_val, &right_type))
      {
	Expression* ret = NULL;
	if (left_type != right_type
	    && left_type != NULL
	    && right_type != NULL
	    && left_type->base() != right_type->base()
	    && op != OPERATOR_LSHIFT
	    && op != OPERATOR_RSHIFT)
	  {
	    // May be a type error--let it be diagnosed later.
	  }
	else if (is_comparison)
	  {
	    bool b = Binary_expression::compare_float(op,
						      (left_type != NULL
						       ? left_type
						       : right_type),
						      left_val, right_val);
	    ret = Expression::make_boolean(b, location);
	  }
	else
	  {
	    mpfr_t val;
	    mpfr_init(val);

	    if (Binary_expression::eval_float(op, left_type, left_val,
					      right_type, right_val, val,
					      location))
	      {
		gcc_assert(op != OPERATOR_OROR && op != OPERATOR_ANDAND
			   && op != OPERATOR_LSHIFT && op != OPERATOR_RSHIFT);
		Type* type;
		if (left_type == NULL)
		  type = right_type;
		else if (right_type == NULL)
		  type = left_type;
		else if (!left_type->is_abstract()
		    && left_type->named_type() != NULL)
		  type = left_type;
		else if (!right_type->is_abstract()
			 && right_type->named_type() != NULL)
		  type = right_type;
		else if (!left_type->is_abstract())
		  type = left_type;
		else if (!right_type->is_abstract())
		  type = right_type;
		else if (left_type->float_type() != NULL)
		  type = left_type;
		else if (right_type->float_type() != NULL)
		  type = right_type;
		else
		  type = left_type;
		ret = Expression::make_float(&val, type, location);
	      }

	    mpfr_clear(val);
	  }

	if (ret != NULL)
	  {
	    mpfr_clear(right_val);
	    mpfr_clear(left_val);
	    return ret;
	  }
      }
    mpfr_clear(right_val);
    mpfr_clear(left_val);
  }

  // String constant expressions.
  if (op == OPERATOR_PLUS
      && left->type()->is_string_type()
      && right->type()->is_string_type())
    {
      std::string left_string;
      std::string right_string;
      if (left->string_constant_value(&left_string)
	  && right->string_constant_value(&right_string))
	return Expression::make_string(left_string + right_string, location);
    }

  return this;
}

// Return the integer constant value, if it has one.

bool
Binary_expression::do_integer_constant_value(bool iota_is_constant, mpz_t val,
					     Type** ptype) const
{
  mpz_t left_val;
  mpz_init(left_val);
  Type* left_type;
  if (!this->left_->integer_constant_value(iota_is_constant, left_val,
					   &left_type))
    {
      mpz_clear(left_val);
      return false;
    }

  mpz_t right_val;
  mpz_init(right_val);
  Type* right_type;
  if (!this->right_->integer_constant_value(iota_is_constant, right_val,
					    &right_type))
    {
      mpz_clear(right_val);
      mpz_clear(left_val);
      return false;
    }

  bool ret;
  if (left_type != right_type
      && left_type != NULL
      && right_type != NULL
      && left_type->base() != right_type->base()
      && this->op_ != OPERATOR_RSHIFT
      && this->op_ != OPERATOR_LSHIFT)
    ret = false;
  else
    ret = Binary_expression::eval_integer(this->op_, left_type, left_val,
					  right_type, right_val,
					  this->location(), val);

  mpz_clear(right_val);
  mpz_clear(left_val);

  if (ret)
    *ptype = (this->op_ == OPERATOR_OROR || this->op_ == OPERATOR_OROR
	      ? right_type
	      : left_type);

  return ret;
}

// Return the floating point constant value, if it has one.

bool
Binary_expression::do_float_constant_value(mpfr_t val, Type** ptype) const
{
  mpfr_t left_val;
  mpfr_init(left_val);
  Type* left_type;
  if (!this->left_->float_constant_value(left_val, &left_type))
    {
      mpfr_clear(left_val);
      return false;
    }

  mpfr_t right_val;
  mpfr_init(right_val);
  Type* right_type;
  if (!this->right_->float_constant_value(right_val, &right_type))
    {
      mpfr_clear(right_val);
      mpfr_clear(left_val);
      return false;
    }

  bool ret;
  if (left_type != right_type
      && left_type != NULL
      && right_type != NULL
      && left_type->base() != right_type->base())
    ret = false;
  else
    ret = Binary_expression::eval_float(this->op_, left_type, left_val,
					right_type, right_val,
					val, this->location());

  mpfr_clear(left_val);
  mpfr_clear(right_val);

  if (ret)
    *ptype = (this->op_ == OPERATOR_OROR || this->op_ == OPERATOR_OROR
	      ? right_type
	      : left_type);

  return ret;
}

// Note that the value is being discarded.

void
Binary_expression::do_discarding_value()
{
  if (this->op_ == OPERATOR_OROR || this->op_ == OPERATOR_ANDAND)
    this->right_->discarding_value();
  else
    this->warn_unused_value();
}

// Get type.

Type*
Binary_expression::do_type()
{
  switch (this->op_)
    {
    case OPERATOR_OROR:
    case OPERATOR_ANDAND:
    case OPERATOR_EQEQ:
    case OPERATOR_NOTEQ:
    case OPERATOR_LT:
    case OPERATOR_LE:
    case OPERATOR_GT:
    case OPERATOR_GE:
      return Type::lookup_bool_type();

    case OPERATOR_PLUS:
    case OPERATOR_MINUS:
    case OPERATOR_OR:
    case OPERATOR_XOR:
    case OPERATOR_MULT:
    case OPERATOR_DIV:
    case OPERATOR_MOD:
    case OPERATOR_AND:
    case OPERATOR_BITCLEAR:
      {
	Type* left_type = this->left_->type();
	Type* right_type = this->right_->type();
	if (!left_type->is_abstract() && left_type->named_type() != NULL)
	  return left_type;
	else if (!right_type->is_abstract() && right_type->named_type() != NULL)
	  return right_type;
	else if (!left_type->is_abstract())
	  return left_type;
	else if (!right_type->is_abstract())
	  return right_type;
	else if (left_type->float_type() != NULL)
	  return left_type;
	else if (right_type->float_type() != NULL)
	  return right_type;
	else
	  return left_type;
      }

    case OPERATOR_LSHIFT:
    case OPERATOR_RSHIFT:
      return this->left_->type();

    default:
      gcc_unreachable();
    }
}

// Set type for a binary expression.

void
Binary_expression::do_determine_type(const Type_context* context)
{
  Type* tleft = this->left_->type();
  Type* tright = this->right_->type();

  // Both sides should have the same type, except for the shift
  // operations.  For a comparison, we should ignore the incoming
  // type.

  bool is_shift_op = (this->op_ == OPERATOR_LSHIFT
		      || this->op_ == OPERATOR_RSHIFT);

  bool is_comparison = (this->op_ == OPERATOR_EQEQ
			|| this->op_ == OPERATOR_NOTEQ
			|| this->op_ == OPERATOR_LT
			|| this->op_ == OPERATOR_LE
			|| this->op_ == OPERATOR_GT
			|| this->op_ == OPERATOR_GE);

  Type_context subcontext(*context);

  if (is_comparison)
    {
      // In a comparison, the context does not determine the types of
      // the operands.
      subcontext.type = NULL;
    }

  // Set the context for the left hand operand.
  if (is_shift_op)
    {
      // The right hand operand plays no role in determining the type
      // of the left hand operand.  A shift of an abstract integer in
      // a string context gets special treatment, which may be a
      // language bug.
      if (subcontext.type != NULL
	  && subcontext.type->is_string_type()
	  && tleft->is_abstract())
	error_at(this->location(), "shift of non-integer operand");
    }
  else if (!tleft->is_abstract())
    subcontext.type = tleft;
  else if (!tright->is_abstract())
    subcontext.type = tright;
  else if (subcontext.type == NULL)
    {
      if ((tleft->integer_type() != NULL && tright->integer_type() != NULL)
	  || (tleft->float_type() != NULL && tright->float_type() != NULL))
	{
	  // Both sides have an abstract integer type or both sides
	  // have an abstract float type.  Just let CONTEXT determine
	  // whether they may remain abstract or not.
	}
      else
	{
	  // Both sides are abstract, but one is integer and one is
	  // floating point.  Convert the abstract integer to floating
	  // point.
	  subcontext.type = tleft->float_type() != NULL ? tleft : tright;
	}
    }

  this->left_->determine_type(&subcontext);

  // The context for the right hand operand is the same as for the
  // left hand operand, except for a shift operator.
  if (is_shift_op)
    {
      subcontext.type = Type::lookup_integer_type("uint");
      subcontext.may_be_abstract = false;
    }

  this->right_->determine_type(&subcontext);
}

// Report an error if the binary operator OP does not support TYPE.
// Return whether the operation is OK.  This should not be used for
// shift.

bool
Binary_expression::check_operator_type(Operator op, Type* type,
				       source_location location)
{
  switch (op)
    {
    case OPERATOR_OROR:
    case OPERATOR_ANDAND:
      if (!type->is_boolean_type())
	{
	  error_at(location, _("expected boolean type"));
	  return false;
	}
      break;

    case OPERATOR_EQEQ:
    case OPERATOR_NOTEQ:
      if (type->integer_type() == NULL
	  && type->float_type() == NULL
	  && !type->is_string_type()
	  && type->points_to() == NULL
	  && !type->is_nil_type()
	  && !type->is_boolean_type()
	  && type->interface_type() == NULL
	  && (type->array_type() == NULL
	      || type->array_type()->length() != NULL)
	  && type->map_type() == NULL
	  && type->channel_type() == NULL
	  && type->function_type() == NULL)
	{
	  error_at(location,
		   _("expected integer, floating, string, pointer, boolean, "
		     "interface, slice, map, channel, or function type"));
	  return false;
	}
      break;

    case OPERATOR_LT:
    case OPERATOR_LE:
    case OPERATOR_GT:
    case OPERATOR_GE:
    case OPERATOR_PLUS:
    case OPERATOR_PLUSEQ:
      if (type->integer_type() == NULL
	  && type->float_type() == NULL
	  && !type->is_string_type())
	{
	  error_at(location,
		   _("expected integer, floating, or string type"));
	  return false;
	}
      break;

    case OPERATOR_MINUS:
    case OPERATOR_MINUSEQ:
    case OPERATOR_MULT:
    case OPERATOR_MULTEQ:
    case OPERATOR_DIV:
    case OPERATOR_DIVEQ:
      if (type->integer_type() == NULL
	  && type->float_type() == NULL)
	{
	  error_at(location, _("expected integer or floating type"));
	  return false;
	}
      break;

    case OPERATOR_MOD:
    case OPERATOR_MODEQ:
    case OPERATOR_OR:
    case OPERATOR_OREQ:
    case OPERATOR_AND:
    case OPERATOR_ANDEQ:
    case OPERATOR_XOR:
    case OPERATOR_XOREQ:
    case OPERATOR_BITCLEAR:
    case OPERATOR_BITCLEAREQ:
      if (type->integer_type() == NULL)
	{
	  error_at(location, _("expected integer type"));
	  return false;
	}
      break;

    default:
      gcc_unreachable();
    }

  return true;
}

// Check types.

void
Binary_expression::do_check_types(Gogo*)
{
  if (this->op_ != OPERATOR_LSHIFT && this->op_ != OPERATOR_RSHIFT)
    {
      Type* type = this->left_->type();

      if (!Type::are_compatible_for_binop(type, this->right_->type()))
	{
	  this->report_error(_("incompatible types in binary expression"));
	  return;
	}

      if (!type->is_error_type()
	  && !this->right_->type()->is_error_type())
	{
	  if (!Binary_expression::check_operator_type(this->op_, type,
						      this->location()))
	    this->set_is_error();
	}
    }
  else
    {
      if (this->left_->type()->integer_type() == NULL
	  && !this->left_->type()->is_error_type())
	this->report_error(_("shift of non-integer operand"));

      Type* shift_type = this->right_->type();
      if (!shift_type->is_error_type())
	{
	  if (!shift_type->is_abstract()
	      && (shift_type->integer_type() == NULL
		  || !shift_type->integer_type()->is_unsigned()))
	    this->report_error(_("shift count not unsigned integer"));
	  else
	    {
	      mpz_t val;
	      mpz_init(val);
	      Type* type;
	      if (this->right_->integer_constant_value(true, val, &type))
		{
		  if (mpz_sgn(val) < 0)
		    this->report_error(_("negative shift count"));
		}
	      mpz_clear(val);
	    }
	}
    }
}

// Copying a binary expression never requires incrementing a reference
// count, but we do have to disable incrementing the reference count.

Expression*
Binary_expression::do_being_copied(Refcounts*, bool)
{
  this->is_being_copied_ = true;
  return this;
}

// A binary expression increments a reference count when adding
// strings.

Expression*
Binary_expression::do_note_decrements(Refcounts* refcounts)
{
  if (this->op_ != OPERATOR_PLUS
      || !this->left_->type()->is_string_type()
      || this->is_being_copied_)
    return this;
  return Expression::make_refcount_adjust(refcounts,
					  REFCOUNT_DECREMENT_NEW,
					  this, false);
}

// Get a tree for a binary expression.

tree
Binary_expression::do_get_tree(Translate_context* context)
{
  tree left = this->left_->get_tree(context);
  tree right = this->right_->get_tree(context);

  if (left == error_mark_node || right == error_mark_node)
    return error_mark_node;

  enum tree_code code;
  bool use_left_type = true;
  bool is_shift_op = false;
  switch (this->op_)
    {
    case OPERATOR_EQEQ:
    case OPERATOR_NOTEQ:
    case OPERATOR_LT:
    case OPERATOR_LE:
    case OPERATOR_GT:
    case OPERATOR_GE:
      return Expression::comparison_tree(context, this->op_,
					 this->left_->type(), left,
					 this->right_->type(), right,
					 this->location());

    case OPERATOR_OROR:
      code = TRUTH_ORIF_EXPR;
      use_left_type = false;
      break;
    case OPERATOR_ANDAND:
      code = TRUTH_ANDIF_EXPR;
      use_left_type = false;
      break;
    case OPERATOR_PLUS:
      code = PLUS_EXPR;
      break;
    case OPERATOR_MINUS:
      code = MINUS_EXPR;
      break;
    case OPERATOR_OR:
      code = BIT_IOR_EXPR;
      break;
    case OPERATOR_XOR:
      code = BIT_XOR_EXPR;
      break;
    case OPERATOR_MULT:
      code = MULT_EXPR;
      break;
    case OPERATOR_DIV:
      // FIXME: Code depends on whether integer or floating point.
      code = TRUNC_DIV_EXPR;
      break;
    case OPERATOR_MOD:
      // FIXME: Code depends on whether integer or floating point.
      code = TRUNC_MOD_EXPR;
      break;
    case OPERATOR_LSHIFT:
      code = LSHIFT_EXPR;
      is_shift_op = true;
      break;
    case OPERATOR_RSHIFT:
      code = RSHIFT_EXPR;
      is_shift_op = true;
      break;
    case OPERATOR_AND:
      code = BIT_AND_EXPR;
      break;
    case OPERATOR_BITCLEAR:
      right = fold_build1(BIT_NOT_EXPR, TREE_TYPE(right), right);
      code = BIT_AND_EXPR;
      break;
    default:
      gcc_unreachable();
    }

  tree type = use_left_type ? TREE_TYPE(left) : TREE_TYPE(right);

  if (this->left_->type()->is_string_type())
    {
      gcc_assert(this->op_ == OPERATOR_PLUS);
      tree string_type = Type::make_string_type()->get_tree(context->gogo());
      static tree string_plus_decl;
      return Gogo::call_builtin(&string_plus_decl,
				this->location(),
				"__go_string_plus",
				2,
				string_type,
				string_type,
				left,
				string_type,
				right);
    }

  tree compute_type = excess_precision_type(type);
  if (compute_type != NULL_TREE)
    {
      left = convert_to_real(compute_type, left);
      right = convert_to_real(compute_type, right);
    }

  tree eval_saved = NULL_TREE;
  if (is_shift_op)
    {
      if (!DECL_P(left))
	left = save_expr(left);
      if (!DECL_P(right))
	right = save_expr(right);
      // Make sure the values are evaluated.
      eval_saved = fold_build2_loc(this->location(), COMPOUND_EXPR,
				   void_type_node, left, right);
    }

  tree ret = fold_build2_loc(this->location(),
			     code,
			     compute_type != NULL_TREE ? compute_type : type,
			     left, right);

  if (compute_type != NULL_TREE)
    ret = convert_to_real(type, ret);

  // In Go, a shift larger than the size of the type is well-defined.
  // This is not true in GENERIC, so we need to insert a conditional.
  if (is_shift_op)
    {
      gcc_assert(INTEGRAL_TYPE_P(TREE_TYPE(left)));
      gcc_assert(this->left_->type()->integer_type() != NULL);
      int bits = TYPE_PRECISION(TREE_TYPE(left));

      tree compare = fold_build2(LT_EXPR, boolean_type_node, right,
				 build_int_cst_type(TREE_TYPE(right), bits));

      tree overflow_result = fold_convert_loc(this->location(),
					      TREE_TYPE(left),
					      integer_zero_node);
      if (this->op_ == OPERATOR_RSHIFT
	  && !this->left_->type()->integer_type()->is_unsigned())
	{
	  tree neg = fold_build2_loc(this->location(), LT_EXPR,
				     boolean_type_node, left,
				     fold_convert_loc(this->location(),
						      TREE_TYPE(left),
						      integer_zero_node));
	  tree neg_one = fold_build2_loc(this->location(),
					 MINUS_EXPR, TREE_TYPE(left),
					 fold_convert_loc(this->location(),
							  TREE_TYPE(left),
							  integer_zero_node),
					 fold_convert_loc(this->location(),
							  TREE_TYPE(left),
							  integer_one_node));
	  overflow_result = fold_build3_loc(this->location(), COND_EXPR,
					    TREE_TYPE(left), neg, neg_one,
					    overflow_result);
	}

      ret = fold_build3_loc(this->location(), COND_EXPR, TREE_TYPE(left),
			    compare, ret, overflow_result);

      ret = fold_build2_loc(this->location(), COMPOUND_EXPR,
			    TREE_TYPE(ret), eval_saved, ret);
    }

  return ret;
}

// Export a binary expression.

void
Binary_expression::do_export(Export* exp) const
{
  exp->write_c_string("(");
  this->left_->export_expression(exp);
  switch (this->op_)
    {
    case OPERATOR_OROR:
      exp->write_c_string(" || ");
      break;
    case OPERATOR_ANDAND:
      exp->write_c_string(" && ");
      break;
    case OPERATOR_EQEQ:
      exp->write_c_string(" == ");
      break;
    case OPERATOR_NOTEQ:
      exp->write_c_string(" != ");
      break;
    case OPERATOR_LT:
      exp->write_c_string(" < ");
      break;
    case OPERATOR_LE:
      exp->write_c_string(" <= ");
      break;
    case OPERATOR_GT:
      exp->write_c_string(" > ");
      break;
    case OPERATOR_GE:
      exp->write_c_string(" >= ");
      break;
    case OPERATOR_PLUS:
      exp->write_c_string(" + ");
      break;
    case OPERATOR_MINUS:
      exp->write_c_string(" - ");
      break;
    case OPERATOR_OR:
      exp->write_c_string(" | ");
      break;
    case OPERATOR_XOR:
      exp->write_c_string(" ^ ");
      break;
    case OPERATOR_MULT:
      exp->write_c_string(" * ");
      break;
    case OPERATOR_DIV:
      exp->write_c_string(" / ");
      break;
    case OPERATOR_MOD:
      exp->write_c_string(" % ");
      break;
    case OPERATOR_LSHIFT:
      exp->write_c_string(" << ");
      break;
    case OPERATOR_RSHIFT:
      exp->write_c_string(" >> ");
      break;
    case OPERATOR_AND:
      exp->write_c_string(" & ");
      break;
    case OPERATOR_BITCLEAR:
      exp->write_c_string(" &^ ");
      break;
    default:
      gcc_unreachable();
    }
  this->right_->export_expression(exp);
  exp->write_c_string(")");
}

// Import a binary expression.

Expression*
Binary_expression::do_import(Import* imp)
{
  imp->require_c_string("(");

  Expression* left = Expression::import_expression(imp);

  Operator op;
  if (imp->match_c_string(" || "))
    {
      op = OPERATOR_OROR;
      imp->advance(4);
    }
  else if (imp->match_c_string(" && "))
    {
      op = OPERATOR_ANDAND;
      imp->advance(4);
    }
  else if (imp->match_c_string(" == "))
    {
      op = OPERATOR_EQEQ;
      imp->advance(4);
    }
  else if (imp->match_c_string(" != "))
    {
      op = OPERATOR_NOTEQ;
      imp->advance(4);
    }
  else if (imp->match_c_string(" < "))
    {
      op = OPERATOR_LT;
      imp->advance(3);
    }
  else if (imp->match_c_string(" <= "))
    {
      op = OPERATOR_LE;
      imp->advance(4);
    }
  else if (imp->match_c_string(" > "))
    {
      op = OPERATOR_GT;
      imp->advance(3);
    }
  else if (imp->match_c_string(" >= "))
    {
      op = OPERATOR_GE;
      imp->advance(4);
    }
  else if (imp->match_c_string(" + "))
    {
      op = OPERATOR_PLUS;
      imp->advance(3);
    }
  else if (imp->match_c_string(" - "))
    {
      op = OPERATOR_MINUS;
      imp->advance(3);
    }
  else if (imp->match_c_string(" | "))
    {
      op = OPERATOR_OR;
      imp->advance(3);
    }
  else if (imp->match_c_string(" ^ "))
    {
      op = OPERATOR_XOR;
      imp->advance(3);
    }
  else if (imp->match_c_string(" * "))
    {
      op = OPERATOR_MULT;
      imp->advance(3);
    }
  else if (imp->match_c_string(" / "))
    {
      op = OPERATOR_DIV;
      imp->advance(3);
    }
  else if (imp->match_c_string(" % "))
    {
      op = OPERATOR_MOD;
      imp->advance(3);
    }
  else if (imp->match_c_string(" << "))
    {
      op = OPERATOR_LSHIFT;
      imp->advance(4);
    }
  else if (imp->match_c_string(" >> "))
    {
      op = OPERATOR_RSHIFT;
      imp->advance(4);
    }
  else if (imp->match_c_string(" & "))
    {
      op = OPERATOR_AND;
      imp->advance(3);
    }
  else if (imp->match_c_string(" &^ "))
    {
      op = OPERATOR_BITCLEAR;
      imp->advance(4);
    }
  else
    {
      error_at(imp->location(), "unrecognized binary operator");
      return Expression::make_error(imp->location());
    }

  Expression* right = Expression::import_expression(imp);

  imp->require_c_string(")");

  return Expression::make_binary(op, left, right, imp->location());
}

// Make a binary expression.

Expression*
Expression::make_binary(Operator op, Expression* left, Expression* right,
			source_location location)
{
  return new Binary_expression(op, left, right, location);
}

// Implement a comparison.

tree
Expression::comparison_tree(Translate_context* context, Operator op,
			    Type* left_type, tree left_tree,
			    Type* right_type, tree right_tree,
			    source_location location)
{
  enum tree_code code;
  switch (op)
    {
    case OPERATOR_EQEQ:
      code = EQ_EXPR;
      break;
    case OPERATOR_NOTEQ:
      code = NE_EXPR;
      break;
    case OPERATOR_LT:
      code = LT_EXPR;
      break;
    case OPERATOR_LE:
      code = LE_EXPR;
      break;
    case OPERATOR_GT:
      code = GT_EXPR;
      break;
    case OPERATOR_GE:
      code = GE_EXPR;
      break;
    default:
      gcc_unreachable();
    }

  if (left_type->is_string_type())
    {
      gcc_assert(right_type->is_string_type());
      tree string_type = Type::make_string_type()->get_tree(context->gogo());
      static tree string_compare_decl;
      left_tree = Gogo::call_builtin(&string_compare_decl,
				     location,
				     "__go_strcmp",
				     2,
				     integer_type_node,
				     string_type,
				     left_tree,
				     string_type,
				     right_tree);
      right_tree = build_int_cst_type(integer_type_node, 0);
    }

  if (left_type->interface_type() != NULL
      && right_type->interface_type() != NULL)
    {
      static tree interface_compare_decl;
      left_tree = Gogo::call_builtin(&interface_compare_decl,
				     location,
				     "__go_interface_compare",
				     2,
				     integer_type_node,
				     const_ptr_type_node,
				     fold_convert(const_ptr_type_node,
						  left_tree),
				     const_ptr_type_node,
				     fold_convert(const_ptr_type_node,
						  right_tree));
      right_tree = build_int_cst_type(integer_type_node, 0);
    }

  if (left_type->is_nil_type()
      && (op == OPERATOR_EQEQ || op == OPERATOR_NOTEQ))
    {
      std::swap(left_type, right_type);
      std::swap(left_tree, right_tree);
    }

  if (right_type->is_nil_type())
    {
      if (left_type->array_type() != NULL
	  && left_type->array_type()->length() == NULL)
	{
	  Array_type* at = left_type->array_type();
	  left_tree = at->value_pointer_tree(context->gogo(), left_tree);
	  right_tree = fold_convert(TREE_TYPE(left_tree), null_pointer_node);
	}
      else
	{
	  gcc_assert(POINTER_TYPE_P(TREE_TYPE(left_tree)));
	  right_tree = fold_convert(TREE_TYPE(left_tree), null_pointer_node);
	}
    }

  tree ret = fold_build2(code, boolean_type_node, left_tree, right_tree);
  if (CAN_HAVE_LOCATION_P(ret))
    SET_EXPR_LOCATION(ret, location);
  return ret;
}

// Class Bound_method_expression.

// Traversal.

int
Bound_method_expression::do_traverse(Traverse* traverse)
{
  if (Expression::traverse(&this->expr_, traverse) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  return Expression::traverse(&this->method_, traverse);
}

// Return the type of a bound method expression.  The type of this
// object is really the type of the method with no receiver.  We
// should be able to get away with just returning the type of the
// method.

Type*
Bound_method_expression::do_type()
{
  return this->method_->type();
}

// Determine the types of a method expression.

void
Bound_method_expression::do_determine_type(const Type_context*)
{
  this->method_->determine_type_no_context();
  Type* mtype = this->method_->type();
  Function_type* fntype = mtype == NULL ? NULL : mtype->function_type();
  if (fntype == NULL || !fntype->is_method())
    this->expr_->determine_type_no_context();
  else
    {
      Type_context subcontext(fntype->receiver()->type(), false);
      this->expr_->determine_type(&subcontext);
    }
}

// Check the types of a method expression.

void
Bound_method_expression::do_check_types(Gogo*)
{
  Type* type = this->method_->type()->deref();
  if (type == NULL
      || type->function_type() == NULL
      || !type->function_type()->is_method())
    this->report_error(_("object is not a method"));
  else
    {
      Type* rtype = type->function_type()->receiver()->type()->deref();
      Type* etype = (this->expr_type_ != NULL
		     ? this->expr_type_
		     : this->expr_->type());
      etype = etype->deref();
      if (!Type::are_identical(rtype, etype))
	this->report_error(_("method type does not match object type"));
    }
}

// Get the tree for a method expression.  There is no standard tree
// representation for this.  The only places it may currently be used
// are in a Call_expression or a Go_statement, which will take it
// apart directly.  So this has nothing to do at present.

tree
Bound_method_expression::do_get_tree(Translate_context*)
{
  gcc_unreachable();
}

// Make a method expression.

Bound_method_expression*
Expression::make_bound_method(Expression* expr, Expression* method,
			      source_location location)
{
  return new Bound_method_expression(expr, method, location);
}

// Class Builtin_call_expression.  This is used for a call to a
// builtin function.

class Builtin_call_expression : public Call_expression
{
 public:
  Builtin_call_expression(Gogo* gogo, Expression* fn, Expression_list* args,
			  source_location location);

 protected:
  // This overrides Call_expression::do_lower.
  Expression*
  do_lower(Gogo*, int);

  bool
  do_is_constant() const;

  bool
  do_integer_constant_value(bool, mpz_t, Type**) const;

  Type*
  do_type();

  void
  do_determine_type(const Type_context*);

  void
  do_check_types(Gogo*);

  Expression*
  do_copy()
  {
    return new Builtin_call_expression(this->gogo_, this->fn()->copy(),
				       this->args()->copy(),
				       this->location());
  }

  tree
  do_get_tree(Translate_context*);

  void
  do_export(Export*) const;

 private:
  // The builtin functions.
  enum Builtin_function_code
    {
      BUILTIN_INVALID,

      // Predeclared builtin functions.
      BUILTIN_CAP,
      BUILTIN_CLOSE,
      BUILTIN_CLOSED,
      BUILTIN_LEN,
      BUILTIN_MAKE,
      BUILTIN_NEW,
      BUILTIN_PANIC,
      BUILTIN_PANICLN,
      BUILTIN_PRINT,
      BUILTIN_PRINTLN,

      // Builtin functions from the unsafe package.
      BUILTIN_ALIGNOF,
      BUILTIN_OFFSETOF,
      BUILTIN_SIZEOF,

      // gccgo specific builtin functions.
      BUILTIN_SQRT
    };

  Expression*
  one_arg() const;

  bool
  check_one_arg();

  // A pointer back to the general IR structure.  This avoids a global
  // variable, or passing it around everywhere.
  Gogo* gogo_;
  // The builtin function being called.
  Builtin_function_code code_;
};

Builtin_call_expression::Builtin_call_expression(Gogo* gogo,
						 Expression* fn,
						 Expression_list* args,
						 source_location location)
  : Call_expression(fn, args, location),
    gogo_(gogo), code_(BUILTIN_INVALID)
{
  Func_expression* fnexp = this->fn()->func_expression();
  gcc_assert(fnexp != NULL);
  const std::string& name(fnexp->named_object()->name());
  if (name == "cap")
    this->code_ = BUILTIN_CAP;
  else if (name == "close")
    this->code_ = BUILTIN_CLOSE;
  else if (name == "closed")
    this->code_ = BUILTIN_CLOSED;
  else if (name == "len")
    this->code_ = BUILTIN_LEN;
  else if (name == "make")
    this->code_ = BUILTIN_MAKE;
  else if (name == "new")
    this->code_ = BUILTIN_NEW;
  else if (name == "panic")
    this->code_ = BUILTIN_PANIC;
  else if (name == "panicln")
    this->code_ = BUILTIN_PANICLN;
  else if (name == "print")
    this->code_ = BUILTIN_PRINT;
  else if (name == "println")
    this->code_ = BUILTIN_PRINTLN;
  else if (name == "Alignof")
    this->code_ = BUILTIN_ALIGNOF;
  else if (name == "Offsetof")
    this->code_ = BUILTIN_OFFSETOF;
  else if (name == "Sizeof")
    this->code_ = BUILTIN_SIZEOF;
  else if (name == "__builtin_sqrt")
    this->code_ = BUILTIN_SQRT;
  else
    gcc_unreachable();
}

// Lower a builtin call expression.  This turns new and make into
// specific expressions.

Expression*
Builtin_call_expression::do_lower(Gogo*, int)
{
  if (this->code_ == BUILTIN_NEW)
    {
      const Expression_list* args = this->args();
      if (args == NULL || args->size() < 1)
	this->report_error(_("not enough arguments"));
      else if (args->size() > 1)
	this->report_error(_("too many arguments"));
      else
	{
	  Expression* arg = args->front();
	  if (!arg->is_type_expression())
	    {
	      error_at(arg->location(), _("expected type"));
	      this->set_is_error();
	    }
	  else
	    return Expression::make_allocation(arg->type(), this->location());
	}
    }
  else if (this->code_ == BUILTIN_MAKE)
    {
      const Expression_list* args = this->args();
      if (args == NULL || args->size() < 1)
	this->report_error(_("not enough arguments"));
      else
	{
	  Expression* arg = args->front();
	  if (!arg->is_type_expression())
	    {
	      error_at(arg->location(), _("expected type"));
	      this->set_is_error();
	    }
	  else
	    {
	      Expression_list* newargs;
	      if (args->size() == 1)
		newargs = NULL;
	      else
		{
		  newargs = new Expression_list();
		  Expression_list::const_iterator p = args->begin();
		  ++p;
		  for (; p != args->end(); ++p)
		    newargs->push_back(*p);
		}
	      return Expression::make_make(arg->type(), newargs,
					   this->location());
	    }
	}
    }

  return this;
}

// Return a single argument, or NULL if there isn't one.

Expression*
Builtin_call_expression::one_arg() const
{
  const Expression_list* args = this->args();
  if (args->size() != 1)
    return NULL;
  return args->front();
}

// Return whether this is constant: len of a string, or len or cap of
// a fixed array, or unsafe.Sizeof, unsafe.Offsetof, unsafe.Alignof.

bool
Builtin_call_expression::do_is_constant() const
{
  if (this->code_ == BUILTIN_LEN
      || this->code_ == BUILTIN_CAP)
    {
      Expression* arg = this->one_arg();
      if (arg == NULL)
	return false;
      Type* arg_type = arg->type();

      if (arg_type->points_to() != NULL
	  && arg_type->points_to()->array_type() != NULL
	  && !arg_type->points_to()->is_open_array_type())
	arg_type = arg_type->points_to();

      if (arg_type->array_type() != NULL
	  && arg_type->array_type()->length() != NULL)
	return arg_type->array_type()->length()->is_constant();

      if (this->code_ == BUILTIN_LEN && arg_type->is_string_type())
	return arg->is_constant();
    }
  else if (this->code_ == BUILTIN_SIZEOF
	   || this->code_ == BUILTIN_ALIGNOF)
    return this->one_arg() != NULL;
  else if (this->code_ == BUILTIN_OFFSETOF)
    {
      Expression* arg = this->one_arg();
      if (arg == NULL)
	return false;
      return arg->field_reference_expression() != NULL;
    }
  return false;
}

// Return an integer constant value if possible.

bool
Builtin_call_expression::do_integer_constant_value(bool iota_is_constant,
						   mpz_t val,
						   Type** ptype) const
{
  if (this->code_ == BUILTIN_LEN
      || this->code_ == BUILTIN_CAP)
    {
      Expression* arg = this->one_arg();
      if (arg == NULL)
	return false;
      Type* arg_type = arg->type();

      if (this->code_ == BUILTIN_LEN
	  && arg_type->is_string_type()
	  && arg->classification() == EXPRESSION_STRING)
	{
	  String_expression* se = static_cast<String_expression*>(arg);
	  mpz_set_ui(val, se->val().length());
	  *ptype = Type::lookup_integer_type("int");
	  return true;
	}

      if (arg_type->points_to() != NULL
	  && arg_type->points_to()->array_type() != NULL
	  && !arg_type->points_to()->is_open_array_type())
	arg_type = arg_type->points_to();

      if (arg_type->array_type() != NULL
	  && arg_type->array_type()->length() != NULL)
	{
	  Expression* e = arg_type->array_type()->length();
	  if (e->integer_constant_value(iota_is_constant, val, ptype))
	    {
	      *ptype = Type::lookup_integer_type("int");
	      return true;
	    }
	}
    }
  else if (this->code_ == BUILTIN_SIZEOF
	   || this->code_ == BUILTIN_ALIGNOF)
    {
      Expression* arg = this->one_arg();
      if (arg == NULL)
	return false;
      Type* arg_type = arg->type();
      if (arg_type->is_error_type())
	return false;
      if (arg_type->is_abstract())
	return false;
      tree arg_type_tree = arg_type->get_tree(this->gogo_);
      unsigned long val_long;
      if (this->code_ == BUILTIN_SIZEOF)
	{
	  tree type_size = TYPE_SIZE_UNIT(arg_type_tree);
	  gcc_assert(TREE_CODE(type_size) == INTEGER_CST);
	  if (TREE_INT_CST_HIGH(type_size) != 0)
	    return false;
	  unsigned HOST_WIDE_INT val_wide = TREE_INT_CST_LOW(type_size);
	  val_long = static_cast<unsigned long>(val_wide);
	  if (val_long != val_wide)
	    return false;
	}
      else if (this->code_ == BUILTIN_ALIGNOF)
	{
	  val_long = TYPE_ALIGN(arg_type_tree);
	  if (arg->field_reference_expression() != NULL)
	    {
	      // Calling unsafe.Alignof(s.f) returns the alignment of
	      // the type of f when it is used as a field in a struct.
#ifdef BIGGEST_FIELD_ALIGNMENT
	      if (val_long > BIGGEST_FIELD_ALIGNMENT)
		val_long = BIGGEST_FIELD_ALIGNMENT;
#endif
#ifdef ADJUST_FIELD_ALIGN
	      // A separate declaration avoids a warning promoted to
	      // an error if ADJUST_FIELD_ALIGN ignores FIELD.
	      tree field;
	      field = build_decl(UNKNOWN_LOCATION, FIELD_DECL, NULL,
				      arg_type_tree);
	      val_long = ADJUST_FIELD_ALIGN(field, val_long);
#endif
	    }
	  val_long /= BITS_PER_UNIT;
	}
      else
	gcc_unreachable();
      mpz_set_ui(val, val_long);
      *ptype = NULL;
      return true;
    }
  else if (this->code_ == BUILTIN_OFFSETOF)
    {
      Expression* arg = this->one_arg();
      if (arg == NULL)
	return false;
      Field_reference_expression* farg = arg->field_reference_expression();
      if (farg == NULL)
	return false;
      Expression* struct_expr = farg->expr();
      Type* st = struct_expr->type();
      if (st->struct_type() == NULL)
	return false;
      tree struct_tree = st->get_tree(this->gogo_);
      gcc_assert(TREE_CODE(struct_tree) == RECORD_TYPE);
      tree field = TYPE_FIELDS(struct_tree);
      for (unsigned int index = farg->field_index(); index > 0; --index)
	{
	  field = TREE_CHAIN(field);
	  gcc_assert(field != NULL_TREE);
	}
      HOST_WIDE_INT offset_wide = int_byte_position (field);
      if (offset_wide < 0)
	return false;
      unsigned long offset_long = static_cast<unsigned long>(offset_wide);
      if (offset_long != static_cast<unsigned HOST_WIDE_INT>(offset_wide))
	return false;
      mpz_set_ui(val, offset_long);
      return true;
    }
  return false;
}

// Return the type.

Type*
Builtin_call_expression::do_type()
{
  switch (this->code_)
    {
    case BUILTIN_INVALID:
    default:
      gcc_unreachable();

    case BUILTIN_NEW:
    case BUILTIN_MAKE:
      {
	const Expression_list* args = this->args();
	if (args == NULL || args->empty())
	  return Type::make_error_type();
	return Type::make_pointer_type(args->front()->type());
      }

    case BUILTIN_CAP:
    case BUILTIN_LEN:
    case BUILTIN_ALIGNOF:
    case BUILTIN_OFFSETOF:
    case BUILTIN_SIZEOF:
      return Type::lookup_integer_type("int");

    case BUILTIN_CLOSE:
    case BUILTIN_PANIC:
    case BUILTIN_PANICLN:
    case BUILTIN_PRINT:
    case BUILTIN_PRINTLN:
      return Type::make_void_type();

    case BUILTIN_CLOSED:
      return Type::lookup_bool_type();

    case BUILTIN_SQRT:
      return Type::lookup_float_type("float64");
    }
}

// Determine the type.

void
Builtin_call_expression::do_determine_type(const Type_context*)
{
  this->fn()->determine_type_no_context();

  Type_context subcontext(NULL, true);
  if (this->code_ == BUILTIN_SQRT)
    subcontext.type = Type::lookup_float_type("float64");
  const Expression_list* args = this->args();
  if (args != NULL)
    {
      for (Expression_list::const_iterator pa = args->begin();
	   pa != args->end();
	   ++pa)
	(*pa)->determine_type(&subcontext);
    }
}

// If there is exactly one argument, return true.  Otherwise give an
// error message and return false.

bool
Builtin_call_expression::check_one_arg()
{
  const Expression_list* args = this->args();
  if (args == NULL || args->size() < 1)
    {
      this->report_error(_("not enough arguments"));
      return false;
    }
  else if (args->size() > 1)
    {
      this->report_error(_("too many arguments"));
      return false;
    }
  if (args->front()->is_error_expression()
      || args->front()->type()->is_error_type())
    {
      this->set_is_error();
      return false;
    }
  return true;
}

// Check argument types for a builtin function.

void
Builtin_call_expression::do_check_types(Gogo*)
{
  switch (this->code_)
    {
    case BUILTIN_INVALID:
    case BUILTIN_NEW:
    case BUILTIN_MAKE:
      return;

    case BUILTIN_LEN:
    case BUILTIN_CAP:
      {
	// The single argument may be either a string or an array or a
	// map or a channel, or a pointer to a closed array.
	if (this->check_one_arg())
	  {
	    Type* arg_type = this->one_arg()->type();
	    if (arg_type->points_to() != NULL
		&& arg_type->points_to()->array_type() != NULL
		&& !arg_type->points_to()->is_open_array_type())
	      arg_type = arg_type->points_to();
	    if (this->code_ == BUILTIN_CAP)
	      {
		if (!arg_type->is_error_type()
		    && arg_type->array_type() == NULL
		    && arg_type->channel_type() == NULL)
		  this->report_error(_("argument must be array or slice "
				       "or channel"));
	      }
	    else
	      {
		if (!arg_type->is_error_type()
		    && !arg_type->is_string_type()
		    && arg_type->array_type() == NULL
		    && arg_type->map_type() == NULL
		    && arg_type->channel_type() == NULL)
		  this->report_error(_("argument must be string or "
				       "array or slice or map or channel"));
	      }
	  }
      }
      break;

    case BUILTIN_PANIC:
    case BUILTIN_PANICLN:
    case BUILTIN_PRINT:
    case BUILTIN_PRINTLN:
      {
	const Expression_list* args = this->args();
	if (args == NULL)
	  {
	    if (this->code_ == BUILTIN_PRINT)
	      warning_at(this->location(), 0,
			 "no arguments for builtin function %<%s%>",
			 (this->code_ == BUILTIN_PRINT
			  ? "print"
			  : "println"));
	  }
	else
	  {
	    for (Expression_list::const_iterator p = args->begin();
		 p != args->end();
		 ++p)
	      {
		Type* type = (*p)->type();
		if (type->is_error_type()
		    || type->is_string_type()
		    || type->integer_type() != NULL
		    || type->float_type() != NULL
		    || type->is_boolean_type()
		    || type->points_to() != NULL
		    || type->array_type() != NULL
		    || type->interface_type() != NULL)
		  ;
		else
		  this->report_error(_("unsupported argument type to "
				       "builtin function"));
	      }
	  }
      }
      break;

    case BUILTIN_CLOSE:
    case BUILTIN_CLOSED:
      if (this->check_one_arg())
	{
	  if (this->one_arg()->type()->channel_type() == NULL)
	    this->report_error(_("argument must be channel"));
	}
      break;

    case BUILTIN_SIZEOF:
    case BUILTIN_ALIGNOF:
      this->check_one_arg();
      break;

    case BUILTIN_OFFSETOF:
      if (this->check_one_arg())
	{
	  Expression* arg = this->one_arg();
	  if (arg->field_reference_expression() == NULL)
	    this->report_error(_("argument must be a field reference"));
	}
      break;

    case BUILTIN_SQRT:
      if (this->check_one_arg())
	{
	  Expression* arg = this->one_arg();
	  if (arg->type()->float_type() == NULL
	      || arg->type()->float_type()->is_abstract()
	      || arg->type()->float_type()->bits() != 64)
	    this->report_error(_("argument 1 has incompatible type"));
	}
      break;

    default:
      gcc_unreachable();
    }
}

// Return the tree for a builtin function.

tree
Builtin_call_expression::do_get_tree(Translate_context* context)
{
  Gogo* gogo = context->gogo();
  switch (this->code_)
    {
    case BUILTIN_INVALID:
    case BUILTIN_NEW:
    case BUILTIN_MAKE:
      gcc_unreachable();

    case BUILTIN_LEN:
    case BUILTIN_CAP:
      {
	const Expression_list* args = this->args();
	gcc_assert(args != NULL && args->size() == 1);
	Expression* arg = *args->begin();
	Type* arg_type = arg->type();
	tree arg_tree = arg->get_tree(context);
	if (arg_tree == error_mark_node)
	  return error_mark_node;

	if (arg_type->points_to() != NULL)
	  {
	    arg_type = arg_type->points_to();
	    gcc_assert(arg_type->array_type() != NULL
		       && !arg_type->is_open_array_type());
	    gcc_assert(POINTER_TYPE_P(TREE_TYPE(arg_tree)));
	    arg_tree = build_fold_indirect_ref(arg_tree);
	  }

	tree val_tree;
	if (this->code_ == BUILTIN_LEN)
	  {
	    if (arg_type->is_string_type())
	      val_tree = String_type::length_tree(gogo, arg_tree);
	    else if (arg_type->array_type() != NULL)
	      val_tree = arg_type->array_type()->length_tree(gogo, arg_tree);
	    else if (arg_type->map_type() != NULL)
	      {
		static tree map_len_fndecl;
		val_tree = Gogo::call_builtin(&map_len_fndecl,
					      this->location(),
					      "__go_map_len",
					      1,
					      sizetype,
					      arg_type->get_tree(gogo),
					      arg_tree);
	      }
	    else if (arg_type->channel_type() != NULL)
	      {
		static tree chan_len_fndecl;
		val_tree = Gogo::call_builtin(&chan_len_fndecl,
					      this->location(),
					      "__go_chan_len",
					      1,
					      sizetype,
					      arg_type->get_tree(gogo),
					      arg_tree);
	      }
	    else
	      gcc_unreachable();
	  }
	else
	  {
	    if (arg_type->array_type() != NULL)
	      val_tree = arg_type->array_type()->capacity_tree(gogo, arg_tree);
	    else if (arg_type->channel_type() != NULL)
	      {
		static tree chan_cap_fndecl;
		val_tree = Gogo::call_builtin(&chan_cap_fndecl,
					      this->location(),
					      "__go_chan_cap",
					      1,
					      sizetype,
					      arg_type->get_tree(gogo),
					      arg_tree);
	      }
	    else
	      gcc_unreachable();
	  }

	tree type_tree = Type::lookup_integer_type("int")->get_tree(gogo);
	if (type_tree == TREE_TYPE(val_tree))
	  return val_tree;
	else
	  return fold(convert_to_integer(type_tree, val_tree));
      }

    case BUILTIN_PANIC:
    case BUILTIN_PANICLN:
    case BUILTIN_PRINT:
    case BUILTIN_PRINTLN:
      {
	const bool is_panic = (this->code_ == BUILTIN_PANIC
			       || this->code_ == BUILTIN_PANICLN);
	const bool is_ln = (this->code_ == BUILTIN_PANICLN
			    || this->code_ == BUILTIN_PRINTLN);
	const Expression_list* call_args = this->args();

	int nargs;
	tree* args;
	std::string format;
	if (call_args == NULL)
	  {
	    if (!is_panic && !is_ln)
	      {
		// A call to print with no arguments.  There is nothing
		// to do.
		return integer_zero_node;
	      }
	    nargs = 1;
	    args = new tree[1];
	  }
	else
	  {
	    nargs = call_args->size() + 1;

	    // We allocate extra space because we use three arguments
	    // to print arrays.
	    args = new tree[nargs * 3];

	    int i = 1;
	    for (Expression_list::const_iterator p = call_args->begin();
		 p != call_args->end();
		 ++p, ++i)
	      {
		if (is_ln && i > 1)
		  format += " ";

		args[i] = (*p)->get_tree(context);

		if (args[i] == error_mark_node)
		  {
		    args[i] = integer_zero_node;
		    format += "%d";
		  }
		else if ((*p)->type()->is_string_type())
		  {
		    // We use a precision to print the right number of
		    // characters.  FIXME: If the string has embedded
		    // null bytes, it won't be printed correctly.
		    tree string = args[i];
		    tree len = String_type::length_tree(gogo, string);
		    args[i] = convert_to_integer(integer_type_node, len);
		    ++i;
		    ++nargs;
		    args[i] = String_type::bytes_tree(gogo, string);
		    format += "%.*s";
		  }
		else if ((*p)->type()->integer_type() != NULL)
		  {
		    const Integer_type* itype = (*p)->type()->integer_type();
		    int bits = TYPE_PRECISION(TREE_TYPE(args[i]));
		    if (bits <= INT_TYPE_SIZE)
		      {
			args[i] = fold_convert((itype->is_unsigned()
						? unsigned_type_node
						: integer_type_node),
					       args[i]);
			format += itype->is_unsigned() ? "%u" : "%d";
		      }
		    else if (bits <= LONG_TYPE_SIZE)
		      {
			args[i] = fold_convert((itype->is_unsigned()
						? long_unsigned_type_node
						: long_integer_type_node),
					       args[i]);
			format += itype->is_unsigned() ? "%lu" : "%ld";
		      }
		    else if (bits <= LONG_LONG_TYPE_SIZE)
		      {
			args[i] = fold_convert((itype->is_unsigned()
						? long_long_unsigned_type_node
						: long_long_integer_type_node),
					       args[i]);
			format += itype->is_unsigned() ? "%llu" : "%lld";
		      }
		    else
		      gcc_unreachable();
		  }
		else if ((*p)->type()->float_type() != NULL)
		  {
		    args[i] = fold_convert(double_type_node, args[i]);
		    format += "%.24g";
		  }
		else if ((*p)->type()->is_boolean_type())
		  {
		    tree string_type = Gogo::const_char_pointer_type_tree();
		    tree true_string = Gogo::string_constant_tree("true");
		    true_string = build_fold_addr_expr(true_string);
		    true_string = fold_convert(string_type, true_string);
		    tree false_string = Gogo::string_constant_tree("false");
		    false_string = build_fold_addr_expr(false_string);
		    false_string = fold_convert(string_type, false_string);
		    args[i] = fold_build3(COND_EXPR, string_type, args[i],
					  true_string, false_string);
		    format += "%s";
		  }
		else if ((*p)->type()->points_to() != NULL
			 || (*p)->type()->interface_type() != NULL)
		  {
		    args[i] = fold_convert(ptr_type_node, args[i]);
		    format += "%p";
		  }
		else if ((*p)->type()->array_type() != NULL)
		  {
		    Array_type* at = (*p)->type()->array_type();
		    tree v = save_expr(args[i]);
		    args[i] = at->length_tree(gogo, v);
		    ++i;
		    ++nargs;
		    args[i] = at->capacity_tree(gogo, v);
		    ++i;
		    ++nargs;
		    args[i] = at->value_pointer_tree(gogo, v);
		    format += "[%zu/%zu]%p";
		  }
		else
		  {
		    args[i] = integer_zero_node;
		    format += "%d";
		  }
	      }
	    gcc_assert(i == nargs);
	  }

	if (is_ln)
	  format += "\n";

	tree string_val = Gogo::string_constant_tree(format);
	args[0] = build_fold_addr_expr(string_val);

	static tree panic_fndecl;
	static tree print_fndecl;
	static tree* pfndecl;
	if (is_panic)
	  pfndecl = &panic_fndecl;
	else
	  pfndecl = &print_fndecl;
	if (*pfndecl == NULL_TREE)
	  {
	    tree fnid = get_identifier(is_panic ? "__go_panic" : "__go_print");
	    tree argtypes = tree_cons(NULL_TREE,
				      Gogo::const_char_pointer_type_tree(),
				      NULL_TREE);
	    tree fntype = build_function_type(void_type_node, argtypes);

	    *pfndecl = build_decl(BUILTINS_LOCATION, FUNCTION_DECL, fnid,
				  fntype);
	    Gogo::mark_fndecl_as_builtin_library(*pfndecl);
	    if (is_panic)
	      {
		// Mark the function as noreturn.
		TREE_THIS_VOLATILE(*pfndecl) = 1;
	      }
	    go_preserve_from_gc(*pfndecl);
	  }

	tree fnptr = build_fold_addr_expr(*pfndecl);
	tree call = build_call_array(void_type_node, fnptr, nargs, args);
	delete[] args;

	SET_EXPR_LOCATION(call, this->location());

	return call;
      }

    case BUILTIN_CLOSE:
    case BUILTIN_CLOSED:
      {
	const Expression_list* args = this->args();
	gcc_assert(args != NULL && args->size() == 1);
	Expression* arg = args->front();
	tree arg_tree = arg->get_tree(context);
	if (arg_tree == error_mark_node)
	  return error_mark_node;
	if (this->code_ == BUILTIN_CLOSE)
	  {
	    static tree close_fndecl;
	    return Gogo::call_builtin(&close_fndecl,
				      this->location(),
				      "__go_builtin_close",
				      1,
				      void_type_node,
				      TREE_TYPE(arg_tree),
				      arg_tree);
	  }
	else
	  {
	    static tree closed_fndecl;
	    return Gogo::call_builtin(&closed_fndecl,
				      this->location(),
				      "__go_builtin_closed",
				      1,
				      boolean_type_node,
				      TREE_TYPE(arg_tree),
				      arg_tree);
	  }
      }

    case BUILTIN_SIZEOF:
    case BUILTIN_OFFSETOF:
    case BUILTIN_ALIGNOF:
      {
	mpz_t val;
	mpz_init(val);
	Type* dummy;
	bool b = this->integer_constant_value(true, val, &dummy);
	gcc_assert(b);
	tree type = Type::lookup_integer_type("int")->get_tree(gogo);
	tree ret = Expression::integer_constant_tree(val, type);
	mpz_clear(val);
	return ret;
      }

    case BUILTIN_SQRT:
      {
	const Expression_list* args = this->args();
	gcc_assert(args != NULL && args->size() == 1);
	Expression* arg = args->front();
	tree arg_tree = arg->get_tree(context);
	if (arg_tree == error_mark_node)
	  return error_mark_node;
	tree type = Type::lookup_float_type("float64")->get_tree(gogo);
	arg_tree = fold_convert_loc(this->location(), type, arg_tree);
	tree fn = built_in_decls[BUILT_IN_SQRT];
	tree call = build1(ADDR_EXPR, build_pointer_type(TREE_TYPE(fn)), fn);
	call = build_call_nary(TREE_TYPE(TREE_TYPE(fn)), call, 1, arg_tree);
	return fold_convert_loc(this->location(), type, call);
      }

    default:
      gcc_unreachable();
    }
}

// We have to support exporting a builtin call expression, because
// code can set a constant to the result of a builtin expression.

void
Builtin_call_expression::do_export(Export* exp) const
{
  mpz_t val;
  mpz_init(val);
  Type* dummy;
  bool b = this->integer_constant_value(true, val, &dummy);
  if (!b)
    error_at(this->location(), "value is not constant");
  char* s = mpz_get_str(NULL, 10, val);
  exp->write_c_string(s);
  free(s);
  // A trailing space lets us reliably identify the end of the number.
  exp->write_c_string(" ");
  mpz_clear(val);
}

// Class Call_expression.

// Traversal.

int
Call_expression::do_traverse(Traverse* traverse)
{
  if (Expression::traverse(&this->fn_, traverse) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  if (this->args_ != NULL)
    {
      if (this->args_->traverse(traverse) == TRAVERSE_EXIT)
	return TRAVERSE_EXIT;
    }
  return TRAVERSE_CONTINUE;
}

// Lower a call statement.  This is where we handle an argument which
// is a call to a function which returns multiple results.  We also
// may discover that we thought was a call is actually a type cast.

Expression*
Call_expression::do_lower(Gogo* gogo, int)
{
  if (this->fn_->is_type_expression()
      && this->args_ != NULL
      && this->args_->size() == 1)
    return Expression::make_cast(this->fn_->type(), this->args_->front(),
				 this->location());

  Func_expression* fne = this->fn_->func_expression();
  if (fne != NULL
      && fne->named_object()->is_function_declaration()
      && fne->named_object()->func_declaration_value()->type()->is_builtin())
    return new Builtin_call_expression(gogo, this->fn_, this->args_,
				       this->location());

  if (this->args_ != NULL
      && this->args_->size() == 1
      && this->args_->front()->call_expression() != NULL
      && this->fn_->type()->function_type() != NULL)
    {
      Function_type* fntype = this->fn_->type()->function_type();
      size_t rc = this->args_->front()->call_expression()->result_count();
      if (rc > 1
	  && fntype->parameters() != NULL
	  && (fntype->parameters()->size() == rc
	      || (fntype->is_varargs()
		  && fntype->parameters()->size() - 1 <= rc)))
	{
	  Call_expression* call = this->args_->front()->call_expression();
	  Expression_list* args = new Expression_list;
	  for (size_t i = 0; i < rc; ++i)
	    args->push_back(Expression::make_call_result(call, i));
	  // We can't return a new call expression here, because this
	  // one may be referenced by Call_result expressions.  FIXME.
	  delete this->args_;
	  this->args_ = args;
	}
    }

  return this;
}

// Get the function type.  Returns NULL if we don't know the type.  If
// this returns NULL, and if_ERROR is true, issues an error.

Function_type*
Call_expression::get_function_type(bool issue_error)
{
  Type* type = this->fn_->type()->deref();
  Function_type* fntype = type->function_type();
  if (fntype != NULL)
    return fntype;
  if (issue_error && !type->is_error_type())
    this->report_error("expected function");
  return NULL;
}

// Return the number of values which this call will return.

size_t
Call_expression::result_count()
{
  Function_type* fntype = this->get_function_type(false);
  if (fntype == NULL)
    return 0;
  if (fntype->results() == NULL)
    return 0;
  return fntype->results()->size();
}

// Get the type.

Type*
Call_expression::do_type()
{
  if (this->type_ != NULL)
    return this->type_;

  Type* ret;
  Function_type* fntype = this->get_function_type(false);
  if (fntype == NULL)
    return Type::make_error_type();

  const Typed_identifier_list* results = fntype->results();
  if (results == NULL)
    ret = Type::make_void_type();
  else if (results->size() == 1)
    ret = results->begin()->type();
  else
    ret = Type::make_call_multiple_result_type(this);

  this->type_ = ret;

  return this->type_;
}

// Determine types for a call expression.  We can use the function
// parameter types to set the types of the arguments.

void
Call_expression::do_determine_type(const Type_context*)
{
  this->fn_->determine_type_no_context();
  Function_type* fntype = this->get_function_type(false);
  const Typed_identifier_list* parameters = NULL;
  if (fntype != NULL)
    parameters = fntype->parameters();
  if (this->args_ != NULL)
    {
      Typed_identifier_list::const_iterator pt;
      if (parameters != NULL)
	pt = parameters->begin();
      for (Expression_list::const_iterator pa = this->args_->begin();
	   pa != this->args_->end();
	   ++pa)
	{
	  if (parameters != NULL
	      && pt != parameters->end()
	      && (!fntype->is_varargs() || pt + 1 != parameters->end()))
	    {
	      Type_context subcontext(pt->type(), false);
	      (*pa)->determine_type(&subcontext);
	      ++pt;
	    }
	  else
	    (*pa)->determine_type_no_context();
	}
    }
}

// Check types for parameter I.

void
Call_expression::check_argument_type(int i, const Type* parameter_type,
				     const Type* argument_type,
				     source_location argument_location)
{
  std::string reason;
  if (!Type::are_compatible_for_assign(parameter_type, argument_type, &reason))
    {
      if (reason.empty())
	error_at(argument_location, "argument %d has incompatible type", i);
      else
	error_at(argument_location, "argument %d has incompatible type (%s)",
		 i, reason.c_str());
      this->set_is_error();
    }
}

// Check types.

void
Call_expression::do_check_types(Gogo*)
{
  Function_type* fntype = this->get_function_type(true);
  if (fntype == NULL)
    return;

  if (fntype->is_method())
    {
      // We don't support pointers to methods, so the function has to
      // be a bound method expression.
      if (this->fn_->bound_method_expression() == NULL)
	this->report_error(_("method call without object"));
    }

  const Typed_identifier_list* parameters = fntype->parameters();
  if (this->args_ == NULL)
    {
      // A single varargs parameter can accept zero arguments.
      if (parameters != NULL
	  && (!fntype->is_varargs() || parameters->size() > 1))
	this->report_error(_("not enough arguments"));
    }
  else if (parameters == NULL)
    this->report_error(_("too many arguments"));
  else
    {
      int i = 0;
      Typed_identifier_list::const_iterator pt = parameters->begin();
      for (Expression_list::const_iterator pa = this->args_->begin();
	   pa != this->args_->end();
	   ++pa, ++pt, ++i)
	{
	  if (fntype->is_varargs() && pt + 1 == parameters->end())
	    {
	      // A varargs parameter can accept any number of
	      // arguments, but you can't pass nil.
	      for (; pa != this->args_->end(); ++pa)
		{
		  if ((*pa)->type()->is_nil_type())
		    {
		      this->report_error(_("invalid use of %<nil%> for "
					   "%<...%> argument"));
		      return;
		    }
		}
	      break;
	    }
	  if (pt == parameters->end())
	    {
	      this->report_error(_("too many arguments"));
	      return;
	    }
	  this->check_argument_type(i + 1, pt->type(), (*pa)->type(),
				    (*pa)->location());
	}
      // The varargs parameter can accept zero arguments.
      if (pt != parameters->end()
	  && (!fntype->is_varargs() || pt + 1 != parameters->end()))
	this->report_error(_("not enough arguments"));
    }
}

// Get the function and the first argument to use when calling a bound
// method.

tree
Call_expression::bound_method_function(Translate_context* context,
				       Bound_method_expression* bound_method,
				       tree* first_arg_ptr)
{
  Expression* first_argument = bound_method->first_argument();
  tree first_arg = first_argument->get_tree(context);
  if (first_arg == error_mark_node)
    return error_mark_node;

  // We always pass a pointer to the first argument when calling a
  // method.
  if (first_argument->type()->points_to() == NULL)
    {
      tree pointer_to_arg_type = build_pointer_type(TREE_TYPE(first_arg));
      if (TREE_ADDRESSABLE(TREE_TYPE(first_arg))
	  || DECL_P(first_arg)
	  || TREE_CODE(first_arg) == INDIRECT_REF
	  || TREE_CODE(first_arg) == COMPONENT_REF)
	{
	  first_arg = build_fold_addr_expr(first_arg);
	  if (DECL_P(first_arg))
	    TREE_ADDRESSABLE(first_arg) = 1;
	}
      else
	{
	  tree tmp = create_tmp_var(TREE_TYPE(first_arg),
				    get_name(first_arg));
	  DECL_IGNORED_P(tmp) = 0;
	  DECL_INITIAL(tmp) = first_arg;
	  first_arg = build2(COMPOUND_EXPR, pointer_to_arg_type,
			     build1(DECL_EXPR, void_type_node, tmp),
			     build_fold_addr_expr(tmp));
	  TREE_ADDRESSABLE(tmp) = 1;
	}
      if (first_arg == error_mark_node)
	return error_mark_node;
    }

  Type* fatype = bound_method->first_argument_type();
  if (fatype != NULL)
    {
      if (fatype->points_to() == NULL)
	fatype = Type::make_pointer_type(fatype);
      first_arg = fold_convert(fatype->get_tree(context->gogo()), first_arg);
      if (first_arg == error_mark_node
	  || TREE_TYPE(first_arg) == error_mark_node)
	return error_mark_node;
    }

  *first_arg_ptr = first_arg;

  return bound_method->method()->get_tree(context);
}

// Get the function and the first argument to use when calling an
// interface method.

tree
Call_expression::interface_method_function(
    Translate_context* context,
    Interface_field_reference_expression* interface_method,
    tree* first_arg_ptr)
{
  tree expr = interface_method->expr()->get_tree(context);
  if (expr == error_mark_node)
    return error_mark_node;
  expr = save_expr(expr);
  tree first_arg = interface_method->get_underlying_object_tree(context, expr);
  if (first_arg == error_mark_node)
    return error_mark_node;
  *first_arg_ptr = first_arg;
  return interface_method->get_function_tree(context, expr);
}

// The call expression is being copied.  There is nothing to do here
// except to disable any decrements--the function should return with
// an appropriate reference count.

Expression*
Call_expression::do_being_copied(Refcounts*, bool)
{
  this->is_being_copied_ = true;
  return this;
}

// Where needed, decrement the reference counts of any values returned
// by this call.

Expression*
Call_expression::do_note_decrements(Refcounts* refcounts)
{
  if (this->is_being_copied_)
    return this;
  Function_type* fntype = this->get_function_type(false);
  if (fntype == NULL)
    return this;
  const Typed_identifier_list* results = fntype->results();
  if (results == NULL)
    return this;
  if (results->size() == 1)
    {
      if (!results->begin()->type()->has_refcounted_component())
	return this;
      return Expression::make_refcount_adjust(refcounts,
					      REFCOUNT_DECREMENT_COMPUTED,
					      this, false);
    }
  else if (!this->is_value_discarded_)
    {
      // If the value is not discarded, each result will be handled
      // separately via Call_expression_result.
      return this;
    }
  else
    {
      for (Typed_identifier_list::const_iterator p = results->begin();
	   p != results->end();
	   p++)
	{
	  if (p->type()->has_refcounted_component())
	    {
	      if (this->refcount_entries_ == NULL)
		this->refcount_entries_ = new std::vector<Refcount_entry>;
	      Refcount_entry re = refcounts->add(REFCOUNT_DECREMENT_COMPUTED,
						 p->type());
	      this->refcount_entries_->push_back(re);
	    }
	}
      return this;
    }
}

// Build the call expression.

tree
Call_expression::do_get_tree(Translate_context* context)
{
  if (this->tree_ != NULL_TREE)
    return this->tree_;

  Function_type* fntype = this->get_function_type(false);
  if (fntype == NULL)
    return error_mark_node;

  if (this->fn_->is_error_expression())
    return error_mark_node;

  Gogo* gogo = context->gogo();
  source_location location = this->location();

  Func_expression* func = this->fn_->func_expression();
  Bound_method_expression* bound_method = this->fn_->bound_method_expression();
  Interface_field_reference_expression* interface_method =
    this->fn_->interface_field_reference_expression();
  const bool has_closure = func != NULL && func->closure() != NULL;
  const bool is_method = bound_method != NULL || interface_method != NULL;
  gcc_assert(!fntype->is_method() || is_method);

  bool is_varargs = fntype->is_varargs();

  int nargs;
  tree* args;
  if (this->args_ == NULL || this->args_->empty())
    {
      nargs = is_method ? 1 : 0;
      if (is_varargs)
	++nargs;
      args = nargs == 0 ? NULL : new tree[nargs];
      if (is_varargs)
	{
	  Type* varargs_type = Function_type::varargs_type();
	  // The old way, passing array[]interface{}.
	  // args[nargs - 1] = varargs_type->get_init_tree(gogo, false);
	  Struct_field_list* fields = new Struct_field_list();
	  Type* struct_type = Type::make_struct_type(fields, location);
	  tree val = struct_type->get_init_tree(gogo, false);
	  args[nargs - 1] = Expression::convert_for_assignment(context,
							       varargs_type,
							       struct_type,
							       val,
							       location);
	}
    }
  else
    {
      const Typed_identifier_list* params = fntype->parameters();
      gcc_assert(params != NULL);

      if (is_varargs)
	nargs = params->size();
      else
	nargs = this->args_->size();
      int i = is_method ? 1 : 0;
      nargs += i;
      args = new tree[nargs];

      Typed_identifier_list::const_iterator pp = params->begin();
      Expression_list::const_iterator pe;
      for (pe = this->args_->begin();
	   pe != this->args_->end();
	   ++pe, ++pp, ++i)
	{
	  Type* pp_type = pp->type();
	  if (is_varargs && pp + 1 == params->end())
	    {
	      // If the last argument is itself a varargs argument for
	      // the caller, we pass it without further encapsulation.
	      if (pe + 1 == this->args_->end()
		  && (*pe)->var_expression() != NULL
		  && (*pe)->var_expression()->named_object()->is_variable()
		  && ((*pe)->var_expression()->named_object()->var_value()->
		      is_varargs_parameter()))
		{
		  pp_type = Function_type::varargs_type();
		  is_varargs = false;
		}
	      else
		break;
	    }
	  tree arg_val = (*pe)->get_tree(context);
	  args[i] = Expression::convert_for_assignment(context,
						       pp_type,
						       (*pe)->type(),
						       arg_val,
						       location);
	  if (args[i] == error_mark_node)
	    return error_mark_node;
	}

      if (is_varargs)
	{
	  Type* varargs_type = Function_type::varargs_type();

#if 0
	  // The old way, passing array[]interface{}.
	  Type* element_type = varargs_type->array_type()->element_type();
	  VEC(constructor_elt,gc)* values = VEC_alloc(constructor_elt, gc,
						      this->args_->size());
	  size_t j = 0;
	  for (; pe != this->args_->end(); ++pe, ++j)
	    {
	      constructor_elt* elt = VEC_quick_push(constructor_elt, values,
						    NULL);
	      elt->index = size_int(j);
	      tree expr_tree = (*pe)->get_tree(context);
	      elt->value = Expression::convert_for_assignment(context,
							      element_type,
							      (*pe)->type(),
							      expr_tree,
							      location);
	    }

	  mpz_t val;
	  mpz_init_set_ui(val, j);
	  Type* array_type =
	    Type::make_array_type(element_type,
				  Expression::make_integer(&val, NULL,
							   location));
	  mpz_clear(val);

	  tree varargs_val = build_constructor(array_type->get_tree(gogo),
					       values);

	  args[i] = Expression::convert_for_assignment(context, varargs_type,
						       array_type,
						       varargs_val,
						       location);
#else
	  // The new way, passing a struct converted to an empty
	  // interface.
	  Struct_field_list* fields = new Struct_field_list();
	  Expression_list::const_iterator pehold = pe;
	  for (; pe != this->args_->end(); ++pe)
	    fields->push_back(Struct_field(Typed_identifier("UNNAMED",
							    (*pe)->type(),
							    location)));
	  Type* struct_type = Type::make_struct_type(fields, location);
	  tree struct_tree = struct_type->get_tree(gogo);
	  if (struct_tree == error_mark_node)
	    return error_mark_node;

	  VEC(constructor_elt,gc)* values = VEC_alloc(constructor_elt, gc,
						      this->args_->size());
	  tree field = TYPE_FIELDS(struct_tree);
	  for (pe = pehold; pe != this->args_->end(); ++pe)
	    {
	      gcc_assert(field != NULL_TREE);
	      constructor_elt* elt = VEC_quick_push(constructor_elt, values,
						    NULL);
	      elt->index = field;
	      elt->value = (*pe)->get_tree(context);
	      field = TREE_CHAIN(field);
	    }
	  gcc_assert(field == NULL_TREE);

	  tree varargs_val = build_constructor(struct_tree, values);

	  args[i] = Expression::convert_for_assignment(context, varargs_type,
						       struct_type,
						       varargs_val,
						       location);
#endif

	  ++i;
	  ++pp;
	}

      gcc_assert(pp == params->end());
      gcc_assert(i == nargs);
    }

  tree rettype = TREE_TYPE(TREE_TYPE(fntype->get_tree(gogo)));
  if (rettype == error_mark_node)
    return error_mark_node;

  tree fn;
  if (has_closure)
    fn = func->get_tree_without_closure(gogo);
  else if (!is_method)
    fn = this->fn_->get_tree(context);
  else if (bound_method != NULL)
    fn = this->bound_method_function(context, bound_method, &args[0]);
  else if (interface_method != NULL)
    fn = this->interface_method_function(context, interface_method, &args[0]);
  else
    gcc_unreachable();

  if (fn == error_mark_node || TREE_TYPE(fn) == error_mark_node)
    return error_mark_node;

  tree ret = build_call_array(rettype, fn, nargs, args);
  delete[] args;

  SET_EXPR_LOCATION(ret, location);

  if (has_closure)
    {
      tree closure_tree = func->closure()->get_tree(context);
      if (closure_tree != error_mark_node)
	CALL_EXPR_STATIC_CHAIN(ret) = closure_tree;
    }

  // If there is more than one result, we will refer to the call
  // multiple times.
  if (fntype->results() != NULL && fntype->results()->size() > 1)
    ret = save_expr(ret);

  if (this->refcount_entries_ != NULL)
    ret = this->set_refcount_queue_entries(context, ret);

  this->tree_ = ret;

  return ret;
}

// Set entries in the decrement queue as needed.

tree
Call_expression::set_refcount_queue_entries(Translate_context* context,
					    tree ret)
{
  Gogo* gogo = context->gogo();
  Refcounts* refcounts = context->function()->func_value()->refcounts();
  tree val = ret;
  Function_type* fntype = this->get_function_type(false);
  const Typed_identifier_list* results = fntype->results();
  gcc_assert(TREE_CODE(TREE_TYPE(val)) == RECORD_TYPE);
  std::vector<Refcount_entry>::const_iterator pre =
    this->refcount_entries_->begin();
  tree field = TYPE_FIELDS(TREE_TYPE(val));
  for (Typed_identifier_list::const_iterator pr = results->begin();
       pr != results->end();
       ++pr, field = TREE_CHAIN(field))
    {
      gcc_assert(field != NULL_TREE);
      if (pr->type()->has_refcounted_component())
	{
	  gcc_assert(pre != this->refcount_entries_->end());
	  Refcount_entry re = *pre;
	  tree f = fold_build3(COMPONENT_REF, TREE_TYPE(field), val, field,
			       NULL_TREE);
	  pr->type()->set_refcount_queue_entry(gogo, refcounts, &re, f);
	  ++pre;
	}
    }
  gcc_assert(pre == this->refcount_entries_->end());
  gcc_assert(field == NULL_TREE);
  return ret;
}

// Make a call expression.

Call_expression*
Expression::make_call(Expression* fn, Expression_list* args,
		      source_location location)
{
  return new Call_expression(fn, args, location);
}

// A single result from a call which returns multiple results.

class Call_result_expression : public Expression
{
 public:
  Call_result_expression(Call_expression* call, unsigned int index)
    : Expression(EXPRESSION_CALL_RESULT, call->location()),
      call_(call), index_(index), is_being_copied_(false)
  { }

 protected:
  int
  do_traverse(Traverse*);

  Type*
  do_type();

  void
  do_determine_type(const Type_context*);

  Expression*
  do_copy()
  {
    return new Call_result_expression(this->call_->call_expression(),
				      this->index_);
  }

  Expression*
  do_being_copied(Refcounts*, bool);

  Expression*
  do_note_decrements(Refcounts*);

  tree
  do_get_tree(Translate_context*);

 private:
  // The underlying call expression.
  Expression* call_;
  // Which result we want.
  unsigned int index_;
  // Whether the result is being copied.
  bool is_being_copied_;
};

// Traverse a call result.  We only traverse the call expression for
// index 0, to avoid traversing it multiple times.

int
Call_result_expression::do_traverse(Traverse* traverse)
{
  if (this->index_ > 0)
    return TRAVERSE_CONTINUE;
  return Expression::traverse(&this->call_, traverse);
}

// Get the type.

Type*
Call_result_expression::do_type()
{
  if (this->call_->is_error_expression())
    return Type::make_error_type();
  Function_type* fntype =
    this->call_->call_expression()->get_function_type(false);
  if (fntype == NULL)
    return Type::make_error_type();
  const Typed_identifier_list* results = fntype->results();
  Typed_identifier_list::const_iterator pr = results->begin();
  for (unsigned int i = 0; i < this->index_; ++i)
    {
      gcc_assert(pr != results->end());
      ++pr;
    }
  gcc_assert(pr != results->end());
  return pr->type();
}

// Determine the type.  We have nothing to do here, but the 0 result
// needs to pass down to the caller.

void
Call_result_expression::do_determine_type(const Type_context*)
{
  if (this->index_ == 0)
    this->call_->determine_type_no_context();
}

// The result will come back with a reference, so we don't need to do
// anything to copy it.

Expression*
Call_result_expression::do_being_copied(Refcounts*, bool)
{
  this->is_being_copied_ = true;
  return this;
}

// Decrement the reference count if necessary.

Expression*
Call_result_expression::do_note_decrements(Refcounts* refcounts)
{
  if (this->is_being_copied_ || !this->type()->has_refcounted_component())
    return this;
  return Expression::make_refcount_adjust(refcounts,
					  REFCOUNT_DECREMENT_COMPUTED,
					  this, false);
}

// Return the tree.

tree
Call_result_expression::do_get_tree(Translate_context* context)
{
  tree call_tree = this->call_->get_tree(context);
  if (call_tree == error_mark_node)
    return error_mark_node;
  gcc_assert(TREE_CODE(TREE_TYPE(call_tree)) == RECORD_TYPE);
  tree field = TYPE_FIELDS(TREE_TYPE(call_tree));
  for (unsigned int i = 0; i < this->index_; ++i)
    {
      gcc_assert(field != NULL_TREE);
      field = TREE_CHAIN(field);
    }
  gcc_assert(field != NULL_TREE);
  return build3(COMPONENT_REF, TREE_TYPE(field), call_tree, field, NULL_TREE);
}

// Make a reference to a single result of a call which returns
// multiple results.

Expression*
Expression::make_call_result(Call_expression* call, unsigned int index)
{
  return new Call_result_expression(call, index);
}

// Class Index_expression.

// Traversal.

int
Index_expression::do_traverse(Traverse* traverse)
{
  if (Expression::traverse(&this->left_, traverse) == TRAVERSE_EXIT
      || Expression::traverse(&this->start_, traverse) == TRAVERSE_EXIT
      || (this->end_ != NULL
	  && Expression::traverse(&this->end_, traverse) == TRAVERSE_EXIT))
    return TRAVERSE_EXIT;
  return TRAVERSE_CONTINUE;
}

// Lower an index expression.  This converts the generic index
// expression into an array index, a string index, or a map index.

Expression*
Index_expression::do_lower(Gogo*, int)
{
  source_location location = this->location();
  Expression* left = this->left_;
  Expression* start = this->start_;
  Expression* end = this->end_;

  Type* type = left->type();
  if (type->is_error_type())
    return Expression::make_error(location);
  else if (type->array_type() != NULL
	   || (type->deref()->array_type() != NULL
	       && !type->deref()->array_type()->is_open_array_type()))
    return Expression::make_array_index(left, start, end, location);
  else if (type->is_string_type())
    return Expression::make_string_index(left, start, end, location);
  else if (type->map_type() != NULL)
    {
      if (end != NULL)
	{
	  error_at(location, "invalid slice of map");
	  return Expression::make_error(location);
	}
      Map_index_expression* ret= Expression::make_map_index(left, start,
							    location);
      if (this->is_lvalue_)
	ret->set_is_lvalue();
      return ret;
    }
  else
    {
      error_at(location,
	       "attempt to index object which is not array, string, or map");
      return Expression::make_error(location);
    }
}

// Make an index expression.

Expression*
Expression::make_index(Expression* left, Expression* start, Expression* end,
		       source_location location)
{
  return new Index_expression(left, start, end, location);
}

// An array index.  This is used for both indexing and slicing.

class Array_index_expression : public Expression
{
 public:
  Array_index_expression(Expression* array, Expression* start,
			 Expression* end, source_location location)
    : Expression(EXPRESSION_ARRAY_INDEX, location),
      array_(array), start_(start), end_(end), type_(NULL)
  { }

 protected:
  int
  do_traverse(Traverse*);

  Type*
  do_type();

  void
  do_determine_type(const Type_context*);

  void
  do_check_types(Gogo*);

  Expression*
  do_copy()
  {
    return Expression::make_array_index(this->array_->copy(),
					this->start_->copy(),
					(this->end_ == NULL
					 ? NULL
					 : this->end_->copy()),
					this->location());
  }

  bool
  do_is_lvalue() const
  { return this->end_ == NULL; }

  bool
  do_address_taken(source_location, bool);

  Expression*
  do_being_copied(Refcounts*, bool);

  Expression*
  do_note_decrements(Refcounts*);

  Expression*
  do_being_set(Refcounts*);

  tree
  do_get_tree(Translate_context*);

 private:
  // The array we are getting a value from.
  Expression* array_;
  // The start or only index.
  Expression* start_;
  // The end index of a slice.  This may be NULL for a simple array
  // index, or it may be a nil expression for the length of the array.
  Expression* end_;
  // The type of the expression.
  Type* type_;
};

// Array index traversal.

int
Array_index_expression::do_traverse(Traverse* traverse)
{
  if (Expression::traverse(&this->array_, traverse) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  if (Expression::traverse(&this->start_, traverse) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  if (this->end_ != NULL)
    {
      if (Expression::traverse(&this->end_, traverse) == TRAVERSE_EXIT)
	return TRAVERSE_EXIT;
    }
  return TRAVERSE_CONTINUE;
}

// Return the type of an array index.

Type*
Array_index_expression::do_type()
{
  if (this->type_ == NULL)
    {
      Array_type* type = this->array_->type()->deref()->array_type();
      if (type == NULL)
	this->type_ = Type::make_error_type();
      else if (this->end_ == NULL)
	this->type_ = type->element_type();
      else if (type->is_open_array_type())
	{
	  // A slice of a slice has the same type as the original
	  // slice.
	  this->type_ = this->array_->type()->deref();
	}
      else
	{
	  // A slice of an array is a slice.
	  this->type_ = Type::make_array_type(type->element_type(), NULL);
	}
    }
  return this->type_;
}

// Set the type of an array index.

void
Array_index_expression::do_determine_type(const Type_context*)
{
  this->array_->determine_type_no_context();
  Type_context subcontext(NULL, true);
  this->start_->determine_type(&subcontext);
  if (this->end_ != NULL)
    this->end_->determine_type(&subcontext);
}

// Check types of an array index.

void
Array_index_expression::do_check_types(Gogo*)
{
  if (this->start_->type()->integer_type() == NULL)
    this->report_error(_("index must be integer"));
  if (this->end_ != NULL
      && this->end_->type()->integer_type() == NULL
      && !this->end_->is_nil_expression())
    this->report_error(_("slice end must be integer"));

  Array_type* array_type = this->array_->type()->deref()->array_type();
  gcc_assert(array_type != NULL);

  Type* dummy;
  mpz_t lval;
  mpz_init(lval);
  bool lval_valid = (array_type->length() != NULL
		     && array_type->length()->integer_constant_value(true,
								     lval,
								     &dummy));
  mpz_t ival;
  mpz_init(ival);
  if (this->start_->integer_constant_value(true, ival, &dummy))
    {
      if (mpz_sgn(ival) < 0
	  || (lval_valid
	      && (this->end_ == NULL
		  ? mpz_cmp(ival, lval) >= 0
		  : mpz_cmp(ival, lval) > 0)))
	{
	  error_at(this->start_->location(), "array index out of bounds");
	  this->set_is_error();
	}
    }
  if (this->end_ != NULL && !this->end_->is_nil_expression())
    {
      if (this->end_->integer_constant_value(true, ival, &dummy))
	{
	  if (mpz_sgn(ival) < 0 || (lval_valid && mpz_cmp(ival, lval) > 0))
	    {
	      error_at(this->end_->location(), "array index out of bounds");
	      this->set_is_error();
	    }
	}
    }
  mpz_clear(ival);
  mpz_clear(lval);
}

// We can take the address of an array index.  We don't support taking
// the address of a slice--I'm not sure what the type of that would
// be.  Taking the address of an array index implies taking the
// address of the array.

bool
Array_index_expression::do_address_taken(source_location location,
					 bool escapes)
{
  if (!this->array_->address_taken(location, escapes))
    return false;
  if (this->end_ != NULL)
    {
      error_at(location, "may not take address of array slice");
      return false;
    }
  return true;
}

// Copying an element of an array may require incrementing a reference
// count.  Copying a slice requires incrementing the reference count
// of the underlying array.

Expression*
Array_index_expression::do_being_copied(Refcounts* refcounts, bool for_local)
{
  if (this->end_ == NULL && !this->type()->has_refcounted_component())
    return this;
  return Expression::make_refcount_adjust(refcounts,
					  REFCOUNT_INCREMENT_COPIED,
					  this, for_local);
}

// Referring to an element of an array does not require changing any
// reference counts.  If the array is changed before the value is
// used, the program is ill-defined.  We do not introduce a reference
// count for a slice, so there is nothing to decrement.  FIXME: Is
// that safe?

Expression*
Array_index_expression::do_note_decrements(Refcounts*)
{
  return this;
}

// Assigning to an element of an array may require decrementing a
// reference count of the old value.

Expression*
Array_index_expression::do_being_set(Refcounts* refcounts)
{
  gcc_assert(this->end_ == NULL);
  if (!this->type()->has_refcounted_component())
    return this;
  return Expression::make_refcount_decrement_lvalue(refcounts, this);
}

// Get a tree for an array index.

tree
Array_index_expression::do_get_tree(Translate_context* context)
{
  Gogo* gogo = context->gogo();
  source_location loc = this->location();

  Type* t = this->array_->type();
  Type* pt = t->points_to();
  if (pt != NULL)
    t = pt;
  Array_type* array_type = t->array_type();
  gcc_assert(array_type != NULL);

  tree type_tree = array_type->get_tree(gogo);

  tree array_tree = this->array_->get_tree(context);
  if (array_tree == error_mark_node)
    return error_mark_node;

  tree bad_index = boolean_false_node;
  if (pt != NULL)
    {
      gcc_assert(!array_type->is_open_array_type());
      if (!DECL_P(array_tree))
	array_tree = save_expr(array_tree);
      bad_index = build2(EQ_EXPR, boolean_type_node, array_tree,
			 fold_convert(TREE_TYPE(array_tree),
				      null_pointer_node));
      array_tree = build_fold_indirect_ref(array_tree);
    }

  tree start_tree = this->start_->get_tree(context);
  if (start_tree == error_mark_node)
    return error_mark_node;

  if (array_type->length() == NULL && !DECL_P(array_tree))
    array_tree = save_expr(array_tree);
  if (!DECL_P(start_tree))
    start_tree = save_expr(start_tree);
  tree length_tree = array_type->length_tree(gogo, array_tree);
  if (this->end_ != NULL && this->end_->is_nil_expression())
    length_tree = save_expr(length_tree);
  start_tree = fold_convert_loc(loc, TREE_TYPE(length_tree), start_tree);
  bad_index = fold_build2_loc(loc, TRUTH_OR_EXPR, boolean_type_node, bad_index,
			      fold_build2_loc(loc,
					      (this->end_ == NULL
					       ? GE_EXPR
					       : GT_EXPR),
					      boolean_type_node, start_tree,
					      length_tree));

  static tree bad_index_fndecl;
  tree crash = Gogo::call_builtin(&bad_index_fndecl,
				  loc,
				  "__go_bad_index",
				  0,
				  void_type_node);
  TREE_THIS_VOLATILE(bad_index_fndecl) = 1;

  if (this->end_ == NULL)
    {
      // Simple array indexing.  This has to return an l-value, so
      // wrap the index check into START_TREE.
      start_tree = build2(COMPOUND_EXPR, TREE_TYPE(start_tree),
			  build3(COND_EXPR, void_type_node,
				 bad_index, crash, NULL_TREE),
			  start_tree);

      if (array_type->length() != NULL)
	{
	  // Fixed array.
	  return build4(ARRAY_REF, TREE_TYPE(type_tree), array_tree,
			start_tree, NULL_TREE, NULL_TREE);
	}
      else
	{
	  // Open array.
	  tree values = array_type->value_pointer_tree(gogo, array_tree);
	  tree element_type_tree = array_type->element_type()->get_tree(gogo);
	  tree element_size = TYPE_SIZE_UNIT(element_type_tree);
	  tree offset = fold_build2_loc(loc, MULT_EXPR, TREE_TYPE(length_tree),
					start_tree,
					fold_convert_loc(loc,
							 TREE_TYPE(length_tree),
							 element_size));
	  tree ptr = fold_build2_loc(loc, POINTER_PLUS_EXPR,
				     TREE_TYPE(values), values,
				     fold_convert_loc(loc, sizetype, offset));
	  return build_fold_indirect_ref(ptr);
	}
    }

  // Array slice.

  tree capacity_tree = array_type->capacity_tree(gogo, array_tree);
  capacity_tree = fold_convert_loc(loc, TREE_TYPE(length_tree),
				   capacity_tree);

  tree end_tree;
  if (this->end_->is_nil_expression())
    end_tree = length_tree;
  else
    {
      end_tree = this->end_->get_tree(context);
      if (end_tree == error_mark_node)
	return error_mark_node;
      end_tree = fold_convert_loc(loc, TREE_TYPE(length_tree), end_tree);

      if (!DECL_P(end_tree))
	end_tree = save_expr(end_tree);
      capacity_tree = save_expr(capacity_tree);
      tree bad_end = fold_build2_loc(loc, TRUTH_OR_EXPR, boolean_type_node,
				     fold_build2_loc(loc, LT_EXPR,
						     boolean_type_node,
						     end_tree, start_tree),
				     fold_build2_loc(loc, GT_EXPR,
						     boolean_type_node,
						     end_tree, capacity_tree));
      bad_index = fold_build2_loc(loc, TRUTH_OR_EXPR, boolean_type_node,
				  bad_index, bad_end);
    }

  tree element_type_tree = array_type->element_type()->get_tree(gogo);
  tree element_size = TYPE_SIZE_UNIT(element_type_tree);
  element_size = fold_convert_loc(loc, TREE_TYPE(length_tree), element_size);

  tree offset = fold_build2_loc(loc, MULT_EXPR, TREE_TYPE(length_tree),
				start_tree, element_size);

  tree value_pointer = array_type->value_pointer_tree(gogo, array_tree);

  value_pointer = fold_build2_loc(loc, POINTER_PLUS_EXPR,
				  TREE_TYPE(value_pointer),
				  value_pointer,
				  fold_convert_loc(loc, sizetype, offset));

  tree result_length_tree = fold_build2_loc(loc, MINUS_EXPR,
					    TREE_TYPE(length_tree),
					    end_tree, start_tree);

  tree result_capacity_tree = fold_build2_loc(loc, MINUS_EXPR,
					      TREE_TYPE(length_tree),
					      capacity_tree, start_tree);

  tree struct_tree = this->type()->get_tree(gogo);
  gcc_assert(TREE_CODE(struct_tree) == RECORD_TYPE);

  VEC(constructor_elt,gc)* init = VEC_alloc(constructor_elt, gc, 3);

  constructor_elt* elt = VEC_quick_push(constructor_elt, init, NULL);
  tree field = TYPE_FIELDS(struct_tree);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__values") == 0);
  elt->index = field;
  elt->value = value_pointer;

  elt = VEC_quick_push(constructor_elt, init, NULL);
  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__count") == 0);
  elt->index = field;
  elt->value = fold_convert_loc(loc, TREE_TYPE(field), result_length_tree);

  elt = VEC_quick_push(constructor_elt, init, NULL);
  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__capacity") == 0);
  elt->index = field;
  elt->value = fold_convert_loc(loc, TREE_TYPE(field), result_capacity_tree);

  tree constructor = build_constructor(struct_tree, init);

  if (TREE_CONSTANT(value_pointer)
      && TREE_CONSTANT(result_length_tree)
      && TREE_CONSTANT(result_capacity_tree))
    TREE_CONSTANT(constructor) = 1;

  return fold_build2_loc(loc, COMPOUND_EXPR, TREE_TYPE(constructor),
			 build3(COND_EXPR, void_type_node,
				bad_index, crash, NULL_TREE),
			 constructor);
}

// Make an array index expression.  END may be NULL.

Expression*
Expression::make_array_index(Expression* array, Expression* start,
			     Expression* end, source_location location)
{
  return new Array_index_expression(array, start, end, location);
}

// A string index.  This is used for both indexing and slicing.

class String_index_expression : public Expression
{
 public:
  String_index_expression(Expression* string, Expression* start,
			  Expression* end, source_location location)
    : Expression(EXPRESSION_STRING_INDEX, location),
      string_(string), start_(start), end_(end), is_being_copied_(false)
  { }

 protected:
  int
  do_traverse(Traverse*);

  Type*
  do_type();

  void
  do_determine_type(const Type_context*);

  void
  do_check_types(Gogo*);

  Expression*
  do_copy()
  {
    return Expression::make_string_index(this->string_->copy(),
					 this->start_->copy(),
					 (this->end_ == NULL
					  ? NULL
					  : this->end_->copy()),
					 this->location());
  }

  bool
  do_address_taken(source_location, bool)
  { return true; }

  Expression*
  do_being_copied(Refcounts*, bool);

  Expression*
  do_note_decrements(Refcounts*);

  tree
  do_get_tree(Translate_context*);

 private:
  // The string we are getting a value from.
  Expression* string_;
  // The start or only index.
  Expression* start_;
  // The end index of a slice.  This may be NULL for a single index,
  // or it may be a nil expression for the length of the string.
  Expression* end_;
  // Whether the slice is being copied.
  bool is_being_copied_;
};

// String index traversal.

int
String_index_expression::do_traverse(Traverse* traverse)
{
  if (Expression::traverse(&this->string_, traverse) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  if (Expression::traverse(&this->start_, traverse) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  if (this->end_ != NULL)
    {
      if (Expression::traverse(&this->end_, traverse) == TRAVERSE_EXIT)
	return TRAVERSE_EXIT;
    }
  return TRAVERSE_CONTINUE;
}

// Return the type of a string index.

Type*
String_index_expression::do_type()
{
  if (this->end_ == NULL)
    return Type::lookup_integer_type("uint8");
  else
    return Type::make_string_type();
}

// Determine the type of a string index.

void
String_index_expression::do_determine_type(const Type_context*)
{
  this->string_->determine_type_no_context();
  Type_context subcontext(NULL, true);
  this->start_->determine_type(&subcontext);
  if (this->end_ != NULL)
    this->end_->determine_type(&subcontext);
}

// Check types of a string index.

void
String_index_expression::do_check_types(Gogo*)
{
  if (this->start_->type()->integer_type() == NULL)
    this->report_error(_("index must be integer"));
  if (this->end_ != NULL
      && this->end_->type()->integer_type() == NULL
      && !this->end_->is_nil_expression())
    this->report_error(_("slice end must be integer"));

  std::string sval;
  bool sval_valid = this->string_->string_constant_value(&sval);

  mpz_t ival;
  mpz_init(ival);
  Type* dummy;
  if (this->start_->integer_constant_value(true, ival, &dummy))
    {
      if (mpz_sgn(ival) < 0
	  || (sval_valid && mpz_cmp_ui(ival, sval.length()) >= 0))
	{
	  error_at(this->start_->location(), "string index out of bounds");
	  this->set_is_error();
	}
    }
  if (this->end_ != NULL && !this->end_->is_nil_expression())
    {
      if (this->end_->integer_constant_value(true, ival, &dummy))
	{
	  if (mpz_sgn(ival) < 0
	      || (sval_valid && mpz_cmp_ui(ival, sval.length()) > 0))
	    {
	      error_at(this->end_->location(), "string index out of bounds");
	      this->set_is_error();
	    }
	}
    }
  mpz_clear(ival);
}

// Copying a string index or a string slice does not require any
// special action.

Expression*
String_index_expression::do_being_copied(Refcounts*, bool)
{
  this->is_being_copied_ = true;
  return this;
}

// A string slice introduces a new reference which must be decremented
// if the value is discarded.

Expression*
String_index_expression::do_note_decrements(Refcounts* refcounts)
{
  if (this->end_ == NULL || this->is_being_copied_)
    return this;
  return Expression::make_refcount_adjust(refcounts, REFCOUNT_DECREMENT_NEW,
					  this, false);
}

// Get a tree for a string index.

tree
String_index_expression::do_get_tree(Translate_context* context)
{
  tree string_tree = this->string_->get_tree(context);
  if (string_tree == error_mark_node)
    return error_mark_node;

  if (this->string_->type()->points_to() != NULL)
    string_tree = build_fold_indirect_ref(string_tree);

  tree start_tree = this->start_->get_tree(context);
  if (start_tree == error_mark_node)
    return error_mark_node;
  start_tree = fold_convert(sizetype, start_tree);

  tree string_type = TREE_TYPE(string_tree);

  if (this->end_ == NULL)
    {
      if (!DECL_P(string_tree))
	string_tree = save_expr(string_tree);
      if (!DECL_P(start_tree))
	start_tree = save_expr(start_tree);

      tree length_tree = String_type::length_tree(context->gogo(),
						  string_tree);
      tree bad_index = fold_build2(GE_EXPR, boolean_type_node,
				   start_tree, length_tree);

      // FIXME: Duplicates Array_index::do_get_tree.
      static tree bad_index_fndecl;
      tree crash = Gogo::call_builtin(&bad_index_fndecl,
				      this->location(),
				      "__go_bad_index",
				      0,
				      void_type_node);
      TREE_THIS_VOLATILE(bad_index_fndecl) = 1;

      tree bytes_tree = String_type::bytes_tree(context->gogo(), string_tree);
      tree ptr = fold_build2(POINTER_PLUS_EXPR, TREE_TYPE(bytes_tree),
			     bytes_tree, start_tree);
      tree index = build_fold_indirect_ref(ptr);

      return fold_build2(COMPOUND_EXPR, TREE_TYPE(index),
			 build3(COND_EXPR, void_type_node,
				bad_index, crash, NULL_TREE),
			 index);
    }
  else
    {
      tree end_tree;
      if (this->end_->is_nil_expression())
	end_tree = integer_minus_one_node;
      else
	{
	  end_tree = this->end_->get_tree(context);
	  if (end_tree == error_mark_node)
	    return error_mark_node;
	}
      end_tree = fold_convert(sizetype, end_tree);
      static tree strslice_fndecl;
      return Gogo::call_builtin(&strslice_fndecl,
				this->location(),
				"__go_string_slice",
				3,
				string_type,
				string_type,
				string_tree,
				sizetype,
				start_tree,
				sizetype,
				end_tree);
    }
}

// Make a string index expression.  END may be NULL.

Expression*
Expression::make_string_index(Expression* string, Expression* start,
			      Expression* end, source_location location)
{
  return new String_index_expression(string, start, end, location);
}

// Class Map_index.

// Get the type of the map.

Map_type*
Map_index_expression::get_map_type() const
{
  Map_type* mt = this->map_->type()->deref()->map_type();
  gcc_assert(mt != NULL);
  return mt;
}

// Map index traversal.

int
Map_index_expression::do_traverse(Traverse* traverse)
{
  if (Expression::traverse(&this->map_, traverse) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  return Expression::traverse(&this->index_, traverse);
}

// Return the type of a map index.

Type*
Map_index_expression::do_type()
{
  Type* type = this->get_map_type()->val_type();
  // If this map index is in a tuple assignment, we actually return a
  // pointer to the value type.  Tuple_map_assignment_statement is
  // responsible for handling this correctly.  We need to get the type
  // right in case this gets assigned to a temporary variable.
  if (this->is_in_tuple_assignment_)
    type = Type::make_pointer_type(type);
  return type;
}

// Fix the type of a map index.

void
Map_index_expression::do_determine_type(const Type_context*)
{
  this->map_->determine_type_no_context();
  Type_context subcontext(this->get_map_type()->key_type(), true);
  this->index_->determine_type(&subcontext);
}

// Check types of a map index.

void
Map_index_expression::do_check_types(Gogo*)
{
  std::string reason;
  if (!Type::are_compatible_for_assign(this->get_map_type()->key_type(),
				       this->index_->type(),
				       &reason))
    {
      if (reason.empty())
	this->report_error(_("incompatible type for map index"));
      else
	{
	  error_at(this->location(), "incompatible type for map index (%s)",
		   reason.c_str());
	  this->set_is_error();
	}
    }
}

// We can take the address of a map index expression if it is an
// lvalue.

bool
Map_index_expression::do_address_taken(source_location location, bool)
{
  if (!this->is_lvalue_)
    {
      this->report_address_taken_error(location);
      return false;
    }
  return true;
}

// If we are copying the map index to a variable, we need to increment
// the reference count.

Expression*
Map_index_expression::do_being_copied(Refcounts* refcounts, bool for_local)
{
  if (!this->type()->has_refcounted_component())
    return this;
  return Expression::make_refcount_adjust(refcounts,
					  REFCOUNT_INCREMENT_COPIED,
					  this, for_local);
}

// We don't do anything with the reference count of a map index if we
// aren't copying it, so there is nothing to do here.  FIXME: Is this
// safe?

Expression*
Map_index_expression::do_note_decrements(Refcounts*)
{
  return this;
}

// If we are changing the map index, we need to decrement the
// reference count for the old value, and we may need to increment the
// reference count for the new index.

Expression*
Map_index_expression::do_being_set(Refcounts* refcounts)
{
  if (this->get_map_type()->key_type()->has_refcounted_component())
    this->index_ = this->index_->being_copied(refcounts, false);
  if (!this->type()->has_refcounted_component())
    return this;
  return Expression::make_refcount_decrement_lvalue(refcounts, this);
}

// Get a tree for a map index.

tree
Map_index_expression::do_get_tree(Translate_context* context)
{
  Map_type* type = this->get_map_type();

  tree valptr = this->get_value_pointer(context, this->is_lvalue_);
  if (valptr == error_mark_node)
    return error_mark_node;
  valptr = save_expr(valptr);

  tree val_type_tree = TREE_TYPE(TREE_TYPE(valptr));

  if (this->is_lvalue_)
    return build_fold_indirect_ref(valptr);
  else if (this->is_in_tuple_assignment_)
    {
      // Tuple_map_assignment_statement is responsible for using this
      // appropriately.
      return valptr;
    }
  else
    {
      return fold_build3(COND_EXPR, val_type_tree,
			 fold_build2(EQ_EXPR, boolean_type_node, valptr,
				     fold_convert(TREE_TYPE(valptr),
						  null_pointer_node)),
			 type->val_type()->get_init_tree(context->gogo(),
							 false),
			 build_fold_indirect_ref(valptr));
    }
}

// Get a tree for the map index.  This returns a tree which evaluates
// to a pointer to a value.  The pointer will be NULL if the key is
// not in the map.

tree
Map_index_expression::get_value_pointer(Translate_context* context,
					bool insert)
{
  Map_type* type = this->get_map_type();

  tree map_tree = this->map_->get_tree(context);
  tree index_tree = this->index_->get_tree(context);
  if (map_tree == error_mark_node || index_tree == error_mark_node)
    return error_mark_node;

  if (this->map_->type()->points_to() != NULL)
    map_tree = build_fold_indirect_ref(map_tree);

  // We need to pass in a pointer to the key, so stuff it into a
  // variable.
  tree tmp = create_tmp_var(TREE_TYPE(index_tree), get_name(index_tree));
  DECL_IGNORED_P(tmp) = 0;
  DECL_INITIAL(tmp) = index_tree;
  tree make_tmp = build1(DECL_EXPR, void_type_node, tmp);
  tree tmpref = fold_convert(const_ptr_type_node, build_fold_addr_expr(tmp));
  TREE_ADDRESSABLE(tmp) = 1;

  static tree map_index_fndecl;
  tree call = Gogo::call_builtin(&map_index_fndecl,
				 this->location(),
				 "__go_map_index",
				 3,
				 const_ptr_type_node,
				 TREE_TYPE(map_tree),
				 map_tree,
				 const_ptr_type_node,
				 tmpref,
				 boolean_type_node,
				 (insert
				  ? boolean_true_node
				  : boolean_false_node));

  tree val_type_tree = type->val_type()->get_tree(context->gogo());
  if (val_type_tree == error_mark_node)
    return error_mark_node;
  tree ptr_val_type_tree = build_pointer_type(val_type_tree);

  return build2(COMPOUND_EXPR, ptr_val_type_tree,
		make_tmp,
		fold_convert(ptr_val_type_tree, call));
}

// Return a tree to delete this key from the map.

tree
Map_index_expression::delete_key(Translate_context* context)
{
  tree map_tree = this->map_->get_tree(context);
  tree index_tree = this->index_->get_tree(context);
  if (map_tree == error_mark_node || index_tree == error_mark_node)
    return error_mark_node;

  if (this->map_->type()->points_to() != NULL)
    map_tree = build_fold_indirect_ref(map_tree);

  // We need to pass in a pointer to the key, so stuff it in a
  // variable.
  tree tmp = create_tmp_var(TREE_TYPE(index_tree), get_name(index_tree));
  DECL_IGNORED_P(tmp) = 0;
  DECL_INITIAL(tmp) = index_tree;
  tree make_tmp = build1(DECL_EXPR, void_type_node, tmp);
  tree tmpref = fold_convert(const_ptr_type_node, build_fold_addr_expr(tmp));
  TREE_ADDRESSABLE(tmp) = 1;

  static tree map_delete_fndecl;
  tree call = Gogo::call_builtin(&map_delete_fndecl,
				 this->location(),
				 "__go_map_delete",
				 2,
				 void_type_node,
				 TREE_TYPE(map_tree),
				 map_tree,
				 const_ptr_type_node,
				 tmpref);

  return build2(COMPOUND_EXPR, void_type_node, make_tmp, call);
}

// Make a map index expression.

Map_index_expression*
Expression::make_map_index(Expression* map, Expression* index,
			   source_location location)
{
  return new Map_index_expression(map, index, location);
}

// Class Field_reference_expression.

// Return the type of a field reference.

Type*
Field_reference_expression::do_type()
{
  Type* expr_type = this->expr_->type();
  Type* points_to = expr_type->points_to();
  if (points_to != NULL)
    expr_type = points_to;
  Struct_type* struct_type = expr_type->struct_type();
  gcc_assert(struct_type != NULL);
  return struct_type->field(this->field_index_)->type();
}

// Check the types for a field reference.

void
Field_reference_expression::do_check_types(Gogo*)
{
  Type* expr_type = this->expr_->type();
  Type* points_to = expr_type->points_to();
  if (points_to != NULL)
    expr_type = points_to;
  Struct_type* struct_type = expr_type->struct_type();
  gcc_assert(struct_type != NULL);
  gcc_assert(struct_type->field(this->field_index_) != NULL);
}

// We can take the address of a field, and it implies taking the
// address of the whole structure.

bool
Field_reference_expression::do_address_taken(source_location location,
					     bool escapes)
{
  if (!this->expr_->address_taken(location, escapes))
    return false;
  return true;
}

// If we are copying the field to a variable, we need to increment the
// reference count.

Expression*
Field_reference_expression::do_being_copied(Refcounts* refcounts,
					    bool for_local)
{
  if (!this->type()->has_refcounted_component())
    return this;
  return Expression::make_refcount_adjust(refcounts,
					  REFCOUNT_INCREMENT_COPIED,
					  this, for_local);
}

// We do not need to bump the reference count; any change to the
// structure before the value of the field is used would mean an
// invalid program.

Expression*
Field_reference_expression::do_note_decrements(Refcounts*)
{
  return this;
}

// If we are changing the field, we need to decrement the reference
// count for the old value.

Expression*
Field_reference_expression::do_being_set(Refcounts* refcounts)
{
  if (!this->type()->has_refcounted_component())
    return this;
  return Expression::make_refcount_decrement_lvalue(refcounts, this);
}

// Get a tree for a field reference.

tree
Field_reference_expression::do_get_tree(Translate_context* context)
{
  tree struct_tree = this->expr_->get_tree(context);
  if (struct_tree == error_mark_node
      || TREE_TYPE(struct_tree) == error_mark_node)
    return error_mark_node;

  if (POINTER_TYPE_P(TREE_TYPE(struct_tree)))
    {
      // If we are dereferencing the pointer to a large struct, we
      // need to check for nil.  We don't bother to check for small
      // structs because we expect the system to crash on a nil
      // pointer dereference.
      HOST_WIDE_INT s = int_size_in_bytes(TREE_TYPE(TREE_TYPE(struct_tree)));
      if (s == -1 || s >= 4096)
	{
	  if (!DECL_P(struct_tree))
	    struct_tree = save_expr(struct_tree);
	  tree compare = build2(EQ_EXPR, boolean_type_node, struct_tree,
				fold_convert(TREE_TYPE(struct_tree),
					     null_pointer_node));
	  // FIXME: This should be a different error message.
	  static tree bad_index_fndecl;
	  tree crash = Gogo::call_builtin(&bad_index_fndecl,
					  this->location(),
					  "__go_bad_index",
					  0,
					  void_type_node);
	  TREE_THIS_VOLATILE(bad_index_fndecl) = 1;
	  struct_tree = build2(COMPOUND_EXPR, TREE_TYPE(struct_tree),
			       build3(COND_EXPR, void_type_node, compare,
				      crash, NULL_TREE),
			       struct_tree);
	}
      struct_tree = build_fold_indirect_ref(struct_tree);
    }

  gcc_assert(TREE_CODE(TREE_TYPE(struct_tree)) == RECORD_TYPE);
  tree field = TYPE_FIELDS(TREE_TYPE(struct_tree));
  gcc_assert(field != NULL_TREE);
  for (unsigned int i = this->field_index_; i > 0; --i)
    {
      field = TREE_CHAIN(field);
      gcc_assert(field != NULL_TREE);
    }
  return build3(COMPONENT_REF, TREE_TYPE(field), struct_tree, field,
		NULL_TREE);
}

// Make a reference to a qualified identifier in an expression.

Field_reference_expression*
Expression::make_field_reference(Expression* expr, unsigned int field_index,
				 source_location location)
{
  return new Field_reference_expression(expr, field_index, location);
}

// Class Interface_field_reference_expression.

// Return a tree for the pointer to the function to call.

tree
Interface_field_reference_expression::get_function_tree(Translate_context*,
							tree expr)
{
  if (this->expr_->type()->points_to() != NULL)
    expr = build_fold_indirect_ref(expr);

  tree expr_type = TREE_TYPE(expr);
  gcc_assert(POINTER_TYPE_P(expr_type));
  expr_type = TREE_TYPE(expr_type);
  expr = build_fold_indirect_ref(expr);
  gcc_assert(TREE_CODE(expr_type) == RECORD_TYPE);

  tree field = TREE_CHAIN(TYPE_FIELDS(expr_type));
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__methods") == 0);

  tree table = build3(COMPONENT_REF, TREE_TYPE(field), expr, field, NULL_TREE);
  gcc_assert(POINTER_TYPE_P(TREE_TYPE(table)));

  table = build_fold_indirect_ref(table);
  gcc_assert(TREE_CODE(TREE_TYPE(table)) == RECORD_TYPE);

  std::string name = Gogo::unpack_hidden_name(this->name_);
  for (field = TYPE_FIELDS(TREE_TYPE(table));
       field != NULL_TREE;
       field = TREE_CHAIN(field))
    {
      if (name == IDENTIFIER_POINTER(DECL_NAME(field)))
	break;
    }
  gcc_assert(field != NULL_TREE);

  return build3(COMPONENT_REF, TREE_TYPE(field), table, field, NULL_TREE);
}

// Return a tree for the first argument to pass to the interface
// function.

tree
Interface_field_reference_expression::get_underlying_object_tree(
    Translate_context*,
    tree expr)
{
  if (this->expr_->type()->points_to() != NULL)
    expr = build_fold_indirect_ref(expr);

  tree expr_type = TREE_TYPE(expr);
  gcc_assert(POINTER_TYPE_P(expr_type));
  expr_type = TREE_TYPE(expr_type);
  expr = build_fold_indirect_ref(expr);
  gcc_assert(TREE_CODE(expr_type) == RECORD_TYPE);

  tree field = TREE_CHAIN(TREE_CHAIN(TYPE_FIELDS(expr_type)));
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__object") == 0);

  return build3(COMPONENT_REF, TREE_TYPE(field), expr, field, NULL_TREE);
}

// Traversal.

int
Interface_field_reference_expression::do_traverse(Traverse* traverse)
{
  return Expression::traverse(&this->expr_, traverse);
}

// Return the type of an interface field reference.

Type*
Interface_field_reference_expression::do_type()
{
  Type* expr_type = this->expr_->type();

  Type* points_to = expr_type->points_to();
  if (points_to != NULL)
    expr_type = points_to;

  Interface_type* interface_type = expr_type->interface_type();
  if (interface_type == NULL)
    return Type::make_error_type();

  const Typed_identifier* method = interface_type->find_method(this->name_);
  if (method == NULL)
    return Type::make_error_type();

  return Type::make_pointer_type(method->type());
}

// Determine types.

void
Interface_field_reference_expression::do_determine_type(const Type_context*)
{
  this->expr_->determine_type_no_context();
}

// Check the types for an interface field reference.

void
Interface_field_reference_expression::do_check_types(Gogo*)
{
  Type* type = this->expr_->type();

  Type* points_to = type->points_to();
  if (points_to != NULL)
    type = points_to;

  Interface_type* interface_type = type->interface_type();
  if (interface_type == NULL)
    this->report_error(_("expected interface or pointer to interface"));
  else
    {
      const Typed_identifier* method =
	interface_type->find_method(this->name_);
      if (method == NULL)
	{
	  error_at(this->location(), "method %qs not in interface",
		   this->name_.c_str());
	  this->set_is_error();
	}
    }
}

// Get a tree for a reference to a field in an interface.  There is no
// standard tree type representation for this: it's a function
// attached to its first argument, like a Bound_method_expression.
// The only places it may currently be used are in a Call_expression
// or a Go_statement, which will take it apart directly.  So this has
// nothing to do at present.

tree
Interface_field_reference_expression::do_get_tree(Translate_context*)
{
  gcc_unreachable();
}

// Make a reference to a field in an interface.

Expression*
Expression::make_interface_field_reference(Expression* expr,
					   const std::string& field,
					   source_location location)
{
  return new Interface_field_reference_expression(expr, field, location);
}

// A general selector.  This is a Parser_expression for LEFT.NAME.  It
// is lowered after we know the type of the left hand side.

class Selector_expression : public Parser_expression
{
 public:
  Selector_expression(Expression* left, const std::string& name,
		      source_location location)
    : Parser_expression(EXPRESSION_SELECTOR, location),
      left_(left), name_(name)
  { }

 protected:
  int
  do_traverse(Traverse* traverse)
  { return Expression::traverse(&this->left_, traverse); }

  Expression*
  do_lower(Gogo*, int);

  Expression*
  do_copy()
  {
    return new Selector_expression(this->left_->copy(), this->name_,
				   this->location());
  }

 private:
  // The expression on the left hand side.
  Expression* left_;
  // The name on the right hand side.
  std::string name_;
};

// Lower a selector expression once we know the real type of the left
// hand side.

Expression*
Selector_expression::do_lower(Gogo*, int)
{
  source_location location = this->location();
  Expression* left = this->left_;
  const std::string& name(this->name_);

  Type* type = left->type();
  Type* ptype = type->deref();
  Struct_type* struct_type = ptype->struct_type();
  Interface_type* interface_type = ptype->interface_type();
  Named_type* named_type = type->named_type();
  if (named_type == NULL && type != ptype)
    named_type = ptype->named_type();

  if (type->is_error_type())
    return Expression::make_error(location);
  else if (struct_type != NULL && named_type == NULL)
    {
      Expression* ret = struct_type->field_reference(left, name, location);
      if (ret != NULL)
	return ret;
    }
  else if (interface_type != NULL && interface_type->find_method(name) != NULL)
    return Expression::make_interface_field_reference(left, name, location);

  if (named_type != NULL)
    {
      bool found_pointer_method;
      Expression* ret = named_type->bind_field_or_method(left, name, location,
							 &found_pointer_method);
      if (ret != NULL)
	return ret;
      if (found_pointer_method)
	{
	  error_at(location, "method requires a pointer");
	  return Expression::make_error(location);
	}
    }

  if (struct_type == NULL && interface_type == NULL && named_type == NULL)
    error_at(location,
	     "request for %qs in something which has no fields or methods",
	     Gogo::unpack_hidden_name(name).c_str());
  else
    error_at(location, "reference to undefined field or method %qs",
	     Gogo::unpack_hidden_name(name).c_str());
  return Expression::make_error(location);
}

// Make a selector expression.

Expression*
Expression::make_selector(Expression* left, const std::string& name,
			  source_location location)
{
  return new Selector_expression(left, name, location);
}

// Implement the builtin function new.

class Allocation_expression : public Expression
{
 public:
  Allocation_expression(Type* type, source_location location)
    : Expression(EXPRESSION_ALLOCATION, location),
      type_(type), is_being_copied_(false)
  { }

 protected:
  int
  do_traverse(Traverse* traverse)
  { return Type::traverse(this->type_, traverse); }

  Type*
  do_type()
  { return Type::make_pointer_type(this->type_); }

  void
  do_determine_type(const Type_context*)
  { }

  void
  do_check_types(Gogo*);

  Expression*
  do_copy()
  { return new Allocation_expression(this->type_, this->location()); }

  Expression*
  do_being_copied(Refcounts*, bool);

  Expression*
  do_note_decrements(Refcounts*);

  tree
  do_get_tree(Translate_context*);

 private:
  // The type we are allocating.
  Type* type_;
  // Whether this value is being copied.
  bool is_being_copied_;
};

// Check the type of an allocation expression.

void
Allocation_expression::do_check_types(Gogo*)
{
  if (this->type_->function_type() != NULL)
    this->report_error(_("invalid new of function type"));
}

// An allocation expression arrives with a reference count, so nothing
// special is needed to copy it.

Expression*
Allocation_expression::do_being_copied(Refcounts*, bool)
{
  this->is_being_copied_ = true;
  return this;
}

// An allocation expression arrives with a reference count, so if we
// don't copy the value we must free it.

Expression*
Allocation_expression::do_note_decrements(Refcounts* refcounts)
{
  if (this->is_being_copied_)
    return this;
  return Expression::make_refcount_adjust(refcounts, REFCOUNT_DECREMENT_NEW,
					  this, false);
}

// Return a tree for an allocation expression.

tree
Allocation_expression::do_get_tree(Translate_context* context)
{
  tree type_tree = this->type_->get_tree(context->gogo());
  tree size_tree = TYPE_SIZE_UNIT(type_tree);
  tree space = context->gogo()->allocate_memory(size_tree, this->location());
  return fold_convert(build_pointer_type (type_tree), space);
}

// Make an allocation expression.

Expression*
Expression::make_allocation(Type* type, source_location location)
{
  return new Allocation_expression(type, location);
}

// Implement the builtin function make.

class Make_expression : public Expression
{
 public:
  Make_expression(Type* type, Expression_list* args, source_location location)
    : Expression(EXPRESSION_MAKE, location),
      type_(type), args_(args), is_being_copied_(false)
  { }

 protected:
  int
  do_traverse(Traverse* traverse);

  Type*
  do_type()
  { return this->type_; }

  void
  do_determine_type(const Type_context*);

  void
  do_check_types(Gogo*);

  Expression*
  do_copy()
  {
    return new Make_expression(this->type_, this->args_->copy(),
			       this->location());
  }

  Expression*
  do_being_copied(Refcounts*, bool);

  Expression*
  do_note_decrements(Refcounts*);

  tree
  do_get_tree(Translate_context*);

 private:
  // The type we are making.
  Type* type_;
  // The arguments to pass to the make routine.
  Expression_list* args_;
  // Whether the expression is being copied.
  bool is_being_copied_;
};

// Traversal.

int
Make_expression::do_traverse(Traverse* traverse)
{
  if (this->args_ != NULL
      && this->args_->traverse(traverse) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  if (Type::traverse(this->type_, traverse) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  return TRAVERSE_CONTINUE;
}

// Set types of arguments.

void
Make_expression::do_determine_type(const Type_context*)
{
  if (this->args_ != NULL)
    {
      for (Expression_list::const_iterator pe = this->args_->begin();
	   pe != this->args_->end();
	   ++pe)
	(*pe)->determine_type_no_context();
    }
}

// Check types for a make expression.

void
Make_expression::do_check_types(Gogo*)
{
  if (this->type_->channel_type() == NULL
      && this->type_->map_type() == NULL
      && (this->type_->array_type() == NULL
	  || this->type_->array_type()->length() != NULL))
    this->report_error(_("invalid type for make function"));
  else if (!this->type_->check_make_expression(this->args_, this->location()))
    this->set_is_error();
}

// An newly created object arrives with a reference count, so nothing
// special is needed to copy it.

Expression*
Make_expression::do_being_copied(Refcounts*, bool)
{
  this->is_being_copied_ = true;
  return this;
}

// A newly created object arrives with a reference count, so if we
// don't copy the value we must free it.

Expression*
Make_expression::do_note_decrements(Refcounts* refcounts)
{
  if (this->is_being_copied_)
    return this;
  return Expression::make_refcount_adjust(refcounts, REFCOUNT_DECREMENT_NEW,
					  this, false);
}

// Return a tree for a make expression.

tree
Make_expression::do_get_tree(Translate_context* context)
{
  return this->type_->make_expression_tree(context, this->args_,
					   this->location());
}

// Make a make expression.

Expression*
Expression::make_make(Type* type, Expression_list* args,
		      source_location location)
{
  return new Make_expression(type, args, location);
}

// Construct a struct.

class Struct_construction_expression : public Expression
{
 public:
  Struct_construction_expression(Type* type, Expression_list* vals,
				 source_location location)
    : Expression(EXPRESSION_STRUCT_CONSTRUCTION, location),
      type_(type), vals_(vals)
  { }

  // Return whether this is a constant initializer.
  bool
  is_constant_struct() const;

 protected:
  int
  do_traverse(Traverse* traverse);

  Type*
  do_type()
  { return this->type_; }

  void
  do_determine_type(const Type_context*);

  void
  do_check_types(Gogo*);

  Expression*
  do_copy()
  {
    return new Struct_construction_expression(this->type_, this->vals_->copy(),
					      this->location());
  }

  bool
  do_address_taken(source_location, bool)
  {
    gcc_assert(this->is_constant_struct());
    return true;
  }

  Expression*
  do_being_copied(Refcounts*, bool);

  Expression*
  do_note_decrements(Refcounts*);

  tree
  do_get_tree(Translate_context*);

  void
  do_export(Export*) const;

 private:
  // The type of the struct to construct.
  Type* type_;
  // The list of values, in order of the fields in the struct.  A NULL
  // entry means that the field should be zero-initialized.
  Expression_list* vals_;
};

// Traversal.

int
Struct_construction_expression::do_traverse(Traverse* traverse)
{
  if (this->vals_ != NULL
      && this->vals_->traverse(traverse) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  if (Type::traverse(this->type_, traverse) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  return TRAVERSE_CONTINUE;
}

// Return whether this is a constant initializer.

bool
Struct_construction_expression::is_constant_struct() const
{
  if (this->vals_ == NULL)
    return true;
  for (Expression_list::const_iterator pv = this->vals_->begin();
       pv != this->vals_->end();
       ++pv)
    {
      if (*pv != NULL
	  && !(*pv)->is_constant()
	  && (!(*pv)->is_composite_literal()
	      || (*pv)->is_nonconstant_composite_literal()))
	return false;
    }
  return true;
}

// Final type determination.

void
Struct_construction_expression::do_determine_type(const Type_context*)
{
  if (this->vals_ == NULL)
    return;
  const Struct_field_list* fields = this->type_->struct_type()->fields();
  Expression_list::const_iterator pv = this->vals_->begin();
  for (Struct_field_list::const_iterator pf = fields->begin();
       pf != fields->end();
       ++pf, ++pv)
    {
      if (pv == this->vals_->end())
	return;
      if (*pv != NULL)
	{
	  Type_context subcontext(pf->type(), false);
	  (*pv)->determine_type(&subcontext);
	}
    }
}

// Check types.

void
Struct_construction_expression::do_check_types(Gogo*)
{
  if (this->vals_ == NULL)
    return;

  Struct_type* st = this->type_->struct_type();
  if (this->vals_->size() > st->field_count())
    {
      this->report_error("too many expressions for struct");
      return;
    }

  const Struct_field_list* fields = st->fields();
  Expression_list::const_iterator pv = this->vals_->begin();
  int i = 0;
  for (Struct_field_list::const_iterator pf = fields->begin();
       pf != fields->end();
       ++pf, ++pv, ++i)
    {
      if (pv == this->vals_->end())
	{
	  this->report_error("too few expressions for struct");
	  break;
	}

      if (*pv == NULL)
	continue;

      std::string reason;
      if (!Type::are_compatible_for_assign(pf->type(), (*pv)->type(),
					   &reason))
	{
	  if (reason.empty())
	    error_at((*pv)->location(),
		     "incompatible type for field %d in struct construction",
		     i + 1);
	  else
	    error_at((*pv)->location(),
		     ("incompatible type for field %d in "
		      "struct construction (%s)"),
		     i + 1, reason.c_str());
	  this->set_is_error();
	}
    }
  gcc_assert(pv == this->vals_->end());
}

// If we are copying the constructed struct, then we need to increment
// the reference count of any elements as needed.

Expression*
Struct_construction_expression::do_being_copied(Refcounts* refcounts,
						bool for_local)
{
  if (this->vals_ == NULL
      || !this->type()->has_refcounted_component()
      || this->is_constant_struct())
    return this;
  Expression_list* newvals = new Expression_list;
  for (Expression_list::const_iterator pv = this->vals_->begin();
       pv != this->vals_->end();
       ++pv)
    {
      if (*pv == NULL)
	newvals->push_back(NULL);
      else if (!(*pv)->type()->has_refcounted_component())
	newvals->push_back(*pv);
      else
	{
	  Expression* e = (*pv)->being_copied(refcounts, for_local);
	  newvals->push_back(e);
	}
    }
  delete this->vals_;
  this->vals_ = newvals;
  return this;
}

// This is called if the constructed struct does not appear on the
// right hand side of an assignment statement--in other words, the
// value is built, used, and discarded.  In this case we don't need to
// do anything.  If any of the values require a reference count
// adjustment, that will be handled by the expression itself.

Expression*
Struct_construction_expression::do_note_decrements(Refcounts*)
{
  return this;
}

// Return a tree for constructing a struct.

tree
Struct_construction_expression::do_get_tree(Translate_context* context)
{
  Gogo* gogo = context->gogo();

  if (this->vals_ == NULL)
    return this->type_->get_init_tree(gogo, false);

  tree type_tree = this->type_->get_tree(gogo);
  if (type_tree == error_mark_node)
    return error_mark_node;
  gcc_assert(TREE_CODE(type_tree) == RECORD_TYPE);

  bool is_constant = true;
  const Struct_field_list* fields = this->type_->struct_type()->fields();
  VEC(constructor_elt,gc)* elts = VEC_alloc(constructor_elt, gc,
					    fields->size());
  Struct_field_list::const_iterator pf = fields->begin();
  Expression_list::const_iterator pv = this->vals_->begin();
  for (tree field = TYPE_FIELDS(type_tree);
       field != NULL_TREE;
       field = TREE_CHAIN(field), ++pf)
    {
      gcc_assert(pf != fields->end());

      tree val;
      if (pv == this->vals_->end())
	val = pf->type()->get_init_tree(gogo, false);
      else if (*pv == NULL)
	{
	  val = pf->type()->get_init_tree(gogo, false);
	  ++pv;
	}
      else
	{
	  val = Expression::convert_for_assignment(context, pf->type(),
						   (*pv)->type(),
						   (*pv)->get_tree(context),
						   this->location());
	  ++pv;
	}

      if (val == error_mark_node || TREE_TYPE(val) == error_mark_node)
	return error_mark_node;

      constructor_elt* elt = VEC_quick_push(constructor_elt, elts, NULL);
      elt->index = field;
      elt->value = val;
      if (!TREE_CONSTANT(val))
	is_constant = false;
    }
  gcc_assert(pf == fields->end());

  tree ret = build_constructor(type_tree, elts);
  if (is_constant)
    TREE_CONSTANT(ret) = 1;
  return ret;
}

// Export a struct construction.

void
Struct_construction_expression::do_export(Export* exp) const
{
  exp->write_c_string("convert(");
  exp->write_type(this->type_);
  for (Expression_list::const_iterator pv = this->vals_->begin();
       pv != this->vals_->end();
       ++pv)
    {
      exp->write_c_string(", ");
      if (*pv != NULL)
	(*pv)->export_expression(exp);
    }
  exp->write_c_string(")");
}

// Make a struct composite literal.  This used by the thunk code.

Expression*
Expression::make_struct_composite_literal(Struct_type* type,
					  Expression_list* vals,
					  source_location location)
{
  return new Struct_construction_expression(type, vals, location);
}

// Construct an array.  This class is not used directly; instead we
// use the child classes, Fixed_array_construction_expression and
// Open_array_construction_expression.

class Array_construction_expression : public Expression
{
 protected:
  Array_construction_expression(Expression_classification classification,
				Type* type, Expression_list* vals,
				source_location location)
    : Expression(classification, location),
      type_(type), vals_(vals)
  { }

 public:
  // Return whether this is a constant initializer.
  bool
  is_constant_array() const;

  // Return the number of elements.
  size_t
  element_count() const
  { return this->vals_ == NULL ? 0 : this->vals_->size(); }

protected:
  int
  do_traverse(Traverse* traverse);

  Type*
  do_type()
  { return this->type_; }

  void
  do_determine_type(const Type_context*);

  void
  do_check_types(Gogo*);

  bool
  do_address_taken(source_location, bool)
  {
    gcc_assert(this->is_constant_array());
    return true;
  }

  Expression*
  do_being_copied(Refcounts*, bool);

  Expression*
  do_note_decrements(Refcounts* refcounts);

  void
  do_export(Export*) const;

  // The list of values.
  Expression_list*
  vals()
  { return this->vals_; }

  // Get a constructor tree for the array values.
  tree
  get_constructor_tree(Translate_context* context, tree type_tree);

 private:
  // The type of the array to construct.
  Type* type_;
  // The list of values.
  Expression_list* vals_;
};

// Traversal.

int
Array_construction_expression::do_traverse(Traverse* traverse)
{
  if (this->vals_ != NULL
      && this->vals_->traverse(traverse) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  if (Type::traverse(this->type_, traverse) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  return TRAVERSE_CONTINUE;
}

// Return whether this is a constant initializer.

bool
Array_construction_expression::is_constant_array() const
{
  if (this->vals_ == NULL)
    return true;

  // There are no constant constructors for interfaces.
  if (this->type_->array_type()->element_type()->interface_type() != NULL)
    return false;

  for (Expression_list::const_iterator pv = this->vals_->begin();
       pv != this->vals_->end();
       ++pv)
    {
      if (*pv != NULL
	  && !(*pv)->is_constant()
	  && (!(*pv)->is_composite_literal()
	      || (*pv)->is_nonconstant_composite_literal()))
	return false;
    }
  return true;
}

// Final type determination.

void
Array_construction_expression::do_determine_type(const Type_context*)
{
  if (this->vals_ == NULL)
    return;
  Type_context subcontext(this->type_->array_type()->element_type(), false);
  for (Expression_list::const_iterator pv = this->vals_->begin();
       pv != this->vals_->end();
       ++pv)
    {
      if (*pv != NULL)
	(*pv)->determine_type(&subcontext);
    }
}

// Check types.

void
Array_construction_expression::do_check_types(Gogo*)
{
  if (this->vals_ == NULL)
    return;

  Array_type* at = this->type_->array_type();
  int i = 0;
  Type* element_type = at->element_type();
  for (Expression_list::const_iterator pv = this->vals_->begin();
       pv != this->vals_->end();
       ++pv, ++i)
    {
      if (*pv != NULL
	  && !Type::are_compatible_for_assign(element_type, (*pv)->type(),
					      NULL))
	{
	  error_at((*pv)->location(),
		   "incompatible type for element %d in composite literal",
		   i + 1);
	  this->set_is_error();
	}
    }

  Expression* length = at->length();
  if (length != NULL)
    {
      mpz_t val;
      mpz_init(val);
      Type* type;
      if (at->length()->integer_constant_value(true, val, &type))
	{
	  if (this->vals_->size() > mpz_get_ui(val))
	    this->report_error(_("too many elements in composite literal"));
	}
      mpz_clear(val);
    }
}

// If we are copying the constructed array, then we need to increment
// the reference count of any elements as needed.

Expression*
Array_construction_expression::do_being_copied(Refcounts* refcounts,
					       bool for_local)
{
  if (this->vals_ == NULL
      || !this->type()->has_refcounted_component()
      || this->is_constant_array())
    return this;
  Expression_list* newvals = new Expression_list;
  for (Expression_list::const_iterator pv = this->vals_->begin();
       pv != this->vals_->end();
       ++pv)
    {
      if (*pv == NULL)
	newvals->push_back(NULL);
      else if (!(*pv)->type()->has_refcounted_component())
	newvals->push_back(*pv);
      else
	{
	  Expression* e = (*pv)->being_copied(refcounts, for_local);
	  newvals->push_back(e);
	}
    }
  delete this->vals_;
  this->vals_ = newvals;
  return this;
}

// This is called if the constructed array does not appear on the
// right hand side of an assignment statement--in other words, the
// value is built, used, and discarded.  In this case we don't need to
// do anything.  If any of the values require a reference count
// adjustment, that will be handled by the expression itself.

Expression*
Array_construction_expression::do_note_decrements(Refcounts*)
{
  return this;
}

// Get a constructor tree for the array values.

tree
Array_construction_expression::get_constructor_tree(Translate_context* context,
						    tree type_tree)
{
  VEC(constructor_elt,gc)* values = VEC_alloc(constructor_elt, gc,
					      (this->vals_ == NULL
					       ? 0
					       : this->vals_->size()));
  Type* element_type = this->type_->array_type()->element_type();
  bool is_constant = true;
  if (this->vals_ != NULL)
    {
      size_t i = 0;
      for (Expression_list::const_iterator pv = this->vals_->begin();
	   pv != this->vals_->end();
	   ++pv, ++i)
	{
	  constructor_elt* elt = VEC_quick_push(constructor_elt, values, NULL);
	  elt->index = size_int(i);
	  if (*pv == NULL)
	    elt->value = element_type->get_init_tree(context->gogo(), false);
	  else
	    {
	      tree value_tree = (*pv)->get_tree(context);
	      elt->value = Expression::convert_for_assignment(context,
							      element_type,
							      (*pv)->type(),
							      value_tree,
							      this->location());
	    }
	  if (elt->value == error_mark_node)
	    return error_mark_node;
	  if (!TREE_CONSTANT(elt->value))
	    is_constant = false;
	}
    }

  tree ret = build_constructor(type_tree, values);
  if (is_constant)
    TREE_CONSTANT(ret) = 1;
  return ret;
}

// Export an array construction.

void
Array_construction_expression::do_export(Export* exp) const
{
  exp->write_c_string("convert(");
  exp->write_type(this->type_);
  if (this->vals_ != NULL)
    {
      for (Expression_list::const_iterator pv = this->vals_->begin();
	   pv != this->vals_->end();
	   ++pv)
	{
	  exp->write_c_string(", ");
	  if (*pv != NULL)
	    (*pv)->export_expression(exp);
	}
    }
  exp->write_c_string(")");
}

// Construct a fixed array.

class Fixed_array_construction_expression :
  public Array_construction_expression
{
 public:
  Fixed_array_construction_expression(Type* type, Expression_list* vals,
				      source_location location)
    : Array_construction_expression(EXPRESSION_FIXED_ARRAY_CONSTRUCTION,
				    type, vals, location)
  {
    gcc_assert(type->array_type() != NULL
	       && type->array_type()->length() != NULL);
  }

 protected:
  Expression*
  do_copy()
  {
    return new Fixed_array_construction_expression(this->type(),
						   (this->vals() == NULL
						    ? NULL
						    : this->vals()->copy()),
						   this->location());
  }

  tree
  do_get_tree(Translate_context*);
};

// Return a tree for constructing a fixed array.

tree
Fixed_array_construction_expression::do_get_tree(Translate_context* context)
{
  return this->get_constructor_tree(context,
				    this->type()->get_tree(context->gogo()));
}

// Construct an open array.

class Open_array_construction_expression : public Array_construction_expression
{
 public:
  Open_array_construction_expression(Type* type, Expression_list* vals,
				     source_location location)
    : Array_construction_expression(EXPRESSION_OPEN_ARRAY_CONSTRUCTION,
				    type, vals, location)
  {
    gcc_assert(type->array_type() != NULL
	       && type->array_type()->length() == NULL);
  }

 protected:
  // Note that taking the address of an open array literal is invalid.

  Expression*
  do_copy()
  {
    return new Open_array_construction_expression(this->type(),
						  (this->vals() == NULL
						   ? NULL
						   : this->vals()->copy()),
						  this->location());
  }

  tree
  do_get_tree(Translate_context*);
};

// Return a tree for constructing an open array.

tree
Open_array_construction_expression::do_get_tree(Translate_context* context)
{
  Type* element_type = this->type()->array_type()->element_type();
  tree element_type_tree = element_type->get_tree(context->gogo());
  tree values;
  tree length_tree;
  if (this->vals() == NULL || this->vals()->empty())
    {
      // We need to create a unique value.
      tree max = size_int(0);
      tree constructor_type = build_array_type(element_type_tree,
					       build_index_type(max));
      if (constructor_type == error_mark_node)
	return error_mark_node;
      VEC(constructor_elt,gc)* vec = VEC_alloc(constructor_elt, gc, 1);
      constructor_elt* elt = VEC_quick_push(constructor_elt, vec, NULL);
      elt->index = size_int(0);
      elt->value = element_type->get_init_tree(context->gogo(), false);
      values = build_constructor(constructor_type, vec);
      if (TREE_CONSTANT(elt->value))
	TREE_CONSTANT(values) = 1;
      length_tree = size_int(0);
    }
  else
    {
      tree max = size_int(this->vals()->size() - 1);
      tree constructor_type = build_array_type(element_type_tree,
					       build_index_type(max));
      if (constructor_type == error_mark_node)
	return error_mark_node;
      values = this->get_constructor_tree(context, constructor_type);
      length_tree = size_int(this->vals()->size());
    }

  if (values == error_mark_node)
    return error_mark_node;

  tree space;
  tree set;
  if (TREE_CONSTANT(values))
    {
      tree tmp = build_decl(this->location(), VAR_DECL,
			    create_tmp_var_name("C"), TREE_TYPE(values));
      DECL_EXTERNAL(tmp) = 0;
      TREE_PUBLIC(tmp) = 0;
      TREE_STATIC(tmp) = 1;
      DECL_ARTIFICIAL(tmp) = 1;
      // We can't make the decl readonly, because the program can
      // change the values in the array.
      DECL_INITIAL(tmp) = values;
      rest_of_decl_compilation(tmp, 1, 0);
      space = build_fold_addr_expr(tmp);
      set = NULL_TREE;
    }
  else
    {
      tree memsize = TYPE_SIZE_UNIT(TREE_TYPE(values));
      space = context->gogo()->allocate_memory(memsize, this->location());
      space = save_expr(space);

      tree s = fold_convert(build_pointer_type(TREE_TYPE(values)), space);
      set = build2(MODIFY_EXPR, void_type_node,
		   build_fold_indirect_ref(s), values);
    }

  // Build a constructor for the open array.

  tree type_tree = this->type()->get_tree(context->gogo());
  gcc_assert(TREE_CODE(type_tree) == RECORD_TYPE);

  VEC(constructor_elt,gc)* init = VEC_alloc(constructor_elt, gc, 3);

  constructor_elt* elt = VEC_quick_push(constructor_elt, init, NULL);
  tree field = TYPE_FIELDS(type_tree);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__values") == 0);
  elt->index = field;
  elt->value = fold_convert(TREE_TYPE(field), space);

  elt = VEC_quick_push(constructor_elt, init, NULL);
  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__count") == 0);
  elt->index = field;
  elt->value = fold_convert(TREE_TYPE(field), length_tree);

  elt = VEC_quick_push(constructor_elt, init, NULL);
  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)),"__capacity") == 0);
  elt->index = field;
  elt->value = fold_convert(TREE_TYPE(field), length_tree);

  tree constructor = build_constructor(type_tree, init);
  if (TREE_CONSTANT(values))
    TREE_CONSTANT(constructor) = 1;

  if (set == NULL_TREE)
    return constructor;
  else
    return build2(COMPOUND_EXPR, type_tree, set, constructor);
}

// Construct a map.

class Map_construction_expression : public Expression
{
 public:
  Map_construction_expression(Type* type, Expression_list* vals,
			      source_location location)
    : Expression(EXPRESSION_MAP_CONSTRUCTION, location),
      type_(type), vals_(vals), is_being_copied_(false)
  { gcc_assert(vals == NULL || vals->size() % 2 == 0); }

 protected:
  int
  do_traverse(Traverse* traverse);

  Type*
  do_type()
  { return this->type_; }

  void
  do_determine_type(const Type_context*);

  void
  do_check_types(Gogo*);

  Expression*
  do_copy()
  {
    return new Map_construction_expression(this->type_, this->vals_->copy(),
					   this->location());
  }

  // Should be turned into Heap_composite_expression.
  bool
  do_address_taken(source_location, bool)
  { gcc_unreachable(); }

  Expression*
  do_being_copied(Refcounts*, bool);

  Expression*
  do_note_decrements(Refcounts* refcounts);

  tree
  do_get_tree(Translate_context*);

  void
  do_export(Export*) const;

 private:
  // The type of the map to construct.
  Type* type_;
  // The list of values.
  Expression_list* vals_;
  // Whether the expression is being copied.
  bool is_being_copied_;
};

// Traversal.

int
Map_construction_expression::do_traverse(Traverse* traverse)
{
  if (this->vals_ != NULL
      && this->vals_->traverse(traverse) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  if (Type::traverse(this->type_, traverse) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  return TRAVERSE_CONTINUE;
}

// Final type determination.

void
Map_construction_expression::do_determine_type(const Type_context*)
{
  if (this->vals_ == NULL)
    return;

  Map_type* mt = this->type_->map_type();
  Type_context key_context(mt->key_type(), false);
  Type_context val_context(mt->val_type(), false);
  for (Expression_list::const_iterator pv = this->vals_->begin();
       pv != this->vals_->end();
       ++pv)
    {
      (*pv)->determine_type(&key_context);
      ++pv;
      (*pv)->determine_type(&val_context);
    }
}

// Check types.

void
Map_construction_expression::do_check_types(Gogo*)
{
  if (this->vals_ == NULL)
    return;

  Map_type* mt = this->type_->map_type();
  int i = 0;
  Type* key_type = mt->key_type();
  Type* val_type = mt->val_type();
  for (Expression_list::const_iterator pv = this->vals_->begin();
       pv != this->vals_->end();
       ++pv, ++i)
    {
      if (!Type::are_compatible_for_assign(key_type, (*pv)->type(), NULL))
	{
	  error_at((*pv)->location(),
		   "incompatible type for element %d key in map construction",
		   i + 1);
	  this->set_is_error();
	}
      ++pv;
      if (!Type::are_compatible_for_assign(val_type, (*pv)->type(), NULL))
	{
	  error_at((*pv)->location(),
		   ("incompatible type for element %d value "
		    "in map construction"),
		   i + 1);
	  this->set_is_error();
	}
    }
}

// Copying a map construction should not require any special action.

Expression*
Map_construction_expression::do_being_copied(Refcounts*, bool)
{
  this->is_being_copied_ = true;
  return this;
}

// Constructing a map always introduces a reference count.

Expression*
Map_construction_expression::do_note_decrements(Refcounts* refcounts)
{
  if (this->is_being_copied_)
    return this;
  return Expression::make_refcount_adjust(refcounts, REFCOUNT_DECREMENT_NEW,
					  this, false);
}

// Return a tree for constructing a map.

tree
Map_construction_expression::do_get_tree(Translate_context* context)
{
  Gogo* gogo = context->gogo();

  if (this->vals_ == NULL || this->vals_->empty())
    return this->type_->get_init_tree(gogo, false);

  Map_type* mt = this->type_->map_type();

  // Build a struct to hold the key and value.
  tree struct_type = make_node(RECORD_TYPE);

  tree id = get_identifier("__key");
  tree key_field = build_decl(this->location(), FIELD_DECL, id,
			      mt->key_type()->get_tree(gogo));
  DECL_CONTEXT(key_field) = struct_type;
  TYPE_FIELDS(struct_type) = key_field;

  id = get_identifier("__val");
  tree val_field = build_decl(this->location(), FIELD_DECL, id,
			      mt->val_type()->get_tree(gogo));
  DECL_CONTEXT(val_field) = struct_type;
  TREE_CHAIN(key_field) = val_field;

  layout_type(struct_type);

  VEC(constructor_elt,gc)* values = VEC_alloc(constructor_elt, gc,
					      this->vals_->size() / 2);

  bool is_constant = true;
  size_t i = 0;
  for (Expression_list::const_iterator pv = this->vals_->begin();
       pv != this->vals_->end();
       ++pv, ++i)
    {
      bool one_is_constant = true;

      VEC(constructor_elt,gc)* one = VEC_alloc(constructor_elt, gc, 2);

      constructor_elt* elt = VEC_quick_push(constructor_elt, one, NULL);
      elt->index = key_field;
      elt->value = (*pv)->get_tree(context);
      if (elt->value == error_mark_node)
	return error_mark_node;
      if (!TREE_CONSTANT(elt->value))
	one_is_constant = false;

      ++pv;

      elt = VEC_quick_push(constructor_elt, one, NULL);
      elt->index = val_field;
      elt->value = (*pv)->get_tree(context);
      if (elt->value == error_mark_node)
	return error_mark_node;
      if (!TREE_CONSTANT(elt->value))
	one_is_constant = false;

      elt = VEC_quick_push(constructor_elt, values, NULL);
      elt->index = size_int(i);
      elt->value = build_constructor(struct_type, one);
      if (one_is_constant)
	TREE_CONSTANT(elt->value) = 1;
      else
	is_constant = false;
    }

  tree index_type = build_index_type(size_int(i - 1));
  tree array_type = build_array_type(struct_type, index_type);
  tree init = build_constructor(array_type, values);
  if (is_constant)
    TREE_CONSTANT(init) = 1;
  tree tmp;
  tree make_tmp;
  if (current_function_decl != NULL)
    {
      tmp = create_tmp_var(array_type, get_name(array_type));
      DECL_INITIAL(tmp) = init;
      make_tmp = build1(DECL_EXPR, void_type_node, tmp);
      SET_EXPR_LOCATION(make_tmp, this->location());
      TREE_ADDRESSABLE(tmp) = 1;
    }
  else
    {
      tmp = build_decl(this->location(), VAR_DECL, create_tmp_var_name("M"),
		       array_type);
      DECL_EXTERNAL(tmp) = 0;
      TREE_PUBLIC(tmp) = 0;
      TREE_STATIC(tmp) = 1;
      DECL_ARTIFICIAL(tmp) = 1;
      if (!TREE_CONSTANT(init))
	{
	  make_tmp = build2(INIT_EXPR, void_type_node, tmp, init);
	  SET_EXPR_LOCATION(make_tmp, this->location());
	}
      else
	{
	  TREE_READONLY(tmp) = 1;
	  TREE_CONSTANT(tmp) = 1;
	  DECL_INITIAL(tmp) = init;
	  make_tmp = NULL_TREE;
	}
      rest_of_decl_compilation(tmp, 1, 0);
    }

  tree descriptor = gogo->map_descriptor(mt);

  tree type_tree = this->type_->get_tree(gogo);

  static tree construct_map_fndecl;
  tree call = Gogo::call_builtin(&construct_map_fndecl,
				 this->location(),
				 "__go_construct_map",
				 6,
				 type_tree,
				 TREE_TYPE(descriptor),
				 descriptor,
				 sizetype,
				 size_int(i),
				 sizetype,
				 TYPE_SIZE_UNIT(struct_type),
				 sizetype,
				 byte_position(val_field),
				 sizetype,
				 TYPE_SIZE_UNIT(TREE_TYPE(val_field)),
				 const_ptr_type_node,
				 fold_convert(const_ptr_type_node,
					      build_fold_addr_expr(tmp)));

  tree ret;
  if (make_tmp == NULL)
    ret = call;
  else
    ret = build2(COMPOUND_EXPR, type_tree, make_tmp, call);
  return ret;
}

// Export an array construction.

void
Map_construction_expression::do_export(Export* exp) const
{
  exp->write_c_string("convert(");
  exp->write_type(this->type_);
  for (Expression_list::const_iterator pv = this->vals_->begin();
       pv != this->vals_->end();
       ++pv)
    {
      exp->write_c_string(", ");
      (*pv)->export_expression(exp);
    }
  exp->write_c_string(")");
}

// A general composite literal.  This is lowered to a type specific
// version.

class Composite_literal_expression : public Parser_expression
{
 public:
  Composite_literal_expression(Type* type, bool has_keys,
			       Expression_list* vals, source_location location)
    : Parser_expression(EXPRESSION_COMPOSITE_LITERAL, location),
      type_(type), vals_(vals), has_keys_(has_keys)
  { }

 protected:
  int
  do_traverse(Traverse* traverse);

  Expression*
  do_lower(Gogo*, int);

  Expression*
  do_copy()
  {
    return new Composite_literal_expression(this->type_, this->has_keys_,
					    (this->vals_ == NULL
					     ? NULL
					     : this->vals_->copy()),
					    this->location());
  }

 private:
  Expression*
  lower_struct();

  Expression*
  lower_array();

  Expression*
  make_array(Expression_list*);

  Expression*
  lower_map();

  // The type of the composite literal.
  Type* type_;
  // The values to put in the composite literal.
  Expression_list* vals_;
  // If this is true, then VALS_ is a list of pairs: a key and a
  // value.  In an array initializer, a missing key will be NULL.
  bool has_keys_;
};

// Traversal.

int
Composite_literal_expression::do_traverse(Traverse* traverse)
{
  if (this->vals_ != NULL
      && this->vals_->traverse(traverse) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  return Type::traverse(this->type_, traverse);
}

// Lower a generic composite literal into a specific version based on
// the type.

Expression*
Composite_literal_expression::do_lower(Gogo*, int)
{
  Type* type = this->type_;
  if (type->is_error_type())
    return Expression::make_error(this->location());
  else if (type->struct_type() != NULL)
    return this->lower_struct();
  else if (type->array_type() != NULL)
    return this->lower_array();
  else if (type->map_type() != NULL)
    return this->lower_map();
  else
    {
      error_at(this->location(),
	       "expected struct, array, or map type for composite literal");
      return Expression::make_error(this->location());
    }
}

// Lower a struct composite literal.

Expression*
Composite_literal_expression::lower_struct()
{
  source_location location = this->location();
  Struct_type* st = this->type_->struct_type();
  if (this->vals_ == NULL || !this->has_keys_)
    return new Struct_construction_expression(this->type_, this->vals_,
					      location);

  size_t field_count = st->field_count();
  std::vector<Expression*> vals(field_count);
  Expression_list::const_iterator p = this->vals_->begin();
  while (p != this->vals_->end())
    {
      Expression* name_expr = *p;

      ++p;
      gcc_assert(p != this->vals_->end());
      Expression* val = *p;

      ++p;

      if (name_expr == NULL)
	{
	  error_at(val->location(), "mixture of field and value initializers");
	  return Expression::make_error(location);
	}

      bool bad_key = false;
      std::string name;
      switch (name_expr->classification())
	{
	case EXPRESSION_UNKNOWN_REFERENCE:
	  name = name_expr->unknown_expression()->name();
	  break;

	case EXPRESSION_CONST_REFERENCE:
	  name = static_cast<Const_expression*>(name_expr)->name();
	  break;

	case EXPRESSION_TYPE:
	  {
	    Type* t = name_expr->type();
	    Named_type* nt = t->named_type();
	    if (nt == NULL)
	      bad_key = true;
	    else
	      name = nt->name();
	  }
	  break;

	case EXPRESSION_VAR_REFERENCE:
	  name = name_expr->var_expression()->name();
	  break;

	case EXPRESSION_FUNC_REFERENCE:
	  name = name_expr->func_expression()->name();
	  break;

	case EXPRESSION_UNARY:
	  // If there is a local variable around with the same name as
	  // the field, and this occurs in the closure, then the
	  // parser may turn the field reference into an indirection
	  // through the closure.  FIXME: This is a mess.
	  {
	    bad_key = true;
	    Unary_expression* ue = static_cast<Unary_expression*>(name_expr);
	    if (ue->op() == OPERATOR_MULT)
	      {
		Field_reference_expression* fre =
		  ue->operand()->field_reference_expression();
		if (fre != NULL)
		  {
		    Struct_type* st =
		      fre->expr()->type()->deref()->struct_type();
		    if (st != NULL)
		      {
			const Struct_field* sf = st->field(fre->field_index());
			name = sf->field_name();
			char buf[20];
			snprintf(buf, sizeof buf, "%u", fre->field_index());
			size_t buflen = strlen(buf);
			if (name.compare(name.length() - buflen, buflen, buf)
			    == 0)
			  {
			    name = name.substr(0, name.length() - buflen);
			    bad_key = false;
			  }
		      }
		  }
	      }
	  }
	  break;

	default:
	  bad_key = true;
	  break;
	}
      if (bad_key)
	{
	  error_at(name_expr->location(), "expected struct field name");
	  return Expression::make_error(location);
	}

      unsigned int index;
      const Struct_field* sf = st->find_local_field(name, &index);
      if (sf == NULL)
	{
	  error_at(name_expr->location(), "no field %qs",
		   Gogo::unpack_hidden_name(name).c_str());
	  return Expression::make_error(location);
	}
      if (vals[index] != NULL)
	{
	  error_at(name_expr->location(), "duplicate value for field %qs",
		   Gogo::unpack_hidden_name(name).c_str());
	  return Expression::make_error(location);
	}

      vals[index] = val;
    }

  Expression_list* list = new Expression_list;
  list->reserve(field_count);
  for (size_t i = 0; i < field_count; ++i)
    list->push_back(vals[i]);

  return new Struct_construction_expression(this->type_, list, location);
}

// Lower an array composite literal.

Expression*
Composite_literal_expression::lower_array()
{
  source_location location = this->location();
  if (this->vals_ == NULL || !this->has_keys_)
    return this->make_array(this->vals_);

  std::vector<Expression*> vals;
  vals.reserve(this->vals_->size());
  unsigned long index = 0;
  Expression_list::const_iterator p = this->vals_->begin();
  while (p != this->vals_->end())
    {
      Expression* index_expr = *p;

      ++p;
      gcc_assert(p != this->vals_->end());
      Expression* val = *p;

      ++p;

      if (index_expr != NULL)
	{
	  mpz_t ival;
	  mpz_init(ival);
	  Type* dummy;
	  if (!index_expr->integer_constant_value(true, ival, &dummy))
	    {
	      mpz_clear(ival);
	      error_at(index_expr->location(),
		       "index expression is not integer constant");
	      return Expression::make_error(location);
	    }
	  if (mpz_sgn(ival) < 0)
	    {
	      mpz_clear(ival);
	      error_at(index_expr->location(), "index expression is negative");
	      return Expression::make_error(location);
	    }
	  index = mpz_get_ui(ival);
	  if (mpz_cmp_ui(ival, index) != 0)
	    {
	      mpz_clear(ival);
	      error_at(index_expr->location(), "index value overflow");
	      return Expression::make_error(location);
	    }
	  mpz_clear(ival);
	}

      if (index == vals.size())
	vals.push_back(val);
      else
	{
	  if (index > vals.size())
	    {
	      vals.reserve(index + 32);
	      vals.resize(index + 1, static_cast<Expression*>(NULL));
	    }
	  if (vals[index] != NULL)
	    {
	      error_at((index_expr != NULL
			? index_expr->location()
			: val->location()),
		       "duplicate value for index %lu",
		       index);
	      return Expression::make_error(location);
	    }
	  vals[index] = val;
	}

      ++index;
    }

  size_t size = vals.size();
  Expression_list* list = new Expression_list;
  list->reserve(size);
  for (size_t i = 0; i < size; ++i)
    list->push_back(vals[i]);

  return this->make_array(list);
}

// Actually build the array composite literal. This handles
// [...]{...}.

Expression*
Composite_literal_expression::make_array(Expression_list* vals)
{
  source_location location = this->location();
  Array_type* at = this->type_->array_type();
  if (at->length() != NULL && at->length()->is_nil_expression())
    {
      size_t size = vals == NULL ? 0 : vals->size();
      mpz_t vlen;
      mpz_init_set_ui(vlen, size);
      Expression* elen = Expression::make_integer(&vlen, NULL, location);
      mpz_clear(vlen);
      at = Type::make_array_type(at->element_type(), elen);
      this->type_ = at;
    }
  if (at->length() != NULL)
    return new Fixed_array_construction_expression(this->type_, vals, location);
  else
    return new Open_array_construction_expression(this->type_, vals, location);
}

// Lower a map composite literal.

Expression*
Composite_literal_expression::lower_map()
{
  source_location location = this->location();
  if (this->vals_ != NULL)
    {
      if (!this->has_keys_)
	{
	  error_at(location, "map composite literal must have keys");
	  return Expression::make_error(location);
	}

      for (Expression_list::const_iterator p = this->vals_->begin();
	   p != this->vals_->end();
	   p += 2)
	{
	  if (*p == NULL)
	    {
	      ++p;
	      error_at((*p)->location(),
		       "map composite literal must have keys for every value");
	      return Expression::make_error(location);
	    }
	}
    }

  return new Map_construction_expression(this->type_, this->vals_, location);
}

// Make a composite literal expression.

Expression*
Expression::make_composite_literal(Type* type, bool has_keys,
				   Expression_list* vals,
				   source_location location)
{
  return new Composite_literal_expression(type, has_keys, vals, location);
}

// Return whether this expression is a composite literal.

bool
Expression::is_composite_literal() const
{
  switch (this->classification_)
    {
    case EXPRESSION_COMPOSITE_LITERAL:
    case EXPRESSION_STRUCT_CONSTRUCTION:
    case EXPRESSION_FIXED_ARRAY_CONSTRUCTION:
    case EXPRESSION_OPEN_ARRAY_CONSTRUCTION:
    case EXPRESSION_MAP_CONSTRUCTION:
      return true;
    default:
      return false;
    }
}

// Return whether this expression is a composite literal which is not
// constant.

bool
Expression::is_nonconstant_composite_literal() const
{
  switch (this->classification_)
    {
    case EXPRESSION_STRUCT_CONSTRUCTION:
      {
	const Struct_construction_expression *psce =
	  static_cast<const Struct_construction_expression*>(this);
	return !psce->is_constant_struct();
      }
    case EXPRESSION_FIXED_ARRAY_CONSTRUCTION:
      {
	const Fixed_array_construction_expression *pace =
	  static_cast<const Fixed_array_construction_expression*>(this);
	return !pace->is_constant_array();
      }
    case EXPRESSION_OPEN_ARRAY_CONSTRUCTION:
      {
	const Open_array_construction_expression *pace =
	  static_cast<const Open_array_construction_expression*>(this);
	return !pace->is_constant_array();
      }
    case EXPRESSION_MAP_CONSTRUCTION:
      return true;
    default:
      return false;
    }
}

// Return true if this is a reference to a local variable.

bool
Expression::is_local_variable() const
{
  const Var_expression* ve = this->var_expression();
  if (ve == NULL)
    return false;
  const Named_object* no = ve->named_object();
  return (no->is_result_variable()
	  || (no->is_variable() && !no->var_value()->is_global()));
}

// Class Type_guard_expression.

// Traversal.

int
Type_guard_expression::do_traverse(Traverse* traverse)
{
  if (Expression::traverse(&this->expr_, traverse) == TRAVERSE_EXIT
      || Type::traverse(this->type_, traverse) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  return TRAVERSE_CONTINUE;
}

// Check types of a type guard expression.  The expression must have
// an interface type, but the actual type conversion is checked at run
// time.

void
Type_guard_expression::do_check_types(Gogo*)
{
  // 6g permits using a type guard with unsafe.pointer; we are
  // compatible.
  Type* expr_type = this->expr_->type();
  if (expr_type->is_unsafe_pointer_type())
    {
      if (this->type_->points_to() == NULL
	  && this->type_->integer_type() == NULL)
	this->report_error(_("invalid unsafe.pointer conversion"));
    }
  else if (this->type_->is_unsafe_pointer_type())
    {
      if (expr_type->interface_type() == NULL
	  && expr_type->points_to() == NULL
	  && expr_type->integer_type() == NULL)
	this->report_error(_("invalid unsafe.pointer conversion"));
    }
  else if (expr_type->interface_type() == NULL)
    this->report_error(_("type guard only valid for interface types"));
}

// If we are copying the type guard to a variable, we don't need to do
// anything; any reference count will already have been incremented.

Expression*
Type_guard_expression::do_being_copied(Refcounts*, bool)
{
  this->is_being_copied_ = true;
  return this;
}

// If we are converting to a reference counted type, then the
// conversion process will increment the reference count.

Expression*
Type_guard_expression::do_note_decrements(Refcounts* refcounts)
{
  if (!this->type_->has_refcounted_component() || this->is_being_copied_)
    return this;
  return Expression::make_refcount_adjust(refcounts,
					  REFCOUNT_DECREMENT_COMPUTED,
					  this, false);
}

// Return a tree for a type guard expression.

tree
Type_guard_expression::do_get_tree(Translate_context* context)
{
  Gogo* gogo = context->gogo();
  tree expr_tree = this->expr_->get_tree(context);
  if (expr_tree == error_mark_node)
    return error_mark_node;
  Type* expr_type = this->expr_->type();
  if ((this->type_->is_unsafe_pointer_type()
       && (expr_type->points_to() != NULL
	   || expr_type->integer_type() != NULL))
      || (expr_type->is_unsafe_pointer_type()
	  && this->type_->points_to() != NULL))
    return convert_to_pointer(this->type_->get_tree(gogo), expr_tree);
  else if (expr_type->is_unsafe_pointer_type()
	   && this->type_->integer_type() != NULL)
    return convert_to_integer(this->type_->get_tree(gogo), expr_tree);
  else
    return Expression::convert_for_assignment(context, this->type_,
					      this->expr_->type(), expr_tree,
					      this->location());
}

// Make a type guard expression.

Expression*
Expression::make_type_guard(Expression* expr, Type* type,
			    source_location location)
{
  return new Type_guard_expression(expr, type, location);
}

// Class Heap_composite_expression.

// When you take the address of a composite literal, it is allocated
// on the heap.  This class implements that.

class Heap_composite_expression : public Expression
{
 public:
  Heap_composite_expression(Expression* expr, bool for_go_statement,
			    source_location location)
    : Expression(EXPRESSION_HEAP_COMPOSITE, location),
      expr_(expr), for_go_statement_(for_go_statement), is_being_copied_(false)
  { }

 protected:
  int
  do_traverse(Traverse* traverse)
  { return Expression::traverse(&this->expr_, traverse); }

  Type*
  do_type()
  { return Type::make_pointer_type(this->expr_->type()); }

  void
  do_determine_type(const Type_context*)
  { this->expr_->determine_type_no_context(); }

  Expression*
  do_copy()
  {
    return Expression::make_heap_composite(this->expr_->copy(),
					   this->for_go_statement_,
					   this->location());
  }

  Expression*
  do_being_copied(Refcounts*, bool);

  Expression*
  do_note_decrements(Refcounts*);

  tree
  do_get_tree(Translate_context*);

  // We only export global objects, and the parser does not generate
  // this in global scope.
  void
  do_export(Export*) const
  { gcc_unreachable(); }

 private:
  // The composite literal which is being put on the heap.
  Expression* expr_;
  // True if this composite literal is being created for a go
  // statement.  This is used to control reference count adjustments.
  bool for_go_statement_;
  // Whether this expression is being copied.
  bool is_being_copied_;
};

// A heap composite is created with a new reference count, so nothing
// special need be done to copy it.  However, we do have to increment
// the reference counts of the composite literal of which it is a
// copy.

Expression*
Heap_composite_expression::do_being_copied(Refcounts* refcounts,
					   bool for_local)
{
  gcc_assert(!this->for_go_statement_);
  this->is_being_copied_ = true;
  this->expr_ = this->expr_->being_copied(refcounts, for_local);
  return this;
}

// Constructing a heap composite always introduces a reference count.
// If we are creating this for a go statement, then the reference
// count should be decremented in the thunk, not here.

Expression*
Heap_composite_expression::do_note_decrements(Refcounts* refcounts)
{
  if (this->for_go_statement_ || this->is_being_copied_)
    return this;
  return Expression::make_refcount_adjust(refcounts, REFCOUNT_DECREMENT_NEW,
					  this, false);
}

// Return a tree which allocates a composite literal on the heap.

tree
Heap_composite_expression::do_get_tree(Translate_context* context)
{
  tree expr_tree = this->expr_->get_tree(context);
  if (expr_tree == error_mark_node)
    return error_mark_node;
  tree expr_size = TYPE_SIZE_UNIT(TREE_TYPE(expr_tree));
  gcc_assert(TREE_CODE(expr_size) == INTEGER_CST);
  tree space = context->gogo()->allocate_memory(expr_size, this->location());
  space = fold_convert(build_pointer_type(TREE_TYPE(expr_tree)), space);
  space = save_expr(space);
  tree ret = build2(COMPOUND_EXPR, TREE_TYPE(space),
		    build2(MODIFY_EXPR, void_type_node,
			   build_fold_indirect_ref(space),
			   expr_tree),
		    space);
  SET_EXPR_LOCATION(ret, this->location());
  return ret;
}

// Allocate a composite literal on the heap.

Expression*
Expression::make_heap_composite(Expression* expr, bool for_go_statement,
				source_location location)
{
  return new Heap_composite_expression(expr, for_go_statement, location);
}

// Class Receive_expression.

// Return the type of a receive expression.

Type*
Receive_expression::do_type()
{
  Channel_type* channel_type = this->channel_->type()->channel_type();
  if (channel_type == NULL)
    return Type::make_error_type();
  return channel_type->element_type();
}

// Check types for a receive expression.

void
Receive_expression::do_check_types(Gogo*)
{
  Type* type = this->channel_->type();
  if (type->is_error_type())
    {
      this->set_is_error();
      return;
    }
  if (type->channel_type() == NULL)
    {
      this->report_error(_("expected channel"));
      return;
    }
  if (!type->channel_type()->may_receive())
    {
      this->report_error(_("invalid receive on send-only channel"));
      return;
    }
}

// If we are copying the received value to a variable, we don't need
// to do anything; any reference count will already have been
// incremented.

Expression*
Receive_expression::do_being_copied(Refcounts*, bool)
{
  this->is_being_copied_ = true;
  return this;
}

// The receive will come with a reference count which we need to
// decrement afterward.

Expression*
Receive_expression::do_note_decrements(Refcounts* refcounts)
{
  if (this->is_being_copied_)
    return this;
  Channel_type* channel_type = this->channel_->type()->channel_type();
  if (channel_type == NULL)
    return this;
  Type* element_type = channel_type->element_type();
  if (!element_type->has_refcounted_component())
    return this;
  return Expression::make_refcount_adjust(refcounts,
					  REFCOUNT_DECREMENT_COMPUTED,
					  this, false);
}

// Get a tree for a receive expression.

tree
Receive_expression::do_get_tree(Translate_context* context)
{
  Channel_type* channel_type = this->channel_->type()->channel_type();
  gcc_assert(channel_type != NULL);
  Type* element_type = channel_type->element_type();
  tree element_type_tree = element_type->get_tree(context->gogo());

  tree channel = this->channel_->get_tree(context);
  if (element_type_tree == error_mark_node || channel == error_mark_node)
    return error_mark_node;

  return Gogo::receive_from_channel(element_type_tree, channel, true,
				    false, this->location());
}

// Make a receive expression.

Expression*
Expression::make_receive(Expression* channel, source_location location)
{
  return new Receive_expression(channel, location);
}

// Class Send_expression.

class Send_expression : public Expression
{
 public:
  Send_expression(Expression* channel, Expression* val,
		  source_location location)
    : Expression(EXPRESSION_SEND, location),
      channel_(channel), val_(val), is_value_discarded_(false)
  { }

 protected:
  int
  do_traverse(Traverse* traverse);

  void
  do_discarding_value()
  { this->is_value_discarded_ = true; }

  Type*
  do_type()
  { return Type::lookup_bool_type(); }

  void
  do_determine_type(const Type_context*);

  void
  do_check_types(Gogo*);

  Expression*
  do_copy()
  {
    return Expression::make_send(this->channel_->copy(), this->val_->copy(),
				 this->location());
  }

  Expression*
  do_being_copied(Refcounts*, bool);

  Expression*
  do_note_decrements(Refcounts*);

  tree
  do_get_tree(Translate_context*);

 private:
  // The channel on which to send the value.
  Expression* channel_;
  // The value to send.
  Expression* val_;
  // Whether the value is being discarded.
  bool is_value_discarded_;
};

// Traversal.

int
Send_expression::do_traverse(Traverse* traverse)
{
  if (Expression::traverse(&this->channel_, traverse) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  return Expression::traverse(&this->val_, traverse);
}

// Set types.

void
Send_expression::do_determine_type(const Type_context*)
{
  this->channel_->determine_type_no_context();

  Type* type = this->channel_->type();
  Type_context subcontext;
  if (type->channel_type() != NULL)
    subcontext.type = type->channel_type()->element_type();
  this->val_->determine_type(&subcontext);
}

// Check types.

void
Send_expression::do_check_types(Gogo*)
{
  Type* type = this->channel_->type();
  if (type->is_error_type())
    {
      this->set_is_error();
      return;
    }
  Channel_type* channel_type = type->channel_type();
  if (channel_type == NULL)
    {
      this->report_error(_("left operand of %<<-%> must be channel"));
      return;
    }
  Type* element_type = channel_type->element_type();
  if (element_type != NULL
      && !Type::are_compatible_for_assign(element_type, this->val_->type(),
					  NULL))
    {
      this->report_error(_("incompatible types in send"));
      return;
    }
  if (!channel_type->may_send())
    {
      this->report_error(_("invalid send on receive-only channel"));
      return;
    }
}

// Called if the value is being copied.  There is nothing to do here.

Expression*
Send_expression::do_being_copied(Refcounts*, bool)
{
  return this;
}

// Reference count adjustments for a send expression.  The expression
// that we are sending is being copied.

Expression*
Send_expression::do_note_decrements(Refcounts* refcounts)
{
  this->val_ = this->val_->being_copied(refcounts, false);
  return this;
}

// Get a tree for a send expression.

tree
Send_expression::do_get_tree(Translate_context* context)
{
  tree channel = this->channel_->get_tree(context);
  tree val = this->val_->get_tree(context);
  if (channel == error_mark_node || val == error_mark_node)
    return error_mark_node;
  Channel_type* channel_type = this->channel_->type()->channel_type();
  val = Expression::convert_for_assignment(context,
					   channel_type->element_type(),
					   this->val_->type(),
					   val,
					   this->location());
  return Gogo::send_on_channel(channel, val, this->is_value_discarded_,
			       false, this->location());
}

// Make a send expression

Expression*
Expression::make_send(Expression* channel, Expression* val,
		      source_location location)
{
  return new Send_expression(channel, val, location);
}

// Class Refcount_adjust_expression.

// The type of adjustment.

int
Refcount_adjust_expression::classification() const
{
  return this->refcount_entry_->classification();
}

// Types should already have been determined and checked.

void
Refcount_adjust_expression::do_determine_type(const Type_context*)
{
  gcc_unreachable();
}

void
Refcount_adjust_expression::do_check_types(Gogo*)
{
  gcc_unreachable();
}

// Return a tree for the reference count adjustment.

tree
Refcount_adjust_expression::do_get_tree(Translate_context* context)
{
  // If we are incrementing a decrement, then skip both adjustments.
  if (this->expr_->classification() == EXPRESSION_REFCOUNT_ADJUST
      && this->classification() == REFCOUNT_INCREMENT_COPIED)
    {
      Refcount_adjust_expression* rae =
	static_cast<Refcount_adjust_expression*>(this->expr_);
      int rae_cl = rae->classification();
      // We should never see an increment of an old value; that makes
      // no sense.
      if (rae_cl == REFCOUNT_DECREMENT_NEW
	  || rae_cl == REFCOUNT_DECREMENT_COMPUTED)
	return rae->expr()->get_tree(context);
    }

  tree expr_tree = this->expr_->get_tree(context);
  if (expr_tree == error_mark_node)
    return error_mark_node;
  if (DECL_P(expr_tree)
      || (TREE_CODE(expr_tree) == ADDR_EXPR
	  && DECL_P(TREE_OPERAND(expr_tree, 0))))
    {
      // Because of the limited context, we don't need to save V or
      // &V.
    }
  else if (this->expr_->type()->array_type() != NULL
	   && !this->expr_->type()->is_open_array_type())
    {
      // We are just going to take the address of a fixed array, don't
      // try to save it.  Saving it fails because it requires an array
      // copy to the temporary variable.
    }
  else
    expr_tree = save_expr(expr_tree);
  Refcounts* refcounts = context->function()->func_value()->refcounts();
  Refcount_entry re = *this->refcount_entry_;
  tree set = this->expr_->type()->set_refcount_queue_entry(context->gogo(),
							   refcounts,
							   &re,
							   expr_tree);
  return build2(COMPOUND_EXPR, TREE_TYPE(expr_tree), set, expr_tree);
}

// Make a reference count adjustment.

Expression*
Expression::make_refcount_adjust(Refcounts* refcounts, int classification,
				 Expression* expr, bool for_local)
{
  // Simplify adjusting the reference count of a reference count
  // adjustment.
  if (expr->classification() == EXPRESSION_REFCOUNT_ADJUST)
    {
      Refcount_adjust_expression* rae =
	static_cast<Refcount_adjust_expression*>(expr);
      int rae_cl = rae->classification();

      // If we are incrementing a decrement, then we can discard both
      // the increment and the decrement.
      if (classification == REFCOUNT_INCREMENT_COPIED)
	{
	  // We should never see an increment of an old value; that
	  // makes no sense.
	  gcc_assert(rae_cl != REFCOUNT_DECREMENT_OLD);
	  if (rae_cl == REFCOUNT_DECREMENT_NEW
	      || rae_cl == REFCOUNT_DECREMENT_COMPUTED)
	    {
	      // FIXME: We don't reclaim the Refcount_entry.
	      return rae->expr();
	    }
	}

      // If we are decrementing an increment, then we can not in
      // general discard both adjustments.  The count might be
      // incremented in order to pass the value to a function.  If we
      // discard both adjustments, then the object may be freed during
      // the function call.
    }

  Refcount_entry entry = refcounts->add(classification, expr->type());
  return new Refcount_adjust_expression(new Refcount_entry(entry),
					expr, for_local);
}

// Class Refcount_decrement_lvalue_expression

// Constructor.

Refcount_decrement_lvalue_expression::Refcount_decrement_lvalue_expression(
    Refcount_entry* entry,
    Expression* expr)
  : Expression(EXPRESSION_REFCOUNT_DECREMENT_LVALUE, expr->location()),
    refcount_entry_(entry), expr_(expr)
{
  gcc_assert(expr->is_lvalue());
}


// Traversal.

int
Refcount_decrement_lvalue_expression::do_traverse(Traverse* traverse)
{
  return Expression::traverse(&this->expr_, traverse);
}

// Types should already have been determined and checked at this
// point.

void
Refcount_decrement_lvalue_expression::do_determine_type(const Type_context*)
{
  gcc_unreachable();
}

void
Refcount_decrement_lvalue_expression::do_check_types(Gogo*)
{
  gcc_unreachable();
}

// The normal tree expansion is simply the one for THIS->EXPR_.  We
// require that the user call the set function appropriately.

tree
Refcount_decrement_lvalue_expression::do_get_tree(Translate_context* context)
{
  return this->expr_->get_tree(context);
}

// Set the expression to VAL.

tree
Refcount_decrement_lvalue_expression::set(Translate_context* context,
					  tree lhs_tree, tree rhs_tree)
{
  if (lhs_tree == error_mark_node || rhs_tree == error_mark_node)
    return error_mark_node;
  lhs_tree = stabilize_reference(lhs_tree);

  // FIXME: If we are changing a value which may be visible to
  // multiple goroutines--i.e., anything other than a local
  // variable--then we should use an atomic swap.  Otherwise we can
  // get inconsistent reference counts.

  Refcounts* refcounts = context->function()->func_value()->refcounts();
  Refcount_entry re = *this->refcount_entry_;
  tree save = this->expr_->type()->set_refcount_queue_entry(context->gogo(),
							    refcounts,
							    &re,
							    lhs_tree);
  tree set = fold_build2(MODIFY_EXPR, void_type_node, lhs_tree, rhs_tree);
  return fold_build2(COMPOUND_EXPR, void_type_node, save, set);
}

// Make a reference count decrement of an lvalue.

Expression*
Expression::make_refcount_decrement_lvalue(Refcounts* refcounts,
					   Expression* expr)
{
  Refcount_entry entry = refcounts->add(REFCOUNT_DECREMENT_OLD, expr->type());
  return new Refcount_decrement_lvalue_expression(new Refcount_entry(entry),
						  expr);
}

// Import an expression.  This comes at the end in order to see the
// various class definitions.

Expression*
Expression::import_expression(Import* imp)
{
  int c = imp->peek_char();
  if (imp->match_c_string("- ")
      || imp->match_c_string("! ")
      || imp->match_c_string("^ "))
    return Unary_expression::do_import(imp);
  else if (c == '(')
    return Binary_expression::do_import(imp);
  else if (imp->match_c_string("true")
	   || imp->match_c_string("false"))
    return Boolean_expression::do_import(imp);
  else if (c == '"')
    return String_expression::do_import(imp);
  else if (c == '-' || (c >= '0' && c <= '9'))
    {
      // This handles both integers and floats.
      return Integer_expression::do_import(imp);
    }
  else if (imp->match_c_string("nil"))
    return Nil_expression::do_import(imp);
  else if (imp->match_c_string("convert"))
    return Type_conversion_expression::do_import(imp);
  else
    {
      error_at(imp->location(), "import error: expected expression");
      return Expression::make_error(imp->location());
    }
}

// Class Expression_list.

// Traverse the list.

int
Expression_list::traverse(Traverse* traverse)
{
  for (Expression_list::iterator p = this->begin();
       p != this->end();
       ++p)
    {
      if (*p != NULL)
	{
	  if (Expression::traverse(&*p, traverse) == TRAVERSE_EXIT)
	    return TRAVERSE_EXIT;
	}
    }
  return TRAVERSE_CONTINUE;
}

// Copy the list.

Expression_list*
Expression_list::copy()
{
  Expression_list* ret = new Expression_list();
  for (Expression_list::iterator p = this->begin();
       p != this->end();
       ++p)
    {
      if (*p == NULL)
	ret->push_back(NULL);
      else
	ret->push_back((*p)->copy());
    }
  return ret;
}

// Return whether an expression list has an error expression.

bool
Expression_list::contains_error() const
{
  for (Expression_list::const_iterator p = this->begin();
       p != this->end();
       ++p)
    if (*p != NULL && (*p)->is_error_expression())
      return true;
  return false;
}
