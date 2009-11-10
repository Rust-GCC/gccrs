// gogo-tree.cc -- convert Go frontend Gogo IR to gcc trees.

// Copyright 2009 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <algorithm>
#include <list>

#include "go-system.h"

#include <gmp.h>

extern "C"
{
#include "tree.h"
#include "gimple.h"
#include "tree-iterator.h"
#include "cgraph.h"
#include "langhooks.h"
#include "convert.h"
#include "output.h"
#include "tm_p.h"
#include "diagnostic.h"
}

#include "go-c.h"
#include "types.h"
#include "expressions.h"
#include "statements.h"
#include "refcount.h"
#include "gogo.h"

// Define a builtin function.

static void
define_builtin(built_in_function bcode, const char* name, tree fntype,
	       bool const_p)
{
  tree decl = add_builtin_function(name, fntype, bcode, BUILT_IN_NORMAL, NULL,
				   NULL_TREE);
  if (const_p)
    TREE_READONLY(decl) = 1;
  built_in_decls[bcode] = decl;
  implicit_built_in_decls[bcode] = decl;
}

// Create trees for implicit builtin functions.

void
Gogo::define_builtin_function_trees()
{
  /* We need to define the fetch_and_add functions, since we use them
     for ++ and --.  */
  tree t = go_type_for_size(BITS_PER_UNIT, 1);
  tree p = build_pointer_type(build_qualified_type(t, TYPE_QUAL_VOLATILE));
  define_builtin(BUILT_IN_ADD_AND_FETCH_1, "__sync_fetch_and_add_1",
		 build_function_type_list(t, p, t, NULL_TREE), false);

  t = go_type_for_size(BITS_PER_UNIT * 2, 1);
  p = build_pointer_type(build_qualified_type(t, TYPE_QUAL_VOLATILE));
  define_builtin (BUILT_IN_ADD_AND_FETCH_2, "__sync_fetch_and_add_2",
		  build_function_type_list(t, p, t, NULL_TREE), false);

  t = go_type_for_size(BITS_PER_UNIT * 4, 1);
  p = build_pointer_type(build_qualified_type(t, TYPE_QUAL_VOLATILE));
  define_builtin(BUILT_IN_ADD_AND_FETCH_4, "__sync_fetch_and_add_4",
		 build_function_type_list(t, p, t, NULL_TREE), false);

  t = go_type_for_size(BITS_PER_UNIT * 8, 1);
  p = build_pointer_type(build_qualified_type(t, TYPE_QUAL_VOLATILE));
  define_builtin(BUILT_IN_ADD_AND_FETCH_8, "__sync_fetch_and_add_8",
		 build_function_type_list(t, p, t, NULL_TREE), false);

  // We use __builtin_expect for magic import functions.
  define_builtin(BUILT_IN_EXPECT, "__builtin_expect",
		 build_function_type_list(long_integer_type_node,
					  long_integer_type_node,
					  long_integer_type_node,
					  NULL_TREE),
		 true);
}

// Get the name to use for the import control function.  If there is a
// global function or variable, then we know that that name must be
// unique in the link, and we use it as the basis for our name.

const std::string&
Gogo::get_init_fn_name()
{
  if (this->init_fn_name_.empty())
    {
      gcc_assert(this->package_ != NULL);
      std::string s = "Go.import.";
      Bindings* bindings = this->package_->bindings();
      for (Bindings::const_definitions_iterator p =
	     bindings->begin_definitions();
	   p != bindings->end_definitions();
	   ++p)
	{
	  if (((*p)->is_variable() || (*p)->is_function())
	      && (*p)->package() == NULL
	      && (!go_default_private || !Gogo::is_hidden_name((*p)->name())))
	    {
	      tree id = (*p)->get_id(this);
	      s.append(IDENTIFIER_POINTER(id), IDENTIFIER_LENGTH(id));
	      break;
	    }
	}

      if (s.empty())
	{
	  // This is the unlikely case when we have an init function but
	  // nothing to export.
	  tree name_tree = get_file_function_name("N");
	  s.append(IDENTIFIER_POINTER(name_tree), IDENTIFIER_LENGTH(name_tree));
	}

      this->init_fn_name_ = s;
    }

  return this->init_fn_name_;
}

// Add statements to INIT_STMT_LIST which run the initialization
// functions for imported packages.  This is only used for the "main"
// package.

void
Gogo::init_imports(tree* init_stmt_list)
{
  gcc_assert(this->package_name() == "main");

  if (this->imported_init_fns_.empty())
    return;

  tree fntype = build_function_type(void_type_node, void_list_node);

  // We must call them in increasing priority order.
  std::vector<Import_init> v;
  for (std::set<Import_init>::const_iterator p =
	 this->imported_init_fns_.begin();
       p != this->imported_init_fns_.end();
       ++p)
    v.push_back(*p);
  std::sort(v.begin(), v.end());

  for (std::vector<Import_init>::const_iterator p = v.begin();
       p != v.end();
       ++p)
    {
      tree decl = build_decl(UNKNOWN_LOCATION, FUNCTION_DECL,
			     get_identifier(p->name().c_str()),
			     fntype);
      TREE_PUBLIC(decl) = 1;
      DECL_EXTERNAL(decl) = 1;
      append_to_statement_list(build_call_expr(decl, 0), init_stmt_list);
    }
}

// Create the magic initialization function.  INIT_STMT_LIST is the
// code that it needs to run.

void
Gogo::write_initialization_function(tree init_stmt_list)
{
  // In the "main" package, we run the initialization function as a
  // constructor.  We use a constructor priority based on the package
  // priority to ensure that if there are multiple files in the "main"
  // package, the initialization functions are run in order.
  if (this->package_name() == "main")
    {
      int priority = (this->package_priority()
		      + MAX_RESERVED_INIT_PRIORITY
		      + 10);
      gcc_assert(priority <= MAX_INIT_PRIORITY);
      cgraph_build_static_cdtor('I', init_stmt_list, priority);
      return;
    }

  // Make sure that we thought we needed an initialization function,
  // as otherwise we will not have reported it in the export data.
  gcc_assert(this->need_init_fn_);

  // If there are multiple files in the "main" package, then it is
  // possible for the import function to be called more than once.
  // Use a guard variable to avoid double initialization.
  // Initialization is always single-threaded, so we don't need to
  // lock access to the variable.

  tree var = build_decl(BUILTINS_LOCATION, VAR_DECL, get_identifier("imp"),
			boolean_type_node);
  DECL_EXTERNAL(var) = 0;
  TREE_PUBLIC(var) = 0;
  TREE_STATIC(var) = 1;
  DECL_ARTIFICIAL(var) = 1;
  rest_of_decl_compilation(var, 1, 0);

  append_to_statement_list(build2(MODIFY_EXPR, void_type_node,
				  var, boolean_true_node),
			   &init_stmt_list);

  // Build a call to __builtin_expect since we expect the guard
  // variable to be false.
  tree expect_fn = built_in_decls[BUILT_IN_EXPECT];
  tree arg_types = TYPE_ARG_TYPES(TREE_TYPE(expect_fn));
  tree pred_type = TREE_VALUE(arg_types);
  tree expected_type = TREE_VALUE(TREE_CHAIN(arg_types));
  tree expect_var = fold_convert(pred_type, var);
  tree expect_val = fold_convert(expected_type, boolean_false_node);
  tree varref = build_call_expr(expect_fn, 2, expect_var, expect_val);

  init_stmt_list = build3(COND_EXPR, void_type_node,
			  fold_convert(boolean_type_node, varref),
			  NULL_TREE, init_stmt_list);

  // Here come the tedious details of building your own function.
  // There doesn't seem to be a helper function for this.

  const std::string& name(this->get_init_fn_name());
  tree fndecl = build_decl(BUILTINS_LOCATION, FUNCTION_DECL,
			   get_identifier(name.c_str()),
			   build_function_type(void_type_node,
					       void_list_node));

  tree resdecl = build_decl(BUILTINS_LOCATION, RESULT_DECL, NULL_TREE,
			    void_type_node);
  DECL_ARTIFICIAL(resdecl) = 1;
  DECL_CONTEXT(resdecl) = fndecl;
  DECL_RESULT(fndecl) = resdecl;

  TREE_STATIC(fndecl) = 1;
  TREE_USED(fndecl) = 1;
  DECL_ARTIFICIAL(fndecl) = 1;
  DECL_SAVED_TREE(fndecl) = init_stmt_list;
  TREE_PUBLIC(fndecl) = 1;

  DECL_INITIAL(fndecl) = make_node(BLOCK);
  TREE_USED(DECL_INITIAL(fndecl)) = 1;

  current_function_decl = fndecl;
  push_struct_function(fndecl);
  cfun->function_end_locus = BUILTINS_LOCATION;

  gimplify_function_tree(fndecl);

  cgraph_add_new_function(fndecl, false);
  cgraph_mark_needed_node(cgraph_node(fndecl));

  current_function_decl = NULL_TREE;
  pop_cfun();
}

// Search for references to VAR in any statements or called functions.

class Find_var : public Traverse
{
 public:
  // A hash table we use to avoid looping.  The index is the name of a
  // named object.  We only look through objects defined in this
  // package.
  typedef std::tr1::unordered_set<std::string> Seen_objects;

  Find_var(Named_object* var, Seen_objects* seen_objects)
    : Traverse(traverse_expressions),
      var_(var), seen_objects_(seen_objects), found_(false)
  { }

  // Whether the variable was found.
  bool
  found() const
  { return this->found_; }

  int
  expression(Expression**);

 private:
  // The variable we are looking for.
  Named_object* var_;
  // Names of objects we have already seen.
  Seen_objects* seen_objects_;
  // True if the variable was found.
  bool found_;
};

// See if EXPR refers to VAR, looking through function calls and
// variable initializations.

int
Find_var::expression(Expression** pexpr)
{
  Expression* e = *pexpr;

  Var_expression* ve = e->var_expression();
  if (ve != NULL)
    {
      Named_object* v = ve->named_object();
      if (v == this->var_)
	{
	  this->found_ = true;
	  return TRAVERSE_EXIT;
	}

      if (v->is_variable() && v->package() == NULL)
	{
	  Expression* init = v->var_value()->init();
	  if (init != NULL)
	    {
	      std::pair<Seen_objects::iterator, bool> ins =
		this->seen_objects_->insert(v->name());
	      if (ins.second)
		{
		  // This is the first time we have seen this name.
		  if (Expression::traverse(&init, this) == TRAVERSE_EXIT)
		    return TRAVERSE_EXIT;
		}
	    }
	}
    }

  // We traverse the code of any function we see.  Note that this
  // means that we will traverse the code of a function whose address
  // is taken even if it is not called.
  Func_expression* fe = e->func_expression();
  if (fe != NULL)
    {
      const Named_object* f = fe->named_object();
      if (f->is_function() && f->package() == NULL)
	{
	  std::pair<Seen_objects::iterator, bool> ins =
	    this->seen_objects_->insert(f->name());
	  if (ins.second)
	    {
	      // This is the first time we have seen this name.
	      if (f->func_value()->block()->traverse(this) == TRAVERSE_EXIT)
		return TRAVERSE_EXIT;
	    }
	}
    }

  return TRAVERSE_CONTINUE;
}

// Return true if EXPR refers to VAR.

static bool
expression_requires(Expression* expr, Named_object* var)
{
  Find_var::Seen_objects seen_objects;
  Find_var find_var(var, &seen_objects);
  Expression::traverse(&expr, &find_var);
  return find_var.found();
}

// Sort variable initializations.  If the initialization expression
// for variable A refers directly or indirectly to the initialization
// expression for variable B, then we must initialize B before A.

class Var_init
{
 public:
  Var_init()
    : var_(NULL), init_(NULL_TREE), waiting_(0)
  { }

  Var_init(Named_object* var, tree init)
    : var_(var), init_(init), waiting_(0)
  { }

  // Return the variable.
  Named_object*
  var() const
  { return this->var_; }

  // Return the initialization expression.
  tree
  init() const
  { return this->init_; }

  // Return the number of variables waiting for this one to be
  // initialized.
  size_t
  waiting() const
  { return this->waiting_; }

  // Increment the number waiting.
  void
  increment_waiting()
  { ++this->waiting_; }

 private:
  // The variable being initialized.
  Named_object* var_;
  // The initialization expression to run.
  tree init_;
  // The number of variables which are waiting for this one.
  size_t waiting_;
};

typedef std::list<Var_init> Var_inits;

// Sort the variable initializations.  The rule we follow is that we
// emit them in the order they appear in the array, except that if the
// initialization expression for a variable V1 depends upon another
// variable V2 then we initialize V1 after V2.

static void
sort_var_inits(Var_inits* var_inits)
{
  Var_inits ready;
  while (!var_inits->empty())
    {
      Var_inits::iterator p1 = var_inits->begin();
      Named_object* var = p1->var();
      Expression* init = var->var_value()->init();

      // Start walking through the list to see which variables VAR
      // needs to wait for.  We can skip P1->WAITING variables--that
      // is the number we've already checked.
      Var_inits::iterator p2 = p1;
      ++p2;
      for (size_t i = p1->waiting(); i > 0; --i)
	++p2;

      for (; p2 != var_inits->end(); ++p2)
	{
	  if (expression_requires(init, p2->var()))
	    {
	      // Check for cycles.
	      if (expression_requires(p2->var()->var_value()->init(), var))
		{
		  std::string n1 = Gogo::unpack_hidden_name(var->name());
		  std::string n2 = Gogo::unpack_hidden_name(p2->var()->name());
		  error_at(var->location(),
			   ("initialization expressions for %qs and "
			    "%qs depend upon each other"),
			   n1.c_str(), n2.c_str());
		  inform(p2->var()->location(), "%qs defined here", n2.c_str());
		  p2 = var_inits->end();
		}
	      else
		{
		  // We can't emit P1 until P2 is emitted.  Move P1.
		  // Note that the WAITING loop always executes at
		  // least once, which is what we want.
		  p2->increment_waiting();
		  Var_inits::iterator p3 = p2;
		  for (size_t i = p2->waiting(); i > 0; --i)
		    ++p3;
		  var_inits->splice(p3, *var_inits, p1);
		}
	      break;
	    }
	}

      if (p2 == var_inits->end())
	{
	  // VAR does not depends upon any other initialization expressions.
	  ready.splice(ready.end(), *var_inits, p1);
	}
    }

  // Now READY is the list in the desired initialization order.
  var_inits->swap(ready);
}

// Write out the global definitions.

