// statements.cc -- Go frontend statements.

// Copyright 2009 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "go-system.h"

#include <tr1/unordered_set>
#include <gmp.h>

extern "C"
{
#include "intl.h"
#include "tree.h"
#include "gimple.h"
#include "convert.h"
#include "tree-iterator.h"
#include "real.h"
}

#include "go-c.h"
#include "types.h"
#include "expressions.h"
#include "gogo.h"
#include "refcount.h"
#include "statements.h"

// Class Statement.

Statement::Statement(Statement_classification classification,
		     source_location location)
  : classification_(classification), location_(location)
{
}

Statement::~Statement()
{
}

// Traverse the tree.  The work of walking the components is handled
// by the subclasses.

int
Statement::traverse(Block* block, size_t* pindex, Traverse* traverse)
{
  if (this->classification_ == STATEMENT_ERROR)
    return TRAVERSE_CONTINUE;

  unsigned int traverse_mask = traverse->traverse_mask();

  if ((traverse_mask & Traverse::traverse_statements) != 0)
    {
      int t = traverse->statement(block, pindex, this);
      if (t == TRAVERSE_EXIT)
	return TRAVERSE_EXIT;
      else if (t == TRAVERSE_SKIP_COMPONENTS)
	return TRAVERSE_CONTINUE;
    }

  // No point in checking traverse_mask here--a statement may contain
  // other blocks or statements, and if we got here we always want to
  // walk them.
  return this->do_traverse(traverse);
}

// Traverse assignments.

bool
Statement::traverse_assignments(Traverse_assignments* tassign)
{
  if (this->classification_ == STATEMENT_ERROR)
    return false;
  return this->do_traverse_assignments(tassign);
}

// Traverse an expression in a statement.  This is a helper function
// for child classes.

int
Statement::traverse_expression(Traverse* traverse, Expression** expr)
{
  if ((traverse->traverse_mask()
       & (Traverse::traverse_types | Traverse::traverse_expressions)) == 0)
    return TRAVERSE_CONTINUE;
  return Expression::traverse(expr, traverse);
}

// Traverse an expression list in a statement.  This is a helper
// function for child classes.

int
Statement::traverse_expression_list(Traverse* traverse,
				    Expression_list* expr_list)
{
  if (expr_list == NULL)
    return TRAVERSE_CONTINUE;
  if ((traverse->traverse_mask() & Traverse::traverse_expressions) == 0)
    return TRAVERSE_CONTINUE;
  return expr_list->traverse(traverse);
}

// Traverse a type in a statement.  This is a helper function for
// child classes.

int
Statement::traverse_type(Traverse* traverse, Type* type)
{
  if ((traverse->traverse_mask()
       & (Traverse::traverse_types | Traverse::traverse_expressions)) == 0)
    return TRAVERSE_CONTINUE;
  return Type::traverse(type, traverse);
}

// Set type information for unnamed constants.  This is really done by
// the child class.

void
Statement::determine_types()
{
  this->do_determine_types();
}

// If this is a thunk statement, return it.

Thunk_statement*
Statement::thunk_statement()
{
  Thunk_statement* ret = this->convert<Thunk_statement, STATEMENT_GO>();
  if (ret == NULL)
    ret = this->convert<Thunk_statement, STATEMENT_DEFER>();
  return ret;
}

// Get a tree for a Statement.  This is really done by the child
// class.

tree
Statement::get_tree(Translate_context* context)
{
  if (this->classification_ == STATEMENT_ERROR)
    return error_mark_node;

  return this->do_get_tree(context);
}

// Build tree nodes and set locations.

tree
Statement::build_stmt_1(int tree_code_value, tree node)
{
  tree ret = build1(static_cast<tree_code>(tree_code_value),
		    void_type_node, node);
  SET_EXPR_LOCATION(ret, this->location_);
  return ret;
}

// Note that this statement is erroneous.  This is called by children
// when they discover an error.

void
Statement::set_is_error()
{
  this->classification_ = STATEMENT_ERROR;
}

// For children to call to report an error conveniently.

void
Statement::report_error(const char* msg)
{
  error_at(this->location_, "%s", msg);
  this->set_is_error();
}

// An error statement, used to avoid crashing after we report an
// error.

class Error_statement : public Statement
{
 public:
  Error_statement(source_location location)
    : Statement(STATEMENT_ERROR, location)
  { }

 protected:
  int
  do_traverse(Traverse*)
  { return TRAVERSE_CONTINUE; }

  tree
  do_get_tree(Translate_context*)
  { gcc_unreachable(); }
};

// Make an error statement.

Statement*
Statement::make_error_statement(source_location location)
{
  return new Error_statement(location);
}

// A variable declaration.  This exists because the middle-end wants
// to see a DECL_EXPR.

class Variable_declaration_statement : public Statement
{
 public:
  Variable_declaration_statement(Named_object* var)
    : Statement(STATEMENT_VARIABLE_DECLARATION, var->var_value()->location()),
      var_(var)
  { }

 protected:
  int
  do_traverse(Traverse*)
  { return TRAVERSE_CONTINUE; }

  bool
  do_traverse_assignments(Traverse_assignments*);

  tree
  do_get_tree(Translate_context*);

 private:
  Named_object* var_;
};

// Traverse the assignments in a variable declaration.  Note that this
// traversal is different from the usual traversal.

bool
Variable_declaration_statement::do_traverse_assignments(
    Traverse_assignments* tassign)
{
  tassign->initialize_variable(this->var_);
  return true;
}

// Return the tree for a variable declaration.

tree
Variable_declaration_statement::do_get_tree(Translate_context* context)
{
  tree val = this->var_->get_tree(context->gogo(), context->function());
  if (val == error_mark_node || TREE_TYPE(val) == error_mark_node)
    return error_mark_node;
  Variable* variable = this->var_->var_value();

  tree init = variable->get_init_tree(context->gogo(), context->function());
  if (init == error_mark_node)
    return error_mark_node;

  // If this variable lives on the heap, we need to allocate it now.
  if (!variable->is_in_heap())
    {
      DECL_INITIAL(val) = init;
      return this->build_stmt_1(DECL_EXPR, val);
    }
  else
    {
      gcc_assert(TREE_CODE(val) == INDIRECT_REF);
      tree decl = TREE_OPERAND(val, 0);
      gcc_assert(TREE_CODE(decl) == VAR_DECL);
      tree type = TREE_TYPE(decl);
      gcc_assert(POINTER_TYPE_P(type));
      tree size = TYPE_SIZE_UNIT(TREE_TYPE(type));
      tree space = context->gogo()->allocate_memory(size, this->location());
      space = fold_convert(TREE_TYPE(decl), space);
      DECL_INITIAL(decl) = space;
      return build2(COMPOUND_EXPR, void_type_node,
		    this->build_stmt_1(DECL_EXPR, decl),
		    build2(MODIFY_EXPR, void_type_node, val, init));
    }
}

// Make a variable declaration.

Statement*
Statement::make_variable_declaration(Named_object* var)
{
  return new Variable_declaration_statement(var);
}

// An assignment statement.

class Assignment_statement : public Statement
{
 public:
  Assignment_statement(Operator op, Expression* lhs, Expression* rhs,
		       source_location location)
    : Statement(STATEMENT_ASSIGNMENT, location),
      op_(op), lhs_(lhs), rhs_(rhs)
  { }

  // Return the tree code for a combining assignment such as +=.
  static tree_code
  combine_code(Operator op);

  // Return a tree for assigning RHS to LHS.
  static tree
  get_assignment_tree(Translate_context*, Operator, Expression* lhs, tree,
		      Expression* rhs, Type* rhs_type, tree, source_location);

 protected:
  int
  do_traverse(Traverse* traverse);

  bool
  do_traverse_assignments(Traverse_assignments*);

  void
  do_determine_types();

  void
  do_check_types(Gogo*);

  tree
  do_get_tree(Translate_context*);

 private:
  // Operator.  This is normally OPERATOR_EQ, but may OPERATOR_PLUSEQ,
  // etc.
  Operator op_;
  // Left hand side--the lvalue.
  Expression* lhs_;
  // Right hand side--the rvalue.
  Expression* rhs_;
};

// Traversal.

