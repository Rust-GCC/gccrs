// refcount.cc -- Go frontend reference counts.

// Copyright 2009 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <algorithm>

#include "go-system.h"

#include <gmp.h>

extern "C"
{
#include "tree.h"
#include "gimple.h"
#include "tree-iterator.h"
#include "rtl.h"
}

#include "go-c.h"
#include "go-dump.h"
#include "gogo.h"
#include "types.h"
#include "expressions.h"
#include "statements.h"
#include "refcount.h"

// Class Refcounts.

// Add a new reference count entry.

Refcount_entry
Refcounts::add(int classification, Type* type)
{
  gcc_assert(!this->is_complete_);
  Entries* pentries = &this->entries_[classification];
  int index = pentries->size();
  Refcount_entry r(classification, index);
  type->add_refcount_queue_entries(this, &r);
  return Refcount_entry(classification, index);
}

// Add a new entry for a type which only requires a single entry.

void
Refcounts::add_one(Type* type, Refcount_entry *entry)
{
  gcc_assert(!this->is_complete_);
  Entries* pentries = &this->entries_[entry->classification()];
  gcc_assert(pentries->size() == entry->index());
  pentries->push_back(type);
  entry->increment();
}

// The type of a reference count entry.

Type*
Refcounts::get_type(Refcount_entry entry)
{
  int cl = entry.classification();
  gcc_assert(cl >= REFCOUNT_DECREMENT_NEW && cl < REFCOUNT_MAX);
  Entries* entries = &this->entries_[cl];
  gcc_assert(entry.index() < entries->size());
  return (*entries)[entry.index()];
}

// Return whether there are no reference count adjustments.

bool
Refcounts::empty() const
{
  for (int i = 0; i < REFCOUNT_MAX; ++i)
    if (!this->entries_[i].empty())
      return false;
  return true;
}

// Whether we can handle this reference count queue in a simple way,
// without building a full queue structure.  We want to use the queue
// structure if there is a decrement of a newly allocated value,
// because an increment in a called function will remove the
// decrement.  Otherwise, we use a simple approach if there is only
// one entry in the queue.

bool
Refcounts::is_simple() const
{
  gcc_assert(this->is_complete_);
  if (!this->entries_[REFCOUNT_DECREMENT_NEW].empty())
    return false;
  size_t count = 0;
  for (int i = 0; i < REFCOUNT_MAX; ++i)
    count += this->entries_[i].size();
  return count <= 1;
}

// Get the descriptor for a simple queue.  Return true if this is an
// increment.

bool
Refcounts::simple_descriptor(Gogo* gogo, tree* pdescriptor) const
{
  for (int i = 0; i < REFCOUNT_MAX; ++i)
    {
      if (!this->entries_[i].empty())
	{
	  gcc_assert(this->entries_[i].size() == 1);
	  Type* type = this->entries_[i][0];
	  *pdescriptor = type->type_descriptor(gogo);
	  return i == REFCOUNT_INCREMENT_COPIED;
	}
    }
  gcc_unreachable();
}

// The type of an entry in the reference count array.  This is the
// structure declared in libgo/runtime/go-refcount.h.

tree
Refcounts::entry_type(Gogo* gogo)
{
  static tree struct_type;
  if (struct_type == NULL_TREE)
    {
      struct_type = make_node(RECORD_TYPE);

      tree dtype = gogo->type_descriptor_type_tree();
      dtype = build_qualified_type(dtype, TYPE_QUAL_CONST);
      tree fields = build_decl(BUILTINS_LOCATION, FIELD_DECL,
			       get_identifier("__descriptor"),
			       build_pointer_type(dtype));

      tree field = build_decl(BUILTINS_LOCATION, FIELD_DECL,
			      get_identifier("__value"),
			      ptr_type_node);
      TREE_CHAIN(field) = fields;
      fields = field;

      finish_builtin_struct(struct_type, "__go_refcount_entry", fields,
			    NULL_TREE);

      go_preserve_from_gc(struct_type);
    }

  return struct_type;
}

// The type of the reference count array for this structure..  This is
// the structure declared in libgo/runtime/go-refcount.h.