void
Gogo::write_globals()
{
  Bindings* bindings = this->current_bindings();
  size_t count = bindings->size_definitions();

  tree* vec = new tree[count];

  tree init_stmt_list = NULL_TREE;

  if (this->package_name() == "main")
    this->init_imports(&init_stmt_list);

  // A list of variable initializations.
  Var_inits var_inits;

  size_t i = 0;
  for (Bindings::const_definitions_iterator p = bindings->begin_definitions();
       p != bindings->end_definitions();
       ++p, ++i)
    {
      gcc_assert(!(*p)->is_type_declaration()
		 && !(*p)->is_function_declaration());
      // There is nothing to do for a package.
      if ((*p)->is_package())
	{
	  --i;
	  --count;
	  continue;
	}

      // There is nothing to do for an object which was imported from
      // a different package into the global scope.
      if ((*p)->package() != NULL)
	{
	  --i;
	  --count;
	  continue;
	}

      // Don't try to output anything for constants which still have
      // abstract type.
      if ((*p)->is_const())
	{
	  Type* type = (*p)->const_value()->type();
	  if (type == NULL)
	    type = (*p)->const_value()->expr()->type();
	  if (type->is_abstract())
	    {
	      --i;
	      --count;
	      continue;
	    }
	}

      vec[i] = (*p)->get_tree(this, NULL);

      if (vec[i] == error_mark_node)
	{
	  gcc_assert(errorcount || sorrycount);
	  --i;
	  --count;
	  continue;
	}

      // If a variable is initialized to a non-constant value, do the
      // initialization in an initialization function.
      if (TREE_CODE(vec[i]) == VAR_DECL)
	{
	  gcc_assert((*p)->is_variable());
	  tree init = (*p)->var_value()->get_init_tree(this, NULL);
	  if (init == error_mark_node)
	    gcc_assert(errorcount || sorrycount);
	  else if ((*p)->name()[0] == '_' && (*p)->name()[1] == '.')
	    {
	      // This is a dummy variable created to run a global
	      // initializer.  We don't want to actually output the
	      // variable, just the initializer.
	      --i;
	      --count;
	      gcc_assert((*p)->var_value()->init() != NULL);
	      if (!TREE_CONSTANT(init))
		var_inits.push_back(Var_init(*p, init));
	    }
	  else if (TREE_CONSTANT(init))
	    DECL_INITIAL(vec[i]) = init;
	  else
	    {
	      tree set = fold_build2(MODIFY_EXPR, void_type_node, vec[i], init);
	      Expression* vinit = (*p)->var_value()->init();
	      if (vinit == NULL)
		append_to_statement_list(set, &init_stmt_list);
	      else
		var_inits.push_back(Var_init(*p, set));
	    }
	}
    }

  // Initialize the variables, first sorting them into a workable
  // order.
  if (!var_inits.empty())
    {
      sort_var_inits(&var_inits);
      for (Var_inits::const_iterator p = var_inits.begin();
	   p != var_inits.end();
	   ++p)
	append_to_statement_list(p->init(), &init_stmt_list);
    }

  // After all the variables are initialized, call the "init"
  // functions if there are any.
  for (std::vector<Named_object*>::const_iterator p =
	 this->init_functions_.begin();
       p != this->init_functions_.end();
       ++p)
    {
      tree decl = (*p)->get_tree(this, NULL);
      tree call = build_call_expr(decl, 0);
      append_to_statement_list(call, &init_stmt_list);
    }

  // Set up a magic function to do all the initialization actions.
  // This will be called if this package is imported.
  if (init_stmt_list != NULL_TREE || this->need_init_fn_)
    this->write_initialization_function(init_stmt_list);

  // Pass everything back to the middle-end.

  cgraph_finalize_compilation_unit();

  wrapup_global_declarations(vec, count);
  check_global_declarations(vec, count);
  emit_debug_global_declarations(vec, count);

  delete[] vec;
}

// Get a tree for the identifier for a named object.

tree
Named_object::get_id(Gogo* gogo)
{
  std::string decl_name;
  if (this->is_function_declaration()
      && !this->func_declaration_value()->asm_name().empty())
    decl_name = this->func_declaration_value()->asm_name();
  else if ((this->is_variable() && !this->var_value()->is_global())
	   || (this->is_type()
	       && this->type_value()->location() == BUILTINS_LOCATION))
    {
      // We don't need the package name for local variables or builtin
      // types.
      decl_name = Gogo::unpack_hidden_name(this->name_);
    }
  else if (this->is_function()
	   && !this->func_value()->is_method()
	   && this->package_ == NULL
	   && Gogo::unpack_hidden_name(this->name_) == "init")
    {
      // A single package can have multiple "init" functions, which
      // means that we need to give them different names.
      static int init_index;
      char buf[20];
      snprintf(buf, sizeof buf, "%d", init_index);
      ++init_index;
      decl_name = gogo->package_name() + ".init." + buf;
    }
  else
    {
      std::string package_name;
      if (this->package_ == NULL)
	package_name = gogo->package_name();
      else
	package_name = this->package_->name();

      decl_name = package_name + '.' + Gogo::unpack_hidden_name(this->name_);

      Function_type* fntype;
      if (this->is_function())
	fntype = this->func_value()->type();
      else if (this->is_function_declaration())
	fntype = this->func_declaration_value()->type();
      else
	fntype = NULL;
      if (fntype != NULL && fntype->is_method())
	{
	  decl_name.push_back('.');
	  decl_name.append(fntype->receiver()->type()->mangled_name(gogo));
	}
    }
  if (this->is_type())
    {
      const Named_object* in_function = this->type_value()->in_function();
      if (in_function != NULL)
	decl_name += '$' + in_function->name();
    }
  return get_identifier_with_length(decl_name.c_str(),
				    decl_name.length());
}

// Get a tree for a named object.

tree
Named_object::get_tree(Gogo* gogo, Named_object* function)
{
  if (this->tree_ != NULL_TREE)
    {
      // If this is a local variable whose address is taken, we must
      // rebuild the INDIRECT_REF each time to avoid invalid sharing.
      tree ret = this->tree_;
      if (this->classification_ == NAMED_OBJECT_VAR
	  && this->var_value()->is_in_heap()
	  && ret != error_mark_node)
	{
	  gcc_assert(TREE_CODE(ret) == INDIRECT_REF);
	  ret = build_fold_indirect_ref_loc(this->location(),
					    TREE_OPERAND(ret, 0));
	}
      return ret;
    }

  tree name;
  if (this->classification_ == NAMED_OBJECT_TYPE)
    name = NULL_TREE;
  else
    name = this->get_id(gogo);
  tree decl;
  switch (this->classification_)
    {
    case NAMED_OBJECT_CONST:
      {
	Named_constant* named_constant = this->u_.const_value;
	Translate_context subcontext(gogo, function, NULL, NULL_TREE);
	tree expr_tree = named_constant->expr()->get_tree(&subcontext);
	if (expr_tree == error_mark_node)
	  decl = error_mark_node;
	else
	  {
	    Type* type = named_constant->type();
	    if (type != NULL && !type->is_abstract())
	      expr_tree = fold_convert(type->get_tree(gogo), expr_tree);
	    if (expr_tree == error_mark_node)
	      decl = error_mark_node;
	    else
	      {
		decl = build_decl(named_constant->location(), CONST_DECL,
				  name, TREE_TYPE(expr_tree));
		DECL_INITIAL(decl) = expr_tree;
		TREE_CONSTANT(decl) = 1;
	      }
	  }
      }
      break;

    case NAMED_OBJECT_TYPE:
      {
	Named_type* named_type = this->u_.type_value;
	tree type_tree = named_type->get_tree(gogo);
	if (type_tree == error_mark_node)
	  decl = error_mark_node;
	else
	  {
	    decl = TYPE_NAME(type_tree);
	    gcc_assert(decl != NULL_TREE);

	    // We need to produce a type descriptor for every named
	    // type, since other files or packages might refer to
	    // them.  Simply calling the type_descriptor method is
	    // enough to create the type descriptor, even though we
	    // don't do anything with it.
	    if (this->package_ == NULL
		&& (!go_default_private || !Gogo::is_hidden_name(this->name_)))
	      named_type->type_descriptor(gogo);
	  }
      }
      break;

    case NAMED_OBJECT_TYPE_DECLARATION:
      error("reference to undefined type %qs", IDENTIFIER_POINTER(name));
      return error_mark_node;

    case NAMED_OBJECT_VAR:
      {
	Variable* var = this->u_.var_value;
	Type* type = var->type();
	if (type->is_error_type()
	    || (type->is_undefined()
		&& (!var->is_global() || this->package() == NULL)))
	  {
	    // Force the error for an undefined type, just in case.
	    type->base();
	    decl = error_mark_node;
	  }
	else
	  {
	    tree var_type = type->get_tree(gogo);
	    bool is_parameter = var->is_parameter();
	    if (var->is_receiver() && type->points_to() == NULL)
	      is_parameter = false;
	    if (var->is_in_heap())
	      {
		is_parameter = false;
		var_type = build_pointer_type(var_type);
	      }
	    decl = build_decl(var->location(),
			      is_parameter ? PARM_DECL : VAR_DECL,
			      name, var_type);
	    if (!var->is_global())
	      {
		tree fnid = function->get_id(gogo);
		tree fndecl = function->func_value()->get_or_make_decl(gogo,
								       function,
								       fnid);
		DECL_CONTEXT(decl) = fndecl;
	      }
	    if (is_parameter)
	      DECL_ARG_TYPE(decl) = TREE_TYPE(decl);

	    if (var->is_global())
	      {
		if (this->package() == NULL)
		  TREE_STATIC(decl) = 1;
		else
		  DECL_EXTERNAL(decl) = 1;
		if (!go_default_private || !Gogo::is_hidden_name(this->name_))
		  TREE_PUBLIC(decl) = 1;
	      }

	    // FIXME: We should only set this for variables which are
	    // actually used somewhere.
	    TREE_USED(decl) = 1;
	  }
      }
      break;

    case NAMED_OBJECT_RESULT_VAR:
      {
	Result_variable* result = this->u_.result_var_value;
	int index = result->index();

	Function* function = result->function();
	tree return_value = function->return_value();
	const Typed_identifier_list* results = function->type()->results();
	if (results->size() == 1)
	  {
	    gcc_assert(index == 0);
	    return return_value;
	  }
	else
	  {
	    tree field;
	    for (field = TYPE_FIELDS(TREE_TYPE(return_value));
		 index > 0;
		 --index, field = TREE_CHAIN(field))
	      gcc_assert(field != NULL_TREE);
	    return build3(COMPONENT_REF, TREE_TYPE(field), return_value,
			  field, NULL_TREE);
	  }
      }
      break;

    case NAMED_OBJECT_SINK:
      gcc_unreachable();

    case NAMED_OBJECT_FUNC:
      {
	Function* func = this->u_.func_value;
	decl = func->get_or_make_decl(gogo, this, name);
	if (decl != error_mark_node)
	  {
	    if (func->block() != NULL)
	      {
		if (DECL_STRUCT_FUNCTION(decl) == NULL)
		  push_struct_function(decl);
		else
		  push_cfun(DECL_STRUCT_FUNCTION(decl));

		cfun->function_end_locus = func->block()->end_location();

		current_function_decl = decl;

		func->build_tree(gogo, this);

		gimplify_function_tree(decl);

		cgraph_finalize_function(decl, true);

		current_function_decl = NULL_TREE;
		pop_cfun();
	      }
	  }
      }
      break;

    default:
      gcc_unreachable();
    }

  if (TREE_TYPE(decl) == error_mark_node)
    decl = error_mark_node;

  tree ret = decl;

  // If this is a local variable whose address is taken, then we
  // actually store it in the heap.  For uses of the variable we need
  // to return a reference to that heap location.
  if (this->classification_ == NAMED_OBJECT_VAR
      && this->var_value()->is_in_heap()
      && ret != error_mark_node)
    {
      gcc_assert(POINTER_TYPE_P(TREE_TYPE(ret)));
      ret = build_fold_indirect_ref(ret);
    }

  this->tree_ = ret;

  if (ret != error_mark_node)
    go_preserve_from_gc(ret);

  return ret;
}

// Get the initial value of a variable as a tree.  This does not
// consider whether the variable is in the heap--it returns the
// initial value as though it were always stored in the stack.

tree
Variable::get_init_tree(Gogo* gogo, Named_object* function)
{
  if (this->init_ == NULL)
    {
      gcc_assert(!this->is_parameter_);
      return this->type_->get_init_tree(gogo, false);
    }
  else
    {
      Translate_context context(gogo, function, NULL, NULL_TREE);
      tree rhs_tree = this->init_->get_tree(&context);
      return Expression::convert_for_assignment(&context, this->type(),
						this->init_->type(),
						rhs_tree, this->location_);
    }
}

// Get a tree for a function decl.

tree
Function::get_or_make_decl(Gogo* gogo, Named_object* no, tree id)
{
  if (this->fndecl_ == NULL_TREE)
    {
      tree functype = this->type_->get_tree(gogo);
      if (functype == error_mark_node)
	this->fndecl_ = error_mark_node;
      else
	{
	  // The type of a function comes back as a pointer, but we
	  // want the real function type for a function declaration.
	  gcc_assert(POINTER_TYPE_P(functype));
	  functype = TREE_TYPE(functype);
	  tree decl = build_decl(this->location(), FUNCTION_DECL, id, functype);

	  this->fndecl_ = decl;

	  TREE_NOTHROW(decl) = 1;

	  if (this->enclosing_ != NULL || Go_statement::is_thunk(no))
	    ;
	  else if (!Gogo::is_hidden_name(no->name()))
	    TREE_PUBLIC(decl) = 1;
	  else if (Gogo::unpack_hidden_name(no->name()) == "init"
		   && !this->type_->is_method())
	    ;
	  else if (!go_default_private)
	    TREE_PUBLIC(decl) = 1;
	  else if (Gogo::unpack_hidden_name(no->name()) == "main"
		   && gogo->package_name() == "main")
	    TREE_PUBLIC(decl) = 1;

	  // Why do we have to do this in the frontend?
	  tree restype = TREE_TYPE(functype);
	  tree resdecl = build_decl(this->location(), RESULT_DECL, NULL_TREE,
				    restype);
	  DECL_ARTIFICIAL(resdecl) = 1;
	  DECL_IGNORED_P(resdecl) = 1;
	  DECL_CONTEXT(resdecl) = decl;
	  DECL_RESULT(decl) = resdecl;

	  if (this->enclosing_ != NULL)
	    DECL_STATIC_CHAIN(decl) = 1;

	  go_preserve_from_gc(decl);

	  if (this->closure_var_ != NULL)
	    {
	      push_struct_function(decl);

	      tree closure_decl = this->closure_var_->get_tree(gogo, no);

	      DECL_ARTIFICIAL(closure_decl) = 1;
	      DECL_IGNORED_P(closure_decl) = 1;
	      TREE_USED(closure_decl) = 1;
	      DECL_ARG_TYPE(closure_decl) = TREE_TYPE(closure_decl);
	      TREE_READONLY(closure_decl) = 1;

	      DECL_STRUCT_FUNCTION(decl)->static_chain_decl = closure_decl;
	      pop_cfun();
	    }
	}
    }
  return this->fndecl_;
}