int
Assignment_statement::do_traverse(Traverse* traverse)
{
  if (this->traverse_expression(traverse, &this->lhs_) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  return this->traverse_expression(traverse, &this->rhs_);
}

bool
Assignment_statement::do_traverse_assignments(Traverse_assignments* tassign)
{
  tassign->assignment(&this->lhs_, &this->rhs_);
  return true;
}

// Set types for the assignment.

void
Assignment_statement::do_determine_types()
{
  this->lhs_->determine_type_no_context();
  if (this->op_ != OPERATOR_LSHIFTEQ && this->op_ != OPERATOR_RSHIFTEQ)
    {
      Type_context context(this->lhs_->type(), false);
      this->rhs_->determine_type(&context);
    }
  else
    {
      Type_context context(NULL, true);
      this->rhs_->determine_type(&context);
    }
}

// Check types for an assignment.

void
Assignment_statement::do_check_types(Gogo*)
{
  if (!this->lhs_->is_lvalue())
    {
      this->report_error(_("invalid left hand side of assignment"));
      return;
    }

  Type* lhs_type = this->lhs_->type();
  Type* rhs_type = this->rhs_->type();
  if (this->op_ != OPERATOR_LSHIFTEQ && this->op_ != OPERATOR_RSHIFTEQ)
    {
      std::string reason;
      if (!Type::are_compatible_for_assign(lhs_type, rhs_type, &reason))
	{
	  if (reason.empty())
	    error_at(this->location(), "incompatible types in assignment");
	  else
	    error_at(this->location(), "incompatible types in assignment (%s)",
		     reason.c_str());
	  this->set_is_error();
	}
      else if (this->op_ != OPERATOR_EQ
	       && !lhs_type->is_error_type()
	       && !rhs_type->is_error_type())
	{
	  if (!Type::are_compatible_for_binop(lhs_type, rhs_type))
	    this->report_error(_("incompatible types in binary expression"));
	  else if (!Binary_expression::check_operator_type(this->op_, lhs_type,
							   this->location()))
	    this->set_is_error();
	}
    }
  else
    {
      if (lhs_type->integer_type() == NULL && !lhs_type->is_error_type())
	this->report_error(_("shift of non-integer operand"));
      else
	{
	  if (!rhs_type->is_error_type()
	      && !rhs_type->is_abstract()
	      && (rhs_type->integer_type() == NULL
		  || !rhs_type->integer_type()->is_unsigned()))
	    this->report_error(_("shift count not unsigned integer"));
	}
    }

  if (lhs_type->is_error_type()
      || rhs_type->is_error_type()
      || lhs_type->is_undefined()
      || rhs_type->is_undefined())
    {
      // Make sure we get the error for an undefined type.
      lhs_type->base();
      rhs_type->base();
      this->set_is_error();
    }
}

// Return the tree code for a combining assignment code.

tree_code
Assignment_statement::combine_code(Operator op)
{
  switch (op)
    {
    case OPERATOR_PLUSEQ:
      return PLUS_EXPR;
    case OPERATOR_MINUSEQ:
      return MINUS_EXPR;
    case OPERATOR_OREQ:
      return BIT_IOR_EXPR;
    case OPERATOR_XOREQ:
      return BIT_XOR_EXPR;
    case OPERATOR_MULTEQ:
      return MULT_EXPR;
    case OPERATOR_DIVEQ:
      return TRUNC_DIV_EXPR;
    case OPERATOR_MODEQ:
      return TRUNC_MOD_EXPR;
    case OPERATOR_LSHIFTEQ:
      return LSHIFT_EXPR;
    case OPERATOR_RSHIFTEQ:
      return RSHIFT_EXPR;
    case OPERATOR_ANDEQ:
      return BIT_AND_EXPR;
    default:
      gcc_unreachable();
    }
}

// Return a tree which implements an assignment from RHS to LHS.  OP
// is the assignment operator--typically OPERATOR_EQ, but it can be
// OPERATOR_PLUSEQ, etc.  LHS_TREE is the tree to use for the left
// hand side; it may be NULL.  LHS may not be NULL.  RHS_TREE is the
// tree to use for the right hand side; it may be NULL.  RHS may be
// NULL, but at least one of RHS and RHS_TREE must not be NULL.  If
// LHS_TREE is not NULL then LHS may not be the right location, but it
// will have the right Type.  RHS_TYPE, if not NULL, is the Type of
// RHS/RHS_TREE; if RHS is NULL, RHS_TYPE must not be NULL.

tree
Assignment_statement::get_assignment_tree(Translate_context* context,
					  Operator op,
					  Expression* lhs,
					  tree lhs_tree,
					  Expression* rhs,
					  Type* rhs_type,
					  tree rhs_tree,
					  source_location location)
{
  gcc_assert(lhs != NULL && (rhs != NULL || rhs_type != NULL));

  if (rhs_tree == NULL_TREE)
    rhs_tree = rhs->get_tree(context);

  if (lhs->is_sink_expression())
    return rhs_tree;

  bool computed_lhs_tree;
  if (lhs_tree != NULL_TREE)
    computed_lhs_tree = false;
  else
    {
      lhs_tree = lhs->get_tree(context);
      computed_lhs_tree = true;
    }

  if (lhs_tree == error_mark_node || rhs_tree == error_mark_node)
    return error_mark_node;

  if (rhs_type == NULL)
    rhs_type = rhs->type();

  if (op != OPERATOR_EQ)
    {
      lhs_tree = stabilize_reference(lhs_tree);
      if (lhs->type()->is_string_type())
	{
	  gcc_assert(TYPE_CANONICAL(TREE_TYPE(lhs_tree))
		     == TYPE_CANONICAL(TREE_TYPE(rhs_tree)));
	  // Implement string append.  FIXME: This is also in
	  // Binary_expression::get_string_plus_tree.
	  static tree string_plus_decl;
	  rhs_tree = Gogo::call_builtin(&string_plus_decl,
					location,
					"__go_string_plus",
					2,
					TREE_TYPE(lhs_tree),
					TREE_TYPE(lhs_tree),
					lhs_tree,
					TREE_TYPE(lhs_tree),
					rhs_tree);
	}
      else if (op == OPERATOR_BITCLEAREQ)
	{
	  gcc_assert(TYPE_MAIN_VARIANT(TREE_TYPE(lhs_tree))
		     == TYPE_MAIN_VARIANT(TREE_TYPE(rhs_tree)));
	  rhs_tree = fold_build2_loc(location, BIT_AND_EXPR,
				     TREE_TYPE(lhs_tree), lhs_tree,
				     fold_build1(BIT_NOT_EXPR,
						 TREE_TYPE(rhs_tree),
						 rhs_tree));
	}
      else
	{
	  tree expr_type = TREE_TYPE(lhs_tree);
	  gcc_assert(op == OPERATOR_LSHIFTEQ
		     || op == OPERATOR_RSHIFTEQ
		     || (TYPE_MAIN_VARIANT(expr_type)
			 == TYPE_MAIN_VARIANT(TREE_TYPE(rhs_tree))));
	  tree compute_type = excess_precision_type(expr_type);
	  tree left = lhs_tree;
	  tree right = rhs_tree;
	  if (compute_type != NULL_TREE)
	    {
	      left = convert_to_real(compute_type, lhs_tree);
	      right = convert_to_real(compute_type, rhs_tree);
	    }
	  rhs_tree = fold_build2_loc(location,
				     Assignment_statement::combine_code(op),
				     (compute_type != NULL_TREE
				      ? compute_type
				      : expr_type),
				     left, right);
	  if (compute_type != NULL_TREE)
	    rhs_tree = convert_to_real(expr_type, rhs_tree);
	}
    }

  rhs_tree = Expression::convert_for_assignment(context, lhs->type(),
						rhs_type, rhs_tree,
						location);

  Refcount_decrement_lvalue_expression* rdle =
    lhs->refcount_decrement_lvalue_expression();
  tree ret;
  if (!computed_lhs_tree || rdle == NULL)
    ret = fold_build2(MODIFY_EXPR, void_type_node, lhs_tree, rhs_tree);
  else
    ret = rdle->set(context, lhs_tree, rhs_tree);

  if (CAN_HAVE_LOCATION_P(ret))
    SET_EXPR_LOCATION(ret, location);

  return ret;
}

// Build a tree for an assignment statement.

tree
Assignment_statement::do_get_tree(Translate_context* context)
{
  return Assignment_statement::get_assignment_tree(context, this->op_,
						   this->lhs_, NULL_TREE,
						   this->rhs_, NULL, NULL_TREE,
						   this->location());
}

// Make an assignment statement.

Statement*
Statement::make_assignment(Operator op, Expression* lhs, Expression* rhs,
			   source_location location)
{
  return new Assignment_statement(op, lhs, rhs, location);
}

// A tuple assignment statement.  This differs from an assignment
// statement in that the right-hand-side expressions are evaluated in
// parallel.

class Tuple_assignment_statement : public Statement
{
 public:
  Tuple_assignment_statement(Operator op, Expression_list* lhs,
			     Expression_list* rhs, source_location location)
    : Statement(STATEMENT_TUPLE_ASSIGNMENT, location),
      op_(op), lhs_(lhs), rhs_(rhs)
  { }

 protected:
  int
  do_traverse(Traverse* traverse);

  bool
  do_traverse_assignments(Traverse_assignments*);

  void
  do_determine_types();

  void
  do_check_types(Gogo*);

  tree
  do_get_tree(Translate_context* context);

 private:
  // Operator.
  Operator op_;
  // Left hand side--a list of lvalues.
  Expression_list* lhs_;
  // Right hand side--a list of rvalues.
  Expression_list* rhs_;
};

// Traversal.

int
Tuple_assignment_statement::do_traverse(Traverse* traverse)
{
  if (this->traverse_expression_list(traverse, this->lhs_) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  return this->traverse_expression_list(traverse, this->rhs_);
}

bool
Tuple_assignment_statement::do_traverse_assignments(
    Traverse_assignments* tassign)
{
  Expression_list::iterator prhs = this->rhs_->begin();
  for (Expression_list::iterator plhs = this->lhs_->begin();
       plhs != this->lhs_->end();
       ++plhs, ++prhs)
    {
      gcc_assert(prhs != this->rhs_->end());
      tassign->assignment(&*plhs, &*prhs);
    }
  gcc_assert(prhs == this->rhs_->end());
  return true;
}

// Set types if necessary.

void
Tuple_assignment_statement::do_determine_types()
{
  Expression_list::const_iterator prhs = this->rhs_->begin();
  for (Expression_list::const_iterator plhs = this->lhs_->begin();
       plhs != this->lhs_->end();
       ++plhs, ++prhs)
    {
      gcc_assert(prhs != this->rhs_->end());
      (*plhs)->determine_type_no_context();
      Type_context context((*plhs)->type(), false);
      (*prhs)->determine_type(&context);
    }
  gcc_assert(prhs == this->rhs_->end());
}

// Check types.

void
Tuple_assignment_statement::do_check_types(Gogo*)
{
  int index = 1;
  Expression_list::const_iterator prhs = this->rhs_->begin();
  for (Expression_list::const_iterator plhs = this->lhs_->begin();
       plhs != this->lhs_->end();
       ++plhs, ++prhs, ++index)
    {
      gcc_assert(prhs != this->rhs_->end());
      std::string reason;
      if (!(*plhs)->is_lvalue())
	this->report_error(_("invalid left hand side of assignment"));
      else if (!Type::are_compatible_for_assign((*plhs)->type(),
						(*prhs)->type(),
						&reason))
	{
	  if (reason.empty())
	    error_at(this->location(), "incompatible types in assignment %d",
		     index);
	  else
	    error_at(this->location(),
		     "incompatible types in assignment %d (%s)",
		     index, reason.c_str());
	  this->set_is_error();
	}
    }
  gcc_assert(prhs == this->rhs_->end());
}

// Build a tree for a tuple assignment.  We compute all the right hand
// side values into temporary variables, and then assign to temporary
// variables to the left hand side expressions.

tree
Tuple_assignment_statement::do_get_tree(Translate_context* context)
{
  Gogo* gogo = context->gogo();

  tree stmt_list = NULL_TREE;
  std::vector<tree> assignments;
  assignments.reserve(this->lhs_->size());

  source_location location = this->location();

  Expression_list::const_iterator prhs = this->rhs_->begin();
  for (Expression_list::const_iterator plhs = this->lhs_->begin();
       plhs != this->lhs_->end();
       ++plhs, ++prhs)
    {
      gcc_assert(prhs != this->rhs_->end());

      Type* type = ((*plhs)->is_sink_expression()
		    ? (*prhs)->type()
		    : (*plhs)->type());
      tree tmpvar = create_tmp_var_raw(type->get_tree(gogo), NULL);
      DECL_SOURCE_LOCATION(tmpvar) = location;
      append_to_statement_list(this->build_stmt_1(DECL_EXPR, tmpvar),
			       &stmt_list);

      tree assign = Assignment_statement::get_assignment_tree(context,
							      this->op_,
							      *plhs, tmpvar,
							      *prhs, NULL,
							      NULL_TREE,
							      location);
      if (assign == error_mark_node)
	continue;

      append_to_statement_list(assign, &stmt_list);

      if ((*plhs)->is_sink_expression())
	continue;

      tree lhs_tree = (*plhs)->get_tree(context);
      if (lhs_tree == error_mark_node)
	continue;

      Refcount_decrement_lvalue_expression* rdle =
	(*plhs)->refcount_decrement_lvalue_expression();
      tree set;
      if (rdle == NULL)
	set = fold_build2(MODIFY_EXPR, void_type_node, lhs_tree, tmpvar);
      else
	set = rdle->set(context, lhs_tree, tmpvar);

      if (CAN_HAVE_LOCATION_P(set))
	SET_EXPR_LOCATION(set, location);

      assignments.push_back(set);
    }
  gcc_assert(prhs == this->rhs_->end());

  for (std::vector<tree>::const_iterator p = assignments.begin();
       p != assignments.end();
       ++p)
    append_to_statement_list(*p, &stmt_list);

  return stmt_list;
}

// Make a tuple assignment statement.

Statement*
Statement::make_tuple_assignment(Operator op, Expression_list* lhs,
				 Expression_list* rhs, source_location location)
{
  return new Tuple_assignment_statement(op, lhs, rhs, location);
}

// A tuple assignment from a map index expression.

class Tuple_map_assignment_statement : public Statement
{
public:
  Tuple_map_assignment_statement(Expression* val, Expression* present,
				 Expression* map_index,
				 source_location location)
    : Statement(STATEMENT_TUPLE_MAP_ASSIGNMENT, location),
      val_(val), present_(present), map_index_(map_index)
  { }

 protected:
  int
  do_traverse(Traverse* traverse);

  bool
  do_traverse_assignments(Traverse_assignments*);

  void
  do_determine_types();

  void
  do_check_types(Gogo*);

  tree
  do_get_tree(Translate_context*);

 private:
  // Lvalue which receives the value from the map.
  Expression* val_;
  // Lvalue which receives whether the key value was present.
  Expression* present_;
  // The map index expression.
  Expression* map_index_;
};

// Traversal.

int
Tuple_map_assignment_statement::do_traverse(Traverse* traverse)
{
  if (this->traverse_expression(traverse, &this->val_) == TRAVERSE_EXIT
      || this->traverse_expression(traverse, &this->present_) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  return this->traverse_expression(traverse, &this->map_index_);
}

// Traverse assignments.

bool
Tuple_map_assignment_statement::do_traverse_assignments(
    Traverse_assignments* tassign)
{
  tassign->assignment(&this->val_, &this->map_index_);
  tassign->assignment(&this->present_, NULL);
  return true;
}

// Set types if necessary.

void
Tuple_map_assignment_statement::do_determine_types()
{
  this->map_index_->determine_type_no_context();

  if (this->map_index_->map_index_expression() != NULL)
    this->map_index_->map_index_expression() ->set_is_in_tuple_assignment();

  Type_context subcontext1(this->map_index_->type()->points_to(), false);
  this->val_->determine_type(&subcontext1);

  Type_context subcontext2(Type::lookup_bool_type(), false);
  this->present_->determine_type(&subcontext2);
}

// Check types.

void
Tuple_map_assignment_statement::do_check_types(Gogo*)
{
  Map_index_expression* map_index = this->map_index_->map_index_expression();
  if (map_index == NULL)
    {
      this->report_error(_("expected map index on right hand side"));
      return;
    }
  gcc_assert(map_index->is_in_tuple_assignment());

  if (!this->val_->is_lvalue() || !this->present_->is_lvalue())
    {
      this->report_error(_("invalid left hand side of assignment"));
      return;
    }

  // The type of the map index comes back as a pointer to the actual
  // value type, because we called set_is_in_tuple_assignment.
  Type* map_index_type = map_index->type()->points_to();
  gcc_assert(map_index_type != NULL);

  std::string reason;
  if (!Type::are_compatible_for_assign(this->val_->type(), map_index_type,
				       &reason))
    {
      if (reason.empty())
	error_at(this->val_->location(),
		 "incompatible types for variable and map");
      else
	error_at(this->val_->location(),
		 "incompatible types for variable and map (%s)",
		 reason.c_str());
      this->set_is_error();
    }

  if (!Type::are_compatible_for_assign(this->present_->type(),
				       Type::lookup_bool_type(),
				       &reason))
    {
      if (reason.empty())
	error_at(this->present_->location(), "incompatible type for map index");
      else
	error_at(this->present_->location(),
		 "incompatible type for map index (%s)",
		 reason.c_str());
      this->set_is_error();
    }
}

// Get a tree for a map assignment which returns a pair of values.

tree
Tuple_map_assignment_statement::do_get_tree(Translate_context* context)
{
  tree valptr = this->map_index_->get_tree(context);
  if (valptr == error_mark_node)
    return error_mark_node;
  valptr = save_expr(valptr);

  tree present_tree = this->present_->get_tree(context);
  if (present_tree == error_mark_node)
    return error_mark_node;

  Type* rhs_type = this->map_index_->type()->points_to();
  tree rhs_tree = build_fold_indirect_ref(valptr);
  source_location location = this->location();
  tree assign_val = Assignment_statement::get_assignment_tree(context,
							      OPERATOR_EQ,
							      this->val_,
							      NULL_TREE,
							      this->map_index_,
							      rhs_type,
							      rhs_tree,
							      location);
  if (assign_val == error_mark_node)
    return error_mark_node;

  return fold_build3(COND_EXPR, void_type_node,
		     fold_build2(EQ_EXPR, boolean_type_node, valptr,
				 fold_convert(TREE_TYPE(valptr),
					      null_pointer_node)),
		     fold_build2(MODIFY_EXPR, void_type_node,
				 present_tree, boolean_false_node),
		     build2(COMPOUND_EXPR, void_type_node,
			    fold_build2(MODIFY_EXPR, void_type_node,
					present_tree, boolean_true_node),
			    assign_val));
}

// Make a map assignment statement which returns a pair of values.

Statement*
Statement::make_tuple_map_assignment(Expression* val, Expression* present,
				     Expression* map_index,
				     source_location location)
{
  return new Tuple_map_assignment_statement(val, present, map_index, location);
}

// Assign a pair of entries to a map.

class Map_assignment_statement : public Statement
{
 public:
  Map_assignment_statement(Expression* map_index,
			   Expression* val, Expression* should_set,
			   source_location location)
    : Statement(STATEMENT_MAP_ASSIGNMENT, location),
      map_index_(map_index), val_(val), should_set_(should_set)
  { }

 protected:
  int
  do_traverse(Traverse* traverse);

  bool
  do_traverse_assignments(Traverse_assignments*);

  void
  do_determine_types();

  void
  do_check_types(Gogo*);

  tree
  do_get_tree(Translate_context* context);

 private:
  // A reference to the map index which should be set or deleted.
  Expression* map_index_;
  // The value to add to the map.
  Expression* val_;
  // Whether or not to add the value.
  Expression* should_set_;
};

// Traverse a map assignment.

int
Map_assignment_statement::do_traverse(Traverse* traverse)
{
  if (this->traverse_expression(traverse, &this->map_index_) == TRAVERSE_EXIT
      || this->traverse_expression(traverse, &this->val_) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  return this->traverse_expression(traverse, &this->should_set_);
}

bool
Map_assignment_statement::do_traverse_assignments(
    Traverse_assignments *tassign)
{
  tassign->assignment(&this->map_index_, NULL);
  tassign->value(&this->val_, true, false);
  tassign->value(&this->should_set_, false, false);
  return true;
}

// Determine types of expressions in a map assignment.

void
Map_assignment_statement::do_determine_types()
{
  this->map_index_->determine_type_no_context();
  Map_type* mt = this->map_index_->map_index_expression()->get_map_type();
  Type_context context1(mt->val_type(), false);
  this->val_->determine_type(&context1);
  Type_context context2(Type::lookup_bool_type(), false);
  this->should_set_->determine_type(&context2);
}

// Check types in a map assignment.

void
Map_assignment_statement::do_check_types(Gogo*)
{
  if (this->map_index_->map_index_expression() == NULL)
    {
      this->report_error("expected map index on left hand side");
      return;
    }

  Map_type *map_type = this->map_index_->map_index_expression()->get_map_type();
  std::string reason;
  if (!Type::are_compatible_for_assign(map_type->val_type(),
				       this->val_->type(),
				       &reason))
    {
      if (reason.empty())
	this->report_error("incompatible types for map and value");
      else
	{
	  error_at(this->location(),
		   "incompatible types for map and value (%s)",
		   reason.c_str());
	  this->set_is_error();
	}
    }

  if (!Type::are_compatible_for_assign(Type::lookup_bool_type(),
					    this->should_set_->type(),
					    &reason))
    {
      if (reason.empty())
	error_at(this->should_set_->location(),
		 "incompatible type for map assignment");
      else
	error_at(this->should_set_->location(),
		 "incompatible type for map assignment (%s)",
		 reason.c_str());
      this->set_is_error();
    }
}

// Return a tree for a map assignment of a pair.

tree
Map_assignment_statement::do_get_tree(Translate_context* context)
{
  tree should_set_tree = this->should_set_->get_tree(context);
  Refcount_decrement_lvalue_expression* rdle =
    this->map_index_->refcount_decrement_lvalue_expression();
  tree ins;
  tree del;
  if (rdle == NULL)
    {
      ins = Assignment_statement::get_assignment_tree(context, OPERATOR_EQ,
						      this->map_index_,
						      NULL, this->val_, NULL,
						      NULL, this->location());
      del = this->map_index_->map_index_expression()->delete_key(context);
    }
  else
    {
      ins = rdle->set(context, rdle->get_tree(context),
		      this->val_->get_tree(context));
      del = rdle->expr()->map_index_expression()->delete_key(context);
    }
  if (should_set_tree == error_mark_node
      || ins == error_mark_node
      || del == error_mark_node)
    return error_mark_node;
  return fold_build3(COND_EXPR, void_type_node, should_set_tree, ins, del);
}

// Make a statement which assigns a pair of entries to a map.

Statement*
Statement::make_map_assignment(Expression* map_index,
			       Expression* val, Expression* should_set,
			       source_location location)
{
  return new Map_assignment_statement(map_index, val, should_set, location);
}

// A tuple assignment from a receive statement.

class Tuple_receive_assignment_statement : public Statement
{
 public:
  Tuple_receive_assignment_statement(Expression* val, Expression* success,
				     Expression* channel,
				     source_location location)
    : Statement(STATEMENT_TUPLE_RECEIVE_ASSIGNMENT, location),
      val_(val), success_(success), channel_(channel)
  { }

 protected:
  int
  do_traverse(Traverse* traverse);

  bool
  do_traverse_assignments(Traverse_assignments*);

  void
  do_determine_types();

  void
  do_check_types(Gogo*);

  tree
  do_get_tree(Translate_context*);

 private:
  // Lvalue which receives the value from the channel.
  Expression* val_;
  // Lvalue which receives whether the read succeeded or failed.
  Expression* success_;
  // The channel on which we receive the value.
  Expression* channel_;
};

// Traversal.

int
Tuple_receive_assignment_statement::do_traverse(Traverse* traverse)
{
  if (this->traverse_expression(traverse, &this->val_) == TRAVERSE_EXIT
      || this->traverse_expression(traverse, &this->success_) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  return this->traverse_expression(traverse, &this->channel_);
}

bool
Tuple_receive_assignment_statement::do_traverse_assignments(
    Traverse_assignments* tassign)
{
  tassign->assignment(&this->val_, NULL);
  tassign->assignment(&this->success_, NULL);
  tassign->value(&this->channel_, false, this->val_->is_local_variable());
  return true;
}

// Set types if necessary.

void
Tuple_receive_assignment_statement::do_determine_types()
{
  this->channel_->determine_type_no_context();
  Type* type = this->channel_->type();
  Channel_type* channel_type = type->channel_type();

  Type_context subcontext1((channel_type != NULL
			    ? channel_type->element_type()
			    : NULL),
			   false);
  this->val_->determine_type(&subcontext1);

  Type_context subcontext2(Type::lookup_bool_type(), false);
  this->success_->determine_type(&subcontext2);
}

// Check types.

void
Tuple_receive_assignment_statement::do_check_types(Gogo*)
{
  Type* type = this->channel_->type();
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

  if (!this->val_->is_lvalue() || !this->success_->is_lvalue())
    {
      this->report_error(_("invalid left hand side of assignment"));
      return;
    }

  std::string reason;
  if (!Type::are_compatible_for_assign(this->val_->type(),
				       type->channel_type()->element_type(),
				       &reason))
    {
      if (reason.empty())
	error_at(this->val_->location(),
		 "incompatible types for variable and channel");
      else
	error_at(this->val_->location(),
		 "incompatible types for variable and channel (%s)",
		 reason.c_str());
      this->set_is_error();
    }

  if (!Type::are_compatible_for_assign(this->success_->type(),
				       Type::lookup_bool_type(),
				       &reason))
    {
      if (reason.empty())
	error_at(this->success_->location(), "incompatible type for receive");
      else
	error_at(this->success_->location(),
		 "incompatible type for receive (%s)",
		 reason.c_str());
      this->set_is_error();
    }
}

// Get a tree for a nonblocking receive statement.

tree
Tuple_receive_assignment_statement::do_get_tree(Translate_context* context)
{
  Gogo* gogo = context->gogo();

  Channel_type* channel_type = this->channel_->type()->channel_type();
  gcc_assert(channel_type != NULL);
  Type* element_type = channel_type->element_type();
  tree element_type_tree = element_type->get_tree(gogo);

  tree stmt_list = NULL_TREE;

  tree channel_tree = this->channel_->get_tree(context);
  if (element_type_tree == error_mark_node || channel_tree == error_mark_node)
    return error_mark_node;

  tree val_success = Gogo::receive_from_channel(element_type_tree,
						channel_tree, false, false,
						this->location());
  val_success = save_expr(val_success);

  // VAL_SUCCESS is a struct.  The first field is the value.  The
  // second field is whether the receive succeeded.
  tree val_field = TYPE_FIELDS(TREE_TYPE(val_success));
  tree success_field = TREE_CHAIN(val_field);
  gcc_assert(TREE_TYPE(success_field) == boolean_type_node);

  tree success = build3(COMPONENT_REF, boolean_type_node, val_success,
			success_field, NULL_TREE);

  tree set_success =
    Assignment_statement::get_assignment_tree(context, OPERATOR_EQ,
					      this->success_, NULL_TREE,
					      NULL,
					      Type::lookup_bool_type(),
					      success,
					      this->location());
  append_to_statement_list(set_success, &stmt_list);

  tree val_rhs = build3(COMPONENT_REF, TREE_TYPE(val_field), val_success,
			val_field, NULL_TREE);

  tree val_type_tree = this->val_->type()->get_tree(gogo);

  // FIXME: Duplicates Gogo::receive_from_channel.
  if (int_size_in_bytes(element_type_tree) <= 8
      && !AGGREGATE_TYPE_P(element_type_tree))
    {
      int bitsize = GET_MODE_BITSIZE(TYPE_MODE(val_type_tree));
      tree int_type_tree = go_type_for_size(bitsize, 1);
      val_rhs = fold_convert_loc(this->location(), int_type_tree, val_rhs);
    }

  val_rhs = fold_convert_loc(this->location(), val_type_tree, val_rhs);
  tree sval = Assignment_statement::get_assignment_tree(context,
							OPERATOR_EQ,
							this->val_,
							NULL_TREE,
							NULL,
							element_type,
							val_rhs,
							this->location());
  tree cmove = build3(COND_EXPR, void_type_node, success, sval, NULL_TREE);
  append_to_statement_list(cmove, &stmt_list);

  return stmt_list;
}

// Make a nonblocking receive statement.

Statement*
Statement::make_tuple_receive_assignment(Expression* val, Expression* success,
					 Expression* channel,
					 source_location location)
{
  return new Tuple_receive_assignment_statement(val, success, channel,
						location);
}

// An assignment to a pair of values from a type guard.  This is a
// conditional type guard.

class Tuple_type_guard_assignment_statement : public Statement
{
 public:
  Tuple_type_guard_assignment_statement(Expression* val, Expression* ok,
					Expression* expr, Type* type,
					source_location location)
    : Statement(STATEMENT_TUPLE_TYPE_GUARD_ASSIGNMENT, location),
      val_(val), ok_(ok), expr_(expr), type_(type)
  { }

 protected:
  int
  do_traverse(Traverse*);

  bool
  do_traverse_assignments(Traverse_assignments*);

  void
  do_determine_types();

  void
  do_check_types(Gogo*);

  tree
  do_get_tree(Translate_context*);

 private:
  // The variable which recieves the converted value.
  Expression* val_;
  // The variable which receives the indication of success.
  Expression* ok_;
  // The expression being converted.
  Expression* expr_;
  // The type to which the expression is being converted.
  Type* type_;
};

// Traverse a type guard tuple assignment.

int
Tuple_type_guard_assignment_statement::do_traverse(Traverse* traverse)
{
  if (this->traverse_expression(traverse, &this->val_) == TRAVERSE_EXIT
      || this->traverse_expression(traverse, &this->ok_) == TRAVERSE_EXIT
      || this->traverse_type(traverse, this->type_) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  return this->traverse_expression(traverse, &this->expr_);
}

bool
Tuple_type_guard_assignment_statement::do_traverse_assignments(
    Traverse_assignments* tassign)
{
  tassign->assignment(&this->val_, NULL);
  tassign->assignment(&this->ok_, NULL);
  tassign->value(&this->expr_, false, this->val_->is_local_variable());
  return true;
}

// Determine types of a type guard tuple assignment.

void
Tuple_type_guard_assignment_statement::do_determine_types()
{
  this->val_->determine_type_no_context();
  Type_context subcontext(Type::lookup_bool_type(), false);
  this->ok_->determine_type(&subcontext);
  this->expr_->determine_type_no_context();
}

// Check types of a type guard tuple assignment.

void
Tuple_type_guard_assignment_statement::do_check_types(Gogo*)
{
  if (this->expr_->type()->interface_type() == NULL)
    {
      this->report_error(_("type guard only valid for interface types"));
      return;
    }

  if (!this->val_->is_lvalue() || !this->ok_->is_lvalue())
    {
      this->report_error(_("invalid left hand side of assignment"));
      return;
    }

  std::string reason;
  if (!Type::are_compatible_for_assign(this->val_->type(), this->type_,
				       &reason))
    {
      if (reason.empty())
	error_at(this->val_->location(),
		 "incompatible types for type guard value");
      else
	error_at(this->val_->location(),
		 "incompatible types for type guard value (%s)",
		 reason.c_str());
      this->set_is_error();
    }

  if (!Type::are_compatible_for_assign(this->ok_->type(),
				       Type::lookup_bool_type(), &reason))
    {
      if (reason.empty())
	error_at(this->ok_->location(), "incompatible type for type guard");
      else
	error_at(this->ok_->location(), "incompatible type for type guard (%s)",
		 reason.c_str());
      this->set_is_error();
    }
}

// Return the tree for a type guard tuple statement.  The right hand
// side (THIS->EXPR_) is an interface type.  There are two cases to
// consider: whether or not we are converting to an interface type.

tree
Tuple_type_guard_assignment_statement::do_get_tree(Translate_context* context)
{
  Gogo* gogo = context->gogo();

  tree expr_tree = this->expr_->get_tree(context);
  if (expr_tree == error_mark_node)
    return error_mark_node;

  source_location location = this->location();

  gcc_assert(this->expr_->type()->interface_type() != NULL);

  Interface_type* interface_type = this->type_->interface_type();
  if (interface_type != NULL)
    {
      tree lhs_type_descriptor = interface_type->type_descriptor(gogo);
      gcc_assert(POINTER_TYPE_P(TREE_TYPE(expr_tree)));

      tree stmt_list = NULL_TREE;

      tree ok_tree = this->ok_->get_tree(context);
      if (ok_tree == error_mark_node)
	return error_mark_node;

      tree tmp;
      tree ok_addr;
      if (TREE_CODE(ok_tree) == VAR_DECL)
	{
	  tmp = NULL_TREE;
	  gcc_assert(TREE_CODE(TREE_TYPE(ok_tree)) == BOOLEAN_TYPE);
	  ok_addr = build_fold_addr_expr(ok_tree);
	  TREE_ADDRESSABLE(ok_tree) = 1;
	}
      else
	{
	  tmp = create_tmp_var(boolean_type_node, get_name(boolean_type_node));
	  DECL_IGNORED_P(tmp) = 0;
	  TREE_ADDRESSABLE(tmp) = 1;
	  tree make_tmp = build1(DECL_EXPR, void_type_node, tmp);
	  SET_EXPR_LOCATION(make_tmp, location);
	  append_to_statement_list(make_tmp, &stmt_list);
	  ok_addr = build_fold_addr_expr(tmp);
	}

      static tree convert_interface_decl;
      tree call = Gogo::call_builtin(&convert_interface_decl,
				     location,
				     "__go_convert_interface",
				     3,
				     ptr_type_node,
				     TREE_TYPE(lhs_type_descriptor),
				     lhs_type_descriptor,
				     ptr_type_node,
				     fold_convert(ptr_type_node, expr_tree),
				     build_pointer_type(boolean_type_node),
				     ok_addr);
      call = save_expr(call);
      append_to_statement_list(call, &stmt_list);
      if (tmp != NULL_TREE)
	append_to_statement_list(build2(MODIFY_EXPR, void_type_node,
					ok_tree, tmp),
				 &stmt_list);
      else
	tmp = ok_tree;

      call = fold_convert(interface_type->get_tree(gogo), call);
      tree assign = Assignment_statement::get_assignment_tree(context,
							      OPERATOR_EQ,
							      this->val_,
							      NULL_TREE,
							      NULL,
							      this->type_,
							      call,
							      location);

      append_to_statement_list(build3(COND_EXPR, void_type_node, tmp, assign,
				      NULL_TREE),
			       &stmt_list);

      return stmt_list;
    }
  else
    {
      // We are converting from an interface to a plain type.  This is
      // OK if the type descriptors are the same.
      expr_tree = save_expr(expr_tree);
      gcc_assert(POINTER_TYPE_P(TREE_TYPE(expr_tree)));
      tree struct_type = TREE_TYPE(TREE_TYPE(expr_tree));
      gcc_assert(TREE_CODE(struct_type) == RECORD_TYPE);
      tree field = TYPE_FIELDS(struct_type);
      gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)),
			"__type_descriptor") == 0);
      tree rhs_td = build3(COMPONENT_REF, TREE_TYPE(field),
			   build_fold_indirect_ref(expr_tree),
			   field, NULL_TREE);

      tree lhs_td = this->type_->type_descriptor(gogo);

      tree ok_true =
	Assignment_statement::get_assignment_tree(context,
						  OPERATOR_EQ,
						  this->ok_,
						  NULL_TREE,
						  NULL,
						  Type::lookup_bool_type(),
						  boolean_true_node,
						  location);
      tree ok_false =
	Assignment_statement::get_assignment_tree(context,
						  OPERATOR_EQ,
						  this->ok_,
						  NULL_TREE,
						  NULL,
						  Type::lookup_bool_type(),
						  boolean_false_node,
						  location);
      tree val_true =
	Assignment_statement::get_assignment_tree(context,
						  OPERATOR_EQ,
						  this->val_,
						  NULL_TREE,
						  this->expr_,
						  NULL,
						  expr_tree,
						  location);

      static tree type_descriptors_equal_fndecl;
      tree compare = Gogo::call_builtin(&type_descriptors_equal_fndecl,
					location,
					"__go_type_descriptors_equal",
					2,
					boolean_type_node,
					TREE_TYPE(lhs_td),
					lhs_td,
					TREE_TYPE(rhs_td),
					rhs_td);

      tree is_nil = fold_build2(EQ_EXPR, boolean_type_node, expr_tree,
				fold_convert(TREE_TYPE(expr_tree),
					     null_pointer_node));

      return fold_build3(COND_EXPR, void_type_node,
			 is_nil,
			 ok_false,
			 build3(COND_EXPR, void_type_node,
				compare,
				build2(COMPOUND_EXPR, void_type_node, ok_true,
				       val_true),
				ok_false));
    }
}

// Make an assignment from a type guard to a pair of variables.

Statement*
Statement::make_tuple_type_guard_assignment(Expression* val, Expression* ok,
					    Expression* expr, Type* type,
					    source_location location)
{
  return new Tuple_type_guard_assignment_statement(val, ok, expr, type,
						   location);
}

// An expression statement.

class Expression_statement : public Statement
{
 public:
  Expression_statement(Expression* expr)
    : Statement(STATEMENT_EXPRESSION, expr->location()),
      expr_(expr)
  { }

 protected:
  int
  do_traverse(Traverse* traverse)
  { return this->traverse_expression(traverse, &this->expr_); }

  void
  do_determine_types()
  { this->expr_->determine_type_no_context(); }

  bool
  do_may_fall_through() const;

  tree
  do_get_tree(Translate_context* context)
  { return this->expr_->get_tree(context); }

 private:
  Expression* expr_;
};

// An expression statement may fall through unless it is a call to a
// function which does not return.

bool
Expression_statement::do_may_fall_through() const
{
  const Call_expression* call = this->expr_->call_expression();
  if (call != NULL)
    {
      const Expression* fn = call->fn();
      const Func_expression* fe = fn->func_expression();
      if (fe != NULL)
	{
	  const Named_object* no = fe->named_object();

	  Function_type* fntype;
	  if (no->is_function())
	    fntype = no->func_value()->type();
	  else if (no->is_function_declaration())
	    fntype = no->func_declaration_value()->type();
	  else
	    fntype = NULL;

	  // The builtin functions panic and panicln do not return.
	  if ((no->name() == "panic" || no->name() == "panicln")
	      && fntype != NULL
	      && fntype->is_builtin())
	    return false;
	}
    }
  return true;
}

// Make an expression statement from an Expression.

Statement*
Statement::make_statement(Expression* expr)
{
  return new Expression_statement(expr);
}

// A block statement--a list of statements which may include variable
// definitions.

class Block_statement : public Statement
{
 public:
  Block_statement(Block* block, source_location location)
    : Statement(STATEMENT_BLOCK, location),
      block_(block)
  { }

 protected:
  int
  do_traverse(Traverse* traverse)
  { return this->block_->traverse(traverse); }

  void
  do_determine_types()
  { this->block_->determine_types(); }

  bool
  do_may_fall_through() const
  { return this->block_->may_fall_through(); }

  tree
  do_get_tree(Translate_context* context)
  { return this->block_->get_tree(context); }

 private:
  Block* block_;
};

// Make a block statement.

Statement*
Statement::make_block_statement(Block* block, source_location location)
{
  return new Block_statement(block, location);
}

// An increment or decrement statement.

class Inc_dec_statement : public Statement
{
 public:
  Inc_dec_statement(bool is_inc, Expression* expr)
    : Statement(STATEMENT_INCDEC, expr->location()),
      expr_(expr), is_inc_(is_inc)
  { }

 protected:
  int
  do_traverse(Traverse* traverse)
  { return this->traverse_expression(traverse, &this->expr_); }

  bool
  do_traverse_assignments(Traverse_assignments*);

  void
  do_determine_types()
  { this->expr_->determine_type_no_context(); }

  void
  do_check_types(Gogo*);

  tree
  do_get_tree(Translate_context*);

 private:
  // The l-value to increment or decrement.
  Expression* expr_;
  // Whether to increment or decrement.
  bool is_inc_;
};

// Traverse.

bool
Inc_dec_statement::do_traverse_assignments(Traverse_assignments* tassign)
{
  // The expression is both used and set.
  tassign->assignment(&this->expr_, NULL);
  tassign->value(&this->expr_, false, this->expr_->is_local_variable());
  return true;
}

// Check types for an increment or decrement statement.

void
Inc_dec_statement::do_check_types(Gogo*)
{
  if (!this->expr_->is_lvalue())
    {
      this->report_error(_("attempt to increment or decrement "
			   "something which is not an object"));
      return;
    }

  Type* type = this->expr_->type();
  if (type->integer_type() == NULL
      && type->float_type() == NULL)
    this->report_error(_("attempt to increment or decrement "
			 "non-integer, non-float value"));
}

// Get a tree for an increment or decrement statement.  Increment and
// decrement statements are atomic.  FIXME: To implement this
// correctly we need special handling for increments of values in
// maps, as in map["keyval"]++.

tree
Inc_dec_statement::do_get_tree(Translate_context* context)
{
  Type* type = this->expr_->type();
  gcc_assert(type->integer_type() != NULL || type->float_type() != NULL);

  tree expr_tree = this->expr_->get_tree(context);

  // We don't need to use atomic operations for a local variable.
  Var_expression* ve = this->expr_->var_expression();
  if (ve != NULL)
    {
      Named_object* no = ve->named_object();
      if ((no->is_variable()
	   && !no->var_value()->is_global()
	   && !no->var_value()->is_address_taken())
	  || no->is_result_variable())
	return build2(this->is_inc_ ? PREINCREMENT_EXPR : PREDECREMENT_EXPR,
		      TREE_TYPE(expr_tree), expr_tree,
		      (type->integer_type() != NULL
		       ? build_int_cst(TREE_TYPE(expr_tree), 1)
		       : build_real(TREE_TYPE(expr_tree), dconst1)));
    }

  if (type->float_type() != NULL)
    {
      sorry("atomic increment/decrement of float");
      return error_mark_node;
    }

  gcc_assert(TREE_CODE(TREE_TYPE(expr_tree)) == INTEGER_TYPE);

  built_in_function bcode;
  switch (TYPE_PRECISION(TREE_TYPE(expr_tree)))
    {
    case 8:
      bcode = BUILT_IN_ADD_AND_FETCH_1;
      break;
    case 16:
      bcode = BUILT_IN_ADD_AND_FETCH_2;
      break;
    case 32:
      bcode = BUILT_IN_ADD_AND_FETCH_4;
      break;
    case 64:
      bcode = BUILT_IN_ADD_AND_FETCH_8;
      break;
    default:
      gcc_unreachable();
    }

  tree val = build_int_cst(TREE_TYPE(expr_tree),
			   this->is_inc_ ? 1 : -1);

  return build_call_expr(implicit_built_in_decls[bcode], 2,
			 build_fold_addr_expr(expr_tree),
			 val);
}

// Make an increment statement.

Statement*
Statement::make_inc_statement(Expression* expr)
{
  return new Inc_dec_statement(true, expr);
}

// Make a decrement statement.

Statement*
Statement::make_dec_statement(Expression* expr)
{
  return new Inc_dec_statement(false, expr);
}

// Class Thunk_statement.  This is the base class for go and defer
// statements.

int Thunk_statement::thunk_count;

const char* const Thunk_statement::thunk_field_fn = "fn";

const char* const Thunk_statement::thunk_field_receiver = "receiver";

// Constructor.

Thunk_statement::Thunk_statement(Statement_classification classification,
				 Call_expression* call,
				 source_location location)
    : Statement(classification, location),
      call_(call), struct_type_(NULL)
{
}

// Return whether this is a simple statement which does not require a
// thunk.

bool
Thunk_statement::is_simple(Function_type* fntype) const
{
  // We need a thunk to call a method, or to pass a variable number of
  // arguments.
  if (fntype->is_method() || fntype->is_varargs())
    return false;

  // We can only permit a single parameter of pointer type.
  const Typed_identifier_list* parameters = fntype->parameters();
  if (parameters != NULL
      && (parameters->size() > 1
	  || (parameters->size() == 1
	      && parameters->begin()->type()->points_to() == NULL)))
    return false;

  // If the single parameter is reference counted, then we need a
  // thunk in order to decrement the reference count when the function
  // is complete.  FIXME: In practice this means that the only simple
  // go statements are the ones with no parameters.
  if (parameters != NULL
      && !parameters->empty()
      && parameters->begin()->type()->has_refcounted_component())
    return false;

  // If the function returns multiple values, or returns a type other
  // than integer, floating point, or pointer, then it may get a
  // hidden first parameter, in which case we need the more
  // complicated approach.  This is true even though we are going to
  // ignore the return value.
  const Typed_identifier_list* results = fntype->results();
  if (results != NULL
      && (results->size() > 1
	  || (results->size() == 1
	      && !results->begin()->type()->is_basic_type()
	      && results->begin()->type()->points_to() == NULL)))
    return false;

  // If the function returns a reference counted type, then we need a
  // thunk in order to discard the function's return value.
  if (results != NULL
      && !results->empty()
      && results->begin()->type()->has_refcounted_component())
    return false;

  // If this calls something which is not a simple function, then we
  // need a thunk.
  Expression* fn = this->call_->call_expression()->fn();
  if (fn->bound_method_expression() != NULL
      || fn->interface_field_reference_expression() != NULL)
    return false;

  return true;
}

// Traverse a thunk statement.

int
Thunk_statement::do_traverse(Traverse* traverse)
{
  return this->traverse_expression(traverse, &this->call_);
}

// We implement traverse_assignment for a thunk statement because it
// effectively copies the function call.

bool
Thunk_statement::do_traverse_assignments(Traverse_assignments* tassign)
{
  Expression* fn = this->call_->call_expression()->fn();
  Expression* fn2 = fn;
  tassign->value(&fn2, true, false);
  gcc_assert(fn == fn2);
  return true;
}

// Determine types in a thunk statement.

void
Thunk_statement::do_determine_types()
{
  this->call_->determine_type_no_context();

  // Now that we know the types of the call, build the struct used to
  // pass parameters.
  Function_type* fntype =
    this->call_->call_expression()->get_function_type(false);
  if (fntype != NULL && !this->is_simple(fntype))
    this->struct_type_ = this->build_struct(fntype);
}

// Check types in a thunk statement.

void
Thunk_statement::do_check_types(Gogo*)
{
  Call_expression* ce = this->call_->call_expression();
  Function_type* fntype = ce->get_function_type(false);
  if (fntype != NULL && fntype->is_method())
    {
      Expression* fn = ce->fn();
      if (fn->bound_method_expression() == NULL
	  && fn->interface_field_reference_expression() == NULL)
	this->report_error(_("no object for method call"));
    }
}

// The Traverse class used to find and simplify thunk statements.

class Simplify_thunk_traverse : public Traverse
{
 public:
  Simplify_thunk_traverse(Gogo* gogo)
    : Traverse(traverse_blocks),
      gogo_(gogo)
  { }

  int
  block(Block*);

 private:
  Gogo* gogo_;
};

int
Simplify_thunk_traverse::block(Block* b)
{
  // The parser ensures that thunk statements always appear in a block
  // which has only a single statement.
  if (b->statements()->size() != 1)
    return TRAVERSE_CONTINUE;
  Thunk_statement* stat = b->statements()->front()->thunk_statement();
  if (stat == NULL)
    return TRAVERSE_CONTINUE;
  if (stat->simplify_statement(this->gogo_, b))
    return TRAVERSE_SKIP_COMPONENTS;
  return TRAVERSE_CONTINUE;
}

// Simplify all thunk statements.

void
Gogo::simplify_thunk_statements()
{
  Simplify_thunk_traverse thunk_traverse(this);
  this->traverse(&thunk_traverse);
}

// Simplify complex thunk statements into simple ones.  A complicated
// thunk statement is one which takes anything other than zero
// parameters or a single pointer parameter.  We rewrite it into code
// which allocates a struct, stores the parameter values into the
// struct, and does a simple go or defer statement which passes the
// struct to a thunk.  The thunk does the real call.

bool
Thunk_statement::simplify_statement(Gogo* gogo, Block* block)
{
  if (this->classification() == STATEMENT_ERROR)
    return false;
  if (this->call_->is_error_expression())
    return false;

  Call_expression* ce = this->call_->call_expression();
  Function_type* fntype = ce->get_function_type(false);
  if (fntype == NULL || this->is_simple(fntype))
    return false;

  Expression* fn = ce->fn();
  Bound_method_expression* bound_method = fn->bound_method_expression();
  Interface_field_reference_expression* interface_method =
    fn->interface_field_reference_expression();
  const bool is_method = bound_method != NULL || interface_method != NULL;

  source_location location = this->location();

  // Get a name to use for the thunk.
  char thunk_name[50];
  snprintf(thunk_name, sizeof thunk_name, "$thunk%d",
	   Thunk_statement::thunk_count);
  ++Thunk_statement::thunk_count;

  // Build the thunk.
  this->build_thunk(gogo, thunk_name, fntype);

  // Generate code to call the thunk.

  // Get the values to store into the struct which is the single
  // argument to the thunk.

  Expression_list* vals = new Expression_list();
  if (fntype->is_builtin())
    ;
  else if (!is_method)
    vals->push_back(fn);
  else if (interface_method != NULL)
    vals->push_back(interface_method->expr());
  else if (bound_method != NULL)
    {
      vals->push_back(bound_method->method());
      Expression* first_arg = bound_method->first_argument();

      // We always pass a pointer when calling a method.
      if (first_arg->type()->points_to() == NULL)
	first_arg = Expression::make_unary(OPERATOR_AND, first_arg, location);

      // If we are calling a method which was inherited from an
      // embedded struct, and the method did not get a stub, then the
      // first type may be wrong.
      Type* fatype = bound_method->first_argument_type();
      if (fatype != NULL)
	{
	  if (fatype->points_to() == NULL)
	    fatype = Type::make_pointer_type(fatype);
	  Type* unsafe = Type::make_pointer_type(Type::make_void_type());
	  first_arg = Expression::make_cast(unsafe, first_arg, location);
	  first_arg = Expression::make_cast(fatype, first_arg, location);
	}

      vals->push_back(first_arg);
    }
  else
    gcc_unreachable();

  if (ce->args() != NULL)
    {
      for (Expression_list::const_iterator p = ce->args()->begin();
	   p != ce->args()->end();
	   ++p)
	vals->push_back(*p);
    }

  // Build the struct.
  Expression* constructor =
    Expression::make_struct_composite_literal(this->struct_type_, vals,
					      location);

  // Allocate the initialized struct on the heap.
  constructor = Expression::make_heap_composite(constructor, true, location);

  // Look up the thunk.
  Named_object* named_thunk = gogo->lookup(thunk_name, NULL);
  gcc_assert(named_thunk != NULL && named_thunk->is_function());

  // Build the call.
  Expression* func = Expression::make_func_reference(named_thunk, NULL,
						     location);
  Expression_list* params = new Expression_list();
  params->push_back(constructor);
  Call_expression* call = Expression::make_call(func, params, location);

  // Build the simple go or defer statement.
  Statement* s;
  if (this->classification() == STATEMENT_GO)
    s = Statement::make_go_statement(call, location);
  else if (this->classification() == STATEMENT_DEFER)
    s = Statement::make_defer_statement(call, location);
  else
    gcc_unreachable();

  // The current block should only have the go statement.
  gcc_assert(block->statements()->size() == 1);
  gcc_assert(block->statements()->front() == this);
  std::vector<Statement*> statements;
  statements.push_back(s);
  block->swap_statements(statements);

  // We already ran the determine_types pass, so we need to run it now
  // for this new block.
  block->determine_types();

  // Sanity check.
  gogo->check_types_in_block(block);

  // Return true to tell the block not to keep looking at statements.
  return true;
}

// Return whether a function is a thunk.

bool
Thunk_statement::is_thunk(const Named_object* no)
{
  return no->name().compare(0, 6, "$thunk") == 0;
}

// Set the name to use for thunk parameter N.

void
Thunk_statement::thunk_field_param(int n, char* buf, size_t buflen)
{
  snprintf(buf, buflen, "a%d", n);
}

// Build a new struct type to hold the parameters for a complicated
// thunk statement.  FNTYPE is the type of the function call.

Struct_type*
Thunk_statement::build_struct(Function_type* fntype)
{
  source_location location = this->location();

  Struct_field_list* fields = new Struct_field_list();

  Call_expression* ce = this->call_->call_expression();
  Expression* fn = ce->fn();

  Interface_field_reference_expression* interface_method =
    fn->interface_field_reference_expression();
  if (interface_method != NULL)
    {
      // If this thunk statement calls a method on an interface, we
      // pass the interface object to the thunk.
      Typed_identifier tid(Thunk_statement::thunk_field_fn,
			   interface_method->expr()->type(),
			   location);
      fields->push_back(Struct_field(tid));
    }
  else if (!fntype->is_builtin())
    {
      // The function to call.
      Typed_identifier tid(Go_statement::thunk_field_fn, fntype, location);
      fields->push_back(Struct_field(tid));
    }

  if (fn->bound_method_expression() != NULL)
    {
      gcc_assert(fntype->is_method());
      Type* rtype = fntype->receiver()->type();
      // We always pass the receiver as a pointer.
      if (rtype->points_to() == NULL)
	rtype = Type::make_pointer_type(rtype);
      Typed_identifier tid(Thunk_statement::thunk_field_receiver, rtype,
			   location);
      fields->push_back(Struct_field(tid));
    }

  const Expression_list* args = ce->args();
  if (args != NULL)
    {
      int i = 0;
      for (Expression_list::const_iterator p = args->begin();
	   p != args->end();
	   ++p, ++i)
	{
	  char buf[50];
	  this->thunk_field_param(i, buf, sizeof buf);
	  fields->push_back(Struct_field(Typed_identifier(buf, (*p)->type(),
							  location)));
	}
    }

  return Type::make_struct_type(fields, location);
}

// Build the thunk we are going to call.  This is a brand new, albeit
// artificial, function.

void
Thunk_statement::build_thunk(Gogo* gogo, const char* thunk_name,
			     Function_type* fntype)
{
  source_location location = this->location();

  // Build the type of the thunk.  The thunk takes a single parameter,
  // which is a pointer to the special structure we build.
  const char* const parameter_name = "__go_thunk_parameter";
  Typed_identifier_list* thunk_parameters = new Typed_identifier_list();
  Type* pointer_to_struct_type = Type::make_pointer_type(this->struct_type_);
  thunk_parameters->push_back(Typed_identifier(parameter_name,
					       pointer_to_struct_type,
					       this->location()));
  Function_type* thunk_type = Type::make_function_type(NULL, thunk_parameters,
						       NULL, this->location());

  // Start building the thunk.
  gogo->start_function(thunk_name, thunk_type, true, location);

  // Get a reference to the parameter.
  Named_object* named_parameter = gogo->lookup(parameter_name, NULL);
  gcc_assert(named_parameter != NULL && named_parameter->is_variable());

  // Build the call.  Note that the field names are the same as the
  // ones used in build_struct.
  Expression* thunk_parameter = Expression::make_var_reference(named_parameter,
							       location);

  Call_expression* ce = this->call_->call_expression();
  Bound_method_expression* bound_method = ce->fn()->bound_method_expression();
  Interface_field_reference_expression* interface_method =
    ce->fn()->interface_field_reference_expression();

  Expression* func_to_call;
  unsigned int next_index;
  if (!fntype->is_builtin())
    {
      func_to_call = Expression::make_field_reference(thunk_parameter,
						      0, location);
      next_index = 1;
    }
  else
    {
      gcc_assert(bound_method == NULL && interface_method == NULL);
      func_to_call = ce->fn();
      next_index = 0;
    }

  if (bound_method != NULL)
    {
      Expression* r = Expression::make_field_reference(thunk_parameter, 1,
						       location);
      // The main program passes in a function pointer from the
      // interface expression, so here we can make a bound method in
      // all cases.
      func_to_call = Expression::make_bound_method(r, func_to_call,
						   location);
      next_index = 2;
    }
  else if (interface_method != NULL)
    {
      // The main program passes the interface object.
      const std::string& name(interface_method->name());
      func_to_call = Expression::make_interface_field_reference(func_to_call,
								name,
								location);
    }

  Expression_list* call_params = new Expression_list();
  const Struct_field_list* fields = this->struct_type_->fields();
  Struct_field_list::const_iterator p = fields->begin();
  for (unsigned int i = 0; i < next_index; ++i)
    ++p;
  for (; p != fields->end(); ++p, ++next_index)
    {
      Expression* thunk_param = Expression::make_var_reference(named_parameter,
							       location);
      Expression* param = Expression::make_field_reference(thunk_param,
							   next_index,
							   location);
      call_params->push_back(param);
    }

  Expression* call = Expression::make_call(func_to_call, call_params, location);
  // We need to lower in case this is a builtin function.
  call = call->lower(gogo, -1);

  Statement* call_statement = Statement::make_statement(call);

  // We already ran the determine_types pass, so we need to run it
  // just for this statement now.
  call_statement->determine_types();

  gogo->add_statement(call_statement);

  // FIXME: Now we need to decrement the reference count of the
  // parameter.

  // That is all the thunk has to do.
  gogo->finish_function(location);
}

// Get the function and argument trees.

void
Thunk_statement::get_fn_and_arg(Translate_context* context, tree* pfn,
				tree* parg)
{
  if (this->call_->is_error_expression())
    {
      *pfn = error_mark_node;
      *parg = error_mark_node;
      return;
    }

  Call_expression* ce = this->call_->call_expression();

  Expression* fn = ce->fn();
  *pfn = fn->get_tree(context);

  const Expression_list* args = ce->args();
  if (args == NULL || args->empty())
    *parg = null_pointer_node;
  else
    {
      gcc_assert(args->size() == 1);
      Expression* arg = args->front();

      // The argument will typically include a reference count
      // decrement.  We do not want that decrement: the thunk is
      // responsible for decrementing the reference count.  FIXME: We
      // will have an entry on the queue which is never used.
      if (arg->refcount_adjust_expression() != NULL)
	{
	  int cl = arg->refcount_adjust_expression()->classification();
	  if (cl != REFCOUNT_INCREMENT_COPIED)
	    arg = arg->refcount_adjust_expression()->expr();
	}

      *parg = args->front()->get_tree(context);
    }
}

// Class Go_statement.

tree
Go_statement::do_get_tree(Translate_context* context)
{
  tree fn_tree;
  tree arg_tree;
  this->get_fn_and_arg(context, &fn_tree, &arg_tree);

  static tree go_fndecl;

  tree fn_arg_type = NULL_TREE;
  if (go_fndecl == NULL_TREE)
    {
      // Only build FN_ARG_TYPE if we need it.
      tree subargtypes = tree_cons(NULL_TREE, ptr_type_node, void_list_node);
      tree subfntype = build_function_type(ptr_type_node, subargtypes);
      fn_arg_type = build_pointer_type(subfntype);
    }

  return Gogo::call_builtin(&go_fndecl,
			    this->location(),
			    "__go_go",
			    2,
			    void_type_node,
			    fn_arg_type,
			    fn_tree,
			    ptr_type_node,
			    arg_tree);
}

// Make a go statement.

Statement*
Statement::make_go_statement(Call_expression* call, source_location location)
{
  return new Go_statement(call, location);
}

// Class Defer_statement.

tree
Defer_statement::do_get_tree(Translate_context* context)
{
  tree fn_tree;
  tree arg_tree;
  this->get_fn_and_arg(context, &fn_tree, &arg_tree);
  if (fn_tree == error_mark_node || arg_tree == error_mark_node)
    return error_mark_node;

  static tree defer_fndecl;

  tree fn_arg_type = NULL_TREE;
  if (defer_fndecl == NULL_TREE)
    {
      // Only build FN_ARG_TYPE if we need it.
      tree subargtypes = tree_cons(NULL_TREE, ptr_type_node, void_list_node);
      tree subfntype = build_function_type(ptr_type_node, subargtypes);
      fn_arg_type = build_pointer_type(subfntype);
    }

  tree defer_stack = context->function()->func_value()->defer_stack();

  tree call = Gogo::call_builtin(&defer_fndecl,
				 this->location(),
				 "__go_defer",
				 3,
				 ptr_type_node,
				 ptr_type_node,
				 defer_stack,
				 fn_arg_type,
				 fn_tree,
				 ptr_type_node,
				 arg_tree);
  return build2(MODIFY_EXPR, void_type_node, defer_stack, call);
}

// Make a defer statement.

Statement*
Statement::make_defer_statement(Call_expression* call,
				source_location location)
{
  return new Defer_statement(call, location);
}

// Class Return_statement.

// Record that we should not increment the reference count of NO.

void
Return_statement::add_do_not_increment(Named_object* no)
{
  gcc_assert(this->vals_ != NULL);
  for (Expression_list::const_iterator p = this->vals_->begin();
       p != this->vals_->end();
       ++p)
    {
      Var_expression* ve = (*p)->var_expression();
      if (ve != NULL && ve->named_object() == no)
	{
	  if (this->do_not_increment_ == NULL)
	    this->do_not_increment_ = new Expression_list;
	  this->do_not_increment_->push_back(ve);
	  return;
	}
    }
  gcc_unreachable();
}

// Traverse assignments.  A return statement doesn't assign any
// values, but it does require that we increment the reference count
// of the values we are returning.  So we treat each return value as a
// top level RHS in an expression.

bool
Return_statement::do_traverse_assignments(Traverse_assignments* tassign)
{
  Expression_list* vals = this->vals_;
  if (vals != NULL)
    {
      for (Expression_list::iterator p = vals->begin();
	   p != vals->end();
	   ++p)
	tassign->value(&*p, true, true);
    }
  return true;
}

// Determine types.

void
Return_statement::do_determine_types()
{
  if (this->vals_ == NULL)
    return;
  Function_type* type = this->function_->type();
  const Typed_identifier_list* results = type->results();
  if (results == NULL)
    return;

  // If the current function has multiple return values, and we are
  // returning a single function call expression, split up the call
  // expression.  We have to determine the type of the call expression
  // first, because we don't know how many values it returns until
  // method references are resolved.
  if (results->size() > 1
      && this->vals_->size() == 1
      && this->vals_->front()->call_expression() != NULL)
    {
      Call_expression* call = this->vals_->front()->call_expression();
      call->determine_type_no_context();
      size_t count = call->result_count();
      if (count > 1)
	{
	  Expression_list* vals = new Expression_list;
	  for (size_t i = 0; i < count; ++i)
	    vals->push_back(Expression::make_call_result(call, i));
	  delete this->vals_;
	  this->vals_ = vals;
	}
      return;
    }

  Typed_identifier_list::const_iterator pt = results->begin();
  for (Expression_list::iterator pe = this->vals_->begin();
       pe != this->vals_->end();
       ++pe)
    {
      if (pt == results->end())
	(*pe)->determine_type_no_context();
      else
	{
	  Type_context context(pt->type(), false);
	  (*pe)->determine_type(&context);
	  ++pt;
	}
    }
}

// Check types.

void
Return_statement::do_check_types(Gogo*)
{
  if (this->vals_ == NULL)
    return;

  Function_type* type = this->function_->type();

  const Typed_identifier_list* results = type->results();
  if (results == NULL)
    {
      this->report_error(_("return with value in function "
			   "with no return type"));
      return;
    }

  int i = 0;
  Typed_identifier_list::const_iterator pt = results->begin();
  for (Expression_list::const_iterator pe = this->vals_->begin();
       pe != this->vals_->end();
       ++pe, ++pt, ++i)
    {
      if (pt == results->end())
	{
	  this->report_error(_("too many values in return statement"));
	  return;
	}
      std::string reason;
      if (!Type::are_compatible_for_assign(pt->type(), (*pe)->type(), &reason))
	{
	  if (reason.empty())
	    error_at(this->location(),
		     "incompatible type for return value %d",
		     i);
	  else
	    error_at(this->location(),
		     "incompatible type for return value %d (%s)",
		     i, reason.c_str());
	  this->set_is_error();
	}
      else if (pt->type()->is_error_type()
	       || (*pe)->type()->is_error_type()
	       || pt->type()->is_undefined()
	       || (*pe)->type()->is_undefined())
	{
	  // Make sure we get the error for an undefined type.
	  pt->type()->base();
	  (*pe)->type()->base();
	  this->set_is_error();
	}
    }

  if (pt != results->end())
    this->report_error(_("not enough values in return statement"));
}

// Build a RETURN_EXPR tree.

tree
Return_statement::do_get_tree(Translate_context* context)
{
  tree fndecl = context->function()->func_value()->get_decl();

  gcc_assert(this->function_ == context->function()->func_value());
  Function_type* type = this->function_->type();
  const Typed_identifier_list* results = type->results();

  if (this->vals_ == NULL)
    {
      tree retval;
      if (VOID_TYPE_P(TREE_TYPE(TREE_TYPE(fndecl))))
	retval = NULL_TREE;
      else
	retval = build2(MODIFY_EXPR, void_type_node, DECL_RESULT(fndecl),
			context->function()->func_value()->return_value());
      return this->build_stmt_1(RETURN_EXPR, retval);
    }
  else if (this->vals_->size() == 1)
    {
      gcc_assert(!VOID_TYPE_P(TREE_TYPE(TREE_TYPE(fndecl))));
      tree val = (*this->vals_->begin())->get_tree(context);
      if (val == error_mark_node)
	return error_mark_node;
      gcc_assert(results != NULL && results->size() == 1);
      val = Expression::convert_for_assignment(context,
					       results->begin()->type(),
					       (*this->vals_->begin())->type(),
					       val, this->location());
      tree set = build2(MODIFY_EXPR, void_type_node,
			DECL_RESULT(fndecl), val);
      SET_EXPR_LOCATION(set, this->location());
      return this->build_stmt_1(RETURN_EXPR, set);
    }
  else
    {
      gcc_assert(!VOID_TYPE_P(TREE_TYPE(TREE_TYPE(fndecl))));
      tree stmt_list = NULL_TREE;
      tree rettype = TREE_TYPE(DECL_RESULT(fndecl));
      tree retvar = create_tmp_var(rettype, "RESULT");
      gcc_assert(results != NULL && results->size() == this->vals_->size());
      Expression_list::const_iterator pv = this->vals_->begin();
      Typed_identifier_list::const_iterator pr = results->begin();
      for (tree field = TYPE_FIELDS(rettype);
	   field != NULL_TREE;
	   ++pv, ++pr, field = TREE_CHAIN(field))
	{
	  gcc_assert(pv != this->vals_->end());
	  tree val = (*pv)->get_tree(context);
	  if (val == error_mark_node)
	    return error_mark_node;
	  val = Expression::convert_for_assignment(context, pr->type(),
						   (*pv)->type(), val,
						   this->location());
	  tree set = build2(MODIFY_EXPR, void_type_node,
			    build3(COMPONENT_REF, TREE_TYPE(field),
				   retvar, field, NULL_TREE),
			    val);
	  SET_EXPR_LOCATION(set, this->location());
	  append_to_statement_list(set, &stmt_list);
	}
      tree set = build2(MODIFY_EXPR, void_type_node, DECL_RESULT(fndecl),
			retvar);
      append_to_statement_list(this->build_stmt_1(RETURN_EXPR, set),
			       &stmt_list);
      return stmt_list;
    }
}

// Make a return statement.

Statement*
Statement::make_return_statement(const Function* function,
				 Expression_list* vals,
				 source_location location)
{
  return new Return_statement(function, vals, location);
}

// A break or continue statement.

class Bc_statement : public Statement
{
 public:
  Bc_statement(bool is_break, Statement* enclosing, source_location location)
    : Statement(STATEMENT_BREAK_OR_CONTINUE, location),
      enclosing_(enclosing), is_break_(is_break)
  { }

  bool
  is_break() const
  { return this->is_break_; }

 protected:
  int
  do_traverse(Traverse*)
  { return TRAVERSE_CONTINUE; }

  bool
  do_may_fall_through() const
  { return false; }

  tree
  do_get_tree(Translate_context*);

 private:
  // The for or switch statement that this applies to.
  Statement* enclosing_;
  // True if this is "break", false if it is "continue".
  bool is_break_;
};

// Return the tree for a break or continue statement.

tree
Bc_statement::do_get_tree(Translate_context*)
{
  Statement_classification sc = this->enclosing_->classification();
  tree label;
  if (sc == STATEMENT_FOR)
    {
      For_statement* f = this->enclosing_->for_statement();
      if (this->is_break_)
	label = f->break_label();
      else
	label = f->continue_label();
    }
  else if (sc == STATEMENT_FOR_RANGE)
    {
      For_range_statement* f = this->enclosing_->for_range_statement();
      if (this->is_break_)
	label = f->break_label();
      else
	label = f->continue_label();
    }
  else if (sc == STATEMENT_SWITCH)
    {
      Switch_statement* s = this->enclosing_->switch_statement();
      gcc_assert(this->is_break_);
      label = s->break_label();
    }
  else if (sc == STATEMENT_TYPE_SWITCH)
    {
      Type_switch_statement* s = this->enclosing_->type_switch_statement();
      gcc_assert(this->is_break_);
      label = s->break_label();
    }
  else if (sc == STATEMENT_SELECT)
    {
      Select_statement* s = this->enclosing_->select_statement();
      gcc_assert(this->is_break_);
      label = s->break_label();
    }
  else
    gcc_unreachable();

  return build_and_jump(&LABEL_EXPR_LABEL(label));
}

// Make a break statement.

Statement*
Statement::make_break_statement(Statement* enclosing, source_location location)
{
  return new Bc_statement(true, enclosing, location);
}

// Make a continue statement.

Statement*
Statement::make_continue_statement(Statement* enclosing,
				   source_location location)
{
  return new Bc_statement(false, enclosing, location);
}

// A goto statement.

class Goto_statement : public Statement
{
 public:
  Goto_statement(Label* label, source_location location)
    : Statement(STATEMENT_GOTO, location),
      label_(label)
  { }

 protected:
  int
  do_traverse(Traverse*)
  { return TRAVERSE_CONTINUE; }

  void
  do_check_types(Gogo*);

  bool
  do_may_fall_through() const
  { return false; }

  tree
  do_get_tree(Translate_context*);

 private:
  Label* label_;
};

// Check types for a label.  There aren't any types per se, but we use
// this to give an error if the label was never defined.

void
Goto_statement::do_check_types(Gogo*)
{
  if (!this->label_->is_defined())
    {
      error_at(this->location(), "reference to undefined label %qs",
	       this->label_->name().c_str());
      this->set_is_error();
    }
}

// Return the tree for the goto statement.

tree
Goto_statement::do_get_tree(Translate_context*)
{
  return this->build_stmt_1(GOTO_EXPR, this->label_->get_decl());
}

// Make a goto statement.

Statement*
Statement::make_goto_statement(Label* label, source_location location)
{
  return new Goto_statement(label, location);
}

// Class Label_statement.

// Traversal.

int
Label_statement::do_traverse(Traverse*)
{
  return TRAVERSE_CONTINUE;
}

// Return a tree defining this label.

tree
Label_statement::do_get_tree(Translate_context* context)
{
  tree ret = this->build_stmt_1(LABEL_EXPR, this->label_->get_decl());

  // If this function has a reference count queue, we flush it at each
  // label.  We need to do this in case the label is used to implement
  // a loop; otherwise some entries in the reference count queue may
  // get reused.
  Refcounts* refcounts = context->function()->func_value()->refcounts();
  if (refcounts != NULL && !refcounts->empty())
    {
      tree flush = refcounts->flush_queue(context->gogo(), false,
					  this->location());
      tree statements = NULL_TREE;
      append_to_statement_list(ret, &statements);
      append_to_statement_list(flush, &statements);
      ret = statements;
    }
  return ret;
}

// Make a label statement.

Statement*
Statement::make_label_statement(Label* label, source_location location)
{
  return new Label_statement(label, location);
}

// An if statement.

class If_statement : public Statement
{
 public:
  If_statement(Expression* cond, Block* then_block, Block* else_block,
	       source_location location)
    : Statement(STATEMENT_IF, location),
      cond_(cond), then_block_(then_block), else_block_(else_block)
  { }

 protected:
  int
  do_traverse(Traverse*);

  void
  do_determine_types();

  void
  do_check_types(Gogo*);

  bool
  do_may_fall_through() const;

  tree
  do_get_tree(Translate_context*);

 private:
  Expression* cond_;
  Block* then_block_;
  Block* else_block_;
};

// Traversal.

int
If_statement::do_traverse(Traverse* traverse)
{
  if (this->cond_ != NULL)
    {
      if (this->traverse_expression(traverse, &this->cond_) == TRAVERSE_EXIT)
	return TRAVERSE_EXIT;
    }
  if (this->then_block_->traverse(traverse) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  if (this->else_block_ != NULL)
    {
      if (this->else_block_->traverse(traverse) == TRAVERSE_EXIT)
	return TRAVERSE_EXIT;
    }
  return TRAVERSE_CONTINUE;
}

void
If_statement::do_determine_types()
{
  if (this->cond_ != NULL)
    {
      Type_context context(Type::lookup_bool_type(), false);
      this->cond_->determine_type(&context);
    }
  this->then_block_->determine_types();
  if (this->else_block_ != NULL)
    this->else_block_->determine_types();
}

// Check types.

void
If_statement::do_check_types(Gogo*)
{
  if (this->cond_ != NULL)
    {
      Type* type = this->cond_->type();
      if (type->is_error_type())
	this->set_is_error();
      else if (!type->is_boolean_type())
	this->report_error(_("if statement expects boolean expression"));
    }
}

// Whether the overall statement may fall through.

bool
If_statement::do_may_fall_through() const
{
  return (this->else_block_ == NULL
	  || this->then_block_->may_fall_through()
	  || this->else_block_->may_fall_through());
}

// Get tree.

tree
If_statement::do_get_tree(Translate_context* context)
{
  gcc_assert(this->cond_ == NULL || this->cond_->type()->is_boolean_type());
  tree ret = build3(COND_EXPR, void_type_node,
		    (this->cond_ == NULL
		     ? boolean_true_node
		     : this->cond_->get_tree(context)),
		    this->then_block_->get_tree(context),
		    (this->else_block_ == NULL
		     ? NULL_TREE
		     : this->else_block_->get_tree(context)));
  SET_EXPR_LOCATION(ret, this->location());
  return ret;
}

// Make an if statement.

Statement*
Statement::make_if_statement(Expression* cond, Block* then_block,
			     Block* else_block, source_location location)
{
  return new If_statement(cond, then_block, else_block, location);
}

// Class Case_clauses::Case_clause.

// Traversal.

int
Case_clauses::Case_clause::traverse(Traverse* traverse)
{
  if (this->cases_ != NULL
      && (traverse->traverse_mask() & Traverse::traverse_expressions) != 0)
    {
      if (this->cases_->traverse(traverse) == TRAVERSE_EXIT)
	return TRAVERSE_EXIT;
    }
  if (this->statements_ != NULL)
    {
      if (this->statements_->traverse(traverse) == TRAVERSE_EXIT)
	return TRAVERSE_EXIT;
    }
  return TRAVERSE_CONTINUE;
}

// Determine types.

void
Case_clauses::Case_clause::determine_types(Type* type)
{
  if (this->cases_ != NULL)
    {
      Type_context case_context(type, true);
      for (Expression_list::iterator p = this->cases_->begin();
	   p != this->cases_->end();
	   ++p)
	{
	  if (!(*p)->is_constant())
	    (*p)->determine_type_no_context();
	  else
	    (*p)->determine_type(&case_context);
	}
    }
  if (this->statements_ != NULL)
    this->statements_->determine_types();
}

// Check types.  Returns false if there was an error.

bool
Case_clauses::Case_clause::check_types(Gogo*, Type* type)
{
  if (this->cases_ != NULL)
    {
      for (Expression_list::iterator p = this->cases_->begin();
	   p != this->cases_->end();
	   ++p)
	{
	  if (!Type::are_compatible_for_binop(type, (*p)->type())
	      && ((*p)->is_constant() || !(*p)->type()->is_boolean_type()))
	    {
	      error_at((*p)->location(),
		       "type mismatch between switch value and case clause");
	      return false;
	    }
	}
    }
  return true;
}

// Check whether all the case expressions are integer constants.

bool
Case_clauses::Case_clause::is_constant() const
{
  if (this->cases_ != NULL)
    {
      for (Expression_list::const_iterator p = this->cases_->begin();
	   p != this->cases_->end();
	   ++p)
	if (!(*p)->is_constant()
	    || (*p)->type()->is_abstract()
	    || (*p)->type()->integer_type() == NULL)
	  return false;
    }
  return true;
}

// Return true if this clause may fall through to the statements
// following the switch statement.

bool
Case_clauses::Case_clause::may_fall_through() const
{
  if (this->statements_ == NULL)
    return true;
  return this->statements_->may_fall_through();
}

// Build up the body of a SWITCH_EXPR when all expressions are
// constants.  Return true if this is a default case.

void
Case_clauses::Case_clause::get_constant_tree(Translate_context* context,
					     tree break_label,
					     Case_constants* case_constants,
					     tree* stmt_list) const
{
  if (this->cases_ != NULL)
    {
      for (Expression_list::const_iterator p = this->cases_->begin();
	   p != this->cases_->end();
	   ++p)
	{
	  Type* itype;
	  mpz_t ival;
	  mpz_init(ival);
	  if (!(*p)->integer_constant_value(true, ival, &itype))
	    gcc_unreachable();
	  if (itype == NULL)
	    itype = Type::lookup_integer_type("int");
	  tree type_tree = itype->get_tree(context->gogo());
	  tree val = Expression::integer_constant_tree(ival, type_tree);
	  mpz_clear(ival);

	  if (val != error_mark_node)
	    {
	      gcc_assert(TREE_CODE(val) == INTEGER_CST);

	      std::pair<Case_constants::iterator, bool> ins =
		case_constants->insert(val);
	      if (!ins.second)
		{
		  // Value was already present.
		  warning_at(this->location_, 0,
			     "duplicate case value will never match");
		  continue;
		}

	      tree label = create_artificial_label(this->location_);
	      append_to_statement_list(build3(CASE_LABEL_EXPR, void_type_node,
					      val, NULL_TREE, label),
				       stmt_list);
	    }
	}
    }

  if (this->is_default_)
    {
      tree label = create_artificial_label(this->location_);
      append_to_statement_list(build3(CASE_LABEL_EXPR, void_type_node,
				      NULL_TREE, NULL_TREE, label),
			       stmt_list);
    }

  if (this->statements_ != NULL)
    {
      tree block_tree = this->statements_->get_tree(context);
      if (block_tree != error_mark_node)
	append_to_statement_list(block_tree, stmt_list);
    }

  if (!this->is_fallthrough_)
    {
      tree t = build_and_jump(&LABEL_EXPR_LABEL(break_label));
      SET_EXPR_LOCATION(t, this->location_);
      append_to_statement_list(t, stmt_list);
    }
}

// Build up a statement list when some case expressions are not
// constants.  If START_LABEL is not NULL, it goes at the start of the
// statements, after the condition test.  We branch to FINISH_LABEL at
// the end.

void
Case_clauses::Case_clause::get_nonconstant_tree(Translate_context* context,
						Type* switch_val_type,
						tree switch_val_tree,
						tree start_label,
						tree finish_label,
						tree* stmt_list) const
{
  tree next_case_label = NULL_TREE;
  if (this->cases_ != NULL && this->cases_->size() > 0)
    {
      next_case_label = build1(LABEL_EXPR, void_type_node, NULL_TREE);
      SET_EXPR_LOCATION(next_case_label, this->location_);

      if (this->cases_->size() == 1)
	{
	  // For a single case CASE_VAL, we generate
	  //   if SWITCH_VAL != CASE_VAL {
	  //     goto next_case
	  //   }
	  tree jump = build_and_jump(&LABEL_EXPR_LABEL(next_case_label));
	  SET_EXPR_LOCATION(jump, this->location_);

	  Expression* case_expr = *this->cases_->begin();
	  tree case_val = case_expr->get_tree(context);
	  if (case_val == error_mark_node)
	    return;

	  tree cond_expr;
	  if (!case_expr->is_constant()
	      && case_expr->type()->is_boolean_type())
	    cond_expr = build3(COND_EXPR, void_type_node,
			       case_val, NULL_TREE, jump);
	  else
	    {
	      tree comparison = Expression::comparison_tree(context,
							    OPERATOR_NOTEQ,
							    switch_val_type,
							    switch_val_tree,
							    case_expr->type(),
							    case_val,
							    this->location_);
	      cond_expr = build3(COND_EXPR, void_type_node,
				 comparison, jump, NULL_TREE);
	    }
	  SET_EXPR_LOCATION(cond_expr, this->location_);
	  append_to_statement_list(cond_expr, stmt_list);
	}
      else
	{
	  // For each CASE_VAL, we generate
	  //   if SWITCH_VAL == CASE_VAL {
	  //     goto this_case
	  //   }
	  // and then we emit
	  //   goto next_case;
	  //  this_case:
	  tree this_case_label = build1(LABEL_EXPR, void_type_node, NULL_TREE);
	  SET_EXPR_LOCATION(this_case_label, this->location_);

	  for (Expression_list::const_iterator p = this->cases_->begin();
	       p != this->cases_->end();
	       ++p)
	    {
	      tree jump = build_and_jump(&LABEL_EXPR_LABEL(this_case_label));
	      SET_EXPR_LOCATION(jump, this->location_);

	      tree case_val = (*p)->get_tree(context);
	      if (case_val == error_mark_node)
		return;

	      tree cond_expr;
	      if (!(*p)->is_constant() && (*p)->type()->is_boolean_type())
		cond_expr = build3(COND_EXPR, void_type_node,
				   case_val, jump, NULL_TREE);
	      else
		{
		  tree comparison =
		    Expression::comparison_tree(context, OPERATOR_EQEQ,
						switch_val_type,
						switch_val_tree,
						(*p)->type(),
						case_val,
						this->location_);
		  cond_expr = build3(COND_EXPR, void_type_node,
				     comparison, jump, NULL_TREE);
		}
	      SET_EXPR_LOCATION(cond_expr, this->location_);
	      append_to_statement_list(cond_expr, stmt_list);
	    }

	  tree next_jump = build_and_jump(&LABEL_EXPR_LABEL(next_case_label));
	  SET_EXPR_LOCATION(next_jump, this->location_);
	  append_to_statement_list(next_jump, stmt_list);

	  append_to_statement_list(this_case_label, stmt_list);
	}
    }

  if (start_label != NULL_TREE)
    {
      append_to_statement_list(start_label, stmt_list);
      if (this->statements_ != NULL)
	SET_EXPR_LOCATION(start_label, this->statements_->start_location());
    }

  if (this->statements_ != NULL)
    {
      tree block_tree = this->statements_->get_tree(context);
      if (block_tree != error_mark_node)
	append_to_statement_list(block_tree, stmt_list);
    }

  tree t = build_and_jump(&LABEL_EXPR_LABEL(finish_label));
  if (this->statements_ == NULL)
    SET_EXPR_LOCATION(t, this->location_);
  else
    SET_EXPR_LOCATION(t, this->statements_->end_location());
  append_to_statement_list(t, stmt_list);

  if (next_case_label != NULL)
    append_to_statement_list(next_case_label, stmt_list);
}

// Class Case_clauses.

// Traversal.

int
Case_clauses::traverse(Traverse* traverse)
{
  for (Clauses::iterator p = this->clauses_.begin();
       p != this->clauses_.end();
       ++p)
    {
      if (p->traverse(traverse) == TRAVERSE_EXIT)
	return TRAVERSE_EXIT;
    }
  return TRAVERSE_CONTINUE;
}

// Determine types.

void
Case_clauses::determine_types(Type* type)
{
  for (Clauses::iterator p = this->clauses_.begin();
       p != this->clauses_.end();
       ++p)
    p->determine_types(type);
}

// Check types.  Returns false if there was an error.

bool
Case_clauses::check_types(Gogo* gogo, Type* type)
{
  bool ret = true;
  for (Clauses::iterator p = this->clauses_.begin();
       p != this->clauses_.end();
       ++p)
    {
      if (!p->check_types(gogo, type))
	ret = false;
    }
  return ret;
}

// Check whether all the case expressions are constant.

bool
Case_clauses::is_constant() const
{
  for (Clauses::const_iterator p = this->clauses_.begin();
       p != this->clauses_.end();
       ++p)
    if (!p->is_constant())
      return false;
  return true;
}

// Return true if these clauses may fall through to the statements
// following the switch statement.

bool
Case_clauses::may_fall_through() const
{
  bool found_default = false;
  for (Clauses::const_iterator p = this->clauses_.begin();
       p != this->clauses_.end();
       ++p)
    {
      if (p->may_fall_through() && !p->is_fallthrough())
	return true;
      if (p->is_default())
	found_default = true;
    }
  return !found_default;
}

// Return a tree when all case expressions are constants.

tree
Case_clauses::get_constant_tree(Translate_context* context,
				tree break_label) const
{
  Case_constants case_constants;
  tree stmt_list = NULL_TREE;
  for (Clauses::const_iterator p = this->clauses_.begin();
       p != this->clauses_.end();
       ++p)
    p->get_constant_tree(context, break_label, &case_constants,
			 &stmt_list);
  return stmt_list;
}

// Build up a statement list when some case expressions are not
// constants.

void
Case_clauses::get_nonconstant_tree(Translate_context* context,
				   Type* switch_val_type, tree switch_val_tree,
				   tree break_label, tree* stmt_list) const
{
  const Case_clause* default_case = NULL;
  tree last_fallthrough_label = NULL_TREE;
  tree default_start_label = NULL_TREE;
  tree default_finish_label = NULL_TREE;
  for (Clauses::const_iterator p = this->clauses_.begin();
       p != this->clauses_.end();
       ++p)
    {
      tree start_label = last_fallthrough_label;
      tree finish_label = break_label;

      last_fallthrough_label = NULL_TREE;
      if (p->is_fallthrough() && p + 1 != this->clauses_.end())
	{
	  finish_label = build1(LABEL_EXPR, void_type_node, NULL_TREE);
	  last_fallthrough_label = finish_label;
	}

      if (!p->is_default())
	p->get_nonconstant_tree(context, switch_val_type, switch_val_tree,
				start_label, finish_label, stmt_list);
      else
	{
	  // We are translating this switch into a series of tests,
	  // which means that we need to move the default case to the
	  // end.
	  default_case = &*p;
	  default_start_label = start_label;
	  default_finish_label = finish_label;
	}
    }

  if (default_case != NULL)
    default_case->get_nonconstant_tree(context, switch_val_type,
				       switch_val_tree, default_start_label,
				       default_finish_label, stmt_list);
}

// Class Switch_statement.

// Traversal.

int
Switch_statement::do_traverse(Traverse* traverse)
{
  if (this->val_ != NULL)
    {
      if (this->traverse_expression(traverse, &this->val_) == TRAVERSE_EXIT)
	return TRAVERSE_EXIT;
    }
  return this->clauses_->traverse(traverse);
}

// Determine types.

void
Switch_statement::do_determine_types()
{
  Type* type;
  if (this->val_ == NULL)
    type = Type::make_boolean_type();
  else
    {
      this->val_->determine_type_no_context();
      type = this->val_->type();
    }
  this->clauses_->determine_types(type);
}

// Check types.

void
Switch_statement::do_check_types(Gogo* gogo)
{
  Type* type;
  if (this->val_ == NULL)
    type = Type::make_boolean_type();
  else
    type = this->val_->type();
  if (!this->clauses_->check_types(gogo, type))
    this->set_is_error();
}

// Return whether this switch may fall through.

bool
Switch_statement::do_may_fall_through() const
{
  if (this->clauses_ == NULL)
    return true;
  return this->clauses_->may_fall_through();
}

// Convert to GENERIC.

tree
Switch_statement::do_get_tree(Translate_context* context)
{
  tree switch_val_tree;
  if (this->val_ == NULL)
    switch_val_tree = boolean_true_node;
  else
    switch_val_tree = this->val_->get_tree(context);

  if (this->clauses_->empty())
    return switch_val_tree;

  tree stmt_list = NULL_TREE;
  if (this->val_ != NULL
      && this->val_->type()->integer_type() != NULL
      && this->clauses_->is_constant())
    {
      tree ret = build3(SWITCH_EXPR, TREE_TYPE(switch_val_tree),
			switch_val_tree,
			this->clauses_->get_constant_tree(context,
							  this->break_label()),
			NULL_TREE);
      SET_EXPR_LOCATION(ret, this->location());
      append_to_statement_list(ret, &stmt_list);
    }
  else
    {
      switch_val_tree = save_expr(switch_val_tree);
      this->clauses_->get_nonconstant_tree(context,
					   (this->val_ == NULL
					    ? Type::make_boolean_type()
					    : this->val_->type()),
					   switch_val_tree,
					   this->break_label(),
					   &stmt_list);
    }

  if (LABEL_EXPR_LABEL(this->break_label_) != NULL_TREE)
    append_to_statement_list(this->break_label_, &stmt_list);

  return stmt_list;
}

// Return the break label for this switch statement, creating it if
// necessary.

tree
Switch_statement::break_label()
{
  if (this->break_label_ == NULL_TREE)
    {
      this->break_label_ = build1(LABEL_EXPR, void_type_node, NULL_TREE);
      SET_EXPR_LOCATION(this->break_label_, this->location());
    }
  return this->break_label_;
}

// Make a switch statement.

Switch_statement*
Statement::make_switch_statement(Expression* val, source_location location)
{
  return new Switch_statement(val, location);
}

// Class Type_case_clauses::Type_case_clause.

// Traversal.

int
Type_case_clauses::Type_case_clause::traverse(Traverse* traverse)
{
  if (!this->is_default_
      && ((traverse->traverse_mask()
	   & (Traverse::traverse_types | Traverse::traverse_expressions)) != 0)
      && Type::traverse(this->type_, traverse) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  if (this->statements_ != NULL)
    return this->statements_->traverse(traverse);
  return TRAVERSE_CONTINUE;
}

// Determine types.

void
Type_case_clauses::Type_case_clause::determine_types()
{
  // The language permits case nil, which is of course a constant
  // rather than a type.  It will appear here as an invalid forwarding
  // type.
  if (this->type_ != NULL && this->type_->is_nil_constant_as_type())
    this->type_ = Type::make_nil_type();

  if (this->statements_ != NULL)
    this->statements_->determine_types();
}

// Return true if this clause may fall through to the statements
// following the overall type switch statement.

bool
Type_case_clauses::Type_case_clause::may_fall_through() const
{
  if (this->statements_ == NULL)
    return true;
  return this->statements_->may_fall_through();
}

// Add code for this clause to the statement list.

void
Type_case_clauses::Type_case_clause::get_tree(Translate_context* context,
					      tree switch_type_descriptor,
					      tree break_label,
					      tree* stmts_label,
					      tree* stmt_list) const
{
  Gogo* gogo = context->gogo();
  tree next_case_label = NULL_TREE;
  if (!this->is_default_)
    {
      // For the normal non-fallthrough case we generate
      //   if (! <types match>) goto next_case;
      //   statements;
      //   goto break_label;
      //  next_case:
      // For the fallthrough case we generate
      //   if (!! <types match>) goto stmts_label;

      tree jump;
      if (!this->is_fallthrough_)
	{
	  next_case_label = build1(LABEL_EXPR, void_type_node, NULL_TREE);
	  jump = build_and_jump(&LABEL_EXPR_LABEL(next_case_label));
	}
      else
	{
	  if (*stmts_label == NULL_TREE)
	    *stmts_label = build1(LABEL_EXPR, void_type_node, NULL_TREE);
	  jump = build_and_jump(&LABEL_EXPR_LABEL(*stmts_label));
	}


      tree comparison;
      Type* type = this->type_;
      if (type->is_nil_type())
	{
	  comparison = build2(EQ_EXPR, boolean_type_node,
			      switch_type_descriptor,
			      fold_convert(TREE_TYPE(switch_type_descriptor),
					   null_pointer_node));
	}
      else if (type->interface_type() == NULL)
	{
	  tree this_type_descriptor = type->type_descriptor(gogo);
	  static tree type_descriptors_equal_fndecl;
	  comparison = Gogo::call_builtin(&type_descriptors_equal_fndecl,
					  this->location(),
					  "__go_type_descriptors_equal",
					  2,
					  boolean_type_node,
					  TREE_TYPE(switch_type_descriptor),
					  switch_type_descriptor,
					  TREE_TYPE(this_type_descriptor),
					  this_type_descriptor);
	}
      else
	{
	  tree this_type_descriptor = type->type_descriptor(gogo);
	  static tree can_convert_to_interface_fndecl;
	  comparison = Gogo::call_builtin(&can_convert_to_interface_fndecl,
					  this->location(),
					  "__go_can_convert_to_interface",
					  2,
					  boolean_type_node,
					  TREE_TYPE(this_type_descriptor),
					  this_type_descriptor,
					  TREE_TYPE(switch_type_descriptor),
					  switch_type_descriptor);
	}

      if (this->is_fallthrough_)
	comparison = fold_build1(TRUTH_NOT_EXPR, boolean_type_node,
				 comparison);

      tree cond_expr = build3(COND_EXPR, void_type_node, comparison,
			      NULL_TREE, jump);
      append_to_statement_list(cond_expr, stmt_list);
    }

  if (this->statements_ != NULL)
    {
      gcc_assert(!this->is_fallthrough_);
      if (*stmts_label != NULL_TREE)
	{
	  gcc_assert(!this->is_default_);
	  append_to_statement_list(*stmts_label, stmt_list);
	  *stmts_label = NULL_TREE;
	}
      tree block_tree = this->statements_->get_tree(context);
      if (block_tree != error_mark_node)
	append_to_statement_list(block_tree, stmt_list);
    }

  if (this->is_fallthrough_)
    gcc_assert(next_case_label == NULL_TREE);
  else
    {
      tree t = build_and_jump(&LABEL_EXPR_LABEL(break_label));
      append_to_statement_list(t, stmt_list);

      if (next_case_label != NULL_TREE)
	append_to_statement_list(next_case_label, stmt_list);
    }
}

// Class Type_case_clauses.

// Traversal.

int
Type_case_clauses::traverse(Traverse* traverse)
{
  for (Type_clauses::iterator p = this->clauses_.begin();
       p != this->clauses_.end();
       ++p)
    {
      if (p->traverse(traverse) == TRAVERSE_EXIT)
	return TRAVERSE_EXIT;
    }
  return TRAVERSE_CONTINUE;
}

// Determine types.

void
Type_case_clauses::determine_types()
{
  for (Type_clauses::iterator p = this->clauses_.begin();
       p != this->clauses_.end();
       ++p)
    p->determine_types();
}

// Check for duplicate types.

void
Type_case_clauses::check_duplicates() const
{
  typedef std::tr1::unordered_set<const Type*, Type_hash,
				  Type_identical> Types_seen;
  Types_seen types_seen;
  for (Type_clauses::const_iterator p = this->clauses_.begin();
       p != this->clauses_.end();
       ++p)
    {
      Type* t = p->type();
      if (t == NULL)
	continue;
      std::pair<Types_seen::iterator, bool> ins = types_seen.insert(t);
      if (!ins.second)
	error_at(p->location(), "duplicate type in switch");
    }
}

// Return true if these clauses may fall through to the statements
// following the type switch statement.

bool
Type_case_clauses::may_fall_through() const
{
  bool found_default = false;
  for (Type_clauses::const_iterator p = this->clauses_.begin();
       p != this->clauses_.end();
       ++p)
    {
      if (p->may_fall_through())
	return true;
      if (p->is_default())
	found_default = true;
    }
  return !found_default;
}

// Build up a statement list for the type switch.

void
Type_case_clauses::get_tree(Translate_context* context,
			    tree switch_type_descriptor,
			    tree break_label,
			    tree* stmt_list) const
{
  const Type_case_clause* default_case = NULL;

  tree stmts_label = NULL_TREE;
  for (Type_clauses::const_iterator p = this->clauses_.begin();
       p != this->clauses_.end();
       ++p)
    {
      if (!p->is_default())
	p->get_tree(context, switch_type_descriptor, break_label,
		    &stmts_label, stmt_list);
      else
	{
	  // We are generating a series of tests, which means that we
	  // need to move the default case to the end.
	  default_case = &*p;
	}
    }

  if (default_case != NULL)
    default_case->get_tree(context, switch_type_descriptor, break_label,
			   &stmts_label, stmt_list);

  gcc_assert(stmts_label == NULL_TREE);
}

// Class Type_switch_statement.

// Traversal.

int
Type_switch_statement::do_traverse(Traverse* traverse)
{
  if (this->var_ == NULL)
    {
      if (this->traverse_expression(traverse, &this->expr_) == TRAVERSE_EXIT)
	return TRAVERSE_EXIT;
    }
  if (this->clauses_ != NULL)
    return this->clauses_->traverse(traverse);
  return TRAVERSE_CONTINUE;
}

// Determine types.

void
Type_switch_statement::do_determine_types()
{
  if (this->var_ == NULL)
    this->expr_->determine_type_no_context();
  if (this->clauses_ != NULL)
    this->clauses_->determine_types();
}

// Check types.  There can't be any type errors here, but we check for
// duplicate cases.

void
Type_switch_statement::do_check_types(Gogo*)
{
  if (this->clauses_ != NULL)
    this->clauses_->check_duplicates();
}

// Return whether this type switch may fall through.

bool
Type_switch_statement::do_may_fall_through() const
{
  if (this->clauses_ == NULL)
    return true;
  return this->clauses_->may_fall_through();
}

// Convert to GENERIC.  We can have multiple type descriptors for the
// same type, so we generate a series of type descriptor comparisons.
// FIXME: If we had a unique hash code which only depended on the
// reflection string, then we could do a constant switch which would
// be faster.  I'm not sure how much different it makes--I don't know
// how big these type switches will get.

tree
Type_switch_statement::do_get_tree(Translate_context* context)
{
  tree switch_val_tree;
  if (this->var_ != NULL)
    switch_val_tree = this->var_->get_tree(context->gogo(),
					   context->function());
  else
    switch_val_tree = this->expr_->get_tree(context);

  if (this->clauses_->empty())
    {
      // Just evaluate for side-effects.
      return switch_val_tree;
    }

  Type* switch_var_type;
  if (this->var_ != NULL)
    switch_var_type = this->var_->var_value()->type();
  else
    switch_var_type = this->expr_->type();
  tree switch_descriptor = this->get_type_descriptor(context,
						     switch_var_type,
						     switch_val_tree);

  tree stmt_list = NULL_TREE;
  this->clauses_->get_tree(context, switch_descriptor, this->break_label(),
			   &stmt_list);

  if (LABEL_EXPR_LABEL(this->break_label_) != NULL_TREE)
    append_to_statement_list(this->break_label_, &stmt_list);

  return stmt_list;
}

// Return a tree for the type descriptor of a type switch.
// SWITCH_VAR_TYPE is the Go type, which is probably an interface
// type.  SWITCH_VAL_TREE is the value.

tree
Type_switch_statement::get_type_descriptor(Translate_context* context,
					   Type* switch_var_type,
					   tree switch_val_tree)
{
  Interface_type* interface_type = switch_var_type->interface_type();
  if (interface_type == NULL)
    {
      // This is a type switch using a value which does not have
      // interface type.  Perhaps we should issue a warning.
      return switch_var_type->type_descriptor(context->gogo());
    }

  // Pull the type descriptor out of SWITCH_VAL_TREE.
  switch_val_tree = save_expr(switch_val_tree);
  gcc_assert(POINTER_TYPE_P(TREE_TYPE(switch_val_tree)));
  tree struct_type = TREE_TYPE(TREE_TYPE(switch_val_tree));
  gcc_assert(TREE_CODE(struct_type) == RECORD_TYPE);
  tree field = TYPE_FIELDS(struct_type);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)),
		    "__type_descriptor") == 0);
  tree ret = build3(COMPONENT_REF, TREE_TYPE(field),
		    build_fold_indirect_ref(switch_val_tree),
		    field, NULL_TREE);
  ret = build3(COND_EXPR, TREE_TYPE(ret),
	       build2(EQ_EXPR, boolean_type_node, switch_val_tree,
		      fold_convert(TREE_TYPE(switch_val_tree),
				   null_pointer_node)),
	       fold_convert(TREE_TYPE(ret), null_pointer_node),
	       ret);
  return save_expr(ret);
}

