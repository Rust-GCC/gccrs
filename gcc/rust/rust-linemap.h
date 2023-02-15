// rust-linemap.h -- interface to location tracking   -*- C++ -*-

// Copyright 2011 The Go Authors. All rights reserved.
// Copyright (C) 2020-2023 Free Software Foundation, Inc.

// Use of this source code is governed by a BSD-style
// license that can be found in the '../go/gofrontend/LICENSE' file.

#ifndef RUST_LINEMAP_H
#define RUST_LINEMAP_H

#include "rust-system.h"

// The backend must define a type named Location which holds
// information about a location in a source file.  The only thing the
// frontend does with instances of Location is pass them back to the
// backend interface.  The Location type must be assignable, and it
// must be comparable: i.e., it must support operator= and operator<.
// The type is normally passed by value rather than by reference, and
// it should support that efficiently.  The type should be defined in
// "rust-location.h".
#include "rust-location.h"

// The Linemap class is a pure abstract interface, plus some static
// convenience functions.  The backend must implement the interface.

/* TODO: probably better to replace linemap implementation as pure abstract
 * interface with some sort of compile-time switch (macros or maybe templates if
 * doable without too much extra annoyance) as to the definition of the methods
 * or whatever. This is to improve performance, as virtual function calls would
 * otherwise have to be made in tight loops like in the lexer. */

class Linemap
{
public:
  Linemap ()
  {
    // Only one instance of Linemap is allowed to exist.
    rust_assert (Linemap::instance_ == NULL);
    Linemap::instance_ = this;
  }

  virtual ~Linemap () { Linemap::instance_ = NULL; }

  // Subsequent Location values will come from the file named
  // FILE_NAME, starting at LINE_BEGIN.  Normally LINE_BEGIN will be
  // 0, but it will be non-zero if the Rust source has a //line comment.
  virtual void start_file (const char *file_name, unsigned int line_begin) = 0;

  // Subsequent Location values will come from the line LINE_NUMBER,
  // in the current file.  LINE_SIZE is the size of the line in bytes.
  // This will normally be called for every line in a source file.
  virtual void start_line (unsigned int line_number, unsigned int line_size)
    = 0;

  // Get a Location representing column position COLUMN on the current
  // line in the current file.
  virtual Location get_location (unsigned int column) = 0;

  // Stop generating Location values.  This will be called after all
  // input files have been read, in case any cleanup is required.
  virtual void stop () = 0;

  // Produce a human-readable description of a Location, e.g.
  // "foo.rust:10". Returns an empty string for predeclared, builtin or
  // unknown locations.
  virtual std::string to_string (Location) = 0;

  // Return the file name for a given location.
  virtual std::string location_file (Location) = 0;

  // Return the line number for a given location.
  virtual int location_line (Location) = 0;

  // Return the column number for a given location.
  virtual int location_column (Location) = 0;

protected:
  // Return a special Location used for predeclared identifiers.  This
  // Location should be different from that for any actual source
  // file.  This location will be used for various different types,
  // functions, and objects created by the frontend.
  virtual Location get_predeclared_location () = 0;

  // Return a special Location which indicates that no actual location
  // is known.  This is used for undefined objects and for errors.
  virtual Location get_unknown_location () = 0;

  // Return whether the argument is the Location returned by
  // get_predeclared_location.
  virtual bool is_predeclared (Location) = 0;

  // Return whether the argument is the Location returned by
  // get_unknown_location.
  virtual bool is_unknown (Location) = 0;

  // The single existing instance of Linemap.
  static Linemap *instance_;

public:
  // Following are convenience static functions, which allow us to
  // access some virtual functions without explicitly passing around
  // an instance of Linemap.

  // Return the special Location used for predeclared identifiers.
  static Location predeclared_location ()
  {
    rust_assert (Linemap::instance_ != NULL);
    return Linemap::instance_->get_predeclared_location ();
  }

  // Return the special Location used when no location is known.
  static Location unknown_location ()
  {
    rust_assert (Linemap::instance_ != NULL);
    return Linemap::instance_->get_unknown_location ();
  }

  // Return whether the argument is the special location used for
  // predeclared identifiers.
  static bool is_predeclared_location (Location loc)
  {
    rust_assert (Linemap::instance_ != NULL);
    return Linemap::instance_->is_predeclared (loc);
  }

  // Return whether the argument is the special location used when no
  // location is known.
  static bool is_unknown_location (Location loc)
  {
    rust_assert (Linemap::instance_ != NULL);
    return Linemap::instance_->is_unknown (loc);
  }

  // Produce a human-readable description of a Location.
  static std::string location_to_string (Location loc)
  {
    rust_assert (Linemap::instance_ != NULL);
    return Linemap::instance_->to_string (loc);
  }

  // Return the file name of a location.
  static std::string location_to_file (Location loc)
  {
    rust_assert (Linemap::instance_ != NULL);
    return Linemap::instance_->location_file (loc);
  }

  // Return line number of a location.
  static int location_to_line (Location loc)
  {
    rust_assert (Linemap::instance_ != NULL);
    return Linemap::instance_->location_line (loc);
  }

  static int location_to_column (Location loc)
  {
    rust_assert (Linemap::instance_ != NULL);
    return Linemap::instance_->location_column (loc);
  }
};

#endif // !defined(RUST_LINEMAP_H)