// Get a tree for a function declaration.

tree
Function_declaration::get_or_make_decl(Gogo* gogo, Named_object*, tree id)
{
  if (this->fndecl_ == NULL_TREE)
    {
      tree functype = this->fntype_->get_tree(gogo);
      tree decl;
      if (functype == error_mark_node)
	decl = error_mark_node;
      else
	{
	  // The type of a function comes back as a pointer, but we
	  // want the real function type for a function declaration.
	  gcc_assert(POINTER_TYPE_P(functype));
	  functype = TREE_TYPE(functype);
	  decl = build_decl(this->location(), FUNCTION_DECL, id, functype);
	  TREE_PUBLIC(decl) = 1;
	  DECL_EXTERNAL(decl) = 1;
	}
      this->fndecl_ = decl;
      go_preserve_from_gc(decl);
    }
  return this->fndecl_;
}

// We always pass the receiver to a method as a pointer.  If the
// receiver is actually declared as a non-pointer type, then we copy
// the value into a local variable, so that it has the right type.  In
// this function we create the real PARM_DECL to use, and set
// DEC_INITIAL of the var_decl to be the value passed in.

tree
Function::make_receiver_parm_decl(Gogo* gogo, Named_object* no, tree var_decl)
{
  // If the function takes the address of a receiver which is passed
  // by value, then we will have an INDIRECT_REF here.  We need to get
  // the real variable.
  bool is_in_heap = no->var_value()->is_in_heap();
  tree val_type;
  if (TREE_CODE(var_decl) != INDIRECT_REF)
    {
      gcc_assert(!is_in_heap);
      val_type = TREE_TYPE(var_decl);
    }
  else
    {
      gcc_assert(is_in_heap);
      var_decl = TREE_OPERAND(var_decl, 0);
      gcc_assert(POINTER_TYPE_P(TREE_TYPE(var_decl)));
      val_type = TREE_TYPE(TREE_TYPE(var_decl));
    }
  gcc_assert(TREE_CODE(var_decl) == VAR_DECL);
  std::string name = IDENTIFIER_POINTER(DECL_NAME(var_decl));
  name += ".pointer";
  tree id = get_identifier(name.c_str());
  tree parm_decl = build_decl(DECL_SOURCE_LOCATION(var_decl), PARM_DECL, id,
			      build_pointer_type(val_type));
  DECL_ARG_TYPE(parm_decl) = TREE_TYPE(parm_decl);

  gcc_assert(DECL_INITIAL(var_decl) == NULL_TREE);
  // The receiver might be passed as a null pointer.
  tree check = build2(NE_EXPR, boolean_type_node, parm_decl,
		      fold_convert(TREE_TYPE(parm_decl), null_pointer_node));
  tree ind = build_fold_indirect_ref(parm_decl);
  tree zero_init = no->var_value()->type()->get_init_tree(gogo, false);
  tree init = build3(COND_EXPR, TREE_TYPE(ind), check, ind, zero_init);

  if (is_in_heap)
    {
      tree size = TYPE_SIZE_UNIT(val_type);
      tree space = gogo->allocate_memory(size, DECL_SOURCE_LOCATION(var_decl));
      space = save_expr(space);
      space = fold_convert(build_pointer_type(val_type), space);
      init = build2(COMPOUND_EXPR, TREE_TYPE(space),
		    build2(MODIFY_EXPR, void_type_node,
			   build_fold_indirect_ref(space),
			   build_fold_indirect_ref(parm_decl)),
		    space);
    }

  DECL_INITIAL(var_decl) = init;

  return parm_decl;
}

// If we take the address of a parameter, then we need to copy it into
// the heap.  We will access it as a local variable via an
// indirection.

tree
Function::copy_parm_to_heap(Gogo* gogo, tree ref)
{
  gcc_assert(TREE_CODE(ref) == INDIRECT_REF);

  tree var_decl = TREE_OPERAND(ref, 0);
  gcc_assert(TREE_CODE(var_decl) == VAR_DECL);
  source_location loc = DECL_SOURCE_LOCATION(var_decl);

  std::string name = IDENTIFIER_POINTER(DECL_NAME(var_decl));
  name += ".param";
  tree id = get_identifier(name.c_str());

  tree type = TREE_TYPE(var_decl);
  gcc_assert(POINTER_TYPE_P(type));
  type = TREE_TYPE(type);

  tree parm_decl = build_decl(loc, PARM_DECL, id, type);
  DECL_ARG_TYPE(parm_decl) = type;

  tree size = TYPE_SIZE_UNIT(type);
  tree space = gogo->allocate_memory(size, loc);
  space = save_expr(space);
  space = fold_convert(TREE_TYPE(var_decl), space);
  tree init = build2(COMPOUND_EXPR, TREE_TYPE(space),
		     build2(MODIFY_EXPR, void_type_node,
			    build_fold_indirect_ref(space),
			    parm_decl),
		     space);
  DECL_INITIAL(var_decl) = init;

  return parm_decl;
}

// Get a tree for function code.

void
Function::build_tree(Gogo* gogo, Named_object* named_function)
{
  tree fndecl = this->fndecl_;
  gcc_assert(fndecl != NULL_TREE);

  tree params = NULL_TREE;
  tree* pp = &params;

  // If we have named return values, we allocate a tree to hold them
  // in case there are any return statements which don't mention any
  // expressions.  We can't just use DECL_RESULT because it might be a
  // list of registers.
  const Typed_identifier_list* results = this->type_->results();
  if (results != NULL
      && !results->empty()
      && !results->front().name().empty())
    this->return_value_ = create_tmp_var(TREE_TYPE(TREE_TYPE(fndecl)),
					 "RETURN");

  tree declare_vars = NULL_TREE;
  for (Bindings::const_definitions_iterator p =
	 this->block_->bindings()->begin_definitions();
       p != this->block_->bindings()->end_definitions();
       ++p)
    {
      if ((*p)->is_variable() && (*p)->var_value()->is_parameter())
	{
	  *pp = (*p)->get_tree(gogo, named_function);

	  // We always pass the receiver to a method as a pointer.  If
	  // the receiver is declared as a non-pointer type, then we
	  // copy the value into a local variable.
	  if ((*p)->var_value()->is_receiver()
	      && (*p)->var_value()->type()->points_to() == NULL)
	    {
	      tree parm_decl = this->make_receiver_parm_decl(gogo, *p, *pp);
	      tree var = *pp;
	      if (TREE_CODE(var) == INDIRECT_REF)
		var = TREE_OPERAND(var, 0);
	      gcc_assert(TREE_CODE(var) == VAR_DECL);
	      TREE_CHAIN(var) = declare_vars;
	      declare_vars = var;
	      *pp = parm_decl;
	    }
	  else if ((*p)->var_value()->is_in_heap())
	    {
	      // If we take the address of a parameter, then we need
	      // to copy it into the heap.
	      tree parm_decl = this->copy_parm_to_heap(gogo, *pp);
	      gcc_assert(TREE_CODE(*pp) == INDIRECT_REF);
	      tree var_decl = TREE_OPERAND(*pp, 0);
	      gcc_assert(TREE_CODE(var_decl) == VAR_DECL);
	      TREE_CHAIN(var_decl) = declare_vars;
	      declare_vars = var_decl;
	      *pp = parm_decl;
	    }

	  if (*pp != error_mark_node)
	    {
	      gcc_assert(TREE_CODE(*pp) == PARM_DECL);
	      pp = &TREE_CHAIN(*pp);
	    }
	}
    }
  *pp = NULL_TREE;

  DECL_ARGUMENTS(fndecl) = params;

  if (this->block_ != NULL)
    {
      gcc_assert(DECL_INITIAL(fndecl) == NULL_TREE);

      // Declare variables if necessary.
      tree bind = NULL_TREE;
      if (declare_vars != NULL_TREE)
	{
	  tree block = make_node(BLOCK);
	  BLOCK_SUPERCONTEXT(block) = fndecl;
	  DECL_INITIAL(fndecl) = block;
	  BLOCK_VARS(block) = declare_vars;
	  TREE_USED(block) = 1;
	  bind = build3(BIND_EXPR, void_type_node, BLOCK_VARS(block),
			NULL_TREE, block);
	  TREE_SIDE_EFFECTS(bind) = 1;
	}

      // Build the trees for all the statements in the function.
      Translate_context context(gogo, named_function, NULL, NULL_TREE);
      tree code = this->block_->get_tree(&context);

      tree init = NULL_TREE;
      tree fini = NULL_TREE;
      source_location end_loc = this->block_->end_location();

      // Initialize variables if necessary.
      for (tree v = declare_vars; v != NULL_TREE; v = TREE_CHAIN(v))
	{
	  tree dv = build1(DECL_EXPR, void_type_node, v);
	  SET_EXPR_LOCATION(dv, DECL_SOURCE_LOCATION(v));
	  if (init == NULL_TREE)
	    init = dv;
	  else
	    init = build2(COMPOUND_EXPR, void_type_node, init, dv);
	}

      // If there is a reference count queue, initialize it at the
      // start of the function.
      bool have_refcounts = (this->refcounts_ != NULL
			     && !this->refcounts_->empty());
      if (have_refcounts)
	{
	  tree iq = this->refcounts_->init_queue(gogo, this->location_);
	  if (init == NULL_TREE)
	    init = iq;
	  else
	    init = build2(COMPOUND_EXPR, void_type_node, init, iq);
	}

      // If we have a defer stack, initialize it at the start of a
      // function.
      if (this->defer_stack_ != NULL_TREE)
	{
	  tree defer_init = build1(DECL_EXPR, void_type_node,
				   this->defer_stack_);
	  if (init == NULL_TREE)
	    init = defer_init;
	  else
	    init = build2(COMPOUND_EXPR, void_type_node, init, defer_init);
	}

      // Clean up the defer stack when we leave the function.
      if (this->defer_stack_ != NULL_TREE)
	{
	  gcc_assert(fini == NULL_TREE);
	  static tree undefer_fndecl;
	  fini = Gogo::call_builtin(&undefer_fndecl,
				    end_loc,
				    "__go_undefer",
				    1,
				    void_type_node,
				    ptr_type_node,
				    this->defer_stack_);
	}

      // Flush the reference count queue when we leave the function.
      if (have_refcounts)
	{
	  tree flush = this->refcounts_->flush_queue(gogo, true, end_loc);
	  if (fini == NULL_TREE)
	    fini = flush;
	  else
	    fini = build2(COMPOUND_EXPR, void_type_node, fini, flush);
	}

      if (code != NULL_TREE && code != error_mark_node)
	{
	  if (init != NULL_TREE)
	    code = build2(COMPOUND_EXPR, void_type_node, init, code);
	  if (fini != NULL_TREE)
	    code = build2(TRY_FINALLY_EXPR, void_type_node, code, fini);
	}

      // Stick the code into the block we built for the receiver, if
      // we built on.
      if (bind != NULL_TREE && code != NULL_TREE && code != error_mark_node)
	{
	  BIND_EXPR_BODY(bind) = code;
	  code = bind;
	}

      DECL_SAVED_TREE(fndecl) = code;
    }
}

// Get the tree for the variable holding the defer stack for this
// function.

tree
Function::defer_stack()
{
  if (this->defer_stack_ == NULL_TREE)
    {
      tree var = create_tmp_var(ptr_type_node, "DEFER");
      DECL_INITIAL(var) = null_pointer_node;
      this->defer_stack_ = var;
    }
  return this->defer_stack_;
}

// Get a tree for the statements in a block.

tree
Block::get_tree(Translate_context* context)
{
  Gogo* gogo = context->gogo();

  tree block = make_node(BLOCK);

  // Put the new block into the block tree.

  if (context->block() == NULL)
    {
      tree fndecl = context->function()->func_value()->get_decl();
      // We may have already created a block for the receiver.
      if (DECL_INITIAL(fndecl) == NULL_TREE)
	{
	  BLOCK_SUPERCONTEXT(block) = fndecl;
	  DECL_INITIAL(fndecl) = block;
	}
      else
	{
	  tree superblock_tree = DECL_INITIAL(fndecl);
	  BLOCK_SUPERCONTEXT(block) = superblock_tree;
	  gcc_assert(BLOCK_CHAIN(block) == NULL_TREE);
	  BLOCK_CHAIN(block) = block;
	}
    }
  else
    {
      tree superblock_tree = context->block_tree();
      BLOCK_SUPERCONTEXT(block) = superblock_tree;
      tree* pp;
      for (pp = &BLOCK_SUBBLOCKS(superblock_tree);
	   *pp != NULL_TREE;
	   pp = &BLOCK_CHAIN(*pp))
	;
      *pp = block;
    }

  // Expand local variables in the block.

  tree* pp = &BLOCK_VARS(block);
  for (Bindings::const_definitions_iterator pv =
	 this->bindings_->begin_definitions();
       pv != this->bindings_->end_definitions();
       ++pv)
    {
      if ((!(*pv)->is_variable() || !(*pv)->var_value()->is_parameter())
	  && !(*pv)->is_result_variable())
	{
	  tree var = (*pv)->get_tree(gogo, context->function());
	  if (var != error_mark_node && TREE_TYPE(var) != error_mark_node)
	    {
	      if ((*pv)->is_variable() && (*pv)->var_value()->is_in_heap())
		{
		  gcc_assert(TREE_CODE(var) == INDIRECT_REF);
		  var = TREE_OPERAND(var, 0);
		  gcc_assert(TREE_CODE(var) == VAR_DECL);
		}
	      *pp = var;
	      pp = &TREE_CHAIN(*pp);
	    }
	}
    }
  *pp = NULL_TREE;

  Translate_context subcontext(context->gogo(), context->function(),
			       this, block);

  tree statements = NULL_TREE;

  // Named result variables--the only sort of result variable we will
  // see in the bindings--must be explicitly zero-initialized.  Since
  // these are not regular DECLs, this is not done anywhere else.
  for (Bindings::const_definitions_iterator pv =
	 this->bindings_->begin_definitions();
       pv != this->bindings_->end_definitions();
       ++pv)
    {
      if ((*pv)->is_result_variable())
	{
	  Result_variable* rv = (*pv)->result_var_value();
	  tree init_tree = rv->type()->get_init_tree(gogo, false);
	  tree statement = build2(MODIFY_EXPR, void_type_node,
				  (*pv)->get_tree(gogo, context->function()),
				  init_tree);
	  append_to_statement_list(statement, &statements);
	}
    }

  // Expand the statements.

  for (std::vector<Statement*>::const_iterator p = this->statements_.begin();
       p != this->statements_.end();
       ++p)
    {
      tree statement = (*p)->get_tree(&subcontext);
      if (statement != error_mark_node)
	append_to_statement_list(statement, &statements);
    }

  if (!this->final_statements_.empty())
    {
      tree final_statements = NULL_TREE;
      source_location loc = UNKNOWN_LOCATION;
      for (std::vector<Statement*>::const_iterator p =
	     this->final_statements_.begin();
	   p != this->final_statements_.end();
	   ++p)
	{
	  tree statement = (*p)->get_tree(&subcontext);
	  if (statement != error_mark_node)
	    {
	      append_to_statement_list(statement, &final_statements);
	      if (loc == UNKNOWN_LOCATION)
		loc = (*p)->location();
	    }
	}
      if (final_statements != NULL_TREE)
	{
	  statements = build2(TRY_FINALLY_EXPR, void_type_node,
			      statements, final_statements);
	  SET_EXPR_LOCATION(statements, loc);
	}
    }

  TREE_USED(block) = 1;

  tree bind = build3(BIND_EXPR, void_type_node, BLOCK_VARS(block), statements,
		     block);
  TREE_SIDE_EFFECTS(bind) = 1;

  return bind;
}