tree
Refcounts::struct_type(Gogo* gogo)
{
  gcc_assert(this->is_complete_);

  tree struct_type = make_node(RECORD_TYPE);

  tree fields = build_decl(BUILTINS_LOCATION, FIELD_DECL,
			   get_identifier("__caller"),
			   build_pointer_type(struct_type));

  tree field = build_decl(BUILTINS_LOCATION, FIELD_DECL,
			  get_identifier("__callers_were_scanned"),
			  boolean_type_node);
  TREE_CHAIN(field) = fields;
  fields = field;

  field = build_decl(BUILTINS_LOCATION, FIELD_DECL,
		     get_identifier("__did_not_scan_decrements"),
		     boolean_type_node);
  TREE_CHAIN(field) = fields;
  fields = field;

  field = build_decl(BUILTINS_LOCATION, FIELD_DECL,
		     get_identifier("__decrement_new_count"),
		     short_unsigned_type_node);
  TREE_CHAIN(field) = fields;
  fields = field;

  field = build_decl(BUILTINS_LOCATION, FIELD_DECL,
		     get_identifier("__decrement_computed_count"),
		     short_unsigned_type_node);
  TREE_CHAIN(field) = fields;
  fields = field;

  field = build_decl(BUILTINS_LOCATION, FIELD_DECL,
		     get_identifier("__decrement_old_count"),
		    short_unsigned_type_node);
  TREE_CHAIN(field) = fields;
  fields = field;

  field = build_decl(BUILTINS_LOCATION, FIELD_DECL,
		     get_identifier("__increment_copy_count"),
		     short_unsigned_type_node);
  TREE_CHAIN(field) = fields;
  fields = field;

  size_t count = 0;
  for (int i = 0; i < REFCOUNT_MAX; ++i)
    count += this->entries_[i].size();
  gcc_assert(count > 0);

  tree entry_type = Refcounts::entry_type(gogo);
  tree index_type = build_index_type(size_int(count - 1));
  tree array_type = build_array_type(entry_type, index_type);
  field = build_decl(BUILTINS_LOCATION, FIELD_DECL,
		     get_identifier("__entries"), array_type);
  TREE_CHAIN(field) = fields;
  fields = field;

  finish_builtin_struct(struct_type, "__go_refcount", fields, NULL_TREE);

  return struct_type;
}

// Get the initializer for the reference count tree.