// Return the break label for this type switch statement, creating it
// if necessary.

tree
Type_switch_statement::break_label()
{
  if (this->break_label_ == NULL_TREE)
    {
      this->break_label_ = build1(LABEL_EXPR, void_type_node, NULL_TREE);
      SET_EXPR_LOCATION(this->break_label_, this->location());
    }
  return this->break_label_;
}

// Make a type switch statement.

Type_switch_statement*
Statement::make_type_switch_statement(Named_object* var, Expression* expr,
				      source_location location)
{
  return new Type_switch_statement(var, expr, location);
}

// Class Select_clauses::Select_clause.

// Traversal.

int
Select_clauses::Select_clause::traverse(Traverse* traverse)
{
  if ((traverse->traverse_mask() & Traverse::traverse_expressions) != 0)
    {
      if (this->channel_ != NULL)
	{
	  if (Expression::traverse(&this->channel_, traverse) == TRAVERSE_EXIT)
	    return TRAVERSE_EXIT;
	}
      if (this->val_ != NULL)
	{
	  if (Expression::traverse(&this->val_, traverse) == TRAVERSE_EXIT)
	    return TRAVERSE_EXIT;
	}
    }
  if (this->statements_ != NULL)
    {
      if (this->statements_->traverse(traverse) == TRAVERSE_EXIT)
	return TRAVERSE_EXIT;
    }
  return TRAVERSE_CONTINUE;
}

