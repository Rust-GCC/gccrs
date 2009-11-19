// expressions.h -- Go frontend expression handling.     -*- C++ -*-

// Copyright 2009 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef GO_EXPRESSIONS_H
#define GO_EXPRESSIONS_H

#include <gmp.h>
#include <mpfr.h>

#include "operator.h"

class Gogo;
class Translate_context;
class Traverse;
class Type;
struct Type_context;
class Function_type;
class Map_type;
class Expression_list;
class Var_expression;
class String_expression;
class Call_expression;
class Func_expression;
class Unknown_expression;
class Index_expression;
class Map_index_expression;
class Bound_method_expression;
class Field_reference_expression;
class Interface_field_reference_expression;
class Type_guard_expression;
class Receive_expression;
class Refcount_adjust_expression;
class Refcount_decrement_lvalue_expression;
class Named_object;
class Export;
class Import;
class Temporary_initialization_statement;
class Refcounts;
class Refcount_entry;

// The base class for all expressions.

class Expression
{
 public:
  // The types of expressions.
  enum Expression_classification
  {
    EXPRESSION_ERROR,
    EXPRESSION_TYPE,
    EXPRESSION_UNARY,
    EXPRESSION_BINARY,
    EXPRESSION_CONST_REFERENCE,
    EXPRESSION_VAR_REFERENCE,
    EXPRESSION_SINK,
    EXPRESSION_FUNC_REFERENCE,
    EXPRESSION_UNKNOWN_REFERENCE,
    EXPRESSION_BOOLEAN,
    EXPRESSION_STRING,
    EXPRESSION_INTEGER,
    EXPRESSION_FLOAT,
    EXPRESSION_NIL,
    EXPRESSION_IOTA,
    EXPRESSION_CALL,
    EXPRESSION_CALL_RESULT,
    EXPRESSION_BOUND_METHOD,
    EXPRESSION_INDEX,
    EXPRESSION_ARRAY_INDEX,
    EXPRESSION_STRING_INDEX,
    EXPRESSION_MAP_INDEX,
    EXPRESSION_SELECTOR,
    EXPRESSION_FIELD_REFERENCE,
    EXPRESSION_INTERFACE_FIELD_REFERENCE,
    EXPRESSION_ALLOCATION,
    EXPRESSION_MAKE,
    EXPRESSION_TYPE_GUARD,
    EXPRESSION_CONVERSION,
    EXPRESSION_STRUCT_CONSTRUCTION,
    EXPRESSION_FIXED_ARRAY_CONSTRUCTION,
    EXPRESSION_OPEN_ARRAY_CONSTRUCTION,
    EXPRESSION_MAP_CONSTRUCTION,
    EXPRESSION_COMPOSITE_LITERAL,
    EXPRESSION_HEAP_COMPOSITE,
    EXPRESSION_RECEIVE,
    EXPRESSION_SEND,
    EXPRESSION_TEMPORARY_REFERENCE,
    EXPRESSION_REFCOUNT_ADJUST,
    EXPRESSION_REFCOUNT_DECREMENT_LVALUE
  };

  Expression(Expression_classification, source_location);

  virtual ~Expression();

  // Make an error expression.  This is used when a parse error occurs
  // to prevent cascading errors.
  static Expression*
  make_error(source_location);

  // Make an expression which is really a type.  This is used during
  // parsing.
  static Expression*
  make_type(Type*, source_location);

  // Make a unary expression.
  static Expression*
  make_unary(Operator, Expression*, source_location);

  // Make a binary expression.
  static Expression*
  make_binary(Operator, Expression*, Expression*, source_location);

  // Make a reference to a constant in an expression.
  static Expression*
  make_const_reference(Named_object*, source_location);

  // Make a reference to a variable in an expression.
  static Expression*
  make_var_reference(Named_object*, source_location);

  // Make a sink expression--a reference to the blank identifier _.
  static Expression*
  make_sink(source_location);

  // Make a reference to a function in an expression.
  static Expression*
  make_func_reference(Named_object*, Expression* closure, source_location);

  // Make a reference to an unknown name.  In a correct program this
  // will always be lowered to a real const/var/func reference.
  static Expression*
  make_unknown_reference(Named_object*, source_location);

  // Make a constant bool expression.
  static Expression*
  make_boolean(bool val, source_location);

  // Make a constant string expression.
  static Expression*
  make_string(const std::string&, source_location);

  // Make a constant integer expression.  TYPE should be NULL for an
  // abstract type.
  static Expression*
  make_integer(const mpz_t*, Type*, source_location);

  // Make a constant float expression.  TYPE should be NULL for an
  // abstract type.
  static Expression*
  make_float(const mpfr_t*, Type*, source_location);

  // Make a nil expression.
  static Expression*
  make_nil(source_location);

  // Make an iota expression.  This is used for the predeclared
  // constant iota.
  static Expression*
  make_iota();

  // Make a call expression.
  static Call_expression*
  make_call(Expression* func, Expression_list* params, source_location);

  // Make a reference to a specific result of a call expression which
  // returns a tuple.
  static Expression*
  make_call_result(Call_expression*, unsigned int index);

  // Make an expression which is a method bound to its first
  // parameter.
  static Bound_method_expression*
  make_bound_method(Expression* object, Expression* method, source_location);

  // Make an index or slice expression.  This is a parser expression
  // which represents LEFT[START:END].  END may be NULL, meaning an
  // index rather than a slice.  At parse time we may not know the
  // type of LEFT.  After parsing this is lowered to an array index, a
  // string index, or a map index.
  static Expression*
  make_index(Expression* left, Expression* start, Expression* end,
	     source_location);