// Get the LABEL_DECL for a label.

tree
Label::get_decl()
{
  if (this->decl_ == NULL)
    {
      tree id = get_identifier_with_length(this->name_.c_str(),
					   this->name_.length());
      this->decl_ = build_decl(this->location_, LABEL_DECL, id, void_type_node);
      DECL_CONTEXT(this->decl_) = current_function_decl;
    }
  return this->decl_;
}

// Return the integer type to use for a size.

extern "C"
tree
go_type_for_size(unsigned int bits, int unsignedp)
{
  const char* name;
  switch (bits)
    {
    case 8:
      name = unsignedp ? "uint8" : "int8";
      break;
    case 16:
      name = unsignedp ? "uint16" : "int16";
      break;
    case 32:
      name = unsignedp ? "uint32" : "int32";
      break;
    case 64:
      name = unsignedp ? "uint64" : "int64";
      break;
    default:
      if (bits == POINTER_SIZE && unsignedp)
	name = "uintptr";
      else
	return NULL_TREE;
    }
  Type* type = Type::lookup_integer_type(name);
  return type->get_tree(go_get_gogo());
}

// Return the type to use for a mode.

extern "C"
tree
go_type_for_mode(enum machine_mode mode, int unsignedp)
{
  // FIXME: This static_cast should be in machmode.h.
  enum mode_class mc = static_cast<enum mode_class>(GET_MODE_CLASS(mode));
  if (mc == MODE_INT)
    return go_type_for_size(GET_MODE_BITSIZE(mode), unsignedp);
  else if (mc == MODE_FLOAT)
    {
      Type* type;
      switch (GET_MODE_BITSIZE (mode))
	{
	case 32:
	  type = Type::lookup_float_type("float32");
	  break;
	case 64:
	  type = Type::lookup_float_type("float64");
	  break;
	default:
	  return NULL_TREE;
	}
      return type->float_type()->type_tree();
    }
  else
    return NULL_TREE;
}

// Return a tree which allocates SIZE bytes.

tree
Gogo::allocate_memory(tree size, source_location location)
{
  static tree new_fndecl;
  return Gogo::call_builtin(&new_fndecl,
			    location,
			    "__go_new",
			    1,
			    ptr_type_node,
			    sizetype,
			    size);
}

// Build a builtin struct with a list of fields.  The name is
// STRUCT_NAME.  STRUCT_TYPE is NULL_TREE or an empty RECORD_TYPE
// node; this exists so that the struct can have fields which point to
// itself.  If PTYPE is not NULL, store the result in *PTYPE.  There
// are NFIELDS fields.  Each field is a name (a const char*) followed
// by a type (a tree).

tree
Gogo::builtin_struct(tree* ptype, const char* struct_name, tree struct_type,
		     int nfields, ...)
{
  if (ptype != NULL && *ptype != NULL_TREE)
    return *ptype;

  va_list ap;
  va_start(ap, nfields);

  tree fields = NULL_TREE;
  for (int i = 0; i < nfields; ++i)
    {
      const char* field_name = va_arg(ap, const char*);
      tree type = va_arg(ap, tree);
      if (type == error_mark_node)
	{
	  if (ptype != NULL)
	    *ptype = error_mark_node;
	  return error_mark_node;
	}
      tree field = build_decl(BUILTINS_LOCATION, FIELD_DECL,
			      get_identifier(field_name), type);
      TREE_CHAIN(field) = fields;
      fields = field;
    }

  if (struct_type == NULL_TREE)
    struct_type = make_node(RECORD_TYPE);
  finish_builtin_struct(struct_type, struct_name, fields, NULL_TREE);

  if (ptype != NULL)
    {
      go_preserve_from_gc(struct_type);
      *ptype = struct_type;
    }

  return struct_type;
}

// Return a type to use for pointer to const char for a string.

tree
Gogo::const_char_pointer_type_tree()
{
  static tree type;
  if (type == NULL_TREE)
    {
      tree const_char_type = build_qualified_type(unsigned_char_type_node,
						  TYPE_QUAL_CONST);
      type = build_pointer_type(const_char_type);
      go_preserve_from_gc(type);
    }
  return type;
}

// Return a tree for a string constant.

tree
Gogo::string_constant_tree(const std::string& val)
{
  tree index_type = build_index_type(size_int(val.length()));
  tree const_char_type = build_qualified_type(unsigned_char_type_node,
					      TYPE_QUAL_CONST);
  tree string_type = build_array_type(const_char_type, index_type);
  string_type = build_variant_type_copy(string_type);
  TYPE_STRING_FLAG(string_type) = 1;
  tree string_val = build_string(val.length(), val.data());
  TREE_TYPE(string_val) = string_type;
  return string_val;
}

// Return a tree for a Go string constant.

tree
Gogo::go_string_constant_tree(const std::string& val)
{
  tree string_type = Type::make_string_type()->get_tree(this);
  tree struct_type = TREE_TYPE(string_type);

  // Build a version of STRING_TYPE with the length of the array
  // specified.
  tree new_struct_type = make_node(RECORD_TYPE);

  tree field = copy_node(TYPE_FIELDS(struct_type));
  DECL_CONTEXT(field) = new_struct_type;
  TYPE_FIELDS(new_struct_type) = field;

  if (!val.empty())
    {
      field = copy_node(TREE_CHAIN(TYPE_FIELDS(struct_type)));
      DECL_CONTEXT(field) = new_struct_type;
      tree index_type = build_index_type(size_int(val.length() - 1));
      TREE_TYPE(field) = build_array_type(TREE_TYPE(TREE_TYPE(field)),
					  index_type);
      TREE_CHAIN(TYPE_FIELDS(new_struct_type)) = field;
    }

  layout_type(new_struct_type);

  VEC(constructor_elt, gc)* init = VEC_alloc(constructor_elt, gc, 2);

  constructor_elt* elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = TYPE_FIELDS(new_struct_type);
  elt->value = size_int(val.length());

  if (!val.empty())
    {
      elt = VEC_quick_push(constructor_elt, init, NULL);
      elt->index = TREE_CHAIN(TYPE_FIELDS(new_struct_type));
      elt->value = Gogo::string_constant_tree(val);
    }

  tree constructor = build_constructor(new_struct_type, init);
  TREE_READONLY(constructor) = 1;
  TREE_CONSTANT(constructor) = 1;

  // FIXME: We won't merge string constants between object files.
  tree decl = build_decl(UNKNOWN_LOCATION, VAR_DECL,
			 create_tmp_var_name("S"), new_struct_type);
  DECL_EXTERNAL(decl) = 0;
  TREE_PUBLIC(decl) = 0;
  TREE_USED(decl) = 1;
  TREE_READONLY(decl) = 1;
  TREE_CONSTANT(decl) = 1;
  TREE_STATIC(decl) = 1;
  DECL_ARTIFICIAL(decl) = 1;
  DECL_INITIAL(decl) = constructor;
  rest_of_decl_compilation(decl, 1, 0);

  return fold_convert(string_type, build_fold_addr_expr(decl));
}

// Return a tree for a pointer to a Go string constant.  This is only
// used for type descriptors, so we return a pointer to a constant
// decl.  FIXME: In gc a string is a two word value, so it makes sense
// for code to work with pointers to strings.  We should adapt the
// code or something.

tree
Gogo::ptr_go_string_constant_tree(const std::string& val)
{
  tree pval = this->go_string_constant_tree(val);

  tree decl = build_decl(UNKNOWN_LOCATION, VAR_DECL,
			 create_tmp_var_name("SP"), TREE_TYPE(pval));
  DECL_EXTERNAL(decl) = 0;
  TREE_PUBLIC(decl) = 0;
  TREE_USED(decl) = 1;
  TREE_READONLY(decl) = 1;
  TREE_CONSTANT(decl) = 1;
  TREE_STATIC(decl) = 1;
  DECL_ARTIFICIAL(decl) = 1;
  DECL_INITIAL(decl) = pval;
  rest_of_decl_compilation(decl, 1, 0);

  return build_fold_addr_expr(decl);
}

// Build the type of the struct that holds a slice for the given
// element type.

tree
Gogo::slice_type_tree(tree element_type_tree)
{
  // We use int for the count and capacity fields in a slice header.
  // This matches 6g.  The language definition guarantees that we
  // can't allocate space of a size which does not fit in int
  // anyhow. FIXME: integer_type_node is the the C type "int" but is
  // not necessarily the Go type "int".  They will differ when the C
  // type "int" has fewer than 32 bits.
  return Gogo::builtin_struct(NULL, "__go_slice", NULL_TREE, 3,
			      "__values",
			      build_pointer_type(element_type_tree),
			      "__count",
			      integer_type_node,
			      "__capacity",
			      integer_type_node);
}

// Given the tree for a slice type, return the tree for the type of
// the elements of the slice.

tree
Gogo::slice_element_type_tree(tree slice_type_tree)
{
  gcc_assert(TREE_CODE(slice_type_tree) == RECORD_TYPE
	     && POINTER_TYPE_P(TREE_TYPE(TYPE_FIELDS(slice_type_tree))));
  return TREE_TYPE(TREE_TYPE(TYPE_FIELDS(slice_type_tree)));
}

// Build a constructor for a slice.  SLICE_TYPE_TREE is the type of
// the slice.  VALUES is the value pointer and COUNT is the number of
// entries.  If CAPACITY is not NULL, it is the capacity; otherwise
// the capacity and the count are the same.

tree
Gogo::slice_constructor(tree slice_type_tree, tree values, tree count,
			tree capacity)
{
  gcc_assert(TREE_CODE(slice_type_tree) == RECORD_TYPE);

  VEC(constructor_elt,gc)* init = VEC_alloc(constructor_elt, gc, 3);

  tree field = TYPE_FIELDS(slice_type_tree);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__values") == 0);
  constructor_elt* elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  gcc_assert(TYPE_MAIN_VARIANT(TREE_TYPE(field))
	     == TYPE_MAIN_VARIANT(TREE_TYPE(values)));
  elt->value = values;

  count = fold_convert(sizetype, count);
  if (capacity == NULL_TREE)
    {
      count = save_expr(count);
      capacity = count;
    }

  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__count") == 0);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = fold_convert(TREE_TYPE(field), count);

  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__capacity") == 0);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = fold_convert(TREE_TYPE(field), capacity);

  return build_constructor(slice_type_tree, init);
}

// Build a constructor for an empty slice.

tree
Gogo::empty_slice_constructor(tree slice_type_tree)
{
  tree element_field = TYPE_FIELDS(slice_type_tree);
  tree ret = Gogo::slice_constructor(slice_type_tree,
				     fold_convert(TREE_TYPE(element_field),
						  null_pointer_node),
				     size_zero_node,
				     size_zero_node);
  TREE_CONSTANT(ret) = 1;
  return ret;
}

// Build a map descriptor for a map of type MAPTYPE.

tree
Gogo::map_descriptor(Map_type* maptype)
{
  if (this->map_descriptors_ == NULL)
    this->map_descriptors_ = new Map_descriptors(10);

  std::pair<const Map_type*, tree> val(maptype, NULL);
  std::pair<Map_descriptors::iterator, bool> ins =
    this->map_descriptors_->insert(val);
  Map_descriptors::iterator p = ins.first;
  if (!ins.second)
    {
      gcc_assert(p->second != NULL_TREE && DECL_P(p->second));
      return build_fold_addr_expr(p->second);
    }

  Type* keytype = maptype->key_type();
  Type* valtype = maptype->val_type();

  std::string mangled_name = ("__go_map_" + maptype->mangled_name(this));

  tree id = get_identifier_with_length(mangled_name.data(),
				       mangled_name.length());

  // Get the type of the map descriptor.  This is __go_map_descriptor
  // in libgo/map.h.

  tree struct_type = this->map_descriptor_type();

  // The map entry type is a struct with three fields.  This struct is
  // specific to MAPTYPE.  Build it.

  tree map_entry_type = make_node(RECORD_TYPE);

  map_entry_type = Gogo::builtin_struct(NULL, "__map", map_entry_type, 3,
					"__next",
					build_pointer_type(map_entry_type),
					"__key",
					keytype->get_tree(this),
					"__val",
					valtype->get_tree(this));

  tree map_entry_key_field = TREE_CHAIN(TYPE_FIELDS(map_entry_type));
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(map_entry_key_field)),
		    "__key") == 0);

  tree map_entry_val_field = TREE_CHAIN(map_entry_key_field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(map_entry_val_field)),
		    "__val") == 0);

  // Initialize the entries.

  tree map_descriptor_field = TYPE_FIELDS(struct_type);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(map_descriptor_field)),
		    "__map_descriptor") == 0);
  tree entry_size_field = TREE_CHAIN(map_descriptor_field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(entry_size_field)),
		    "__entry_size") == 0);
  tree key_offset_field = TREE_CHAIN(entry_size_field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(key_offset_field)),
		    "__key_offset") == 0);
  tree val_offset_field = TREE_CHAIN(key_offset_field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(val_offset_field)),
		    "__val_offset") == 0);

  VEC(constructor_elt, gc)* descriptor = VEC_alloc(constructor_elt, gc, 6);

  constructor_elt* elt = VEC_quick_push(constructor_elt, descriptor, NULL);
  elt->index = map_descriptor_field;
  elt->value = maptype->type_descriptor(this);

  elt = VEC_quick_push(constructor_elt, descriptor, NULL);
  elt->index = entry_size_field;
  elt->value = TYPE_SIZE_UNIT(map_entry_type);

  elt = VEC_quick_push(constructor_elt, descriptor, NULL);
  elt->index = key_offset_field;
  elt->value = byte_position(map_entry_key_field);

  elt = VEC_quick_push(constructor_elt, descriptor, NULL);
  elt->index = val_offset_field;
  elt->value = byte_position(map_entry_val_field);

  tree constructor = build_constructor(struct_type, descriptor);

  tree decl = build_decl(BUILTINS_LOCATION, VAR_DECL, id, struct_type);
  TREE_STATIC(decl) = 1;
  TREE_USED(decl) = 1;
  TREE_READONLY(decl) = 1;
  TREE_CONSTANT(decl) = 1;
  DECL_INITIAL(decl) = constructor;
  make_decl_one_only(decl, DECL_ASSEMBLER_NAME(decl));
  resolve_unique_section(decl, 1, 0);

  rest_of_decl_compilation(decl, 1, 0);

  go_preserve_from_gc(decl);
  p->second = decl;

  return build_fold_addr_expr(decl);
}

