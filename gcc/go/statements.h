// statements.h -- Go frontend statements.     -*- C++ -*-

// Copyright 2009 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef GO_STATEMENTS_H
#define GO_STATEMENTS_H

#include <tr1/unordered_set>

#include "operator.h"

class Gogo;
class Traverse;
class Block;
class Function;
class Return_statement;
class Thunk_statement;
class Label_statement;
class For_statement;
class For_range_statement;
class Switch_statement;
class Type_switch_statement;
class Select_statement;
class Variable;
class Named_object;
class Label;
class Translate_context;
class Expression;
class Expression_list;
class Struct_type;
class Call_expression;
class Map_index_expression;
class Receive_expression;
class Case_clauses;
class Type_case_clauses;
class Select_clauses;
class Refcounts;
class Refcount_entry;

// This class is used to traverse assignments made by a statement
// which makes assignments.

class Traverse_assignments
{
 public:
  Traverse_assignments()
  { }

  virtual ~Traverse_assignments()
  { }

  // This is called for a variable initialization.
  virtual void
  initialize_variable(Named_object*) = 0;

  // This is called for each assignment made by the statement.  PLHS
  // points to the left hand side, and PRHS points to the right hand
  // side.  PRHS may be NULL if there is no associated expression, as
  // in the bool set by a non-blocking receive.
  virtual void
  assignment(Expression** plhs, Expression** prhs) = 0;

  // This is called for each expression which is not passed to the
  // assignment function.  This is used for some of the statements
  // which assign two values, for which there is no expression which
  // describes the value.  For ++ and -- the value is passed to both
  // the assignment method and the rhs method.  IS_STORED is true if
  // this value is being stored directly.  It is false if the value is
  // computed but not stored.  IS_LOCAL is true if the value is being
  // stored in a local variable or this is being called by a return
  // statement.
  virtual void
  value(Expression**, bool is_stored, bool is_local) = 0;
};

// A single statement.

class Statement
{
 public:
  // The types of statements.
  enum Statement_classification
  {
    STATEMENT_ERROR,
    STATEMENT_VARIABLE_DECLARATION,
    STATEMENT_ASSIGNMENT,
    STATEMENT_TUPLE_ASSIGNMENT,
    STATEMENT_TUPLE_MAP_ASSIGNMENT,
    STATEMENT_MAP_ASSIGNMENT,
    STATEMENT_TUPLE_RECEIVE_ASSIGNMENT,
    STATEMENT_TUPLE_TYPE_GUARD_ASSIGNMENT,
    STATEMENT_EXPRESSION,
    STATEMENT_BLOCK,
    STATEMENT_INCDEC,
    STATEMENT_GO,
    STATEMENT_DEFER,
    STATEMENT_RETURN,
    STATEMENT_BREAK_OR_CONTINUE,
    STATEMENT_GOTO,
    STATEMENT_LABEL,
    STATEMENT_IF,
    STATEMENT_SWITCH,
    STATEMENT_TYPE_SWITCH,
    STATEMENT_SELECT,
    STATEMENT_FOR,
    STATEMENT_FOR_RANGE,
    STATEMENT_REFCOUNT_QUEUE_ASSIGNMENT
  };

  Statement(Statement_classification, source_location);

  virtual ~Statement();

  // Make a variable declaration.
  static Statement*
  make_variable_declaration(Named_object*);

  // Make an assignment statement.
  static Statement*
  make_assignment(Operator, Expression*, Expression*, source_location);

  // Make a tuple assignment statement.
  static Statement*
  make_tuple_assignment(Operator, Expression_list*, Expression_list*,
			source_location);

  // Make an assignment from a map index to a pair of variables.
  static Statement*
  make_tuple_map_assignment(Expression* val, Expression* present,
			    Expression*, source_location);

  // Make a statement which assigns a pair of values to a map.
  static Statement*
  make_map_assignment(Expression*, Expression* val,
		      Expression* should_set, source_location);

  // Make an assignment from a nonblocking receive to a pair of
  // variables.
  static Statement*
  make_tuple_receive_assignment(Expression* val, Expression* success,
				Expression* channel, source_location);

