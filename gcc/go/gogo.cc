// gogo.cc -- Go frontend parsed representation.

// Copyright 2009 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <algorithm>
#include <list>

#include "go-system.h"

#include "go-dump.h"
#include "lex.h"
#include "types.h"
#include "statements.h"
#include "expressions.h"
#include "dataflow.h"
#include "import.h"
#include "export.h"
#include "refcount.h"
#include "gogo.h"

// Class Gogo.

Gogo::Gogo()
  : package_(NULL),
    functions_(),
    globals_(new Bindings(NULL)),
    imports_(),
    packages_(),
    map_descriptors_(NULL),
    type_descriptor_decls_(NULL),
    init_functions_(),
    need_init_fn_(false),
    init_fn_name_(),
    imported_init_fns_()
{
  const source_location loc = BUILTINS_LOCATION;

  Named_type* uint8_type = Type::make_integer_type("uint8", true, 8,
						   RUNTIME_TYPE_CODE_UINT8);
  this->add_named_type(uint8_type);
  this->add_named_type(Type::make_integer_type("uint16", true,  16,
					       RUNTIME_TYPE_CODE_UINT16));
  this->add_named_type(Type::make_integer_type("uint32", true,  32,
					       RUNTIME_TYPE_CODE_UINT32));
  this->add_named_type(Type::make_integer_type("uint64", true,  64,
					       RUNTIME_TYPE_CODE_UINT64));

  this->add_named_type(Type::make_integer_type("int8",  false,   8,
					       RUNTIME_TYPE_CODE_INT8));
  this->add_named_type(Type::make_integer_type("int16", false,  16,
					       RUNTIME_TYPE_CODE_INT16));
  this->add_named_type(Type::make_integer_type("int32", false,  32,
					       RUNTIME_TYPE_CODE_INT32));
  this->add_named_type(Type::make_integer_type("int64", false,  64,
					       RUNTIME_TYPE_CODE_INT64));

  this->add_named_type(Type::make_float_type("float32", 32,
					     RUNTIME_TYPE_CODE_FLOAT32));
  this->add_named_type(Type::make_float_type("float64", 64,
					     RUNTIME_TYPE_CODE_FLOAT64));

  const int int_type_size = std::max(INT_TYPE_SIZE, 32);
  this->add_named_type(Type::make_integer_type("uint", true,
					       int_type_size,
					       RUNTIME_TYPE_CODE_UINT));
  Named_type* int_type = Type::make_integer_type("int", false, int_type_size,
						 RUNTIME_TYPE_CODE_INT);
  this->add_named_type(int_type);

  // "byte" is an alias for "uint8".  Construct a Named_object which
  // points to UINT8_TYPE.  Note that this breaks the normal pairing
  // in which a Named_object points to a Named_type which points back
  // to the same Named_object.
  Named_object* byte_type = this->declare_type("byte", loc);
  byte_type->set_type_value(uint8_type);

  this->add_named_type(Type::make_integer_type("uintptr", true,
					       POINTER_SIZE,
					       RUNTIME_TYPE_CODE_UINTPTR));

  this->add_named_type(Type::make_float_type("float", FLOAT_TYPE_SIZE,
					     RUNTIME_TYPE_CODE_FLOAT));

  this->add_named_type(Type::make_named_bool_type());

  this->add_named_type(Type::make_named_string_type());

  this->globals_->add_constant(Typed_identifier("true",
						Type::make_boolean_type(),
						loc),
			       NULL,
			       Expression::make_boolean(true, loc),
			       0);
  this->globals_->add_constant(Typed_identifier("false",
						Type::make_boolean_type(),
						loc),
			       NULL,
			       Expression::make_boolean(false, loc),
			       0);

  this->globals_->add_constant(Typed_identifier("nil", Type::make_nil_type(),
						loc),
			       NULL,
			       Expression::make_nil(loc),
			       0);

  Type* abstract_int_type = Type::make_abstract_integer_type();
  this->globals_->add_constant(Typed_identifier("iota", abstract_int_type,
						loc),
			       NULL,
			       Expression::make_iota(),
			       0);

  Function_type* new_type = Type::make_function_type(NULL, NULL, NULL, loc);
  new_type->set_is_varargs();
  new_type->set_is_builtin();
  this->globals_->add_function_declaration("new", NULL, new_type, loc);

  Function_type* make_type = Type::make_function_type(NULL, NULL, NULL, loc);
  make_type->set_is_varargs();
  make_type->set_is_builtin();
  this->globals_->add_function_declaration("make", NULL, make_type, loc);

  Typed_identifier_list* len_result = new Typed_identifier_list();
  len_result->push_back(Typed_identifier("", int_type, loc));
  Function_type* len_type = Type::make_function_type(NULL, NULL, len_result,
						     loc);
  len_type->set_is_builtin();
  this->globals_->add_function_declaration("len", NULL, len_type, loc);

  Typed_identifier_list* cap_result = new Typed_identifier_list();
  cap_result->push_back(Typed_identifier("", int_type, loc));
  Function_type* cap_type = Type::make_function_type(NULL, NULL, len_result,
						     loc);
  cap_type->set_is_builtin();
  this->globals_->add_function_declaration("cap", NULL, cap_type, loc);

  Function_type* print_type = Type::make_function_type(NULL, NULL, NULL, loc);
  print_type->set_is_varargs();
  print_type->set_is_builtin();
  this->globals_->add_function_declaration("print", NULL, print_type, loc);

  print_type = Type::make_function_type(NULL, NULL, NULL, loc);
  print_type->set_is_varargs();
  print_type->set_is_builtin();
  this->globals_->add_function_declaration("println", NULL, print_type, loc);

  Function_type* panic_type = Type::make_function_type(NULL, NULL, NULL, loc);
  panic_type->set_is_varargs();
  panic_type->set_is_builtin();
  this->globals_->add_function_declaration("panic", NULL, panic_type, loc);

  panic_type = Type::make_function_type(NULL, NULL, NULL, loc);
  panic_type->set_is_varargs();
  panic_type->set_is_builtin();
  this->globals_->add_function_declaration("panicln", NULL, panic_type, loc);

  Function_type* close_type = Type::make_function_type(NULL, NULL, NULL, loc);
  close_type->set_is_varargs();
  close_type->set_is_builtin();
  this->globals_->add_function_declaration("close", NULL, close_type, loc);

  Typed_identifier_list* closed_result = new Typed_identifier_list();
  closed_result->push_back(Typed_identifier("", Type::lookup_bool_type(),
					    loc));
  Function_type* closed_type = Type::make_function_type(NULL, NULL,
							closed_result, loc);
  closed_type->set_is_varargs();
  closed_type->set_is_builtin();
  this->globals_->add_function_declaration("closed", NULL, closed_type, loc);

  Typed_identifier_list* copy_result = new Typed_identifier_list();
  copy_result->push_back(Typed_identifier("", int_type, loc));
  Function_type* copy_type = Type::make_function_type(NULL, NULL,
						      copy_result, loc);
  copy_type->set_is_varargs();
  copy_type->set_is_builtin();
  this->globals_->add_function_declaration("copy", NULL, copy_type, loc);

  this->define_builtin_function_trees();

  // Declare "init", to ensure that it is not defined with parameters
  // or return values.
  this->declare_function("init",
			 Type::make_function_type(NULL, NULL, NULL, loc),
			 loc);
}

// Get the package name.

const std::string&
Gogo::package_name() const
{
  gcc_assert(this->package_ != NULL);
  return this->package_->name();
}

// Set the package name.

void
Gogo::set_package_name(const std::string& package_name,
		       source_location location)
{
  if (this->package_ != NULL && this->package_->name() != package_name)
    {
      error_at(location, "expected package %<%s%>",
	       this->package_->name().c_str());
      return;
    }

  this->package_ = this->register_package(package_name, location);
  this->package_->set_being_compiled();

  // We used to permit people to qualify symbols with the current
  // package name (e.g., P.x), but we no longer do.
  // this->globals_->add_package(package_name, this->package_);

  if (package_name == "main")
    {
      // Declare "main" as a function which takes no parameters and
      // returns no value.
      this->declare_function("main",
			     Type::make_function_type(NULL, NULL, NULL,
						      BUILTINS_LOCATION),
			     BUILTINS_LOCATION);
    }
}

// Import a package.

void
Gogo::import_package(const std::string& filename,
		     const std::string& local_name,
		     bool is_local_name_exported,
		     source_location location)
{
  if (filename == "unsafe")
    {
      this->import_unsafe(local_name, is_local_name_exported, location);
      return;
    }

  Imports::const_iterator p = this->imports_.find(filename);
  if (p != this->imports_.end())
    {
      Package* package = p->second;
      package->set_location(location);
      std::string ln = local_name;
      bool is_ln_exported = is_local_name_exported;
      if (ln.empty())
	{
	  ln = package->name();
	  is_ln_exported = Lex::is_exported_name(ln);
	}
      if (ln != ".")
	ln = this->pack_hidden_name(ln, is_ln_exported);
      this->package_->bindings()->add_package(ln, package);
      if (ln == ".")
	{
	  Bindings* bindings = package->bindings();
	  for (Bindings::const_declarations_iterator p =
		 bindings->begin_declarations();
	       p != bindings->end_declarations();
	       ++p)
	    this->add_named_object(p->second);
	}
      return;
    }

  Import::Stream* stream = Import::open_package(filename, location);
  if (stream == NULL)
    {
      error_at(location, "import file %qs not found", filename.c_str());
      return;
    }

  Import imp(stream, location);
  imp.register_builtin_types(this);
  Package* package = imp.import(this, local_name, is_local_name_exported);
  this->imports_.insert(std::make_pair(filename, package));
}

// Return whether we are at the global binding level.

bool
Gogo::in_global_scope() const
{
  return this->functions_.empty();
}

// Return the current binding contour.

Bindings*
Gogo::current_bindings()
{
  if (!this->functions_.empty())
    return this->functions_.back().blocks.back()->bindings();
  else if (this->package_ != NULL)
    return this->package_->bindings();
  else
    return this->globals_;
}

const Bindings*
Gogo::current_bindings() const
{
  if (!this->functions_.empty())
    return this->functions_.back().blocks.back()->bindings();
  else if (this->package_ != NULL)
    return this->package_->bindings();
  else
    return this->globals_;
}

// Return the current block.

Block*
Gogo::current_block()
{
  if (this->functions_.empty())
    return NULL;
  else
    return this->functions_.back().blocks.back();
}