// Return a tree for the type of a map descriptor.  This is struct
// __go_map_descriptor in libgo/runtime/map.h.  This is the same for
// all map types.

tree
Gogo::map_descriptor_type()
{
  static tree struct_type;
  tree dtype = this->map_type_descriptor_type_tree();
  dtype = build_qualified_type(dtype, TYPE_QUAL_CONST);
  return Gogo::builtin_struct(&struct_type, "__go_map_descriptor", NULL_TREE,
			      4,
			      "__map_descriptor",
			      build_pointer_type(dtype),
			      "__entry_size",
			      sizetype,
			      "__key_offset",
			      sizetype,
			      "__val_offset",
			      sizetype);
}

// Return pointers to functions which compute a hash code for TYPE and
// which compare whether two TYPEs are equal.

void
Gogo::type_functions(const Type* keytype, tree* hash_fn, tree* equal_fn)
{
  const char* hash_fn_name;
  const char* equal_fn_name;
  switch (keytype->base()->classification())
    {
    case Type::TYPE_ERROR:
    case Type::TYPE_VOID:
    case Type::TYPE_NIL:
    case Type::TYPE_VARARGS:
      // These types can not be hashed or compared.
      hash_fn_name = "__go_type_hash_error";
      equal_fn_name = "__go_type_equal_error";
      break;

    case Type::TYPE_BOOLEAN:
    case Type::TYPE_INTEGER:
    case Type::TYPE_FLOAT:
    case Type::TYPE_POINTER:
    case Type::TYPE_FUNCTION:
    case Type::TYPE_CHANNEL:
      hash_fn_name = "__go_type_hash_identity";
      equal_fn_name = "__go_type_equal_identity";
      break;

    case Type::TYPE_STRING:
      hash_fn_name = "__go_type_hash_string";
      equal_fn_name = "__go_type_equal_string";
      break;

    case Type::TYPE_STRUCT:
    case Type::TYPE_ARRAY:
    case Type::TYPE_MAP:
      // These types can not be hashed or compared.
      hash_fn_name = "__go_type_hash_error";
      equal_fn_name = "__go_type_equal_error";
      break;

    case Type::TYPE_INTERFACE:
      hash_fn_name = "__go_type_hash_interface";
      equal_fn_name = "__go_type_equal_interface";
      break;

    case Type::TYPE_NAMED:
    case Type::TYPE_FORWARD:
      gcc_unreachable();

    default:
      gcc_unreachable();
    }

  tree id = get_identifier(hash_fn_name);
  tree fntype = build_function_type_list(sizetype, const_ptr_type_node,
					 sizetype, NULL_TREE);
  tree decl = build_decl(BUILTINS_LOCATION, FUNCTION_DECL, id, fntype);
  Gogo::mark_fndecl_as_builtin_library(decl);
  *hash_fn = build_fold_addr_expr(decl);
  go_preserve_from_gc(decl);

  id = get_identifier(equal_fn_name);
  fntype = build_function_type_list(boolean_type_node, const_ptr_type_node,
				    sizetype, const_ptr_type_node,
				    sizetype, NULL_TREE);
  decl = build_decl(BUILTINS_LOCATION, FUNCTION_DECL, id, fntype);
  Gogo::mark_fndecl_as_builtin_library(decl);
  *equal_fn = build_fold_addr_expr(decl);
  go_preserve_from_gc(decl);
}

// Build and return the tree type for a type descriptor.

tree
Gogo::type_descriptor_type_tree()
{
  static tree descriptor_type;
  if (descriptor_type == NULL_TREE)
    {
      tree uncommon_type = make_node(RECORD_TYPE);

      tree string_type = Type::make_string_type()->get_tree(this);
      tree string_pointer_type = build_pointer_type(string_type);

      tree hash_fntype = build_function_type_list(sizetype, const_ptr_type_node,
						  sizetype, NULL_TREE);
      hash_fntype = build_pointer_type(hash_fntype);

      tree equal_fntype = build_function_type_list(boolean_type_node,
						   const_ptr_type_node,
						   const_ptr_type_node,
						   sizetype,
						   NULL_TREE);
      equal_fntype = build_pointer_type(equal_fntype);

      Gogo::builtin_struct(&descriptor_type, "__go_type_descriptor", NULL_TREE,
			   8,
			   "__code",
			   unsigned_char_type_node,
			   "__align",
			   unsigned_char_type_node,
			   "__field_align",
			   unsigned_char_type_node,
			   "__size",
			   Type::lookup_integer_type("uintptr")->get_tree(this),
			   "__hash",
			   hash_fntype,
			   "__equal",
			   equal_fntype,
			   "__reflection",
			   string_pointer_type,
			   "__uncommon",
			   build_pointer_type(uncommon_type));

      tree descriptor_pointer_type = build_pointer_type(descriptor_type);

      tree method_type = Gogo::builtin_struct(NULL, "__go_method", NULL_TREE,
					      5,
					      "__hash",
					      uint32_type_node,
					      "__name",
					      string_pointer_type,
					      "__pkg_path",
					      string_pointer_type,
					      "__type",
					      descriptor_pointer_type,
					      "__function",
					      const_ptr_type_node);

      Gogo::builtin_struct(NULL, "__go_uncommon_type", uncommon_type, 3,
			   "__name",
			   string_pointer_type,
			   "__pkg_path",
			   string_pointer_type,
			   "__methods",
			   this->slice_type_tree(method_type));
    }

  return descriptor_type;
}

// Return the name to use for a type descriptor decl for TYPE.  This
// is used when TYPE does not have a name.

std::string
Gogo::unnamed_type_descriptor_decl_name(const Type* type)
{
  return "__go_td_" + type->mangled_name(this);
}

// Return the name to use for a type descriptor decl for a type named
// NAME, defined in the function IN_FUNCTION.  IN_FUNCTION will
// normally be NULL.

std::string
Gogo::type_descriptor_decl_name(const Named_object* no,
				const Named_object* in_function)
{
  const std::string& package_name(no->package() == NULL
				  ? this->package_name()
				  : no->package()->name());
  std::string ret = "__go_tdn_" + package_name + ".";
  if (in_function != NULL)
    ret += Gogo::unpack_hidden_name(in_function->name()) + '.';
  ret += no->name();
  return ret;
}

// Build a constructor for a slice in a type descriptor.
// SLICE_TYPE_TREE is the type of the slice.  INIT is a vector of
// elements to store in the slice.  The result is a constant
// constructor.  This is a static function to avoid putting the type
// VEC(constructor_elt,gc) into gogo.h.

static tree
type_descriptor_slice(tree slice_type_tree, VEC(constructor_elt,gc)* init)
{
  // Build the array of initial values.
  gcc_assert(!VEC_empty(constructor_elt, init));
  tree max = size_int(VEC_length(constructor_elt, init) - 1);
  tree entry_type = Gogo::slice_element_type_tree(slice_type_tree);
  tree index_type = build_index_type(max);
  tree array_type = build_array_type(entry_type, index_type);
  tree constructor = build_constructor(array_type, init);
  TREE_CONSTANT(constructor) = 1;

  // Push the array into memory so that we can take its address.
  tree decl = build_decl(BUILTINS_LOCATION, VAR_DECL,
			 create_tmp_var_name("C"), array_type);
  DECL_EXTERNAL(decl) = 0;
  TREE_PUBLIC(decl) = 0;
  TREE_STATIC(decl) = 1;
  DECL_ARTIFICIAL(decl) = 1;
  TREE_READONLY(decl) = 1;
  TREE_CONSTANT(decl) = 1;
  DECL_INITIAL(decl) = constructor;
  rest_of_decl_compilation(decl, 1, 0);

  tree values = fold_convert(build_pointer_type(entry_type),
			     build_fold_addr_expr(decl));
  tree count = size_int(VEC_length(constructor_elt, init));
  tree ret = Gogo::slice_constructor(slice_type_tree, values, count, count);
  TREE_CONSTANT(ret) = 1;
  return ret;
}

// Sort methods by name.

class Sort_methods
{
 public:
  bool
  operator()(const std::pair<std::string, const Method*>& m1,
	     const std::pair<std::string, const Method*>& m2) const
  { return m1.first < m2.first; }
};

// Build a constructor for one entry in a method table.

tree
Gogo::type_method_table_entry(tree method_entry_tree,
			      const std::string& method_name,
			      const Method* m)
{
  VEC(constructor_elt,gc)* init = VEC_alloc(constructor_elt, gc, 5);

  Function_type* mtype = m->type();

  tree field = TYPE_FIELDS(method_entry_tree);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__hash") == 0);
  constructor_elt* elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = build_int_cst_type(TREE_TYPE(field), mtype->hash_for_method());

  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__name") == 0);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  const std::string& n(Gogo::unpack_hidden_name(method_name));
  elt->value = this->ptr_go_string_constant_tree(Gogo::unpack_hidden_name(n));

  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__pkg_path") == 0);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  if (!Gogo::is_hidden_name(method_name))
    elt->value = fold_convert(TREE_TYPE(field), null_pointer_node);
  else
    {
      std::string s = Gogo::hidden_name_package(method_name);
      elt->value = this->ptr_go_string_constant_tree(s);
    }

  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__type") == 0);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = mtype->type_descriptor(this);

  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__function") == 0);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  Named_object* no = m->named_object();
  tree fnid = no->get_id(this);
  tree fndecl;
  if (no->is_function())
    fndecl = no->func_value()->get_or_make_decl(this, no, fnid);
  else if (no->is_function_declaration())
    fndecl = no->func_declaration_value()->get_or_make_decl(this, no, fnid);
  else
    gcc_unreachable();
  elt->value = fold_convert(const_ptr_type_node,
			    build_fold_addr_expr(fndecl));

  tree ret = build_constructor(method_entry_tree, init);
  TREE_CONSTANT(ret) = 1;
  return ret;
}

// Build a method table for a type descriptor.  METHOD_TYPE_TREE is
// the type of the method table, and should be the type of a slice.
// METHODS_TYPE is the type which gives the methods.  If
// ONLY_VALUE_METHODS is true, then only value methods are used.  This
// returns a constructor for a slice.

tree
Gogo::type_method_table(tree method_type_tree, Named_type* methods_type,
			bool only_value_methods)
{
  const Methods* methods = methods_type->methods();

  std::vector<std::pair<std::string, const Method*> > smethods;
  if (methods != NULL)
    {
      for (Methods::const_iterator p = methods->begin();
	   p != methods->end();
	   ++p)
	{
	  if (p->second->is_ambiguous())
	    continue;
	  if (only_value_methods && !p->second->is_value_method())
	    continue;
	  smethods.push_back(std::make_pair(p->first, p->second));
	}
    }

  if (smethods.empty())
    return Gogo::empty_slice_constructor(method_type_tree);

  std::sort(smethods.begin(), smethods.end(), Sort_methods());

  VEC(constructor_elt,gc)* init = VEC_alloc(constructor_elt, gc,
					    smethods.size());
  tree method_entry_tree = Gogo::slice_element_type_tree(method_type_tree);
  size_t i = 0;
  for (std::vector<std::pair<std::string, const Method*> >::const_iterator p
	 = smethods.begin();
       p != smethods.end();
       ++p, ++i)
    {
      constructor_elt* elt = VEC_quick_push(constructor_elt, init, NULL);
      elt->index = size_int(i);
      elt->value = this->type_method_table_entry(method_entry_tree,
						 p->first, p->second);
    }

  return type_descriptor_slice(method_type_tree, init);
}

// Build a decl for uncommon type information for a type descriptor.
// UNCOMMON_TYPE_TREE is the type of the uncommon type struct--struct
// __go_uncommon_type in libgo/runtime/go-type.h.  If NAME is not
// NULL, it is the name of the type.  If METHODS_TYPE is NULL, then
// NAME must not be NULL, and the methods are the value methods of
// NAME.  If METHODS_TYPE is not NULL, then NAME may be NULL, and the
// methods are all the methods of METHODS_TYPE.  This returns a
// pointer to the decl that it builds.

tree
Gogo::uncommon_type_information(tree uncommon_type_tree, Named_type* name,
				Named_type* methods_type)
{
  gcc_assert(TREE_CODE(uncommon_type_tree) == RECORD_TYPE);

  tree string_type_tree = Type::make_string_type()->get_tree(this);
  tree ptr_string_type_tree = build_pointer_type(string_type_tree);

  tree name_value;
  tree pkg_path_value;
  if (name == NULL)
    {
      name_value = fold_convert(ptr_string_type_tree, null_pointer_node);
      pkg_path_value = fold_convert(ptr_string_type_tree, null_pointer_node);
    }
  else
    {
      Named_object* no = name->named_object();
      std::string n = Gogo::unpack_hidden_name(no->name());
      name_value = this->ptr_go_string_constant_tree(n);
      if (name->is_builtin())
	pkg_path_value = fold_convert(ptr_string_type_tree, null_pointer_node);
      else if (Gogo::is_hidden_name(no->name()))
	n = Gogo::hidden_name_package(no->name());
      else if (no->package() != NULL)
	n = no->package()->name();
      else
	n = this->package_name();
      if (name->in_function() != NULL)
	n += "." + Gogo::unpack_hidden_name(name->in_function()->name());
      pkg_path_value = this->ptr_go_string_constant_tree(n);
    }

  VEC(constructor_elt,gc)* init = VEC_alloc(constructor_elt, gc, 3);

  tree field = TYPE_FIELDS(uncommon_type_tree);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__name") == 0);
  constructor_elt* elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = name_value;

  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__pkg_path") == 0);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = pkg_path_value;

  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__methods") == 0);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = this->type_method_table(TREE_TYPE(field),
				       (methods_type != NULL
					? methods_type
					: name),
				       methods_type == NULL);

  tree decl = build_decl(BUILTINS_LOCATION, VAR_DECL, create_tmp_var_name("U"),
			 uncommon_type_tree);
  DECL_EXTERNAL(decl) = 0;
  TREE_PUBLIC(decl) = 0;
  TREE_STATIC(decl) = 1;
  TREE_CONSTANT(decl) = 1;
  TREE_READONLY(decl) = 1;

  DECL_INITIAL(decl) = build_constructor(uncommon_type_tree, init);

  rest_of_decl_compilation(decl, 1, 0);

  return build_fold_addr_expr(decl);
}