  // Make an assignment from a type guard to a pair of variables.
  static Statement*
  make_tuple_type_guard_assignment(Expression* val, Expression* ok,
				   Expression* expr, Type* type,
				   source_location);

  // Make an expression statement from an Expression.
  static Statement*
  make_statement(Expression*);

  // Make a block statement from a Block.  This is an embedded list of
  // statements which may also include variable definitions.
  static Statement*
  make_block_statement(Block*, source_location);

  // Make an increment statement.
  static Statement*
  make_inc_statement(Expression*);

  // Make a decrement statement.
  static Statement*
  make_dec_statement(Expression*);

  // Make a go statement.
  static Statement*
  make_go_statement(Call_expression* call, source_location);

  // Make a defer statement.
  static Statement*
  make_defer_statement(Call_expression* call, source_location);

  // Make a return statement.
  static Statement*
  make_return_statement(const Function*, Expression_list*, source_location);

  // Make a break statement.
  static Statement*
  make_break_statement(Statement* enclosing, source_location);

  // Make a continue statement.
  static Statement*
  make_continue_statement(Statement* enclosing, source_location);

  // Make a goto statement.
  static Statement*
  make_goto_statement(Label* label, source_location);

  // Make a label statement.
  static Statement*
  make_label_statement(Label* label, source_location);

  // Make an if statement.
  static Statement*
  make_if_statement(Expression* cond, Block* then_block, Block* else_block,
		    source_location);

  // Make a switch statement.
  static Switch_statement*
  make_switch_statement(Expression* switch_val, source_location);

  // Make a type switch statement.
  static Type_switch_statement*
  make_type_switch_statement(Named_object* var, Expression*, source_location);

  // Make a select statement.
  static Select_statement*
  make_select_statement(source_location);

  // Make a for statement.
  static For_statement*
  make_for_statement(Block* init, Expression* cond, Block* post,
		     source_location location);

  // Make a for statement with a range clause.
  static For_range_statement*
  make_for_range_statement(Expression* index_var, Expression* value_var,
			   Expression* range, source_location);

  // Make a statement which stores a value in the reference count
  // queue.
  static Statement*
  make_refcount_queue_assignment_statement(Refcounts*, Refcount_entry*,
					   Expression*, source_location);

  // Return the statement classification.
  Statement_classification
  classification() const
  { return this->classification_; }

  // Get the statement location.
  source_location
  location() const
  { return this->location_; }

  // Traverse the tree.
  int
  traverse(Block*, size_t* index, Traverse*);

  // If this statement assigns some values, it calls a function for
  // each value to which this statement assigns a value, and returns
  // true.  If this statement does not assign any values, it returns
  // false.
  bool
  traverse_assignments(Traverse_assignments* tassign);

  // Lower a statement.  This is called immediately after parsing to
  // simplify statements for further processing.  It returns the same
  // Statement or a new one.
  Statement*
  lower(Gogo* gogo)
  { return this->do_lower(gogo); }

  // Set type information for unnamed constants.
  void
  determine_types();

  // Check types in a statement.  This simply checks that any
  // expressions used by the statement have the right type.
  void
  check_types(Gogo* gogo)
  { this->do_check_types(gogo); }

  // Return whether this is a block statement.
  bool
  is_block_statement() const
  { return this->classification_ == STATEMENT_BLOCK; }

  // If this is a return statement, return it.  Otherwise return NULL.
  Return_statement*
  return_statement()
  { return this->convert<Return_statement, STATEMENT_RETURN>(); }

  // If this is a thunk statement (a go or defer statement), return
  // it.  Otherwise return NULL.
  Thunk_statement*
  thunk_statement();

  // If this is a label statement, return it.  Otherwise return NULL.
  Label_statement*
  label_statement()
  { return this->convert<Label_statement, STATEMENT_LABEL>(); }

  // If this is a for statement, return it.  Otherwise return NULL.
  For_statement*
  for_statement()
  { return this->convert<For_statement, STATEMENT_FOR>(); }

  // If this is a for statement over a range clause, return it.
  // Otherwise return NULL.
  For_range_statement*
  for_range_statement()
  { return this->convert<For_range_statement, STATEMENT_FOR_RANGE>(); }

  // If this is a switch statement, return it.  Otherwise return NULL.
  Switch_statement*
  switch_statement()
  { return this->convert<Switch_statement, STATEMENT_SWITCH>(); }