tree
Refcounts::get_initializer(Gogo* gogo)
{
  gcc_assert(this->is_complete_);
  gcc_assert(!this->is_simple());

  size_t count = 0;
  for (int i = 0; i < REFCOUNT_MAX; ++i)
    count += this->entries_[i].size();
  gcc_assert(count > 0);

  VEC(constructor_elt, gc)* init = VEC_alloc(constructor_elt, gc, 8);

  tree decl = this->get_tree(gogo);
  tree type = TREE_TYPE(decl);

  tree field = TYPE_FIELDS(type);

  constructor_elt* elt = VEC_quick_push(constructor_elt, init, NULL);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)),
		    "__caller") == 0);
  elt->index = field;
  elt->value = this->queue_head_decl(gogo);

  field = TREE_CHAIN(field);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)),
		    "__callers_were_scanned") == 0);
  elt->index = field;
  elt->value = boolean_false_node;

  field = TREE_CHAIN(field);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)),
		    "__did_not_scan_decrements") == 0);
  elt->index = field;
  elt->value = boolean_false_node;

  field = TREE_CHAIN(field);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)),
		    "__decrement_new_count") == 0);
  elt->index = field;
  size_t c = this->entries_[REFCOUNT_DECREMENT_NEW].size();
  gcc_assert(c <= 0xffff);
  elt->value = build_int_cst_type(TREE_TYPE(field), c);

  field = TREE_CHAIN(field);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)),
		    "__decrement_computed_count") == 0);
  elt->index = field;
  c = this->entries_[REFCOUNT_DECREMENT_COMPUTED].size();
  gcc_assert(c <= 0xffff);
  elt->value = build_int_cst_type(TREE_TYPE(field), c);

  field = TREE_CHAIN(field);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)),
		    "__decrement_old_count") == 0);
  elt->index = field;
  c = this->entries_[REFCOUNT_DECREMENT_OLD].size();
  gcc_assert(c <= 0xffff);
  elt->value = build_int_cst_type(TREE_TYPE(field), c);

  field = TREE_CHAIN(field);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)),
		    "__increment_copy_count") == 0);
  elt->index = field;
  c = this->entries_[REFCOUNT_INCREMENT_COPIED].size();
  gcc_assert(c <= 0xffff);
  elt->value = build_int_cst_type(TREE_TYPE(field), c);

  field = TREE_CHAIN(field);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)),
		    "__entries") == 0);
  elt->index = field;

  tree desc_field = TYPE_FIELDS(TREE_TYPE(TREE_TYPE(field)));
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(desc_field)),
		    "__descriptor") == 0);
  tree value_field = TREE_CHAIN(desc_field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(value_field)),
		    "__value") == 0);

  VEC(constructor_elt, gc)* entries_init = VEC_alloc(constructor_elt,
						     gc, count);
  size_t index = 0;
  for (int i = 0; i < REFCOUNT_MAX; ++i)
    {
      Entries* entries = &this->entries_[i];
      for (Entries::const_iterator p = entries->begin();
	   p != entries->end();
	   ++p, ++index)
	{
	  VEC(constructor_elt, gc)* entry_init = VEC_alloc(constructor_elt,
							   gc, 2);
	  constructor_elt* eelt = VEC_quick_push(constructor_elt,
						 entry_init, NULL);
	  eelt->index = desc_field;
	  Type* type = *p;
	  eelt->value = type->type_descriptor(gogo);

	  eelt = VEC_quick_push(constructor_elt, entry_init, NULL);
	  eelt->index = value_field;
	  eelt->value = fold_convert(TREE_TYPE(value_field),
				     null_pointer_node);

	  eelt = VEC_quick_push(constructor_elt, entries_init, NULL);
	  eelt->index = size_int(index);
	  eelt->value = build_constructor(TREE_TYPE(TREE_TYPE(field)),
					  entry_init);
	}
    }

  elt->value = build_constructor(TREE_TYPE(field), entries_init);

  return build_constructor(TREE_TYPE(decl), init);
}

// Get the local variable which holds the reference count tree.

tree
Refcounts::get_tree(Gogo* gogo)
{
  gcc_assert(!this->is_simple());
  if (this->decl_ == NULL_TREE)
    {
      tree type = this->struct_type(gogo);
      this->decl_ = create_tmp_var(type, NULL);
    }
  return this->decl_;
}

// Get the local variable which holds the only entry we need if the
// queue is simple.

tree
Refcounts::get_simple_tree(Gogo*)
{
  gcc_assert(this->is_simple());
  if (this->simple_decl_ == NULL_TREE)
    this->simple_decl_ = create_tmp_var(ptr_type_node, NULL);
  return this->simple_decl_;
}

// Return a tree which sets the queue entry for ENTRY to VAL.

tree
Refcounts::set_entry_tree(Gogo* gogo, Refcount_entry entry, tree val)
{
  gcc_assert(this->is_complete_);

  gcc_assert(POINTER_TYPE_P(TREE_TYPE(val)));

  if (this->is_simple())
    return build2(MODIFY_EXPR, void_type_node, this->get_simple_tree(gogo),
		  fold_convert(ptr_type_node, val));

  int cl = entry.classification();
  gcc_assert(cl >= REFCOUNT_DECREMENT_NEW && cl < REFCOUNT_MAX);
  Entries* entries = &this->entries_[cl];
  tree decl = this->get_tree(gogo);
  tree field = TYPE_FIELDS(TREE_TYPE(decl));
  field = TREE_CHAIN(TREE_CHAIN(field));
  field = TREE_CHAIN(TREE_CHAIN(field));
  field = TREE_CHAIN(TREE_CHAIN(field));
  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__entries") == 0);

  // Set R to the __entries array.
  tree r = build3(COMPONENT_REF, TREE_TYPE(field), decl, field, NULL_TREE);

  unsigned int index = entry.index();
  gcc_assert(index < entries->size());
  for (int i = 0; i < cl; ++i)
    index += this->entries_[i].size();

  // Set R to the specific entry in the __entries array.
  r = build4(ARRAY_REF, TREE_TYPE(TREE_TYPE(r)), r, size_int(index),
	     NULL_TREE, NULL_TREE);

  field = TREE_CHAIN(TYPE_FIELDS(TREE_TYPE(r)));
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__value") == 0);

  // Set R to the __value field in the specific entry in the __entries
  // array.
  r = build3(COMPONENT_REF, TREE_TYPE(field), r, field, NULL_TREE);

  // An assignment to the field.
  tree set = build2(MODIFY_EXPR, void_type_node, r,
		    fold_convert(ptr_type_node, val));

  // Set the changed flag to true.
  tree set_flag = build2(MODIFY_EXPR, void_type_node,
			 this->get_flag_tree(gogo), boolean_true_node);

  // Return the pair of assignments.
  return build2(COMPOUND_EXPR, void_type_node, set, set_flag);
}