  // Make an array index expression.  END may be NULL, in which case
  // this is an lvalue.
  static Expression*
  make_array_index(Expression* array, Expression* start, Expression* end,
		   source_location);

  // Make a string index expression.  END may be NULL.  This is never
  // an lvalue.
  static Expression*
  make_string_index(Expression* string, Expression* start, Expression* end,
		    source_location);

  // Make a map index expression.  This is an lvalue.
  static Map_index_expression*
  make_map_index(Expression* map, Expression* val, source_location);

  // Make a selector.  This is a parser expression which represents
  // LEFT.NAME.  At parse time we may not know the type of the left
  // hand side.
  static Expression*
  make_selector(Expression* left, const std::string& name, source_location);

  // Make a reference to a field in a struct.
  static Field_reference_expression*
  make_field_reference(Expression*, unsigned int field_index, source_location);

  // Make a reference to a field of an interface, with an associated
  // object.
  static Expression*
  make_interface_field_reference(Expression*, const std::string&,
				 source_location);

  // Make an allocation expression.
  static Expression*
  make_allocation(Type*, source_location);

  // Make a call to the builtin function make.
  static Expression*
  make_make(Type*, Expression_list*, source_location);

  // Make a type guard expression.
  static Expression*
  make_type_guard(Expression*, Type*, source_location);

  // Make a type cast expression.
  static Expression*
  make_cast(Type*, Expression*, source_location);

  // Make a composit_literal.
  static Expression*
  make_composite_literal(Type*, bool has_keys, Expression_list*,
			 source_location);

  // Make a struct composite literal.
  static Expression*
  make_struct_composite_literal(Struct_type*, Expression_list*,
				source_location);

  // Take a composite literal and allocate it on the heap.
  static Expression*
  make_heap_composite(Expression*, bool for_go_statement, source_location);

  // Make a receive expression.  VAL is NULL for a unary receive.
  static Expression*
  make_receive(Expression* channel, source_location);

  // Make a send expression.
  static Expression*
  make_send(Expression* channel, Expression* val, source_location);

  // Make a reference to a temporary variable created to decrement a
  // reference count.
  static Expression*
  make_temporary_reference(Temporary_initialization_statement*);

  // Make an expression which evaluates another expression and stores
  // the value into the reference queue.  This expression then
  // evaluates to the same value.  FOR_LOCAL is true if this is for a
  // local variable.
  static Expression*
  make_refcount_adjust(Refcounts*, int classification, Expression*,
		       bool for_local);

  // Make an expression which decrements the reference count of the
  // old value of an lvalue.
  static Expression*
  make_refcount_decrement_lvalue(Refcounts*, Expression*);

  // Return the expression classification.
  Expression_classification
  classification() const
  { return this->classification_; }

  // Return the location of the expression.
  source_location
  location() const
  { return this->location_; }

  // Return whether this is a constant expression.
  bool
  is_constant() const
  { return this->do_is_constant(); }

  // If this is not a constant expression with integral type, return
  // false.  If it is one, return true, and set VAL to the value.  VAL
  // should already be initialized.  If this returns true, it sets
  // *PTYPE to the type of the value, or NULL for an abstract type.
  // If IOTA_IS_CONSTANT is true, then an iota expression is assumed
  // to have its final value.
  bool
  integer_constant_value(bool iota_is_constant, mpz_t val, Type** ptype) const;

  // If this is not a constant expression with floating point type,
  // return false.  If it is one, return true, and set VAL to the
  // value.  VAL should already be initialized.  If this returns true,
  // it sets *PTYPE to the type of the value, or NULL for an abstract
  // type.
  bool
  float_constant_value(mpfr_t val, Type** ptype) const;

  // If this is not a constant expression with string type, return
  // false.  If it is one, return true, and set VAL to the value.
  bool
  string_constant_value(std::string* val) const
  { return this->do_string_constant_value(val); }

  // This is called by the parser if the value of this expression is
  // being discarded.  This issues warnings about computed values
  // being unused, and handles send expressions which act differently
  // depending upon whether the value is used.
  void
  discarding_value()
  { this->do_discarding_value(); }

  // Return whether this is an error expression.
  bool
  is_error_expression() const
  { return this->classification_ == EXPRESSION_ERROR; }

  // Return whether this expression really represents a type.
  bool
  is_type_expression() const
  { return this->classification_ == EXPRESSION_TYPE; }

  // If this is a variable reference, return the Var_expression
  // structure.  Otherwise, return NULL.  This is a controlled dynamic
  // cast.
  Var_expression*
  var_expression()
  { return this->convert<Var_expression, EXPRESSION_VAR_REFERENCE>(); }

  const Var_expression*
  var_expression() const
  { return this->convert<const Var_expression, EXPRESSION_VAR_REFERENCE>(); }

  // Return whether this is a sink expression.
  bool
  is_sink_expression() const
  { return this->classification_ == EXPRESSION_SINK; }

  // If this is a string expression, return the String_expression
  // structure.  Otherwise, return NULL.
  String_expression*
  string_expression()
  { return this->convert<String_expression, EXPRESSION_STRING>(); }

  // Return whether this is the expression nil.
  bool
  is_nil_expression() const
  { return this->classification_ == EXPRESSION_NIL; }

  // If this is a call expression, return the Call_expression
  // structure.  Otherwise, return NULL.  This is a controlled dynamic
  // cast.
  Call_expression*
  call_expression()
  { return this->convert<Call_expression, EXPRESSION_CALL>(); }

  // If this is an expression which refers to a function, return the
  // Func_expression structure.  Otherwise, return NULL.
  Func_expression*
  func_expression()
  { return this->convert<Func_expression, EXPRESSION_FUNC_REFERENCE>(); }

