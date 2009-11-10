// types.h -- Go frontend types.     -*- C++ -*-

// Copyright 2009 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef GO_TYPES_H
#define GO_TYPES_H

#include <map>
#include <tr1/unordered_set>

class Gogo;
class Package;
class Traverse;
class Typed_identifier;
class Typed_identifier_list;
class Integer_type;
class Float_type;
class String_type;
class Function_type;
class Struct_field;
class Struct_field_list;
class Struct_type;
class Pointer_type;
class Array_type;
class Map_type;
class Channel_type;
class Interface_type;
class Named_type;
class Forward_declaration_type;
class Expression;
class Expression_list;
class Call_expression;
class Field_reference_expression;
class Bound_method_expression;
class Bindings;
class Named_object;
class Function;
class Translate_context;
class Export;
class Import;
class Refcounts;
class Refcount_entry;

// Type codes used in type descriptors.  These must match the values
// in libgo/runtime/go-type.h.

static const int RUNTIME_TYPE_CODE_BOOL = 0;
static const int RUNTIME_TYPE_CODE_FLOAT32 = 1;
static const int RUNTIME_TYPE_CODE_FLOAT64 = 2;
static const int RUNTIME_TYPE_CODE_FLOAT = 3;
static const int RUNTIME_TYPE_CODE_INT16 = 4;
static const int RUNTIME_TYPE_CODE_INT32 = 5;
static const int RUNTIME_TYPE_CODE_INT64 = 6;
static const int RUNTIME_TYPE_CODE_INT8 = 7;
static const int RUNTIME_TYPE_CODE_INT = 8;
static const int RUNTIME_TYPE_CODE_UINT16 = 9;
static const int RUNTIME_TYPE_CODE_UINT32 = 10;
static const int RUNTIME_TYPE_CODE_UINT64 = 11;
static const int RUNTIME_TYPE_CODE_UINT8 = 12;
static const int RUNTIME_TYPE_CODE_UINT = 13;
static const int RUNTIME_TYPE_CODE_STRING = 14;
static const int RUNTIME_TYPE_CODE_UINTPTR = 15;
static const int RUNTIME_TYPE_CODE_DOTDOTDOT = 16;
static const int RUNTIME_TYPE_CODE_UNSAFE_POINTER = 17;
static const int RUNTIME_TYPE_CODE_ARRAY = 18;
static const int RUNTIME_TYPE_CODE_SLICE = 19;
static const int RUNTIME_TYPE_CODE_CHAN = 20;
static const int RUNTIME_TYPE_CODE_FUNC = 21;
static const int RUNTIME_TYPE_CODE_INTERFACE = 22;
static const int RUNTIME_TYPE_CODE_MAP = 23;
static const int RUNTIME_TYPE_CODE_PTR = 24;
static const int RUNTIME_TYPE_CODE_STRUCT = 25;

// The base class for all types.

class Type
{
 public:
  // The types of types.
  enum Type_classification
  {
    TYPE_ERROR,
    TYPE_VOID,
    TYPE_BOOLEAN,
    TYPE_INTEGER,
    TYPE_FLOAT,
    TYPE_STRING,
    TYPE_SINK,
    TYPE_FUNCTION,
    TYPE_VARARGS,
    TYPE_POINTER,
    TYPE_NIL,
    TYPE_CALL_MULTIPLE_RESULT,
    TYPE_STRUCT,
    TYPE_ARRAY,
    TYPE_MAP,
    TYPE_CHANNEL,
    TYPE_INTERFACE,
    TYPE_NAMED,
    TYPE_FORWARD
  };

  virtual ~Type();

  // Creators.

  static Type*
  make_error_type();

  static Type*
  make_void_type();

  // Get the unnamed bool type.
  static Type*
  make_boolean_type();

  // Get the named type "bool".
  static Named_type*
  lookup_bool_type();

  // Make the named type "bool".
  static Named_type*
  make_named_bool_type();

  // Make an abstract integer type.
  static Integer_type*
  make_abstract_integer_type();

  // Make a named integer type with a specified size.
  // RUNTIME_TYPE_CODE is the code to use in reflection information,
  // to distinguish int and int32.
  static Named_type*
  make_integer_type(const char* name, bool is_unsigned, int bits,
		    int runtime_type_code);

  // Look up a named integer type.
  static Named_type*
  lookup_integer_type(const char* name);

  // Make an abstract floating point type.
  static Float_type*
  make_abstract_float_type();

  // Make a named floating point type with a specific size.
  // RUNTIME_TYPE_CODE is the code to use in reflection information,
  // to distinguish float and float32.
  static Named_type*
  make_float_type(const char* name, int bits, int runtime_type_code);

  // Look up a named float type.
  static Named_type*
  lookup_float_type(const char* name);

  // Get the unnamed string type.
  static Type*
  make_string_type();

  // Get the named type "string".
  static Named_type*
  lookup_string_type();

  // Make the named type "string".
  static Named_type*
  make_named_string_type();

  static Type*
  make_sink_type();

  static Function_type*
  make_function_type(Typed_identifier* receiver,
		     Typed_identifier_list* parameters,
		     Typed_identifier_list* results,
		     source_location);

  static Type*
  make_varargs_type();

  static Pointer_type*
  make_pointer_type(Type*);

  static Type*
  make_nil_type();

  static Type*
  make_call_multiple_result_type(Call_expression*);

  static Struct_type*
  make_struct_type(Struct_field_list* fields, source_location);

  static Array_type*
  make_array_type(Type* element_type, Expression* length);

  static Map_type*
  make_map_type(Type* key_type, Type* value_type, source_location);

  static Channel_type*
  make_channel_type(bool send, bool receive, Type*);

  static Interface_type*
  make_interface_type(Typed_identifier_list* methods, source_location);

  static Named_type*
  make_named_type(Named_object*, Type*, source_location);

  static Type*
  make_forward_declaration(Named_object*);

  // Traverse a type.
  static int
  traverse(Type*, Traverse*);

  // Verify the type.  This is called after parsing, and verifies that
  // types are complete and meet the language requirements.  This
  // returns false if the type is invalid.
  bool
  verify()
  { return this->do_verify(); }

  // Return true if two types are compatible for use in a binary
  // operation.  This is an equivalence relation.
  static bool
  are_compatible_for_binop(const Type* t1, const Type* t2);

  // Return true if a value with type RHS may be assigned to a value
  // with type LHS.  This also controls whether a value with type RHS
  // may be passed to a function which expects type LHS, and whether a
  // value with type RHS may be returned from a function which returns
  // type LHS.  This is not an equivalence relation.  If this returns
  // false, and REASON is not NULL, it sets *REASON to an optional
  // reason.
  static bool
  are_compatible_for_assign(const Type* lhs, const Type* rhs,
			    std::string* reason)
  { return Type::are_assignment_compatible(lhs, rhs, false, reason); }

  // Return true if a value with type RHS may be converted to a
  // valuewith type LHS.  If this returns false, and REASON is not
  // NULL, it sets *REASON to an optional reason.
  static bool
  are_compatible_for_conversion(const Type* lhs, const Type* rhs,
				std::string* reason)
  { return Type::are_assignment_compatible(lhs, rhs, true, reason); }

  // Return true if two types are compatible.
  static bool
  are_compatible(const Type* lhs, const Type* rhs)
  { return Type::are_compatible_for(lhs, rhs, COMPATIBLE_COMPATIBLE, NULL); }

  // Return true if two types are identical.
  static bool
  are_identical(const Type* lhs, const Type* rhs)
  { return Type::are_compatible_for(lhs, rhs, COMPATIBLE_IDENTICAL, NULL); }

