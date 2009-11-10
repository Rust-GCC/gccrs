// refcount.h -- Go fronetnd reference counts.    -*- C++ -*-

// Copyright 2009 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef GO_REFCOUNT_H
#define GO_REFCOUNT_H

// This file defines classes used to insert reference count
// manipulation as needed in the generated program.

class Gogo;
class Type;
class Block;
class Expression;
class Call_expression;

// Each function maintains a local queue of values whose reference
// count must be incremented or decremented when exiting the function.
// These are the different sorts of values found on that queue.  This
// is not an enum because we want to declare values of this in other
// header files without including this one.

// Decrement reference count for newly allocated value created via a
// call to new or some builtin function such as string addition.
const int REFCOUNT_DECREMENT_NEW = 0;

// Decrement reference count for value referenced during the course of
// an expression, such as a value returned by a function call.
const int REFCOUNT_DECREMENT_COMPUTED = 1;

// Decrement reference count for old value stored in a variable or
// memory location to which a new value was assigned.
const int REFCOUNT_DECREMENT_OLD = 2;

// Increment reference count for value copied into variable or memory
// location.
const int REFCOUNT_INCREMENT_COPIED = 3;

// One more than the largest value.
const int REFCOUNT_MAX = 4;

// An entry in a reference count queue.

class Refcount_entry
{
 public:
  Refcount_entry(int classification, unsigned int index)
    : classification_(classification), index_(index)
  { }

  // The type of reference count adjustment.
  int
  classification() const
  { return this->classification_; }

  // The index in the specific reference count queue.
  unsigned int
  index() const
  { return this->index_; }

  // Update this entry to point to the next entry in the queue.
  void
  increment()
  { ++this->index_; }

 private:
  // Which sort of adjustment this is.
  int classification_;
  // The index in the specific reference count queue.
  unsigned int index_;
};

// The overall reference count queue.  There will be one of these
// objects for each function.

class Refcounts
{
 public:
  Refcounts()
    : entries_(), decl_(NULL), flag_decl_(NULL), queue_head_decl_(NULL),
      simple_decl_(NULL), is_complete_(false)
  { }

  // Add a new entry.
  Refcount_entry
  add(int classification, Type*);

  // Add a new entry for a type which only requires a single entry.
  // Increment *ENTRY.
  void
  add_one(Type*, Refcount_entry* entry);

  // Called after all entries have been added.
  void
  set_is_complete()
  { this->is_complete_ = true; }

  // Get the type of the variable stored in ENTRY.
  Type*
  get_type(Refcount_entry);

  // Return whether there are no reference count adjustments.
  bool
  empty() const;

  // Return a tree which sets the queue entry for ENTRY to VAL.  VAL
  // must be a pointer type.
  tree
  set_entry_tree(Gogo*, Refcount_entry entry, tree val);

  // Initialize the reference count queue at the start of a function.
  tree
  init_queue(Gogo*, source_location);

  // Return the tree for flushing the reference count queue.
  tree
  flush_queue(Gogo*, bool at_end_of_function, source_location);

 private:
  bool
  is_simple() const;

  bool
  simple_descriptor(Gogo*, tree*) const;

  static tree
  entry_type(Gogo*);

  tree
  struct_type(Gogo*);

  tree
  get_tree(Gogo*);

  tree
  get_initializer(Gogo*);

  tree
  get_flag_tree(Gogo*);

  tree
  queue_head_decl(Gogo*);

  tree
  get_simple_tree(Gogo*);

  /* The type of a list of entries.  */
  typedef std::vector<Type*> Entries;

  // Reference count adjustments, indexed by a REFCOUNT_xxx value.
  Entries entries_[REFCOUNT_MAX];
  // The local variable which holds the reference count queue.
  tree decl_;
  // The local variable which is a flag which is set when anything
  // changes.
  tree flag_decl_;
  // A decl for the head of the list of reference count queues.
  tree queue_head_decl_;
  // A decl used if the queue is simple (i.e., has only one entry).
  tree simple_decl_;
  // Set to true after we have added all reference counts.
  bool is_complete_;
};

#endif // !defined(GO_REFCOUNT_H)