// Return the tree for the flag which indicates whether anything
// changed.

tree
Refcounts::get_flag_tree(Gogo*)
{
  gcc_assert(!this->is_simple());
  if (this->flag_decl_ == NULL_TREE)
    {
      tree decl = create_tmp_var(boolean_type_node, NULL);
      DECL_INITIAL(decl) = boolean_false_node;
      this->flag_decl_ = decl;
    }
  return this->flag_decl_;
}

// Return a decl for the thread-local variable which holds the head of
// the list of reference count queues.

tree
Refcounts::queue_head_decl(Gogo* gogo)
{
  gcc_assert(!this->is_simple());
  if (this->queue_head_decl_ == NULL_TREE)
    {
      tree local_decl = this->get_tree(gogo);
      tree decl = build_decl(BUILTINS_LOCATION, VAR_DECL,
			     get_identifier("__go_refcount_head"),
			     build_pointer_type(TREE_TYPE(local_decl)));
      TREE_PUBLIC(decl) = 1;
      DECL_EXTERNAL(decl) = 1;
      DECL_TLS_MODEL(decl) = decl_default_tls_model(decl);
      go_preserve_from_gc(decl);
      this->queue_head_decl_ = decl;
    }
  return this->queue_head_decl_;
}

// Return the tree for initializating the reference count queue at the
// start of a function.  We use a thread-local variable to record the
// current queue.

tree
Refcounts::init_queue(Gogo* gogo, source_location location)
{
  tree statements = NULL_TREE;

  if (this->is_simple())
    {
      tree decl = this->get_simple_tree(gogo);
      tree set = build2(INIT_EXPR, void_type_node, decl, null_pointer_node);
      SET_EXPR_LOCATION(set, location);
      append_to_statement_list(set, &statements);
    }
  else
    {
      tree decl = this->get_tree(gogo);
      tree init = build2(INIT_EXPR, void_type_node, decl,
			 this->get_initializer(gogo));
      SET_EXPR_LOCATION(init, location);
      append_to_statement_list(init, &statements);

      tree queue_head = this->queue_head_decl(gogo);
      tree set = build2(MODIFY_EXPR, void_type_node, queue_head,
			fold_convert(TREE_TYPE(queue_head),
				     build_fold_addr_expr(decl)));
      SET_EXPR_LOCATION(set, location);
      append_to_statement_list(set, &statements);

      init = build1(DECL_EXPR, void_type_node, this->get_flag_tree(gogo));
      SET_EXPR_LOCATION(init, location);
      append_to_statement_list(init, &statements);
    }

  return statements;
}

// Return the tree for flushing the reference count queue.