// Look up a name in the current binding contour.  If PFUNCTION is not
// NULL, set it to the function in which the name is defined, or NULL
// if the name is defined in global scope.

Named_object*
Gogo::lookup(const std::string& name, Named_object** pfunction) const
{
  if (Gogo::is_sink_name(name))
    return Named_object::make_sink();

  for (Open_functions::const_reverse_iterator p = this->functions_.rbegin();
       p != this->functions_.rend();
       ++p)
    {
      Named_object* ret = p->blocks.back()->bindings()->lookup(name);
      if (ret != NULL)
	{
	  if (pfunction != NULL)
	    *pfunction = p->function;
	  return ret;
	}
    }

  if (pfunction != NULL)
    *pfunction = NULL;

  if (this->package_ != NULL)
    {
      Named_object* ret = this->package_->bindings()->lookup(name);
      if (ret != NULL)
	return ret;
    }

  // We do not look in the global namespace.  If we did, the global
  // namespace would effectively hide names which were defined in
  // package scope which we have not yet seen.  Instead,
  // define_global_names is called after parsing is over to connect
  // undefined names at package scope with names defined at global
  // scope.

  return NULL;
}

// Look up a name in the current block, without searching enclosing
// blocks.

Named_object*
Gogo::lookup_in_block(const std::string& name) const
{
  gcc_assert(!this->functions_.empty());
  gcc_assert(!this->functions_.back().blocks.empty());
  return this->functions_.back().blocks.back()->bindings()->lookup_local(name);
}

// Look up a name in the global namespace.

Named_object*
Gogo::lookup_global(const char* name) const
{
  return this->globals_->lookup(name);
}

// Add an imported package.

Package*
Gogo::add_imported_package(const std::string& real_name,
			   const std::string& alias_arg,
			   bool is_alias_exported,
			   source_location location,
			   bool* padd_to_globals)
{
  // FIXME: Now that we compile packages as a whole, should we permit
  // importing the current package?
  if (this->package_name() == real_name)
    {
      *padd_to_globals = false;
      if (!alias_arg.empty() && alias_arg != ".")
	{
	  std::string alias = this->pack_hidden_name(alias_arg,
						     is_alias_exported);
	  this->package_->bindings()->add_package(alias, this->package_);
	}
      return this->package_;
    }
  else if (alias_arg == ".")
    {
      *padd_to_globals = true;
      return this->register_package(real_name, location);
    }
  else if (alias_arg == "_")
    return this->register_package(real_name, location);
  else
    {
      *padd_to_globals = false;
      std::string alias = alias_arg;
      if (alias.empty())
	{
	  alias = real_name;
	  is_alias_exported = Lex::is_exported_name(alias);
	}
      alias = this->pack_hidden_name(alias, is_alias_exported);
      Named_object* no = this->add_package(real_name, alias, location);
      if (!no->is_package())
	return NULL;
      return no->package_value();
    }
}

// Add a package.

Named_object*
Gogo::add_package(const std::string& real_name, const std::string& alias,
		  source_location location)
{
  gcc_assert(this->in_global_scope());

  // Register the package.  Note that we might have already seen it in
  // an earlier import.
  Package* package = this->register_package(real_name, location);

  return this->package_->bindings()->add_package(alias, package);
}

// Register a package.  This package may or may not be imported.  This
// returns the Package structure for the package, creating if it
// necessary.

Package*
Gogo::register_package(const std::string& name, source_location location)
{
  Package* package = NULL;
  std::pair<Packages::iterator, bool> ins =
    this->packages_.insert(std::make_pair(name, package));
  if (!ins.second)
    {
      // We have seen this package name before.
      package = ins.first->second;
      gcc_assert(package != NULL);
      if (package->location() == UNKNOWN_LOCATION)
	package->set_location(location);
    }
  else
    {
      // First time we have seen this package name.
      package = new Package(name, location);
      gcc_assert(ins.first->second == NULL);
      ins.first->second = package;
    }

  return package;
}

// Start compiling a function.

Named_object*
Gogo::start_function(const std::string& name, Function_type* type,
		     bool add_method_to_type, source_location location)
{
  bool at_top_level = this->functions_.empty();

  Block* block = new Block(NULL, location);

  Function* enclosing = (at_top_level
			 ? NULL
			 : this->functions_.back().function->func_value());

  Function* function = new Function(type, enclosing, block, location);

  if (type->is_method())
    {
      const Typed_identifier* receiver = type->receiver();
      Variable* this_param = new Variable(receiver->type(), NULL, false,
					  true, true, location);
      std::string name = receiver->name();
      if (name.empty())
	{
	  // We need to give receivers a name since they wind up in
	  // DECL_ARGUMENTS.  FIXME.
	  static unsigned int count;
	  char buf[50];
	  snprintf(buf, sizeof buf, "r.%u", count);
	  ++count;
	  name = buf;
	}
      block->bindings()->add_variable(name, NULL, this_param);
    }

  const Typed_identifier_list* parameters = type->parameters();
  if (parameters != NULL)
    {
      for (Typed_identifier_list::const_iterator p = parameters->begin();
	   p != parameters->end();
	   ++p)
	{
	  Type* param_type = p->type();

	  // We use a varargs type in the function type, but an empty
	  // interface for the actual parameter.
	  bool is_varargs_param = false;
	  if (param_type->is_varargs_type())
	    {
	      param_type = Function_type::varargs_type();
	      is_varargs_param = true;
	    }

	  Variable* param = new Variable(param_type, NULL, false, true, false,
					 location);

	  if (is_varargs_param)
	    param->set_is_varargs_parameter();

	  std::string name = p->name();
	  if (name.empty() || Gogo::is_sink_name(name))
	    {
	      // We need to give parameters a name since they wind up
	      // in DECL_ARGUMENTS.  FIXME.
	      static unsigned int count;
	      char buf[50];
	      snprintf(buf, sizeof buf, "p.%u", count);
	      ++count;
	      name = buf;
	    }
	  block->bindings()->add_variable(name, NULL, param);
	}
    }

  const Typed_identifier_list* results = type->results();
  if (results != NULL
      && !results->empty()
      && !results->front().name().empty())
    {
      int index = 0;
      for (Typed_identifier_list::const_iterator p = results->begin();
	   p != results->end();
	   ++p, ++index)
	{
	  Result_variable* result = new Result_variable(p->type(), function,
							index);
	  block->bindings()->add_result_variable(p->name(), result);
	}
    }

  const std::string* pname;
  std::string nested_name;
  if (!name.empty())
    pname = &name;
  else
    {
      // Invent a name for a nested function.
      static int nested_count;
      char buf[30];
      snprintf(buf, sizeof buf, "__nested%d", nested_count);
      ++nested_count;
      nested_name = buf;
      pname = &nested_name;
    }

  Named_object* ret;
  if (Gogo::is_sink_name(*pname))
    ret = Named_object::make_sink();
  else if (!type->is_method())
    {
      ret = this->package_->bindings()->add_function(*pname, NULL, function);
      if (!ret->is_function())
	{
	  // Redefinition error.
	  ret = Named_object::make_function(name, NULL, function);
	}
    }
  else
    {
      if (!add_method_to_type)
	ret = Named_object::make_function(name, NULL, function);
      else
	{
	  gcc_assert(at_top_level);
	  Type* rtype = type->receiver()->type();

	  // We want to look through the pointer created by the
	  // parser, without getting an error if the type is not yet
	  // defined.
	  if (rtype->classification() == Type::TYPE_POINTER)
	    rtype = rtype->points_to();

	  if (rtype->is_error_type())
	    ret = Named_object::make_function(name, NULL, function);
	  else if (rtype->named_type() != NULL)
	    {
	      ret = rtype->named_type()->add_method(name, function);
	      if (!ret->is_function())
		{
		  // Redefinition error.
		  ret = Named_object::make_function(name, NULL, function);
		}
	    }
	  else if (rtype->forward_declaration_type() != NULL)
	    {
	      Named_object* type_no =
		rtype->forward_declaration_type()->named_object();
	      if (type_no->is_unknown())
		{
		  // If we are seeing methods it really must be a
		  // type.  Declare it as such.  An alternative would
		  // be to support lists of methods for unknown
		  // expressions.  Either way the error messages if
		  // this is not a type are going to get confusing.
		  Named_object* declared =
		    this->declare_package_type(type_no->name(),
					       type_no->location());
		  gcc_assert(declared
			     == type_no->unknown_value()->real_named_object());
		}
	      ret = rtype->forward_declaration_type()->add_method(name,
								  function);
	    }
	  else
	    gcc_unreachable();
	}
      this->package_->bindings()->add_method(ret);
    }

  this->functions_.resize(this->functions_.size() + 1);
  Open_function& of(this->functions_.back());
  of.function = ret;
  of.blocks.push_back(block);

  if (!type->is_method() && Gogo::unpack_hidden_name(name) == "init")
    {
      this->init_functions_.push_back(ret);
      this->need_init_fn_ = true;
    }

  return ret;
}

// Finish compiling a function.

void
Gogo::finish_function(source_location location)
{
  this->finish_block(location);
  gcc_assert(this->functions_.back().blocks.empty());
  this->functions_.pop_back();
}

// Return the current function.

Named_object*
Gogo::current_function() const
{
  gcc_assert(!this->functions_.empty());
  return this->functions_.back().function;
}

// Start a new block.

void
Gogo::start_block(source_location location)
{
  gcc_assert(!this->functions_.empty());
  Block* block = new Block(this->current_block(), location);
  this->functions_.back().blocks.push_back(block);
}

// Finish a block.

Block*
Gogo::finish_block(source_location location)
{
  gcc_assert(!this->functions_.empty());
  gcc_assert(!this->functions_.back().blocks.empty());
  Block* block = this->functions_.back().blocks.back();
  this->functions_.back().blocks.pop_back();
  block->set_end_location(location);
  return block;
}

// Add an unknown name.

Named_object*
Gogo::add_unknown_name(const std::string& name, source_location location)
{
  return this->package_->bindings()->add_unknown_name(name, location);
}

// Declare a function.

