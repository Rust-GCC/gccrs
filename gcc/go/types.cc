// types.cc -- Go frontend types.

// Copyright 2009 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <algorithm>

#include "go-system.h"

#include <gmp.h>

extern "C"
{
#include "intl.h"
#include "tree.h"
#include "gimple.h"
#include "real.h"
#include "diagnostic.h"
}

#include "go-c.h"
#include "gogo.h"
#include "operator.h"
#include "expressions.h"
#include "statements.h"
#include "export.h"
#include "import.h"
#include "refcount.h"
#include "types.h"

// Class Type.

Type::Type(Type_classification classification)
  : classification_(classification), tree_(NULL_TREE),
    type_descriptor_decl_(NULL_TREE)
{
}

Type::~Type()
{
}

// Get the base type for a type--skip names and forward declarations.

Type*
Type::base()
{
  switch (this->classification_)
    {
    case TYPE_NAMED:
      return static_cast<Named_type*>(this)->real_type()->base();
    case TYPE_FORWARD:
      return static_cast<Forward_declaration_type*>(this)->real_type()->base();
    default:
      return this;
    }
}

const Type*
Type::base() const
{
  switch (this->classification_)
    {
    case TYPE_NAMED:
      return static_cast<const Named_type*>(this)->real_type()->base();
    case TYPE_FORWARD:
      {
	const Forward_declaration_type* ftype =
	  static_cast<const Forward_declaration_type*>(this);
	return ftype->real_type()->base();
      }
    default:
      return this;
    }
}

// Skip defined forward declarations.

Type*
Type::forwarded()
{
  Type* t = this;
  Forward_declaration_type* ftype = t->forward_declaration_type();
  while (ftype != NULL && ftype->is_defined())
    {
      t = ftype->real_type();
      ftype = t->forward_declaration_type();
    }
  return t;
}

const Type*
Type::forwarded() const
{
  const Type* t = this;
  const Forward_declaration_type* ftype = t->forward_declaration_type();
  while (ftype != NULL && ftype->is_defined())
    {
      t = ftype->real_type();
      ftype = t->forward_declaration_type();
    }
  return t;
}

// If this is a named type, return it.  Otherwise, return NULL.

Named_type*
Type::named_type()
{
  return this->forwarded()->convert_no_base<Named_type, TYPE_NAMED>();
}

const Named_type*
Type::named_type() const
{
  return this->forwarded()->convert_no_base<const Named_type, TYPE_NAMED>();
}

// Return true if this type is not defined.

bool
Type::is_undefined() const
{
  return this->forwarded()->forward_declaration_type() != NULL;
}

// Return true if this is a basic type: a type which is not composed
// of other types, and is not void.

bool
Type::is_basic_type() const
{
  switch (this->classification_)
    {
    case TYPE_INTEGER:
    case TYPE_FLOAT:
    case TYPE_BOOLEAN:
    case TYPE_STRING:
    case TYPE_NIL:
      return true;

    case TYPE_ERROR:
    case TYPE_VOID:
    case TYPE_FUNCTION:
    case TYPE_POINTER:
    case TYPE_STRUCT:
    case TYPE_ARRAY:
    case TYPE_MAP:
    case TYPE_CHANNEL:
    case TYPE_INTERFACE:
      return false;

    case TYPE_NAMED:
    case TYPE_FORWARD:
      return this->base()->is_basic_type();

    default:
      gcc_unreachable();
    }
}

// Return true if this is an abstract type.

bool
Type::is_abstract() const
{
  const Integer_type* it = this->integer_type();
  if (it != NULL)
    return it->is_abstract();
  const Float_type* ft = this->float_type();
  if (ft != NULL)
    return ft->is_abstract();
  return false;
}

// Return a non-abstract version of an abstract type.

Type*
Type::make_non_abstract_type()
{
  gcc_assert(this->is_abstract());
  if (this->integer_type() != NULL)
    return Type::lookup_integer_type("int");
  else if (this->float_type() != NULL)
    return Type::lookup_float_type("float");
  else
    gcc_unreachable();
}

// Add entries to the reference count queue for this type.  This is
// the default version, which adds a single pointer.

void
Type::do_add_refcount_queue_entries(Refcounts* refcounts,
				    Refcount_entry* entry)
{
  gcc_assert(this->is_refcounted());
  refcounts->add_one(this, entry);
}

// Return true if this is an error type.  Don't give an error if we
// try to dereference an undefined forwarding type, as this is called
// in the parser when the type may legitimately be undefined.

bool
Type::is_error_type() const
{
  const Type* t = this->forwarded();
  // Note that we return false for an undefined forward type.
  switch (t->classification_)
    {
    case TYPE_ERROR:
      return true;
    case TYPE_NAMED:
      return t->named_type()->real_type()->is_error_type();
    default:
      return false;
    }
}

// If this is a pointer type, return the type to which it points.
// Otherwise, return NULL.

Type*
Type::points_to() const
{
  const Pointer_type* ptype = this->convert<const Pointer_type,
					    TYPE_POINTER>();
  return ptype == NULL ? NULL : ptype->points_to();
}

// Return whether this is an open array type.

bool
Type::is_open_array_type() const
{
  return this->array_type() != NULL && this->array_type()->length() == NULL;
}

// Return whether this is the predeclared constant nil being used as a
// type.

bool
Type::is_nil_constant_as_type() const
{
  const Type* t = this->forwarded();
  if (t->forward_declaration_type() != NULL)
    {
      const Named_object* no = t->forward_declaration_type()->named_object();
      if (no->is_unknown())
	no = no->unknown_value()->real_named_object();
      if (no != NULL
	  && no->is_const()
	  && no->const_value()->expr()->is_nil_expression())
	return true;
    }
  return false;
}

// Traverse a type.

int
Type::traverse(Type* type, Traverse* traverse)
{
  gcc_assert((traverse->traverse_mask() & Traverse::traverse_types) != 0
	     || (traverse->traverse_mask()
		 & Traverse::traverse_expressions) != 0);
  if (traverse->remember_type(type))
    {
      // We have already traversed this type.
      return TRAVERSE_CONTINUE;
    }
  if ((traverse->traverse_mask() & Traverse::traverse_types) != 0)
    {
      int t = traverse->type(type);
      if (t == TRAVERSE_EXIT)
	return TRAVERSE_EXIT;
      else if (t == TRAVERSE_SKIP_COMPONENTS)
	return TRAVERSE_CONTINUE;
    }
  // An array type has an expression which we need to traverse if
  // traverse_expressions is set.
  if (type->do_traverse(traverse) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  return TRAVERSE_CONTINUE;
}

// Default implementation for do_traverse for child class.

int
Type::do_traverse(Traverse*)
{
  return TRAVERSE_CONTINUE;
}

// Return true if two types are compatible according to COMPATIBLE.
// If REASON is not NULL, optionally set *REASON to the reason the
// types are incompatible.

bool
Type::are_compatible_for(const Type* t1, const Type* t2,
			 Type_compatible compatible, std::string* reason)
{
  if (t1 == NULL || t2 == NULL)
    {
      // Something is wrong.  Return true to avoid cascading errors.
      return true;
    }

  // Skip defined forward declarations.
  t1 = t1->forwarded();
  t2 = t2->forwarded();

  if (t1 == t2)
    return true;

  // An undefined forward declaration is an error, so we return true
  // to avoid cascading errors.
  if (t1->forward_declaration_type() != NULL
      || t2->forward_declaration_type() != NULL)
    return true;

  // Avoid cascading errors with error types.
  if (t1->is_error_type() || t2->is_error_type())
    return true;

  // Get a good reason for the sink type.  Note that the sink type on
  // the left hand side of an assignment is handled in
  // are_compatible_for_assign.
  if (t1->is_sink_type() || t2->is_sink_type())
    {
      if (reason != NULL)
	*reason = "invalid use of _";
      return false;
    }

  // Cut off infinite recursion by using a flag on a named type.
  const Named_type* nt1 = t1->named_type();
  if (nt1 != NULL)
    return nt1->is_compatible(t2, compatible, reason);

  const Type* t1base = t1->base();
  const Type* t2base = t2->base();

  if (t1base->classification_ != t2base->classification_)
    return false;

  switch (t1base->classification_)
    {
    case TYPE_VOID:
    case TYPE_BOOLEAN:
    case TYPE_STRING:
    case TYPE_NIL:
      // These types are always compatible with themselves.
      return true;

    case TYPE_INTEGER:
      return t1base->integer_type()->is_compatible(t2base->integer_type());

    case TYPE_FLOAT:
      return t1base->float_type()->is_compatible(t2base->float_type());

    case TYPE_FUNCTION:
      return t1base->function_type()->is_compatible(t2base->function_type(),
						    compatible,
						    false,
						    reason);

    case TYPE_POINTER:
      return Type::are_compatible_for(t1base->points_to(), t2base->points_to(),
				      compatible, reason);

    case TYPE_STRUCT:
      return t1base->struct_type()->is_compatible(t2base->struct_type(),
						  compatible);

    case TYPE_ARRAY:
      return t1base->array_type()->is_compatible(t2base->array_type(),
						 compatible);

    case TYPE_MAP:
      return t1base->map_type()->is_compatible(t2base->map_type(),
					       compatible);

    case TYPE_CHANNEL:
      return t1base->channel_type()->is_compatible(t2base->channel_type(),
						   compatible);

    case TYPE_INTERFACE:
      return t1base->interface_type()->is_compatible(t2base->interface_type(),
						     compatible);

    default:
      gcc_unreachable();
    }
}

// Return true if it's OK to have a binary operation with types LHS and RHS.

bool
Type::are_compatible_for_binop(const Type* lhs, const Type* rhs)
{
  if (Type::are_compatible(lhs, rhs))
    return true;

  lhs = lhs->base();
  rhs = rhs->base();

  // A constant of abstract integer or float type may be mixed with an
  // integer or float type.
  if ((rhs->is_abstract()
       && (lhs->integer_type() != NULL || lhs->float_type() != NULL))
      || (lhs->is_abstract()
	  && (rhs->integer_type() != NULL || rhs->float_type() != NULL)))
    return true;

  // The nil type may be compared to a pointer, an interface type, a
  // slice type, a channel type, a map type, or a function type.
  if (lhs->is_nil_type()
      && (rhs->points_to() != NULL
	  || rhs->interface_type() != NULL
	  || rhs->is_open_array_type()
	  || rhs->map_type() != NULL
	  || rhs->channel_type() != NULL
	  || rhs->function_type() != NULL))
    return true;
  if (rhs->is_nil_type()
      && (lhs->points_to() != NULL
	  || lhs->interface_type() != NULL
	  || lhs->is_open_array_type()
	  || lhs->map_type() != NULL
	  || lhs->channel_type() != NULL
	  || lhs->function_type() != NULL))
    return true;

  return false;
}


// Return true if a value with type RHS may be assigned to a value
// with type LHS.  IS_CONVERSION is true to check for conversion
// compatibility rather than assignment compatibility.

bool
Type::are_assignment_compatible(const Type* lhs, const Type* rhs,
				bool is_conversion, std::string* reason)
{
  // Any value may be assigned to the blank identifier.  Make sure
  // that types are reasonable before calling base().
  if (lhs != NULL && lhs->forwarded()->forward_declaration_type() == NULL)
    {
      if (!is_conversion && lhs->is_sink_type())
	return true;

      // Check for hidden fields before checking for compatibility.
      if (rhs != NULL && rhs->forwarded()->forward_declaration_type() == NULL)
	{
	  if (lhs->has_hidden_fields(NULL, reason)
	      || rhs->has_hidden_fields(NULL, reason))
	    return false;
	}
    }

  if (Type::are_compatible_for(lhs, rhs,
			       (is_conversion
				? COMPATIBLE_FOR_CONVERSION
				: COMPATIBLE_COMPATIBLE),
			       reason))
    return true;

  const Type* orig_lhs = lhs;
  lhs = lhs->base();
  const Type* orig_rhs = rhs;
  rhs = rhs->base();

  // Any type may be assigned to an interface type if it implements
  // the required methods.  Note that we must pass ORIG_RHS here, not
  // RHS, since the methods will be attached to ORIG_RHS.
  if (lhs->interface_type() != NULL
      && lhs->interface_type()->implements_interface(orig_rhs, reason))
    return true;

  // Interface assignment is permitted if RHS is known to implement
  // all the methods in LHS.
  if (lhs->interface_type() != NULL
      && rhs->interface_type() != NULL
      && lhs->interface_type()->is_compatible_for_assign(rhs->interface_type(),
							 reason))
    return true;

  // Other than the cases checked above, two different named types may
  // be compatible for conversion, but are never compatible for
  // assignment.
  if (!is_conversion
      && orig_lhs->named_type() != NULL
      && orig_rhs->named_type() != NULL)
    return false;

  // The compatibility level to check for when checking subtypes.
  Type_compatible pass = (is_conversion
			  ? COMPATIBLE_FOR_CONVERSION
			  : COMPATIBLE_COMPATIBLE);

  // A pointer to an array may be assigned to a slice of the
  // same element type.
  if (lhs->is_open_array_type()
      && rhs->points_to() != NULL
      && rhs->points_to()->array_type() != NULL
      && !rhs->is_open_array_type())
    {
      Type* e1 = lhs->array_type()->element_type();
      Type* e2 = rhs->points_to()->array_type()->element_type();
      if (Type::are_compatible_for(e1, e2, pass, reason))
	return true;
    }

  // We can assign a bidirectional channel to a uni-directional
  // channel.
  if (lhs->channel_type() != NULL
      && rhs->channel_type() != NULL
      && rhs->channel_type()->may_send()
      && rhs->channel_type()->may_receive()
      && Type::are_compatible_for(lhs->channel_type()->element_type(),
				  rhs->channel_type()->element_type(),
				  pass, reason))
    return true;

  // A constant of abstract integer or float type may be assigned to
  // integer or float type.
  if (rhs->is_abstract()
      && (lhs->integer_type() != NULL || lhs->float_type() != NULL))
    return true;

  // The nil type may be assigned to a pointer type, an interface
  // type, a slice type, a map type, a channel type, or a function
  // type.
  if (rhs->is_nil_type()
      && (lhs->points_to() != NULL
	  || lhs->interface_type() != NULL
	  || lhs->is_open_array_type()
	  || lhs->map_type() != NULL
	  || lhs->channel_type() != NULL
	  || lhs->function_type() != NULL))
    return true;

  // Give some better error messages.
  if (reason != NULL && reason->empty())
    {
      if (rhs->interface_type() != NULL)
	reason->assign(_("need explicit conversion"));
      else if (rhs->is_call_multiple_result_type())
	reason->assign(_("multiple value function call in "
			 "single value context"));
    }

  return false;
}

// Return whether this type has any hidden fields.  This is only a
// possibility for a few types.

bool
Type::has_hidden_fields(const Named_type* within, std::string* reason) const
{
  switch (this->forwarded()->classification_)
    {
    case TYPE_NAMED:
      return this->named_type()->named_type_has_hidden_fields(reason);
    case TYPE_STRUCT:
      return this->struct_type()->struct_has_hidden_fields(within, reason);
    case TYPE_ARRAY:
      return this->array_type()->array_has_hidden_fields(within, reason);
    default:
      return false;
    }
}

// Return a hash code for the type to be used for method lookup.

size_t
Type::hash_for_method() const
{
  size_t ret = 0;
  if (this->classification_ != TYPE_FORWARD)
    ret += this->classification_;
  return ret + this->do_hash_for_method();
}

// Default implementation of do_hash_for_method.  This is appropriate
// for types with no subfields.

size_t
Type::do_hash_for_method() const
{
  return 0;
}

// Default check for the expression passed to make.  Any type which
// may be used with make implements its own version of this.

bool
Type::do_check_make_expression(Expression_list*, source_location)
{
  gcc_unreachable();
}

// Return a tree representing this type.

tree
Type::get_tree(Gogo* gogo)
{
  if (this->tree_ == NULL)
    {
      this->tree_ = this->do_get_tree(gogo);
      go_preserve_from_gc(this->tree_);
    }
  return this->tree_;
}

// Store an incomplete type tree during construction.

void
Type::set_incomplete_type_tree(tree incomplete)
{
  gcc_assert(this->tree_ == NULL);
  this->tree_ = incomplete;
}

// Return a tree representing a zero initialization for this type.

tree
Type::get_init_tree(Gogo* gogo, bool is_clear)
{
  return this->do_init_tree(gogo, is_clear);
}

// Any type which supports the builtin make function must implement
// this.

tree
Type::do_make_expression_tree(Translate_context*, Expression_list*,
			      source_location)
{
  gcc_unreachable();
}

// Return a tree copying VAL, a value of this type, into the reference
// count queue at ENTRY.  This modifies ENTRY.

tree
Type::set_refcount_queue_entry(Gogo* gogo, Refcounts* refcounts,
			       Refcount_entry* entry, tree val) const
{
  return this->do_set_refcount_queue_entry(gogo, refcounts, entry, val);
}

// Default implementation of do_set_refcount_queue_entry.  This works
// for any reference count type which is represented as a pointer.

tree
Type::do_set_refcount_queue_entry(Gogo* gogo, Refcounts* refcounts,
				  Refcount_entry* entry, tree val) const
{
  gcc_assert(this->is_refcounted());
  gcc_assert(POINTER_TYPE_P(TREE_TYPE(val)));
  tree ret = refcounts->set_entry_tree(gogo, *entry, val);
  entry->increment();
  return ret;
}

// Return a type descriptor for this type.

tree
Type::type_descriptor(Gogo* gogo)
{
  if (this->type_descriptor_decl_ == NULL_TREE)
    {
      this->do_type_descriptor_decl(gogo, NULL, &this->type_descriptor_decl_);
      gcc_assert(this->type_descriptor_decl_ != NULL_TREE
		 && (this->type_descriptor_decl_ == error_mark_node
		     || DECL_P(this->type_descriptor_decl_)));
    }
  if (this->type_descriptor_decl_ == error_mark_node)
    return error_mark_node;
  return build_fold_addr_expr(this->type_descriptor_decl_);
}

// Set *PDECL to the decl for a type descriptor for TYPE with name
// NAME.

void
Type::named_type_descriptor(Gogo* gogo, Type* type, Named_type* name,
			    tree* pdecl)
{
  gcc_assert(name != NULL && type->named_type() != name);
  type->do_type_descriptor_decl(gogo, name, pdecl);
}

// Return the type reflection string for this type.

std::string
Type::reflection(Gogo* gogo) const
{
  std::string ret;

  // The do_reflection virtual function should set RET to the
  // reflection string.
  this->do_reflection(gogo, &ret);

  return ret;
}

// Return a mangled name for the type.

std::string
Type::mangled_name(Gogo* gogo) const
{
  std::string ret;

  // The do_mangled_name virtual function should set RET to the
  // mangled name.  For a composite type it should append a code for
  // the composition and then call do_mangled_name on the components.
  this->do_mangled_name(gogo, &ret);

  return ret;
}

// Default function to export a type.

void
Type::do_export(Export*) const
{
  gcc_unreachable();
}

// Import a type.

Type*
Type::import_type(Import* imp)
{
  if (imp->match_c_string("("))
    return Function_type::do_import(imp);
  else if (imp->match_c_string("*"))
    return Pointer_type::do_import(imp);
  else if (imp->match_c_string("struct "))
    return Struct_type::do_import(imp);
  else if (imp->match_c_string("["))
    return Array_type::do_import(imp);
  else if (imp->match_c_string("map "))
    return Map_type::do_import(imp);
  else if (imp->match_c_string("chan "))
    return Channel_type::do_import(imp);
  else if (imp->match_c_string("interface"))
    return Interface_type::do_import(imp);
  else if (imp->match_c_string("..."))
    {
      imp->advance(3);
      return Type::make_varargs_type();
    }
  else
    {
      error_at(imp->location(), "import error: expected type");
      return Type::make_error_type();
    }
}

// A type used to indicate a parsing error.  This exists to simplify
// later error detection.

class Error_type : public Type
{
 public:
  Error_type()
    : Type(TYPE_ERROR)
  { }

 protected:
  tree
  do_get_tree(Gogo*)
  { return error_mark_node; }

  tree
  do_init_tree(Gogo*, bool)
  { return error_mark_node; }

  void
  do_type_descriptor_decl(Gogo*, Named_type*, tree* pdecl)
  { *pdecl = error_mark_node; }

  void
  do_reflection(Gogo*, std::string*) const
  { gcc_assert(errorcount + sorrycount > 0); }

  void
  do_mangled_name(Gogo*, std::string* ret) const
  { ret->push_back('E'); }
};

Type*
Type::make_error_type()
{
  static Error_type singleton_error_type;
  return &singleton_error_type;
}

// The void type.

class Void_type : public Type
{
 public:
  Void_type()
    : Type(TYPE_VOID)
  { }

 protected:
  tree
  do_get_tree(Gogo*)
  { return void_type_node; }

  tree
  do_init_tree(Gogo*, bool)
  { gcc_unreachable(); }

  void
  do_type_descriptor_decl(Gogo*, Named_type*, tree*)
  { gcc_unreachable(); }

  void
  do_reflection(Gogo*, std::string*) const
  { }

  void
  do_mangled_name(Gogo*, std::string* ret) const
  { ret->push_back('v'); }
};

Type*
Type::make_void_type()
{
  static Void_type singleton_void_type;
  return &singleton_void_type;
}

// The boolean type.

class Boolean_type : public Type
{
 public:
  Boolean_type()
    : Type(TYPE_BOOLEAN)
  { }

 protected:
  tree
  do_get_tree(Gogo*)
  { return boolean_type_node; }

  tree
  do_init_tree(Gogo*, bool is_clear)
  { return is_clear ? NULL : boolean_false_node; }

  void
  do_type_descriptor_decl(Gogo* gogo, Named_type* name, tree* pdecl);

  // We should not be asked for the reflection string of a basic type.
  void
  do_reflection(Gogo*, std::string* ret) const
  { ret->append("bool"); }

  void
  do_mangled_name(Gogo*, std::string* ret) const
  { ret->push_back('b'); }
};

// Make the type descriptor.

void
Boolean_type::do_type_descriptor_decl(Gogo* gogo, Named_type* name, tree* pdecl)
{
  if (name != NULL)
    gogo->type_descriptor_decl(RUNTIME_TYPE_CODE_BOOL, this, name, pdecl);
  else
    {
      Named_object* no = gogo->lookup_global("bool");
      gcc_assert(no != NULL);
      *pdecl = build_fold_indirect_ref(no->type_value()->type_descriptor(gogo));
    }
}

Type*
Type::make_boolean_type()
{
  static Boolean_type boolean_type;
  return &boolean_type;
}

// The named type "bool".

static Named_type* named_bool_type;

// Get the named type "bool".

Named_type*
Type::lookup_bool_type()
{
  return named_bool_type;
}

// Make the named type "bool".

Named_type*
Type::make_named_bool_type()
{
  Type* bool_type = Type::make_boolean_type();
  Named_object* named_object = Named_object::make_type("bool", NULL,
						       bool_type,
						       BUILTINS_LOCATION);
  Named_type* named_type = named_object->type_value();
  named_bool_type = named_type;
  return named_type;
}

// Class Integer_type.

Integer_type::Named_integer_types Integer_type::named_integer_types;

// Create a new integer type.  Non-abstract integer types always have
// names.

Named_type*
Integer_type::create_integer_type(const char* name, bool is_unsigned,
				  int bits, int runtime_type_code)
{
  Integer_type* integer_type = new Integer_type(false, is_unsigned, bits,
						runtime_type_code);
  std::string sname(name);
  Named_object* named_object = Named_object::make_type(sname, NULL,
						       integer_type,
						       BUILTINS_LOCATION);
  Named_type* named_type = named_object->type_value();
  std::pair<Named_integer_types::iterator, bool> ins =
    Integer_type::named_integer_types.insert(std::make_pair(sname, named_type));
  gcc_assert(ins.second);
  return named_type;
}

// Look up an existing integer type.

Named_type*
Integer_type::lookup_integer_type(const char* name)
{
  Named_integer_types::const_iterator p =
    Integer_type::named_integer_types.find(name);
  gcc_assert(p != Integer_type::named_integer_types.end());
  return p->second;
}

// Create a new abstract integer type.

Integer_type*
Integer_type::create_abstract_integer_type()
{
  static Integer_type* abstract_type;
  if (abstract_type == NULL)
    abstract_type = new Integer_type(true, false, INT_TYPE_SIZE,
				     RUNTIME_TYPE_CODE_INT);
  return abstract_type;
}

// Integer type compatibility.

bool
Integer_type::is_compatible(const Integer_type* t) const
{
  if (this->is_unsigned_ != t->is_unsigned_ || this->bits_ != t->bits_)
    return false;
  return this->is_abstract_ == t->is_abstract_;
}

// Hash code.

size_t
Integer_type::do_hash_for_method() const
{
  return ((this->bits_ << 4)
	  + ((this->is_unsigned_ ? 1 : 0) << 8)
	  + ((this->is_abstract_ ? 1 : 0) << 9));
}

// Get the tree for an Integer_type.

tree
Integer_type::do_get_tree(Gogo*)
{
  gcc_assert(!this->is_abstract_);
  if (this->is_unsigned_)
    {
      if (this->bits_ == INT_TYPE_SIZE)
	return unsigned_type_node;
      else if (this->bits_ == CHAR_TYPE_SIZE)
	return unsigned_char_type_node;
      else if (this->bits_ == SHORT_TYPE_SIZE)
	return short_unsigned_type_node;
      else if (this->bits_ == LONG_TYPE_SIZE)
	return long_unsigned_type_node;
      else if (this->bits_ == LONG_LONG_TYPE_SIZE)
	return long_long_unsigned_type_node;
      else
	return make_unsigned_type(this->bits_);
    }
  else
    {
      if (this->bits_ == INT_TYPE_SIZE)
	return integer_type_node;
      else if (this->bits_ == CHAR_TYPE_SIZE)
	return signed_char_type_node;
      else if (this->bits_ == SHORT_TYPE_SIZE)
	return short_integer_type_node;
      else if (this->bits_ == LONG_TYPE_SIZE)
	return long_integer_type_node;
      else if (this->bits_ == LONG_LONG_TYPE_SIZE)
	return long_long_integer_type_node;
      else
	return make_signed_type(this->bits_);
    }
}

tree
Integer_type::do_init_tree(Gogo* gogo, bool is_clear)
{
  return is_clear ? NULL : build_int_cst(this->get_tree(gogo), 0);
}

// The type descriptor for an integer type.  Integer types are always
// named.

void
Integer_type::do_type_descriptor_decl(Gogo* gogo, Named_type* name, tree* pdecl)
{
  gcc_assert(name != NULL);
  gogo->type_descriptor_decl(this->runtime_type_code_, this, name, pdecl);
}

// We should not be asked for the reflection string of a basic type.

void
Integer_type::do_reflection(Gogo*, std::string*) const
{
  gcc_unreachable();
}

// Mangled name.

void
Integer_type::do_mangled_name(Gogo*, std::string* ret) const
{
  char buf[100];
  snprintf(buf, sizeof buf, "i%s%s%de",
	   this->is_abstract_ ? "a" : "",
	   this->is_unsigned_ ? "u" : "",
	   this->bits_);
  ret->append(buf);
}

// Make an integer type.

Named_type*
Type::make_integer_type(const char* name, bool is_unsigned, int bits,
			int runtime_type_code)
{
  return Integer_type::create_integer_type(name, is_unsigned, bits,
					   runtime_type_code);
}

// Make an abstract integer type.

Integer_type*
Type::make_abstract_integer_type()
{
  return Integer_type::create_abstract_integer_type();
}

// Look up an integer type.

Named_type*
Type::lookup_integer_type(const char* name)
{
  return Integer_type::lookup_integer_type(name);
}

// Class Float_type.

Float_type::Named_float_types Float_type::named_float_types;

// Create a new float type.  Non-abstract float types always have
// names.

Named_type*
Float_type::create_float_type(const char* name, int bits,
			      int runtime_type_code)
{
  Float_type* float_type = new Float_type(false, bits, runtime_type_code);
  std::string sname(name);
  Named_object* named_object = Named_object::make_type(sname, NULL, float_type,
						       BUILTINS_LOCATION);
  Named_type* named_type = named_object->type_value();
  std::pair<Named_float_types::iterator, bool> ins =
    Float_type::named_float_types.insert(std::make_pair(sname, named_type));
  gcc_assert(ins.second);
  return named_type;
}

// Look up an existing float type.

Named_type*
Float_type::lookup_float_type(const char* name)
{
  Named_float_types::const_iterator p =
    Float_type::named_float_types.find(name);
  gcc_assert(p != Float_type::named_float_types.end());
  return p->second;
}

// Create a new abstract float type.

Float_type*
Float_type::create_abstract_float_type()
{
  static Float_type* abstract_type;
  if (abstract_type == NULL)
    abstract_type = new Float_type(true, FLOAT_TYPE_SIZE,
				   RUNTIME_TYPE_CODE_FLOAT);
  return abstract_type;
}

// Whether this type is compatible with T.

bool
Float_type::is_compatible(const Float_type* t) const
{
  if (this->bits_ != t->bits_)
    return false;
  return this->is_abstract_ == t->is_abstract_;
}

// Hash code.

size_t
Float_type::do_hash_for_method() const
{
  return (this->bits_ << 4) + ((this->is_abstract_ ? 1 : 0) << 8);
}

// Get a tree without using a Gogo*.

tree
Float_type::type_tree() const
{
  if (this->bits_ == FLOAT_TYPE_SIZE)
    return float_type_node;
  else if (this->bits_ == DOUBLE_TYPE_SIZE)
    return double_type_node;
  else if (this->bits_ == LONG_DOUBLE_TYPE_SIZE)
    return long_double_type_node;
  else
    {
      tree ret = make_node(REAL_TYPE);
      TYPE_PRECISION(ret) = this->bits_;
      layout_type(ret);
      return ret;
    }
}

// Get a tree.

tree
Float_type::do_get_tree(Gogo*)
{
  return this->type_tree();
}

tree
Float_type::do_init_tree(Gogo* gogo, bool is_clear)
{
  if (is_clear)
    return NULL;
  tree type = this->get_tree(gogo);
  REAL_VALUE_TYPE r;
  real_from_integer(&r, TYPE_MODE(type), 0, 0, 0);
  return build_real(type, r);
}

// The type descriptor for a float type.  Float types are always named.

void
Float_type::do_type_descriptor_decl(Gogo* gogo, Named_type* name, tree* pdecl)
{
  gcc_assert(name != NULL);
  gogo->type_descriptor_decl(this->runtime_type_code_, this, name, pdecl);
}

// We should not be asked for the reflection string of a basic type.

void
Float_type::do_reflection(Gogo*, std::string*) const
{
  gcc_unreachable();
}

// Mangled name.

void
Float_type::do_mangled_name(Gogo*, std::string* ret) const
{
  char buf[100];
  snprintf(buf, sizeof buf, "f%s%de",
	   this->is_abstract_ ? "a" : "",
	   this->bits_);
  ret->append(buf);
}

// Make a floating point type.

Named_type*
Type::make_float_type(const char* name, int bits, int runtime_type_code)
{
  return Float_type::create_float_type(name, bits, runtime_type_code);
}

// Make an abstract float type.

Float_type*
Type::make_abstract_float_type()
{
  return Float_type::create_abstract_float_type();
}

// Look up a float type.

Named_type*
Type::lookup_float_type(const char* name)
{
  return Float_type::lookup_float_type(name);
}

// Class String_type.

// Return the tree for String_type.  We represent strings as a pointer
// to a struct.
//   struct __go_string { size_t __length; unsigned char __data[]; };

tree
String_type::do_get_tree(Gogo*)
{
  static tree struct_type;
  if (struct_type == NULL_TREE)
    {
      struct_type = make_node(RECORD_TYPE);

      tree name = get_identifier("__length");
      tree field = build_decl(BUILTINS_LOCATION, FIELD_DECL, name, sizetype);
      DECL_CONTEXT(field) = struct_type;
      TYPE_FIELDS(struct_type) = field;
      tree last_field = field;

      name = get_identifier("__data");
      tree t = build_qualified_type(unsigned_char_type_node, TYPE_QUAL_CONST);
      t = build_array_type(t, NULL);
      field = build_decl(BUILTINS_LOCATION, FIELD_DECL, name, t);
      DECL_CONTEXT(field) = struct_type;
      TREE_CHAIN(last_field) = field;

      layout_type(struct_type);

      go_preserve_from_gc(struct_type);

      // Give the struct a name for better debugging info.
      name = get_identifier("__go_string");
      tree type_decl = build_decl(BUILTINS_LOCATION, TYPE_DECL, name,
				  struct_type);
      DECL_ARTIFICIAL(type_decl) = 1;
      TYPE_NAME(struct_type) = type_decl;
      go_preserve_from_gc(type_decl);
      rest_of_decl_compilation(type_decl, 1, 0);
    }
  return build_pointer_type(struct_type);
}

// Return a tree for the length of STRING.

tree
String_type::length_tree(Gogo*, tree string)
{
  tree string_type = TREE_TYPE(string);
  gcc_assert(POINTER_TYPE_P(string_type));
  tree struct_type = TREE_TYPE(string_type);
  gcc_assert(TREE_CODE(struct_type) == RECORD_TYPE);
  tree length_field = TYPE_FIELDS(struct_type);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(length_field)),
		    "__length") == 0);

  string = save_expr(string);
  return fold_build3(COND_EXPR, sizetype,
		     fold_build2(EQ_EXPR, boolean_type_node, string,
				 fold_convert(string_type, null_pointer_node)),
		     size_zero_node,
		     fold_build3(COMPONENT_REF, sizetype,
				 build_fold_indirect_ref(string),
				 length_field, NULL_TREE));
}