  // Whether this type has any hidden fields which are not visible in
  // the current compilation, such as a field whose name begins with a
  // lower case letter in a struct imported from a different package.
  // WITHIN is not NULL if we are looking at fields in a named type.
  bool
  has_hidden_fields(const Named_type* within, std::string* reason) const;

  // Return a hash code for this type for the method hash table.
  // Types which are equivalent according to are_compatible_for_method
  // will have the same hash code.
  size_t
  hash_for_method() const;

  // Return the type classification.
  Type_classification
  classification() const
  { return this->classification_; }

  // Return the base type for this type.  This looks through forward
  // declarations and names.  Using this with a forward declaration
  // which has not been defined will return an error type.
  Type*
  base();

  const Type*
  base() const;

  // Return the type skipping defined forward declarations.  If this
  // type is a forward declaration which has not been defined, it will
  // return the Forward_declaration_type.  This differs from base() in
  // that it will return a Named_type, and for a
  // Forward_declaration_type which is not defined it will return that
  // type rather than an error type.
  Type*
  forwarded();

  const Type*
  forwarded() const;

  // Return true if this is a basic type: a type which is not composed
  // of other types, and is not void.
  bool
  is_basic_type() const;

  // Return true if this is an abstract type--an integer or floating
  // point type whose size has not been determined.
  bool
  is_abstract() const;

  // Return a non-abstract version of an abstract type.
  Type*
  make_non_abstract_type();

  // Return true if this type requires reference counting: if copying
  // or destroying a value of this type requires adjusting a reference
  // count.
  bool
  is_refcounted() const
  { return this->do_is_refcounted(); }

  // Return true if this type has some component which is reference
  // counted.  If IS_REFCOUNTED returns true for this type, then this
  // returns true as well.
  bool
  has_refcounted_component() const
  { return this->do_has_refcounted_component(); }

  // Add entries to the reference count queue to hold an object of
  // this type.  This normally adds a single entry but may add more
  // for structures.
  void
  add_refcount_queue_entries(Refcounts* refcounts, Refcount_entry* entry)
  { return this->do_add_refcount_queue_entries(refcounts, entry); }

  // Return true if this is an error type.  An error type indicates a
  // parsing error.
  bool
  is_error_type() const;

  // Return true if this is a void type.
  bool
  is_void_type() const
  { return this->classification_ == TYPE_VOID; }

  // If this is an integer type, return the Integer_type.  Otherwise,
  // return NULL.  This is a controlled dynamic_cast.
  Integer_type*
  integer_type()
  { return this->convert<Integer_type, TYPE_INTEGER>(); }

  const Integer_type*
  integer_type() const
  { return this->convert<const Integer_type, TYPE_INTEGER>(); }

  // If this is a floating point type, return the Float_type.
  // Otherwise, return NULL.  This is a controlled dynamic_cast.
  Float_type*
  float_type()
  { return this->convert<Float_type, TYPE_FLOAT>(); }

  const Float_type*
  float_type() const
  { return this->convert<const Float_type, TYPE_FLOAT>(); }

  // Return true if this is a boolean type.
  bool
  is_boolean_type() const
  { return this->base()->classification_ == TYPE_BOOLEAN; }

  // Return true if this is a string type.
  bool
  is_string_type() const
  { return this->base()->classification_ == TYPE_STRING; }

  // Return true if this is the sink type.  This is the type of the
  // blank identifier _.
  bool
  is_sink_type() const
  { return this->base()->classification_ == TYPE_SINK; }

  // If this is a function type, return it.  Otherwise, return NULL.
  Function_type*
  function_type()
  { return this->convert<Function_type, TYPE_FUNCTION>(); }

  const Function_type*
  function_type() const
  { return this->convert<const Function_type, TYPE_FUNCTION>(); }

  // If this is a pointer type, return the type to which it points.
  // Otherwise, return NULL.
  Type*
  points_to() const;

  // If this is a pointer type, return the type to which it points.
  // Otherwise, return the type itself.
  Type*
  deref()
  {
    Type* pt = this->points_to();
    return pt != NULL ? pt : this;
  }

  const Type*
  deref() const
  {
    const Type* pt = this->points_to();
    return pt != NULL ? pt : this;
  }

  // Return true if this is the varargs type.  We don't use base()
  // here, because this can be called during parse, and there is no
  // way to name the varargs type anyhow.
  bool
  is_varargs_type() const
  { return this->classification_ == TYPE_VARARGS; }

  // Return true if this is the nil type.  We don't use base() here,
  // because this can be called during parse, and there is no way to
  // name the nil type anyhow.
  bool
  is_nil_type() const
  { return this->classification_ == TYPE_NIL; }

  // Return true if this is the predeclared constant nil being used as
  // a type.  This is what the parser produces for type switches which
  // use "case nil".
  bool
  is_nil_constant_as_type() const;

  // Return true if this is the return type of a function which
  // returns multiple values.
  bool
  is_call_multiple_result_type() const
  { return this->base()->classification_ == TYPE_CALL_MULTIPLE_RESULT; }

  // If this is a struct type, return it.  Otherwise, return NULL.
  Struct_type*
  struct_type()
  { return this->convert<Struct_type, TYPE_STRUCT>(); }

  const Struct_type*
  struct_type() const
  { return this->convert<const Struct_type, TYPE_STRUCT>(); }

  // If this is an array type, return it.  Otherwise, return NULL.
  Array_type*
  array_type()
  { return this->convert<Array_type, TYPE_ARRAY>(); }

  const Array_type*
  array_type() const
  { return this->convert<const Array_type, TYPE_ARRAY>(); }

  // Return whether if this is an open array type.
  bool
  is_open_array_type() const;

  // If this is a map type, return it.  Otherwise, return NULL.
  Map_type*
  map_type()
  { return this->convert<Map_type, TYPE_MAP>(); }

  const Map_type*
  map_type() const
  { return this->convert<const Map_type, TYPE_MAP>(); }

  // If this is a channel type, return it.  Otherwise, return NULL.
  Channel_type*
  channel_type()
  { return this->convert<Channel_type, TYPE_CHANNEL>(); }

  const Channel_type*
  channel_type() const
  { return this->convert<const Channel_type, TYPE_CHANNEL>(); }

  // If this is an interface type, return it.  Otherwise, return NULL.
  Interface_type*
  interface_type()
  { return this->convert<Interface_type, TYPE_INTERFACE>(); }

  const Interface_type*
  interface_type() const
  { return this->convert<const Interface_type, TYPE_INTERFACE>(); }

  // If this is a named type, return it.  Otherwise, return NULL.
  Named_type*
  named_type();

  const Named_type*
  named_type() const;

  // If this is a forward declaration, return it.  Otherwise, return
  // NULL.
  Forward_declaration_type*
  forward_declaration_type()
  { return this->convert_no_base<Forward_declaration_type, TYPE_FORWARD>(); }

  const Forward_declaration_type*
  forward_declaration_type() const
  {
    return this->convert_no_base<const Forward_declaration_type,
				 TYPE_FORWARD>();
  }

  // Return true if this type is not yet defined.
  bool
  is_undefined() const;

  // Return true if this is the unsafe.pointer type.  We currently
  // represent that as pointer-to-void.
  bool
  is_unsafe_pointer_type() const
  { return this->points_to() != NULL && this->points_to()->is_void_type(); }

  // This type was passed to the builtin function make.  ARGS are the
  // arguments passed to make after the type; this may be NULL if
  // there were none.  Issue any required errors.
  bool
  check_make_expression(Expression_list* args, source_location location)
  { return this->do_check_make_expression(args, location); }

  // Return a tree representing this type.
  tree
  get_tree(Gogo*);

  // Return a tree representing a zero initialization for this type.
  // This will be something like an INTEGER_CST or a CONSTRUCTOR.  If
  // IS_CLEAR is true, then the memory is known to be zeroed; in that
  // case, this will return NULL if there is nothing to be done.
  tree
  get_init_tree(Gogo*, bool is_clear);