  const Func_expression*
  func_expression() const
  { return this->convert<const Func_expression, EXPRESSION_FUNC_REFERENCE>(); }

  // If this is an expression which refers to an unknown name, return
  // the Unknown_expression structure.  Otherwise, return NULL.
  Unknown_expression*
  unknown_expression()
  { return this->convert<Unknown_expression, EXPRESSION_UNKNOWN_REFERENCE>(); }

  const Unknown_expression*
  unknown_expression() const
  {
    return this->convert<const Unknown_expression,
			 EXPRESSION_UNKNOWN_REFERENCE>();
  }

  // If this is an index expression, return the Index_expression
  // structure.  Otherwise, return NULL.
  Index_expression*
  index_expression()
  { return this->convert<Index_expression, EXPRESSION_INDEX>(); }

  // If this is an expression which refers to indexing in a map,
  // return the Map_index_expression structure.  Otherwise, return
  // NULL.
  Map_index_expression*
  map_index_expression()
  { return this->convert<Map_index_expression, EXPRESSION_MAP_INDEX>(); }

  // If this is a bound method expression, return the
  // Bound_method_expression structure.  Otherwise, return NULL.
  Bound_method_expression*
  bound_method_expression()
  { return this->convert<Bound_method_expression, EXPRESSION_BOUND_METHOD>(); }

  // If this is a reference to a field in a struct, return the
  // Field_reference_expression structure.  Otherwise, return NULL.
  Field_reference_expression*
  field_reference_expression()
  {
    return this->convert<Field_reference_expression,
			 EXPRESSION_FIELD_REFERENCE>();
  }

  // If this is a reference to a field in an interface, return the
  // Interface_field_reference_expression structure.  Otherwise,
  // return NULL.
  Interface_field_reference_expression*
  interface_field_reference_expression()
  {
    return this->convert<Interface_field_reference_expression,
			 EXPRESSION_INTERFACE_FIELD_REFERENCE>();
  }

  // If this is a type guard expression, return the
  // Type_guard_expression structure.  Otherwise, return NULL.
  Type_guard_expression*
  type_guard_expression()
  { return this->convert<Type_guard_expression, EXPRESSION_TYPE_GUARD>(); }

  // If this is a receive expression, return the Receive_expression
  // structure.  Otherwise, return NULL.
  Receive_expression*
  receive_expression()
  { return this->convert<Receive_expression, EXPRESSION_RECEIVE>(); }

  // If this is a reference count adjustment, return the
  // Refcount_adjust_expression.  Otherwise, return NULL.
  Refcount_adjust_expression*
  refcount_adjust_expression()
  {
    return this->convert<Refcount_adjust_expression,
			 EXPRESSION_REFCOUNT_ADJUST>();
  }

  // If this is a reference decrement expression, return it.
  // Otherwise, return NULL.
  Refcount_decrement_lvalue_expression*
  refcount_decrement_lvalue_expression()
  {
    return this->convert<Refcount_decrement_lvalue_expression,
  			 EXPRESSION_REFCOUNT_DECREMENT_LVALUE>();
  }

  // Return true if this is a composite literal.
  bool
  is_composite_literal() const;

  // Return true if this is a composite literal which is not constant.
  bool
  is_nonconstant_composite_literal() const;

  // Return true if this is a reference to a local variable.
  bool
  is_local_variable() const;

  // Traverse an expression.
  static int
  traverse(Expression**, Traverse*);

  // Traverse subexpressions of this expression.
  int
  traverse_subexpressions(Traverse*);

  // Lower an expression.  This is called immediately after parsing.
  // IOTA_VALUE is the value that we should give to any iota
  // expressions.  This function must resolve expressions which could
  // not be fully parsed into their final form.  It returns the same
  // Expression or a new one.
  Expression*
  lower(Gogo* gogo, int iota_value)
  { return this->do_lower(gogo, iota_value); }

  // Determine the real type of an expression with abstract integer or
  // floating point type.  TYPE_CONTEXT describes the expected type.
  void
  determine_type(const Type_context*);

  // Check types in an expression.
  void
  check_types(Gogo* gogo)
  { this->do_check_types(gogo); }

  // Determine the type when there is no context.
  void
  determine_type_no_context();

  // Return the current type of the expression.  This may be changed
  // by determine_type.
  Type*
  type()
  { return this->do_type(); }

  // Return a copy of an expression.
  Expression*
  copy()
  { return this->do_copy(); }

  // Return whether the expression is an lvalue--something which may
  // appear on the left hand side of an assignment statement.
  bool
  is_lvalue() const
  { return this->do_is_lvalue(); }

  // We are taking the address of this expression.  If this is
  // invalid, report an error and return false.  ESCAPES is true if
  // this address escapes the current function.
  bool
  address_taken(source_location location, bool escapes)
  { return this->do_address_taken(location, escapes); }

  // This is called when the value of an expression is being stored
  // somewhere.  In some cases this requires that the reference count
  // be incremented.  FOR_LOCAL is true if this is called when either
  // a local variable appears on the left hand of the assignment or
  // when this is in a return statement.  This should return a new
  // Expression which does the appropriate reference count adjustment.
  // When no reference count needs to be increment, this should simply
  // return THIS.  The note_decrements method will be called on this
  // expression later; in some cases all that being_copied has to do
  // is prevent note_decrements from doing anything.
  Expression*
  being_copied(Refcounts* refcounts, bool for_local)
  { return this->do_being_copied(refcounts, for_local); }