// Traverse assignments.  Any value sent on a channel is effectively
// an RHS.

void
Select_clauses::Select_clause::traverse_assignments(
    Traverse_assignments* tassign)
{
  if (!this->is_default_)
    {
      if (this->is_send_)
	tassign->value(&this->val_, true, false);
      else
	{
	  if (this->val_ != NULL)
	    tassign->assignment(&this->val_, NULL);
	  else if (this->var_ != NULL)
	    tassign->initialize_variable(this->var_);
	  // FIXME: Otherwise we may need to decrement the reference
	  // count.
	}
      tassign->value(&this->channel_, false, false);
    }
}

// Determine types.

void
Select_clauses::Select_clause::determine_types()
{
  if (this->channel_ != NULL)
    this->channel_->determine_type_no_context();
  if (this->val_ != NULL)
    {
      Channel_type* channel_type = this->channel_->type()->channel_type();
      Type_context subcontext((channel_type == NULL
			       ? NULL
			       : channel_type->element_type()),
			      false);
      this->val_->determine_type(&subcontext);
    }
  if (this->statements_ != NULL)
    this->statements_->determine_types();
}

// Check types.  Returns false if there was an error.

bool
Select_clauses::Select_clause::check_types(Gogo*)
{
  if (this->is_default_)
    return true;
  Channel_type* channel_type = this->channel_->type()->channel_type();
  if (channel_type == NULL)
    {
      error_at(this->location_, "expected channel");
      return false;
    }
  else if (this->is_send_ && !channel_type->may_send())
    {
      error_at(this->location_, "invalid send on receive-only channel");
      return false;
    }
  else if (!this->is_send_ && !channel_type->may_receive())
    {
      error_at(this->location_, "invalid receive on send-only channel");
      return false;
    }
  else if (this->val_ != NULL)
    {
      Type* element_type = channel_type->element_type();
      Type* val_type = this->val_->type();
      if (this->is_send_
	  ? !Type::are_compatible_for_assign(element_type, val_type, NULL)
	  : !Type::are_compatible_for_assign(val_type, element_type, NULL))
	{
	  error_at(this->location_, "incompatible types");
	  return false;
	}
    }
  return true;
}

