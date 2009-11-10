// unsafe.cc -- Go frontend builtin unsafe package.

// Copyright 2009 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "go-system.h"

#include "types.h"
#include "gogo.h"

// Set up the builtin unsafe package.  This should probably be driven
// by a table.

void
Gogo::import_unsafe(const std::string& local_name, bool is_local_name_exported,
		    source_location location)
{
  location_t bloc = BUILTINS_LOCATION;

  // Importing the "unsafe" package automatically disables TBAA.
  flag_strict_aliasing = false;

  bool add_to_globals;
  Package* package = this->add_imported_package("unsafe", local_name,
						is_local_name_exported,
						location, &add_to_globals);

  Bindings* bindings = package->bindings();

  // The type may have already been created by an import.
  Named_object* no = package->bindings()->lookup("Pointer");
  if (no == NULL)
    {
      Type* type = Type::make_pointer_type(Type::make_void_type());
      no = bindings->add_type("Pointer", package, type, bloc);
    }
  else
    {
      gcc_assert(no->package() == package);
      gcc_assert(no->is_type());
      gcc_assert(no->type_value()->is_unsafe_pointer_type());
      no->type_value()->set_is_visible();
    }
  Named_type* pointer_type = no->type_value();

  Type* int_type = this->lookup_global("int")->type_value();

  // Sizeof.
  Typed_identifier_list* results = new Typed_identifier_list;
  results->push_back(Typed_identifier("", int_type, bloc));
  Function_type* fntype = Type::make_function_type(NULL, NULL, results, bloc);
  fntype->set_is_builtin();
  bindings->add_function_declaration("Sizeof", package, fntype, bloc);

  // Offsetof.
  results = new Typed_identifier_list;
  results->push_back(Typed_identifier("", int_type, bloc));
  fntype = Type::make_function_type(NULL, NULL, results, bloc);
  fntype->set_is_varargs();
  fntype->set_is_builtin();
  bindings->add_function_declaration("Offsetof", package, fntype, bloc);

  // Alignof.
  results = new Typed_identifier_list;
  results->push_back(Typed_identifier("", int_type, bloc));
  fntype = Type::make_function_type(NULL, NULL, results, bloc);
  fntype->set_is_varargs();
  fntype->set_is_builtin();
  no = bindings->add_function_declaration("Alignof", package, fntype, bloc);

  // Typeof.
  Typed_identifier_list* empty_methods = new Typed_identifier_list();
  Type* empty_interface = Type::make_interface_type(empty_methods, bloc);
  Typed_identifier_list* parameters = new Typed_identifier_list;
  parameters->push_back(Typed_identifier("i", empty_interface, bloc));
  results = new Typed_identifier_list;
  results->push_back(Typed_identifier("", empty_interface, bloc));
  fntype = Type::make_function_type(NULL, parameters, results, bloc);
  bindings->add_function_declaration("Typeof", package, fntype, bloc);

  // Reflect.
  parameters = new Typed_identifier_list;
  parameters->push_back(Typed_identifier("it", empty_interface, bloc));
  results = new Typed_identifier_list;
  results->push_back(Typed_identifier("", empty_interface, bloc));
  results->push_back(Typed_identifier("", pointer_type, bloc));
  fntype = Type::make_function_type(NULL, parameters, results, bloc);
  bindings->add_function_declaration("Reflect", package, fntype, bloc);

  // Unreflect.
  parameters = new Typed_identifier_list;
  parameters->push_back(Typed_identifier("typ", empty_interface, bloc));
  parameters->push_back(Typed_identifier("addr", pointer_type, bloc));
  results = new Typed_identifier_list;
  results->push_back(Typed_identifier("", empty_interface, bloc));
  fntype = Type::make_function_type(NULL, parameters, results, bloc);
  bindings->add_function_declaration("Unreflect", package, fntype, bloc);
}