  // If this expression computes any values whose reference count must
  // be decremented when the statement is complete, store them in the
  // reference count decrement queue.  This is not called for an
  // expression which is the complete left hand side of an assignment.
  // It will be called for subexpressions of LHS's, and for all RHS
  // expressions.  This returns a new expression which should replace
  // the current expression.  When no reference counts need to be
  // decremented, this should simply return THIS.
  Expression*
  note_decrements(Refcounts* refcounts)
  { return this->do_note_decrements(refcounts); }

  // This is called when the expression is being set.  The expression
  // must be an lvalue.  In some cases this requires that the
  // reference count of the old value of the expression be
  // decremented.  This should return a new Expression which does the
  // appropriate adjustment.  When no reference count needs to be
  // decremented, this should simply return THIS.
  Expression*
  being_set(Refcounts* refcounts)
  { return this->do_being_set(refcounts); }

  // Return the tree for this expression.
  tree
  get_tree(Translate_context*);

  // Return a tree handling any conversions which must be done during
  // assignment.
  static tree
  convert_for_assignment(Translate_context*, Type* lhs_type, Type* rhs_type,
			 tree rhs_tree, source_location location);

  // Return a tree implementing the comparison LHS_TREE OP RHS_TREE.
  // TYPE is the type of both sides.
  static tree
  comparison_tree(Translate_context*, Operator op, Type* left_type,
		  tree left_tree, Type* right_type, tree right_tree,
		  source_location);

  // Return a tree for the multi-precision integer VAL in TYPE.
  static tree
  integer_constant_tree(mpz_t val, tree type);

  // Return a tree for the floating point value VAL in TYPE.
  static tree
  float_constant_tree(mpfr_t val, tree type);

  // Export the expression.  This is only used for constants.  It will
  // be used for things like values of named constants and sizes of
  // arrays.
  void
  export_expression(Export* exp) const
  { this->do_export(exp); }

  // Import an expression.
  static Expression*
  import_expression(Import*);

 protected:
  // May be implemented by child class: traverse the expressions.
  virtual int
  do_traverse(Traverse*);

  // Return a lowered expression.
  virtual Expression*
  do_lower(Gogo*, int)
  { return this; }

  // Return whether this is a constant expression.
  virtual bool
  do_is_constant() const
  { return false; }

  // Return whether this is a constant expression of integral type,
  // and set VAL to the value.
  virtual bool
  do_integer_constant_value(bool, mpz_t, Type**) const
  { return false; }

  // Return whether this is a constant expression of floating point
  // type, and set VAL to the value.
  virtual bool
  do_float_constant_value(mpfr_t, Type**) const
  { return false; }

  // Return whether this is a constant expression of string type, and
  // set VAL to the value.
  virtual bool
  do_string_constant_value(std::string*) const
  { return false; }

  // Called by the parser if the value is being discarded.
  virtual void
  do_discarding_value();

  // Child class holds type.
  virtual Type*
  do_type() = 0;

  // Child class implements determining type information.
  virtual void
  do_determine_type(const Type_context*) = 0;

  // Child class implements type checking if needed.
  virtual void
  do_check_types(Gogo*)
  { }

  // Child class implements copying.
  virtual Expression*
  do_copy() = 0;

  // Child class implements whether the expression is an lvalue.
  virtual bool
  do_is_lvalue() const
  { return false; }

  // Child class implements taking the address of an expression.
  virtual bool
  do_address_taken(source_location, bool);

  // Child class implements incrementing reference count.
  virtual Expression*
  do_being_copied(Refcounts*, bool)
  { return this; }

  // Child class implements noting reference count decrements.
  virtual Expression*
  do_note_decrements(Refcounts*)
  { return this; }

  // Child class implements whether the old value of an lvalue needs
  // to have its reference count decremented.
  virtual Expression*
  do_being_set(Refcounts*);

  // Child class implements conversion to tree.
  virtual tree
  do_get_tree(Translate_context*) = 0;

  // Child class implements export.
  virtual void
  do_export(Export*) const;

  // For children to call to warn about an unused value.
  void
  warn_unused_value();

  // For children to call when they detect that they are in error.
  void
  set_is_error();

  // For children to call to report an error conveniently.
  void
  report_error(const char*);

  // For errors when taking the address.
  void
  report_address_taken_error(source_location);

 private:
  // Convert to the desired statement classification, or return NULL.
  // This is a controlled dynamic cast.
  template<typename Expression_class,
	   Expression_classification expr_classification>
  Expression_class*
  convert()
  {
    return (this->classification_ == expr_classification
	    ? static_cast<Expression_class*>(this)
	    : NULL);
  }

  template<typename Expression_class,
	   Expression_classification expr_classification>
  const Expression_class*
  convert() const
  {
    return (this->classification_ == expr_classification
	    ? static_cast<const Expression_class*>(this)
	    : NULL);
  }

  // The expression classification.
  Expression_classification classification_;
  // The location in the input file.
  source_location location_;
};

// A list of Expressions.

class Expression_list
{
 public:
  Expression_list()
    : entries_()
  { }

  // Return whether the list is empty.
  bool
  empty() const
  { return this->entries_.empty(); }

  // Return the number of entries in the list.
  size_t
  size() const
  { return this->entries_.size(); }

  // Add an entry to the end of the list.
  void
  push_back(Expression* expr)
  { this->entries_.push_back(expr); }

  void
  append(Expression_list* add)
  { this->entries_.insert(this->entries_.end(), add->begin(), add->end()); }

  // Reserve space in the list.
  void
  reserve(size_t size)
  { this->entries_.reserve(size); }

  // Traverse the expressions in the list.
  int
  traverse(Traverse*);

  // Copy the list.
  Expression_list*
  copy();

  // Return true if the list contains an error expression.
  bool
  contains_error() const;