// Build a constructor for the basic type descriptor struct for TYPE.
// RUNTIME_TYPE_CODE is the value to store in the __code field.  If
// NAME is not NULL, it is the name to use.  If METHODS_TYPE is NULL,
// then if NAME is NULL there are no methods, and if NAME is not NULL
// then we use the value methods for NAME.  If METHODS_TYPE is not
// NULL, then we use all the methods from METHODS_TYPE.

tree
Gogo::type_descriptor_constructor(int runtime_type_code, Type* type,
				  Named_type* name, Named_type* methods_type)
{
  tree type_descriptor_type_tree = this->type_descriptor_type_tree();
  tree type_tree = type->get_tree(this);

  VEC(constructor_elt,gc)* init = VEC_alloc(constructor_elt, gc, 8);

  tree field = TYPE_FIELDS(type_descriptor_type_tree);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__code") == 0);
  constructor_elt* elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = build_int_cstu(TREE_TYPE(field), runtime_type_code);

  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__align") == 0);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = build_int_cstu(TREE_TYPE(field), TYPE_ALIGN_UNIT(type_tree));

  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)),
		    "__field_align") == 0);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  unsigned HOST_WIDE_INT val = TYPE_ALIGN(type_tree);
#ifdef BIGGEST_FIELD_ALIGNMENT
  if (val > BIGGEST_FIELD_ALIGNMENT)
    val = BIGGEST_FIELD_ALIGNMENT;
#endif
#ifdef ADJUST_FIELD_ALIGN
  {
    tree f = build_decl(UNKNOWN_LOCATION, FIELD_DECL, NULL, type_tree);
    val = ADJUST_FIELD_ALIGN(f, val);
  }
#endif
  elt->value = build_int_cstu(TREE_TYPE(field), val / BITS_PER_UNIT);

  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__size") == 0);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = TYPE_SIZE_UNIT(type_tree);

  tree hash_fn;
  tree equal_fn;
  this->type_functions(type, &hash_fn, &equal_fn);

  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__hash") == 0);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = hash_fn;

  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__equal") == 0);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = equal_fn;

  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)),
		    "__reflection") == 0);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = this->ptr_go_string_constant_tree(name != NULL
						 ? name->reflection(this)
						 : type->reflection(this));

  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)),
		    "__uncommon") == 0);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;

  if (name == NULL
      && (methods_type == NULL || !methods_type->has_any_methods()))
    elt->value = fold_convert(TREE_TYPE(field), null_pointer_node);
  else
    elt->value = this->uncommon_type_information(TREE_TYPE(TREE_TYPE(field)),
						 name, methods_type);

  tree ret = build_constructor(type_descriptor_type_tree, init);
  TREE_CONSTANT(ret) = 1;
  return ret;
}

// Create the decl which will hold the type descriptor for TYPE.
// DESCRIPTOR_TYPE_TREE is the type of the decl to create.  NAME is
// the name of the type; it may be NULL.  The new decl is stored into
// *PDECL.  This returns true if we need to build the descriptor,
// false if not.

bool
Gogo::build_type_descriptor_decl(const Type* type, tree descriptor_type_tree,
				 Named_type* name, tree* pdecl)
{
  // We can have multiple instances of unnamed types, but we only want
  // to emit the type descriptor once.  We use a hash table to handle
  // this.  This is not necessary for named types, as they are unique,
  // and we store the type descriptor decl in the type itself.
  tree* phash = NULL;
  if (name == NULL)
    {
      if (this->type_descriptor_decls_ == NULL)
	this->type_descriptor_decls_ = new Type_descriptor_decls(10);

      std::pair<Type_descriptor_decls::iterator, bool> ins =
	this->type_descriptor_decls_->insert(std::make_pair(type, NULL_TREE));
      if (!ins.second)
	{
	  // We've already built a type descriptor for this type.
	  *pdecl = ins.first->second;
	  return false;
	}
      phash = &ins.first->second;
    }

  std::string decl_name;
  if (name == NULL)
    decl_name = this->unnamed_type_descriptor_decl_name(type);
  else
    decl_name = this->type_descriptor_decl_name(name->named_object(),
						name->in_function());
  tree id = get_identifier_with_length(decl_name.data(), decl_name.length());
  tree decl = build_decl(name == NULL ? BUILTINS_LOCATION : name->location(),
			 VAR_DECL, id,
			 build_qualified_type(descriptor_type_tree,
					      TYPE_QUAL_CONST));
  TREE_READONLY(decl) = 1;
  TREE_CONSTANT(decl) = 1;

  // Store the new decl now.  This breaks a potential recursion in
  // which the length of an array calls the len function on another
  // array with the same type descriptor, and that other array is
  // initialized with values which require reference count
  // adjustments.
  go_preserve_from_gc(decl);
  *pdecl = decl;
  if (phash != NULL)
    *phash = decl;

  // For a named type defined in a different package, we just want to
  // refer to the exported type identifier.
  if (name != NULL && name->named_object()->package() != NULL)
    {
      TREE_PUBLIC(decl) = 1;
      DECL_EXTERNAL(decl) = 1;
      return false;
    }
  else
    {
      TREE_STATIC(decl) = 1;
      TREE_USED(decl) = 1;
      return true;
    }
}

// Initialize and finish the type descriptor decl *PDECL.  NAME is the
// name of the type; it may be NULL.  CONSTRUCTOR is the value to
// which is should be initialized.

void
Gogo::finish_type_descriptor_decl(tree* pdecl, Named_type* name,
				  tree constructor)
{
  tree decl = *pdecl;
  DECL_INITIAL(decl) = constructor;

  if (name == NULL || name->is_builtin())
    {
      // All type descriptors for the same unnamed or builtin type
      // should be shared.
      make_decl_one_only(decl, DECL_ASSEMBLER_NAME(decl));
      resolve_unique_section(decl, 1, 0);
    }
  else if (!go_default_private || !Gogo::is_hidden_name(name->name()))
    {
      // Give the decl protected visibility.  This avoids out-of-range
      // references with shared libraries with the x86_64 small model
      // when the type descriptor gets a COPY reloc into the main
      // executable.
      DECL_VISIBILITY(decl) = VISIBILITY_PROTECTED;
      DECL_VISIBILITY_SPECIFIED(decl) = 1;

      TREE_PUBLIC(decl) = 1;
    }

  rest_of_decl_compilation(decl, 1, 0);
}

// Build a type descriptor decl for TYPE.  RUNTIME_TYPE_CODE is the
// value to store in the __code field.  If NAME is not NULL, it is the
// name to use as well as the list of methods.  Store the decl into
// *PDECL.

void
Gogo::type_descriptor_decl(int runtime_type_code, Type* type, Named_type* name,
			   tree* pdecl)
{
  tree type_descriptor_type_tree = this->type_descriptor_type_tree();

  if (!this->build_type_descriptor_decl(type, type_descriptor_type_tree,
					name, pdecl))
    return;

  tree constructor = this->type_descriptor_constructor(runtime_type_code,
						       type, name, NULL);

  this->finish_type_descriptor_decl(pdecl, name, constructor);
}

// Build a decl for the type descriptor of an undefined type.

void
Gogo::undefined_type_descriptor_decl(Forward_declaration_type *forward,
				     Named_type* name, tree* pdecl)
{
  Named_object* no = (name != NULL
		      ? name->named_object()
		      : forward->named_object());
  std::string decl_name = this->type_descriptor_decl_name(no, NULL);
  tree id = get_identifier_with_length(decl_name.data(),
				       decl_name.length());
  tree decl = build_decl(no->location(), VAR_DECL, id,
			 this->type_descriptor_type_tree());
  TREE_READONLY(decl) = 1;
  TREE_CONSTANT(decl) = 1;
  TREE_PUBLIC(decl) = 1;
  DECL_EXTERNAL(decl) = 1;
  go_preserve_from_gc(decl);
  *pdecl = decl;
}

// The type of a type descriptor for a pointer.  This must match
// struct __go_ptr_type in libgo/runtime/go-type.h.

tree
Gogo::pointer_type_descriptor_type_tree()
{
  static tree ptr_descriptor_type;
  if (ptr_descriptor_type == NULL_TREE)
    {
      tree common = this->type_descriptor_type_tree();
      Gogo::builtin_struct(&ptr_descriptor_type, "__go_ptr_type", NULL_TREE, 2,
			   "__common",
			   this->type_descriptor_type_tree(),
			   "__element_type",
			   build_pointer_type(common));
    }
  return ptr_descriptor_type;
}

// Build a type descriptor for the pointer type TYPE.

void
Gogo::pointer_type_descriptor_decl(Pointer_type* type, Named_type* name,
				   tree* pdecl)
{
  tree type_descriptor_type_tree = this->pointer_type_descriptor_type_tree();

  if (!this->build_type_descriptor_decl(type, type_descriptor_type_tree,
					name, pdecl))
    return;

  VEC(constructor_elt,gc)* init = VEC_alloc(constructor_elt, gc, 2);

  tree field = TYPE_FIELDS(type_descriptor_type_tree);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__common") == 0);
  constructor_elt* elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  Named_type* method_type = type->points_to()->named_type();
  elt->value = this->type_descriptor_constructor(RUNTIME_TYPE_CODE_PTR, type,
						 name, method_type);

  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)),
		    "__element_type") == 0);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = type->points_to()->type_descriptor(this);

  this->finish_type_descriptor_decl(pdecl,
				    name,
				    build_constructor(type_descriptor_type_tree,
						      init));
}

// The type of a type descriptor for a function.  This must match
// struct __go_func_type in libgo/runtime/go-type.h.

tree
Gogo::function_type_descriptor_type_tree()
{
  static tree func_descriptor_type;
  if (func_descriptor_type == NULL_TREE)
    {
      tree common = this->type_descriptor_type_tree();
      tree ptr_common = build_pointer_type(common);
      Gogo::builtin_struct(&func_descriptor_type, "__go_func_type",
			   NULL_TREE, 3,
			   "__common",
			   common,
			   "__in",
			   this->slice_type_tree(ptr_common),
			   "__out",
			   this->slice_type_tree(ptr_common));
    }
  return func_descriptor_type;
}

// Build a slice constructor for the parameters or results of a
// function type.

tree
Gogo::function_type_params(tree slice_type_tree,
			   const Typed_identifier* receiver,
			   const Typed_identifier_list* params)
{
  size_t count = ((params == NULL ? 0 : params->size())
		  + (receiver != NULL ? 1 : 0));
  if (count == 0)
    return Gogo::empty_slice_constructor(slice_type_tree);

  VEC(constructor_elt,gc)* init = VEC_alloc(constructor_elt, gc, count);
  size_t i = 0;
  if (receiver != NULL)
    {
      constructor_elt* elt = VEC_quick_push(constructor_elt, init, NULL);
      elt->index = size_int(i);
      Type* rtype = receiver->type();
      // The receiver is always passed as a pointer.
      if (rtype->points_to() == NULL)
	rtype = Type::make_pointer_type(rtype);
      elt->value = rtype->type_descriptor(this);
      ++i;
    }
  if (params != NULL)
    {
      for (Typed_identifier_list::const_iterator p = params->begin();
	   p != params->end();
	   ++p, ++i)
	{
	  constructor_elt* elt = VEC_quick_push(constructor_elt, init, NULL);
	  elt->index = size_int(i);
	  elt->value = p->type()->type_descriptor(this);
	}
    }
  gcc_assert(i == count);

  return type_descriptor_slice(slice_type_tree, init);
}

// Build a type descriptor for the function type TYPE.

void
Gogo::function_type_descriptor_decl(Function_type* type, Named_type* name,
				    tree* pdecl)
{
  tree type_descriptor_type_tree = this->function_type_descriptor_type_tree();

  if (!this->build_type_descriptor_decl(type, type_descriptor_type_tree,
					name, pdecl))
    return;

  VEC(constructor_elt,gc)* init = VEC_alloc(constructor_elt, gc, 3);

  tree field = TYPE_FIELDS(type_descriptor_type_tree);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__common") == 0);
  constructor_elt* elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = this->type_descriptor_constructor(RUNTIME_TYPE_CODE_FUNC, type,
						 name, NULL);

  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__in") == 0);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = this->function_type_params(TREE_TYPE(field), type->receiver(),
					  type->parameters());

  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__out") == 0);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = this->function_type_params(TREE_TYPE(field), NULL,
					  type->results());

  this->finish_type_descriptor_decl(pdecl, name,
				    build_constructor(type_descriptor_type_tree,
						      init));
}

// The type of a type descriptor for a struct.  This must match struct
// __go_struct_type in libgo/runtime/go-type.h.

tree
Gogo::struct_type_descriptor_type_tree()
{
  static tree struct_descriptor_type;
  if (struct_descriptor_type == NULL_TREE)
    {
      tree common = this->type_descriptor_type_tree();
      tree ptr_common = build_pointer_type(common);

      tree string_type_tree = Type::make_string_type()->get_tree(this);
      tree ptr_string_type_tree = build_pointer_type(string_type_tree);

      tree uintptr_type_tree =
	Type::lookup_integer_type("uintptr")->get_tree(this);

      tree struct_field_type = Gogo::builtin_struct(NULL, "__go_struct_field",
						    NULL_TREE, 5,
						    "__name",
						    ptr_string_type_tree,
						    "__pkg_path",
						    ptr_string_type_tree,
						    "__type",
						    ptr_common,
						    "__tag",
						    ptr_string_type_tree,
						    "__offset",
						    uintptr_type_tree);

      Gogo::builtin_struct(&struct_descriptor_type, "__go_struct_type",
			   NULL_TREE, 2,
			   "__common",
			   common,
			   "__fields",
			   this->slice_type_tree(struct_field_type));
    }
  return struct_descriptor_type;
}

// Build a constructor for __go_struct_field describing a single
// struct field.