  // If this is a type switch statement, return it.  Otherwise return
  // NULL.
  Type_switch_statement*
  type_switch_statement()
  { return this->convert<Type_switch_statement, STATEMENT_TYPE_SWITCH>(); }

  // If this is a select statement, return it.  Otherwise return NULL.
  Select_statement*
  select_statement()
  { return this->convert<Select_statement, STATEMENT_SELECT>(); }

  // Return true if this statement may fall through--if after
  // executing this statement we may go on to execute the following
  // statement, if any.
  bool
  may_fall_through() const
  { return this->do_may_fall_through(); }

  // Return the tree for a statement.  BLOCK is the enclosing block.
  tree
  get_tree(Translate_context*);

 protected:
  // Implemented by child class: traverse the tree.
  virtual int
  do_traverse(Traverse*) = 0;

  // Implemented by child class: traverse assignments.  Any statement
  // which includes an assignment should implement this.
  virtual bool
  do_traverse_assignments(Traverse_assignments*)
  { return false; }

  // Implemented by the child class: lower this statement to a simpler
  // one.
  virtual Statement*
  do_lower(Gogo*)
  { return this; }

  // Implemented by child class: set type information for unnamed
  // constants.  Any statement which includes an expression needs to
  // implement this.
  virtual void
  do_determine_types()
  { }

  // Implemented by child class: check types of expressions used in a
  // statement.
  virtual void
  do_check_types(Gogo*)
  { }

  // Implemented by child class: return true if this statement may
  // fall through.
  virtual bool
  do_may_fall_through() const
  { return true; }

  // Implemented by child class: return a tree.
  virtual tree
  do_get_tree(Translate_context*) = 0;

  // Traverse an expression in a statement.
  int
  traverse_expression(Traverse*, Expression**);

  // Traverse an expression list in a statement.  The Expression_list
  // may be NULL.
  int
  traverse_expression_list(Traverse*, Expression_list*);

  // Traverse a type in a statement.
  int
  traverse_type(Traverse*, Type*);

  // Build a tree node with one operand, setting the location.  The
  // first operand really has type "enum tree_code", but that enum is
  // not defined here.
  tree
  build_stmt_1(int tree_code_value, tree);

  // For children to call when they detect that they are in error.
  void
  set_is_error();

  // For children to call to report an error conveniently.
  void
  report_error(const char*);

 private:
  // Convert to the desired statement classification, or return NULL.
  // This is a controlled dynamic cast.
  template<typename Statement_class, Statement_classification sc>
  Statement_class*
  convert()
  {
    return (this->classification_ == sc
	    ? static_cast<Statement_class*>(this)
	    : NULL);
  }

  template<typename Statement_class, Statement_classification sc>
  const Statement_class*
  convert() const
  {
    return (this->classification_ == sc
	    ? static_cast<const Statement_class*>(this)
	    : NULL);
  }

  static Statement*
  make_error_statement(source_location);

  // The statement classification.
  Statement_classification classification_;
  // The location in the input file of the start of this statement.
  source_location location_;
};

// A return statement.

class Return_statement : public Statement
{
 public:
  Return_statement(const Function* function, Expression_list* vals,
		   source_location location)
    : Statement(STATEMENT_RETURN, location),
      function_(function), vals_(vals), do_not_increment_(NULL)
  { }

  // The list of values being returned.  This may be NULL.
  const Expression_list*
  vals() const
  { return this->vals_; }

  // Return list of expressions whose reference count should not be
  // incremented.  This is modified by the caller.
  Expression_list*
  do_not_increment()
  { return this->do_not_increment_; }

  // Indicate that we should not increment the reference count of the
  // variable.  The variable is being returned by this statement.
  void
  add_do_not_increment(Named_object*);

 protected:
  int
  do_traverse(Traverse* traverse)
  { return this->traverse_expression_list(traverse, this->vals_); }

  bool
  do_traverse_assignments(Traverse_assignments*);

  void
  do_determine_types();

  void
  do_check_types(Gogo*);

  bool
  do_may_fall_through() const
  { return false; }

  tree
  do_get_tree(Translate_context*);