// Whether this clause may fall through to the statement which follows
// the overall select statement.

bool
Select_clauses::Select_clause::may_fall_through() const
{
  if (this->statements_ == NULL)
    return true;
  return this->statements_->may_fall_through();
}

// Return a tree for the statements to execute.

tree
Select_clauses::Select_clause::get_statements_tree(Translate_context* context)
{
  if (this->statements_ == NULL)
    return NULL_TREE;
  return this->statements_->get_tree(context);
}

// Class Select_clauses.

// Traversal.

int
Select_clauses::traverse(Traverse* traverse)
{
  for (Clauses::iterator p = this->clauses_.begin();
       p != this->clauses_.end();
       ++p)
    {
      if (p->traverse(traverse) == TRAVERSE_EXIT)
	return TRAVERSE_EXIT;
    }
  return TRAVERSE_CONTINUE;
}

// Traversal of assignments.

void
Select_clauses::traverse_assignments(Traverse_assignments* tassign)
{
  for (Clauses::iterator p = this->clauses_.begin();
       p != this->clauses_.end();
       ++p)
    p->traverse_assignments(tassign);
}

// Determine types.

void
Select_clauses::determine_types()
{
  for (Clauses::iterator p = this->clauses_.begin();
       p != this->clauses_.end();
       ++p)
    p->determine_types();
}