Named_object*
Gogo::declare_function(const std::string& name, Function_type* type,
		       source_location location)
{
  if (!type->is_method())
    return this->current_bindings()->add_function_declaration(name, NULL, type,
							      location);
  else
    {
      // We don't bother to add this to the list of global
      // declarations.
      Type* rtype = type->receiver()->type();

      // We want to look through the pointer created by the
      // parser, without getting an error if the type is not yet
      // defined.
      if (rtype->classification() == Type::TYPE_POINTER)
	rtype = rtype->points_to();

      if (rtype->is_error_type())
	return NULL;
      else if (rtype->named_type() != NULL)
	return rtype->named_type()->add_method_declaration(name, NULL, type,
							   location);
      else if (rtype->forward_declaration_type() != NULL)
	{
	  Forward_declaration_type* ftype = rtype->forward_declaration_type();
	  return ftype->add_method_declaration(name, type, location);
	}
      else
	gcc_unreachable();
    }
}

// Add a label definition.

Label*
Gogo::add_label_definition(const std::string& label_name,
			   source_location location)
{
  gcc_assert(!this->functions_.empty());
  Function* func = this->functions_.back().function->func_value();
  Label* label = func->add_label_definition(label_name, location);
  this->add_statement(Statement::make_label_statement(label, location));
  return label;
}

// Add a label reference.

Label*
Gogo::add_label_reference(const std::string& label_name)
{
  gcc_assert(!this->functions_.empty());
  Function* func = this->functions_.back().function->func_value();
  return func->add_label_reference(label_name);
}

// Add a statement.

void
Gogo::add_statement(Statement* statement)
{
  gcc_assert(!this->functions_.empty()
	     && !this->functions_.back().blocks.empty());
  this->functions_.back().blocks.back()->add_statement(statement);
}

// Add a block.

void
Gogo::add_block(Block* block, source_location location)
{
  gcc_assert(!this->functions_.empty()
	     && !this->functions_.back().blocks.empty());
  Statement* statement = Statement::make_block_statement(block, location);
  this->functions_.back().blocks.back()->add_statement(statement);
}

// Add a constant.

Named_object*
Gogo::add_constant(const Typed_identifier& tid, Expression* expr,
		   int iota_value)
{
  return this->current_bindings()->add_constant(tid, NULL, expr, iota_value);
}

// Add a type.

void
Gogo::add_type(const std::string& name, Type* type, source_location location)
{
  Named_object* no = this->current_bindings()->add_type(name, NULL, type,
							location);
  if (!this->in_global_scope())
    no->type_value()->set_in_function(this->functions_.back().function);
}

// Add a named type.

void
Gogo::add_named_type(Named_type* type)
{
  gcc_assert(this->in_global_scope());
  this->current_bindings()->add_named_type(type);
}

// Declare a type.

Named_object*
Gogo::declare_type(const std::string& name, source_location location)
{
  Bindings* bindings = this->current_bindings();
  Named_object* no = bindings->add_type_declaration(name, NULL, location);
  if (!this->in_global_scope())
    {
      Named_object* f = this->functions_.back().function;
      no->type_declaration_value()->set_in_function(f);
    }
  return no;
}

// Declare a type at the package level.

Named_object*
Gogo::declare_package_type(const std::string& name, source_location location)
{
  return this->package_->bindings()->add_type_declaration(name, NULL, location);
}

// Define a type which was already declared.

void
Gogo::define_type(Named_object* no, Named_type* type)
{
  this->current_bindings()->define_type(no, type);
}

// Add a variable.

Named_object*
Gogo::add_variable(const std::string& name, Variable* variable)
{
  Named_object* no = this->current_bindings()->add_variable(name, NULL,
							    variable);

  // In a function the middle-end wants to see a DECL_EXPR node.
  if (no != NULL
      && no->is_variable()
      && !no->var_value()->is_parameter()
      && !this->functions_.empty())
    this->add_statement(Statement::make_variable_declaration(no));

  return no;
}

// Add a sink--a reference to the blank identifier _.

Named_object*
Gogo::add_sink()
{
  return Named_object::make_sink();
}

// Add a named object.

void
Gogo::add_named_object(Named_object* no)
{
  this->current_bindings()->add_named_object(no);
}

// Define the global names.  We do this only after parsing all the
// input files, because the program might define the global names
// itself.

void
Gogo::define_global_names()
{
  for (Bindings::const_declarations_iterator p =
	 this->globals_->begin_declarations();
       p != this->globals_->end_declarations();
       ++p)
    {
      Named_object* global_no = p->second;
      std::string name(Gogo::pack_hidden_name(global_no->name(), false));
      Named_object* no = this->package_->bindings()->lookup(name);
      if (no == NULL)
	continue;
      no = no->resolve();
      if (no->is_type_declaration())
	{
	  if (global_no->is_type())
	    {
	      if (no->type_declaration_value()->has_methods())
		error_at(no->location(),
			 "may not define methods for global type");
	      no->set_type_value(global_no->type_value());
	    }
	  else
	    {
	      error_at(no->location(), "expected type");
	      Type* errtype = Type::make_error_type();
	      Named_object* err = Named_object::make_type("error", NULL,
							  errtype,
							  BUILTINS_LOCATION);
	      no->set_type_value(err->type_value());
	    }
	}
      else if (no->is_unknown())
	no->unknown_value()->set_real_named_object(global_no);
    }
}

// Clear out names in file scope.

void
Gogo::clear_file_scope()
{
  this->package_->bindings()->clear_file_scope();
}

// Traverse the tree.

void
Gogo::traverse(Traverse* traverse)
{
  // Traverse the current package first for consistency.  The other
  // packages will only contain imported types, constants, and
  // declarations.
  if (this->package_->bindings()->traverse(traverse, true) == TRAVERSE_EXIT)
    return;
  for (Packages::const_iterator p = this->packages_.begin();
       p != this->packages_.end();
       ++p)
    {
      if (p->second != this->package_)
	{
	  if (p->second->bindings()->traverse(traverse, true) == TRAVERSE_EXIT)
	    break;
	}
    }
}

// Traversal class used to verify types.

class Verify_types : public Traverse
{
 public:
  Verify_types()
    : Traverse(traverse_types)
  { }

  int
  type(Type*);
};

// Verify that a type is correct.

int
Verify_types::type(Type* t)
{
  // Don't verify types defined in other packages.
  Named_type* nt = t->named_type();
  if (nt != NULL && nt->named_object()->package() != NULL)
    return TRAVERSE_SKIP_COMPONENTS;

  if (!t->verify())
    return TRAVERSE_SKIP_COMPONENTS;
  return TRAVERSE_CONTINUE;
}

// Verify that all types are correct.

void
Gogo::verify_types()
{
  Verify_types traverse;
  this->traverse(&traverse);
}

// Traversal class used to lower parse tree.

class Lower_parse_tree : public Traverse
{
 public:
  Lower_parse_tree(Gogo* gogo)
    : Traverse(traverse_constants
	       | traverse_functions
	       | traverse_statements
	       | traverse_expressions),
      gogo_(gogo), iota_value_(-1)
  { }

  int
  constant(Named_object*, bool);

  int
  function(Named_object*);

  int
  statement(Block*, size_t* pindex, Statement*);

  int
  expression(Expression**);

 private:
  // General IR.
  Gogo* gogo_;
  // Value to use for the predeclared constant iota.
  int iota_value_;
};

// Lower constants.  We handle constants specially so that we can set
// the right value for the predeclared constant iota.  This works in
// conjunction with the way we lower Const_expression objects.

int
Lower_parse_tree::constant(Named_object* no, bool)
{
  Named_constant* nc = no->const_value();

  // We can recursively a constant if the initializer expression
  // manages to refer to itself.
  if (nc->lowering())
    return TRAVERSE_CONTINUE;
  nc->set_lowering();

  gcc_assert(this->iota_value_ == -1);
  this->iota_value_ = nc->iota_value();
  nc->traverse_expression(this);
  this->iota_value_ = -1;

  nc->clear_lowering();

  // We will traverse the expression a second time, but that will be
  // fast.

  return TRAVERSE_CONTINUE;
}

// Lower function closure types.

int
Lower_parse_tree::function(Named_object* no)
{
  no->func_value()->set_closure_type();
  return TRAVERSE_CONTINUE;
}

// Lower statement parse trees.

int
Lower_parse_tree::statement(Block* block, size_t* pindex, Statement* s)
{
  Statement* snew = s->lower(this->gogo_);
  if (snew == s)
    return TRAVERSE_CONTINUE;
  block->replace_statement(*pindex, snew);
  int t = snew->traverse(block, pindex, this);
  if (t != TRAVERSE_CONTINUE)
    return t;
  return TRAVERSE_SKIP_COMPONENTS;
}

// Lower expression parse trees.

int
Lower_parse_tree::expression(Expression** pexpr)
{
  // We have to lower all subexpressions first, so that we can get
  // their type if necessary.  This is awkward, because we don't have
  // a postorder traversal pass.
  if ((*pexpr)->traverse_subexpressions(this) == TRAVERSE_EXIT)
    return TRAVERSE_EXIT;
  // Keep lowering until nothing changes.
  while (true)
    {
      Expression* e = *pexpr;
      Expression* enew = e->lower(this->gogo_, this->iota_value_);
      if (enew == e)
	break;
      *pexpr = enew;
    }
  return TRAVERSE_SKIP_COMPONENTS;
}

// Lower the parse tree.  This is called after the parse is complete,
// when all names should be resolved.

void
Gogo::lower_parse_tree()
{
  Lower_parse_tree lower_parse_tree(this);
  this->traverse(&lower_parse_tree);
}

// Lower a constant.  This is called when lowering a reference to a
// constant.  We have to make sure that the constant has already been
// lowered.

void
Gogo::lower_constant(Named_object* no)
{
  gcc_assert(no->is_const());
  Lower_parse_tree lower(this);
  lower.constant(no, false);
}

// Look for interface types to finalize methods of inherited
// interfaces.

class Finalize_methods : public Traverse
{
 public:
  Finalize_methods()
    : Traverse(traverse_types)
  { }

  int
  type(Type*);
};

// Finalize the methods of an interface type.

int
Finalize_methods::type(Type* t)
{
  // Check the classification so that we don't finalize the methods
  // twice for a named interface type.
  if (t->classification() == Type::TYPE_INTERFACE)
    t->interface_type()->finalize_methods();
  return TRAVERSE_CONTINUE;
}

// Finalize method lists and build stub methods for named types.