 private:
  // The function we are returning from.  We use this to get the
  // return types.
  const Function* function_;
  // Return values.  This may be NULL.
  Expression_list* vals_;
  // List of variables whose reference count should not be
  // incremented.  This may be NULL.  This is only used during the
  // reference counting pass.
  Expression_list* do_not_increment_;
};

// Select_clauses holds the clauses of a select statement.  This is
// built by the parser.

class Select_clauses
{
 public:
  Select_clauses()
    : clauses_()
  { }

  // Add a new clause.  IS_SEND is true if this is a send clause,
  // false for a receive clause.  For a send clause CHANNEL is the
  // channel and VAL is the value to send.  For a receive clause
  // CHANNEL is the channel and VAL is either NULL or a Var_expression
  // for the variable to set; if VAL is NULL, VAR may be a variable
  // which is initialized with the received value.  IS_DEFAULT is true
  // if this is the default clause.  STATEMENTS is the list of
  // statements to execute.
  void
  add(bool is_send, Expression* channel, Expression* val, Named_object* var,
      bool is_default, Block* statements, source_location location)
  {
    this->clauses_.push_back(Select_clause(is_send, channel, val, var,
					   is_default, statements, location));
  }

  // Traverse the select clauses.
  int
  traverse(Traverse*);

  // Traverse assignments in the select clauses.
  void
  traverse_assignments(Traverse_assignments*);

  // Determine types.
  void
  determine_types();

  // Check types.
  bool
  check_types(Gogo*);

  // Whether the select clauses may fall through to the statement
  // which follows the overall select statement.
  bool
  may_fall_through() const;

  // Return a tree implementing the select statement.
  tree
  get_tree(Translate_context*, tree break_label, source_location);

 private:
  // A single clause.
  class Select_clause
  {
   public:
    Select_clause()
      : channel_(NULL), val_(NULL), var_(NULL), statements_(NULL),
	is_send_(false), is_default_(false)
    { }

    Select_clause(bool is_send, Expression* channel, Expression* val,
		  Named_object* var, bool is_default, Block* statements,
		  source_location location)
      : channel_(channel), val_(val), var_(var), statements_(statements),
	location_(location), is_send_(is_send), is_default_(is_default)
    { gcc_assert(is_default ? channel == NULL : channel != NULL); }

    // Traverse the select clause.
    int
    traverse(Traverse*);

    // Traverse assignments in the select clause.
    void
    traverse_assignments(Traverse_assignments*);

    // Determine types.
    void
    determine_types();

    // Check types.
    bool
    check_types(Gogo*);

    // Return true if this is the default clause.
    bool
    is_default() const
    { return this->is_default_; }

    // Return the channel.  This will return NULL for the default
    // clause.
    Expression*
    channel() const
    { return this->channel_; }

    // Return the value.  This will return NULL for the default
    // clause, or for a receive clause for which no value was given.
    Expression*
    val() const
    { return this->val_; }

    // Return the variable to set when a receive clause is also a
    // variable definition (v := <- ch).  This will return NULL for
    // the default case, or for a send clause, or for a receive clause
    // which does not define a variable.
    Named_object*
    var() const
    { return this->var_; }

    // Return true for a send, false for a receive.
    bool
    is_send() const
    {
      gcc_assert(!this->is_default_);
      return this->is_send_;
    }

    // Return the location.
    source_location
    location() const
    { return this->location_; }

    // Whether this clause may fall through to the statement which
    // follows the overall select statement.
    bool
    may_fall_through() const;

    // Return a tree for the statements to execute.
    tree
    get_statements_tree(Translate_context*);

   private:
    // The channel.
    Expression* channel_;
    // The value to send or the variable to set.
    Expression* val_;
    // The variable to initialize, for "case a := <- ch".
    Named_object* var_;
    // The statements to execute.
    Block* statements_;
    // The location of this clause.
    source_location location_;
    // Whether this is a send or a receive.
    bool is_send_;
    // Whether this is the default.
    bool is_default_;
  };

  void
  add_clause_tree(Translate_context*, int, Select_clause*, tree, Type*, tree,
		  tree, tree*);

  typedef std::vector<Select_clause> Clauses;

  Clauses clauses_;
};

// A select statement.