  // Return the first and last elements.
  Expression*&
  front()
  { return this->entries_.front(); }

  Expression*
  front() const
  { return this->entries_.front(); }

  Expression*&
  back()
  { return this->entries_.back(); }

  Expression*
  back() const
  { return this->entries_.back(); }

  // Iterators.

  typedef std::vector<Expression*>::iterator iterator;
  typedef std::vector<Expression*>::const_iterator const_iterator;

  iterator
  begin()
  { return this->entries_.begin(); }

  const_iterator
  begin() const
  { return this->entries_.begin(); }

  iterator
  end()
  { return this->entries_.end(); }

  const_iterator
  end() const
  { return this->entries_.end(); }

  // Erase an entry.
  void
  erase(iterator p)
  { this->entries_.erase(p); }

 private:
  std::vector<Expression*> entries_;
};

// An abstract base class for an expression which is only used by the
// parser, and is lowered in the lowering pass.

class Parser_expression : public Expression
{
 public:
  Parser_expression(Expression_classification classification,
		    source_location location)
    : Expression(classification, location)
  { }

 protected:
  virtual Expression*
  do_lower(Gogo*, int) = 0;

  Type*
  do_type()
  { gcc_unreachable(); }

  void
  do_determine_type(const Type_context*)
  { gcc_unreachable(); }

  void
  do_check_types(Gogo*)
  { gcc_unreachable(); }

  tree
  do_get_tree(Translate_context*)
  { gcc_unreachable(); }
};

// An expression which is simply a variable.

class Var_expression : public Expression
{
 public:
  Var_expression(Named_object* variable, source_location location)
    : Expression(EXPRESSION_VAR_REFERENCE, location),
      variable_(variable)
  { }

  // Return the variable.
  Named_object*
  named_object() const
  { return this->variable_; }

  // Return the name of the variable.
  const std::string&
  name() const;

 protected:
  Expression*
  do_lower(Gogo*, int);

  Type*
  do_type();

  void
  do_determine_type(const Type_context*)
  { }

  Expression*
  do_copy()
  { return this; }

  bool
  do_is_lvalue() const
  { return true; }

  bool
  do_address_taken(source_location, bool);

  Expression*
  do_being_copied(Refcounts*, bool);

  Expression*
  do_being_set(Refcounts*);

  tree
  do_get_tree(Translate_context*);

 private:
  // The variable we are referencing.
  Named_object* variable_;
};

// A string expression.

class String_expression : public Expression
{
 public:
  String_expression(const std::string& val, source_location location)
    : Expression(EXPRESSION_STRING, location),
      val_(val)
  { }

  const std::string&
  val() const
  { return this->val_; }

  static Expression*
  do_import(Import*);

 protected:
  bool
  do_is_constant() const
  { return true; }

  bool
  do_string_constant_value(std::string* val) const
  {
    *val = this->val_;
    return true;
  }

  Type*
  do_type();

  void
  do_determine_type(const Type_context*)
  { }

  Expression*
  do_copy()
  { return this; }

  tree
  do_get_tree(Translate_context*);

  void
  do_export(Export*) const;

 private:
  // The string value.  This is immutable.
  const std::string val_;
};

// A binary expression.

class Binary_expression : public Expression
{
 public:
  Binary_expression(Operator op, Expression* left, Expression* right,
		    source_location location)
    : Expression(EXPRESSION_BINARY, location),
      op_(op), left_(left), right_(right), is_being_copied_(false)
  { }

  // Apply binary opcode OP to LEFT_VAL and RIGHT_VAL, setting VAL.
  // LEFT_TYPE is the type of LEFT_VAL, RIGHT_TYPE is the type of
  // RIGHT_VAL; LEFT_TYPE and/or RIGHT_TYPE may be NULL.  Return true
  // if this could be done, false if not.
  static bool
  eval_integer(Operator op, Type* left_type, mpz_t left_val,
	       Type* right_type, mpz_t right_val, source_location,
	       mpz_t val);

  // Apply binary opcode OP to LEFT_VAL and RIGHT_VAL, setting VAL.
  // Return true if this could be done, false if not.
  static bool
  eval_float(Operator op, Type* left_type, mpfr_t left_val,
	     Type* right_type, mpfr_t right_val, mpfr_t val,
	     source_location);

  // Compare integer constants according to OP.
  static bool
  compare_integer(Operator op, mpz_t left_val, mpz_t right_val);

  // Compare floating point constants according to OP.
  static bool
  compare_float(Operator op, Type* type, mpfr_t left_val, mpfr_t right_val);

  static Expression*
  do_import(Import*);

  // Report an error if OP can not be applied to TYPE.  Return whether
  // it can.
  static bool
  check_operator_type(Operator op, Type* type, source_location);

 protected:
  int
  do_traverse(Traverse* traverse);

  Expression*
  do_lower(Gogo*, int);

  bool
  do_is_constant() const
  { return this->left_->is_constant() && this->right_->is_constant(); }

  bool
  do_integer_constant_value(bool, mpz_t val, Type**) const;

  bool
  do_float_constant_value(mpfr_t val, Type**) const;

  void
  do_discarding_value();

  Type*
  do_type();

  void
  do_determine_type(const Type_context*);

  void
  do_check_types(Gogo*);

  Expression*
  do_copy()
  {
    return Expression::make_binary(this->op_, this->left_->copy(),
				   this->right_->copy(), this->location());
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
  // The binary operator to apply.
  Operator op_;
  // The left hand side operand.
  Expression* left_;
  // The right hand side operand.
  Expression* right_;
  // Whether the expression is being copied.
  bool is_being_copied_;
};

// A call expression.  The go statement needs to dig inside this.

class Call_expression : public Expression
{
 public:
  Call_expression(Expression* fn, Expression_list* args,
		  source_location location)
    : Expression(EXPRESSION_CALL, location),
      fn_(fn), args_(args), type_(NULL), tree_(NULL), refcount_entries_(NULL),
      is_value_discarded_(false), is_being_copied_(false)
  { }