  // Return a tree for a make expression applied to this type.
  tree
  make_expression_tree(Translate_context* context, Expression_list* args,
		       source_location location)
  { return this->do_make_expression_tree(context, args, location); }

  // Return a tree copying VAL, a value of this type, into the
  // reference count queue at ENTRY.  This modifies ENTRY.
  tree
  set_refcount_queue_entry(Gogo* gogo, Refcounts* refcounts,
			   Refcount_entry* entry, tree val) const;

  // Build a type descriptor entry for this type.  Return a pointer to
  // it.
  tree
  type_descriptor(Gogo* gogo);

  // Return the type reflection string for this type.
  std::string
  reflection(Gogo*) const;

  // Return a mangled name for the type.  This is a name which can be
  // used in assembler code.  Types which compare as equal according
  // to COMPATIBLE_SAME should have the same manged name.
  std::string
  mangled_name(Gogo*) const;

  // Export the type.
  void
  export_type(Export* exp) const
  { this->do_export(exp); }

  // Import a type.
  static Type*
  import_type(Import*);

 protected:
  Type(Type_classification);

  // Different sorts of type compatibility.
  enum Type_compatible
  {
    // Identical types.
    COMPATIBLE_IDENTICAL,
    // Compatible types.
    COMPATIBLE_COMPATIBLE,
    // Compatible for conversion.
    COMPATIBLE_FOR_CONVERSION
  };

  // Return whether types are compatible.
  static bool
  are_compatible_for(const Type*, const Type*, Type_compatible,
		     std::string* reason);

  // Return whether types are assignment compatible.
  static bool
  are_assignment_compatible(const Type*, const Type*, bool is_conversion,
			    std::string* reason);

  // Functions implemented by the child class.

  // Traverse the subtypes.
  virtual int
  do_traverse(Traverse*);

  // Verify the type.
  virtual bool
  do_verify()
  { return true; }

  virtual bool
  do_is_refcounted() const
  { return false; }

  virtual bool
  do_has_refcounted_component() const
  { return this->do_is_refcounted(); }

  virtual void
  do_add_refcount_queue_entries(Refcounts* refcounts, Refcount_entry* entry);

  virtual size_t
  do_hash_for_method() const;

  virtual bool
  do_check_make_expression(Expression_list* args, source_location);


  virtual tree
  do_get_tree(Gogo*) = 0;

  virtual tree
  do_init_tree(Gogo*, bool) = 0;

  virtual tree
  do_make_expression_tree(Translate_context*, Expression_list*,
			  source_location);

  virtual tree
  do_set_refcount_queue_entry(Gogo* gogo, Refcounts* refcounts,
			      Refcount_entry* entry, tree val) const;

  virtual void
  do_type_descriptor_decl(Gogo* gogo, Named_type* name, tree* pdecl) = 0;

  virtual void
  do_reflection(Gogo*, std::string*) const = 0;


  virtual void
  do_mangled_name(Gogo*, std::string*) const = 0;

  virtual void
  do_export(Export*) const;

  // Build a type descriptor entry for TYPE, using NAME as the name of
  // the type.  PACKAGE is the package where TYPE is defined, or NULL
  // if defined in the package currently being compiled.  Store the
  // decl in *PDECL.
  void
  named_type_descriptor(Gogo* gogo, Type* type, Named_type* name, tree* pdecl);

  // For the benefit of child class reflection string generation.
  void
  append_reflection(const Type* type, Gogo* gogo, std::string* ret) const
  { type->do_reflection(gogo, ret); }

  // For the benefit of child class mangling.
  void
  append_mangled_name(const Type* type, Gogo* gogo, std::string* ret) const
  { type->do_mangled_name(gogo, ret); }

  // Store the type tree during construction.
  void
  set_incomplete_type_tree(tree);

 private:
  // Convert to the desired type classification, or return NULL.  This
  // is a controlled dynamic_cast.
  template<typename Type_class, Type_classification type_classification>
  Type_class*
  convert()
  {
    Type* base = this->base();
    return (base->classification_ == type_classification
	    ? static_cast<Type_class*>(base)
	    : NULL);
  }

  template<typename Type_class, Type_classification type_classification>
  const Type_class*
  convert() const
  {
    const Type* base = this->base();
    return (base->classification_ == type_classification
	    ? static_cast<Type_class*>(base)
	    : NULL);
  }

  template<typename Type_class, Type_classification type_classification>
  Type_class*
  convert_no_base()
  {
    return (this->classification_ == type_classification
	    ? static_cast<Type_class*>(this)
	    : NULL);
  }

  template<typename Type_class, Type_classification type_classification>
  const Type_class*
  convert_no_base() const
  {
    return (this->classification_ == type_classification
	    ? static_cast<Type_class*>(this)
	    : NULL);
  }

  static tree
  build_receive_return_type(tree type);

  tree
  inc_or_dec_refcount(Gogo* gogo, tree expr_tree, source_location,
		      bool is_local, bool is_increment);

  // The type classification.
  Type_classification classification_;
  // The tree representation of the type, once it has been determined.
  tree tree_;
  // The decl for the type descriptor for this type.  This starts out
  // as NULL and is filled in as needed.
  tree type_descriptor_decl_;
};

// Type hash table operations.

class Type_hash
{
 public:
  size_t
  operator()(const Type* type) const
  { return type->hash_for_method(); }
};

class Type_identical
{
 public:
  bool
  operator()(const Type* t1, const Type* t2) const
  { return Type::are_identical(t1, t2); }
};

// An identifier with a type.

class Typed_identifier
{
 public:
  Typed_identifier(const std::string& name, Type* type,
		   source_location location)
    : name_(name), type_(type), location_(location)
  { }

  // Get the name.
  const std::string&
  name() const
  { return this->name_; }

  // Get the type.
  Type*
  type() const
  { return this->type_; }

  // Return the location where the name was seen.  This is not always
  // meaningful.
  source_location
  location() const
  { return this->location_; }

  // Set the type--sometimes we see the identifier before the type.
  void
  set_type(Type* type)
  {
    gcc_assert(this->type_ == NULL || type->is_error_type());
    this->type_ = type;
  }

 private:
  // Identifier name.
  std::string name_;
  // Type.
  Type* type_;
  // The location where the name was seen.
  source_location location_;
};

// A list of Typed_identifiers.

class Typed_identifier_list
{
 public:
  Typed_identifier_list()
    : entries_()
  { }

  // Whether the list is empty.
  bool
  empty() const
  { return this->entries_.empty(); }

  // Return the number of entries in the list.
  size_t
  size() const
  { return this->entries_.size(); }

  // Add an entry to the end of the list.
  void
  push_back(const Typed_identifier& td)
  { this->entries_.push_back(td); }

  // Remove an entry from the end of the list.
  void
  pop_back()
  { this->entries_.pop_back(); }

  // Set the type of entry I to TYPE.
  void
  set_type(size_t i, Type* type)
  {
    gcc_assert(i < this->entries_.size());
    this->entries_[i].set_type(type);
  }

  // Sort the entries by name.
  void
  sort_by_name();

  // Traverse types.
  int
  traverse(Traverse*);

  // Return the first and last elements.
  Typed_identifier&
  front()
  { return this->entries_.front(); }

  const Typed_identifier&
  front() const
  { return this->entries_.front(); }

  Typed_identifier&
  back()
  { return this->entries_.back(); }

  const Typed_identifier&
  back() const
  { return this->entries_.back(); }

  const Typed_identifier&
  at(size_t i) const
  { return this->entries_.at(i); }

  void
  set(size_t i, const Typed_identifier& t)
  { this->entries_.at(i) = t; }