class Select_statement : public Statement
{
 public:
  Select_statement(source_location location)
    : Statement(STATEMENT_SELECT, location),
      clauses_(NULL), break_label_(NULL)
  { }

  // Add the clauses.
  void
  add_clauses(Select_clauses* clauses)
  {
    gcc_assert(this->clauses_ == NULL);
    this->clauses_ = clauses;
  }

  // Rcord that a break statement is used for this select statement.
  // This is called during parsing.  Actually we always need a break
  // label, so we don't care about this.
  void
  set_needs_break_label()
  { }

  // Return the break LABEL_EXPR for this select statement.  This is
  // called when converting to GENERIC.
  tree
  break_label();

 protected:
  int
  do_traverse(Traverse* traverse)
  { return this->clauses_->traverse(traverse); }

  bool
  do_traverse_assignments(Traverse_assignments*);

  void
  do_determine_types()
  { this->clauses_->determine_types(); }

  void
  do_check_types(Gogo* gogo);

  bool
  do_may_fall_through() const
  { return this->clauses_->may_fall_through(); }

  tree
  do_get_tree(Translate_context*);

 private:
  // The select clauses.
  Select_clauses* clauses_;
  // The break LABEL_EXPR.
  tree break_label_;
};

// A statement which requires a thunk: go or defer.

class Thunk_statement : public Statement
{
 public:
  Thunk_statement(Statement_classification, Call_expression*,
		  source_location);

  // Return the call expression.
  Expression*
  call()
  { return this->call_; }

  // Simplify a go or defer statement so that it only uses a single
  // parameter.
  bool
  simplify_statement(Gogo*, Block*);

  // Return whether a function is a thunk.
  static bool
  is_thunk(const Named_object*);

 protected:
  int
  do_traverse(Traverse* traverse);

  bool
  do_traverse_assignments(Traverse_assignments*);

  void
  do_determine_types();

  void
  do_check_types(Gogo*);

  // Return the function and argument trees for the call.
  void
  get_fn_and_arg(Translate_context*, tree* pfn, tree* parg);

 private:
  // An counter to generate new thunk names.
  static int thunk_count;

  // Return whether this is a simple go statement.
  bool
  is_simple(Function_type*) const;

  // Build the struct to use for a complex case.
  Struct_type*
  build_struct(Function_type* fntype);

  // Build the thunk.
  void
  build_thunk(Gogo*, const char*, Function_type* fntype);

  // The field name used in the thunk structure for the function
  // pointer.
  static const char* const thunk_field_fn;

  // The field name used in the thunk structure for the receiver, if
  // there is one.
  static const char* const thunk_field_receiver;

  // Set the name to use for thunk field N.
  void
  thunk_field_param(int n, char* buf, size_t buflen);

  // The function call to be executed in a separate thread (go) or
  // later (defer).
  Expression* call_;
  // The type used for a struct to pass to a thunk, if this is not a
  // simple call.
  Struct_type* struct_type_;
};

// A go statement.

class Go_statement : public Thunk_statement
{
 public:
  Go_statement(Call_expression* call, source_location location)
    : Thunk_statement(STATEMENT_GO, call, location)
  { }

 protected:
  tree
  do_get_tree(Translate_context*);
};

// A defer statement.

class Defer_statement : public Thunk_statement
{
 public:
  Defer_statement(Call_expression* call, source_location location)
    : Thunk_statement(STATEMENT_DEFER, call, location)
  { }

 protected:
  tree
  do_get_tree(Translate_context*);
};

// A label statement.

class Label_statement : public Statement
{
 public:
  Label_statement(Label* label, source_location location)
    : Statement(STATEMENT_LABEL, location),
      label_(label)
  { }

  // Return the label itself.
  const Label*
  label() const
  { return this->label_; }

 protected:
  int
  do_traverse(Traverse*);

  tree
  do_get_tree(Translate_context*);

 private:
  // The label.
  Label* label_;
};

// A for statement.

class For_statement : public Statement
{
 public:
  For_statement(Block* init, Expression* cond, Block* post,
		source_location location)
    : Statement(STATEMENT_FOR, location),
      init_(init), cond_(cond), post_(post), statements_(NULL),
      break_label_(NULL), continue_label_(NULL), needs_break_label_(false),
      needs_continue_label_(false)
  { }