  // The function to call.
  Expression*
  fn() const
  { return this->fn_; }

  // The arguments.
  Expression_list*
  args()
  { return this->args_; }

  const Expression_list*
  args() const
  { return this->args_; }

  // Get the function type.
  Function_type*
  get_function_type(bool issue_error);

  // Return the number of values this call will return.
  size_t
  result_count();

 protected:
  int
  do_traverse(Traverse*);

  virtual Expression*
  do_lower(Gogo*, int);

  void
  do_discarding_value()
  { this->is_value_discarded_ = true; }

  virtual Type*
  do_type();

  virtual void
  do_determine_type(const Type_context*);

  virtual void
  do_check_types(Gogo*);

  Expression*
  do_copy()
  {
    return Expression::make_call(this->fn_->copy(), this->args_->copy(),
				 this->location());
  }

  Expression*
  do_being_copied(Refcounts*, bool);

  Expression*
  do_note_decrements(Refcounts*);

  virtual tree
  do_get_tree(Translate_context*);

 private:
  void
  check_argument_type(int, const Type*, const Type*, source_location);

  tree
  bound_method_function(Translate_context*, Bound_method_expression*, tree*);

  tree
  interface_method_function(Translate_context*,
			    Interface_field_reference_expression*,
			    tree*);

  tree
  set_refcount_queue_entries(Translate_context*, tree ret);

  // The function to call.
  Expression* fn_;
  // The arguments to pass.  This may be NULL if there are no
  // arguments.
  Expression_list* args_;
  // The type of the expression, to avoid recomputing it.
  Type* type_;
  // The tree for the call, used for a call which returns a tuple.
  tree tree_;
  // If needed a list of entries in the reference count queue.  These
  // are set to values returned by a tuple call if some of the values
  // need to have their reference counts decremented.
  std::vector<Refcount_entry>* refcount_entries_;
  // True if the value is being discarded.
  bool is_value_discarded_;
  // True if the value is being copied.
  bool is_being_copied_;
};

// An expression which represents a pointer to a function.

class Func_expression : public Expression
{
 public:
  Func_expression(Named_object* function, Expression* closure,
		  source_location location)
    : Expression(EXPRESSION_FUNC_REFERENCE, location),
      function_(function), closure_(closure)
  { }

  // Return the object associated with the function.
  const Named_object*
  named_object() const
  { return this->function_; }

  // Return the name of the function.
  const std::string&
  name() const;

  // Return the closure for this function.  This will return NULL if
  // the function has no closure, which is the normal case.
  Expression*
  closure()
  { return this->closure_; }

  // Return a tree for this function without evaluating the closure.
  tree
  get_tree_without_closure(Gogo*);

 protected:
  int
  do_traverse(Traverse*);

  Type*
  do_type();

  void
  do_determine_type(const Type_context*)
  {
    if (this->closure_ != NULL)
      this->closure_->determine_type_no_context();
  }

  Expression*
  do_copy()
  {
    return Expression::make_func_reference(this->function_,
					   this->closure_->copy(),
					   this->location());
  }

  Expression*
  do_being_copied(Refcounts*, bool);

  Expression*
  do_note_decrements(Refcounts*);

  tree
  do_get_tree(Translate_context*);

 private:
  // The function itself.
  Named_object* function_;
  // A closure.  This is normally NULL.  For a nested function, it may
  // be a heap-allocated struct holding pointers to all the variables
  // referenced by this function and defined in enclosing functions.
  Expression* closure_;
};

// A reference to an unknown name.

class Unknown_expression : public Parser_expression
{
 public:
  Unknown_expression(Named_object* named_object, source_location location)
    : Parser_expression(EXPRESSION_UNKNOWN_REFERENCE, location),
      named_object_(named_object), is_composite_literal_key_(false)
  { }

  // The associated named object.
  Named_object*
  named_object() const
  { return this->named_object_; }

  // The name of the identifier which was unknown.
  const std::string&
  name() const;

  // Note that this expression is being used as the key in a composite
  // literal, so it may be OK if it is not resolved.
  void
  set_is_composite_literal_key()
  { this->is_composite_literal_key_ = true; }

 protected:
  Expression*
  do_lower(Gogo*, int);

  Expression*
  do_copy()
  { return new Unknown_expression(this->named_object_, this->location()); }

 private:
  // The unknown name.
  Named_object* named_object_;
  // True if this is the key in a composite literal.
  bool is_composite_literal_key_;
};

// An index expression.  This is lowered to an array index, a string
// index, or a map index.

class Index_expression : public Parser_expression
{
 public:
  Index_expression(Expression* left, Expression* start, Expression* end,
		   source_location location)
    : Parser_expression(EXPRESSION_INDEX, location),
      left_(left), start_(start), end_(end), is_lvalue_(false)
  { }

  // Record that this expression is an lvalue.
  void
  set_is_lvalue()
  { this->is_lvalue_ = true; }

 protected:
  int
  do_traverse(Traverse*);

  Expression*
  do_lower(Gogo*, int);

  Expression*
  do_copy()
  {
    return new Index_expression(this->left_->copy(), this->start_->copy(),
				(this->end_ == NULL
				 ? NULL
				 : this->end_->copy()),
				this->location());
  }