tree
Gogo::struct_type_field(tree field_type_tree, const Struct_field* struct_field,
			tree struct_field_tree)
{
  VEC(constructor_elt,gc)* init = VEC_alloc(constructor_elt, gc, 5);

  tree field = TYPE_FIELDS(field_type_tree);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__name") == 0);
  constructor_elt* elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  if (struct_field->is_anonymous())
    elt->value = fold_convert(TREE_TYPE(field), null_pointer_node);
  else
    {
      std::string n = Gogo::unpack_hidden_name(struct_field->field_name());
      elt->value = this->ptr_go_string_constant_tree(n);
    }

  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__pkg_path") == 0);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  if (!Gogo::is_hidden_name(struct_field->field_name()))
    elt->value = fold_convert(TREE_TYPE(field), null_pointer_node);
  else
    {
      std::string s = Gogo::hidden_name_package(struct_field->field_name());
      elt->value = this->ptr_go_string_constant_tree(s);
    }

  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__type") == 0);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = struct_field->type()->type_descriptor(this);

  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__tag") == 0);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  if (!struct_field->has_tag())
    elt->value = fold_convert(TREE_TYPE(field), null_pointer_node);
  else
    elt->value = this->ptr_go_string_constant_tree(struct_field->tag());

  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__offset") == 0);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = fold_convert(TREE_TYPE(field), byte_position(struct_field_tree));

  tree ret = build_constructor(field_type_tree, init);
  TREE_CONSTANT(ret) = 1;
  return ret;
}

// Build a slice constructor for the fields of a struct.

tree
Gogo::struct_type_fields(Struct_type* struct_type, tree slice_type_tree)
{
  const Struct_field_list* fields = struct_type->fields();
  if (fields == NULL || fields->empty())
    return Gogo::empty_slice_constructor(slice_type_tree);

  tree field_type_tree = Gogo::slice_element_type_tree(slice_type_tree);
  size_t count = fields->size();
  VEC(constructor_elt,gc)* init = VEC_alloc(constructor_elt, gc, count);
  tree struct_field = TYPE_FIELDS(struct_type->get_tree(this));
  size_t i = 0;
  for (Struct_field_list::const_iterator p = fields->begin();
       p != fields->end();
       ++p, ++i, struct_field = TREE_CHAIN(struct_field))
    {
      gcc_assert(struct_field != NULL_TREE);
      constructor_elt* elt = VEC_quick_push(constructor_elt, init, NULL);
      elt->index = size_int(i);
      elt->value = this->struct_type_field(field_type_tree, &*p, struct_field);
    }
  gcc_assert(i == count && struct_field == NULL_TREE);

  return type_descriptor_slice(slice_type_tree, init);
}

// Build a type descriptor for the struct type TYPE.

void
Gogo::struct_type_descriptor_decl(Struct_type* type, Named_type* name,
				  tree* pdecl)
{
  tree type_descriptor_type_tree = this->struct_type_descriptor_type_tree();

  if (!this->build_type_descriptor_decl(type, type_descriptor_type_tree,
					name, pdecl))
    return;

  VEC(constructor_elt,gc)* init = VEC_alloc(constructor_elt, gc, 2);

  tree field = TYPE_FIELDS(type_descriptor_type_tree);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__common") == 0);
  constructor_elt* elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = this->type_descriptor_constructor(RUNTIME_TYPE_CODE_STRUCT,
						 type, name, NULL);

  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__fields") == 0);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = this->struct_type_fields(type, TREE_TYPE(field));

  this->finish_type_descriptor_decl(pdecl, name,
				    build_constructor(type_descriptor_type_tree,
						      init));
}

// The type of a type descriptor for an array.  This must match struct
// __go_array_type in libgo/runtime/go-type.h.

tree
Gogo::array_type_descriptor_type_tree()
{
  static tree array_descriptor_type;
  if (array_descriptor_type == NULL_TREE)
    {
      tree common = this->type_descriptor_type_tree();
      tree ptr_common = build_pointer_type(common);

      tree uintptr_type_tree =
	Type::lookup_integer_type("uintptr")->get_tree(this);

      Gogo::builtin_struct(&array_descriptor_type, "__go_array_type",
			   NULL_TREE, 3,
			   "__common",
			   common,
			   "__element_type",
			   ptr_common,
			   "__len",
			   uintptr_type_tree);
    }
  return array_descriptor_type;
}

// Build a type descriptor for the array type TYPE.

void
Gogo::array_type_descriptor_decl(Array_type* type, Named_type* name,
				 tree* pdecl)
{
  tree type_descriptor_type_tree = this->array_type_descriptor_type_tree();

  if (!this->build_type_descriptor_decl(type, type_descriptor_type_tree,
					name, pdecl))
    return;

  VEC(constructor_elt,gc)* init = VEC_alloc(constructor_elt, gc, 3);

  tree field = TYPE_FIELDS(type_descriptor_type_tree);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__common") == 0);
  constructor_elt* elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = this->type_descriptor_constructor(RUNTIME_TYPE_CODE_ARRAY,
						 type, name, NULL);

  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)),
		    "__element_type") == 0);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = type->element_type()->type_descriptor(this);

  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__len") == 0);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = fold_convert(TREE_TYPE(field),
			    type->length_tree(this, null_pointer_node));

  this->finish_type_descriptor_decl(pdecl, name,
				    build_constructor(type_descriptor_type_tree,
						      init));
}

// The type of a type descriptor for a slice.  This must match struct
// __go_slice_type in libgo/runtime/go-type.h.

tree
Gogo::slice_type_descriptor_type_tree()
{
  static tree slice_descriptor_type;
  if (slice_descriptor_type == NULL_TREE)
    {
      tree common = this->type_descriptor_type_tree();
      tree ptr_common = build_pointer_type(common);

      Gogo::builtin_struct(&slice_descriptor_type, "__go_slice_type",
			   NULL_TREE, 2,
			   "__common",
			   common,
			   "__element_type",
			   ptr_common);
    }
  return slice_descriptor_type;
}

// Build a type descriptor for the slice type TYPE.

void
Gogo::slice_type_descriptor_decl(Array_type* type, Named_type* name,
				 tree* pdecl)
{
  tree type_descriptor_type_tree = this->slice_type_descriptor_type_tree();

  if (!this->build_type_descriptor_decl(type, type_descriptor_type_tree,
					name, pdecl))
    return;

  VEC(constructor_elt,gc)* init = VEC_alloc(constructor_elt, gc, 2);

  tree field = TYPE_FIELDS(type_descriptor_type_tree);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__common") == 0);
  constructor_elt* elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = this->type_descriptor_constructor(RUNTIME_TYPE_CODE_SLICE,
						 type, name, NULL);

  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)),
		    "__element_type") == 0);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = type->element_type()->type_descriptor(this);

  this->finish_type_descriptor_decl(pdecl, name,
				    build_constructor(type_descriptor_type_tree,
						      init));
}

// The type of a type descriptor for a map.  This must match struct
// __go_map_type in libgo/runtime/go-type.h.

tree
Gogo::map_type_descriptor_type_tree()
{
  static tree map_descriptor_type;
  if (map_descriptor_type == NULL_TREE)
    {
      tree common = this->type_descriptor_type_tree();
      tree ptr_common = build_pointer_type(common);

      Gogo::builtin_struct(&map_descriptor_type, "__go_map_type",
			   NULL_TREE, 3,
			   "__common",
			   common,
			   "__key_type",
			   ptr_common,
			   "__val_type",
			   ptr_common);
    }
  return map_descriptor_type;
}

// Build a type descriptor for the map type TYPE.

void
Gogo::map_type_descriptor_decl(Map_type* type, Named_type* name, tree* pdecl)
{
  tree type_descriptor_type_tree = this->map_type_descriptor_type_tree();

  if (!this->build_type_descriptor_decl(type, type_descriptor_type_tree,
					name, pdecl))
    return;

  VEC(constructor_elt,gc)* init = VEC_alloc(constructor_elt, gc, 3);

  tree field = TYPE_FIELDS(type_descriptor_type_tree);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__common") == 0);
  constructor_elt* elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = this->type_descriptor_constructor(RUNTIME_TYPE_CODE_MAP,
						 type, name, NULL);

  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__key_type") == 0);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = type->key_type()->type_descriptor(this);

  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__val_type") == 0);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = type->val_type()->type_descriptor(this);

  this->finish_type_descriptor_decl(pdecl, name,
				    build_constructor(type_descriptor_type_tree,
						      init));
}

// The type of a type descriptor for a channel.  This must match
// struct __go_channel_type in libgo/runtime/go-type.h.

tree
Gogo::channel_type_descriptor_type_tree()
{
  static tree channel_descriptor_type;
  if (channel_descriptor_type == NULL_TREE)
    {
      tree common = this->type_descriptor_type_tree();
      tree ptr_common = build_pointer_type(common);

      tree uintptr_type_tree =
	Type::lookup_integer_type("uintptr")->get_tree(this);

      Gogo::builtin_struct(&channel_descriptor_type, "__go_channel_type",
			   NULL_TREE, 3,
			   "__common",
			   common,
			   "__element_type",
			   ptr_common,
			   "__dir",
			   uintptr_type_tree);
    }
  return channel_descriptor_type;
}

// Build a type descriptor for the channel type TYPE.

void
Gogo::channel_type_descriptor_decl(Channel_type* type, Named_type* name,
				   tree* pdecl)
{
  tree type_descriptor_type_tree = this->channel_type_descriptor_type_tree();

  if (!this->build_type_descriptor_decl(type, type_descriptor_type_tree,
					name, pdecl))
    return;

  VEC(constructor_elt,gc)* init = VEC_alloc(constructor_elt, gc, 3);

  tree field = TYPE_FIELDS(type_descriptor_type_tree);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__common") == 0);
  constructor_elt* elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = this->type_descriptor_constructor(RUNTIME_TYPE_CODE_CHAN,
						 type, name, NULL);

  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)),
		    "__element_type") == 0);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = type->element_type()->type_descriptor(this);

  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__dir") == 0);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;

  // These bits must match the ones in libgo/runtime/go-type.h.
  int val = 0;
  if (type->may_receive())
    val |= 1;
  if (type->may_send())
    val |= 2;

  elt->value = build_int_cst_type(TREE_TYPE(field), val);

  this->finish_type_descriptor_decl(pdecl, name,
				    build_constructor(type_descriptor_type_tree,
						      init));
}

// The type of a type descriptor for an interface.  This must match
// struct __go_interface_type in libgo/runtime/go-type.h.

tree
Gogo::interface_type_descriptor_type_tree()
{
  static tree interface_descriptor_type;
  if (interface_descriptor_type == NULL_TREE)
    {
      tree common = this->type_descriptor_type_tree();
      tree ptr_common = build_pointer_type(common);

      tree string_type_tree = Type::make_string_type()->get_tree(this);
      tree ptr_string_type_tree = build_pointer_type(string_type_tree);

      tree method_type = Gogo::builtin_struct(NULL, "__go_interface_method",
					      NULL_TREE, 4,
					      "__hash",
					      uint32_type_node,
					      "__name",
					      ptr_string_type_tree,
					      "__pkg_path",
					      ptr_string_type_tree,
					      "__type",
					      ptr_common);

      Gogo::builtin_struct(&interface_descriptor_type, "__go_interface_type",
			   NULL_TREE, 2,
			   "__common",
			   common,
			   "__methods",
			   this->slice_type_tree(method_type));
    }
  return interface_descriptor_type;
}

// Build a constructor for __go_interface_method describing a single
// interface method.

tree
Gogo::interface_type_method(tree method_type_tree,
			    const Typed_identifier* method)
{
  VEC(constructor_elt,gc)* init = VEC_alloc(constructor_elt, gc, 4);

  tree field = TYPE_FIELDS(method_type_tree);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__hash") == 0);
  constructor_elt* elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = build_int_cst_type(TREE_TYPE(field),
				  method->type()->hash_for_method());

  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__name") == 0);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  std::string n = Gogo::unpack_hidden_name(method->name());
  elt->value = this->ptr_go_string_constant_tree(n);

  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__pkg_path") == 0);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index= field;
  if (!Gogo::is_hidden_name(method->name()))
    elt->value = fold_convert(TREE_TYPE(field), null_pointer_node);
  else
    {
      std::string s = Gogo::hidden_name_package(method->name());
      elt->value = this->ptr_go_string_constant_tree(s);
    }

  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__type") == 0);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = method->type()->type_descriptor(this);

  tree ret = build_constructor(method_type_tree, init);
  TREE_CONSTANT(ret) = 1;
  return ret;
}

// Build a slice constructor for the methods of an interface.

tree
Gogo::interface_type_methods(const Interface_type* interface_type,
			     tree slice_type_tree)
{
  const Typed_identifier_list* methods = interface_type->methods();
  if (methods == NULL || methods->empty())
    return Gogo::empty_slice_constructor(slice_type_tree);

  tree method_type_tree = Gogo::slice_element_type_tree(slice_type_tree);
  size_t count = methods->size();
  VEC(constructor_elt,gc)* init = VEC_alloc(constructor_elt, gc, count);
  size_t i = 0;
  for (Typed_identifier_list::const_iterator p = methods->begin();
       p != methods->end();
       ++p, ++i)
    {
      constructor_elt* elt = VEC_quick_push(constructor_elt, init, NULL);
      elt->index = size_int(i);
      elt->value = this->interface_type_method(method_type_tree, &*p);
    }
  gcc_assert(i == count);

  return type_descriptor_slice(slice_type_tree, init);
}

// Build a type descriptor for the interface type TYPE.

void
Gogo::interface_type_descriptor_decl(Interface_type* type, Named_type* name,
				     tree* pdecl)
{
  tree type_descriptor_type_tree = this->interface_type_descriptor_type_tree();

  if (!this->build_type_descriptor_decl(type, type_descriptor_type_tree,
					name, pdecl))
    return;

  VEC(constructor_elt,gc)* init = VEC_alloc(constructor_elt, gc, 2);

  tree field = TYPE_FIELDS(type_descriptor_type_tree);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__common") == 0);
  constructor_elt* elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = this->type_descriptor_constructor(RUNTIME_TYPE_CODE_INTERFACE,
						 type, name, NULL);

  field = TREE_CHAIN(field);
  gcc_assert(strcmp(IDENTIFIER_POINTER(DECL_NAME(field)), "__methods") == 0);
  elt = VEC_quick_push(constructor_elt, init, NULL);
  elt->index = field;
  elt->value = this->interface_type_methods(type, TREE_TYPE(field));

  this->finish_type_descriptor_decl(pdecl, name,
				    build_constructor(type_descriptor_type_tree,
						      init));
}

// Build an interface method table for a type: a list of function
// pointers, one for each interface method.  This is used for
// interfaces.