  // Add the statements.
  void
  add_statements(Block* statements)
  {
    gcc_assert(this->statements_ == NULL);
    this->statements_ = statements;
  }

  // Record that a break statement is used for this for statement.
  // This is called during parsing.
  void
  set_needs_break_label()
  { this->needs_break_label_ = true; }

  // Record that a continue statement is used for this for statement.
  // This is called during parsing.
  void
  set_needs_continue_label()
  { this->needs_continue_label_ = true; }

  // Return the break LABEL_EXPR for this for statement.  This is
  // called when converting to GENERIC.
  tree
  break_label();

  // Return the continue LABEL_EXPR for this for statement.  This is
  // called when converting to GENERIC.
  tree
  continue_label();

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
  // The initialization statements.  This may be NULL.
  Block* init_;
  // The condition.  This may be NULL.
  Expression* cond_;
  // The statements to run after each iteration.  This may be NULL.
  Block* post_;
  // The statements in the loop itself.
  Block* statements_;
  // The break LABEL_EXPR, if needed.
  tree break_label_;
  // The continue LABEL_EXPR, if needed.
  tree continue_label_;
  // True if we need a break label.
  bool needs_break_label_;
  // True if we need a continue label.
  bool needs_continue_label_;
};

// A for statement over a range clause.

class For_range_statement : public Statement
{
 public:
  For_range_statement(Expression* index_var, Expression* value_var,
		      Expression* range, source_location location)
    : Statement(STATEMENT_FOR_RANGE, location),
      index_var_(index_var), value_var_(value_var), range_(range),
      statements_(NULL), break_label_(NULL), continue_label_(NULL),
      needs_break_label_(false), needs_continue_label_(false)
  { }

  // Add the statements.
  void
  add_statements(Block* statements)
  {
    gcc_assert(this->statements_ == NULL);
    this->statements_ = statements;
  }

  // Record that a break statement is used for this for statement.
  // This is called during parsing.
  void
  set_needs_break_label()
  { this->needs_break_label_ = true; }

  // Record that a continue statement is used for this for statement.
  // This is called during parsing.
  void
  set_needs_continue_label()
  { this->needs_continue_label_ = true; }

  // Return the break LABEL_EXPR for this for statement.
  tree
  break_label();

  // Return the continue LABEL_EXPR for this for statement.
  tree
  continue_label();

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
  void
  get_array_iteration(Translate_context*, tree, tree, tree, tree*, tree*,
		      tree*, tree*);

  void
  get_string_iteration(Translate_context*, tree, tree, tree, tree*, tree*,
		       tree*, tree*);

  void
  get_map_iteration(Translate_context*, tree, tree, tree, tree*, tree*,
		    tree*, tree*);

  void
  get_channel_iteration(Translate_context*, tree, tree, tree*, tree*,tree*,
			tree*);

  // The variable which is set to the index value.
  Expression* index_var_;
  // The variable which is set to the element value.  This may be
  // NULL.
  Expression* value_var_;
  // The expression we are ranging over.
  Expression* range_;
  // The statements in the block.
  Block* statements_;
  // The break LABEL_EXPR, if needed.
  tree break_label_;
  // The continue LABEL_EXPR, if needed.
  tree continue_label_;
  // True if we need a break label.
  bool needs_break_label_;
  // True if we need a continue label.
  bool needs_continue_label_;
};

// Class Case_clauses holds the clauses of a switch statement.  This
// is built by the parser.

class Case_clauses
{
 public:
  Case_clauses()
    : clauses_()
  { }

  // Add a new clause.  CASES is a list of case expressions; it may be
  // NULL.  IS_DEFAULT is true if this is the default case.
  // STATEMENTS is a block of statements.  IS_FALLTHROUGH is true if
  // after the statements the case clause should fall through to the
  // next clause.
  void
  add(Expression_list* cases, bool is_default, Block* statements,
      bool is_fallthrough, source_location location)
  {
    this->clauses_.push_back(Case_clause(cases, is_default, statements,
					 is_fallthrough, location));
  }

  // Return whether there are no clauses.
  bool
  empty() const
  { return this->clauses_.empty(); }

  // Traverse the case clauses.
  int
  traverse(Traverse*);