// Return a tree for a pointer to the bytes of STRING.

tree
String_type::bytes_tree(Gogo*, tree string)
{
  tree string_type = TREE_TYPE(string);
  gcc_assert(POINTER_TYPE_P(string_type));
  tree struct_type = TREE_TYPE(string_type);
  gcc_assert(TREE_CODE(struct_type) == RECORD_TYPE);
  tree bytes_field = TREE_CHAIN(TYPE_FIELDS(struct_type));
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(bytes_field)),
		    "__data") == 0);

  string = save_expr(string);

  tree r = fold_build3(COMPONENT_REF, TREE_TYPE(bytes_field),
		       build_fold_indirect_ref(string),
		       bytes_field, NULL_TREE);
  r = build_fold_addr_expr(r);
  r = fold_convert(Gogo::const_char_pointer_type_tree(), r);

  return fold_build3(COND_EXPR, TREE_TYPE(r),
		     fold_build2(EQ_EXPR, boolean_type_node, string,
				 fold_convert(string_type, null_pointer_node)),
		     fold_convert(TREE_TYPE(r), null_pointer_node),
		     r);
}

// We initialize a string to simply a nil pointer.

tree
String_type::do_init_tree(Gogo* gogo, bool is_clear)
{
  return is_clear ? NULL : build_int_cst(this->get_tree(gogo), 0);
}

// The type descriptor for the string type.

void
String_type::do_type_descriptor_decl(Gogo* gogo, Named_type* name, tree* pdecl)
{
  if (name != NULL)
    gogo->type_descriptor_decl(RUNTIME_TYPE_CODE_STRING, this, name, pdecl);
  else
    {
      Named_object* no = gogo->lookup_global("string");
      gcc_assert(no != NULL);
      *pdecl = build_fold_indirect_ref(no->type_value()->type_descriptor(gogo));
    }
}

// We should not be asked for the reflection string of a basic type.

void
String_type::do_reflection(Gogo*, std::string* ret) const
{
  ret->append("string");
}

// Mangled name of a string type.

void
String_type::do_mangled_name(Gogo*, std::string* ret) const
{
  ret->push_back('z');
}

// Make a string type.

Type*
Type::make_string_type()
{
  static String_type string_type;
  return &string_type;
}

// The named type "string".

static Named_type* named_string_type;

// Get the named type "string".

Named_type*
Type::lookup_string_type()
{
  return named_string_type;
}

// Make the named type string.

Named_type*
Type::make_named_string_type()
{
  Type* string_type = Type::make_string_type();
  Named_object* named_object = Named_object::make_type("string", NULL,
						       string_type,
						       BUILTINS_LOCATION);
  Named_type* named_type = named_object->type_value();
  named_string_type = named_type;
  return named_type;
}

// The sink type.  This is the type of the blank identifier _.  Any
// type may be assigned to it.

class Sink_type : public Type
{
 public:
  Sink_type()
    : Type(TYPE_SINK)
  { }

 protected:
  tree
  do_get_tree(Gogo*)
  { gcc_unreachable(); }

  tree
  do_init_tree(Gogo*, bool)
  { gcc_unreachable(); }

  void
  do_type_descriptor_decl(Gogo*, Named_type*, tree*)
  { gcc_unreachable(); }

  void
  do_reflection(Gogo*, std::string*) const
  { gcc_unreachable(); }

  void
  do_mangled_name(Gogo*, std::string*) const
  { gcc_unreachable(); }
};

// Make the sink type.

Type*
Type::make_sink_type()
{
  static Sink_type sink_type;
  return &sink_type;
}

// Class Function_type.

// Traversal.