tree
Gogo::interface_method_table_for_type(const Interface_type* interface,
				      const Named_type* type)
{
  const Typed_identifier_list* interface_methods = interface->methods();
  gcc_assert(!interface_methods->empty());

  size_t count = interface_methods->size();
  VEC(constructor_elt, gc)* pointers = VEC_alloc(constructor_elt, gc, count);

  size_t i = 0;
  for (Typed_identifier_list::const_iterator p = interface_methods->begin();
       p != interface_methods->end();
       ++p, ++i)
    {
      bool is_ambiguous;
      Method* m = type->method_function(p->name(), &is_ambiguous);
      gcc_assert(m != NULL);

      Named_object* no = m->named_object();

      tree fnid = no->get_id(this);

      tree fndecl;
      if (no->is_function())
	fndecl = no->func_value()->get_or_make_decl(this, no, fnid);
      else if (no->is_function_declaration())
	fndecl = no->func_declaration_value()->get_or_make_decl(this, no,
								fnid);
      else
	gcc_unreachable();
      fndecl = build_fold_addr_expr(fndecl);

      constructor_elt* elt = VEC_quick_push(constructor_elt, pointers,
					    NULL);
      elt->index = size_int(i);
      elt->value = fold_convert(const_ptr_type_node, fndecl);
    }
  gcc_assert(i == count);

  tree array_type = build_array_type(const_ptr_type_node,
				     build_index_type(size_int(count - 1)));
  tree constructor = build_constructor(array_type, pointers);

  std::string mangled_name = ("__go_imt_"
			      + interface->mangled_name(this)
			      + "__"
			      + type->mangled_name(this));

  tree id = get_identifier_with_length(mangled_name.data(),
				       mangled_name.length());

  tree decl = build_decl(BUILTINS_LOCATION, VAR_DECL, id, array_type);
  TREE_STATIC(decl) = 1;
  TREE_USED(decl) = 1;
  TREE_READONLY(decl) = 1;
  TREE_CONSTANT(decl) = 1;
  DECL_INITIAL(decl) = constructor;
  make_decl_one_only(decl, DECL_ASSEMBLER_NAME(decl));
  resolve_unique_section(decl, 1, 0);

  rest_of_decl_compilation(decl, 1, 0);

  go_preserve_from_gc(decl);

  return decl;
}

// Mark a function as a builtin library function.

void
Gogo::mark_fndecl_as_builtin_library(tree fndecl)
{
  DECL_EXTERNAL(fndecl) = 1;
  TREE_PUBLIC(fndecl) = 1;
  DECL_ARTIFICIAL(fndecl) = 1;
  TREE_NOTHROW(fndecl) = 1;
  DECL_VISIBILITY(fndecl) = VISIBILITY_DEFAULT;
  DECL_VISIBILITY_SPECIFIED(fndecl) = 1;
}

// Build a call to a builtin function.

tree
Gogo::call_builtin(tree* pdecl, source_location location, const char* name,
		   int nargs, tree rettype, ...)
{
  if (rettype == error_mark_node)
    return error_mark_node;

  tree* types = new tree[nargs];
  tree* args = new tree[nargs];

  va_list ap;
  va_start(ap, rettype);
  for (int i = 0; i < nargs; ++i)
    {
      types[i] = va_arg(ap, tree);
      args[i] = va_arg(ap, tree);
      if (types[i] == error_mark_node || args[i] == error_mark_node)
	return error_mark_node;
    }
  va_end(ap);

  if (*pdecl == NULL_TREE)
    {
      tree fnid = get_identifier(name);

      tree argtypes = NULL_TREE;
      tree* pp = &argtypes;
      for (int i = 0; i < nargs; ++i)
	{
	  *pp = tree_cons(NULL_TREE, types[i], NULL_TREE);
	  pp = &TREE_CHAIN(*pp);
	}
      *pp = void_list_node;

      tree fntype = build_function_type(rettype, argtypes);

      *pdecl = build_decl(BUILTINS_LOCATION, FUNCTION_DECL, fnid, fntype);
      Gogo::mark_fndecl_as_builtin_library(*pdecl);
      go_preserve_from_gc(*pdecl);
    }

  tree fnptr = build_fold_addr_expr(*pdecl);
  if (CAN_HAVE_LOCATION_P(fnptr))
    SET_EXPR_LOCATION(fnptr, location);

  tree ret = build_call_array(rettype, fnptr, nargs, args);
  SET_EXPR_LOCATION(ret, location);

  delete[] types;
  delete[] args;

  return ret;
}

// Send VAL on CHANNEL.  If BLOCKING is true, the resulting tree has a
// void type.  If BLOCKING is false, the resulting tree has a boolean
// type, and it will evaluate as true if the value was sent.  If
// FOR_SELECT is true, this is being done because it was chosen in a
// select statement.

tree
Gogo::send_on_channel(tree channel, tree val, bool blocking, bool for_select,
		      source_location location)
{
  if (int_size_in_bytes(TREE_TYPE(val)) <= 8
      && !AGGREGATE_TYPE_P(TREE_TYPE(val)))
    {
      val = convert_to_integer(uint64_type_node, val);
      if (blocking)
	{
	  static tree send_small_fndecl;
	  return Gogo::call_builtin(&send_small_fndecl,
				    location,
				    "__go_send_small",
				    3,
				    void_type_node,
				    ptr_type_node,
				    channel,
				    uint64_type_node,
				    val,
				    boolean_type_node,
				    (for_select
				     ? boolean_true_node
				     : boolean_false_node));
	}
      else
	{
	  gcc_assert(!for_select);
	  static tree send_nonblocking_small_fndecl;
	  return Gogo::call_builtin(&send_nonblocking_small_fndecl,
				    location,
				    "__go_send_nonblocking_small",
				    2,
				    boolean_type_node,
				    ptr_type_node,
				    channel,
				    uint64_type_node,
				    val);
	}
    }
  else
    {
      tree make_tmp;
      if (TREE_ADDRESSABLE(TREE_TYPE(val)) || TREE_CODE(val) == VAR_DECL)
	{
	  make_tmp = NULL_TREE;
	  val = build_fold_addr_expr(val);
	  if (DECL_P(val))
	    TREE_ADDRESSABLE(val) = 1;
	}
      else
	{
	  tree tmp = create_tmp_var(TREE_TYPE(val), get_name(val));
	  DECL_IGNORED_P(tmp) = 0;
	  DECL_INITIAL(tmp) = val;
	  TREE_ADDRESSABLE(tmp) = 1;
	  make_tmp = build1(DECL_EXPR, void_type_node, tmp);
	  SET_EXPR_LOCATION(make_tmp, location);
	  val = build_fold_addr_expr(tmp);
	}
      val = fold_convert(ptr_type_node, val);

      tree call;
      if (blocking)
	{
	  static tree send_big_fndecl;
	  call = Gogo::call_builtin(&send_big_fndecl,
				    location,
				    "__go_send_big",
				    3,
				    void_type_node,
				    ptr_type_node,
				    channel,
				    ptr_type_node,
				    val,
				    boolean_type_node,
				    (for_select
				     ? boolean_true_node
				     : boolean_false_node));
	}
      else
	{
	  gcc_assert(!for_select);
	  static tree send_nonblocking_big_fndecl;
	  call = Gogo::call_builtin(&send_nonblocking_big_fndecl,
				    location,
				    "__go_send_nonblocking_big",
				    2,
				    boolean_type_node,
				    ptr_type_node,
				    channel,
				    ptr_type_node,
				    val);
	}

      if (make_tmp == NULL_TREE)
	return call;
      else
	{
	  tree ret = build2(COMPOUND_EXPR, TREE_TYPE(call), make_tmp, call);
	  SET_EXPR_LOCATION(ret, location);
	  return ret;
	}
    }
}

// Build the return type for a nonblocking receive of a value 64 bits
// or smaller.  This is a struct with two fields.  The first field is
// the value, of type uint64_t.  The second is a boolean which is true
// if a value was received.

tree
Gogo::build_small_nonblocking_receive_return_type()
{
  static tree struct_type;
  return Gogo::builtin_struct(&struct_type, "__go_receive_nonblocking_small",
			      NULL_TREE, 2,
			      "__val",
			      uint64_type_node,
			      "__success",
			      boolean_type_node);
}

// Build the return type for a nonblocking receive of a value more
// than 64 bits in size.  This is a struct with two fields.  The first
// field is the value, of the specified type.  The second is a boolean
// which is true if a value was received.

tree
Gogo::build_big_nonblocking_receive_return_type(tree valtype)
{
  return Gogo::builtin_struct(NULL, "__go_receive_nonblocking_big",
			      NULL_TREE, 2,
			      "__val",
			      valtype,
			      "__success",
			      boolean_type_node);
}

// Return a tree for receiving a value of type TYPE_TREE on CHANNEL.
// If BLOCKING is true this does a blocking receive, and returns the
// value read from the channel.  If BLOCKING is false, this does a
// nonblocking read, and returns a struct with two fields.  The first
// field is the value read from the channel, and the second field is a
// boolean which will be true if a value was read.  As a special hack,
// the first field may not have the correct type, and the caller may
// need to convert it.  If FOR_SELECT is true, this is being done
// because it was chosen in a select statement.

tree
Gogo::receive_from_channel(tree type_tree, tree channel, bool blocking,
			   bool for_select, source_location location)
{
  if (int_size_in_bytes(type_tree) <= 8
      && !AGGREGATE_TYPE_P(type_tree))
    {
      if (blocking)
	{
	  static tree receive_small_fndecl;
	  tree call = Gogo::call_builtin(&receive_small_fndecl,
					 location,
					 "__go_receive_small",
					 2,
					 uint64_type_node,
					 ptr_type_node,
					 channel,
					 boolean_type_node,
					 (for_select
					  ? boolean_true_node
					  : boolean_false_node));
	  int bitsize = GET_MODE_BITSIZE(TYPE_MODE(type_tree));
	  tree int_type_tree = go_type_for_size(bitsize, 1);
	  return fold_convert_loc(location, type_tree,
				  fold_convert_loc(location, int_type_tree,
						   call));
	}
      else
	{
	  gcc_assert(!for_select);
	  tree ret_type = Gogo::build_small_nonblocking_receive_return_type();
	  static tree receive_nonblocking_fndecl;
	  return Gogo::call_builtin(&receive_nonblocking_fndecl,
				    location,
				    "__go_receive_nonblocking_small",
				    1,
				    ret_type,
				    ptr_type_node,
				    channel);
	}
    }
  else
    {
      tree tmp = create_tmp_var(type_tree, get_name(type_tree));
      DECL_IGNORED_P(tmp) = 0;
      TREE_ADDRESSABLE(tmp) = 1;
      tree make_tmp = build1(DECL_EXPR, void_type_node, tmp);
      SET_EXPR_LOCATION(make_tmp, location);
      tree tmpaddr = build_fold_addr_expr(tmp);
      tmpaddr = fold_convert(ptr_type_node, tmpaddr);
      if (blocking)
	{
	  static tree receive_big_fndecl;
	  tree call = Gogo::call_builtin(&receive_big_fndecl,
					 location,
					 "__go_receive_big",
					 3,
					 void_type_node,
					 ptr_type_node,
					 channel,
					 ptr_type_node,
					 tmpaddr,
					 boolean_type_node,
					 (for_select
					  ? boolean_true_node
					  : boolean_false_node));
	  return build2(COMPOUND_EXPR, type_tree, make_tmp,
			build2(COMPOUND_EXPR, type_tree, call, tmp));
	}
      else
	{
	  gcc_assert(!for_select);
	  static tree receive_nonblocking_big_fndecl;
	  tree call = Gogo::call_builtin(&receive_nonblocking_big_fndecl,
					 location,
					 "__go_receive_nonblocking_big",
					 2,
					 boolean_type_node,
					 ptr_type_node,
					 channel,
					 ptr_type_node,
					 tmpaddr);

	  tree tmp2 = create_tmp_var(boolean_type_node,
				     get_name(boolean_type_node));
	  DECL_IGNORED_P(tmp2) = 0;
	  DECL_INITIAL(tmp2) = call;
	  tree make_tmp2 = build1(DECL_EXPR, void_type_node, tmp2);
	  SET_EXPR_LOCATION(make_tmp2, location);

	  tree rettype =
	    Gogo::build_big_nonblocking_receive_return_type(type_tree);
	  VEC(constructor_elt,gc)* values = VEC_alloc(constructor_elt, gc, 2);
	  constructor_elt* elt = VEC_quick_push(constructor_elt, values, NULL);
	  elt->index = TYPE_FIELDS(rettype);
	  elt->value = tmp;
	  elt = VEC_quick_push(constructor_elt, values, NULL);
	  elt->index = TREE_CHAIN(TYPE_FIELDS(rettype));
	  elt->value = tmp2;
	  tree retval = build_constructor(rettype, values);

	  return build2(COMPOUND_EXPR, rettype,
			build2(COMPOUND_EXPR, void_type_node, make_tmp,
			       make_tmp2),
			retval);
	}
    }
}

// Return the type of a function trampoline.  This is like
// get_trampoline_type in tree-nested.c.

tree
Gogo::trampoline_type_tree()
{
  static tree type_tree;
  if (type_tree == NULL_TREE)
    {
      unsigned int align = TRAMPOLINE_ALIGNMENT;
      unsigned int size = TRAMPOLINE_SIZE;
      tree t = build_index_type(build_int_cst(integer_type_node, size - 1));
      t = build_array_type(char_type_node, t);

      type_tree = Gogo::builtin_struct(NULL, "__go_trampoline", NULL_TREE, 1,
				       "__data", t);
      t = TYPE_FIELDS(type_tree);
      DECL_ALIGN(t) = align;
      DECL_USER_ALIGN(t) = 1;

      go_preserve_from_gc(type_tree);
    }
  return type_tree;
}

// Make a trampoline which calls FNADDR passing CLOSURE.

tree
Gogo::make_trampoline(tree fnaddr, tree closure, source_location location)
{
  tree trampoline_type = Gogo::trampoline_type_tree();
  tree trampoline_size = TYPE_SIZE_UNIT(trampoline_type);

  // We allocate the trampoline using a special function which will
  // mark it as executable.
  static tree trampoline_fndecl;
  tree x = Gogo::call_builtin(&trampoline_fndecl,
			      location,
			      "__go_allocate_trampoline",
			      1,
			      ptr_type_node,
			      size_type_node,
			      trampoline_size);

  x = save_expr(x);

  // Initialize the trampoline.
  tree ini = build_call_expr(implicit_built_in_decls[BUILT_IN_INIT_TRAMPOLINE],
			     3, x, fnaddr, closure);

  // On some targets the trampoline address needs to be adjusted.  For
  // example, when compiling in Thumb mode on the ARM, the address
  // needs to have the low bit set.
  x = build_call_expr(implicit_built_in_decls[BUILT_IN_ADJUST_TRAMPOLINE],
		      1, x);
  x = fold_convert(TREE_TYPE(fnaddr), x);

  return build2(COMPOUND_EXPR, TREE_TYPE(x), ini, x);
}