  // Determine types of expressions.  The Type parameter is the type
  // of the switch value.
  void
  determine_types(Type*);

  // Check types.  The Type parameter is the type of the switch value.
  bool
  check_types(Gogo*, Type*);

  // Return true if all the clauses are constant values.
  bool
  is_constant() const;

  // Return true if these clauses may fall through to the statements
  // following the switch statement.
  bool
  may_fall_through() const;

  // Return the body of a SWITCH_EXPR when all the clauses are
  // constants.
  tree
  get_constant_tree(Translate_context*, tree break_label) const;

  // Build up a statement list when some clauses are not constants.
  void
  get_nonconstant_tree(Translate_context*, Type* switch_val_type,
		       tree switch_val_tree, tree break_label,
		       tree* stmt_list) const;

 private:
  // For a constant tree we need to keep a record of constants we have
  // already seen.  Note that INTEGER_CST trees are interned.
  typedef std::tr1::unordered_set<tree> Case_constants;

  // One case clause.
  class Case_clause
  {
   public:
    Case_clause()
      : cases_(NULL), statements_(NULL), is_default_(false),
	is_fallthrough_(false), location_(UNKNOWN_LOCATION)
    { }

    Case_clause(Expression_list* cases, bool is_default, Block* statements,
		bool is_fallthrough, source_location location)
      : cases_(cases), statements_(statements), is_default_(is_default),
	is_fallthrough_(is_fallthrough), location_(location)
    { }

    // Whether this clause falls through to the next clause.
    bool
    is_fallthrough() const
    { return this->is_fallthrough_; }

    // Whether this is the default.
    bool
    is_default() const
    { return this->is_default_; }

    // The location of this clause.
    source_location
    location() const
    { return this->location_; }

    // Traversal.
    int
    traverse(Traverse*);

    // Determine types.
    void
    determine_types(Type*);

    // Check types.
    bool
    check_types(Gogo*, Type*);

    // Return true if all the case expressions are constant.
    bool
    is_constant() const;

    // Return true if this clause may fall through to execute the
    // statements following the switch statement.  This is not the
    // same as whether this clause falls through to the next clause.
    bool
    may_fall_through() const;

    // Build up the body of a SWITCH_EXPR when the case expressions
    // are constant.
    void
    get_constant_tree(Translate_context*, tree break_label,
		      Case_constants* case_constants, tree* stmt_list) const;

    // Build up a statement list when some clauses are not constants.
    void
    get_nonconstant_tree(Translate_context*, Type* switch_val_type,
			 tree switch_val_tree, tree start_label,
			 tree finish_label, tree* stmt_list) const;

   private:
    // The list of case expressions.
    Expression_list* cases_;
    // The statements to execute.
    Block* statements_;
    // Whether this is the default case.
    bool is_default_;
    // Whether this falls through after the statements.
    bool is_fallthrough_;
    // The location of this case clause.
    source_location location_;
  };

  friend class Case_clause;

  // The type of the list of clauses.
  typedef std::vector<Case_clause> Clauses;

  // All the case clauses.
  Clauses clauses_;
};

// A switch statement.

class Switch_statement : public Statement
{
 public:
  Switch_statement(Expression* val, source_location location)
    : Statement(STATEMENT_SWITCH, location),
      val_(val), clauses_(NULL), break_label_(NULL),
      needs_break_label_(false)
  { }

  // Add the clauses.
  void
  add_clauses(Case_clauses* clauses)
  {
    gcc_assert(this->clauses_ == NULL);
    this->clauses_ = clauses;
  }

  // Record that a break statement is used for this switch statement.
  // This is called during parsing.
  void
  set_needs_break_label()
  { this->needs_break_label_ = true; }

  // Return the break LABEL_EXPR for this switch statement.  This is
  // called when converting to GENERIC.
  tree
  break_label();

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
  // The value to switch on.  This may be NULL.
  Expression* val_;
  // The case clauses.
  Case_clauses* clauses_;
  // The break LABEL_EXPR, if needed.
  tree break_label_;
  // True if we need a break label.
  bool needs_break_label_;
};

// Class Type_case_clauses holds the clauses of a type switch
// statement.  This is built by the parser.

class Type_case_clauses
{
 public:
  Type_case_clauses()
    : clauses_()
  { }

