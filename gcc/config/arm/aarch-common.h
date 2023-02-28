/* Types shared between arm and aarch64.

   Copyright (C) 2009-2021 Free Software Foundation, Inc.
   Contributed by Arm Ltd.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 3, or (at your
   option) any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

#ifndef GCC_AARCH_COMMON_H
#define GCC_AARCH_COMMON_H

/* Enum describing the various ways that the
   aarch*_parse_{arch,tune,cpu,extension} functions can fail.
   This way their callers can choose what kind of error to give.  */

enum aarch_parse_opt_result
{
  AARCH_PARSE_OK,			/* Parsing was successful.  */
  AARCH_PARSE_MISSING_ARG,		/* Missing argument.  */
  AARCH_PARSE_INVALID_FEATURE,		/* Invalid feature modifier.  */
  AARCH_PARSE_INVALID_ARG		/* Invalid arch, tune, cpu arg.  */
};

/* Function types -msign-return-address should sign.  */
enum aarch_function_type {
  /* Don't sign any function.  */
  AARCH_FUNCTION_NONE,
  /* Non-leaf functions.  */
  AARCH_FUNCTION_NON_LEAF,
  /* All functions.  */
  AARCH_FUNCTION_ALL
};

/* The key type that -msign-return-address should use.  */
enum aarch_key_type {
  AARCH_KEY_A,
  AARCH_KEY_B
};

struct aarch_branch_protect_type
{
  /* The type's name that the user passes to the branch-protection option
     string.  */
  const char* name;
  /* Function to handle the protection type and set global variables.
     First argument is the string token corresponding with this type and the
     second argument is the next token in the option string.
     Return values:
     * AARCH_PARSE_OK: Handling was sucessful.
     * AARCH_INVALID_ARG: The type is invalid in this context and the caller
     should print an error.
     * AARCH_INVALID_FEATURE: The type is invalid and the handler prints its
     own error.  */
  enum aarch_parse_opt_result (*handler)(char*, char*);
  /* A list of types that can follow this type in the option string.  */
  const struct aarch_branch_protect_type* subtypes;
  unsigned int num_subtypes;
};

#endif /* GCC_AARCH_COMMON_H */