  void
  resize(size_t c)
  {
    gcc_assert(c <= this->entries_.size());
    this->entries_.resize(c, Typed_identifier("", NULL, UNKNOWN_LOCATION));
  }

  // Iterators.

  typedef std::vector<Typed_identifier>::iterator iterator;
  typedef std::vector<Typed_identifier>::const_iterator const_iterator;

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

  // Return a copy of this list.  This returns an independent copy of
  // the vector, but does not copy the types.
  Typed_identifier_list*
  copy() const;

 private:
  std::vector<Typed_identifier> entries_;
};

// The type of an integer.

class Integer_type : public Type
{
 public:
  // Create a new integer type.
  static Named_type*
  create_integer_type(const char* name, bool is_unsigned, int bits,
		      int runtime_type_code);

  // Look up an existing integer type.
  static Named_type*
  lookup_integer_type(const char* name);

  // Create an abstract integer type.
  static Integer_type*
  create_abstract_integer_type();

  // Whether this is an abstract integer type.
  bool
  is_abstract() const
  { return this->is_abstract_; }

  // Whether this is an unsigned type.
  bool
  is_unsigned() const
  { return this->is_unsigned_; }

  // The number of bits.
  int
  bits() const
  { return this->bits_; }

  // Whether this type is the same as T.
  bool
  is_compatible(const Integer_type* t) const;

 protected:
  size_t
  do_hash_for_method() const;

  tree
  do_get_tree(Gogo*);

  tree
  do_init_tree(Gogo*, bool);

  void
  do_type_descriptor_decl(Gogo*, Named_type*, tree*);

  void
  do_reflection(Gogo*, std::string*) const;

  void
  do_mangled_name(Gogo*, std::string*) const;

 private:
  Integer_type(bool is_abstract, bool is_unsigned, int bits,
	       int runtime_type_code)
    : Type(TYPE_INTEGER),
      is_abstract_(is_abstract), is_unsigned_(is_unsigned), bits_(bits),
      runtime_type_code_(runtime_type_code)
  { }

  // Map names of integer types to the types themselves.
  typedef std::map<std::string, Named_type*> Named_integer_types;
  static Named_integer_types named_integer_types;

  // True if this is an abstract type.
  bool is_abstract_;
  // True if this is an unsigned type.
  bool is_unsigned_;
  // The number of bits.
  int bits_;
  // The runtime type code used in the type descriptor for this type.
  int runtime_type_code_;
};

// The type of a floating point number.

class Float_type : public Type
{
 public:
  // Create a new float type.
  static Named_type*
  create_float_type(const char* name, int bits, int runtime_type_code);

  // Look up an existing float type.
  static Named_type*
  lookup_float_type(const char* name);

  // Create an abstract float type.
  static Float_type*
  create_abstract_float_type();

  // Whether this is an abstract float type.
  bool
  is_abstract() const
  { return this->is_abstract_; }

  // The number of bits.
  int
  bits() const
  { return this->bits_; }

  // Whether this type is the same as T.
  bool
  is_compatible(const Float_type* t) const;

  // Return a tree for this type without using a Gogo*.
  tree
  type_tree() const;

 protected:
  size_t
  do_hash_for_method() const;

  tree
  do_get_tree(Gogo*);

  tree
  do_init_tree(Gogo*, bool);

  void
  do_type_descriptor_decl(Gogo*, Named_type*, tree*);

  void
  do_reflection(Gogo*, std::string*) const;

  void
  do_mangled_name(Gogo*, std::string*) const;

 private:
  Float_type(bool is_abstract, int bits, int runtime_type_code)
    : Type(TYPE_FLOAT),
      is_abstract_(is_abstract), bits_(bits),
      runtime_type_code_(runtime_type_code)
  { }

  // Map names of float types to the types themselves.
  typedef std::map<std::string, Named_type*> Named_float_types;
  static Named_float_types named_float_types;

  // True if this is an abstract type.
  bool is_abstract_;
  // The number of bits in the floating point value.
  int bits_;
  // The runtime type code used in the type descriptor for this type.
  int runtime_type_code_;
};

// The type of a string.

class String_type : public Type
{
 public:
  String_type()
    : Type(TYPE_STRING)
  { }

  // Return a tree for the length of STRING.
  static tree
  length_tree(Gogo*, tree string);

  // Return a tree which points to the bytes of STRING.
  static tree
  bytes_tree(Gogo*, tree string);

 protected:
  bool
  do_is_refcounted() const
  { return true; }

  tree
  do_get_tree(Gogo*);

  tree
  do_init_tree(Gogo* gogo, bool);

  void
  do_type_descriptor_decl(Gogo*, Named_type*, tree*);

  void
  do_reflection(Gogo*, std::string*) const;

  void
  do_mangled_name(Gogo*, std::string* ret) const;

 private:
  // The named string type.
  static Named_type* string_type_;
};

// The type of a function.

class Function_type : public Type
{
 public:
  Function_type(Typed_identifier* receiver, Typed_identifier_list* parameters,
		Typed_identifier_list* results, source_location location)
    : Type(TYPE_FUNCTION),
      receiver_(receiver), parameters_(parameters), results_(results),
      location_(location), is_varargs_(false), is_builtin_(false)
  { }

  // Get the receiver.
  const Typed_identifier*
  receiver() const
  { return this->receiver_; }

  // Get the return names and types.
  const Typed_identifier_list*
  results() const
  { return this->results_; }

  // Get the parameter names and types.
  const Typed_identifier_list*
  parameters() const
  { return this->parameters_; }

  // Whether this is a varargs function.
  bool
  is_varargs() const
  { return this->is_varargs_; }

  // Whether this is a builtin function.
  bool
  is_builtin() const
  { return this->is_builtin_; }

  // The location where this type was defined.
  source_location
  location() const
  { return this->location_; }

  // Return whether this is a method type.
  bool
  is_method() const
  { return this->receiver_ != NULL; }

  // Whether T is a valid redeclaration of this type.  This is called
  // when a function is declared more than once.
  bool
  is_valid_redeclaration(const Function_type* t, std::string*) const;

  // Whether this type is the same as T.
  bool
  is_compatible(const Function_type* t, Type_compatible,
		bool ignore_receiver, std::string*) const;

  // Record that this is a varargs function.
  void
  set_is_varargs()
  { this->is_varargs_ = true; }

  // Record that this is a builtin function.
  void
  set_is_builtin()
  { this->is_builtin_ = true; }

  // Import a function type.
  static Function_type*
  do_import(Import*);

  // Return the real type to use for the varargs parameter.
  static Type*
  varargs_type();

 protected:
  int
  do_traverse(Traverse*);

  // Function types are really pointers, and they are reference
  // counted in case that pointer points to a trampoline.
  bool
  do_is_refcounted() const
  { return true; }

  size_t
  do_hash_for_method() const;

  tree
  do_get_tree(Gogo*);

  tree
  do_init_tree(Gogo*, bool);

  void
  do_type_descriptor_decl(Gogo*, Named_type*, tree*);

  void
  do_reflection(Gogo*, std::string*) const;

  void
  do_mangled_name(Gogo*, std::string*) const;

  void
  do_export(Export*) const;

 private:
  // The receiver name and type.  This will be NULL for a normal
  // function, non-NULL for a method.
  Typed_identifier* receiver_;
  // The parameter names and types.
  Typed_identifier_list* parameters_;
  // The result names and types.  This will be NULL if no result was
  // specified.
  Typed_identifier_list* results_;
  // The location where this type was defined.  This exists solely to
  // give a location for the fields of the struct if this function
  // returns multiple values.
  source_location location_;
  // Whether this function takes a variable number of arguments.
  bool is_varargs_;
  // Whether this is a special builtin function which can not simply
  // be called.  This is used for len, cap, etc.
  bool is_builtin_;
};