  // Add a new clause.  TYPE is the type for this clause; it may be
  // NULL.  IS_FALLTHROUGH is true if this falls through to the next
  // clause; in this case STATEMENTS will be NULL.  IS_DEFAULT is true
  // if this is the default case.  STATEMENTS is a block of
  // statements; it may be NULL.
  void
  add(Type* type, bool is_fallthrough, bool is_default, Block* statements,
      source_location location)
  {
    this->clauses_.push_back(Type_case_clause(type, is_fallthrough, is_default,
					      statements, location));
  }

  // Return whether there are no clauses.
  bool
  empty() const
  { return this->clauses_.empty(); }

  // Traverse the type case clauses.
  int
  traverse(Traverse*);

  // Determine types of expressions.
  void
  determine_types();

  // Check for duplicates.
  void
  check_duplicates() const;

  // Return true if these clauses may fall through to the statements
  // following the type switch statement.
  bool
  may_fall_through() const;

  // Build up a statement list for this type switch statement.
  void
  get_tree(Translate_context*, tree switch_descriptor_tree,
	   tree break_label, tree* stmt_list) const;

 private:
  // One type case clause.
  class Type_case_clause
  {
   public:
    Type_case_clause()
      : type_(NULL), statements_(NULL), is_default_(false),
	location_(UNKNOWN_LOCATION)
    { }

    Type_case_clause(Type* type, bool is_fallthrough, bool is_default,
		     Block* statements, source_location location)
      : type_(type), statements_(statements), is_fallthrough_(is_fallthrough),
	is_default_(is_default), location_(location)
    { }

    // The type.
    Type*
    type() const
    { return this->type_; }

    // Whether this falls through--this is true for "case T1, T2".
    bool
    is_fallthrough() const
    { return this->is_fallthrough_; }

    // Whether this is the default.
    bool
    is_default() const
    { return this->is_default_; }

    // The location of this type clause.
    source_location
    location() const
    { return this->location_; }

    // Traversal.
    int
    traverse(Traverse*);

    // Determine types.
    void
    determine_types();

    // Return true if this clause may fall through to execute the
    // statements following the type switch statement.  This does not
    // mean that this type clause falls through to the next type
    // clause.
    bool
    may_fall_through() const;

    // Build up a statement list.
    void
    get_tree(Translate_context*, tree switch_type_descriptor, tree break_label,
	     tree* stmts_label, tree* stmt_list) const;

   private:
    // The type for this type clause.
    Type* type_;
    // The statements to execute.
    Block* statements_;
    // Whether this falls through.
    bool is_fallthrough_;
    // Whether this is the default case.
    bool is_default_;
    // The location of this type case clause.
    source_location location_;
  };

  friend class Type_case_clause;

  // The type of the list of type clauses.
  typedef std::vector<Type_case_clause> Type_clauses;

  // All the type case clauses.
  Type_clauses clauses_;
};

// A type switch statement.

class Type_switch_statement : public Statement
{
 public:
  Type_switch_statement(Named_object* var, Expression* expr,
			source_location location)
    : Statement(STATEMENT_TYPE_SWITCH, location),
      var_(var), expr_(expr), clauses_(NULL), break_label_(NULL),
      needs_break_label_(false)
  { gcc_assert(var == NULL || expr == NULL); }

  // Add the clauses.
  void
  add_clauses(Type_case_clauses* clauses)
  {
    gcc_assert(this->clauses_ == NULL);
    this->clauses_ = clauses;
  }

  // Record that a break statement is used for this type switch
  // statement.
  void
  set_needs_break_label()
  { this->needs_break_label_ = true; }

  // Return the break LABEL_EXPR for this type switch statement.
  tree
  break_label();

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
  // Get the type descriptor.
  tree
  get_type_descriptor(Translate_context*, Type*, tree);

  // The variable holding the value we are switching on.
  Named_object* var_;
  // The expression we are switching on if there is no variable.
  Expression* expr_;
  // The type case clauses.
  Type_case_clauses* clauses_;
  // The break LABEL_EXPR, if needed.
  tree break_label_;
  // True if we need a break label.
  bool needs_break_label_;
};

#endif // !defined(GO_STATEMENTS_H)