// Check types.  Returns false if there was an error.

bool
Select_clauses::check_types(Gogo* gogo)
{
  bool ret = true;
  for (Clauses::iterator p = this->clauses_.begin();
       p != this->clauses_.end();
       ++p)
    {
      if (!p->check_types(gogo))
	ret = false;
    }
  return ret;
}

// Return whether these select clauses fall through to the statement
// following the overall select statement.

bool
Select_clauses::may_fall_through() const
{
  for (Clauses::const_iterator p = this->clauses_.begin();
       p != this->clauses_.end();
       ++p)
    if (p->may_fall_through())
      return true;
  return false;
}

// Return a tree.  We build a call to
//   size_t __go_select(size_t count, _Bool has_default, ...);
// There are COUNT pairs of arguments: a pointer to a channel followed
// by a boolean: true for send, false for receive.  __go_select
// returns an integer from 0 to count, inclusive.  A return of 0 means
// that the default case should be run; this only happens if
// HAS_DEFAULT is non-zero.  Otherwise the number indicates the case
// to run.

// FIXME: This doesn't handle channels which send interface types
// where the receiver has a static type which matches that interface.

tree
Select_clauses::get_tree(Translate_context* context, tree break_label,
			 source_location location)
{
  if (this->clauses_.empty())
    return integer_zero_node;

  static tree select_fndecl;
  if (select_fndecl == NULL_TREE)
    {
      tree fnid = get_identifier("__go_select");
      tree argtypes = tree_cons(NULL_TREE, sizetype,
				tree_cons(NULL_TREE, boolean_type_node,
					  NULL_TREE));
      tree fntype = build_function_type(sizetype, argtypes);
      select_fndecl = build_decl(BUILTINS_LOCATION, FUNCTION_DECL, fnid,
				 fntype);
      Gogo::mark_fndecl_as_builtin_library(select_fndecl);
      go_preserve_from_gc(select_fndecl);
    }

  int nargs = 2 + this->clauses_.size() * 2;
  tree* args = new tree[nargs];

  Select_clause* default_clause = NULL;
  for (Clauses::iterator p = this->clauses_.begin();
       p != this->clauses_.end();
       ++p)
    {
      if (p->is_default())
	{
	  default_clause = &*p;
	  break;
	}
    }
  if (default_clause == NULL)
    args[1] = boolean_false_node;
  else
    {
      args[1] = boolean_true_node;
      nargs -= 2;
    }

  args[0] = size_int(nargs / 2 - 1);

  std::vector<tree> channels;
  std::vector<tree> sendvars;
  std::vector<Type*> element_types;
  channels.reserve(this->clauses_.size());
  sendvars.reserve(this->clauses_.size());
  element_types.reserve(this->clauses_.size());

  tree final_stmt_list = NULL_TREE;

  int i = 2;
  for (Clauses::iterator p = this->clauses_.begin();
       p != this->clauses_.end();
       ++p)
    {
      if (!p->is_default())
	{
	  Expression* channel = p->channel();
	  Channel_type* ctype = channel->type()->channel_type();
	  gcc_assert(ctype != NULL);
	  element_types.push_back(ctype->element_type());

	  tree channel_tree = channel->get_tree(context);
	  if (channel_tree == error_mark_node)
	    return error_mark_node;

	  channel_tree = save_expr(channel_tree);
	  channels.push_back(channel_tree);

	  if (!p->is_send())
	    sendvars.push_back(NULL_TREE);
	  else
	    {
	      tree val = p->val()->get_tree(context);
	      tree sendvar = create_tmp_var(TREE_TYPE(val), get_name(val));
	      DECL_IGNORED_P(sendvar) = 0;
	      DECL_INITIAL(sendvar) = val;
	      tree decl_expr = build1(DECL_EXPR, void_type_node, sendvar);
	      SET_EXPR_LOCATION(decl_expr, location);
	      append_to_statement_list(decl_expr, &final_stmt_list);
	      sendvars.push_back(sendvar);
	    }

	  args[i] = channel_tree;
	  args[i + 1] = build_int_cst(integer_type_node, p->is_send() ? 1 : 0);
	  i += 2;
	}
    }
  gcc_assert(i == nargs);

  tree fnptr = build_fold_addr_expr(select_fndecl);
  tree call = build_call_array(sizetype, fnptr, nargs, args);
  delete[] args;

  SET_EXPR_LOCATION(call, location);

  tree stmt_list = NULL_TREE;

  if (default_clause != NULL)
    this->add_clause_tree(context, 0, default_clause, NULL_TREE,
			  NULL, NULL_TREE, break_label, &stmt_list);

  i = 1;
  for (Clauses::iterator p = this->clauses_.begin();
       p != this->clauses_.end();
       ++p)
    {
      if (!p->is_default())
	{
	  this->add_clause_tree(context, i, &*p, channels[i - 1],
				element_types[i - 1], sendvars[i - 1],
				break_label, &stmt_list);
	  ++i;
	}
    }

  append_to_statement_list(break_label, &stmt_list);

  tree switch_stmt = build3(SWITCH_EXPR, sizetype, call, stmt_list, NULL_TREE);
  SET_EXPR_LOCATION(switch_stmt, location);
  append_to_statement_list(switch_stmt, &final_stmt_list);

  return final_stmt_list;
}