tree
Refcounts::flush_queue(Gogo* gogo, bool at_end_of_function,
		       source_location location)
{
  if (this->is_simple())
    {
      tree decl = this->get_simple_tree(gogo);
      static tree increment_fndecl;
      static tree decrement_fndecl;
      static tree* pfndecl;
      const char* name;
      tree descriptor;
      if (this->simple_descriptor(gogo, &descriptor))
	{
	  pfndecl = &increment_fndecl;
	  name = "__go_increment_refcount";
	}
      else
	{
	  pfndecl = &decrement_fndecl;
	  name = "__go_decrement_refcount";
	}
      return Gogo::call_builtin(pfndecl,
				location,
				name,
				2,
				void_type_node,
				ptr_type_node,
				decl,
				TREE_TYPE(descriptor),
				descriptor);
    }

  tree decl = this->get_tree(gogo);
  tree flag_decl = this->get_flag_tree(gogo);
  static tree flush_fndecl;
  tree call = Gogo::call_builtin(&flush_fndecl,
				 location,
				 "__go_refcount_flush_queue",
				 1,
				 void_type_node,
				 build_pointer_type(TREE_TYPE(decl)),
				 build_fold_addr_expr(decl));
  tree set = build2(MODIFY_EXPR, void_type_node, flag_decl,
		    boolean_false_node);
  tree ret = build3(COND_EXPR, void_type_node, flag_decl,
		    build2(COMPOUND_EXPR, void_type_node, call, set),
		    NULL_TREE);
  SET_EXPR_LOCATION(ret, location);

  if (at_end_of_function)
    {
      tree queue_head = this->queue_head_decl(gogo);
      tree decl = this->get_tree(gogo);
      tree field = TYPE_FIELDS(TREE_TYPE(decl));
      gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)),
			"__caller") == 0);
      tree field_ref = build3(COMPONENT_REF, TREE_TYPE(field), decl, field,
			      NULL_TREE);
      tree set_head = build2(MODIFY_EXPR, void_type_node, queue_head,
			     field_ref);
      SET_EXPR_LOCATION(set_head, location);
      ret = build2(COMPOUND_EXPR, void_type_node, ret, set_head);
    }

  return ret;
}

// Class Refcount_traverse_expressions.

// Gather expressions which need to have their reference count
// decremented after a statement is complete.  This is called for
// expressions which are not in themselves lvalues or rvalues.

class Refcount_traverse_expressions : public Traverse
{
 public:
  Refcount_traverse_expressions(Refcounts* refcounts,
				Expression* skip)
    : Traverse(traverse_blocks | traverse_expressions),
      refcounts_(refcounts), skip_(skip)
  { }

  void
  set_skip(Expression* skip)
  { this->skip_ = skip; }

 protected:
  // We only want to look at expressions which are the top level of a
  // statement.  If a statement includes a block, we do not want to
  // look at the expressions in the statements in the block.  For
  // example, we want to look at the condition of an If_statement, not
  // the expressions in the statements in the then and else blocks.
  int
  block(Block*)
  { return TRAVERSE_SKIP_COMPONENTS; }

  int
  expression(Expression**);

 private:
  // Temporary values which need decrementing.
  Refcounts* refcounts_;
  // An Expression to skip, because it occurs as the complete RHS.
  Expression* skip_;
};

int
Refcount_traverse_expressions::expression(Expression** pexpr)
{
  Expression* expr = *pexpr;

  if (expr == this->skip_)
    return TRAVERSE_CONTINUE;

  // If EXPR is a constant, we don't need to increment any reference
  // counts.  We check here to catch cases like &global_variable,
  // where we can tell that is a constant when we take the address,
  // but not if we recurse down to look at the global variable.
  if (expr->is_constant())
    return TRAVERSE_SKIP_COMPONENTS;

  *pexpr = expr->note_decrements(this->refcounts_);

  return TRAVERSE_CONTINUE;
}

// Class Refcount_traverse_assignments.

// For each assignment in a statement, gather values which need
// reference count increments and decrements.

class Refcount_traverse_assignments : public Traverse_assignments
{
 public:
  Refcount_traverse_assignments(Refcounts* refcounts,
				Expression_list* do_not_increment)
    : refcounts_(refcounts), do_not_increment_(do_not_increment)
  { }

 protected:
  void
  initialize_variable(Named_object*);

  void
  assignment(Expression**, Expression**);

  void
  value(Expression**, bool, bool);

 private:
  static bool
  holds_only_arg_vars(Expression* expr);

  // Temporary values which need decrementing.
  Refcounts* refcounts_;
  // A list of expressions whose reference counts should not be
  // incremented.  This may be NULL.
  Expression_list* do_not_increment_;
};

// Handle reference counts for a variable initialization.