// The type of a pointer.

class Pointer_type : public Type
{
 public:
  Pointer_type(Type* to_type)
    : Type(TYPE_POINTER),
      to_type_(to_type)
  {}

  Type*
  points_to() const
  { return this->to_type_; }

  // Import a pointer type.
  static Pointer_type*
  do_import(Import*);

 protected:
  int
  do_traverse(Traverse*);

  bool
  do_is_refcounted() const
  { return !this->is_unsafe_pointer_type(); }

  size_t
  do_hash_for_method() const;

  tree
  do_get_tree(Gogo*);

  tree
  do_init_tree(Gogo*, bool);

  void
  do_type_descriptor_decl(Gogo*, Named_type*, tree*);

  void
  do_reflection(Gogo*, std::string*) const;

  void
  do_mangled_name(Gogo*, std::string*) const;

  void
  do_export(Export*) const;

 private:
  // The type to which this type points.
  Type* to_type_;
};

// The type of a field in a struct.

class Struct_field
{
 public:
  explicit Struct_field(const Typed_identifier& typed_identifier)
    : typed_identifier_(typed_identifier), tag_(NULL)
  { }

  // The field name.
  const std::string&
  field_name() const;

  // The field type.
  Type*
  type() const
  { return this->typed_identifier_.type(); }

  // The field location.
  source_location
  location() const
  { return this->typed_identifier_.location(); }

  // Whether the field has a tag.
  bool
  has_tag() const
  { return this->tag_ != NULL; }

  // The tag.
  const std::string&
  tag() const
  {
    gcc_assert(this->tag_ != NULL);
    return *this->tag_;
  }

  // Whether this is an anonymous field.
  bool
  is_anonymous() const
  { return this->typed_identifier_.name().empty(); }

  // Set the tag.  FIXME: This is never freed.
  void
  set_tag(const std::string& tag)
  { this->tag_ = new std::string(tag); }

  // Set the type.  This is only used in error cases.
  void
  set_type(Type* type)
  { this->typed_identifier_.set_type(type); }

 private:
  // The field name, type, and location.
  Typed_identifier typed_identifier_;
  // The field tag.  This is NULL if the field has no tag.
  std::string* tag_;
};

// A list of struct fields.

class Struct_field_list
{
 public:
  Struct_field_list()
    : entries_()
  { }

  // Whether the list is empty.
  bool
  empty() const
  { return this->entries_.empty(); }

  // Return the number of entries.
  size_t
  size() const
  { return this->entries_.size(); }

  // Add an entry to the end of the list.
  void
  push_back(const Struct_field& sf)
  { this->entries_.push_back(sf); }

  // Index into the list.
  const Struct_field&
  at(size_t i) const
  { return this->entries_.at(i); }

  // Last entry in list.
  Struct_field&
  back()
  { return this->entries_.back(); }

  // Iterators.

  typedef std::vector<Struct_field>::iterator iterator;
  typedef std::vector<Struct_field>::const_iterator const_iterator;

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

 private:
  std::vector<Struct_field> entries_;
};

// The type of a struct.

class Struct_type : public Type
{
 public:
  Struct_type(Struct_field_list* fields, source_location location)
    : Type(TYPE_STRUCT),
      fields_(fields), location_(location)
  { }

  // Return the field NAME.  This only looks at local fields, not at
  // embedded types.  If the field is found, and PINDEX is not NULL,
  // this sets *PINDEX to the field index.  If the field is not found,
  // this returns NULL.
  const Struct_field*
  find_local_field(const std::string& name, unsigned int *pindex) const;

  // Return the field number INDEX.
  const Struct_field*
  field(unsigned int index) const
  { return &this->fields_->at(index); }

  // Get the struct fields.
  const Struct_field_list*
  fields() const
  { return this->fields_; }

  // Return the number of fields.
  size_t
  field_count() const
  { return this->fields_->size(); }

  // Push a new field onto the end of the struct.  This is used when
  // building a closure variable.
  void
  push_field(const Struct_field& sf)
  { this->fields_->push_back(sf); }

  // Return an expression referring to field NAME in STRUCT_EXPR, or
  // NULL if there is no field with that name.
  Field_reference_expression*
  field_reference(Expression* struct_expr, const std::string& name,
		  source_location) const;

  // Return the total number of fields, including embedded fields.
  // This is the number of values which can appear in a conversion to
  // this type.
  unsigned int
  total_field_count() const;

  // Whether this type is compatible with T.
  bool
  is_compatible(const Struct_type* t, Type_compatible) const;

  // Whether this struct type has any hidden fields.  This returns
  // true if any fields have hidden names, or if any non-pointer
  // anonymous fields have types with hidden fields.
  bool
  struct_has_hidden_fields(const Named_type* within, std::string*) const;

  // Import a struct type.
  static Struct_type*
  do_import(Import*);

 protected:
  int
  do_traverse(Traverse*);

  bool
  do_verify();

  bool
  do_has_refcounted_component() const;

  void
  do_add_refcount_queue_entries(Refcounts*, Refcount_entry*);

  size_t
  do_hash_for_method() const;

  tree
  do_get_tree(Gogo*);

  tree
  do_init_tree(Gogo*, bool);

  tree
  do_set_refcount_queue_entry(Gogo* gogo, Refcounts* refcounts,
			      Refcount_entry* entry, tree val) const;

  void
  do_type_descriptor_decl(Gogo*, Named_type*, tree*);

  void
  do_reflection(Gogo*, std::string*) const;

  void
  do_mangled_name(Gogo*, std::string*) const;

  void
  do_export(Export*) const;

 private:
  Field_reference_expression*
  field_reference_depth(Expression* struct_expr, const std::string& name,
			source_location, unsigned int* depth) const;

  // The fields of the struct.
  Struct_field_list* fields_;
  // The place where the struct was declared.
  source_location location_;
};

// The type of an array.

class Array_type : public Type
{
 public:
  Array_type(Type* element_type, Expression* length)
    : Type(TYPE_ARRAY),
      element_type_(element_type), length_(length), length_tree_(NULL)
  { }

  // Return the element type.
  Type*
  element_type() const
  { return this->element_type_; }

  // Return the length.  This will return NULL for an open array.
  Expression*
  length() const
  { return this->length_; }

  // Whether this type is compatible with T.
  bool
  is_compatible(const Array_type* t, Type_compatible) const;

  // Whether this type has any hidden fields.
  bool
  array_has_hidden_fields(const Named_type* within, std::string* reason) const
  { return this->element_type_->has_hidden_fields(within, reason); }

  // Return a tree for the pointer to the values in an array.
  tree
  value_pointer_tree(Gogo*, tree array) const;

  // Return a tree for the length of an array with this type.
  tree
  length_tree(Gogo*, tree array);

  // Return a tree for the capacity of an array with this type.
  tree
  capacity_tree(Gogo*, tree array);

  // Import an array type.
  static Array_type*
  do_import(Import*);

 protected:
  int
  do_traverse(Traverse* traverse);

  bool
  do_is_refcounted() const
  {
    return (this->length_ == NULL
	    || this->element_type_->has_refcounted_component());
  }

  size_t
  do_hash_for_method() const;

  bool
  do_check_make_expression(Expression_list*, source_location);

  tree
  do_get_tree(Gogo*);

  tree
  do_init_tree(Gogo*, bool);

  tree
  do_make_expression_tree(Translate_context*, Expression_list*,
			  source_location);

  tree
  do_set_refcount_queue_entry(Gogo* gogo, Refcounts* refcounts,
			      Refcount_entry* entry, tree val) const;

  void
  do_type_descriptor_decl(Gogo*, Named_type*, tree*);