void
Gogo::finalize_methods()
{
  // Finalizing methods can add new definitions, so gather the types
  // first.
  std::vector<Named_type*> types;
  for (Packages::const_iterator p = this->packages_.begin();
       p != this->packages_.end();
       ++p)
    {
      Bindings* bindings = p->second->bindings();
      for (Bindings::const_definitions_iterator pd =
	     bindings->begin_definitions();
	   pd != bindings->end_definitions();
	   ++pd)
	{
	  if (p->second == this->package_ && (*pd)->package() != NULL)
	    {
	      // This was brought in via 'import . "pkg"'.  We will
	      // pick it up when we look at its original package.
	      continue;
	    }

	  if ((*pd)->is_type())
	    types.push_back((*pd)->type_value());
	}
    }

  for (std::vector<Named_type*>::const_iterator p = types.begin();
       p != types.end();
       ++p)
    (*p)->finalize_methods(this);

  Finalize_methods finalize;
  this->traverse(&finalize);
}

// Set types for unspecified variables and constants.

void
Gogo::determine_types()
{
  Bindings* bindings = this->current_bindings();
  for (Bindings::const_definitions_iterator p = bindings->begin_definitions();
       p != bindings->end_definitions();
       ++p)
    {
      if ((*p)->is_function())
	(*p)->func_value()->determine_types();
      else if ((*p)->is_variable())
	(*p)->var_value()->determine_type();
      else if ((*p)->is_const())
	(*p)->const_value()->determine_type();

      // If this is a global variable which requires runtime
      // initialization, we need an initialization function.  We know
      // that we will see all global variables here.
      if ((*p)->is_variable())
	{
	  Variable* variable = (*p)->var_value();
	  if (!variable->is_global() || variable->init() == NULL)
	    ;
	  else if (variable->type()->interface_type() != NULL)
	    this->need_init_fn_ = true;
	  else if (variable->init()->is_constant())
	    ;
	  else if (!variable->init()->is_composite_literal())
	    this->need_init_fn_ = true;
	  else if (variable->init()->is_nonconstant_composite_literal())
	    this->need_init_fn_ = true;
	}
    }

  // Determine the types of constants in packages.
  for (Packages::const_iterator p = this->packages_.begin();
       p != this->packages_.end();
       ++p)
    p->second->determine_types();
}

// Traversal class used for type checking.

class Check_types_traverse : public Traverse
{
 public:
  Check_types_traverse(Gogo* gogo)
    : Traverse(traverse_variables
	       | traverse_constants
	       | traverse_statements
	       | traverse_expressions),
      gogo_(gogo)
  { }

  int
  variable(Named_object*);

  int
  constant(Named_object*, bool);

  int
  statement(Block*, size_t* pindex, Statement*);

  int
  expression(Expression**);

 private:
  // General IR.
  Gogo* gogo_;
};

// Check that a variable initializer has the right type.

int
Check_types_traverse::variable(Named_object* named_object)
{
  if (named_object->is_variable())
    {
      Variable* var = named_object->var_value();
      Expression* init = var->init();
      std::string reason;
      if (init != NULL
	  && !Type::are_compatible_for_assign(var->type(), init->type(),
					      &reason))
	{
	  if (reason.empty())
	    error_at(var->location(), "incompatible type in initialization");
	  else
	    error_at(var->location(),
		     "incompatible type in initialization (%s)",
		     reason.c_str());
	  var->clear_init();
	}
    }
  return TRAVERSE_CONTINUE;
}

// Check that a constant initializer has the right type.

int
Check_types_traverse::constant(Named_object* named_object, bool)
{
  Named_constant* constant = named_object->const_value();
  if (!constant->expr()->is_constant())
    error_at(constant->expr()->location(), "expression is not constant");
  else if (!Type::are_compatible_for_assign(constant->type(),
					    constant->expr()->type(), NULL))
    error_at(constant->location(), "initialization expression has wrong type");
  return TRAVERSE_CONTINUE;
}

// Check that types are valid in a statement.

int
Check_types_traverse::statement(Block*, size_t*, Statement* s)
{
  s->check_types(this->gogo_);
  return TRAVERSE_CONTINUE;
}

// Check that types are valid in an expression.

int
Check_types_traverse::expression(Expression** expr)
{
  (*expr)->check_types(this->gogo_);
  return TRAVERSE_CONTINUE;
}

// Check that types are valid.

void
Gogo::check_types()
{
  Check_types_traverse traverse(this);
  this->traverse(&traverse);
}

// Check the types in a single block.

void
Gogo::check_types_in_block(Block* block)
{
  Check_types_traverse traverse(this);
  block->traverse(&traverse);
}

// A dump for find_only_arg_vars.

static Go_dump find_only_arg_vars_dump("argvars");

// A traversal class to find all the local variables.

class Traverse_find_vars : public Traverse
{
 public:
  Traverse_find_vars(std::list<Named_object*>* vars)
    : Traverse(traverse_variables),
      vars_(vars)
  { }

 protected:
  int
  variable(Named_object* var)
  {
    if (var->is_variable() && !var->var_value()->is_global())
      this->vars_->push_back(var);
    return TRAVERSE_CONTINUE;
  }

 private:
  // We add variables to this list.
  std::list<Named_object*>* vars_;
};

// Find variables which are only set to argument values.

void
Gogo::find_only_arg_vars()
{
  std::list<Named_object*> vars;
  Traverse_find_vars tfv(&vars);
  this->traverse(&tfv);

  Dataflow dataflow;
  dataflow.initialize(this);

  // We start with all the variables.  As we find variables which are
  // set to values which are not arguments, we remove them from the
  // list.  Note that this is flow-insensitive; we could in some cases
  // do slightly better by being flow-sensitive.

  bool changed = true;
  while (changed)
    {
      changed = false;
      std::list<Named_object*>::iterator pv = vars.begin();
      while (pv != vars.end())
	{
	  const Dataflow::Defs* defs = dataflow.find_defs(*pv);
	  if (defs == NULL)
	    {
	      // The variable is never set.
	      gcc_assert((*pv)->var_value()->is_parameter());
	      ++pv;
	    }
	  else
	    {
	      Dataflow::Defs::const_iterator pd;
	      for (pd = defs->begin(); pd != defs->end(); ++pd)
		{
		  if (pd->val == NULL)
		    {
		      if (!pd->is_init)
			break;
		    }
		  else if (pd->val->var_expression() == NULL)
		    break;
		  else
		    {
		      Var_expression* ve = pd->val->var_expression();
		      Named_object* source = ve->named_object();
		      if (std::find(vars.begin(), vars.end(), source)
			  == vars.end())
			break;
		    }
		}
	      if (pd == defs->end())
		++pv;
	      else
		{
		  // This variable is set to something which is not an
		  // argument value.
		  pv = vars.erase(pv);
		  changed = true;
		}
	    }
	}
    }

  for (std::list<Named_object*>::const_iterator pv = vars.begin();
       pv != vars.end();
       ++pv)
    {
      (*pv)->var_value()->set_holds_only_args();

      // FIXME: This should go to a file.
      if (find_only_arg_vars_dump.is_enabled())
	inform((*pv)->location(), "%qs: only args var", (*pv)->name().c_str());
    }
}

// Traversal class used to check for return statements.

class Check_return_statements_traverse : public Traverse
{
 public:
  Check_return_statements_traverse()
    : Traverse(traverse_functions)
  { }

  int
  function(Named_object*);
};

// Check that a function has a return statement if it needs one.

int
Check_return_statements_traverse::function(Named_object* no)
{
  Function* func = no->func_value();
  const Function_type* fntype = func->type();
  const Typed_identifier_list* results = fntype->results();

  // We only need a return statement if there is a return value.
  if (results == NULL || results->empty())
    return TRAVERSE_CONTINUE;

  if (func->block()->may_fall_through())
    error_at(func->location(), "control reaches end of non-void function");

  return TRAVERSE_CONTINUE;
}

// Check return statements.

void
Gogo::check_return_statements()
{
  Check_return_statements_traverse traverse;
  this->traverse(&traverse);
}

// Work out the package priority.  It is one more than the maximum
// priority of an imported package.

int
Gogo::package_priority() const
{
  int priority = 0;
  for (Packages::const_iterator p = this->packages_.begin();
       p != this->packages_.end();
       ++p)
    if (p->second->priority() > priority)
      priority = p->second->priority();
  return priority + 1;
}

// Export identifiers as requested.

void
Gogo::do_exports()
{
  // For now we always stream to a section.  Later we may want to
  // support streaming to a separate file.
  Stream_to_section stream;

  Export exp(&stream);
  exp.register_builtin_types(this);
  exp.export_globals(this->package_name(),
		     this->package_priority(),
		     (this->need_init_fn_ && this->package_name() != "main"
		      ? this->get_init_fn_name()
		      : ""),
		     this->imported_init_fns_,
		     this->package_->bindings());
}

// Class Function.

Function::Function(Function_type* type, Function* enclosing, Block* block,
		   source_location location)
  : type_(type), enclosing_(enclosing), closure_var_(NULL), refcounts_(NULL),
    block_(block), location_(location), fndecl_(NULL), return_value_(NULL),
    defer_stack_(NULL)
{
}

// Return the closure variable, creating it if necessary.

Named_object*
Function::closure_var()
{
  if (this->closure_var_ == NULL)
    {
      // We don't know the type of the variable yet.  We add fields as
      // we find them.
      source_location loc = this->type_->location();
      Struct_field_list* sfl = new Struct_field_list;
      Type* struct_type = Type::make_struct_type(sfl, loc);
      Variable* var = new Variable(Type::make_pointer_type(struct_type),
				   NULL, false, true, false, loc);
      this->closure_var_ = Named_object::make_variable("closure", NULL, var);
      // Note that the new variable is not in any binding contour.
    }
  return this->closure_var_;
}

// Set the type of the closure variable.

void
Function::set_closure_type()
{
  if (this->closure_var_ == NULL)
    return;
  Named_object* closure = this->closure_var_;
  Struct_type* st = closure->var_value()->type()->deref()->struct_type();
  unsigned int index = 0;
  for (Closure_fields::const_iterator p = this->closure_fields_.begin();
       p != this->closure_fields_.end();
       ++p, ++index)
    {
      Named_object* no = p->first;
      char buf[20];
      snprintf(buf, sizeof buf, "%u", index);
      std::string n = no->name() + buf;
      Type* var_type = no->var_value()->type();
      Type* field_type = Type::make_pointer_type(var_type);
      st->push_field(Struct_field(Typed_identifier(n, field_type, p->second)));
    }
}

// Allocate reference count information for the function.