void
Refcount_traverse_assignments::initialize_variable(Named_object* var)
{
  Expression* init = var->var_value()->init();
  if (init != NULL)
    {
      Expression* init_hold = init;
      if (!var->var_value()->holds_only_args()
	  || !Refcount_traverse_assignments::holds_only_arg_vars(init))
	init = init->being_copied(this->refcounts_,
				  !var->var_value()->is_global());
      Refcount_traverse_expressions rte(this->refcounts_, NULL);
      Expression::traverse(&init, &rte);
      if (init != init_hold)
	var->var_value()->set_init(init);
    }
}

// Return whether EXPR is a variable which only holds argument values.

bool
Refcount_traverse_assignments::holds_only_arg_vars(Expression* expr)
{
  if (expr == NULL)
    return false;
  Var_expression* ve = expr->var_expression();
  if (ve == NULL)
    return false;
  Named_object* no = ve->named_object();
  return no->is_variable() && no->var_value()->holds_only_args();
}

// Handle reference counts for an assignment in a statement.

void
Refcount_traverse_assignments::assignment(Expression** plhs, Expression** prhs)
{
  if (!Refcount_traverse_assignments::holds_only_arg_vars(*plhs)
      || !Refcount_traverse_assignments::holds_only_arg_vars(*prhs))
    {
      bool is_local = (*plhs)->is_local_variable();
      if (plhs != NULL)
	*plhs = (*plhs)->being_set(this->refcounts_);
      if (prhs != NULL)
	*prhs = (*prhs)->being_copied(this->refcounts_,
				      is_local);
    }
  Refcount_traverse_expressions rte(this->refcounts_, NULL);
  if (plhs != NULL)
    {
      rte.set_skip(*plhs);
      Expression::traverse(plhs, &rte);
    }
  if (prhs != NULL)
    {
      rte.set_skip(NULL);
      Expression::traverse(prhs, &rte);
    }
}

// Handle reference counts for a value used in an assignment.

void
Refcount_traverse_assignments::value(Expression** pval, bool is_stored,
				     bool is_local)
{
  if (is_stored)
    {
      bool increment = true;
      if (this->do_not_increment_ != NULL)
	{
	  Expression_list::iterator p =
	    std::find(this->do_not_increment_->begin(),
		      this->do_not_increment_->end(),
		      *pval);
	  if (p != this->do_not_increment_->end())
	    {
	      this->do_not_increment_->erase(p);
	      increment = false;
	    }
	}

      if (increment)
	*pval = (*pval)->being_copied(this->refcounts_,
				      is_local);
    }

  Refcount_traverse_expressions rte(this->refcounts_, NULL);
  Expression::traverse(pval, &rte);
}

// Class Refcount_traverse_statements.

// Find statements which need reference count adjustments.

class Refcount_traverse_statements : public Traverse
{
 public:
  Refcount_traverse_statements(Refcounts* refcounts)
    : Traverse(traverse_functions | traverse_blocks | traverse_statements),
      refcounts_(refcounts)
  { }

 protected:
  int
  function(Named_object*)
  { return TRAVERSE_SKIP_COMPONENTS; }

  int
  block(Block*);

  int
  statement(Block*, size_t* pindex, Statement*);

 private:
  // Reference count entries for this function.
  Refcounts* refcounts_;
};

// Adjust reference counts for a block.  This is where we decrement
// the reference count for local variables.

