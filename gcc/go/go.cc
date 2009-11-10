// go.cc -- Go frontend main file for gcc.

// Copyright 2009 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <cerrno>
#include <cstdio>
#include <string>

#include "go-system.h"

extern "C" {
#include "options.h"
#include "diagnostic.h"
}

#include "go-c.h"

#include "lex.h"
#include "parse.h"
#include "gogo.h"

// The data structures we build to represent the file.
static Gogo* gogo;

// Create the main IR data structure.

extern "C"
void
go_create_gogo()
{
  gcc_assert(::gogo == NULL);
  ::gogo = new Gogo();
}

// Parse the input files.

extern "C"
void
go_parse_input_files(const char** filenames, unsigned int filename_count)
{
  gcc_assert(filename_count > 0);
  for (unsigned int i = 0; i < filename_count; ++i)
    {
      if (i > 0)
	::gogo->clear_file_scope();

      const char* filename = filenames[i];
      FILE* file;
      if (strcmp(filename, "-") == 0)
	file = stdin;
      else
	{
	  file = fopen(filename, "r");
	  if (file == NULL)
	    fatal_error("cannot open %s: %s", filename, strerror(errno));
	}

      Lex lexer(filename, file);

      Parse parse(&lexer, ::gogo);
      parse.program();

      if (strcmp(filename, "-") != 0)
	fclose(file);
    }

  // If the global predeclared names are referenced but not defined,
  // define them now.
  ::gogo->define_global_names();

  // Finalize method lists and build stub methods for named types.
  ::gogo->finalize_methods();

  // Now that we have seen all the names, lower the parse tree into a
  // form which is easier to use.
  ::gogo->lower_parse_tree();

  // Now that we have seen all the names, verify that types are
  // correct.
  ::gogo->verify_types();

  if (flag_syntax_only)
    return;

  // Work out types of unspecified constants and variables.
  ::gogo->determine_types();

  // Check types and issue errors as appropriate.
  ::gogo->check_types();

  // Check that functions have return statements.
  if (go_require_return_statement)
    ::gogo->check_return_statements();

  // Export global identifiers as appropriate.
  ::gogo->do_exports();

  // Convert complicated go and defer statements into simpler ones.
  ::gogo->simplify_thunk_statements();

  // Check which variables are only set to argument values.
  ::gogo->find_only_arg_vars();

  // Add reference counts for garbage collection.
  ::gogo->add_refcounts();
}

// Write out globals.

extern "C"
void
go_write_globals()
{
  return ::gogo->write_globals();
}

// Return the global IR structure.  This is used by some of the
// langhooks to pass to other code.

Gogo*
go_get_gogo()
{
  return ::gogo;
}