Refcounts*
Function::refcounts()
{
  if (this->refcounts_ == NULL)
    this->refcounts_ = new Refcounts;
  return this->refcounts_;
}

// Return whether this function is a method.

bool
Function::is_method() const
{
  return this->type_->is_method();
}

// Add a label definition.

Label*
Function::add_label_definition(const std::string& label_name,
			       source_location location)
{
  Label* lnull = NULL;
  std::pair<Labels::iterator, bool> ins =
    this->labels_.insert(std::make_pair(label_name, lnull));
  if (ins.second)
    {
      // This is a new label.
      Label* label = new Label(label_name);
      label->define(location);
      ins.first->second = label;
      return label;
    }
  else
    {
      // The label was already in the hash table.
      Label* label = ins.first->second;
      if (!label->is_defined())
	{
	  label->define(location);
	  return label;
	}
      else
	{
	  error_at(location, "redefinition of label %qs", label_name.c_str());
	  inform(label->location(), "previous definition of %qs was here",
		 label_name.c_str());
	  return new Label(label_name);
	}
    }
}

// Add a reference to a label.

Label*
Function::add_label_reference(const std::string& label_name)
{
  Label* lnull = NULL;
  std::pair<Labels::iterator, bool> ins =
    this->labels_.insert(std::make_pair(label_name, lnull));
  if (!ins.second)
    {
      // The label was already in the hash table.
      return ins.first->second;
    }
  else
    {
      gcc_assert(ins.first->second == NULL);
      Label* label = new Label(label_name);
      ins.first->second = label;
      return label;
    }
}

// Traverse the tree.

int
Function::traverse(Traverse* traverse)
{
  unsigned int traverse_mask = traverse->traverse_mask();

  // FIXME: We should check traverse_functions here if nested
  // functions are stored in block bindings.
  if (this->block_ != NULL
      && (traverse_mask
	  & (Traverse::traverse_variables
	     | Traverse::traverse_constants
	     | Traverse::traverse_blocks
	     | Traverse::traverse_statements
	     | Traverse::traverse_expressions
	     | Traverse::traverse_types)) != 0)
    {
      if (this->block_->traverse(traverse) == TRAVERSE_EXIT)
	return TRAVERSE_EXIT;
    }

  return TRAVERSE_CONTINUE;
}

// Work out types for unspecified variables and constants.

void
Function::determine_types()
{
  if (this->block_ != NULL)
    this->block_->determine_types();
}

// Export the function.

void
Function::export_func(Export* exp, const std::string& name) const
{
  Function::export_func_with_type(exp, name, this->type_);
}

// Export a function with a type.