 private:
  // The expression being indexed.
  Expression* left_;
  // The first index.
  Expression* start_;
  // The second index.  This is NULL for an index, non-NULL for a
  // slice.
  Expression* end_;
  // Whether this is being used as an l-value.  We set this during the
  // parse because map index expressions need to know.
  bool is_lvalue_;
};

// An index into a map.

class Map_index_expression : public Expression
{
 public:
  Map_index_expression(Expression* map, Expression* index,
		       source_location location)
    : Expression(EXPRESSION_MAP_INDEX, location),
      map_(map), index_(index), is_lvalue_(false),
      is_in_tuple_assignment_(false)
  { }

  // Return the map.
  Expression*
  map()
  { return this->map_; }

  const Expression*
  map() const
  { return this->map_; }

  // Return the index.
  Expression*
  index()
  { return this->index_; }

  const Expression*
  index() const
  { return this->index_; }

  // Get the type of the map being indexed.
  Map_type*
  get_map_type() const;

  // Record that this map expression is an lvalue.  The difference is
  // that an lvalue always inserts the key.
  void
  set_is_lvalue()
  { this->is_lvalue_ = true; }

  // Return whether this map expression occurs in an assignment to a
  // pair of values.
  bool
  is_in_tuple_assignment() const
  { return this->is_in_tuple_assignment_; }

  // Record that this map expression occurs in an assignment to a pair
  // of values.
  void
  set_is_in_tuple_assignment()
  { this->is_in_tuple_assignment_ = true; }

  // Return a tree for the map index.  This returns a tree which
  // evaluates to a pointer to a value in the map.  If INSERT is true,
  // the key will be inserted if not present, and the value pointer
  // will be zero initialized.  If INSERT is false, and the key is not
  // present in the map, the pointer will be NULL.
  tree
  get_value_pointer(Translate_context*, bool insert);

  // Return a tree to delete this key from the map.
  tree
  delete_key(Translate_context*);

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
    return Expression::make_map_index(this->map_->copy(),
				      this->index_->copy(),
				      this->location());
  }

  bool
  do_is_lvalue() const
  { return this->is_lvalue_; }

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
  // The map we are looking into.
  Expression* map_;
  // The index.
  Expression* index_;
  // Whether this is an lvalue.
  bool is_lvalue_;
  // Whether this is in a tuple assignment to a pair of values.
  bool is_in_tuple_assignment_;
};

// An expression which represents a method bound to its first
// argument.

class Bound_method_expression : public Expression
{
 public:
  Bound_method_expression(Expression* expr, Expression* method,
			  source_location location)
    : Expression(EXPRESSION_BOUND_METHOD, location),
      expr_(expr), expr_type_(NULL), method_(method)
  { }

  // Return the object which is the first argument.
  Expression*
  first_argument()
  { return this->expr_; }

  // Return the implicit type of the first argument.  This will be
  // non-NULL when using a method from an anonymous field without
  // using an explicit stub.
  Type*
  first_argument_type() const
  { return this->expr_type_; }

  // Return the reference to the method function.
  Expression*
  method()
  { return this->method_; }

  // Set the implicit type of the expression.
  void
  set_first_argument_type(Type* type)
  { this->expr_type_ = type; }

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
    return new Bound_method_expression(this->expr_->copy(),
				       this->method_->copy(),
				       this->location());
  }

  tree
  do_get_tree(Translate_context*);

 private:
  // The object used to find the method.  This is passed to the method
  // as the first argument.
  Expression* expr_;
  // The implicit type of the object to pass to the method.  This is
  // NULL in the normal case, non-NULL when using a method from an
  // anonymous field which does not require a stub.
  Type* expr_type_;
  // The method itself.  This is a Func_expression.
  Expression* method_;
};

// A reference to a field in a struct.

class Field_reference_expression : public Expression
{
 public:
  Field_reference_expression(Expression* expr, unsigned int field_index,
			     source_location location)
    : Expression(EXPRESSION_FIELD_REFERENCE, location),
      expr_(expr), field_index_(field_index)
  { }

  // Return the struct expression.
  Expression*
  expr() const
  { return this->expr_; }

  // Return the field index.
  unsigned int
  field_index() const
  { return this->field_index_; }

  // Set the struct expression.  This is used when parsing.
  void
  set_struct_expression(Expression* expr)
  {
    gcc_assert(this->expr_ == NULL);
    this->expr_ = expr;
  }

 protected:
  int
  do_traverse(Traverse* traverse)
  { return Expression::traverse(&this->expr_, traverse); }

  Type*
  do_type();

  void
  do_determine_type(const Type_context*)
  { this->expr_->determine_type_no_context(); }

  void
  do_check_types(Gogo*);

  Expression*
  do_copy()
  {
    return Expression::make_field_reference(this->expr_->copy(),
					    this->field_index_,
					    this->location());
  }

  bool
  do_is_lvalue() const
  { return true; }

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
  // The expression we are looking into.  This should have a type of
  // struct or pointer to struct.
  Expression* expr_;
  // The zero-based index of the field we are retrieving.
  unsigned int field_index_;
};

// A reference to a field of an interface.

class Interface_field_reference_expression : public Expression
{
 public:
  Interface_field_reference_expression(Expression* expr,
				       const std::string& name,
				       source_location location)
    : Expression(EXPRESSION_INTERFACE_FIELD_REFERENCE, location),
      expr_(expr), name_(name)
  { }

  // Return the expression for the interface object.
  Expression*
  expr()
  { return this->expr_; }

  // Return the name of the method to call.
  const std::string&
  name() const
  { return this->name_; }

  // Return a tree for the pointer to the function to call, given a
  // tree for the expression.
  tree
  get_function_tree(Translate_context*, tree);