  void
  do_reflection(Gogo*, std::string*) const;

  void
  do_mangled_name(Gogo*, std::string*) const;

  void
  do_export(Export*) const;

 private:
  tree
  get_length_tree(Gogo*);

  // A mapping from Type to tree, used to ensure that arrays of
  // identical types are identical.
  typedef std::tr1::unordered_map<const Type*, tree, Type_hash,
				  Type_identical> Array_trees;

  static Array_trees array_trees;

  // The type of elements of the array.
  Type* element_type_;
  // The number of elements.  This may be NULL.
  Expression* length_;
  // The length as a tree.  We only want to compute this once.
  tree length_tree_;
};

// The type of a map.

class Map_type : public Type
{
 public:
  Map_type(Type* key_type, Type* val_type, source_location location)
    : Type(TYPE_MAP),
      key_type_(key_type), val_type_(val_type), location_(location)
  { }

  // Return the key type.
  Type*
  key_type() const
  { return this->key_type_; }

  // Return the value type.
  Type*
  val_type() const
  { return this->val_type_; }

  // Whether this type is compatible with T.
  bool
  is_compatible(const Map_type* t, Type_compatible) const;

  // Import a map type.
  static Map_type*
  do_import(Import*);

 protected:
  int
  do_traverse(Traverse*);

  bool
  do_verify();

  bool
  do_is_refcounted() const
  { return true; }

  size_t
  do_hash_for_method() const;

  bool
  do_check_make_expression(Expression_list*, source_location);

  tree
  do_get_tree(Gogo*);

  tree
  do_init_tree(Gogo*, bool);

  tree
  do_make_expression_tree(Translate_context*, Expression_list*,
			  source_location);

  void
  do_type_descriptor_decl(Gogo*, Named_type*, tree*);

  void
  do_reflection(Gogo*, std::string*) const;

  void
  do_mangled_name(Gogo*, std::string*) const;

  void
  do_export(Export*) const;

 private:
  // The key type.
  Type* key_type_;
  // The value type.
  Type* val_type_;
  // Where the type was defined.
  source_location location_;
};

// The type of a channel.

class Channel_type : public Type
{
 public:
  Channel_type(bool may_send, bool may_receive, Type* element_type)
    : Type(TYPE_CHANNEL),
      may_send_(may_send), may_receive_(may_receive),
      element_type_(element_type)
  { gcc_assert(may_send || may_receive); }

  // Whether this channel can send data.
  bool
  may_send() const
  { return this->may_send_; }

  // Whether this channel can receive data.
  bool
  may_receive() const
  { return this->may_receive_; }

  // The type of the values that may be sent on this channel.  This is
  // NULL if any type may be sent.
  Type*
  element_type() const
  { return this->element_type_; }

  // Whether this type is compatible with T.
  bool
  is_compatible(const Channel_type* t, Type_compatible) const;

  // Import a channel type.
  static Channel_type*
  do_import(Import*);

 protected:
  int
  do_traverse(Traverse* traverse)
  { return Type::traverse(this->element_type_, traverse); }

  bool
  do_is_refcounted() const
  { return true; }

  size_t
  do_hash_for_method() const;

  bool
  do_check_make_expression(Expression_list*, source_location);

  tree
  do_get_tree(Gogo*);

  tree
  do_init_tree(Gogo*, bool);

  tree
  do_make_expression_tree(Translate_context*, Expression_list*,
			  source_location);

  void
  do_type_descriptor_decl(Gogo*, Named_type*, tree*);

  void
  do_reflection(Gogo*, std::string*) const;

  void
  do_mangled_name(Gogo*, std::string*) const;

  void
  do_export(Export*) const;

 private:
  // Whether this channel can send data.
  bool may_send_;
  // Whether this channel can receive data.
  bool may_receive_;
  // The types of elements which may be sent on this channel.  If this
  // is NULL, it means that any type may be sent.
  Type* element_type_;
};

// An interface type.

class Interface_type : public Type
{
 public:
  Interface_type(Typed_identifier_list* methods, source_location location)
    : Type(TYPE_INTERFACE),
      methods_(methods), location_(location)
  { }

  // Return the list of methods.
  const Typed_identifier_list*
  methods() const
  { return this->methods_; }

  // Return the number of methods.
  size_t
  method_count() const
  { return this->methods_->size(); }

  // Return the method NAME, or NULL.
  const Typed_identifier*
  find_method(const std::string& name) const;

  // Return the zero-based index of method NAME.
  size_t
  method_index(const std::string& name) const;

  // Finalize the methods.  This handles interface inheritance.
  void
  finalize_methods();

  // Return true if T implements this interface.  If this returns
  // false, and REASON is not NULL, it sets *REASON to the reason that
  // it fails.
  bool
  implements_interface(const Type* t, std::string* reason) const;

  // Whether this type is compatible with T.  REASON is as in
  // implements_interface.
  bool
  is_compatible(const Interface_type* t, Type_compatible) const;

  // Whether we can assign T to this type.  is_compatible is known to
  // be false.
  bool
  is_compatible_for_assign(const Interface_type*, std::string* reason) const;

  // Import an interface type.
  static Interface_type*
  do_import(Import*);

 protected:
  int
  do_traverse(Traverse*);

  bool
  do_is_refcounted() const
  { return true; }

  size_t
  do_hash_for_method() const;

  tree
  do_get_tree(Gogo*);

  tree
  do_init_tree(Gogo* gogo, bool);

  void
  do_type_descriptor_decl(Gogo*, Named_type*, tree*);

  void
  do_reflection(Gogo*, std::string*) const;

  void
  do_mangled_name(Gogo*, std::string*) const;

  void
  do_export(Export*) const;

 private:
  // The list of methods associated with the interface.
  Typed_identifier_list* methods_;
  // The location where the interface was defined.
  source_location location_;
};

// To build the complete list of methods for a named type we need to
// gather all methods from anonymous fields.  Those methods may
// require an arbitrary set of indirections and field offsets.  There
// is also the possibility of ambiguous methods, which we could ignore
// except that we want to give a better error message for that case.
// This is a base class.  There are two types of methods: named
// methods, and methods which are inherited from an anonymous field of
// interface type.

class Method
{
 public:
  // For methods in anonymous types we need to know the sequence of
  // field references used to extract the pointer to pass to the
  // method.  Since each method for a particular anonymous field will
  // have the sequence of field indexes, and since the indexes can be
  // shared going down the chain, we use a manually managed linked
  // list.  The first entry in the list is the field index for the
  // last field, the one passed to the method.

  struct Field_indexes
  {
    const Field_indexes* next;
    unsigned int field_index;
  };

  virtual ~Method()
  { }

  // Get the list of field indexes.
  const Field_indexes*
  field_indexes() const
  { return this->field_indexes_; }

  // Get the depth.
  unsigned int
  depth() const
  { return this->depth_; }

  // Return whether this is a value method--a method which does not
  // require a pointer expression.
  bool
  is_value_method() const
  { return this->is_value_method_; }

  // Return whether we need a stub method--this is true if we can't
  // just pass the main object to the method.
  bool
  needs_stub_method() const
  { return this->needs_stub_method_; }

  // Return whether this is an ambiguous method name.
  bool
  is_ambiguous() const
  { return this->is_ambiguous_; }

  // Note that this method is ambiguous.
  void
  set_is_ambiguous()
  { this->is_ambiguous_ = true; }

  // Return the type of the method.
  Function_type*
  type() const
  { return this->do_type(); }

  // Return the location of the method receiver.
  source_location
  receiver_location() const
  { return this->do_receiver_location(); }

  // Return an expression which binds this method to EXPR.  This is
  // something which can be used with a function call.
  Expression*
  bind_method(Expression* expr, source_location location) const;