void
Function::export_func_with_type(Export* exp, const std::string& name,
				const Function_type* fntype)
{
  exp->write_c_string("func ");

  if (fntype->is_method())
    {
      exp->write_c_string("(");
      exp->write_type(fntype->receiver()->type());
      exp->write_c_string(") ");
    }

  exp->write_string(name);

  exp->write_c_string(" (");
  const Typed_identifier_list* parameters = fntype->parameters();
  if (parameters != NULL)
    {
      bool first = true;
      for (Typed_identifier_list::const_iterator p = parameters->begin();
	   p != parameters->end();
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

  const Typed_identifier_list* results = fntype->results();
  if (results != NULL)
    {
      if (results->size() == 1)
	{
	  exp->write_c_string(" ");
	  exp->write_type(results->begin()->type());
	}
      else
	{
	  exp->write_c_string(" (");
	  bool first = true;
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
  exp->write_c_string(";\n");
}

// Import a function.

void
Function::import_func(Import* imp, std::string* pname,
		      Typed_identifier** preceiver,
		      Typed_identifier_list** pparameters,
		      Typed_identifier_list** presults,
		      bool* is_varargs)
{
  imp->require_c_string("func ");

  *preceiver = NULL;
  if (imp->peek_char() == '(')
    {
      imp->require_c_string("(");
      Type* rtype = imp->read_type();
      *preceiver = new Typed_identifier(Import::import_marker, rtype,
					imp->location());
      imp->require_c_string(") ");
    }

  *pname = imp->read_identifier();

  Typed_identifier_list* parameters;
  *is_varargs = false;
  imp->require_c_string(" (");
  if (imp->peek_char() == ')')
    parameters = NULL;
  else
    {
      parameters = new Typed_identifier_list();
      while (true)
	{
	  Type* ptype = imp->read_type();
	  if (ptype->is_varargs_type())
	    *is_varargs = true;
	  parameters->push_back(Typed_identifier(Import::import_marker,
						 ptype, imp->location()));
	  if (imp->peek_char() != ',')
	    break;
	  imp->require_c_string(", ");
	}
    }
  imp->require_c_string(")");
  *pparameters = parameters;

  Typed_identifier_list* results;
  if (imp->peek_char() != ' ')
    results = NULL;
  else
    {
      results = new Typed_identifier_list();
      imp->require_c_string(" ");
      if (imp->peek_char() != '(')
	{
	  Type* rtype = imp->read_type();
	  results->push_back(Typed_identifier(Import::import_marker, rtype,
					      imp->location()));
	}
      else
	{
	  imp->require_c_string("(");
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
  imp->require_c_string(";\n");
  *presults = results;
}

// Class Block.

Block::Block(Block* enclosing, source_location location)
  : enclosing_(enclosing), statements_(),
    bindings_(new Bindings(enclosing == NULL
			   ? NULL
			   : enclosing->bindings())),
    final_statements_(),
    start_location_(location),
    end_location_(UNKNOWN_LOCATION)
{
}

// Add a statement to a block.

void
Block::add_statement(Statement* statement)
{
  this->statements_.push_back(statement);
}

// Add a statement to the front of a block.  This is slow but is only
// used for reference counts of parameters.

void
Block::add_statement_at_front(Statement* statement)
{
  this->statements_.insert(this->statements_.begin(), statement);
}

// Add a statement which runs when the block is complete.

void
Block::add_final_statement(Statement* statement)
{
  this->final_statements_.push_back(statement);
}

// Replace a statement in a block.

void
Block::replace_statement(size_t index, Statement* s)
{
  gcc_assert(index < this->statements_.size());
  this->statements_[index] = s;
}

// Add a statement before another statement.

void
Block::insert_statement_before(size_t index, Statement* s)
{
  this->statements_.insert(this->statements_.begin() + index, s);
}

// Traverse the tree.

int
Block::traverse(Traverse* traverse)
{
  unsigned int traverse_mask = traverse->traverse_mask();

  if ((traverse_mask & Traverse::traverse_blocks) != 0)
    {
      int t = traverse->block(this);
      if (t == TRAVERSE_EXIT)
	return TRAVERSE_EXIT;
      else if (t == TRAVERSE_SKIP_COMPONENTS)
	return TRAVERSE_CONTINUE;
    }

  if ((traverse_mask
       & (Traverse::traverse_variables
	  | Traverse::traverse_constants
	  | Traverse::traverse_expressions
	  | Traverse::traverse_types)) != 0)
    {
      for (Bindings::const_definitions_iterator pb =
	     this->bindings_->begin_definitions();
	   pb != this->bindings_->end_definitions();
	   ++pb)
	{
	  switch ((*pb)->classification())
	    {
	    case Named_object::NAMED_OBJECT_CONST:
	      if ((traverse_mask & Traverse::traverse_constants) != 0)
		{
		  if (traverse->constant(*pb, false) == TRAVERSE_EXIT)
		    return TRAVERSE_EXIT;
		}
	      if ((traverse_mask & Traverse::traverse_types) != 0
		  || (traverse_mask & Traverse::traverse_expressions) != 0)
		{
		  Type* t = (*pb)->const_value()->type();
		  if (t != NULL
		      && Type::traverse(t, traverse) == TRAVERSE_EXIT)
		    return TRAVERSE_EXIT;
		}
	      if ((traverse_mask & Traverse::traverse_expressions) != 0
		  || (traverse_mask & Traverse::traverse_types) != 0)
		{
		  if ((*pb)->const_value()->traverse_expression(traverse)
		      == TRAVERSE_EXIT)
		    return TRAVERSE_EXIT;
		}
	      break;

	    case Named_object::NAMED_OBJECT_VAR:
	    case Named_object::NAMED_OBJECT_RESULT_VAR:
	      if ((traverse_mask & Traverse::traverse_variables) != 0)
		{
		  if (traverse->variable(*pb) == TRAVERSE_EXIT)
		    return TRAVERSE_EXIT;
		}
	      if (((traverse_mask & Traverse::traverse_types) != 0
		   || (traverse_mask & Traverse::traverse_expressions) != 0)
		  && ((*pb)->is_result_variable()
		      || (*pb)->var_value()->has_type()))
		{
		  Type* t = ((*pb)->is_variable()
			     ? (*pb)->var_value()->type()
			     : (*pb)->result_var_value()->type());
		  if (t != NULL
		      && Type::traverse(t, traverse) == TRAVERSE_EXIT)
		    return TRAVERSE_EXIT;
		}
	      if ((*pb)->is_variable()
		  && ((traverse_mask & Traverse::traverse_expressions) != 0
		      || (traverse_mask & Traverse::traverse_types) != 0))
		{
		  if ((*pb)->var_value()->traverse_expression(traverse)
		      == TRAVERSE_EXIT)
		    return TRAVERSE_EXIT;
		}
	      break;

	    case Named_object::NAMED_OBJECT_FUNC:
	    case Named_object::NAMED_OBJECT_FUNC_DECLARATION:
	      // FIXME: Where will nested functions be found?
	      gcc_unreachable();

	    case Named_object::NAMED_OBJECT_TYPE:
	      if ((traverse_mask & Traverse::traverse_types) != 0
		  || (traverse_mask & Traverse::traverse_expressions) != 0)
		{
		  if (Type::traverse((*pb)->type_value(), traverse)
		      == TRAVERSE_EXIT)
		    return TRAVERSE_EXIT;
		}
	      break;

	    case Named_object::NAMED_OBJECT_TYPE_DECLARATION:
	    case Named_object::NAMED_OBJECT_UNKNOWN:
	      break;

	    case Named_object::NAMED_OBJECT_PACKAGE:
	    case Named_object::NAMED_OBJECT_SINK:
	      gcc_unreachable();

	    default:
	      gcc_unreachable();
	    }
	}
    }

  // No point in checking traverse_mask here--if we got here we always
  // want to walk the statements.  The traversal can insert new
  // statements, but only before the current statement.
  for (size_t i = 0; i < this->statements_.size(); ++i)
    {
      if (this->statements_[i]->traverse(this, &i, traverse) == TRAVERSE_EXIT)
	return TRAVERSE_EXIT;
    }

  return TRAVERSE_CONTINUE;
}

// Work out types for unspecified variables and constants.

void
Block::determine_types()
{
  for (Bindings::const_definitions_iterator pb =
	 this->bindings_->begin_definitions();
       pb != this->bindings_->end_definitions();
       ++pb)
    {
      if ((*pb)->is_variable())
	(*pb)->var_value()->determine_type();
      else if ((*pb)->is_const())
	(*pb)->const_value()->determine_type();
    }

  for (std::vector<Statement*>::const_iterator ps = this->statements_.begin();
       ps != this->statements_.end();
       ++ps)
    (*ps)->determine_types();
}

// Return true if the statements in this block may fall through.

bool
Block::may_fall_through() const
{
  if (this->statements_.empty())
    return true;
  return this->statements_.back()->may_fall_through();
}

// Class Variable.

Variable::Variable(Type* type, Expression* init, bool is_global,
		   bool is_parameter, bool is_receiver,
		   source_location location)
  : type_(type), init_(init), location_(location), is_global_(is_global),
    is_parameter_(is_parameter), is_receiver_(is_receiver),
    is_varargs_parameter_(false), is_address_taken_(false),
    holds_only_args_(false), init_is_lowered_(false),
    type_from_init_tuple_(false), type_from_range_index_(false),
    type_from_range_value_(false), type_from_chan_element_(false),
    is_type_switch_var_(false)
{
  gcc_assert(type != NULL || init != NULL);
  gcc_assert(!is_parameter || init == NULL);
}

// Traverse the initializer expression.

int
Variable::traverse_expression(Traverse* traverse)
{
  if (this->init_ == NULL)
    return TRAVERSE_CONTINUE;
  return Expression::traverse(&this->init_, traverse);
}

// Lower the initialization expression after parsing is complete.

void
Variable::lower_init_expression(Gogo* gogo)
{
  if (this->init_ != NULL && !this->init_is_lowered_)
    {
      Lower_parse_tree lower_parse_tree(gogo);
      lower_parse_tree.expression(&this->init_);
      this->init_is_lowered_ = true;
    }
}

// In an assignment which sets a variable to a tuple of EXPR, return
// the type of the first element of the tuple.

Type*
Variable::type_from_tuple(Expression* expr, bool report_error) const
{
  if (expr->map_index_expression() != NULL)
    return expr->map_index_expression()->get_map_type()->val_type();
  else if (expr->receive_expression() != NULL)
    {
      Expression* channel = expr->receive_expression()->channel();
      return channel->type()->channel_type()->element_type();
    }
  else
    {
      if (report_error)
	error_at(this->location(), "invalid tuple definition");
      return Type::make_error_type();
    }
}

// Given EXPR used in a range clause, return either the index type or
// the value type of the range, depending upon GET_INDEX_TYPE.

Type*
Variable::type_from_range(Expression* expr, bool get_index_type,
			  bool report_error) const
{
  Type* t = expr->type();
  if (t->array_type() != NULL
      || (t->points_to() != NULL
	  && t->points_to()->array_type() != NULL
	  && !t->points_to()->is_open_array_type()))
    {
      if (get_index_type)
	return Type::lookup_integer_type("int");
      else
	return t->deref()->array_type()->element_type();
    }
  else if (t->is_string_type())
    return Type::lookup_integer_type("int");
  else if (t->map_type() != NULL)
    {
      if (get_index_type)
	return t->map_type()->key_type();
      else
	return t->map_type()->val_type();
    }
  else if (t->channel_type() != NULL)
    {
      if (get_index_type)
	return t->channel_type()->element_type();
      else
	{
	  if (report_error)
	    error_at(this->location(),
		     "invalid definition of value variable for channel range");
	  return Type::make_error_type();
	}
    }
  else
    {
      if (report_error)
	error_at(this->location(), "invalid type for range clause");
      return Type::make_error_type();
    }
}

// EXPR should be a channel.  Return the channel's element type.

Type*
Variable::type_from_chan_element(Expression* expr, bool report_error) const
{
  Type* t = expr->type();
  if (t->channel_type() != NULL)
    return t->channel_type()->element_type();
  else
    {
      if (report_error)
	error_at(this->location(), "expected channel");
      return Type::make_error_type();
    }
}

// Return the type of the Variable.  This may be called before
// Variable::determine_type is called, which means that we may need to
// get the type from the initializer.  FIXME: If we combine lowering
// with type determination, then this should be unnecessary.

Type*
Variable::type() const
{
  // A variable in a type switch with a nil case will have the wrong
  // type here.  This gets fixed up in determine_type, below.
  Type* type = this->type_;
  Expression* init = this->init_;
  if (this->is_type_switch_var_
      && this->type_->is_nil_constant_as_type())
    {
      Type_guard_expression* tge = this->init_->type_guard_expression();
      gcc_assert(tge != NULL);
      init = tge->expr();
      type = NULL;
    }

  if (type != NULL)
    return type;
  else if (this->type_from_init_tuple_)
    return this->type_from_tuple(init, false);
  else if (this->type_from_range_index_ || this->type_from_range_value_)
    return this->type_from_range(init, this->type_from_range_index_, false);
  else if (this->type_from_chan_element_)
    return this->type_from_chan_element(init, false);
  else
    {
      gcc_assert(init != NULL);
      type = init->type();
      gcc_assert(type != NULL);

      // Variables should not have abstract types.
      if (type->is_abstract())
	type = type->make_non_abstract_type();

      if (type->is_void_type())
	type = Type::make_error_type();

      return type;
    }
}

// Set the type if necessary.

void
Variable::determine_type()
{
  // A variable in a type switch with a nil case will have the wrong
  // type here.  It will have an initializer which is a type guard.
  // We want to initialize it to the value without the type guard, and
  // use the type of that value as well.
  if (this->is_type_switch_var_ && this->type_->is_nil_constant_as_type())
    {
      Type_guard_expression* tge = this->init_->type_guard_expression();
      gcc_assert(tge != NULL);
      this->type_ = NULL;
      this->init_ = tge->expr();
    }

  if (this->init_ == NULL)
    gcc_assert(this->type_ != NULL && !this->type_->is_abstract());
  else if (this->type_from_init_tuple_)
    {
      Expression *init = this->init_;
      init->determine_type_no_context();
      this->type_ = this->type_from_tuple(init, true);
      this->init_ = NULL;
    }
  else if (this->type_from_range_index_ || this->type_from_range_value_)
    {
      Expression* init = this->init_;
      init->determine_type_no_context();
      this->type_ = this->type_from_range(init, this->type_from_range_index_,
					  true);
      this->init_ = NULL;
    }
  else if (this->type_from_chan_element_)
    {
      Expression* init = this->init_;
      init->determine_type_no_context();
      this->type_ = this->type_from_chan_element(init, true);
      this->init_ = NULL;
    }
  else
    {
      Type_context context(this->type_, false);
      this->init_->determine_type(&context);
      if (this->type_ == NULL)
	{
	  Type* type = this->init_->type();
	  gcc_assert(type != NULL);
	  if (type->is_abstract())
	    type = type->make_non_abstract_type();

	  if (type->is_void_type())
	    {
	      error_at(this->location_, "variable has no type");
	      type = Type::make_error_type();
	    }
	  else if (type->is_nil_type())
	    {
	      error_at(this->location_, "variable defined to nil type");
	      type = Type::make_error_type();
	    }
	  else if (type->is_call_multiple_result_type())
	    {
	      error_at(this->location_,
		       "single variable set to multiple value function call");
	      type = Type::make_error_type();
	    }

	  this->type_ = type;
	}
    }
}

// Export the variable

void
Variable::export_var(Export* exp, const std::string& name) const
{
  gcc_assert(this->is_global_);
  exp->write_c_string("var ");
  exp->write_string(name);
  exp->write_c_string(" ");
  exp->write_type(this->type());
  exp->write_c_string(";\n");
}

// Import a variable.

void
Variable::import_var(Import* imp, std::string* pname, Type** ptype)
{
  imp->require_c_string("var ");
  *pname = imp->read_identifier();
  imp->require_c_string(" ");
  *ptype = imp->read_type();
  imp->require_c_string(";\n");
}

// Class Named_constant.

// Traverse the initializer expression.

int
Named_constant::traverse_expression(Traverse* traverse)
{
  return Expression::traverse(&this->expr_, traverse);
}

// Determine the type of the constant.

void
Named_constant::determine_type()
{
  if (this->type_ != NULL)
    {
      Type_context context(this->type_, false);
      this->expr_->determine_type(&context);
    }
  else
    {
      // A constant may have an abstract type.
      Type_context context(NULL, true);
      this->expr_->determine_type(&context);
      this->type_ = this->expr_->type();
      gcc_assert(this->type_ != NULL);
    }
}

// Export a constant.

void
Named_constant::export_const(Export* exp, const std::string& name) const
{
  exp->write_c_string("const ");
  exp->write_string(name);
  exp->write_c_string(" ");
  if (!this->type_->is_abstract())
    {
      exp->write_type(this->type_);
      exp->write_c_string(" ");
    }
  exp->write_c_string("= ");
  this->expr()->export_expression(exp);
  exp->write_c_string(";\n");
}

// Import a constant.

void
Named_constant::import_const(Import* imp, std::string* pname, Type** ptype,
			     Expression** pexpr)
{
  imp->require_c_string("const ");
  *pname = imp->read_identifier();
  imp->require_c_string(" ");
  if (imp->peek_char() == '=')
    *ptype = NULL;
  else
    {
      *ptype = imp->read_type();
      imp->require_c_string(" ");
    }
  imp->require_c_string("= ");
  *pexpr = Expression::import_expression(imp);
  imp->require_c_string(";\n");
}

// Add a method.

Named_object*
Type_declaration::add_method(const std::string& name, Function* function)
{
  Named_object* ret = Named_object::make_function(name, NULL, function);
  this->methods_.push_back(ret);
  return ret;
}

// Add a method declaration.

Named_object*
Type_declaration::add_method_declaration(const std::string&  name,
					 Function_type* type,
					 source_location location)
{
  Named_object* ret = Named_object::make_function_declaration(name, NULL, type,
							      location);
  this->methods_.push_back(ret);
  return ret;
}

// Return whether any methods ere defined.

bool
Type_declaration::has_methods() const
{
  return !this->methods_.empty();
}

// Define methods for the real type.

void
Type_declaration::define_methods(Named_type* nt)
{
  for (Methods::const_iterator p = this->methods_.begin();
       p != this->methods_.end();
       ++p)
    nt->add_existing_method(*p);
}

// We are using the type.  Return true if we should issue a warning.

bool
Type_declaration::using_type()
{
  bool ret = !this->issued_warning_;
  this->issued_warning_ = true;
  return ret;
}

// Class Unknown_name.

// Set the real named object.

void
Unknown_name::set_real_named_object(Named_object* no)
{
  gcc_assert(this->real_named_object_ == NULL);
  gcc_assert(!no->is_unknown());
  this->real_named_object_ = no;
}

// Class Named_object.

Named_object::Named_object(const std::string& name,
			   const Package* package,
			   Classification classification)
  : name_(name), package_(package), classification_(classification),
    tree_(NULL)
{
  if (Gogo::is_sink_name(name))
    gcc_assert(classification == NAMED_OBJECT_SINK);
}

// Make an unknown name.  This is used by the parser.  The name must
// be resolved later.  Unknown names are only added in the current
// package.

Named_object*
Named_object::make_unknown_name(const std::string& name,
				source_location location)
{
  Named_object* named_object = new Named_object(name, NULL,
						NAMED_OBJECT_UNKNOWN);
  Unknown_name* value = new Unknown_name(location);
  named_object->u_.unknown_value = value;
  return named_object;
}

// Make a constant.

Named_object*
Named_object::make_constant(const Typed_identifier& tid,
			    const Package* package, Expression* expr,
			    int iota_value)
{
  Named_object* named_object = new Named_object(tid.name(), package,
						NAMED_OBJECT_CONST);
  Named_constant* named_constant = new Named_constant(tid.type(), expr,
						      iota_value,
						      tid.location());
  named_object->u_.const_value = named_constant;
  return named_object;
}

// Make a named type.

Named_object*
Named_object::make_type(const std::string& name, const Package* package,
			Type* type, source_location location)
{
  Named_object* named_object = new Named_object(name, package,
						NAMED_OBJECT_TYPE);
  Named_type* named_type = Type::make_named_type(named_object, type, location);
  named_object->u_.type_value = named_type;
  return named_object;
}

// Make a type declaration.

Named_object*
Named_object::make_type_declaration(const std::string& name,
				    const Package* package,
				    source_location location)
{
  Named_object* named_object = new Named_object(name, package,
						NAMED_OBJECT_TYPE_DECLARATION);
  Type_declaration* type_declaration = new Type_declaration(location);
  named_object->u_.type_declaration = type_declaration;
  return named_object;
}

// Make a variable.

Named_object*
Named_object::make_variable(const std::string& name, const Package* package,
			    Variable* variable)
{
  Named_object* named_object = new Named_object(name, package,
						NAMED_OBJECT_VAR);
  named_object->u_.var_value = variable;
  return named_object;
}

// Make a result variable.

Named_object*
Named_object::make_result_variable(const std::string& name,
				   Result_variable* result)
{
  Named_object* named_object = new Named_object(name, NULL,
						NAMED_OBJECT_RESULT_VAR);
  named_object->u_.result_var_value = result;
  return named_object;
}

// Make a sink.  This is used for the special blank identifier _.

Named_object*
Named_object::make_sink()
{
  return new Named_object("_", NULL, NAMED_OBJECT_SINK);
}

// Make a named function.

Named_object*
Named_object::make_function(const std::string& name, const Package* package,
			    Function* function)
{
  Named_object* named_object = new Named_object(name, package,
						NAMED_OBJECT_FUNC);
  named_object->u_.func_value = function;
  return named_object;
}

// Make a function declaration.

Named_object*
Named_object::make_function_declaration(const std::string& name,
					const Package* package,
					Function_type* fntype,
					source_location location)
{
  Named_object* named_object = new Named_object(name, package,
						NAMED_OBJECT_FUNC_DECLARATION);
  Function_declaration *func_decl = new Function_declaration(fntype, location);
  named_object->u_.func_declaration_value = func_decl;
  return named_object;
}

// Make a package.

Named_object*
Named_object::make_package(const std::string& alias, Package* package)
{
  Named_object* named_object = new Named_object(alias, NULL,
						NAMED_OBJECT_PACKAGE);
  named_object->u_.package_value = package;
  return named_object;
}

// Set the type when a declaration is defined.

void
Named_object::set_type_value(Named_type* named_type)
{
  gcc_assert(this->classification_ == NAMED_OBJECT_TYPE_DECLARATION);
  Type_declaration* td = this->u_.type_declaration;
  td->define_methods(named_type);
  Named_object* in_function = td->in_function();
  if (in_function != NULL)
    named_type->set_in_function(in_function);
  delete td;
  this->classification_ = NAMED_OBJECT_TYPE;
  this->u_.type_value = named_type;
}

// Define a function which was previously declared.

void
Named_object::set_function_value(Function* function)
{
  gcc_assert(this->classification_ == NAMED_OBJECT_FUNC_DECLARATION);
  this->classification_ = NAMED_OBJECT_FUNC;
  // FIXME: We should free the old value.
  this->u_.func_value = function;
}

// Return the location of a named object.

source_location
Named_object::location() const
{
  switch (this->classification_)
    {
    default:
    case NAMED_OBJECT_UNINITIALIZED:
      gcc_unreachable();

    case NAMED_OBJECT_UNKNOWN:
      return this->unknown_value()->location();

    case NAMED_OBJECT_CONST:
      return this->const_value()->location();

    case NAMED_OBJECT_TYPE:
      return this->type_value()->location();

    case NAMED_OBJECT_TYPE_DECLARATION:
      return this->type_declaration_value()->location();

    case NAMED_OBJECT_VAR:
      return this->var_value()->location();

    case NAMED_OBJECT_RESULT_VAR:
      return this->result_var_value()->function()->location();

    case NAMED_OBJECT_SINK:
      gcc_unreachable();

    case NAMED_OBJECT_FUNC:
      return this->func_value()->location();

    case NAMED_OBJECT_FUNC_DECLARATION:
      return this->func_declaration_value()->location();

    case NAMED_OBJECT_PACKAGE:
      return this->package_value()->location();
    }
}

// Export a named object.

void
Named_object::export_named_object(Export* exp) const
{
  switch (this->classification_)
    {
    default:
    case NAMED_OBJECT_UNINITIALIZED:
    case NAMED_OBJECT_UNKNOWN:
      gcc_unreachable();

    case NAMED_OBJECT_CONST:
      this->const_value()->export_const(exp, this->name_);
      break;

    case NAMED_OBJECT_TYPE:
      this->type_value()->export_named_type(exp, this->name_);
      break;

    case NAMED_OBJECT_TYPE_DECLARATION:
      error_at(this->type_declaration_value()->location(),
	       "attempt to export %<%s%> which was declared but not defined",
	       this->name_.c_str());
      break;

    case NAMED_OBJECT_FUNC_DECLARATION:
      this->func_declaration_value()->export_func(exp, this->name_);
      break;

    case NAMED_OBJECT_VAR:
      this->var_value()->export_var(exp, this->name_);
      break;

    case NAMED_OBJECT_RESULT_VAR:
    case NAMED_OBJECT_SINK:
      gcc_unreachable();

    case NAMED_OBJECT_FUNC:
      this->func_value()->export_func(exp, this->name_);
      break;
    }
}

// Class Bindings.

Bindings::Bindings(Bindings* enclosing)
  : enclosing_(enclosing), named_objects_(), bindings_()
{
}

// Clear imports.

void
Bindings::clear_file_scope()
{
  Contour::iterator p = this->bindings_.begin();
  while (p != this->bindings_.end())
    {
      bool keep;
      if (p->second->package() != NULL)
	keep = false;
      else if (p->second->is_package())
	keep = false;
      else if (p->second->is_function()
	       && !p->second->func_value()->type()->is_method()
	       && Gogo::unpack_hidden_name(p->second->name()) == "init")
	keep = false;
      else
	keep = true;

      if (keep)
	++p;
      else
	p = this->bindings_.erase(p);
    }
}

// Look up a symbol.

Named_object*
Bindings::lookup(const std::string& name) const
{
  Contour::const_iterator p = this->bindings_.find(name);
  if (p != this->bindings_.end())
    return p->second->resolve();
  else if (this->enclosing_ != NULL)
    return this->enclosing_->lookup(name);
  else
    return NULL;
}

// Look up a symbol locally.

Named_object*
Bindings::lookup_local(const std::string& name) const
{
  Contour::const_iterator p = this->bindings_.find(name);
  if (p == this->bindings_.end())
    return NULL;
  return p->second;
}

// Add a method to the list of objects.  This is not added to the
// lookup table.  This is so that we have a single list of objects
// declared at the top level, which we walk through when it's time to
// convert to trees.

void
Bindings::add_method(Named_object* method)
{
  this->named_objects_.push_back(method);
}

// Add a generic Named_object to a Contour.

Named_object*
Bindings::add_named_object_to_contour(Contour* contour,
				      Named_object* named_object)
{
  gcc_assert(named_object == named_object->resolve());
  const std::string& name(named_object->name());
  gcc_assert(!Gogo::is_sink_name(name));

  std::pair<Contour::iterator, bool> ins =
    contour->insert(std::make_pair(name, named_object));
  if (!ins.second)
    {
      // The name was already there.
      if (named_object->package() != NULL
	  && ins.first->second->package() == named_object->package()
	  && (ins.first->second->classification()
	      == named_object->classification()))
	{
	  // This is a second import of the same object.
	  return ins.first->second;
	}
      ins.first->second = this->new_definition(ins.first->second,
					       named_object);
      return ins.first->second;
    }
  else
    {
      // Don't push declarations on the list.  We push them on when
      // and if we find the definitions.  That way we genericize the
      // functions in order.
      if (!named_object->is_type_declaration()
	  && !named_object->is_function_declaration()
	  && !named_object->is_unknown())
	this->named_objects_.push_back(named_object);
      return named_object;
    }
}

// We had an existing named object OLD_OBJECT, and we've seen a new
// one NEW_OBJECT with the same name.  FIXME: This does not free the
// new object when we don't need it.

Named_object*
Bindings::new_definition(Named_object* old_object, Named_object* new_object)
{
  std::string reason;
  switch (old_object->classification())
    {
    default:
    case Named_object::NAMED_OBJECT_UNINITIALIZED:
      gcc_unreachable();

    case Named_object::NAMED_OBJECT_UNKNOWN:
      {
	Named_object* real = old_object->unknown_value()->real_named_object();
	if (real != NULL)
	  return this->new_definition(real, new_object);
	gcc_assert(!new_object->is_unknown());
	old_object->unknown_value()->set_real_named_object(new_object);
	if (!new_object->is_type_declaration()
	    && !new_object->is_function_declaration())
	  this->named_objects_.push_back(new_object);
	return new_object;
      }

    case Named_object::NAMED_OBJECT_CONST:
      break;

    case Named_object::NAMED_OBJECT_TYPE:
      if (new_object->is_type_declaration())
	return old_object;
      break;

    case Named_object::NAMED_OBJECT_TYPE_DECLARATION:
      if (new_object->is_type_declaration())
	return old_object;
      if (new_object->is_type())
	{
	  old_object->set_type_value(new_object->type_value());
	  new_object->type_value()->set_named_object(old_object);
	  this->named_objects_.push_back(old_object);
	  return old_object;
	}
      break;

    case Named_object::NAMED_OBJECT_VAR:
    case Named_object::NAMED_OBJECT_RESULT_VAR:
      break;

    case Named_object::NAMED_OBJECT_SINK:
      gcc_unreachable();

    case Named_object::NAMED_OBJECT_FUNC:
      if (new_object->is_function_declaration())
	{
	  if (!new_object->func_declaration_value()->asm_name().empty())
	    sorry("__asm__ for function definitions");
	  Function_type* old_type = old_object->func_value()->type();
	  Function_type* new_type =
	    new_object->func_declaration_value()->type();
	  if (old_type->is_valid_redeclaration(new_type, &reason))
	    return old_object;
	}
      break;

    case Named_object::NAMED_OBJECT_FUNC_DECLARATION:
      {
	Function_type* old_type = old_object->func_declaration_value()->type();
	if (new_object->is_function_declaration())
	  {
	    Function_type* new_type =
	      new_object->func_declaration_value()->type();
	    if (old_type->is_valid_redeclaration(new_type, &reason))
	      return old_object;
	  }
	if (new_object->is_function())
	  {
	    Function_type* new_type = new_object->func_value()->type();
	    if (old_type->is_valid_redeclaration(new_type, &reason))
	      {
		if (!old_object->func_declaration_value()->asm_name().empty())
		  sorry("__asm__ for function definitions");
		old_object->set_function_value(new_object->func_value());
		this->named_objects_.push_back(old_object);
		return old_object;
	      }
	  }
      }
      break;

    case Named_object::NAMED_OBJECT_PACKAGE:
      if (new_object->is_package()
	  && (old_object->package_value()->name()
	      == new_object->package_value()->name()))
	return old_object;

      break;
    }

  std::string n = Gogo::unpack_hidden_name(old_object->name());
  if (reason.empty())
    error_at(new_object->location(), "redefinition of %qs", n.c_str());
  else
    error_at(new_object->location(), "redefinition of %qs: %s", n.c_str(),
	     reason.c_str());

  inform(old_object->location(), "previous definition of %qs was here",
	 n.c_str());

  return old_object;
}

// Add a named type.

Named_object*
Bindings::add_named_type(Named_type* named_type)
{
  return this->add_named_object(named_type->named_object());
}

// Add a function.

Named_object*
Bindings::add_function(const std::string& name, const Package* package,
		       Function* function)
{
  return this->add_named_object(Named_object::make_function(name, package,
							    function));
}

// Add a function declaration.

Named_object*
Bindings::add_function_declaration(const std::string& name,
				   const Package* package,
				   Function_type* type,
				   source_location location)
{
  Named_object* no = Named_object::make_function_declaration(name, package,
							     type, location);
  return this->add_named_object(no);
}

// Define a type which was previously declared.

void
Bindings::define_type(Named_object* no, Named_type* type)
{
  no->set_type_value(type);
  this->named_objects_.push_back(no);
}

// Traverse bindings.

int
Bindings::traverse(Traverse* traverse, bool is_global)
{
  unsigned int traverse_mask = traverse->traverse_mask();

  // We don't use an iterator because we permit the traversal to add
  // new global objects.
  for (size_t i = 0; i < this->named_objects_.size(); ++i)
    {
      Named_object* p = this->named_objects_[i];
      switch (p->classification())
	{
	case Named_object::NAMED_OBJECT_CONST:
	  if ((traverse_mask & Traverse::traverse_constants) != 0)
	    {
	      if (traverse->constant(p, is_global) == TRAVERSE_EXIT)
		return TRAVERSE_EXIT;
	    }
	  if ((traverse_mask & Traverse::traverse_types) != 0
	      || (traverse_mask & Traverse::traverse_expressions) != 0)
	    {
	      Type* t = p->const_value()->type();
	      if (t != NULL
		  && Type::traverse(t, traverse) == TRAVERSE_EXIT)
		return TRAVERSE_EXIT;
	    }
	  if ((traverse_mask & Traverse::traverse_expressions) != 0)
	    {
	      if (p->const_value()->traverse_expression(traverse)
		  == TRAVERSE_EXIT)
		return TRAVERSE_EXIT;
	    }
	  break;

	case Named_object::NAMED_OBJECT_VAR:
	case Named_object::NAMED_OBJECT_RESULT_VAR:
	  if ((traverse_mask & Traverse::traverse_variables) != 0)
	    {
	      if (traverse->variable(p) == TRAVERSE_EXIT)
		return TRAVERSE_EXIT;
	    }
	  if (((traverse_mask & Traverse::traverse_types) != 0
	       || (traverse_mask & Traverse::traverse_expressions) != 0)
	      && (p->is_result_variable()
		  || p->var_value()->has_type()))
	    {
	      Type* t = (p->is_variable()
			 ? p->var_value()->type()
			 : p->result_var_value()->type());
	      if (t != NULL
		  && Type::traverse(t, traverse) == TRAVERSE_EXIT)
		return TRAVERSE_EXIT;
	    }
	  if (p->is_variable()
	      && (traverse_mask & Traverse::traverse_expressions) != 0)
	    {
	      if (p->var_value()->traverse_expression(traverse)
		  == TRAVERSE_EXIT)
		return TRAVERSE_EXIT;
	    }
	  break;

	case Named_object::NAMED_OBJECT_FUNC:
	  if ((traverse_mask & Traverse::traverse_functions) != 0)
	    {
	      int t = traverse->function(p);
	      if (t == TRAVERSE_EXIT)
		return TRAVERSE_EXIT;
	      else if (t == TRAVERSE_SKIP_COMPONENTS)
		break;
	    }

	  if ((traverse_mask
	       & (Traverse::traverse_variables
		  | Traverse::traverse_constants
		  | Traverse::traverse_functions
		  | Traverse::traverse_blocks
		  | Traverse::traverse_statements
		  | Traverse::traverse_expressions
		  | Traverse::traverse_types)) != 0)
	    {
	      if (p->func_value()->traverse(traverse) == TRAVERSE_EXIT)
		return TRAVERSE_EXIT;
	    }
	  break;

	case Named_object::NAMED_OBJECT_PACKAGE:
	  // These are traversed in Gogo::traverse.
	  gcc_assert(is_global);
	  break;

	case Named_object::NAMED_OBJECT_TYPE:
	  if ((traverse_mask & Traverse::traverse_types) != 0
	      || (traverse_mask & Traverse::traverse_expressions) != 0)
	    {
	      if (Type::traverse(p->type_value(), traverse) == TRAVERSE_EXIT)
		return TRAVERSE_EXIT;
	    }
	  break;

	case Named_object::NAMED_OBJECT_TYPE_DECLARATION:
	case Named_object::NAMED_OBJECT_FUNC_DECLARATION:
	case Named_object::NAMED_OBJECT_UNKNOWN:
	  break;

	case Named_object::NAMED_OBJECT_SINK:
	default:
	  gcc_unreachable();
	}
    }

  return TRAVERSE_CONTINUE;
}

// Class Package.

Package::Package(const std::string& name, source_location location)
  : name_(name), bindings_(new Bindings(NULL)), priority_(0),
    location_(location), being_compiled_(false)
{
}

// Set the priority.  We may see multiple priorities for an imported
// package; we want to use the largest one.

void
Package::set_priority(int priority)
{
  if (priority > this->priority_)
    this->priority_ = priority;
}

// Determine types of constants.  Everything else in a package
// (variables, function declarations) should already have a fixed
// type.  Constants may have abstract types.

void
Package::determine_types()
{
  Bindings* bindings = this->bindings_;
  for (Bindings::const_definitions_iterator p = bindings->begin_definitions();
       p != bindings->end_definitions();
       ++p)
    {
      if ((*p)->is_const())
	(*p)->const_value()->determine_type();
    }
}

// Class Traverse.

// Destructor.

Traverse::~Traverse()
{
  if (this->types_seen_ != NULL)
    delete this->types_seen_;
}

// Record that we are looking at a type, and return true if we have
// already seen it.

bool
Traverse::remember_type(const Type* type)
{
  gcc_assert((this->traverse_mask() & traverse_types) != 0
	     || (this->traverse_mask() & traverse_expressions) != 0);
  // We only have to remember named types, as they are the only ones
  // we can see multiple times in a traversal.
  if (type->classification() != Type::TYPE_NAMED)
    return false;
  if (this->types_seen_ == NULL)
    this->types_seen_ = new Types_seen();
  std::pair<Types_seen::iterator, bool> ins = this->types_seen_->insert(type);
  return !ins.second;
}

// The default versions of these functions should never be called: the
// traversal mask indicates which functions may be called.

int
Traverse::variable(Named_object*)
{
  gcc_unreachable();
}

int
Traverse::constant(Named_object*, bool)
{
  gcc_unreachable();
}

int
Traverse::function(Named_object*)
{
  gcc_unreachable();
}

int
Traverse::block(Block*)
{
  gcc_unreachable();
}

int
Traverse::statement(Block*, size_t*, Statement*)
{
  gcc_unreachable();
}

int
Traverse::expression(Expression**)
{
  gcc_unreachable();
}

int
Traverse::type(Type*)
{
  gcc_unreachable();
}