  // Return a tree for the first argument to pass to the interface
  // function, given a tree for the expression.  This is the real
  // object associated with the interface object.
  tree
  get_underlying_object_tree(Translate_context*, tree);

 protected:
  int
  do_traverse(Traverse* traverse);

  Type*
  do_type();

  void
  do_determine_type(const Type_context*);

  void
  do_check_types(Gogo*);

  Expression*
  do_copy()
  {
    return Expression::make_interface_field_reference(this->expr_->copy(),
						      this->name_,
						      this->location());
  }

  bool
  do_address_taken(source_location, bool)
  { return true; }

  tree
  do_get_tree(Translate_context*);

 private:
  // The expression for the interface object.  This should have a type
  // of interface or pointer to interface.
  Expression* expr_;
  // The field we are retrieving--the name of the method.
  std::string name_;
};

// A type guard expression.

class Type_guard_expression : public Expression
{
 public:
  Type_guard_expression(Expression* expr, Type* type, source_location location)
    : Expression(EXPRESSION_TYPE_GUARD, location),
      expr_(expr), type_(type), is_being_copied_(false)
  { }

  // Return the expression to convert.
  Expression*
  expr()
  { return this->expr_; }

  // Return the type to which to convert.
  Type*
  type()
  { return this->type_; }

 protected:
  int
  do_traverse(Traverse* traverse);

  Type*
  do_type()
  { return this->type_; }

  void
  do_determine_type(const Type_context*)
  { this->expr_->determine_type_no_context(); }

  void
  do_check_types(Gogo*);

  Expression*
  do_copy()
  {
    return new Type_guard_expression(this->expr_->copy(), this->type_,
				     this->location());
  }

  Expression*
  do_being_copied(Refcounts*, bool);

  Expression*
  do_note_decrements(Refcounts*);

  tree
  do_get_tree(Translate_context*);

 private:
  // The expression to convert.
  Expression* expr_;
  // The type to which to convert.
  Type* type_;
  // Whether this expression is being copied.
  bool is_being_copied_;
};

// A receive expression.

class Receive_expression : public Expression
{
 public:
  Receive_expression(Expression* channel, source_location location)
    : Expression(EXPRESSION_RECEIVE, location),
      channel_(channel), is_value_discarded_(false), is_being_copied_(false)
  { }

  // Return the channel.
  Expression*
  channel()
  { return this->channel_; }

 protected:
  int
  do_traverse(Traverse* traverse)
  { return Expression::traverse(&this->channel_, traverse); }

  void
  do_discarding_value()
  { this->is_value_discarded_ = true; }

  Type*
  do_type();

  void
  do_determine_type(const Type_context*)
  { this->channel_->determine_type_no_context(); }

  void
  do_check_types(Gogo*);

  Expression*
  do_copy()
  {
    return Expression::make_receive(this->channel_->copy(), this->location());
  }

  Expression*
  do_being_copied(Refcounts*, bool);

  Expression*
  do_note_decrements(Refcounts*);

  tree
  do_get_tree(Translate_context*);

 private:
  // The channel from which we are receiving.
  Expression* channel_;
  // Whether the value is being discarded.
  bool is_value_discarded_;
  // Whether the value is being copied.
  bool is_being_copied_;
};

// Adjust the reference count of a value.

class Refcount_adjust_expression : public Expression
{
 public:
  Refcount_adjust_expression(Refcount_entry* entry, Expression* expr,
			     bool for_local)
    : Expression(EXPRESSION_REFCOUNT_ADJUST, expr->location()),
      refcount_entry_(entry), expr_(expr), for_local_(for_local)
  { }

  // The type of adjustment.
  int
  classification() const;

  // The underlying expression.
  Expression*
  expr()
  { return this->expr_; }

 protected:
  int
  do_traverse(Traverse* traverse)
  { return Expression::traverse(&this->expr_, traverse); }

  Type*
  do_type()
  { return this->expr_->type(); }

  void
  do_determine_type(const Type_context*);

  void
  do_check_types(Gogo*);

  Expression*
  do_copy()
  {
    return new Refcount_adjust_expression(this->refcount_entry_,
					  this->expr_,
					  this->for_local_);
  }

  tree
  do_get_tree(Translate_context*);

 private:
  // The reference count entry.
  Refcount_entry* refcount_entry_;
  // The expression whose reference count we are incrementing.
  Expression* expr_;
  // Whether this is to be stored in a local variable, for debugging
  // purposes.
  bool for_local_;
};

// Decrement the reference count of the old value of an lvalue in an
// assignment.

class Refcount_decrement_lvalue_expression : public Expression
{
 public:
  Refcount_decrement_lvalue_expression(Refcount_entry* entry,
				       Expression* expr);

  // The underlying expression.
  Expression*
  expr()
  { return this->expr_; }

  // Set THIS->EXPR_ (an lvalue) to RHS_TREE.  LHS_TREE is THIS
  // converted to tree form.  This should be called instead of
  // get_tree.
  tree
  set(Translate_context*, tree lhs_tree, tree rhs_tree);

 protected:
  int
  do_traverse(Traverse*);

  Type*
  do_type()
  { return this->expr_->type(); }

  void
  do_determine_type(const Type_context*);

  void
  do_check_types(Gogo*);

  Expression*
  do_copy()
  {
    return new Refcount_decrement_lvalue_expression(this->refcount_entry_,
						    this->expr_);
  }

  bool
  do_is_lvalue() const
  { return true; }

  tree
  do_get_tree(Translate_context*);

 private:
  // The reference count entry.
  Refcount_entry* refcount_entry_;
  // The expression which is being set.
  Expression* expr_;
};

#endif // !defined(GO_EXPRESSIONS_H)