int
Refcount_traverse_statements::block(Block* b)
{
  // If the block ends with a return statement, build a list of
  // variables which are returned.  Use the list to optimize returning
  // a reference counted local variable.  FIXME: This is a very
  // simple-minded version of this optimization.
  std::vector<Named_object*> returned;
  Return_statement* r = NULL;
  const std::vector<Statement*>* statements = b->statements();
  if (!statements->empty())
    {
      r = statements->back()->return_statement();
      if (r != NULL)
	{
	  const Expression_list* vals = r->vals();
	  if (vals != NULL)
	    {
	      for (Expression_list::const_iterator pv = vals->begin();
		   pv != vals->end();
		   ++pv)
		{
		  Var_expression* ve = (*pv)->var_expression();
		  if (ve != NULL)
		    {
		      Named_object* no = ve->named_object();
		      if (std::find(returned.begin(), returned.end(), no)
			  == returned.end())
			returned.push_back(no);
		    }
		}
	    }
	}
    }

  Bindings* bindings = b->bindings();
  for (Bindings::const_definitions_iterator p = bindings->begin_definitions();
       p != bindings->end_definitions();
       ++p)
    {
      // If we find a parameter which does not only hold parameter
      // values, we need to queue up a reference count increment at
      // the start of the function.
      if ((*p)->is_variable()
	  && (*p)->var_value()->is_parameter()
	  && !(*p)->var_value()->holds_only_args()
	  && (*p)->var_value()->type()->has_refcounted_component())
	{
	  source_location loc = (*p)->location();
	  Refcounts* rc = this->refcounts_;
	  Refcount_entry re = rc->add(REFCOUNT_INCREMENT_COPIED,
				      (*p)->var_value()->type());
	  Expression* rhs = Expression::make_var_reference(*p, loc);
	  Statement* s =
	    Statement::make_refcount_queue_assignment_statement(rc, &re, rhs,
								loc);
	  b->add_statement_at_front(s);
	}

      // We need to queue up reference count decrements for all local
      // variables at the end of the block.
      if ((*p)->is_variable()
	  && ((*p)->var_value()->is_in_heap()
	      || (!(*p)->var_value()->holds_only_args()
		  && (*p)->var_value()->type()->has_refcounted_component())))
	{
	  std::vector<Named_object*>::iterator prv =
	    std::find(returned.begin(), returned.end(), *p);
	  if (prv == returned.end())
	    {
	      source_location loc = b->end_location();
	      Refcounts* rc = this->refcounts_;
	      Type* type = (*p)->var_value()->type();
	      const bool is_in_heap = (*p)->var_value()->is_in_heap();
	      if (is_in_heap)
		type = Type::make_pointer_type(type);
	      Refcount_entry re = rc->add(REFCOUNT_DECREMENT_OLD, type);
	      Expression* rhs = Expression::make_var_reference(*p, loc);
	      if (is_in_heap)
		rhs = Expression::make_unary(OPERATOR_AND, rhs, loc);
	      Statement* s =
		Statement::make_refcount_queue_assignment_statement(rc, &re,
								    rhs, loc);
	      b->add_final_statement(s);
	    }
	  else
	    {
	      r->add_do_not_increment(*p);
	      returned.erase(prv);
	    }
	}
    }
  return TRAVERSE_CONTINUE;
}

// Adjust reference counts for a statement.

int
Refcount_traverse_statements::statement(Block* block, size_t* pindex,
					Statement* s)
{
  if (s->is_block_statement())
    return TRAVERSE_CONTINUE;

  // Simple avoidance of reference count adjustments when returning a
  // local variable.
  Expression_list* do_not_increment = NULL;
  Return_statement* r = s->return_statement();
  if (r != NULL)
    do_not_increment = r->do_not_increment();

  Refcount_traverse_assignments rta(this->refcounts_, do_not_increment);
  if (!s->traverse_assignments(&rta))
    {
      Refcount_traverse_expressions rte(this->refcounts_, NULL);
      s->traverse(block, pindex, &rte);
    }

  gcc_assert(do_not_increment == NULL || do_not_increment->empty());

  return TRAVERSE_CONTINUE;
}

// Class Refcount_traverse_functions.

// Walk over the functions adding reference count adjustments.

class Refcount_traverse_functions : public Traverse
{
 public:
  Refcount_traverse_functions()
    : Traverse(traverse_functions)
  { }

 protected:
  int
  function(Named_object*);
};

int
Refcount_traverse_functions::function(Named_object* no)
{
  Refcounts* refcounts = no->func_value()->refcounts();
  Refcount_traverse_statements rts(refcounts);
  no->func_value()->block()->traverse(&rts);
  refcounts->set_is_complete();
  return TRAVERSE_CONTINUE;
}

// Walk the tree adding reference count adjustments.

void
Gogo::add_refcounts()
{
  Refcount_traverse_functions rtf;
  this->traverse(&rtf);
}