// Add the tree for CLAUSE to STMT_LIST.

void
Select_clauses::add_clause_tree(Translate_context* context, int case_index,
				Select_clause* clause, tree channel,
				Type* element_type, tree sendvar,
				tree bottom_label, tree* stmt_list)
{
  tree label = create_artificial_label(clause->location());
  append_to_statement_list(build3(CASE_LABEL_EXPR, void_type_node,
				  build_int_cst(sizetype, case_index),
				  NULL_TREE, label),
			   stmt_list);
  tree rec = NULL_TREE;
  if (!clause->is_default())
    {
      if (clause->is_send())
	{
	  gcc_assert(sendvar != NULL_TREE);
	  sendvar = Expression::convert_for_assignment(context,
						       element_type,
						       clause->val()->type(),
						       sendvar,
						       clause->location());
	  tree send = Gogo::send_on_channel(channel, sendvar, true, true,
					    clause->location());
	  append_to_statement_list(send, stmt_list);
	}
      else
	{
	  gcc_assert(sendvar == NULL_TREE);
	  tree element_type_tree = element_type->get_tree(context->gogo());
	  gcc_assert(element_type_tree != error_mark_node);
	  rec = Gogo::receive_from_channel(element_type_tree, channel,
					   true, true, clause->location());
	  Expression* val = clause->val();
	  if (val != NULL)
	    {
	      tree assign =
		Assignment_statement::get_assignment_tree(context, OPERATOR_EQ,
							  val, NULL_TREE,
							  NULL, element_type,
							  rec,
							  clause->location());
	      append_to_statement_list(assign, stmt_list);
	    }
	  else if (clause->var() == NULL)
	    append_to_statement_list(rec, stmt_list);
	}
    }

  tree statements_tree = clause->get_statements_tree(context);

  if (clause->var() != NULL)
    {
      // This is the case where the receive clause defines a variable.
      // We expect to see a BIND_EXPR.  The first statement within the
      // BIND_EXPR should be a DECL_EXPR for the variable we defining.
      // We set the initialization of the variable to the value
      // received.
      std::string s = clause->var()->name();
      s = Gogo::unpack_hidden_name(s);

      gcc_assert(rec != NULL_TREE);
      gcc_assert(TREE_CODE(statements_tree) == BIND_EXPR);

      tree body = BIND_EXPR_BODY(statements_tree);
      if (TREE_CODE(body) == TRY_FINALLY_EXPR)
	body = TREE_OPERAND(body, 0);

      gcc_assert(TREE_CODE(body) == STATEMENT_LIST);

      tree_stmt_iterator p = tsi_start(body);
      tree vd = tsi_stmt(p);

      if (TREE_CODE(vd) == DECL_EXPR)
	{
	  tree decl = DECL_EXPR_DECL(vd);
	  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(decl)),
			    s.c_str()) == 0);
	  DECL_INITIAL(decl) = rec;
	}
      else if (TREE_CODE(vd) == COMPOUND_EXPR)
	{
	  // The variable is on the heap.
	  gcc_assert(TREE_CODE(TREE_OPERAND(vd, 0)) == DECL_EXPR);
	  gcc_assert(TREE_CODE(TREE_OPERAND(vd, 1)) == MODIFY_EXPR);
	  tree decl = DECL_EXPR_DECL(TREE_OPERAND(vd, 0));
	  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(decl)),
			    s.c_str()) == 0);
	  TREE_OPERAND(TREE_OPERAND(vd, 1), 1) = rec;
	}
      else
	gcc_unreachable();
    }

  append_to_statement_list(statements_tree, stmt_list);
  append_to_statement_list(build_and_jump(&LABEL_EXPR_LABEL(bottom_label)),
			   stmt_list);
}

// Class Select_statement.

// Check types for a select statement.

void
Select_statement::do_check_types(Gogo* gogo)
{
  if (!this->clauses_->check_types(gogo))
    this->set_is_error();
}

// Return the break label for this switch statement, creating it if
// necessary.

tree
Select_statement::break_label()
{
  if (this->break_label_ == NULL_TREE)
    {
      this->break_label_ = build1(LABEL_EXPR, void_type_node, NULL_TREE);
      SET_EXPR_LOCATION(this->break_label_, this->location());
    }
  return this->break_label_;
}

// Traverse assignments in a select statement.

bool
Select_statement::do_traverse_assignments(Traverse_assignments* tassign)
{
  this->clauses_->traverse_assignments(tassign);
  return true;
}

// Return the tree for a select statement.

tree
Select_statement::do_get_tree(Translate_context* context)
{
  return this->clauses_->get_tree(context, this->break_label(),
				  this->location());
}

// Make a select statement.

Select_statement*
Statement::make_select_statement(source_location location)
{
  return new Select_statement(location);
}

// Class For_statement.

// Traversal.

int
For_statement::do_traverse(Traverse* traverse)
{
  if (this->init_ != NULL)
    {
      if (this->init_->traverse(traverse) == TRAVERSE_EXIT)
	return TRAVERSE_EXIT;
    }
  if (this->cond_ != NULL)
    {
      if (this->traverse_expression(traverse, &this->cond_) == TRAVERSE_EXIT)
	return TRAVERSE_EXIT;
    }
  if (this->post_ != NULL)
    {
      if (this->post_->traverse(traverse) == TRAVERSE_EXIT)
	return TRAVERSE_EXIT;
    }
  return this->statements_->traverse(traverse);
}

void
For_statement::do_determine_types()
{
  if (this->init_ != NULL)
    this->init_->determine_types();
  if (this->cond_ != NULL)
    {
      Type_context context(Type::lookup_bool_type(), false);
      this->cond_->determine_type(&context);
    }
  if (this->post_ != NULL)
    this->post_->determine_types();
  this->statements_->determine_types();
}

// Check types.

void
For_statement::do_check_types(Gogo*)
{
  if (this->cond_ != NULL
      && !this->cond_->type()->is_boolean_type()
      && !this->cond_->type()->is_error_type())
    this->report_error(_("for statement expects boolean expression "
			 "for condition"));
}

// Traversal class used to look for a break out of a for.

class Find_break_traverse : public Traverse
{
 public:
  Find_break_traverse()
    : Traverse(traverse_statements),
      found_break_or_goto_(false)
  { }

  int
  statement(Block*, size_t* pindex, Statement*);

  bool
  found_break_or_goto() const
  { return this->found_break_or_goto_; }

 private:
  bool
  found_break_or_goto_;
};

int
Find_break_traverse::statement(Block*, size_t*, Statement* s)
{
  if (s->classification() == Statement::STATEMENT_GOTO)
    {
      this->found_break_or_goto_ = true;
      return TRAVERSE_EXIT;
    }

  // FIXME: We don't check for breaks out of a different loop.
  if (s->classification() == Statement::STATEMENT_BREAK_OR_CONTINUE)
    {
      const Bc_statement* bcs = static_cast<const Bc_statement*>(s);
      if (bcs->is_break())
	{
	  this->found_break_or_goto_ = true;
	  return TRAVERSE_EXIT;
	}
    }

  return TRAVERSE_CONTINUE;
}

// Return whether this may fall through.

bool
For_statement::do_may_fall_through() const
{
  if (this->cond_ != NULL)
    {
      // FIXME: Should check "for true { }".
      return true;
    }

  Find_break_traverse fbt;
  this->statements_->traverse(&fbt);
  return fbt.found_break_or_goto();
}

// Return the tree.

tree
For_statement::do_get_tree(Translate_context* context)
{
  tree statements = NULL_TREE;

  if (this->init_ != NULL)
    append_to_statement_list(this->init_->get_tree(context), &statements);

  tree entry = NULL_TREE;
  if (this->cond_ != NULL)
    {
      entry = build1(LABEL_EXPR, void_type_node, NULL_TREE);
      tree t = build_and_jump(&LABEL_EXPR_LABEL(entry));
      SET_EXPR_LOCATION(t, this->location());
      append_to_statement_list(t, &statements);
    }

  tree top = build1(LABEL_EXPR, void_type_node, NULL_TREE);
  append_to_statement_list(top, &statements);

  // If this function has a reference count queue, we flush it at the
  // start of each loop iteration.  We must do this, since each
  // iteration of the loop may change the same entries in the queue.
  Refcounts* refcounts = context->function()->func_value()->refcounts();
  if (refcounts != NULL && !refcounts->empty())
    {
      source_location loc = this->statements_->start_location();
      append_to_statement_list(refcounts->flush_queue(context->gogo(), false,
						      loc),
			       &statements);
    }

  append_to_statement_list(this->statements_->get_tree(context),
			   &statements);

  if (this->needs_continue_label_)
    append_to_statement_list(this->continue_label(), &statements);

  if (this->post_ != NULL)
    append_to_statement_list(this->post_->get_tree(context),
			     &statements);
  if (this->cond_ == NULL)
    {
      tree t = build_and_jump(&LABEL_EXPR_LABEL(top));
      SET_EXPR_LOCATION(t, this->location());
      append_to_statement_list(t, &statements);
    }
  else
    {
      append_to_statement_list(entry, &statements);
      tree bottom = build1(LABEL_EXPR, void_type_node, NULL_TREE);
      tree t = fold_build3(COND_EXPR, void_type_node,
			   this->cond_->get_tree(context),
			   build_and_jump(&LABEL_EXPR_LABEL(top)),
			   build_and_jump(&LABEL_EXPR_LABEL(bottom)));
      if (CAN_HAVE_LOCATION_P(t))
	SET_EXPR_LOCATION(t, this->location());
      append_to_statement_list(t, &statements);
      append_to_statement_list(bottom, &statements);
    }

  if (this->needs_break_label_)
    append_to_statement_list(this->break_label(), &statements);

  return statements;
}

// Return the break LABEL_EXPR.

tree
For_statement::break_label()
{
  gcc_assert(this->needs_break_label_);
  if (this->break_label_ == NULL_TREE)
    this->break_label_ = build1(LABEL_EXPR, void_type_node,
				create_artificial_label(this->location()));
  return this->break_label_;
}

// Return the continue LABEL_EXPR.

tree
For_statement::continue_label()
{
  gcc_assert(this->needs_continue_label_);
  if (this->continue_label_ == NULL_TREE)
    this->continue_label_ = build1(LABEL_EXPR, void_type_node,
				   create_artificial_label(this->location()));
  return this->continue_label_;
}

// Make a for statement.

For_statement*
Statement::make_for_statement(Block* init, Expression* cond, Block* post,
			      source_location location)
{
  return new For_statement(init, cond, post, location);
}

// Class For_range_statement.

// Traversal.