  // Return the named object for this method.  This may only be called
  // after methods are finalized.
  Named_object*
  named_object() const;

  // Set the stub object.
  void
  set_stub_object(Named_object* no)
  {
    gcc_assert(this->stub_ == NULL);
    this->stub_ = no;
  }

 protected:
  // These objects are only built by the child classes.
  Method(const Field_indexes* field_indexes, unsigned int depth,
	 bool is_value_method, bool needs_stub_method)
    : field_indexes_(field_indexes), depth_(depth), stub_(NULL),
      is_value_method_(is_value_method), needs_stub_method_(needs_stub_method),
      is_ambiguous_(false)
  { }

  // The named object for this method.
  virtual Named_object*
  do_named_object() const = 0;

  // The type of the method.
  virtual Function_type*
  do_type() const = 0;

  // Return the location of the method receiver.
  virtual source_location
  do_receiver_location() const = 0;

  // Bind a method to an object.
  virtual Expression*
  do_bind_method(Expression* expr, source_location location) const = 0;

 private:
  // The sequence of field indexes used for this method.  If this is
  // NULL, then the method is defined for the current type.
  const Field_indexes* field_indexes_;
  // The depth at which this method was found.
  unsigned int depth_;
  // If a stub method is required, this is its object.  This is only
  // set after stub methods are built in finalize_methods.
  Named_object* stub_;
  // Whether this is a value method--a method that does not require a
  // pointer.
  bool is_value_method_;
  // Whether a stub method is required.
  bool needs_stub_method_;
  // Whether this method is ambiguous.
  bool is_ambiguous_;
};

// A named method.  This is what you get with a method declaration,
// either directly on the type, or inherited from some anonymous
// embedded field.

class Named_method : public Method
{
 public:
  Named_method(Named_object* named_object, const Field_indexes* field_indexes,
	       unsigned int depth, bool is_value_method,
	       bool needs_stub_method)
    : Method(field_indexes, depth, is_value_method, needs_stub_method),
      named_object_(named_object)
  { }

 protected:
  // Get the Named_object for the method.
  Named_object*
  do_named_object() const
  { return this->named_object_; }

  // The type of the method.
  Function_type*
  do_type() const;

  // Return the location of the method receiver.
  source_location
  do_receiver_location() const;

  // Bind a method to an object.
  Expression*
  do_bind_method(Expression* expr, source_location location) const;

 private:
  // The method itself.  For a method which needs a stub, this starts
  // out as the underlying method, and is later replaced with the stub
  // method.
  Named_object* named_object_;
};

// An interface method.  This is used when an interface appears as an
// anonymous field in a named struct.

class Interface_method : public Method
{
 public:
  Interface_method(const std::string& name, source_location location,
		   Function_type* fntype, const Field_indexes* field_indexes,
		   unsigned int depth)
    : Method(field_indexes, depth, true, true),
      name_(name), location_(location), fntype_(fntype)
  { }

 protected:
  // Get the Named_object for the method.  This should never be
  // called, as we always create a stub.
  Named_object*
  do_named_object() const
  { gcc_unreachable(); }

  // The type of the method.
  Function_type*
  do_type() const
  { return this->fntype_; }

  // Return the location of the method receiver.
  source_location
  do_receiver_location() const
  { return this->location_; }

  // Bind a method to an object.
  Expression*
  do_bind_method(Expression* expr, source_location location) const;

 private:
  // The name of the interface method to call.
  std::string name_;
  // The location of the definition of the interface method.
  source_location location_;
  // The type of the interface method.
  Function_type* fntype_;
};

// A mapping from method name to Method.  This is a wrapper around a
// hash table.

class Methods
{
 private:
  typedef std::tr1::unordered_map<std::string, Method*> Method_map;

 public:
  typedef Method_map::const_iterator const_iterator;

  Methods()
    : methods_()
  { }

  // Insert a new method.  Returns true if it was inserted, false if
  // it was overidden or ambiguous.
  bool
  insert(const std::string& name, Method* m);

  // The number of (unambiguous) methods.
  size_t
  count() const;

  // Iterate.
  const_iterator
  begin() const
  { return this->methods_.begin(); }

  const_iterator
  end() const
  { return this->methods_.end(); }

  // Lookup.
  const_iterator
  find(const std::string& name) const
  { return this->methods_.find(name); }

 private:
  Method_map methods_;
};

// The value we keep for a named type.  This lets us get the right
// name when we convert to trees.  Note that we don't actually keep
// the name here; the name is in the Named_object which points to
// this.  This object exists to hold a unique tree which represents
// the type.

class Named_type : public Type
{
 public:
  Named_type(Named_object* named_object, Type* type, source_location location)
    : Type(TYPE_NAMED),
      named_object_(named_object), in_function_(NULL), type_(type),
      local_methods_(NULL), all_methods_(NULL),
      interface_method_tables_(NULL), location_(location), is_visible_(true),
      is_error_(false), seen_(false)
  { }

  // Return the associated Named_object.  This holds the actual name.
  Named_object*
  named_object()
  { return this->named_object_; }

  const Named_object*
  named_object() const
  { return this->named_object_; }

  // Set the Named_object.  This is used when we see a type
  // declaration followed by a type.
  void
  set_named_object(Named_object* no)
  { this->named_object_ = no; }

  // Return the function in which this type is defined.  This will
  // return NULL for a type defined in global scope.
  const Named_object*
  in_function() const
  { return this->in_function_; }

  // Set the function in which this type is defined.
  void
  set_in_function(Named_object* f)
  { this->in_function_ = f; }

  // Return the name of the type.
  const std::string&
  name() const;

  // Return the underlying type.
  Type*
  real_type()
  { return this->type_; }

  const Type*
  real_type() const
  { return this->type_; }

  // Return the location.
  source_location
  location() const
  { return this->location_; }

  // Whether this type is visible.  This only matters when parsing.
  bool
  is_visible() const
  { return this->is_visible_; }

  // Mark this type as visible.
  void
  set_is_visible()
  { this->is_visible_ = true; }

  // Mark this type as invisible.
  void
  clear_is_visible()
  { this->is_visible_ = false; }

  // Whether this is a builtin type.
  bool
  is_builtin() const
  { return this->location_ == BUILTINS_LOCATION; }

  // Add a method to this type.
  Named_object*
  add_method(const std::string& name, Function*);

  // Add a method declaration to this type.
  Named_object*
  add_method_declaration(const std::string& name, Package* package,
			 Function_type* type, source_location location);

  // Add an existing method--one defined before the type itself was
  // defined--to a type.
  void
  add_existing_method(Named_object*);

  // Build the complete list of methods, including those from
  // anonymous fields, and build method stubs if needed.
  void
  finalize_methods(Gogo*);

  // Return whether this type has any methods.  This should only be
  // called after the finalize_methods pass.
  bool
  has_any_methods() const
  { return this->all_methods_ != NULL; }

  // Return the methods for this type.  This should only be called
  // after the finalized_methods pass.
  const Methods*
  methods() const
  { return this->all_methods_; }

  // EXPR has this type.  Look for field or method NAME associated
  // with this type.  Return a Field_reference_expression or
  // Bound_method_expression for the field or method bound to EXPR.
  // Return NULL if there is no such field or method.  If this returns
  // NULL, it sets *FOUND_POINTER_METHOD if a method was found which
  // takes a pointer but EXPR is value whose address can not be taken.
  Expression*
  bind_field_or_method(Expression* expr, const std::string& name,
		       source_location, bool *found_pointer_method) const;

  // Return the method to use for NAME.  This returns NULL if there is
  // no such method or if the method is ambiguous.  When it returns
  // NULL, this sets *IS_AMBIGUOUS if the method name is ambiguous.
  Method*
  method_function(const std::string& name, bool *is_ambiguous) const;