int
Function_type::do_traverse(Traverse* traverse)
{
  if (this->receiver_ != NULL
      && Type::traverse(this->receiver_->type(), traverse) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  if (this->parameters_ != NULL
      && this->parameters_->traverse(traverse) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  if (this->results_ != NULL
      && this->results_->traverse(traverse) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  return TRAVERSE_CONTINUE;
}

// Returns whether T is a valid redeclaration of this type.  If this
// returns false, and REASON is not NULL, *REASON may be set to a
// brief explanation of why it returned false.

bool
Function_type::is_valid_redeclaration(const Function_type* t,
				      std::string* reason) const
{
  if (!this->is_compatible(t, COMPATIBLE_IDENTICAL, false, reason))
    return false;

  // A redeclaration of a function is required to use the same names
  // for the receiver and parameters.
  if (this->receiver() != NULL
      && this->receiver()->name() != t->receiver()->name()
      && this->receiver()->name() != Import::import_marker
      && t->receiver()->name() != Import::import_marker)
    {
      if (reason != NULL)
	*reason = "receiver name changed";
      return false;
    }

  const Typed_identifier_list* parms1 = this->parameters();
  const Typed_identifier_list* parms2 = t->parameters();
  if (parms1 != NULL)
    {
      Typed_identifier_list::const_iterator p1 = parms1->begin();
      for (Typed_identifier_list::const_iterator p2 = parms2->begin();
	   p2 != parms2->end();
	   ++p2, ++p1)
	{
	  if (p1->name() != p2->name()
	      && p1->name() != Import::import_marker
	      && p2->name() != Import::import_marker)
	    {
	      if (reason != NULL)
		*reason = "parameter name changed";
	      return false;
	    }

	  // This is called at parse time, so we may have unknown
	  // types.  They will appear compatible according to
	  // Type::is_compatible.
	  Type* t1 = p1->type()->forwarded();
	  Type* t2 = p2->type()->forwarded();
	  if (t1 != t2
	      && t1->forward_declaration_type() != NULL
	      && (t2->forward_declaration_type() == NULL
		  || (t1->forward_declaration_type()->named_object()
		      != t2->forward_declaration_type()->named_object())))
	    return false;
	}
    }

  const Typed_identifier_list* results1 = this->results();
  const Typed_identifier_list* results2 = t->results();
  if (results1 != NULL)
    {
      Typed_identifier_list::const_iterator res1 = results1->begin();
      for (Typed_identifier_list::const_iterator res2 = results2->begin();
	   res2 != results2->end();
	   ++res2, ++res1)
	{
	  if (res1->name() != res2->name()
	      && res1->name() != Import::import_marker
	      && res2->name() != Import::import_marker)
	    {
	      if (reason != NULL)
		*reason = "result name changed";
	      return false;
	    }

	  // This is called at parse time, so we may have unknown
	  // types.  They will appear compatible according to
	  // Type::is_compatible.
	  Type* t1 = res1->type()->forwarded();
	  Type* t2 = res2->type()->forwarded();
	  if (t1 != t2
	      && t1->forward_declaration_type() != NULL
	      && (t2->forward_declaration_type() == NULL
		  || (t1->forward_declaration_type()->named_object()
		      != t2->forward_declaration_type()->named_object())))
	    return false;
	}
    }

  return true;
}

// Check whether T is the same as this type.

bool
Function_type::is_compatible(const Function_type* t,
			     Type_compatible compatible,
			     bool ignore_receiver,
			     std::string* reason) const
{
  if (!ignore_receiver)
    {
      const Typed_identifier* r1 = this->receiver();
      const Typed_identifier* r2 = t->receiver();
      if ((r1 != NULL) != (r2 != NULL))
	{
	  if (reason != NULL)
	    *reason = _("different receiver types");
	  return false;
	}
      if (r1 != NULL)
	{
	  if (!Type::are_compatible_for(r1->type(), r2->type(), compatible,
					reason))
	    {
	      if (reason != NULL && !reason->empty())
		*reason = "receiver: " + *reason;
	      return false;
	    }
	}
    }

  const Typed_identifier_list* parms1 = this->parameters();
  const Typed_identifier_list* parms2 = t->parameters();
  if ((parms1 != NULL) != (parms2 != NULL))
    {
      if (reason != NULL)
	*reason = _("different number of parameters");
      return false;
    }
  if (parms1 != NULL)
    {
      Typed_identifier_list::const_iterator p1 = parms1->begin();
      for (Typed_identifier_list::const_iterator p2 = parms2->begin();
	   p2 != parms2->end();
	   ++p2, ++p1)
	{
	  if (p1 == parms1->end())
	    {
	      if (reason != NULL)
		*reason = _("different number of parameters");
	      return false;
	    }

	  if (!Type::are_compatible_for(p1->type(), p2->type(), compatible,
					NULL))
	    {
	      if (reason != NULL)
		*reason = _("different parameter types");
	      return false;
	    }
	}
      if (p1 != parms1->end())
	{
	  if (reason != NULL)
	    *reason = _("different number of parameters");
	return false;
	}
    }

  if (this->is_varargs() != t->is_varargs())
    {
      if (reason != NULL)
	*reason = _("different varargs");
      return false;
    }

  const Typed_identifier_list* results1 = this->results();
  const Typed_identifier_list* results2 = t->results();
  if ((results1 != NULL) != (results2 != NULL))
    {
      if (reason != NULL)
	*reason = _("different number of results");
      return false;
    }
  if (results1 != NULL)
    {
      Typed_identifier_list::const_iterator res1 = results1->begin();
      for (Typed_identifier_list::const_iterator res2 = results2->begin();
	   res2 != results2->end();
	   ++res2, ++res1)
	{
	  if (res1 == results1->end())
	    {
	      if (reason != NULL)
		*reason = _("different number of results");
	      return false;
	    }

	  if (!Type::are_compatible_for(res1->type(), res2->type(), compatible,
					NULL))
	    {
	      if (reason != NULL)
		*reason = _("different result types");
	      return false;
	    }
	}
      if (res1 != results1->end())
	{
	  if (reason != NULL)
	    *reason = _("different number of results");
	  return false;
	}
    }

  return true;
}

// Hash code.

size_t
Function_type::do_hash_for_method() const
{
  size_t ret = 0;
  // We ignore the receiver type for hash codes, because we need to
  // get the same hash code for a method in an interface and a method
  // declared for a type.  The former will not have a receiver.
  if (this->parameters_ != NULL)
    {
      int shift = 1;
      for (Typed_identifier_list::const_iterator p = this->parameters_->begin();
	   p != this->parameters_->end();
	   ++p, ++shift)
	ret += p->type()->hash_for_method() << shift;
    }
  if (this->results_ != NULL)
    {
      int shift = 2;
      for (Typed_identifier_list::const_iterator p = this->results_->begin();
	   p != this->results_->end();
	   ++p, ++shift)
	ret += p->type()->hash_for_method() << shift;
    }
  if (this->is_varargs_)
    ret += 1;
  ret <<= 4;
  return ret;
}

// Get the tree for a function type.

tree
Function_type::do_get_tree(Gogo* gogo)
{
  tree args = NULL_TREE;
  tree* pp = &args;

  if (this->receiver_ != NULL)
    {
      Type* rtype = this->receiver_->type();
      tree ptype = rtype->get_tree(gogo);
      if (ptype == error_mark_node)
	return error_mark_node;

      // We always pass the address of the receiver parameter, in
      // order to make interface calls work with unknown types.
      if (rtype->points_to() == NULL)
	ptype = build_pointer_type(ptype);

      *pp = tree_cons (NULL_TREE, ptype, NULL_TREE);
      pp = &TREE_CHAIN (*pp);
    }

  if (this->parameters_ != NULL)
    {
      for (Typed_identifier_list::const_iterator p = this->parameters_->begin();
	   p != this->parameters_->end();
	   ++p)
	{
	  tree ptype = p->type()->get_tree(gogo);
	  if (ptype == error_mark_node)
	    return error_mark_node;
	  *pp = tree_cons (NULL_TREE, ptype, NULL_TREE);
	  pp = &TREE_CHAIN (*pp);
	}
    }

  // Varargs is handled entirely at the Go level.  At the tree level,
  // functions are not varargs.
  *pp = void_list_node;

  tree result;
  if (this->results_ == NULL)
    result = void_type_node;
  else if (this->results_->size() == 1)
    result = this->results_->begin()->type()->get_tree(gogo);
  else
    {
      result = make_node(RECORD_TYPE);
      tree field_trees = NULL_TREE;
      tree* pp = &field_trees;
      for (Typed_identifier_list::const_iterator p = this->results_->begin();
	   p != this->results_->end();
	   ++p)
	{
	  const std::string name = (p->name().empty()
				    ? "UNNAMED"
				    : Gogo::unpack_hidden_name(p->name()));
	  tree name_tree = get_identifier_with_length(name.data(),
						      name.length());
	  tree field_type_tree = p->type()->get_tree(gogo);
	  if (field_type_tree == error_mark_node)
	    return error_mark_node;
	  tree field = build_decl(this->location_, FIELD_DECL, name_tree,
				  field_type_tree);
	  DECL_CONTEXT(field) = result;
	  *pp = field;
	  pp = &TREE_CHAIN(field);
	}
      TYPE_FIELDS(result) = field_trees;
      layout_type(result);
    }

  if (result == error_mark_node)
    return error_mark_node;

  tree ret = build_function_type(result, args);
  if (ret == error_mark_node)
    return ret;

  // The type "func ()" is represented as a pointer to a function.
  return build_pointer_type(ret);
}

// Functions are initialized to NULL.

tree
Function_type::do_init_tree(Gogo* gogo, bool is_clear)
{
  if (is_clear)
    return NULL;
  tree type_tree = this->get_tree(gogo);
  if (type_tree == error_mark_node)
    return error_mark_node;
  return fold_convert(type_tree, null_pointer_node);
}

// The type descriptor for a function.

void
Function_type::do_type_descriptor_decl(Gogo* gogo, Named_type* name,
				       tree* pdecl)
{
  gogo->function_type_descriptor_decl(this, name, pdecl);
}

// The reflection string.

void
Function_type::do_reflection(Gogo* gogo, std::string* ret) const
{
  // FIXME: Turn this off until we straighten out the type of the
  // struct field used in a go statement which calls a method.
  // gcc_assert(this->receiver_ == NULL);

  ret->append("func");

  if (this->receiver_ != NULL)
    {
      ret->push_back('(');
      this->append_reflection(this->receiver_->type(), gogo, ret);
      ret->push_back(')');
    }

  ret->push_back('(');
  const Typed_identifier_list* params = this->parameters();
  if (params != NULL)
    {
      for (Typed_identifier_list::const_iterator p = params->begin();
	   p != params->end();
	   ++p)
	{
	  if (p != params->begin())
	    ret->append(", ");
	  this->append_reflection(p->type(), gogo, ret);
	}
    }
  ret->push_back(')');

  const Typed_identifier_list* results = this->results();
  if (results != NULL)
    {
      ret->append(" (");
      for (Typed_identifier_list::const_iterator p = results->begin();
	   p != results->end();
	   ++p)
	{
	  if (p != results->begin())
	    ret->append(", ");
	  this->append_reflection(p->type(), gogo, ret);
	}
      ret->push_back(')');
    }
}

// Mangled name.

void
Function_type::do_mangled_name(Gogo* gogo, std::string* ret) const
{
  ret->push_back('F');

  if (this->receiver_ != NULL)
    {
      ret->push_back('m');
      this->append_mangled_name(this->receiver_->type(), gogo, ret);
    }

  const Typed_identifier_list* params = this->parameters();
  if (params != NULL)
    {
      ret->push_back('p');
      for (Typed_identifier_list::const_iterator p = params->begin();
	   p != params->end();
	   ++p)
	this->append_mangled_name(p->type(), gogo, ret);
      ret->push_back('e');
    }

  const Typed_identifier_list* results = this->results();
  if (results != NULL)
    {
      ret->push_back('r');
      for (Typed_identifier_list::const_iterator p = results->begin();
	   p != results->end();
	   ++p)
	this->append_mangled_name(p->type(), gogo, ret);
      ret->push_back('e');
    }

  ret->push_back('e');
}

// Export a function type.

void
Function_type::do_export(Export* exp) const
{
  // We don't write out the receiver.  The only function types which
  // should have a receiver are the ones associated with explicitly
  // defined methods.  For those the receiver type is written out by
  // Function::export_func.

  exp->write_c_string("(");
  bool first = true;
  if (this->parameters_ != NULL)
    {
      for (Typed_identifier_list::const_iterator p =
	     this->parameters_->begin();
	   p != this->parameters_->end();
	   ++p)
	{
	  if (first)
	    first = false;
	  else
	    exp->write_c_string(", ");
	  exp->write_type(p->type());
	}
    }
  exp->write_c_string(")");

  const Typed_identifier_list* results = this->results_;
  if (results != NULL)
    {
      exp->write_c_string(" ");
      if (results->size() == 1)
	exp->write_type(results->begin()->type());
      else
	{
	  first = true;
	  exp->write_c_string("(");
	  for (Typed_identifier_list::const_iterator p = results->begin();
	       p != results->end();
	       ++p)
	    {
	      if (first)
		first = false;
	      else
		exp->write_c_string(", ");
	      exp->write_type(p->type());
	    }
	  exp->write_c_string(")");
	}
    }
}

// Import a function type.

Function_type*
Function_type::do_import(Import* imp)
{
  imp->require_c_string("(");
  Typed_identifier_list* parameters;
  bool is_varargs = false;
  if (imp->peek_char() == ')')
    parameters = NULL;
  else
    {
      parameters = new Typed_identifier_list();
      while (true)
	{
	  Type* ptype = imp->read_type();
	  if (ptype->is_varargs_type())
	    is_varargs = true;
	  parameters->push_back(Typed_identifier(Import::import_marker,
						 ptype, imp->location()));
	  if (imp->peek_char() != ',')
	    break;
	  imp->require_c_string(", ");
	}
    }
  imp->require_c_string(")");

  Typed_identifier_list* results;
  if (imp->peek_char() != ' ')
    results = NULL;
  else
    {
      imp->advance(1);
      results = new Typed_identifier_list;
      if (imp->peek_char() != '(')
	{
	  Type* rtype = imp->read_type();
	  results->push_back(Typed_identifier(Import::import_marker, rtype,
					      imp->location()));
	}
      else
	{
	  imp->advance(1);
	  while (true)
	    {
	      Type* rtype = imp->read_type();
	      results->push_back(Typed_identifier(Import::import_marker,
						  rtype, imp->location()));
	      if (imp->peek_char() != ',')
		break;
	      imp->require_c_string(", ");
	    }
	  imp->require_c_string(")");
	}
    }

  Function_type* ret = Type::make_function_type(NULL, parameters, results,
						imp->location());
  if (is_varargs)
    ret->set_is_varargs();
  return ret;
}

// The type of a varargs parameter: an empty interface.

Type*
Function_type::varargs_type()
{
  static Type* vt;
  if (vt == NULL)
    {
      Typed_identifier_list* methods = new Typed_identifier_list();
      vt = Type::make_interface_type(methods, BUILTINS_LOCATION);
    }
  return vt;
}

// Make a function type.

Function_type*
Type::make_function_type(Typed_identifier* receiver,
			 Typed_identifier_list* parameters,
			 Typed_identifier_list* results,
			 source_location location)
{
  return new Function_type(receiver, parameters, results, location);
}

// The varargs type.  This is the type of the last parameter of a
// varargs function.

class Varargs_type : public Type
{
 public:
  Varargs_type()
    : Type(TYPE_VARARGS)
  { }

 protected:
  tree
  do_get_tree(Gogo*);

  tree
  do_init_tree(Gogo*, bool)
  { gcc_unreachable(); }

  void
  do_type_descriptor_decl(Gogo* gogo, Named_type* name, tree* pdecl)
  {
    gogo->type_descriptor_decl(RUNTIME_TYPE_CODE_DOTDOTDOT, this, name, pdecl);
  }

  void
  do_reflection(Gogo*, std::string* ret) const
  { ret->append("..."); }

  void
  do_mangled_name(Gogo*, std::string* ret) const
  { ret->push_back('V'); }

  void
  do_export(Export* exp) const
  { exp->write_c_string("..."); }
};

// The varargs type is an empty interface.

tree
Varargs_type::do_get_tree(Gogo* gogo)
{
  return Function_type::varargs_type()->get_tree(gogo);
}

// Make the varargs type.

Type*
Type::make_varargs_type()
{
  static Varargs_type singleton_varargs_type;
  return &singleton_varargs_type;
}

// Class Pointer_type.

// Traversal.

int
Pointer_type::do_traverse(Traverse* traverse)
{
  return Type::traverse(this->to_type_, traverse);
}

// Hash code.

size_t
Pointer_type::do_hash_for_method() const
{
  return this->to_type_->hash_for_method() << 4;
}

// The tree for a pointer type.

tree
Pointer_type::do_get_tree(Gogo* gogo)
{
  return build_pointer_type(this->to_type_->get_tree(gogo));
}

// Initialize a pointer type.

tree
Pointer_type::do_init_tree(Gogo* gogo, bool is_clear)
{
  if (is_clear)
    return NULL;
  tree type_tree = this->get_tree(gogo);
  if (type_tree == error_mark_node)
    return error_mark_node;
  return fold_convert(type_tree, null_pointer_node);
}

// The type descriptor.

void
Pointer_type::do_type_descriptor_decl(Gogo* gogo, Named_type* name, tree* pdecl)
{
  if (!this->is_unsafe_pointer_type())
    gogo->pointer_type_descriptor_decl(this, name, pdecl);
  else
    {
      gcc_assert(name != NULL);
      gogo->type_descriptor_decl(RUNTIME_TYPE_CODE_UNSAFE_POINTER, this,
				 name, pdecl);
    }
}

// Reflection string.

void
Pointer_type::do_reflection(Gogo* gogo, std::string* ret) const
{
  ret->push_back('*');
  this->append_reflection(this->to_type_, gogo, ret);
}

// Mangled name.

void
Pointer_type::do_mangled_name(Gogo* gogo, std::string* ret) const
{
  ret->push_back('p');
  this->append_mangled_name(this->to_type_, gogo, ret);
}

// Export.

void
Pointer_type::do_export(Export* exp) const
{
  exp->write_c_string("*");
  if (this->is_unsafe_pointer_type())
    exp->write_c_string("any");
  else
    exp->write_type(this->to_type_);
}

// Import.

Pointer_type*
Pointer_type::do_import(Import* imp)
{
  imp->require_c_string("*");
  if (imp->match_c_string("any"))
    {
      imp->advance(3);
      return Type::make_pointer_type(Type::make_void_type());
    }
  Type* to = imp->read_type();
  return Type::make_pointer_type(to);
}

// Make a pointer type.

Pointer_type*
Type::make_pointer_type(Type* to_type)
{
  typedef std::tr1::unordered_map<Type*, Pointer_type*> Hashtable;
  static Hashtable pointer_types;
  Hashtable::const_iterator p = pointer_types.find(to_type);
  if (p != pointer_types.end())
    return p->second;
  Pointer_type* ret = new Pointer_type(to_type);
  pointer_types[to_type] = ret;
  return ret;
}

// The nil type.  We use a special type for nil because it is not the
// same as any other type.  In C term nil has type void*, but there is
// no such type in Go.

class Nil_type : public Type
{
 public:
  Nil_type()
    : Type(TYPE_NIL)
  { }

 protected:
  tree
  do_get_tree(Gogo*)
  { return ptr_type_node; }

  tree
  do_init_tree(Gogo*, bool is_clear)
  { return is_clear ? NULL : null_pointer_node; }

  void
  do_type_descriptor_decl(Gogo*, Named_type*, tree*)
  { gcc_unreachable(); }

  void
  do_reflection(Gogo*, std::string*) const
  { gcc_unreachable(); }

  void
  do_mangled_name(Gogo*, std::string* ret) const
  { ret->push_back('n'); }
};

// Make the nil type.

Type*
Type::make_nil_type()
{
  static Nil_type singleton_nil_type;
  return &singleton_nil_type;
}

// The type of a function call which returns multiple values.  This is
// really a struct, but we don't want to confuse a function call which
// returns a struct with a function call which returns multiple
// values.

class Call_multiple_result_type : public Type
{
 public:
  Call_multiple_result_type(Call_expression* call)
    : Type(TYPE_CALL_MULTIPLE_RESULT),
      call_(call)
  { }

 protected:
  tree
  do_get_tree(Gogo*);

  tree
  do_init_tree(Gogo*, bool)
  { gcc_unreachable(); }

  void
  do_type_descriptor_decl(Gogo*, Named_type*, tree*)
  { gcc_unreachable(); }

  void
  do_reflection(Gogo*, std::string*) const
  { gcc_unreachable(); }

  void
  do_mangled_name(Gogo*, std::string*) const
  { gcc_unreachable(); }

 private:
  // The expression being called.
  Call_expression* call_;
};

// Return the tree for a call result.

tree
Call_multiple_result_type::do_get_tree(Gogo* gogo)
{
  Function_type* fntype = this->call_->get_function_type(false);
  gcc_assert(fntype != NULL);
  const Typed_identifier_list* results = fntype->results();
  gcc_assert(results != NULL && results->size() > 1);

  Struct_field_list* sfl = new Struct_field_list;
  for (Typed_identifier_list::const_iterator p = results->begin();
       p != results->end();
       ++p)
    {
      const std::string name = ((p->name().empty()
				 || p->name() == Import::import_marker)
				? "UNNAMED"
				: p->name());
      sfl->push_back(Struct_field(Typed_identifier(name, p->type(),
						   this->call_->location())));
    }
  return Type::make_struct_type(sfl, this->call_->location())->get_tree(gogo);
}

// Make a call result type.

Type*
Type::make_call_multiple_result_type(Call_expression* call)
{
  return new Call_multiple_result_type(call);
}

// Class Struct_field.

// Get the name of a field.

const std::string&
Struct_field::field_name() const
{
  const std::string& name(this->typed_identifier_.name());
  if (!name.empty())
    return name;
  else
    {
      // This is called during parsing, before anything is lowered, so
      // we have to be pretty careful to avoid dereferencing an
      // unknown type name.
      Type* t = this->typed_identifier_.type();
      Type* dt = t;
      if (t->classification() == Type::TYPE_POINTER)
	{
	  // Very ugly.
	  Pointer_type* ptype = static_cast<Pointer_type*>(t);
	  dt = ptype->points_to();
	}
      if (dt->forward_declaration_type() != NULL)
	return dt->forward_declaration_type()->name();
      else if (dt->named_type() != NULL)
	return dt->named_type()->name();
      else if (t->is_error_type() || dt->is_error_type())
	{
	  static const std::string error_string = "*error*";
	  return error_string;
	}
      else
	{
	  // Avoid crashing in the erroneous case where T is named but
	  // DT is not.
	  gcc_assert(t != dt);
	  if (t->forward_declaration_type() != NULL)
	    return t->forward_declaration_type()->name();
	  else if (t->named_type() != NULL)
	    return t->named_type()->name();
	  else
	    gcc_unreachable();
	}
    }
}

// Class Struct_type.

// Traversal.

int
Struct_type::do_traverse(Traverse* traverse)
{
  Struct_field_list* fields = this->fields_;
  if (fields != NULL)
    {
      for (Struct_field_list::iterator p = fields->begin();
	   p != fields->end();
	   ++p)
	{
	  if (Type::traverse(p->type(), traverse) == TRAVERSE_EXIT)
	    return TRAVERSE_EXIT;
	}
    }
  return TRAVERSE_CONTINUE;
}

// Verify that the struct type is complete and valid.

bool
Struct_type::do_verify()
{
  Struct_field_list* fields = this->fields_;
  if (fields == NULL)
    return true;
  for (Struct_field_list::iterator p = fields->begin();
       p != fields->end();
       ++p)
    {
      Type* t = p->type();
      if (t->is_undefined())
	{
	  error_at(p->location(), "struct field type is incomplete");
	  p->set_type(Type::make_error_type());
	  return false;
	}
      else if (p->is_anonymous())
	{
	  if (t->named_type() != NULL && t->points_to() != NULL)
	    {
	      error_at(p->location(), "embedded type may not be a pointer");
	      p->set_type(Type::make_error_type());
	      return false;
	    }
	}
    }
  return true;
}

// Whether this contains a reference counted component.

bool
Struct_type::do_has_refcounted_component() const
{
  const Struct_field_list* fields = this->fields();
  if (fields == NULL)
    return false;
  for (Struct_field_list::const_iterator p = fields->begin();
       p != fields->end();
       ++p)
    {
      if (p->type()->has_refcounted_component())
	return true;
    }
  return false;
}

// Add entries to the refcount queue for this type.

void
Struct_type::do_add_refcount_queue_entries(Refcounts* refcounts,
					   Refcount_entry* entry)
{
  const Struct_field_list* fields = this->fields();
  gcc_assert(fields != NULL);
  for (Struct_field_list::const_iterator p = fields->begin();
       p != fields->end();
       ++p)
    {
      if (p->type()->has_refcounted_component())
	p->type()->add_refcount_queue_entries(refcounts, entry);
    }
}

// Whether this type is compatible with T.

bool
Struct_type::is_compatible(const Struct_type* t,
			   Type_compatible compatible) const
{
  const Struct_field_list* fields1 = this->fields();
  const Struct_field_list* fields2 = t->fields();
  if (fields1 == NULL || fields2 == NULL)
    return fields1 == fields2;
  Struct_field_list::const_iterator pf2 = fields2->begin();
  for (Struct_field_list::const_iterator pf1 = fields1->begin();
       pf1 != fields1->end();
       ++pf1, ++pf2)
    {
      if (pf2 == fields2->end())
	return false;
      if (compatible == COMPATIBLE_IDENTICAL
	  && pf1->field_name() != pf2->field_name())
	return false;
      if (pf1->is_anonymous() != pf2->is_anonymous()
	  || !Type::are_compatible_for(pf1->type(), pf2->type(), compatible,
				       NULL))
	return false;
      if (compatible == COMPATIBLE_IDENTICAL)
	{
	  if (!pf1->has_tag())
	    {
	      if (pf2->has_tag())
		return false;
	    }
	  else
	    {
	      if (!pf2->has_tag())
		return false;
	      if (pf1->tag() != pf2->tag())
		return false;
	    }
	}
    }
  if (pf2 != fields2->end())
    return false;
  return true;
}

// Whether this struct type has any hidden fields.

bool
Struct_type::struct_has_hidden_fields(const Named_type* within,
				      std::string* reason) const
{
  const Struct_field_list* fields = this->fields();
  if (fields == NULL)
    return false;
  const Package* within_package = (within == NULL
				   ? NULL
				   : within->named_object()->package());
  bool is_in_current_package = (within_package == NULL
				|| within_package->being_compiled());
  for (Struct_field_list::const_iterator pf = fields->begin();
       pf != fields->end();
       ++pf)
    {
      if (!is_in_current_package
	  && !pf->is_anonymous()
	  && Gogo::is_hidden_name(pf->field_name()))
	{
	  if (reason != NULL)
	    {
	      std::string within_name = (within_package->name()
					 + '.'
					 + within->named_object()->name());
	      std::string name = Gogo::unpack_hidden_name(pf->field_name());
	      size_t bufsize = 200 + within_name.length() + name.length();
	      char* buf = new char[bufsize];
	      snprintf(buf, bufsize,
		       _("implicit assignment of %s%s%s hidden field %s%s%s"),
		       open_quote, within_name.c_str(), close_quote,
		       open_quote, name.c_str(), close_quote);
	      reason->assign(buf);
	      delete[] buf;
	    }
	  return true;
	}

      if (pf->type()->has_hidden_fields(within, reason))
	return true;
    }

  return false;
}

// Hash code.

size_t
Struct_type::do_hash_for_method() const
{
  size_t ret = 0;
  if (this->fields() != NULL)
    {
      for (Struct_field_list::const_iterator pf = this->fields()->begin();
	   pf != this->fields()->end();
	   ++pf)
	ret = (ret << 1) + pf->type()->hash_for_method();
    }
  return ret <<= 2;
}

// Find the local field NAME.

const Struct_field*
Struct_type::find_local_field(const std::string& name,
			      unsigned int *pindex) const
{
  const Struct_field_list* fields = this->fields_;
  if (fields == NULL)
    return NULL;
  unsigned int i = 0;
  for (Struct_field_list::const_iterator pf = fields->begin();
       pf != fields->end();
       ++pf, ++i)
    {
      if (pf->field_name() == name)
	{
	  if (pindex != NULL)
	    *pindex = i;
	  return &*pf;
	}
    }
  return NULL;
}

// Return an expression for field NAME in STRUCT_EXPR, or NULL.

Field_reference_expression*
Struct_type::field_reference(Expression* struct_expr, const std::string& name,
			     source_location location) const
{
  unsigned int depth;
  return this->field_reference_depth(struct_expr, name, location, &depth);
}

// Return an expression for a field, along with the depth at which it
// was found.

Field_reference_expression*
Struct_type::field_reference_depth(Expression* struct_expr,
				   const std::string& name,
				   source_location location,
				   unsigned int* depth) const
{
  const Struct_field_list* fields = this->fields_;
  if (fields == NULL)
    return NULL;

  // Look for a field with this name.
  unsigned int i = 0;
  for (Struct_field_list::const_iterator pf = fields->begin();
       pf != fields->end();
       ++pf, ++i)
    {
      if (pf->field_name() == name)
	{
	  *depth = 0;
	  return Expression::make_field_reference(struct_expr, i, location);
	}
    }

  // Look for an anonymous field which contains a field with this
  // name.
  Field_reference_expression* ret = NULL;
  const Struct_field* parent = NULL;
  unsigned int retdepth = 0;
  i = 0;
  for (Struct_field_list::const_iterator pf = fields->begin();
       pf != fields->end();
       ++pf, ++i)
    {
      if (!pf->is_anonymous())
	continue;

      Struct_type* st = pf->type()->deref()->struct_type();
      if (st == NULL)
	continue;

      // Look for a reference using a NULL struct expression.  If we
      // find one, fill in the struct expression with a reference to
      // this field.
      unsigned int subdepth;
      Field_reference_expression* sub = st->field_reference_depth(NULL, name,
								  location,
								  &subdepth);
      if (sub == NULL)
	continue;

      if (ret == NULL || subdepth < retdepth)
	{
	  if (ret != NULL)
	    delete ret;
	  ret = sub;
	  parent = &*pf;
	  retdepth = subdepth;
	  Expression* here = Expression::make_field_reference(struct_expr, i,
							      location);
	  while (sub->expr() != NULL)
	    {
	      sub = sub->expr()->field_reference_expression();
	      gcc_assert(sub != NULL);
	    }
	  sub->set_struct_expression(here);
	}
      else if (subdepth > retdepth)
	delete sub;
      else
	{
	  error_at(location, "field %<%s%> is ambiguous via %<%s%> and %<%s%>",
		   name.c_str(), parent->field_name().c_str(),
		   pf->field_name().c_str());
	  delete sub;
	}
    }

  *depth = retdepth + 1;
  return ret;
}

// Return the total number of fields, including embedded fields.

unsigned int
Struct_type::total_field_count() const
{
  if (this->fields_ == NULL)
    return 0;
  unsigned int ret = 0;
  for (Struct_field_list::const_iterator pf = this->fields_->begin();
       pf != this->fields_->end();
       ++pf)
    {
      if (!pf->is_anonymous() || pf->type()->deref()->struct_type() == NULL)
	++ret;
      else
	ret += pf->type()->struct_type()->total_field_count();
    }
  return ret;
}

// Get the tree for a struct type.

tree
Struct_type::do_get_tree(Gogo* gogo)
{
  tree type = make_node(RECORD_TYPE);
  this->set_incomplete_type_tree(type);

  tree field_trees = NULL_TREE;
  tree* pp = &field_trees;
  gcc_assert(this->fields_ != NULL);
  for (Struct_field_list::const_iterator p = this->fields_->begin();
       p != this->fields_->end();
       ++p)
    {
      std::string name = Gogo::unpack_hidden_name(p->field_name());
      tree name_tree = get_identifier_with_length(name.data(), name.length());
      tree field_type_tree = p->type()->get_tree(gogo);
      if (field_type_tree == error_mark_node)
	return error_mark_node;
      tree field = build_decl(p->location(), FIELD_DECL, name_tree,
			      field_type_tree);
      DECL_CONTEXT(field) = type;
      *pp = field;
      pp = &TREE_CHAIN(field);
    }

  TYPE_FIELDS(type) = field_trees;

  layout_type(type);

  return type;
}

// Initialize struct fields.

tree
Struct_type::do_init_tree(Gogo* gogo, bool is_clear)
{
  tree type_tree = this->get_tree(gogo);
  if (type_tree == error_mark_node)
    return error_mark_node;

  if (this->fields_ == NULL || this->fields_->empty())
    {
      if (is_clear)
	return NULL;
      else
	{
	  tree ret = build_constructor(type_tree,
				       VEC_alloc(constructor_elt, gc, 0));
	  TREE_CONSTANT(ret) = 1;
	  return ret;
	}
    }

  bool is_constant = true;
  bool any_fields_set = false;
  VEC(constructor_elt,gc)* init = VEC_alloc(constructor_elt, gc,
					    this->fields_->size());
  Struct_field_list::const_iterator p = this->fields_->begin();
  for (tree field = TYPE_FIELDS(type_tree);
       field != NULL_TREE;
       field = TREE_CHAIN(field), ++p)
    {
      gcc_assert(p != this->fields_->end());
      tree value = p->type()->get_init_tree(gogo, is_clear);
      if (value != NULL)
	{
	  constructor_elt* elt = VEC_quick_push(constructor_elt, init, NULL);
	  elt->index = field;
	  elt->value = value;
	  any_fields_set = true;
	  if (!TREE_CONSTANT(value))
	    is_constant = false;
	}
    }
  gcc_assert(p == this->fields_->end());

  if (!any_fields_set)
    {
      gcc_assert(is_clear);
      VEC_free(constructor_elt, gc, init);
      return NULL;
    }

  tree ret = build_constructor(type_tree, init);
  if (is_constant)
    TREE_CONSTANT(ret) = 1;
  return ret;
}

// Copy a struct into the reference count queue.

tree
Struct_type::do_set_refcount_queue_entry(Gogo* gogo, Refcounts* refcounts,
					 Refcount_entry* entry, tree val) const
{
  const Struct_field_list* fields = this->fields();
  gcc_assert(fields != NULL);
  tree ret = NULL_TREE;
  tree field = TYPE_FIELDS(TREE_TYPE(val));
  for (Struct_field_list::const_iterator p = fields->begin();
       p != fields->end();
       ++p, field = TREE_CHAIN(field))
    {
      gcc_assert(field != NULL_TREE);
      if (p->type()->has_refcounted_component())
	{
	  tree f = build3(COMPONENT_REF, TREE_TYPE(field), val, field,
			  NULL_TREE);
	  tree set = p->type()->set_refcount_queue_entry(gogo, refcounts,
							 entry, f);
	  if (ret == NULL_TREE)
	    ret = set;
	  else
	    ret = build2(COMPOUND_EXPR, void_type_node, ret, set);
	}
    }
  gcc_assert(field == NULL_TREE);
  gcc_assert(ret != NULL_TREE);
  return ret;
}

// Type descriptor.

void
Struct_type::do_type_descriptor_decl(Gogo* gogo, Named_type* name, tree* pdecl)
{
  gogo->struct_type_descriptor_decl(this, name, pdecl);
}

// Reflection string.

void
Struct_type::do_reflection(Gogo* gogo, std::string* ret) const
{
  ret->append("struct { ");

  for (Struct_field_list::const_iterator p = this->fields_->begin();
       p != this->fields_->end();
       ++p)
    {
      if (p != this->fields_->begin())
	ret->append("; ");
      if (p->is_anonymous())
	ret->push_back('?');
      else
	ret->append(Gogo::unpack_hidden_name(p->field_name()));
      ret->push_back(' ');
      this->append_reflection(p->type(), gogo, ret);

      if (p->has_tag())
	{
	  const std::string& tag(p->tag());
	  ret->append(" \"");
	  for (std::string::const_iterator p = tag.begin();
	       p != tag.end();
	       ++p)
	    {
	      if (*p == '\0')
		ret->append("\\x00");
	      else if (*p == '\n')
		ret->append("\\n");
	      else if (*p == '\t')
		ret->append("\\t");
	      else if (*p == '"')
		ret->append("\\\"");
	      else if (*p == '\\')
		ret->append("\\\\");
	      else
		ret->push_back(*p);
	    }
	  ret->push_back('"');
	}
    }

  ret->append(" }");
}

// Mangled name.

void
Struct_type::do_mangled_name(Gogo* gogo, std::string* ret) const
{
  ret->push_back('S');

  const Struct_field_list* fields = this->fields_;
  if (fields != NULL)
    {
      for (Struct_field_list::const_iterator p = fields->begin();
	   p != fields->end();
	   ++p)
	{
	  if (p->is_anonymous())
	    ret->append("0_");
	  else
	    {
	      std::string n = Gogo::unpack_hidden_name(p->field_name());
	      char buf[20];
	      snprintf(buf, sizeof buf, "%u_",
		       static_cast<unsigned int>(n.length()));
	      ret->append(buf);
	      ret->append(n);
	    }
	  this->append_mangled_name(p->type(), gogo, ret);
	  if (p->has_tag())
	    {
	      const std::string& tag(p->tag());
	      std::string out;
	      for (std::string::const_iterator p = tag.begin();
		   p != tag.end();
		   ++p)
		{
		  if (ISALNUM(*p) || *p == '_')
		    out.push_back(*p);
		  else
		    {
		      char buf[20];
		      snprintf(buf, sizeof buf, ".%x.",
			       static_cast<unsigned int>(*p));
		      out.append(buf);
		    }
		}
	      char buf[20];
	      snprintf(buf, sizeof buf, "T%u_",
		       static_cast<unsigned int>(out.length()));
	      ret->append(buf);
	      ret->append(out);
	    }
	}
    }

  ret->push_back('e');
}

// Export.

void
Struct_type::do_export(Export* exp) const
{
  exp->write_c_string("struct { ");
  const Struct_field_list* fields = this->fields_;
  gcc_assert(fields != NULL);
  for (Struct_field_list::const_iterator p = fields->begin();
       p != fields->end();
       ++p)
    {
      if (p->is_anonymous())
	exp->write_string("? ");
      else
	{
	  exp->write_string(p->field_name());
	  exp->write_c_string(" ");
	}
      exp->write_type(p->type());

      if (p->has_tag())
	{
	  exp->write_c_string(" ");
	  Expression* expr = Expression::make_string(p->tag(),
						     BUILTINS_LOCATION);
	  expr->export_expression(exp);
	  delete expr;
	}

      exp->write_c_string("; ");
    }
  exp->write_c_string("}");
}

// Import.

Struct_type*
Struct_type::do_import(Import* imp)
{
  imp->require_c_string("struct { ");
  Struct_field_list* fields = new Struct_field_list;
  if (imp->peek_char() != '}')
    {
      while (true)
	{
	  std::string name;
	  if (imp->match_c_string("? "))
	    imp->advance(2);
	  else
	    {
	      name = imp->read_identifier();
	      imp->require_c_string(" ");
	    }
	  Type* ftype = imp->read_type();

	  Struct_field sf(Typed_identifier(name, ftype, imp->location()));

	  if (imp->peek_char() == ' ')
	    {
	      imp->advance(1);
	      Expression* expr = Expression::import_expression(imp);
	      String_expression* sexpr = expr->string_expression();
	      gcc_assert(sexpr != NULL);
	      sf.set_tag(sexpr->val());
	      delete sexpr;
	    }

	  imp->require_c_string("; ");
	  fields->push_back(sf);
	  if (imp->peek_char() == '}')
	    break;
	}
    }
  imp->require_c_string("}");

  return Type::make_struct_type(fields, imp->location());
}

// Make a struct type.

Struct_type*
Type::make_struct_type(Struct_field_list* fields,
		       source_location location)
{
  return new Struct_type(fields, location);
}

// Class Array_type.

Array_type::Array_trees Array_type::array_trees;

// Whether two array types are compatible.

bool
Array_type::is_compatible(const Array_type* t,
			  Type_compatible compatible) const
{
  if (!Type::are_compatible_for(this->element_type(), t->element_type(),
				compatible, NULL))
    return false;

  Expression* l1 = this->length();
  Expression* l2 = t->length();

  // Open arrays of the same element type are identical.
  if (l1 == NULL && l2 == NULL)
    return true;

  // Fixed arrays of the same element type are identical if they have
  // the same length.
  if (l1 != NULL && l2 != NULL)
    {
      if (l1 == l2)
	return true;

      // Try to determine the lengths.  If we can't, assume the arrays
      // are not identical.
      bool ret = false;
      mpz_t v1;
      mpz_init(v1);
      Type* type1;
      mpz_t v2;
      mpz_init(v2);
      Type* type2;
      if (l1->integer_constant_value(true, v1, &type1)
	  && l2->integer_constant_value(true, v2, &type2))
	ret = mpz_cmp(v1, v2) == 0;
      mpz_clear(v1);
      mpz_clear(v2);
      return ret;
    }

  // Otherwise the arrays are not identical.
  return false;
}

// Traversal.

int
Array_type::do_traverse(Traverse* traverse)
{
  if (Type::traverse(this->element_type_, traverse) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  if (this->length_ != NULL
      && Expression::traverse(&this->length_, traverse) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  return TRAVERSE_CONTINUE;
}

// Array type hash code.

size_t
Array_type::do_hash_for_method() const
{
  // There is no very convenient way to get a hash code for the
  // length.
  return this->element_type_->hash_for_method();
}

// See if the expression passed to make is suitable.  The first
// argument is required, and gives the length.  An optional second
// argument is permitted for the capacity.

bool
Array_type::do_check_make_expression(Expression_list* args,
				     source_location location)
{
  gcc_assert(this->length_ == NULL);
  if (args == NULL || args->empty())
    {
      error_at(location, "length required when allocating a slice");
      return false;
    }
  else if (args->size() > 2)
    {
      error_at(location, "too many expressions passed to make");
      return false;
    }
  else
    {
      Type* type = (*args->begin())->type();
      if (type == NULL || type->integer_type() == NULL)
	{
	  error_at(location, "bad type for length when making slice");
	  return false;
	}
      if (args->size() > 1)
	{
	  type = args->back()->type();
	  if (type == NULL || type->integer_type() == NULL)
	    {
	      error_at(location, "bad type for capacity when making slice");
	      return false;
	    }
	}
      return true;
    }
}

// Get a tree for the length of a fixed array.  The length may be
// computed using a function call, so we must only evaluate it once.

tree
Array_type::get_length_tree(Gogo* gogo)
{
  gcc_assert(this->length_ != NULL);
  if (this->length_tree_ == NULL_TREE)
    {
      mpz_t val;
      mpz_init(val);
      Type* t;
      if (this->length_->integer_constant_value(true, val, &t))
	{
	  if (t == NULL)
	    t = Type::lookup_integer_type("int");
	  else if (t->is_abstract())
	    t = t->make_non_abstract_type();
	  tree tt = t->get_tree(gogo);
	  this->length_tree_ = Expression::integer_constant_tree(val, tt);
	  mpz_clear(val);
	}
      else
	{
	  mpz_clear(val);

	  // Make up a translation context for the array length
	  // expression.  FIXME: This won't work in general.
	  Translate_context context(gogo, NULL, NULL, NULL_TREE);
	  this->length_tree_ = save_expr(this->length_->get_tree(&context));
	}
    }
  return this->length_tree_;
}

// Get a tree for the type of this array.  A fixed array is simply
// represented as ARRAY_TYPE with the appropriate index--i.e., it is
// just like an array in C.  An open array is a struct with three
// fields: a data pointer, the length, and the capacity.

tree
Array_type::do_get_tree(Gogo* gogo)
{
  tree element_type_tree = this->element_type_->get_tree(gogo);
  if (element_type_tree == error_mark_node)
    return error_mark_node;

  if (this->length_ != NULL)
    {
      tree length_tree = this->get_length_tree(gogo);
      if (length_tree == error_mark_node)
	return error_mark_node;
      length_tree = fold_convert(sizetype, length_tree);
      // build_index_type takes the maximum index, which is one less
      // than the length.
      tree index_type = build_index_type(fold_build2(MINUS_EXPR, sizetype,
						     length_tree,
						     size_one_node));
      tree ret = build_array_type(element_type_tree, index_type);
      // If the element type requires reference counting, then we need
      // this to be stored in memory.
      if (this->element_type_->has_refcounted_component())
	TREE_ADDRESSABLE(ret) = 1;
      return ret;
    }

  // Two different open arrays of the same element type are really the
  // same type.  In order to make that valid at the tree level, we
  // make sure to return the same struct.

  std::pair<Type*, tree> val(this->element_type_, NULL);
  std::pair<Array_trees::iterator, bool> ins =
    Array_type::array_trees.insert(val);
  if (!ins.second)
    {
      // We've already created a tree type for an array with this
      // element type.
      gcc_assert(ins.first->second != NULL_TREE);
      return ins.first->second;
    }

  // An open array is a struct.
  tree struct_type = gogo->slice_type_tree(element_type_tree);
  ins.first->second = struct_type;
  return struct_type;
}

// Return an initializer for an array type.

tree
Array_type::do_init_tree(Gogo* gogo, bool is_clear)
{
  tree type_tree = this->get_tree(gogo);

  if (this->length_ == NULL)
    {
      // Open array.

      if (is_clear)
	return NULL;

      gcc_assert(TREE_CODE(type_tree) == RECORD_TYPE);

      VEC(constructor_elt,gc)* init = VEC_alloc(constructor_elt, gc, 3);

      for (tree field = TYPE_FIELDS(type_tree);
	   field != NULL_TREE;
	   field = TREE_CHAIN(field))
	{
	  constructor_elt* elt = VEC_quick_push(constructor_elt, init,
						NULL);
	  elt->index = field;
	  elt->value = fold_convert(TREE_TYPE(field), size_zero_node);
	}

      tree ret = build_constructor(type_tree, init);
      TREE_CONSTANT(ret) = 1;
      return ret;
    }
  else
    {
      // Fixed array.

      tree value = this->element_type_->get_init_tree(gogo, is_clear);
      if (value == NULL)
	return NULL;

      tree length_tree = this->get_length_tree(gogo);
      length_tree = fold_convert(sizetype, length_tree);
      tree range = build2(RANGE_EXPR, sizetype, size_zero_node,
			  fold_build2(MINUS_EXPR, sizetype,
				      length_tree, size_one_node));
      tree ret = build_constructor_single(type_tree, range, value);
      if (TREE_CONSTANT(value))
	TREE_CONSTANT(ret) = 1;
      return ret;
    }
}

// Handle the builtin make function for a slice.

tree
Array_type::do_make_expression_tree(Translate_context* context,
				    Expression_list* args,
				    source_location location)
{
  gcc_assert(this->length_ == NULL);

  Gogo* gogo = context->gogo();
  tree type_tree = this->get_tree(gogo);
  if (type_tree == error_mark_node)
    return error_mark_node;

  tree element_type_tree = this->element_type_->get_tree(gogo);
  if (element_type_tree == error_mark_node)
    return error_mark_node;
  tree element_size_tree = TYPE_SIZE_UNIT(element_type_tree);

  tree value = this->element_type_->get_init_tree(gogo, true);

  // The first argument is the number of elements, the optional second
  // argument is the capacity.
  gcc_assert(args != NULL && args->size() >= 1 && args->size() <= 2);

  tree length_tree = args->front()->get_tree(context);
  length_tree = save_expr(length_tree);
  length_tree = fold_convert(sizetype, length_tree);
  tree capacity_tree;
  if (args->size() == 1)
    capacity_tree = length_tree;
  else
    {
      capacity_tree = args->back()->get_tree(context);
      capacity_tree = save_expr(capacity_tree);
      capacity_tree = fold_convert(sizetype, capacity_tree);
    }

  tree size_tree = fold_build2(MULT_EXPR, sizetype,
			       element_size_tree, capacity_tree);

  tree space = context->gogo()->allocate_memory(size_tree, location);

  if (value != NULL_TREE)
    space = save_expr(space);

  tree field = TYPE_FIELDS(type_tree);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__values") == 0);
  space = fold_convert(TREE_TYPE(field), space);

  tree constructor = gogo->slice_constructor(type_tree, space, length_tree,
					     capacity_tree);

  if (value == NULL_TREE)
    {
      // The array contents are zero initialized.
      return constructor;
    }

  // The elements must be initialized.

  tree max = fold_build2(MINUS_EXPR, sizetype, capacity_tree, size_one_node);

  tree array_type = build_array_type(element_type_tree,
				     build_index_type(max));

  tree value_pointer = fold_convert(build_pointer_type(array_type), space);

  tree range = build2(RANGE_EXPR, sizetype, size_zero_node, max);
  tree space_init = build_constructor_single(array_type, range, value);

  return build2(COMPOUND_EXPR, TREE_TYPE(space),
		build2(MODIFY_EXPR, void_type_node,
		       build_fold_indirect_ref(value_pointer),
		       space_init),
		constructor);
}

// Copy an array into the reference count queue.

tree
Array_type::do_set_refcount_queue_entry(Gogo* gogo, Refcounts* refcounts,
					Refcount_entry* entry, tree val) const
{
  tree ret = refcounts->set_entry_tree(gogo, *entry,
				       this->value_pointer_tree(gogo, val));
  entry->increment();
  return ret;
}

// Return a tree for a pointer to the values in ARRAY.

tree
Array_type::value_pointer_tree(Gogo*, tree array) const
{
  tree ret;
  if (this->length() != NULL)
    {
      // Fixed array.
      ret = fold_convert(build_pointer_type(TREE_TYPE(TREE_TYPE(array))),
			 build_fold_addr_expr(array));
    }
  else
    {
      // Open array.
      tree field = TYPE_FIELDS(TREE_TYPE(array));
      gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)),
			"__values") == 0);
      ret = fold_build3(COMPONENT_REF, TREE_TYPE(field), array, field,
			NULL_TREE);
    }
  if (TREE_CONSTANT(array))
    TREE_CONSTANT(ret) = 1;
  return ret;
}

// Return a tree for the length of the array ARRAY which has this
// type.

tree
Array_type::length_tree(Gogo* gogo, tree array)
{
  if (this->length_ != NULL)
    {
      if (TREE_CODE(array) == SAVE_EXPR)
	return fold_convert(integer_type_node, this->get_length_tree(gogo));
      else
	return omit_one_operand(integer_type_node,
				this->get_length_tree(gogo), array);
    }

  // This is an open array.  We need to read the length field.

  tree type = TREE_TYPE(array);
  gcc_assert(TREE_CODE(type) == RECORD_TYPE);

  tree field = TREE_CHAIN(TYPE_FIELDS(type));
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__count") == 0);

  tree ret = build3(COMPONENT_REF, TREE_TYPE(field), array, field, NULL_TREE);
  if (TREE_CONSTANT(array))
    TREE_CONSTANT(ret) = 1;
  return ret;
}

// Return a tree for the capacity of the array ARRAY which has this
// type.

tree
Array_type::capacity_tree(Gogo* gogo, tree array)
{
  if (this->length_ != NULL)
    return omit_one_operand(sizetype, this->get_length_tree(gogo), array);

  // This is an open array.  We need to read the capacity field.

  tree type = TREE_TYPE(array);
  gcc_assert(TREE_CODE(type) == RECORD_TYPE);

  tree field = TREE_CHAIN(TREE_CHAIN(TYPE_FIELDS(type)));
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__capacity") == 0);

  return build3(COMPONENT_REF, TREE_TYPE(field), array, field, NULL_TREE);
}

// Export.

void
Array_type::do_export(Export* exp) const
{
  exp->write_c_string("[");
  if (this->length_ != NULL)
    this->length_->export_expression(exp);
  exp->write_c_string("] ");
  exp->write_type(this->element_type_);
}

// Import.

Array_type*
Array_type::do_import(Import* imp)
{
  imp->require_c_string("[");
  Expression* length;
  if (imp->peek_char() == ']')
    length = NULL;
  else
    length = Expression::import_expression(imp);
  imp->require_c_string("] ");
  Type* element_type = imp->read_type();
  return Type::make_array_type(element_type, length);
}

// Type descriptor.

void
Array_type::do_type_descriptor_decl(Gogo* gogo, Named_type* name, tree* pdecl)
{
  if (this->length_ != NULL)
    gogo->array_type_descriptor_decl(this, name, pdecl);
  else
    gogo->slice_type_descriptor_decl(this, name, pdecl);
}

// Reflection string.

void
Array_type::do_reflection(Gogo* gogo, std::string* ret) const
{
  ret->push_back('[');
  if (this->length_ != NULL)
    {
      mpz_t val;
      mpz_init(val);
      Type* type;
      if (!this->length_->integer_constant_value(true, val, &type))
	error_at(this->length_->location(),
		 "array length must be integer constant expression");
      else if (mpz_cmp_si(val, 0) < 0)
	error_at(this->length_->location(), "array length is negative");
      else if (mpz_cmp_ui(val, mpz_get_ui(val)) != 0)
	error_at(this->length_->location(), "array length is too large");
      else
	{
	  char buf[50];
	  snprintf(buf, sizeof buf, "%lu", mpz_get_ui(val));
	  ret->append(buf);
	}
      mpz_clear(val);
    }
  ret->push_back(']');

  this->append_reflection(this->element_type_, gogo, ret);
}

// Mangled name.

void
Array_type::do_mangled_name(Gogo* gogo, std::string* ret) const
{
  ret->push_back('A');
  this->append_mangled_name(this->element_type_, gogo, ret);
  if (this->length_ != NULL)
    {
      mpz_t val;
      mpz_init(val);
      Type* type;
      if (!this->length_->integer_constant_value(true, val, &type))
	error_at(this->length_->location(),
		 "array length must be integer constant expression");
      else if (mpz_cmp_si(val, 0) < 0)
	error_at(this->length_->location(), "array length is negative");
      else if (mpz_cmp_ui(val, mpz_get_ui(val)) != 0)
	error_at(this->length_->location(), "array size is too large");
      else
	{
	  char buf[50];
	  snprintf(buf, sizeof buf, "%lu", mpz_get_ui(val));
	  ret->append(buf);
	}
      mpz_clear(val);
    }
  ret->push_back('e');
}

// Make an array type.

Array_type*
Type::make_array_type(Type* element_type, Expression* length)
{
  return new Array_type(element_type, length);
}

// Class Map_type.

// Traversal.

int
Map_type::do_traverse(Traverse* traverse)
{
  if (Type::traverse(this->key_type_, traverse) == TRAVERSE_EXIT
      || Type::traverse(this->val_type_, traverse) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  return TRAVERSE_CONTINUE;
}

// Check that the map type is OK.

bool
Map_type::do_verify()
{
  if (this->key_type_->struct_type() != NULL
      || this->key_type_->array_type() != NULL
      || this->key_type_->map_type() != NULL)
    {
      error_at(this->location_, "invalid map key type");
      return false;
    }
  return true;
}

// Whether two map types are compatible.

bool
Map_type::is_compatible(const Map_type* t, Type_compatible compatible) const
{
  return (Type::are_compatible_for(this->key_type(), t->key_type(),
				   compatible, NULL)
	  && Type::are_compatible_for(this->val_type(), t->val_type(),
				      compatible, NULL));
}

// Hash code.

size_t
Map_type::do_hash_for_method() const
{
  return (this->key_type_->hash_for_method()
	  + this->val_type_->hash_for_method());
}

// Check that a call to the builtin make function is valid.  For a map
// the optional argument is the number of spaces to preallocate for
// values.

bool
Map_type::do_check_make_expression(Expression_list* args,
				   source_location location)
{
  if (args != NULL && !args->empty())
    {
      Type* type = (*args->begin())->type();
      if (type == NULL || type->integer_type() == NULL)
	{
	  error_at(location, "bad type for map size");
	  return false;
	}
      else if (args->size() > 1)
	{
	  error_at(location, "too many arguments when making map");
	  return false;
	}
    }
  return true;
}

// Get a tree for a map type.  A map type is represented as a pointer
// to a struct.  The struct is __go_map in libgo/map.h.

tree
Map_type::do_get_tree(Gogo* gogo)
{
  static tree type_tree;
  if (type_tree == NULL_TREE)
    {
      tree struct_type = make_node(RECORD_TYPE);

      tree map_descriptor_type = gogo->map_descriptor_type();
      tree const_map_descriptor_type =
	build_qualified_type(map_descriptor_type, TYPE_QUAL_CONST);
      tree name = get_identifier("__descriptor");
      tree field = build_decl(BUILTINS_LOCATION, FIELD_DECL, name,
			      build_pointer_type(const_map_descriptor_type));
      DECL_CONTEXT(field) = struct_type;
      TYPE_FIELDS(struct_type) = field;
      tree last_field = field;

      name = get_identifier("__element_count");
      field = build_decl(BUILTINS_LOCATION, FIELD_DECL, name, sizetype);
      DECL_CONTEXT(field) = struct_type;
      TREE_CHAIN(last_field) = field;
      last_field = field;

      name = get_identifier("__bucket_count");
      field = build_decl(BUILTINS_LOCATION, FIELD_DECL, name, sizetype);
      DECL_CONTEXT(field) = struct_type;
      TREE_CHAIN(last_field) = field;
      last_field = field;

      name = get_identifier("__buckets");
      field = build_decl(BUILTINS_LOCATION, FIELD_DECL, name,
			 build_pointer_type(ptr_type_node));
      DECL_CONTEXT(field) = struct_type;
      TREE_CHAIN(last_field) = field;

      layout_type(struct_type);

      // Give the struct a name for better debugging info.
      name = get_identifier("__go_map");
      tree type_decl = build_decl(BUILTINS_LOCATION, TYPE_DECL, name,
				  struct_type);
      DECL_ARTIFICIAL(type_decl) = 1;
      TYPE_NAME(struct_type) = type_decl;
      go_preserve_from_gc(type_decl);
      rest_of_decl_compilation(type_decl, 1, 0);

      type_tree = build_pointer_type(struct_type);
      go_preserve_from_gc(type_tree);
    }

  return type_tree;
}

// Initialize a map.

tree
Map_type::do_init_tree(Gogo* gogo, bool is_clear)
{
  if (is_clear)
    return NULL;
  return fold_convert(this->get_tree(gogo), null_pointer_node);
}

// Return an expression for a newly allocated map.

tree
Map_type::do_make_expression_tree(Translate_context* context,
				  Expression_list* args,
				  source_location location)
{
  tree expr_tree;
  if (args == NULL || args->empty())
    expr_tree = size_zero_node;
  else
    {
      expr_tree = (*args->begin())->get_tree(context);
      if (expr_tree == error_mark_node)
	return error_mark_node;
    }

  tree map_type = this->get_tree(context->gogo());

  static tree new_map_fndecl;
  return Gogo::call_builtin(&new_map_fndecl,
			    location,
			    "__go_new_map",
			    2,
			    map_type,
			    TREE_TYPE(TYPE_FIELDS(TREE_TYPE(map_type))),
			    context->gogo()->map_descriptor(this),
			    sizetype,
			    expr_tree);
}

// Type descriptor.

void
Map_type::do_type_descriptor_decl(Gogo* gogo, Named_type* name, tree* pdecl)
{
  gogo->map_type_descriptor_decl(this, name, pdecl);
}

// Reflection string for a map.

void
Map_type::do_reflection(Gogo* gogo, std::string* ret) const
{
  ret->append("map[");
  this->append_reflection(this->key_type_, gogo, ret);
  ret->append("] ");
  this->append_reflection(this->val_type_, gogo, ret);
}

// Mangled name for a map.

void
Map_type::do_mangled_name(Gogo* gogo, std::string* ret) const
{
  ret->push_back('M');
  this->append_mangled_name(this->key_type_, gogo, ret);
  ret->append("__");
  this->append_mangled_name(this->val_type_, gogo, ret);
}

// Export a map type.

void
Map_type::do_export(Export* exp) const
{
  exp->write_c_string("map [");
  exp->write_type(this->key_type_);
  exp->write_c_string("] ");
  exp->write_type(this->val_type_);
}

// Import a map type.

Map_type*
Map_type::do_import(Import* imp)
{
  imp->require_c_string("map [");
  Type* key_type = imp->read_type();
  imp->require_c_string("] ");
  Type* val_type = imp->read_type();
  return Type::make_map_type(key_type, val_type, imp->location());
}

// Make a map type.

Map_type*
Type::make_map_type(Type* key_type, Type* val_type, source_location location)
{
  return new Map_type(key_type, val_type, location);
}

// Class Channel_type.

// Hash code.

size_t
Channel_type::do_hash_for_method() const
{
  size_t ret = 0;
  if (this->may_send_)
    ret += 1;
  if (this->may_receive_)
    ret += 2;
  if (this->element_type_ != NULL)
    ret += this->element_type_->hash_for_method() << 2;
  return ret << 3;
}

// Whether this type is the same as T.

bool
Channel_type::is_compatible(const Channel_type* t,
			    Type_compatible compatible) const
{
  if (!Type::are_compatible_for(this->element_type(), t->element_type(),
				compatible, NULL))
    return false;
  return (this->may_send_ == t->may_send_
	  && this->may_receive_ == t->may_receive_);
}

// Check whether the parameters for a call to the builtin function
// make are OK for a channel.  A channel can take an optional single
// parameter which is the buffer size.

bool
Channel_type::do_check_make_expression(Expression_list* args,
				      source_location location)
{
  if (args != NULL && !args->empty())
    {
      Type* type = args->front()->type();
      if (type == NULL || type->integer_type() == NULL)
	{
	  error_at(location, "bad type for channel buffer size");
	  return false;
	}
      else if (args->size() > 1)
	{
	  error_at(location, "too many arguments when making channel");
	  return false;
	}
    }
  return true;
}

// Return the tree for a channel type.  A channel is a pointer to a
// __go_channel struct.  The __go_channel struct is defined in
// libgo/runtime/channel.h.

tree
Channel_type::do_get_tree(Gogo*)
{
  static tree type_tree;
  if (type_tree == NULL_TREE)
    {
      tree ret = make_node(RECORD_TYPE);
      TYPE_NAME(ret) = get_identifier("__go_channel");
      TYPE_STUB_DECL(ret) = build_decl(BUILTINS_LOCATION, TYPE_DECL, NULL_TREE,
				       ret);
      type_tree = build_pointer_type(ret);
      go_preserve_from_gc(type_tree);
    }
  return type_tree;
}

// Initialize a channel variable.

tree
Channel_type::do_init_tree(Gogo* gogo, bool is_clear)
{
  if (is_clear)
    return NULL;
  return fold_convert(this->get_tree(gogo), null_pointer_node);
}

// Handle the builtin function make for a channel.

tree
Channel_type::do_make_expression_tree(Translate_context* context,
				      Expression_list* args,
				      source_location location)
{
  Gogo* gogo = context->gogo();
  tree channel_type = this->get_tree(gogo);

  tree element_tree = this->element_type_->get_tree(gogo);
  tree element_size_tree = size_in_bytes(element_tree);

  tree expr_tree;
  if (args != NULL && !args->empty())
    expr_tree = fold_convert(sizetype, (*args->begin())->get_tree(context));
  else
    expr_tree = size_zero_node;

  static tree new_channel_fndecl;
  return Gogo::call_builtin(&new_channel_fndecl,
			    location,
			    "__go_new_channel",
			    2,
			    channel_type,
			    sizetype,
			    element_size_tree,
			    sizetype,
			    expr_tree);
}

// Type descriptor.

void
Channel_type::do_type_descriptor_decl(Gogo* gogo, Named_type* name, tree* pdecl)
{
  gogo->channel_type_descriptor_decl(this, name, pdecl);
}

// Reflection string.

void
Channel_type::do_reflection(Gogo* gogo, std::string* ret) const
{
  if (!this->may_send_)
    ret->append("<-");
  ret->append("chan");
  if (!this->may_receive_)
    ret->append("<-");
  ret->push_back(' ');
  this->append_reflection(this->element_type_, gogo, ret);
}

// Mangled name.

void
Channel_type::do_mangled_name(Gogo* gogo, std::string* ret) const
{
  ret->push_back('C');
  this->append_mangled_name(this->element_type_, gogo, ret);
  if (this->may_send_)
    ret->push_back('s');
  if (this->may_receive_)
    ret->push_back('r');
  ret->push_back('e');
}

// Export.

void
Channel_type::do_export(Export* exp) const
{
  exp->write_c_string("chan ");
  if (this->may_send_ && !this->may_receive_)
    exp->write_c_string("-< ");
  else if (this->may_receive_ && !this->may_send_)
    exp->write_c_string("<- ");
  exp->write_type(this->element_type_);
}

// Import.

Channel_type*
Channel_type::do_import(Import* imp)
{
  imp->require_c_string("chan ");

  bool may_send;
  bool may_receive;
  if (imp->match_c_string("-< "))
    {
      imp->advance(3);
      may_send = true;
      may_receive = false;
    }
  else if (imp->match_c_string("<- "))
    {
      imp->advance(3);
      may_receive = true;
      may_send = false;
    }
  else
    {
      may_send = true;
      may_receive = true;
    }

  Type* element_type = imp->read_type();

  return Type::make_channel_type(may_send, may_receive, element_type);
}

// Make a new channel type.

Channel_type*
Type::make_channel_type(bool send, bool receive, Type* element_type)
{
  return new Channel_type(send, receive, element_type);
}

// Class Interface_type.

// Traversal.

int
Interface_type::do_traverse(Traverse* traverse)
{
  return this->methods_->traverse(traverse);
}

// Finalize the methods.  This handles interface inheritance.

void
Interface_type::finalize_methods()
{
  if (this->methods_ == NULL)
    return;
  bool is_recursive = false;
  size_t from = 0;
  size_t to = 0;
  while (from < this->methods_->size())
    {
      const Typed_identifier* p = &this->methods_->at(from);
      if (!p->name().empty())
	{
	  if (from != to)
	    this->methods_->set(to, *p);
	  ++from;
	  ++to;
	  continue;
	}
      Interface_type* it = p->type()->interface_type();
      if (it == NULL)
	{
	  error_at(p->location(), "interface contains embedded non-interface");
	  ++from;
	  continue;
	}
      if (it == this)
	{
	  if (!is_recursive)
	    {
	      error_at(p->location(), "invalid recursive interface");
	      is_recursive = true;
	    }
	  ++from;
	  continue;
	}
      const Typed_identifier_list* methods = it->methods();
      for (Typed_identifier_list::const_iterator q = methods->begin();
	   q != methods->end();
	   ++q)
	{
	  if (q->name().empty() || this->find_method(q->name()) == NULL)
	    this->methods_->push_back(*q);
	  else
	    {
	      if (!is_recursive)
		error_at(p->location(), "inherited method %qs is ambiguous",
			 Gogo::unpack_hidden_name(q->name()).c_str());
	    }
	}
      ++from;
    }
  this->methods_->resize(to);
  this->methods_->sort_by_name();
}

// Return the method NAME, or NULL.

const Typed_identifier*
Interface_type::find_method(const std::string& name) const
{
  for (Typed_identifier_list::const_iterator p = this->methods_->begin();
       p != this->methods_->end();
       ++p)
    if (p->name() == name)
      return &*p;
  return NULL;
}

// Return the method index.

size_t
Interface_type::method_index(const std::string& name) const
{
  size_t ret = 0;
  for (Typed_identifier_list::const_iterator p = this->methods_->begin();
       p != this->methods_->end();
       ++p, ++ret)
    if (p->name() == name)
      return ret;
  gcc_unreachable();
}

// Whether this type is compatible with T.

bool
Interface_type::is_compatible(const Interface_type* t,
			      Type_compatible compatible) const
{
  // We require the same methods with the same types.  The methods
  // have already been sorted.
  if (this->methods() == NULL || t->methods() == NULL)
    return this->methods() == t->methods();

  Typed_identifier_list::const_iterator p1 = this->methods()->begin();
  for (Typed_identifier_list::const_iterator p2 = t->methods()->begin();
       p2 != t->methods()->end();
       ++p1, ++p2)
    {
      if (p1 == this->methods()->end())
	return false;
      if (p1->name() != p2->name()
	  || !Type::are_compatible_for(p1->type(), p2->type(), compatible,
				       NULL))
	return false;
    }
  if (p1 != this->methods()->end())
    return false;
  return true;
}

// Whether we can assign the interface type T to this type.  The types
// are known to not be compatible.  An interface assignment is only
// permitted if T is known to implement all methods in THIS.
// Otherwise a type guard is required.

bool
Interface_type::is_compatible_for_assign(const Interface_type* t,
					 std::string* reason) const
{
  if (this->methods() == NULL)
    return true;
  for (Typed_identifier_list::const_iterator p = this->methods()->begin();
       p != this->methods()->end();
       ++p)
    {
      const Typed_identifier* m = t->find_method(p->name());
      if (m == NULL)
	{
	  if (reason != NULL)
	    {
	      char buf[200];
	      snprintf(buf, sizeof buf,
		       _("need explicit conversion; missing method %s%s%s"),
		       open_quote,
		       Gogo::unpack_hidden_name(p->name()).c_str(),
		       close_quote);
	      reason->assign(buf);
	    }
	  return false;
	}

      std::string subreason;
      if (!Type::are_compatible_for(p->type(), m->type(), COMPATIBLE_COMPATIBLE,
				    &subreason))
	{
	  if (reason != NULL)
	    {
	      size_t len = 200 + p->name().length() + subreason.length();
	      char* buf = new char[len];
	      if (subreason.empty())
		snprintf(buf, len, _("incompatible type for method %s%s%s"),
			 open_quote,
			 Gogo::unpack_hidden_name(p->name()).c_str(),
			 close_quote);
	      else
		snprintf(buf, len,
			 _("incompatible type for method %s%s%s (%s)"),
			 open_quote,
			 Gogo::unpack_hidden_name(p->name()).c_str(),
			 close_quote, subreason.c_str());
	      reason->assign(buf);
	      delete[] buf;
	    }
	  return false;
	}
    }

  return true;
}

// Hash code.

size_t
Interface_type::do_hash_for_method() const
{
  size_t ret = 0;
  if (this->methods_ != NULL)
    {
      for (Typed_identifier_list::const_iterator p = this->methods_->begin();
	   p != this->methods_->end();
	   ++p)
	{
	  for (const char* s = p->name().c_str(); *s != '\0'; ++s)
	    {
	      ret += *s;
	      ret <<= 1;
	    }
	  ret += p->type()->hash_for_method();
	  ret <<= 1;
	}
    }
  return ret;
}

// Return true if T implements the interface.  If it does not, and
// REASON is not NULL, set *REASON to a useful error message.

bool
Interface_type::implements_interface(const Type* t, std::string* reason) const
{
  if (this->methods_ == NULL || this->methods_->empty())
    return true;

  bool is_pointer = false;
  const Named_type* nt = t->named_type();
  // If we start with a named type, we don't dereference it to find
  // methods.
  if (nt == NULL)
    {
      const Type* pt = t->points_to();
      if (pt != NULL)
	{
	  // If T is a pointer to a named type, then we need to look at
	  // the type to which it points.
	  is_pointer = true;
	  nt = pt->named_type();
	}
    }

  // Only named types have methods.
  if (nt == NULL)
    {
      if (reason != NULL)
	reason->assign(_("type has no methods"));
      return false;
    }

  if (!nt->has_any_methods())
    {
      if (reason != NULL)
	reason->assign(_("type has no methods"));
      return false;
    }

  for (Typed_identifier_list::const_iterator p = this->methods_->begin();
       p != this->methods_->end();
       ++p)
    {
      bool is_ambiguous = false;
      Method* m = nt->method_function(p->name(), &is_ambiguous);
      if (m == NULL)
	{
	  if (reason != NULL)
	    {
	      char buf[200];

	      if (is_ambiguous)
		snprintf(buf, sizeof buf, _("ambiguous method %s%s%s"),
			 open_quote,
			 Gogo::unpack_hidden_name(p->name()).c_str(),
			 close_quote);
	      else
		snprintf(buf, sizeof buf, _("missing method %s%s%s"),
			 open_quote,
			 Gogo::unpack_hidden_name(p->name()).c_str(),
			 close_quote);
	      reason->assign(buf);
	    }
	  return false;
	}

      Function_type *p_fn_type = p->type()->function_type();
      Function_type* m_fn_type = m->type()->function_type();
      gcc_assert(p_fn_type != NULL && m_fn_type != NULL);
      std::string subreason;
      if (!p_fn_type->is_compatible(m_fn_type, COMPATIBLE_COMPATIBLE, true,
				    &subreason))
	{
	  if (reason != NULL)
	    {
	      size_t len = 200 + p->name().length() + subreason.length();
	      char* buf = new char[len];
	      if (subreason.empty())
		snprintf(buf, len, _("incompatible type for method %s%s%s"),
			 open_quote,
			 Gogo::unpack_hidden_name(p->name()).c_str(),
			 close_quote);
	      else
		snprintf(buf, len,
			 _("incompatible type for method %s%s%s (%s)"),
			 open_quote,
			 Gogo::unpack_hidden_name(p->name()).c_str(),
			 close_quote, subreason.c_str());
	      reason->assign(buf);
	      delete[] buf;
	    }
	  return false;
	}

      if (!is_pointer && !m->is_value_method())
	{
	  if (reason != NULL)
	    {
	      size_t len = 200 + p->name().length();
	      char* buf = new char[len];
	      snprintf(buf, len, _("method %s%s%s requires a pointer"),
		       open_quote,
		       Gogo::unpack_hidden_name(p->name()).c_str(),
		       close_quote);
	      reason->assign(buf);
	      delete[] buf;
	    }
	  return false;
	}
    }

  return true;
}

// Return a tree for an interface type.  An interface is a pointer to
// a struct.  The struct has three fields.  The first field is a
// pointer to the type descriptor for the dynamic type of the object.
// The second field is a pointer to a table of methods for the
// interface to be used with the object.  The third field is the value
// of the object itself.

tree
Interface_type::do_get_tree(Gogo* gogo)
{
  // Interface types can refer to themselves via pointers.
  tree ret = make_node(POINTER_TYPE);
  SET_TYPE_MODE(ret, ptr_mode);
  layout_type(ret);
  this->set_incomplete_type_tree(ret);

  // Build the type of the table of methods.
  std::string last_name = "";
  tree method_table = make_node(RECORD_TYPE);
  if (this->methods_ != NULL)
    {
      tree* pp = &TYPE_FIELDS(method_table);
      for (Typed_identifier_list::const_iterator p = this->methods_->begin();
	   p != this->methods_->end();
	   ++p)
	{
	  std::string name = Gogo::unpack_hidden_name(p->name());
	  tree name_tree = get_identifier_with_length(name.data(),
						      name.length());
	  tree field_type = p->type()->get_tree(gogo);
	  if (field_type == error_mark_node)
	    return error_mark_node;
	  tree field = build_decl(this->location_, FIELD_DECL, name_tree,
				  field_type);
	  DECL_CONTEXT(field) = method_table;
	  *pp = field;
	  pp = &TREE_CHAIN(field);
	  // Sanity check: the names should be sorted.
	  gcc_assert(p->name() > last_name);
	  last_name = p->name();
	}
    }
  layout_type(method_table);

  // Build the type of the struct.  We don't use finish_builtin_struct
  // because we want to set the name of the interface to the name of
  // the type, if possible.
  tree struct_type = make_node(RECORD_TYPE);

  tree id = get_identifier("__type_descriptor");
  tree dtype = gogo->type_descriptor_type_tree();
  dtype = build_qualified_type(dtype, TYPE_QUAL_CONST);
  tree field = build_decl(this->location_, FIELD_DECL, id,
			  build_pointer_type(dtype));
  DECL_CONTEXT(field) = struct_type;
  TYPE_FIELDS(struct_type) = field;
  tree last_field = field;

  id = get_identifier("__methods");
  field = build_decl(this->location_, FIELD_DECL, id,
		     build_pointer_type(method_table));
  DECL_CONTEXT(field) = struct_type;
  TREE_CHAIN(last_field) = field;
  last_field = field;

  id = get_identifier("__object");
  field = build_decl(this->location_, FIELD_DECL, id, ptr_type_node);
  DECL_CONTEXT(field) = struct_type;
  TREE_CHAIN(last_field) = field;

  layout_type(struct_type);

  // We have to do this by hand since we had to create the node
  // already.
  TREE_TYPE(ret) = struct_type;
  TYPE_POINTER_TO(struct_type) = ret;
  if (TYPE_STRUCTURAL_EQUALITY_P(struct_type))
    SET_TYPE_STRUCTURAL_EQUALITY(ret);
  return ret;
}

// Initialization value.

tree
Interface_type::do_init_tree(Gogo* gogo, bool is_clear)
{
  return (is_clear
	  ? NULL
	  : fold_convert(this->get_tree(gogo), null_pointer_node));
}

// Type descriptor.

void
Interface_type::do_type_descriptor_decl(Gogo* gogo, Named_type* name,
					tree* pdecl)
{
  gogo->interface_type_descriptor_decl(this, name, pdecl);
}

// Reflection string.

void
Interface_type::do_reflection(Gogo* gogo, std::string* ret) const
{
  ret->append("interface {");
  for (Typed_identifier_list::const_iterator p = this->methods_->begin();
       p != this->methods_->end();
       ++p)
    {
      if (p != this->methods_->begin())
	ret->append(";");
      ret->push_back(' ');
      ret->append(Gogo::unpack_hidden_name(p->name()));
      ret->push_back(' ');
      std::string sub = p->type()->reflection(gogo);
      gcc_assert(sub.compare(0, 4, "func") == 0);
      sub = sub.substr(4);
      ret->append(sub);
    }
  ret->append(" }");
}

// Mangled name.

void
Interface_type::do_mangled_name(Gogo* gogo, std::string* ret) const
{
  ret->push_back('I');

  const Typed_identifier_list* methods = this->methods_;
  if (methods != NULL)
    {
      for (Typed_identifier_list::const_iterator p = methods->begin();
	   p != methods->end();
	   ++p)
	{
	  std::string n = Gogo::unpack_hidden_name(p->name());
	  char buf[20];
	  snprintf(buf, sizeof buf, "%u_",
		   static_cast<unsigned int>(n.length()));
	  ret->append(buf);
	  ret->append(n);
	  this->append_mangled_name(p->type(), gogo, ret);
	}
    }

  ret->push_back('e');
}

// Export.

void
Interface_type::do_export(Export* exp) const
{
  exp->write_c_string("interface { ");

  const Typed_identifier_list* methods = this->methods_;
  gcc_assert(methods != NULL);
  for (Typed_identifier_list::const_iterator pm = methods->begin();
       pm != methods->end();
       ++pm)
    {
      exp->write_string(pm->name());
      exp->write_c_string(" (");

      const Function_type* fntype = pm->type()->function_type();

      bool first = true;
      const Typed_identifier_list* parameters = fntype->parameters();
      if (parameters != NULL)
	{
	  for (Typed_identifier_list::const_iterator pp =
		 parameters->begin();
	       pp != parameters->end();
	       ++pp)
	    {
	      if (first)
		first = false;
	      else
		exp->write_c_string(", ");
	      exp->write_type(pp->type());
	    }
	}

      exp->write_c_string(")");

      const Typed_identifier_list* results = fntype->results();
      if (results != NULL)
	{
	  exp->write_c_string(" ");
	  if (results->size() == 1)
	    exp->write_type(results->begin()->type());
	  else
	    {
	      first = true;
	      exp->write_c_string("(");
	      for (Typed_identifier_list::const_iterator p =
		     results->begin();
		   p != results->end();
		   ++p)
		{
		  if (first)
		    first = false;
		  else
		    exp->write_c_string(", ");
		  exp->write_type(p->type());
		}
	      exp->write_c_string(")");
	    }
	}

      exp->write_c_string("; ");
    }

  exp->write_c_string("}");
}

// Import an interface type.

Interface_type*
Interface_type::do_import(Import* imp)
{
  imp->require_c_string("interface { ");

  Typed_identifier_list* methods = new Typed_identifier_list;
  while (imp->peek_char() != '}')
    {
      std::string name = imp->read_identifier();
      imp->require_c_string(" (");

      Typed_identifier_list* parameters;
      if (imp->peek_char() == ')')
	parameters = NULL;
      else
	{
	  parameters = new Typed_identifier_list;
	  while (true)
	    {
	      Type* ptype = imp->read_type();
	      parameters->push_back(Typed_identifier(Import::import_marker,
						     ptype, imp->location()));
	      if (imp->peek_char() != ',')
		break;
	      imp->require_c_string(", ");
	    }
	}
      imp->require_c_string(")");

      Typed_identifier_list* results;
      if (imp->peek_char() != ' ')
	results = NULL;
      else
	{
	  results = new Typed_identifier_list;
	  imp->advance(1);
	  if (imp->peek_char() != '(')
	    {
	      Type* rtype = imp->read_type();
	      results->push_back(Typed_identifier(Import::import_marker,
						  rtype, imp->location()));
	    }
	  else
	    {
	      imp->advance(1);
	      while (true)
		{
		  Type* rtype = imp->read_type();
		  results->push_back(Typed_identifier(Import::import_marker,
						      rtype, imp->location()));
		  if (imp->peek_char() != ',')
		    break;
		  imp->require_c_string(", ");
		}
	      imp->require_c_string(")");
	    }
	}

      Type* fntype = Type::make_function_type(NULL, parameters, results,
					      imp->location());
      methods->push_back(Typed_identifier(name, fntype, imp->location()));

      imp->require_c_string("; ");
    }

  imp->require_c_string("}");

  return Type::make_interface_type(methods, imp->location());
}

// Make an interface type.

Interface_type*
Type::make_interface_type(Typed_identifier_list* methods,
			  source_location location)
{
  return new Interface_type(methods, location);
}

// Class Method.

// Bind a method to an object.

Expression*
Method::bind_method(Expression* expr, source_location location) const
{
  if (this->stub_ == NULL)
    {
      // When there is no stub object, the binding is determined by
      // the child class.
      return this->do_bind_method(expr, location);
    }

  Expression* func = Expression::make_func_reference(this->stub_, NULL,
						     location);
  return Expression::make_bound_method(expr, func, location);
}

// Return the named object associated with a method.  This may only be
// called after methods are finalized.

Named_object*
Method::named_object() const
{
  if (this->stub_ != NULL)
    return this->stub_;
  return this->do_named_object();
}

// Class Named_method.

// The type of the method.

Function_type*
Named_method::do_type() const
{
  if (this->named_object_->is_function())
    return this->named_object_->func_value()->type();
  else if (this->named_object_->is_function_declaration())
    return this->named_object_->func_declaration_value()->type();
  else
    gcc_unreachable();
}

// Return the location of the method receiver.

source_location
Named_method::do_receiver_location() const
{
  return this->do_type()->receiver()->location();
}

// Bind a method to an object.

Expression*
Named_method::do_bind_method(Expression* expr, source_location location) const
{
  Expression* func = Expression::make_func_reference(this->named_object_, NULL,
						     location);
  Bound_method_expression* bme = Expression::make_bound_method(expr, func,
							       location);
  // If this is not a local method, and it does not use a stub, then
  // the real method expects a different type.  We need to cast the
  // first argument.
  if (this->depth() > 0 && !this->needs_stub_method())
    {
      Function_type* ftype = this->do_type();
      gcc_assert(ftype->is_method());
      Type* frtype = ftype->receiver()->type();
      bme->set_first_argument_type(frtype);
    }
  return bme;
}

// Class Interface_method.

// Bind a method to an object.

Expression*
Interface_method::do_bind_method(Expression* expr,
				 source_location location) const
{
  return Expression::make_interface_field_reference(expr, this->name_,
						    location);
}

// Class Methods.

// Insert a new method.  Return true if it was inserted, false
// otherwise.

bool
Methods::insert(const std::string& name, Method* m)
{
  std::pair<Method_map::iterator, bool> ins =
    this->methods_.insert(std::make_pair(name, m));
  if (ins.second)
    return true;
  else
    {
      Method* old_method = ins.first->second;
      if (m->depth() < old_method->depth())
	{
	  delete old_method;
	  ins.first->second = m;
	  return true;
	}
      else
	{
	  if (m->depth() == old_method->depth())
	    old_method->set_is_ambiguous();
	  return false;
	}
    }
}

// Return the number of unambiguous methods.

size_t
Methods::count() const
{
  size_t ret = 0;
  for (Method_map::const_iterator p = this->methods_.begin();
       p != this->methods_.end();
       ++p)
    if (!p->second->is_ambiguous())
      ++ret;
  return ret;
}

// Class Named_type.

// Return the name of the type.

const std::string&
Named_type::name() const
{
  return this->named_object_->name();
}

// Add a method to this type.

Named_object*
Named_type::add_method(const std::string& name, Function* function)
{
  if (this->local_methods_ == NULL)
    this->local_methods_ = new Bindings(NULL);
  return this->local_methods_->add_function(name, NULL, function);
}

// Add a method declaration to this type.

Named_object*
Named_type::add_method_declaration(const std::string& name, Package* package,
				   Function_type* type,
				   source_location location)
{
  if (this->local_methods_ == NULL)
    this->local_methods_ = new Bindings(NULL);
  return this->local_methods_->add_function_declaration(name, package, type,
							location);
}

// Add an existing method to this type.

void
Named_type::add_existing_method(Named_object* no)
{
  if (this->local_methods_ == NULL)
    this->local_methods_ = new Bindings(NULL);
  this->local_methods_->add_named_object(no);
}

// Return whether NO is a method for which the receiver is a pointer.

bool
Named_type::method_expects_pointer(const Named_object* no)
{
  const Function_type *fntype;
  if (no->is_function())
    fntype = no->func_value()->type();
  else if (no->is_function_declaration())
    fntype = no->func_declaration_value()->type();
  else
    gcc_unreachable();
  return fntype->receiver()->type()->points_to() != NULL;
}

// Look for field or method NAME associated with this type at parse
// time.  Return an Expression for the field or method bound to EXPR,
// or NULL if there is no such field or method.  When returning NULL,
// this sets *FOUND_POINTER_METHOD if we found a method we couldn't
// use because it requires a pointer.

Expression*
Named_type::bind_field_or_method(Expression* expr, const std::string& name,
				 source_location location,
				 bool *found_pointer_method) const
{
  *found_pointer_method = false;

  const Struct_type* st = this->deref()->struct_type();
  if (st != NULL)
    {
      unsigned int index = 0;
      const Struct_field* sf = st->find_local_field(name, &index);
      if (sf != NULL)
	return Expression::make_field_reference(expr, index, location);
    }

  if (this->local_methods_ != NULL)
    {
      Named_object* no = this->local_methods_->lookup(name);
      if (no != NULL)
	{
	  // Take the address of the expression if necessary.
	  if (expr->type()->points_to() == NULL
	      && Named_type::method_expects_pointer(no))
	    {
	      if (expr->is_lvalue())
		expr = Expression::make_unary(OPERATOR_AND, expr, location);
	      else
		{
		  *found_pointer_method = true;
		  return NULL;
		}
	    }
	  Expression* func = Expression::make_func_reference(no, NULL,
							     location);
	  return Expression::make_bound_method(expr, func, location);
	}
    }

  bool receiver_can_be_pointer = (expr->type()->points_to() != NULL
				  || expr->is_lvalue());
  bool is_method;
  if (this->find_field_or_method(name, receiver_can_be_pointer, NULL,
				 &is_method, found_pointer_method))
    {
      if (is_method)
	{
	  bool is_ambiguous;
	  Method* m = this->method_function(name, &is_ambiguous);
	  if (m == NULL)
	    {
	      // We just found the method, so this can only fail if it
	      // is ambiguous.
	      gcc_assert(is_ambiguous);
	      error_at(location, "reference to method %qs is ambiguous",
		       Gogo::unpack_hidden_name(name).c_str());
	      return Expression::make_error(location);
	    }
	  return m->bind_method(expr, location);
	}
      else
	{
	  // This is a field in an embedded struct.  We look it up
	  // again, looking only at fields this time.  This is simpler
	  // than building the subexpression we need as we go along.
	  gcc_assert(this->struct_type() != NULL);
	  Expression* ret = this->struct_type()->field_reference(expr, name,
								 location);
	  gcc_assert(ret != NULL);
	  return ret;
	}
    }

  return NULL;
}

// Look for a field or method named NAME, returning whether one is
// found.  This looks through embedded anonymous fields and handles
// ambiguity.  If a method is found, set *IS_METHOD to true;
// otherwise, if a field is found, set it to false.  If
// RECEIVER_CAN_BE_POINTER is false, then the receiver is a value
// whose address can not be taken.  When returning false, this sets
// *FOUND_POINTER_METHOD if we found a method we couldn't use because
// it requires a pointer.  LEVEL is used for recursive calls.

bool
Named_type::find_field_or_method(const std::string& name,
				 bool receiver_can_be_pointer,
				 int* level,
				 bool* is_method,
				 bool* found_pointer_method) const
{
  if (this->local_methods_ != NULL)
    {
      Named_object* no = this->local_methods_->lookup(name);
      if (no != NULL)
	{
	  if (!receiver_can_be_pointer
	      && Named_type::method_expects_pointer(no))
	    *found_pointer_method = true;
	  else
	    {
	      *is_method = true;
	      return true;
	    }
	}
    }

  const Interface_type* it = this->interface_type();
  if (it != NULL)
    {
      const Typed_identifier* tid = it->find_method(name);
      if (tid == NULL)
	return false;
      *is_method = true;
      return true;
    }

  const Struct_type* st = this->struct_type();
  if (st == NULL)
    return false;

  const Struct_field_list* fields = st->fields();
  if (fields == NULL)
    return false;

  bool found = false;
  bool found_is_method = false;
  int found_level = 0;
  for (Struct_field_list::const_iterator pf = fields->begin();
       pf != fields->end();
       ++pf)
    {
      if (pf->field_name() == name)
	{
	  *is_method = false;
	  return true;
	}

      if (!pf->is_anonymous())
	continue;

      Named_type* fntype = pf->type()->deref()->named_type();
      gcc_assert(fntype != NULL);

      int sublevel = level == NULL ? 1 : *level + 1;
      bool sub_is_method;
      bool subfound = fntype->find_field_or_method(name,
						   receiver_can_be_pointer,
						   &sublevel,
						   &sub_is_method,
						   found_pointer_method);
      if (subfound && (!found || sublevel < found_level))
	{
	  found = true;
	  found_is_method = sub_is_method;
	  found_level = sublevel;
	}
    }

  if (found)
    {
      if (level != NULL)
	*level = found_level;
      *is_method = found_is_method;
      return true;
    }

  return false;
}

// Build the complete list of methods for this type, which means
// recursively including all methods for anonymous fields.  Create all
// stub methods.

void
Named_type::finalize_methods(Gogo* gogo)
{
  if (this->local_methods_ != NULL && this->points_to() != NULL)
    {
      const Bindings* lm = this->local_methods_;
      for (Bindings::const_declarations_iterator p = lm->begin_declarations();
	   p != lm->end_declarations();
	   ++p)
	error_at(p->second->location(), "invalid pointer receiver type");
      delete this->local_methods_;
      this->local_methods_ = NULL;
      return;
    }

  Types_seen types_seen;
  this->add_methods_for_type(this, NULL, 0, false, false, &types_seen);

  this->build_stub_methods(gogo);
}

// Add the method for TYPE to the methods for THIS.  FIELD_INDEX is
// the index of the field.  FIELD_INDEX will be -1U when TYPE == THIS.
// Otherwise TYPE is the type of an anonymous field.  DEPTH is the
// depth of the field within THIS.  IS_EMBEDDED_POINTER is true if we
// are adding these methods for an anonymous field with pointer type.
// NEEDS_STUB_METHOD is true if we need to use a stub method which
// calls the real method.  TYPES_SEEN is used to avoid infinite
// recursion.  This returns true if any methods were added.

bool
Named_type::add_methods_for_type(const Named_type* type,
				 const Method::Field_indexes* field_indexes,
				 unsigned int depth,
				 bool is_embedded_pointer,
				 bool needs_stub_method,
				 Types_seen* types_seen)
{
  // Pointer types may not have methods.
  if (type->points_to() != NULL)
    return false;

  std::pair<Types_seen::iterator, bool> ins = types_seen->insert(type);
  if (!ins.second)
    return false;

  bool ret = false;

  if (this->add_local_methods_for_type(type, field_indexes, depth,
				       is_embedded_pointer, needs_stub_method))
    ret = true;

  if (this->add_embedded_methods_for_type(type, field_indexes, depth,
					  is_embedded_pointer,
					  needs_stub_method,
					  types_seen))
    ret = true;

  // If we are called with depth > 0, then we are looking at an
  // anonymous field of a struct.  If such a field has interface type,
  // then we need to add the interface methods.
  if (depth > 0)
    {
      if (this->add_interface_methods_for_type(type, field_indexes, depth))
	ret = true;
    }

  return ret;
}

// Add the local methods for TYPE to this type.  The parameters and
// return value are as for add_methods_for_type.

bool
Named_type::add_local_methods_for_type(
    const Named_type* type,
    const Method::Field_indexes* field_indexes,
    unsigned int depth,
    bool is_embedded_pointer,
    bool needs_stub_method)
{
  if (type->local_methods_ == NULL)
    return false;

  if (this->all_methods_ == NULL)
    this->all_methods_ = new Methods();

  bool ret = false;
  const Bindings* lm = type->local_methods_;
  for (Bindings::const_declarations_iterator p = lm->begin_declarations();
       p != lm->end_declarations();
       ++p)
    {
      Named_object* no = p->second;
      bool is_value_method = (is_embedded_pointer
			      || !Named_type::method_expects_pointer(no));
      Method* m = new Named_method(no, field_indexes, depth, is_value_method,
				   needs_stub_method);
      if (this->all_methods_->insert(no->name(), m))
	ret = true;
      else
	delete m;
    }

  return ret;
}

// Add the embedded methods for TYPE to this type.  These are the
// methods attached to anonymous fields.  The parameters and return
// value are as for add_methods_for_type.

bool
Named_type::add_embedded_methods_for_type(
    const Named_type* type,
    const Method::Field_indexes* field_indexes,
    unsigned int depth,
    bool is_embedded_pointer,
    bool needs_stub_method,
    Types_seen* types_seen)
{
  // Look for anonymous fields in TYPE.  TYPE has fields if it is a
  // struct.
  const Struct_type* st = type->struct_type();
  if (st == NULL)
    return false;

  const Struct_field_list* fields = st->fields();
  if (fields == NULL)
    {
      // Having an incomplete type is most likely an error which will
      // be reported elsewhere.  Here we just avoid a crash.
      return false;
    }

  bool ret = false;
  unsigned int i = 0;
  for (Struct_field_list::const_iterator pf = fields->begin();
       pf != fields->end();
       ++pf, ++i)
    {
      if (!pf->is_anonymous())
	continue;

      Type* ftype = pf->type();
      bool is_pointer = false;
      if (ftype->points_to() != NULL)
	{
	  ftype = ftype->points_to();
	  is_pointer = true;
	}
      Named_type* fntype = ftype->named_type();
      gcc_assert(fntype != NULL);

      Method::Field_indexes* sub_field_indexes = new Method::Field_indexes();
      sub_field_indexes->next = field_indexes;
      sub_field_indexes->field_index = i;

      if (this->add_methods_for_type(fntype, sub_field_indexes, depth + 1,
				     (is_embedded_pointer || is_pointer),
				     (needs_stub_method
				      || is_pointer
				      || i > 0),
				     types_seen))
	ret = true;
      else
	delete sub_field_indexes;
    }

  return ret;
}

// If TYPE is an interface type, then add its methods to this type.
// This is for interface methods attached to an anonymous field.  The
// parameters and return value are as for add_methods_for_type, except
// that the boolean parameters are unnecessary.

bool
Named_type::add_interface_methods_for_type(
    const Named_type* type,
    const Method::Field_indexes* field_indexes,
    unsigned int depth)
{
  const Interface_type* it = type->interface_type();
  if (it == NULL)
    return false;

  const Typed_identifier_list* methods = it->methods();
  if (methods == NULL)
    return false;

  if (this->all_methods_ == NULL)
    this->all_methods_ = new Methods();

  bool ret = false;
  for (Typed_identifier_list::const_iterator pm = methods->begin();
       pm != methods->end();
       ++pm)
    {
      Function_type *fntype = pm->type()->function_type();
      gcc_assert(fntype != NULL);
      Method* m = new Interface_method(pm->name(), pm->location(), fntype,
				       field_indexes, depth);
      if (this->all_methods_->insert(pm->name(), m))
	ret = true;
      else
	delete m;
    }

  return ret;
}

// The name we use for the receiver of a stub method.

const char* const Named_type::receiver_name = "__this";

// Build stub methods as needed.  A stub method may be needed when we
// inherit a method from an anonymous field.  When we need the address
// of the method, as in a type descriptor, we need to build a little
// stub which does the required field dereferences and jumps to the
// real method.

void
Named_type::build_stub_methods(Gogo* gogo)
{
  if (this->all_methods_ == NULL)
    return;
  for (Methods::const_iterator p = this->all_methods_->begin();
       p != this->all_methods_->end();
       ++p)
    {
      Method* m = p->second;
      if (m->is_ambiguous() || !m->needs_stub_method())
	continue;

      const std::string& name(p->first);

      // Build a stub method.

      const Function_type* fntype = m->type();

      static int counter;
      char buf[100];
      snprintf(buf, sizeof buf, "%s%d", Named_type::receiver_name, counter);
      ++counter;

      Type* receiver_type = Type::make_pointer_type(this);
      source_location receiver_location = m->receiver_location();
      Typed_identifier* receiver = new Typed_identifier(buf, receiver_type,
							receiver_location);

      const Typed_identifier_list* fnparams = fntype->parameters();
      Typed_identifier_list* stub_params;
      if (fnparams == NULL)
	stub_params = NULL;
      else
	{
	  // We give each stub parameter we create a unique name.
	  static unsigned int count;
	  stub_params = new Typed_identifier_list;
	  for (Typed_identifier_list::const_iterator p = fnparams->begin();
	       p != fnparams->end();
	       ++p, ++count)
	    {
	      char buf[100];
	      snprintf(buf, sizeof buf, "p%u", count);
	      stub_params->push_back(Typed_identifier(buf, p->type(),
						      p->location()));
	    }
	}

      const Typed_identifier_list* fnresults = fntype->results();
      Typed_identifier_list* stub_results;
      if (fnresults == NULL)
	stub_results = NULL;
      else
	{
	  // We create the result parameters without any names, since
	  // we won't refer to them.
	  stub_results = new Typed_identifier_list;
	  for (Typed_identifier_list::const_iterator p = fnresults->begin();
	       p != fnresults->end();
	       ++p)
	    stub_results->push_back(Typed_identifier("", p->type(),
						     p->location()));
	}

      Function_type* stub_type = Type::make_function_type(receiver,
							  stub_params,
							  stub_results,
							  fntype->location());
      if (fntype->is_varargs())
	stub_type->set_is_varargs();

      // We only create the function in the source file which creates
      // the type.
      Named_object* stub;
      if (this->named_object_->package() != NULL)
	{
	  stub = gogo->declare_function(name, stub_type,
					fntype->location());
	  stub->set_package(this->named_object_->package());
	}
      else
	{
	  stub = gogo->start_function(name, stub_type, false,
				      fntype->location());
	  this->build_one_stub_method(gogo, m, buf, stub_params);
	  gogo->finish_function(fntype->location());
	}

      m->set_stub_object(stub);
    }
}

// Build a stub method which adjusts the receiver according to
// FIELD_INDEXES and calls METHOD.  PARAMS is the list of function
// parameters.

void
Named_type::build_one_stub_method(Gogo* gogo, Method* method,
				  const char* receiver_name,
				  const Typed_identifier_list* params)
{
  // FIXME: Using a predetermined name is a hack.
  Named_object* this_object = gogo->lookup(receiver_name, NULL);
  gcc_assert(this_object != NULL);

  source_location location = this->location_;
  Expression* expr = Expression::make_var_reference(this_object, location);

  expr = this->apply_field_indexes(expr, method->field_indexes());
  if (expr->type()->points_to() == NULL)
    expr = Expression::make_unary(OPERATOR_AND, expr, location);

  Expression_list* arguments;
  if (params == NULL)
    arguments = NULL;
  else
    {
      arguments = new Expression_list;
      for (Typed_identifier_list::const_iterator p = params->begin();
	   p != params->end();
	   ++p)
	{
	  Named_object* param = gogo->lookup(p->name(), NULL);
	  gcc_assert(param != NULL);
	  Expression* param_ref = Expression::make_var_reference(param,
								 location);
	  arguments->push_back(param_ref);
	}
    }

  Expression* func = method->bind_method(expr, location);
  Call_expression* call = Expression::make_call(func, arguments, location);
  size_t count = call->result_count();
  if (count == 0)
    gogo->add_statement(Statement::make_statement(call));
  else
    {
      Expression_list* retvals = new Expression_list();
      if (count <= 1)
	retvals->push_back(call);
      else
	{
	  for (size_t i = 0; i < count; ++i)
	    retvals->push_back(Expression::make_call_result(call, i));
	}
      const Function* function = gogo->current_function()->func_value();
      Statement* retstat = Statement::make_return_statement(function, retvals,
							    location);
      gogo->add_statement(retstat);
    }
}

// Apply FIELD_INDEXES to EXPR.  The field indexes have to be applied
// in reverse order.

Expression*
Named_type::apply_field_indexes(Expression* expr,
				const Method::Field_indexes* field_indexes)
{
  if (field_indexes == NULL)
    return expr;
  expr = this->apply_field_indexes(expr, field_indexes->next);
  Struct_type* stype = expr->type()->deref()->struct_type();
  gcc_assert(stype != NULL
	     && field_indexes->field_index < stype->field_count());
  return Expression::make_field_reference(expr, field_indexes->field_index,
					  this->location_);
}

// Return the method NAME, or NULL if there isn't one or if it is
// ambiguous.  Set *IS_AMBIGUOUS if the method exists but is
// ambiguous.

Method*
Named_type::method_function(const std::string& name, bool* is_ambiguous) const
{
  *is_ambiguous = false;
  if (this->all_methods_ == NULL)
    return NULL;
  Methods::const_iterator p = this->all_methods_->find(name);
  if (p == this->all_methods_->end())
    return NULL;
  Method* m = p->second;
  if (m->is_ambiguous())
    {
      *is_ambiguous = true;
      return NULL;
    }
  return m;
}

// Return a pointer to the interface method table for this type for
// the interface INTERFACE.  IS_POINTER is true if this is for value
// of pointer type.

tree
Named_type::interface_method_table(Gogo* gogo, const Interface_type* interface)
{
  if (interface->method_count() == 0)
    return null_pointer_node;

  if (this->interface_method_tables_ == NULL)
    this->interface_method_tables_ = new Interface_method_tables(5);

  std::pair<const Interface_type*, tree> val(interface, NULL_TREE);
  std::pair<Interface_method_tables::iterator, bool> ins =
    this->interface_method_tables_->insert(val);

  if (ins.second)
    {
      // This is a new entry in the hash table.
      gcc_assert(ins.first->second == NULL_TREE);
      ins.first->second = gogo->interface_method_table_for_type(interface,
								this);
    }

  tree decl = ins.first->second;
  if (decl == error_mark_node)
    return error_mark_node;
  gcc_assert(decl != NULL_TREE && TREE_CODE(decl) == VAR_DECL);
  return build_fold_addr_expr(decl);
}

// Whether this type is compatible with T.

bool
Named_type::is_compatible(const Type* t, Type_compatible compatible,
			  std::string* reason) const
{
  if (compatible == COMPATIBLE_IDENTICAL)
    return this == t;

  // In general, different named types are not compatible other than
  // for type conversion.  An exception is assigning to an variable
  // with interface type.
  if (t->named_type() != NULL && compatible != COMPATIBLE_FOR_CONVERSION)
    {
      if (reason != NULL)
	{
	  size_t len = (this->name().length()
			+ t->named_type()->name().length()
			+ 100);
	  char* buf = new char[len];
	  snprintf(buf, len, _("cannot use type %s as type %s"),
		   t->named_type()->name().c_str(),
		   this->name().c_str());
	  reason->assign(buf);
	  delete[] buf;
	}
      return false;
    }

  // We use the seen_ flag to cut off recursion.
  if (this->seen_)
    return this->type_->base() == t->base();
  this->seen_ = true;
  bool ret = Type::are_compatible_for(this->type_, t, compatible, reason);
  this->seen_ = false;
  return ret;
}

// Return whether a named type has any hidden fields.

bool
Named_type::named_type_has_hidden_fields(std::string* reason) const
{
  if (this->seen_)
    return false;
  this->seen_ = true;
  bool ret = this->type_->has_hidden_fields(this, reason);
  this->seen_ = false;
  return ret;
}

// Look for a use of a complete type within another type.  This is
// used to check that we don't try to use a type within itself.

class Find_type_use : public Traverse
{
 public:
  Find_type_use(Type* find_type)
    : Traverse(traverse_types),
      find_type_(find_type), found_(false)
  { }

  // Whether we found the type.
  bool
  found() const
  { return this->found_; }

 protected:
  int
  type(Type*);

 private:
  // The type we are looking for.
  Type* find_type_;
  // Whether we found the type.
  bool found_;
};

// Check for FIND_TYPE in TYPE.

int
Find_type_use::type(Type* type)
{
  if (this->find_type_ == type)
    {
      this->found_ = true;
      return TRAVERSE_EXIT;
    }
  // It's OK if we see a reference to the type in any type which is
  // essentially a pointer: a pointer, a slice, a function, a map, or
  // a channel.
  if (type->points_to() != NULL
      || type->is_open_array_type()
      || type->function_type() != NULL
      || type->map_type() != NULL
      || type->channel_type() != NULL)
    return TRAVERSE_SKIP_COMPONENTS;

  // For an interface, a reference to the type in a method type should
  // be ignored, but we have to consider direct inheritance.  When
  // this is called, there may be cases of direct inheritance
  // represented as a method with no name.
  if (type->interface_type() != NULL)
    {
      const Typed_identifier_list* methods = type->interface_type()->methods();
      for (Typed_identifier_list::const_iterator p = methods->begin();
	   p != methods->end();
	   ++p)
	{
	  if (p->name().empty())
	    {
	      if (Type::traverse(p->type(), this) == TRAVERSE_EXIT)
		return TRAVERSE_EXIT;
	    }
	}
      return TRAVERSE_SKIP_COMPONENTS;
    }

  return TRAVERSE_CONTINUE;
}

// Verify that a named type does not refer to itself.

bool
Named_type::do_verify()
{
  Find_type_use find(this);
  Type::traverse(this->type_, &find);
  if (find.found())
    {
      error_at(this->location_, "invalid recursive type %qs",
	       this->name().c_str());
      this->is_error_ = true;
      return false;
    }

  // Check whether any of the local methods overloads an existing
  // struct field or interface method.  We don't need to check the
  // list of methods against itself: that is handled by the Bindings
  // code.
  if (this->local_methods_ != NULL)
    {
      Struct_type* st = this->type_->struct_type();
      Interface_type* it = this->type_->interface_type();
      bool found_dup = false;
      if (st != NULL || it != NULL)
	{
	  for (Bindings::const_declarations_iterator p =
		 this->local_methods_->begin_declarations();
	       p != this->local_methods_->end_declarations();
	       ++p)
	    {
	      const std::string& name(p->first);
	      if (st != NULL && st->find_local_field(name, NULL) != NULL)
		{
		  error_at(p->second->location(),
			   "method %qs redeclares struct field name",
			   Gogo::unpack_hidden_name(name).c_str());
		  found_dup = true;
		}
	      if (it != NULL && it->find_method(name) != NULL)
		{
		  error_at(p->second->location(),
			   "method %qs redeclares interface method name",
			   Gogo::unpack_hidden_name(name).c_str());
		  found_dup = true;
		}
	    }
	}
      if (found_dup)
	return false;
    }

  return true;
}

// The number of reference count queue entries required.

void
Named_type::do_add_refcount_queue_entries(Refcounts* refcounts,
					  Refcount_entry* entry)
{
  // If this is not a struct or an array, just use the type itself.
  if (this->struct_type() == NULL && this->array_type() == NULL)
    this->Type::do_add_refcount_queue_entries(refcounts, entry);
  else
    this->type_->add_refcount_queue_entries(refcounts, entry);
}

// Return a hash code.  This is used for method lookup.  We simply
// hash on the name itself.

size_t
Named_type::do_hash_for_method() const
{
  size_t ret = 0;
  const std::string& name(this->named_object()->name());
  size_t len = name.length();
  const char* p = name.data();
  for (size_t i = 0; i < len; ++i, ++p)
    ret = (ret << 1) + *p;
  return ret;
}

// Get a tree for a named type.

tree
Named_type::do_get_tree(Gogo* gogo)
{
  if (this->is_error_)
    return error_mark_node;

  tree type_tree = this->type_->get_tree(gogo);
  // If an interface refers to itself recursively, then we may have an
  // incomplete type here.  It should get filled in somewhere higher
  // on the call stack.
  if (type_tree != error_mark_node
      && (!POINTER_TYPE_P (type_tree) || TREE_TYPE(type_tree) != NULL_TREE))
    {
      tree id = this->named_object_->get_id(gogo);

      // If we are looking at a struct or an interface, we don't need
      // to make a copy to hold the type.  Doing this makes it easier
      // for the middle-end to notice when the types refer to
      // themselves.
      if (TYPE_NAME(type_tree) == NULL
	  && (this->type_->struct_type() != NULL
	      || this->type_->interface_type() != NULL))
	;
      else
	{
	  // Make a copy so that we can set TYPE_NAME.
	  type_tree = build_variant_type_copy(type_tree);
	}

      tree decl = build_decl(this->location_, TYPE_DECL, id, type_tree);
      TYPE_NAME(type_tree) = decl;

      // Interfaces are pointers to structs; give the struct a name so
      // that the debugging information will be more useful.
      if (this->type_->interface_type() != NULL)
	{
	  gcc_assert(TREE_CODE(type_tree) == POINTER_TYPE);
	  tree stree = TREE_TYPE(type_tree);
	  gcc_assert(TREE_CODE(stree) == RECORD_TYPE);
	  if (TYPE_NAME(stree) != NULL)
	    stree = build_variant_type_copy(stree);
	  TYPE_NAME(stree) = decl;
	}
    }
  return type_tree;
}

// Type descriptor decl.

void
Named_type::do_type_descriptor_decl(Gogo* gogo, Named_type* name, tree* pdecl)
{
  // If NAME is not NULL, then we don't really want the type
  // descriptor for this type; we want the descriptor for the
  // underlying type, given the name NAME.
  this->named_type_descriptor(gogo, this->type_,
			      name == NULL ? this : name,
			      pdecl);
}

// Add to the reflection string.  This is used mostly for the name of
// the type used in a type descriptor, not for actual reflection
// strings.

void
Named_type::do_reflection(Gogo* gogo, std::string* ret) const
{
  if (this->location() != BUILTINS_LOCATION)
    {
      const Package* package = this->named_object_->package();
      if (package != NULL)
	ret->append(package->name());
      else
	ret->append(gogo->package_name());
      ret->push_back('.');
    }
  if (this->in_function_ != NULL)
    {
      ret->append(Gogo::unpack_hidden_name(this->in_function_->name()));
      ret->push_back('$');
    }
  ret->append(Gogo::unpack_hidden_name(this->named_object_->name()));
}

// Get the mangled name.

void
Named_type::do_mangled_name(Gogo* gogo, std::string* ret) const
{
  std::string package_name;
  if (this->named_object_->package() == NULL)
    package_name = gogo->package_name();
  else
    package_name = this->named_object_->package()->name();
  std::string name = package_name + '.';
  if (this->in_function_ != NULL)
    name += Gogo::unpack_hidden_name(this->in_function_->name()) + '$';
  name += Gogo::unpack_hidden_name(this->named_object_->name());
  char buf[20];
  snprintf(buf, sizeof buf, "N%u_", static_cast<unsigned int>(name.length()));
  ret->append(buf);
  ret->append(name);
}

// Export the type.  This is called to export a global type.

void
Named_type::export_named_type(Export* exp, const std::string&) const
{
  // We don't need to write the name of the type here, because it will
  // be written by Export::write_type anyhow.
  exp->write_c_string("type ");
  exp->write_type(this);
  exp->write_c_string(";\n");
}

// Import a named type.

void
Named_type::import_named_type(Import* imp, Named_type** ptype)
{
  imp->require_c_string("type ");
  Type *type = imp->read_type();
  *ptype = type->named_type();
  gcc_assert(*ptype != NULL);
  imp->require_c_string(";\n");
}

// Export the type when it is referenced by another type.  In this
// case Export::export_type will already have issued the name.

void
Named_type::do_export(Export* exp) const
{
  exp->write_type(this->type_);

  // To save space, we only export the methods directly attached to
  // this type.
  Bindings* methods = this->local_methods_;
  if (methods == NULL)
    return;

  exp->write_c_string("\n");
  for (Bindings::const_definitions_iterator p = methods->begin_definitions();
       p != methods->end_definitions();
       ++p)
    {
      exp->write_c_string(" ");
      (*p)->export_named_object(exp);
    }

  for (Bindings::const_declarations_iterator p = methods->begin_declarations();
       p != methods->end_declarations();
       ++p)
    {
      if (p->second->is_function_declaration())
	{
	  exp->write_c_string(" ");
	  p->second->export_named_object(exp);
	}
    }
}

// Make a named type.

Named_type*
Type::make_named_type(Named_object* named_object, Type* type,
		      source_location location)
{
  return new Named_type(named_object, type, location);
}

// Class Forward_declaration.

Forward_declaration_type::Forward_declaration_type(Named_object* named_object)
  : Type(TYPE_FORWARD),
    named_object_(named_object->resolve()), warned_(false)
{
  gcc_assert(this->named_object_->is_unknown()
	     || this->named_object_->is_type_declaration());
}

// Return the named object.

Named_object*
Forward_declaration_type::named_object()
{
  return this->named_object_->resolve();
}

const Named_object*
Forward_declaration_type::named_object() const
{
  return this->named_object_->resolve();
}

// Return the name of the forward declared type.

const std::string&
Forward_declaration_type::name() const
{
  return this->named_object()->name();
}

// Warn about a use of a type which has been declared but not defined.

void
Forward_declaration_type::warn() const
{
  Named_object* no = this->named_object_->resolve();
  if (no->is_unknown())
    {
      // The name was not defined anywhere.
      if (!this->warned_)
	{
	  error_at(this->named_object_->location(),
		   "use of undefined type %qs",
		   Gogo::unpack_hidden_name(no->name()).c_str());
	  this->warned_ = true;
	}
    }
  else if (no->is_type_declaration())
    {
      // The name was seen as a type, but the type was never defined.
      if (no->type_declaration_value()->using_type())
	{
	  error_at(this->named_object_->location(),
		   "use of undefined type %qs",
		   Gogo::unpack_hidden_name(no->name()).c_str());
	  this->warned_ = true;
	}
    }
  else
    {
      // The name was defined, but not as a type.
      if (!this->warned_)
	{
	  error_at(this->named_object_->location(), "expected type");
	  this->warned_ = true;
	}
    }
}

// Get the base type of a declaration.  This gives an error if the
// type has not yet been defined.

Type*
Forward_declaration_type::real_type()
{
  if (this->is_defined())
    return this->named_object()->type_value();
  else
    {
      this->warn();
      return Type::make_error_type();
    }
}

const Type*
Forward_declaration_type::real_type() const
{
  if (this->is_defined())
    return this->named_object()->type_value();
  else
    {
      this->warn();
      return Type::make_error_type();
    }
}

// Return whether the base type is defined.

bool
Forward_declaration_type::is_defined() const
{
  return this->named_object()->is_type();
}

// Add a method.  This is used when methods are defined before the
// type.

Named_object*
Forward_declaration_type::add_method(const std::string& name,
				     Function* function)
{
  Named_object* no = this->named_object();
  gcc_assert(no->is_type_declaration());
  return no->type_declaration_value()->add_method(name, function);
}

// Add a method declaration.  This is used when methods are declared
// before the type.

Named_object*
Forward_declaration_type::add_method_declaration(const std::string& name,
						 Function_type* type,
						 source_location location)
{
  Named_object* no = this->named_object();
  gcc_assert(no->is_type_declaration());
  Type_declaration* td = no->type_declaration_value();
  return td->add_method_declaration(name, type, location);
}

// Traversal.

int
Forward_declaration_type::do_traverse(Traverse* traverse)
{
  if (this->is_defined()
      && Type::traverse(this->real_type(), traverse) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  return TRAVERSE_CONTINUE;
}

// Get a tree for the type.

tree
Forward_declaration_type::do_get_tree(Gogo* gogo)
{
  if (this->is_defined())
    return this->real_type()->get_tree(gogo);

  if (this->warned_)
    return error_mark_node;

  // We represent an undefined type as a struct with no fields.  That
  // should work fine for the middle-end, since the same case can
  // arise in C.
  Named_object* no = this->named_object();
  tree type_tree = make_node(RECORD_TYPE);
  tree id = no->get_id(gogo);
  tree decl = build_decl(no->location(), TYPE_DECL, id, type_tree);
  TYPE_NAME(type_tree) = decl;
  return type_tree;
}

// The type descriptor.

void
Forward_declaration_type::do_type_descriptor_decl(Gogo* gogo, Named_type* name,
						  tree* pdecl)
{
  if (!this->is_defined())
    gogo->undefined_type_descriptor_decl(this, name, pdecl);
  else
    {
      Type* t = this->real_type();
      if (name != NULL)
	this->named_type_descriptor(gogo, t, name, pdecl);
      else
	{
	  tree descriptor = t->type_descriptor(gogo);
	  gcc_assert(TREE_CODE(descriptor) == ADDR_EXPR
		     && DECL_P(TREE_OPERAND(descriptor, 0)));
	  *pdecl = TREE_OPERAND(descriptor, 0);
	}
    }
}

// The reflection string.

void
Forward_declaration_type::do_reflection(Gogo* gogo, std::string* ret) const
{
  this->append_reflection(this->real_type(), gogo, ret);
}

// The mangled name.

void
Forward_declaration_type::do_mangled_name(Gogo* gogo, std::string* ret) const
{
  if (this->is_defined())
    this->append_mangled_name(this->real_type(), gogo, ret);
  else
    {
      const Named_object* no = this->named_object();
      std::string name;
      if (no->package() == NULL)
	name = gogo->package_name();
      else
	name = no->package()->name();
      name += '.';
      name += Gogo::unpack_hidden_name(no->name());
      char buf[20];
      snprintf(buf, sizeof buf, "N%u_",
	       static_cast<unsigned int>(name.length()));
      ret->append(buf);
      ret->append(name);
    }
}

// Export a forward declaration.  This can happen when a defined type
// refers to a type which is only declared (and is presumably defined
// in some other file in the same package).

void
Forward_declaration_type::do_export(Export*) const
{
  // If there is a base type, that should be exported instead of this.
  gcc_assert(!this->is_defined());

  // We don't output anything.
}

// Make a forward declaration.

Type*
Type::make_forward_declaration(Named_object* named_object)
{
  return new Forward_declaration_type(named_object);
}

// Class Typed_identifier_list.

// Sort the entries by name.

struct Typed_identifier_list_sort
{
 public:
  bool
  operator()(const Typed_identifier& t1, const Typed_identifier& t2) const
  { return t1.name() < t2.name(); }
};

void
Typed_identifier_list::sort_by_name()
{
  std::sort(this->entries_.begin(), this->entries_.end(),
	    Typed_identifier_list_sort());
}

// Traverse types.

int
Typed_identifier_list::traverse(Traverse* traverse)
{
  for (Typed_identifier_list::const_iterator p = this->begin();
       p != this->end();
       ++p)
    {
      if (Type::traverse(p->type(), traverse) == TRAVERSE_EXIT)
	return TRAVERSE_EXIT;
    }
  return TRAVERSE_CONTINUE;
}

// Copy the list.

Typed_identifier_list*
Typed_identifier_list::copy() const
{
  Typed_identifier_list* ret = new Typed_identifier_list();
  for (Typed_identifier_list::const_iterator p = this->begin();
       p != this->end();
       ++p)
    ret->push_back(Typed_identifier(p->name(), p->type(), p->location()));
  return ret;
}