int
For_range_statement::do_traverse(Traverse* traverse)
{
  if (this->traverse_expression(traverse, &this->index_var_) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  if (this->value_var_ != NULL)
    {
      if (this->traverse_expression(traverse, &this->value_var_)
	  == TRAVERSE_EXIT)
	return TRAVERSE_EXIT;
    }
  if (this->traverse_expression(traverse, &this->range_) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  return this->statements_->traverse(traverse);
}

// Traverse assignments.

bool
For_range_statement::do_traverse_assignments(Traverse_assignments* tassign)
{
  tassign->assignment(&this->index_var_, NULL);
  if (this->value_var_ != NULL)
    tassign->assignment(&this->value_var_, NULL);
  tassign->value(&this->range_, false, this->index_var_->is_local_variable());
  return true;
}

// Determine types.

void
For_range_statement::do_determine_types()
{
  this->range_->determine_type_no_context();
  Type* range_type = this->range_->type();
  if (range_type->points_to() != NULL
      && range_type->points_to()->array_type() != NULL
      && !range_type->points_to()->is_open_array_type())
    range_type = range_type->points_to();
  Type* index_type;
  Type* value_type;
  if (range_type->array_type() != NULL)
    {
      index_type = Type::lookup_integer_type("int");
      value_type = range_type->array_type()->element_type();
    }
  else if (range_type->is_string_type())
    {
      index_type = Type::lookup_integer_type("int");
      value_type = index_type;
    }
  else if (range_type->map_type() != NULL)
    {
      index_type = range_type->map_type()->key_type();
      value_type = range_type->map_type()->val_type();
    }
  else if (range_type->channel_type() != NULL)
    {
      index_type = range_type->channel_type()->element_type();
      value_type = NULL;
    }
  else
    {
      index_type = NULL;
      value_type = NULL;
    }
  if (index_type != NULL)
    {
      Type_context context1(index_type, false);
      this->index_var_->determine_type(&context1);
    }
  if (value_type != NULL && this->value_var_ != NULL)
    {
      Type_context context2(value_type, false);
      this->value_var_->determine_type(&context2);
    }
  this->statements_->determine_types();
}

// Check types.

void
For_range_statement::do_check_types(Gogo*)
{
  Type* range_type = this->range_->type();
  if (range_type->points_to() != NULL
      && range_type->points_to()->array_type() != NULL
      && !range_type->points_to()->is_open_array_type())
    range_type = range_type->points_to();
  Type* index_type;
  Type* value_type;
  if (range_type->array_type() != NULL)
    {
      index_type = Type::lookup_integer_type("int");
      value_type = range_type->array_type()->element_type();
    }
  else if (range_type->is_string_type())
    {
      index_type = Type::lookup_integer_type("int");
      value_type = index_type;
    }
  else if (range_type->map_type() != NULL)
    {
      index_type = range_type->map_type()->key_type();
      value_type = range_type->map_type()->val_type();
    }
  else if (range_type->channel_type() != NULL)
    {
      index_type = range_type->channel_type()->element_type();
      if (this->value_var_ != NULL
	  && !this->value_var_->type()->is_error_type())
	{
	  this->report_error(_("too many variables for range clause "
			       "with channel"));
	  return;
	}
      value_type = Type::make_error_type();
    }
  else
    {
      this->report_error(_("range clause must have "
			   "array, string, slice, map, or channel type"));
      return;
    }
  if (!this->index_var_->is_lvalue()
      || (this->value_var_ != NULL && !this->value_var_->is_lvalue()))
    this->report_error("invalid left hand side of assignment");
  else if (!Type::are_compatible_for_assign(this->index_var_->type(),
					    index_type, NULL))
    this->report_error("incompatible types for range index");
  else if (this->value_var_ != NULL
	   && !Type::are_compatible_for_assign(this->value_var_->type(),
					       value_type, NULL))
    this->report_error("incompatible types for range value");
}

// Return the tree.

tree
For_range_statement::do_get_tree(Translate_context* context)
{
  tree index_var_tree = this->index_var_->get_tree(context);
  if (index_var_tree == error_mark_node)
    return error_mark_node;
  index_var_tree = stabilize_reference(index_var_tree);
  tree value_var_tree;
  if (this->value_var_ == NULL)
    value_var_tree = NULL_TREE;
  else
    {
      value_var_tree = this->value_var_->get_tree(context);
      if (value_var_tree == error_mark_node)
	return error_mark_node;
      value_var_tree = stabilize_reference(value_var_tree);
    }

  tree range_tree = this->range_->get_tree(context);
  if (range_tree == error_mark_node)
    return false;
  Type* range_type = this->range_->type();
  if (range_type->points_to() != NULL)
    {
      range_type = range_type->points_to();
      gcc_assert(range_type->array_type() != NULL
		 && !range_type->is_open_array_type());
      range_tree = build_fold_indirect_ref(range_tree);
    }

  tree prep;
  tree test;
  tree setvars;
  tree next;
  if (this->range_->type()->deref()->array_type() != NULL)
    this->get_array_iteration(context, index_var_tree, value_var_tree,
			      range_tree, &prep, &test, &setvars, &next);
  else if (this->range_->type()->deref()->is_string_type())
    this->get_string_iteration(context, index_var_tree, value_var_tree,
			       range_tree, &prep, &test, &setvars, &next);
  else if (this->range_->type()->deref()->map_type() != NULL)
    this->get_map_iteration(context, index_var_tree, value_var_tree,
			    range_tree, &prep, &test, &setvars, &next);
  else if (this->range_->type()->channel_type() != NULL)
    this->get_channel_iteration(context, index_var_tree, range_tree,
				&prep, &test, &setvars, &next);
  else
    gcc_unreachable();

  tree statements = NULL_TREE;

  if (prep != NULL_TREE)
    append_to_statement_list(prep, &statements);

  tree entry = build1(LABEL_EXPR, void_type_node, NULL_TREE);
  tree t = build_and_jump(&LABEL_EXPR_LABEL(entry));
  SET_EXPR_LOCATION(t, this->location());
  append_to_statement_list(t, &statements);

  tree top = build1(LABEL_EXPR, void_type_node, NULL_TREE);
  append_to_statement_list(top, &statements);

  // If this function has a reference count queue, we flush it at the
  // start of each loop iteration.  We must do this, since each
  // iteration of the loop may change the same entries in the queue.
  Refcounts* refcounts = context->function()->func_value()->refcounts();
  if (refcounts != NULL && !refcounts->empty())
    {
      source_location loc = this->statements_->start_location();
      append_to_statement_list(refcounts->flush_queue(context->gogo(), false,
						      loc),
			       &statements);
    }

  append_to_statement_list(setvars, &statements);

  append_to_statement_list(this->statements_->get_tree(context),
			   &statements);

  if (this->needs_continue_label_)
    append_to_statement_list(this->continue_label(), &statements);

  if (next != NULL)
    append_to_statement_list(next, &statements);

  append_to_statement_list(entry, &statements);
  tree bottom = build1(LABEL_EXPR, void_type_node, NULL_TREE);
  t = fold_build3(COND_EXPR, void_type_node, test,
		  build_and_jump(&LABEL_EXPR_LABEL(top)),
		  build_and_jump(&LABEL_EXPR_LABEL(bottom)));
  if (CAN_HAVE_LOCATION_P(t))
    SET_EXPR_LOCATION(t, this->location());
  append_to_statement_list(t, &statements);
  append_to_statement_list(bottom, &statements);

  if (this->needs_break_label_)
    append_to_statement_list(this->break_label(), &statements);

  return statements;
}

// The steps for iterating over an array.

void
For_range_statement::get_array_iteration(Translate_context* context,
					 tree index_var_tree,
					 tree value_var_tree, tree range_tree,
					 tree* prep, tree* test, tree* setvars,
					 tree* next)
{
  Array_type* range_type = this->range_->type()->deref()->array_type();
  Type* element_type = range_type->element_type();
  tree element_type_tree = element_type->get_tree(context->gogo());

  tree values_tree;
  if (value_var_tree == NULL)
    values_tree = NULL_TREE;
  else if (range_type->length() != NULL)
    {
      range_tree = stabilize_reference(range_tree);
      values_tree = NULL_TREE;
    }
  else
    {
      range_tree = save_expr(range_tree);
      values_tree = range_type->value_pointer_tree(context->gogo(),
						   range_tree);
      values_tree = save_expr(values_tree);
    }

  tree tmp_index = create_tmp_var(integer_type_node, NULL);
  DECL_INITIAL(tmp_index) = integer_zero_node;

  tree tmp_len = create_tmp_var(integer_type_node, NULL);
  DECL_INITIAL(tmp_len) = range_type->length_tree(context->gogo(), range_tree);

  *prep = build2(COMPOUND_EXPR, void_type_node,
		 build1(DECL_EXPR, void_type_node, tmp_index),
		 build1(DECL_EXPR, void_type_node, tmp_len));
  SET_EXPR_LOCATION(*prep, this->location());

  *test = build2(LT_EXPR, boolean_type_node, tmp_index, tmp_len);
  SET_EXPR_LOCATION(*test, this->location());

  tree set = build2(MODIFY_EXPR, void_type_node, index_var_tree, tmp_index);
  SET_EXPR_LOCATION(set, this->location());
  if (value_var_tree == NULL_TREE)
    *setvars = set;
  else
    {
      tree val;
      if (range_type->length() != NULL)
	val = build4(ARRAY_REF, TREE_TYPE(TREE_TYPE(range_tree)),
		     range_tree, tmp_index, NULL_TREE, NULL_TREE);
      else
	{
	  tree element_size = TYPE_SIZE_UNIT(element_type_tree);
	  tree ptr = fold_build2(POINTER_PLUS_EXPR, TREE_TYPE(values_tree),
				 values_tree,
				 fold_build2(MULT_EXPR, sizetype,
					     fold_convert(sizetype,
							  tmp_index),
					     element_size));
	  val = build_fold_indirect_ref(ptr);
	}
      tree set2 = build2(MODIFY_EXPR, void_type_node, value_var_tree, val);
      SET_EXPR_LOCATION(set2, this->location());
      *setvars = build2(COMPOUND_EXPR, void_type_node, set, set2);
    }
  SET_EXPR_LOCATION(*setvars, this->location());

  *next = build2(MODIFY_EXPR, void_type_node, tmp_index,
		 build2(PLUS_EXPR, integer_type_node, tmp_index,
			integer_one_node));
  SET_EXPR_LOCATION(*next, this->location());
}

// The steps for iterating over a string.

void
For_range_statement::get_string_iteration(Translate_context*,
					  tree index_var_tree,
					  tree value_var_tree, tree range_tree,
					  tree* prep, tree* test,
					  tree* setvars, tree* next)
{
  range_tree = save_expr(range_tree);

  tree tmp_index = create_tmp_var(integer_type_node, NULL);
  DECL_INITIAL(tmp_index) = integer_zero_node;
  TREE_ADDRESSABLE(tmp_index) = 1;

  tree tmp_prev_index = create_tmp_var(integer_type_node, NULL);
  DECL_INITIAL(tmp_prev_index) = integer_zero_node;

  tree tmps = build2(COMPOUND_EXPR, void_type_node,
		     build1(DECL_EXPR, void_type_node, tmp_index),
		     build1(DECL_EXPR, void_type_node, tmp_prev_index));
  SET_EXPR_LOCATION(tmps, this->location());

  tree tmp_val = NULL_TREE;
  if (value_var_tree != NULL_TREE)
    {
      tmp_val = create_tmp_var(integer_type_node, NULL);
      tmps = build2(COMPOUND_EXPR, void_type_node, tmps,
		    build1(DECL_EXPR, void_type_node, tmp_val));
      SET_EXPR_LOCATION(tmps, this->location());
      TREE_ADDRESSABLE(tmp_val) = 1;
    }

  *prep = tmps;

  tree value_ptr_tree;
  if (value_var_tree != NULL_TREE)
    value_ptr_tree = build_fold_addr_expr(tmp_val);
  else
    value_ptr_tree = fold_convert(build_pointer_type(integer_type_node),
				  null_pointer_node);

  static tree string_range_fndecl;
  *test = Gogo::call_builtin(&string_range_fndecl,
			     this->location(),
			     "__go_string_range",
			     3,
			     boolean_type_node,
			     TREE_TYPE(range_tree),
			     range_tree,
			     build_pointer_type(integer_type_node),
			     build_fold_addr_expr(tmp_index),
			     build_pointer_type(integer_type_node),
			     value_ptr_tree);

  tree set = build2(MODIFY_EXPR, void_type_node, index_var_tree,
		    tmp_prev_index);
  SET_EXPR_LOCATION(set, this->location());
  if (value_var_tree == NULL_TREE)
    *setvars = set;
  else
    {
      tree set2 = build2(MODIFY_EXPR, void_type_node, value_var_tree, tmp_val);
      SET_EXPR_LOCATION(set2, this->location());
      *setvars = build2(COMPOUND_EXPR, void_type_node, set, set2);
    }

  *next = build2(MODIFY_EXPR, void_type_node, tmp_prev_index, tmp_index);
  SET_EXPR_LOCATION(*next, this->location());
}

// The steps for iterating over a map.

void
For_range_statement::get_map_iteration(Translate_context* context,
				       tree index_var_tree,
				       tree value_var_tree, tree range_tree,
				       tree* prep, tree* test, tree* setvars,
				       tree* next)
{
  tree tmp_bucket = create_tmp_var(sizetype, NULL);
  DECL_INITIAL(tmp_bucket) = size_zero_node;
  TREE_ADDRESSABLE(tmp_bucket) = 1;

  tree entry_type = build_pointer_type(const_ptr_type_node);
  tree tmp_entry = create_tmp_var(entry_type, NULL);
  DECL_INITIAL(tmp_entry) = fold_convert(entry_type, null_pointer_node);
  TREE_ADDRESSABLE(tmp_entry) = 1;

  *prep = build2(COMPOUND_EXPR, void_type_node,
		 build1(DECL_EXPR, void_type_node, tmp_bucket),
		 build1(DECL_EXPR, void_type_node, tmp_entry));
  SET_EXPR_LOCATION(*prep, this->location());

  tree index_ptr_type = build_pointer_type(TREE_TYPE(index_var_tree));
  tree key_ptr = create_tmp_var(index_ptr_type, NULL);
  TREE_ADDRESSABLE(key_ptr) = 1;
  tree val_ptr_type;
  tree val_ptr;
  if (value_var_tree == NULL_TREE)
    {
      val_ptr_type = NULL_TREE;
      val_ptr = NULL_TREE;
    }
  else
    {
      val_ptr_type = build_pointer_type(TREE_TYPE(value_var_tree));
      val_ptr = create_tmp_var(val_ptr_type, NULL);
      TREE_ADDRESSABLE(val_ptr) = 1;
    }

  tree ptr_ptr_type = build_pointer_type(ptr_type_node);

  range_tree = save_expr(range_tree);
  static tree map_range_fndecl;
  *test = Gogo::call_builtin(&map_range_fndecl,
			     this->location(),
			     "__go_map_range",
			     5,
			     boolean_type_node,
			     TREE_TYPE(range_tree),
			     range_tree,
			     build_pointer_type(sizetype),
			     build_fold_addr_expr(tmp_bucket),
			     build_pointer_type(entry_type),
			     build_fold_addr_expr(tmp_entry),
			     ptr_ptr_type,
			     fold_convert(ptr_ptr_type,
					  build_fold_addr_expr(key_ptr)),
			     ptr_ptr_type,
			     fold_convert(ptr_ptr_type,
					  (val_ptr == NULL_TREE
					   ? null_pointer_node
					   : build_fold_addr_expr(val_ptr))));

  Map_type* mt = this->range_->type()->deref()->map_type();
  tree key_val = build_fold_indirect_ref(key_ptr);
  tree set = Assignment_statement::get_assignment_tree(context,
						       OPERATOR_EQ,
						       this->index_var_,
						       index_var_tree,
						       NULL,
						       mt->key_type(),
						       key_val,
						       this->location());
  if (value_var_tree == NULL_TREE)
    *setvars = set;
  else
    {
      tree val_val = build_fold_indirect_ref(val_ptr);
      tree set2 = Assignment_statement::get_assignment_tree(context,
							    OPERATOR_EQ,
							    this->value_var_,
							    value_var_tree,
							    NULL,
							    mt->val_type(),
							    val_val,
							    this->location());
      *setvars = build2(COMPOUND_EXPR, void_type_node, set, set2);
    }

  *next = NULL_TREE;
}

// The steps for iterating over an channel.

void
For_range_statement::get_channel_iteration(Translate_context* context,
					   tree index_var_tree,
					   tree range_tree,
					   tree* prep, tree* test,
					   tree* setvars, tree* next)
{
  Channel_type* range_type = this->range_->type()->channel_type();
  Type* element_type = range_type->element_type();
  tree element_type_tree = element_type->get_tree(context->gogo());

  tree tmp_chan = create_tmp_var(TREE_TYPE(range_tree), get_name(range_tree));
  DECL_INITIAL(tmp_chan) = range_tree;

  tree tmp_var = create_tmp_var(element_type_tree, NULL);

  *prep = build2(COMPOUND_EXPR, void_type_node,
		 build1(DECL_EXPR, void_type_node, tmp_chan),
		 build1(DECL_EXPR, void_type_node, tmp_var));

  // Closed only returns true after we have read a nil value.  The
  // test for the range clause is
  //   tmp := <- chan; !closed(chan)

  tree recv = Gogo::receive_from_channel(element_type_tree, tmp_chan, true,
					 false, this->location());
  tree set_tmp = build2(MODIFY_EXPR, void_type_node, tmp_var, recv);
  // FIXME: Duplicates Builtin_call_expression::do_get_tree.
  static tree closed_fndecl;
  tree is_closed = Gogo::call_builtin(&closed_fndecl,
				      this->location(),
				      "__go_builtin_closed",
				      1,
				      boolean_type_node,
				      TREE_TYPE(tmp_chan),
				      tmp_chan);
  tree is_not_closed = build1(TRUTH_NOT_EXPR, boolean_type_node, is_closed);
  *test = build2(COMPOUND_EXPR, boolean_type_node, set_tmp, is_not_closed);

  // Set the variable to the value we received from the channel.
  *setvars = Assignment_statement::get_assignment_tree(context,
						       OPERATOR_EQ,
						       this->index_var_,
						       index_var_tree,
						       NULL,
						       element_type,
						       tmp_var,
						       this->location());

  *next = NULL_TREE;
}

// Return the break LABEL_EXPR.

tree
For_range_statement::break_label()
{
  gcc_assert(this->needs_break_label_);
  if (this->break_label_ == NULL_TREE)
    this->break_label_ = build1(LABEL_EXPR, void_type_node,
				create_artificial_label(this->location()));
  return this->break_label_;
}

// Return the continue LABEL_EXPR.

tree
For_range_statement::continue_label()
{
  gcc_assert(this->needs_continue_label_);
  if (this->continue_label_ == NULL_TREE)
    this->continue_label_ = build1(LABEL_EXPR, void_type_node,
				   create_artificial_label(this->location()));
  return this->continue_label_;
}

// Make a for statement with a range clause.

For_range_statement*
Statement::make_for_range_statement(Expression* index_var,
				    Expression* value_var,
				    Expression* range,
				    source_location location)
{
  return new For_range_statement(index_var, value_var, range, location);
}

// Return the tree for looping over an array.

// An assignment to an entry in the reference count queue.

class Refcount_queue_assignment_statement : public Statement
{
 public:
  Refcount_queue_assignment_statement(Refcount_entry entry,
				      Expression* expr,
				      source_location location)
    : Statement(STATEMENT_REFCOUNT_QUEUE_ASSIGNMENT, location),
      entry_(entry), expr_(expr)
  { }

 protected:
  int
  do_traverse(Traverse* traverse)
  { return this->traverse_expression(traverse, &this->expr_); }

  // Types should have been determined and checked before any of these
  // statements are created.

  void
  do_determine_types()
  { gcc_unreachable(); }

  void
  do_check_types(Gogo*)
  { gcc_unreachable(); }

  tree
  do_get_tree(Translate_context*);

 private:
  // The entry in the reference count queue.
  Refcount_entry entry_;
  // The expression we are storing in the queue.
  Expression* expr_;
};

// Implement the actual assignment.

tree
Refcount_queue_assignment_statement::do_get_tree(Translate_context* context)
{
  // We only need this statement for variables.  It will only work for
  // expressions without side effects.
  tree expr_tree = this->expr_->get_tree(context);
  if (expr_tree == error_mark_node)
    return error_mark_node;
  gcc_assert(TREE_CODE(expr_tree) == VAR_DECL
	     || TREE_CODE(expr_tree) == PARM_DECL
	     || (TREE_CODE(expr_tree) == INDIRECT_REF
		 && TREE_CODE(TREE_OPERAND(expr_tree, 0)) == VAR_DECL));
  Refcounts* refcounts = context->function()->func_value()->refcounts();
  return this->expr_->type()->set_refcount_queue_entry(context->gogo(),
						       refcounts,
						       &this->entry_,
						       expr_tree);
}

// Make an assignment to an entry in the reference count queue.

Statement*
Statement::make_refcount_queue_assignment_statement(Refcounts*,
						    Refcount_entry* entry,
						    Expression* expr,
						    source_location location)
{
  return new Refcount_queue_assignment_statement(*entry, expr, location);
}