  // Return a pointer to the interface method table for this type for
  // the interface INTERFACE.
  tree
  interface_method_table(Gogo*, const Interface_type* interface);

  // Whether this type is compatible with T.
  bool
  is_compatible(const Type* t, Type_compatible, std::string* reason) const;

  // Whether this type has any hidden fields.
  bool
  named_type_has_hidden_fields(std::string* reason) const;

  // Export the type.
  void
  export_named_type(Export*, const std::string& name) const;

  // Import a named type.
  static void
  import_named_type(Import*, Named_type**);

 protected:
  int
  do_traverse(Traverse* traverse)
  { return Type::traverse(this->type_, traverse); }

  bool
  do_verify();

  bool
  do_is_refcounted() const
  { return this->type_->is_refcounted(); }

  bool
  do_has_refcounted_component() const
  { return this->type_->has_refcounted_component(); }

  void
  do_add_refcount_queue_entries(Refcounts* refcounts, Refcount_entry* entry);

  size_t
  do_hash_for_method() const;

  bool
  do_check_make_expression(Expression_list* args, source_location location)
  { return this->type_->check_make_expression(args, location); }

  tree
  do_get_tree(Gogo*);

  tree
  do_init_tree(Gogo* gogo, bool is_clear)
  { return this->type_->get_init_tree(gogo, is_clear); }

  tree
  do_make_expression_tree(Translate_context* context, Expression_list* args,
			  source_location location)
  { return this->type_->make_expression_tree(context, args, location); }

  tree
  do_set_refcount_queue_entry(Gogo* gogo, Refcounts* refcounts,
			      Refcount_entry* entry, tree val) const
  {
    return this->type_->set_refcount_queue_entry(gogo, refcounts, entry, val);
  }

  void
  do_type_descriptor_decl(Gogo*, Named_type*, tree*);

  void
  do_reflection(Gogo*, std::string*) const;

  void
  do_mangled_name(Gogo*, std::string* ret) const;

  void
  do_export(Export*) const;

 private:
  // A mapping from interfaces to the associated interface method
  // tables for this type.  This maps to a decl.
  typedef std::tr1::unordered_map<const Interface_type*, tree, Type_hash,
				  Type_identical> Interface_method_tables;

  static const char* const receiver_name;

  // Return whether a method expects a pointer as the receiver.
  static bool
  method_expects_pointer(const Named_object*);

  // Look for a field or method named NAME.
  bool
  find_field_or_method(const std::string& name, bool receiver_can_be_pointer,
		       int* level, bool* is_method,
		       bool* found_pointer_method) const;

  // A hash table we use to avoid infinite recursion.
  typedef std::tr1::unordered_set<const Named_type*, Type_hash,
				  Type_identical> Types_seen;

  // Add all methods for TYPE to the list of methods for THIS.
  bool
  add_methods_for_type(const Named_type* type, const Method::Field_indexes*,
		       unsigned int depth, bool, bool, Types_seen*);

  bool
  add_local_methods_for_type(const Named_type* type,
			     const Method::Field_indexes*,
			     unsigned int depth, bool, bool);

  bool
  add_embedded_methods_for_type(const Named_type* type,
				const Method::Field_indexes*,
				unsigned int depth, bool, bool, Types_seen*);

  bool
  add_interface_methods_for_type(const Named_type* type,
				 const Method::Field_indexes*,
				 unsigned int depth);

  // Build stub methods.
  void
  build_stub_methods(Gogo*);

  void
  build_one_stub_method(Gogo*, Method*, const char* receiver_name,
			const Typed_identifier_list*);

  Expression*
  apply_field_indexes(Expression*, const Method::Field_indexes*);

  // A pointer back to the Named_object for this type.
  Named_object* named_object_;
  // If this type is defined in a function, a pointer back to the
  // function in which it is defined.
  Named_object* in_function_;
  // The actual type.
  Type* type_;
  // The list of methods defined for this type.  Any named type can
  // have methods.
  Bindings* local_methods_;
  // The full list of methods for this type, including methods
  // declared for anonymous fields.
  Methods* all_methods_;
  // A mapping from interfaces to the associated interface method
  // tables for this type.
  Interface_method_tables* interface_method_tables_;
  // The location where this type was defined.
  source_location location_;
  // Whether this type is visible.  This is false if this type was
  // created because it was referenced by an imported object, but the
  // type itself was not exported.  This will always be true for types
  // created in the current package.
  bool is_visible_;
  // Whether this type is erroneous.
  bool is_error_;
  // In a recursive operation such as is_compatible, this flag is used
  // to prevent infinite recursion when a type refers to itself.  This
  // is mutable because it is always reset to false when the function
  // exits.
  mutable bool seen_;
};

// A forward declaration.  This handles a type which has been declared
// but not defined.

class Forward_declaration_type : public Type
{
 public:
  Forward_declaration_type(Named_object* named_object);

  // The named object associated with this type declaration.  This
  // will be resolved.
  Named_object*
  named_object();

  const Named_object*
  named_object() const;

  // Return the name of the type.
  const std::string&
  name() const;

  // Return the type to which this points.  Give an error if the type
  // has not yet been defined.
  Type*
  real_type();

  const Type*
  real_type() const;

  // Whether the base type has been defined.
  bool
  is_defined() const;

  // Add a method to this type.
  Named_object*
  add_method(const std::string& name, Function*);

  // Add a method declaration to this type.
  Named_object*
  add_method_declaration(const std::string& name, Function_type*,
			 source_location);

 protected:
  int
  do_traverse(Traverse* traverse);

  bool
  do_is_refcounted() const
  { return this->base()->is_refcounted(); }

  bool
  do_has_refcounted_component() const
  { return this->base()->has_refcounted_component(); }

  void
  do_add_refcount_queue_entries(Refcounts* refcounts, Refcount_entry* entry)
  { return this->real_type()->add_refcount_queue_entries(refcounts, entry); }

  size_t
  do_hash_for_method() const
  { return this->real_type()->hash_for_method(); }

  bool
  do_check_make_expression(Expression_list* args, source_location location)
  { return this->base()->check_make_expression(args, location); }

  tree
  do_get_tree(Gogo* gogo);

  tree
  do_init_tree(Gogo* gogo, bool is_clear)
  { return this->base()->get_init_tree(gogo, is_clear); }

  tree
  do_make_expression_tree(Translate_context* context, Expression_list* args,
			  source_location location)
  { return this->base()->make_expression_tree(context, args, location); }

  tree
  do_set_refcount_queue_entry(Gogo* gogo, Refcounts* refcounts,
			      Refcount_entry* entry, tree val) const
  {
    return this->base()->set_refcount_queue_entry(gogo, refcounts, entry, val);
  }

  void
  do_type_descriptor_decl(Gogo*, Named_type*, tree*);

  void
  do_reflection(Gogo*, std::string*) const;

  void
  do_mangled_name(Gogo*, std::string* ret) const;

  void
  do_export(Export*) const;

 private:
  // Issue a warning about a use of an undefined type.
  void
  warn() const;

  // The type declaration.
  Named_object* named_object_;
  // Whether we have issued a warning about this type.
  mutable bool warned_;
};

// The Type_context struct describes what we expect for the type of an
// expression.

struct Type_context
{
  // The exact type we expect, if known.  This may be NULL.
  Type* type;
  // Whether an abstract type is permitted.
  bool may_be_abstract;

  // Constructors.
  Type_context()
    : type(NULL), may_be_abstract(false)
  { }

  Type_context(Type* a_type, bool a_may_be_abstract)
    : type(a_type), may_be_abstract(a_may_be_abstract)
  { }
};

#endif // !defined(GO_TYPES_H)
