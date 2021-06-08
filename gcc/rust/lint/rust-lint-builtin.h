// Copyright (C) 2021 Free Software Foundation, Inc.

// This file is part of GCC.

// GCC is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 3, or (at your option) any later
// version.

// GCC is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.

// You should have received a copy of the GNU General Public License
// along with GCC; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

#ifndef RUST_LINT_BUILTIN_H
#define RUST_LINT_BUILTIN_H

#include "rust-lint-defs.h"
#include <string>
namespace Rust {
namespace Lint {

#define BUILTIN_LIST(V)                                                        \
  V (FORBIDDEN_LINT_GROUPS, Warn, "applying forbid to lint-groups")            \
  V (ILL_FORMED_ATTRIBUTE_INPUT, Deny,                                         \
     "ill-formed attribute inputs that were previously accepted and used in "  \
     "practice")                                                               \
  V (CONFLICTING_REPR_HINTS, Deny,                                             \
     "conflicts between `#[repr(..)]` hints that were previously accepted "    \
     "and used in practice")                                                   \
  V (META_VARIABLE_MISUSE, Allow,                                              \
     "possible meta-variable misuse at macro definition")                      \
  V (INCOMPLETE_INCLUDE, Deny, "trailing content in included file")            \
  V (ARITHMETIC_OVERFLOW, Deny, "arithmetic operation overflows")              \
  V (UNCONDITIONAL_PANIC, Deny, "operation will cause a panic at runtime")     \
  V (CONST_ERR, Deny, "constant evaluation encountered erroneous expression")  \
  V (UNUSED_IMPORTS, Warn, "imports that are never used")                      \
  V (UNUSED_EXTERN_CRATES, Allow, "extern crates that are never used")         \
  V (UNUSED_CRATE_DEPENDENCIES, Allow,                                         \
     "crate dependencies that are never used")                                 \
  V (UNUSED_QUALIFICATIONS, Allow, "detects unnecessarily qualified names")    \
  V (UNKNOWN_LINTS, Warn, "unrecognized lint attribute")                       \
  V (UNUSED_VARIABLES, Warn, "detect variables which are not used in any way") \
  V (UNUSED_ASSIGNMENTS, Warn, "detect assignments that will never be read")   \
  V (DEAD_CODE, Warn, "detect unused, unexported items")                       \
  V (UNUSED_ATTRIBUTES, Warn,                                                  \
     "detects attributes that were not used by the compiler")                  \
  V (UNREACHABLE_CODE, Warn, "detects unreachable code paths")                 \
  V (UNREACHABLE_PATTERNS, Warn, "detects unreachable patterns")               \
  V (OVERLAPPING_RANGE_ENDPOINTS, Warn,                                        \
     "detects range patterns with overlapping endpoints")                      \
  V (BINDINGS_WITH_VARIANT_NAME, Warn,                                         \
     "detects pattern bindings with the same name as one of the matched "      \
     "variants")                                                               \
  V (UNUSED_MACROS, Warn, "detects macros that were not used")                 \
  V (WARNINGS, Warn, "mass-change the level for lints which produce warnings") \
  V (UNUSED_FEATURES, Warn,                                                    \
     "unused features found in crate-level `#[feature]` directives")           \
  V (STABLE_FEATURES, Warn, "stable features found in `#[feature]` directive") \
  V (UNKNOWN_CRATE_TYPES, Deny,                                                \
     "unknown crate type found in `#[crate_type]` directive")                  \
  V (TRIVIAL_CASTS, Allow, "detects trivial casts which could be removed")     \
  V (TRIVIAL_NUMERIC_CASTS, Allow,                                             \
     "detects trivial casts of numeric types which could be removed")          \
  V (PRIVATE_IN_PUBLIC, Warn,                                                  \
     "detect private items in public interfaces not caught by the old "        \
     "implementation")                                                         \
  V (EXPORTED_PRIVATE_DEPENDENCIES, Warn,                                      \
     "public interface leaks type from a private dependency")                  \
  V (PUB_USE_OF_PRIVATE_EXTERN_CRATE, Deny,                                    \
     "detect public re-exports of private extern crates")                      \
  V (INVALID_TYPE_PARAM_DEFAULT, Deny,                                         \
     "type parameter default erroneously allowed in invalid location")         \
  V (RENAMED_AND_REMOVED_LINTS, Warn,                                          \
     "lints that have been renamed or removed")                                \
  V (UNALIGNED_REFERENCES, Warn,                                               \
     "detects unaligned references to fields of packed structs")               \
  V (CONST_ITEM_MUTATION, Warn, "detects attempts to mutate a `const` item")   \
  V (PATTERNS_IN_FNS_WITHOUT_BODY, Deny,                                       \
     "patterns in functions without body were erroneously allowed")            \
  V (MISSING_FRAGMENT_SPECIFIER, Deny,                                         \
     "detects missing fragment specifiers in unused `macro_rules!` patterns")  \
  V (LATE_BOUND_LIFETIME_ARGUMENTS, Warn,                                      \
     "detects generic lifetime arguments in path segments with late bound "    \
     "lifetime parameters")                                                    \
  V (ORDER_DEPENDENT_TRAIT_OBJECTS, Deny,                                      \
     "trait-object types were treated as different depending on marker-trait " \
     "order")                                                                  \
  V (COHERENCE_LEAK_CHECK, Warn,                                               \
     "distinct impls distinguished only by the leak-check code")               \
  V (DEPRECATED, Warn, "detects use of deprecated items")                      \
  V (UNUSED_UNSAFE, Warn, "unnecessary use of an `unsafe` block")              \
  V (UNUSED_MUT, Warn, "detect mut variables which don't need to be mutable")  \
  V (UNCONDITIONAL_RECURSION, Warn,                                            \
     "functions that cannot return without calling themselves")                \
  V (SINGLE_USE_LIFETIMES, Allow,                                              \
     "detects lifetime parameters that are only used once")                    \
  V (UNUSED_LIFETIMES, Allow,                                                  \
     "detects lifetime parameters that are never used")                        \
  V (TYVAR_BEHIND_RAW_POINTER, Warn, "raw pointer to an inference variable")   \
  V (ELIDED_LIFETIMES_IN_PATHS, Allow,                                         \
     "hidden lifetime parameters in types are deprecated")                     \
  V (BARE_TRAIT_OBJECTS, Warn, "suggest using `dyn Trait` for trait objects")  \
  V (ABSOLUTE_PATHS_NOT_STARTING_WITH_CRATE, Allow,                            \
     "fully qualified paths that start with a module name \
     instead of `crate`, `self`, or an extern crate name")                     \
  V (ILLEGAL_FLOATING_POINT_LITERAL_PATTERN, Warn,                             \
     "floating-point literals cannot be used in patterns")                     \
  V (UNSTABLE_NAME_COLLISIONS, Warn,                                           \
     "detects name collision with an existing but unstable method")            \
  V (IRREFUTABLE_LET_PATTERNS, Warn,                                           \
     "detects irrefutable patterns in `if let` and `while let` statements")    \
  V (UNUSED_LABELS, Warn, "detects labels that are never used")                \
  V (WHERE_CLAUSES_OBJECT_SAFETY, Warn,                                        \
     "checks the object safety of where clauses")                              \
  V (                                                                          \
    PROC_MACRO_DERIVE_RESOLUTION_FALLBACK, Warn,                               \
    "detects proc macro derives using inaccessible names from parent modules") \
  V (                                                                          \
    MACRO_USE_EXTERN_CRATE, Allow,                                             \
    "the `#[macro_use]` attribute is now deprecated in favor of using macros \
     via the module syste")                                                    \
  V (MACRO_EXPANDED_MACRO_EXPORTS_ACCESSED_BY_ABSOLUTE_PATHS, Deny,            \
     "macro-expanded `macro_export` macros from the current crate \
     cannot be referred to by absolute paths")                                 \
  V (EXPLICIT_OUTLIVES_REQUIREMENTS, Allow,                                    \
     "outlives requirements can be inferred")                                  \
  V (INDIRECT_STRUCTURAL_MATCH, Warn,                                          \
     "constant used in pattern contains value of non-structural-match type "   \
     "in a field or a variant")                                                \
  V (DEPRECATED_IN_FUTURE, Allow,                                              \
     "detects use of items that will be deprecated in a future versio")        \
  V (POINTER_STRUCTURAL_MATCH, Allow, "pointers are not structural-match")     \
  V (                                                                          \
    NONTRIVIAL_STRUCTURAL_MATCH, Warn,                                         \
    "constant used in pattern of non-structural-match type and the constant's initializer \
    expression contains values of non-structural-match types")                 \
  V (AMBIGUOUS_ASSOCIATED_ITEMS, Deny, "ambiguous associated items")           \
  V (MUTABLE_BORROW_RESERVATION_CONFLICT, Warn,                                \
     "reservation of a two-phased borrow conflicts with other shared borrows") \
  V (SOFT_UNSTABLE, Deny,                                                      \
     "a feature gate that doesn't break dependent crates")                     \
  V (INLINE_NO_SANITIZE, Warn,                                                 \
     "detects incompatible use of `#[inline(always)]` and "                    \
     "`#[no_sanitize(...)]`")                                                  \
  V (ASM_SUB_REGISTER, Warn,                                                   \
     "using only a subset of a register for inline asm inputs")                \
  V (BAD_ASM_STYLE, Warn, "incorrect use of inline assembly")                  \
  V (UNSAFE_OP_IN_UNSAFE_FN, Allow,                                            \
     "unsafe operations in unsafe functions without an explicit unsafe block " \
     "are deprecated")                                                         \
  V (WARNINGCENUM_IMPL_DROP_CASTS, Warn,                                       \
     "a C-like enum implementing Drop is cast")                                \
  V (CONST_EVALUATABLE_UNCHECKED, Warn,                                        \
     "detects a generic constant is used in a type without a emitting a "      \
     "warning")                                                                \
  V (FUNCTION_ITEM_REFERENCES, Warn,                                           \
     "suggest casting to a function pointer when attempting to take "          \
     "references to function items")                                           \
  V (UNINHABITED_STATIC, Warn, "uninhabited static")                           \
  V (WARNUSELESS_DEPRECATEDINGS, Deny,                                         \
     "detects deprecation attributes with no effect")                          \
  V (UNSUPPORTED_NAKED_FUNCTIONS, Warn,                                        \
     "unsupported naked function definitions")                                 \
  V (INEFFECTIVE_UNSTABLE_TRAIT_IMPL, Deny,                                    \
     "detects `#[unstable]` on stable trait implementations for stable types") \
  V (SEMICOLON_IN_EXPRESSIONS_FROM_MACROS, Allow,                              \
     "trailing semicolon in macro body used as expression")                    \
  V (LEGACY_DERIVE_HELPERS, Warn,                                              \
     "detects derive helper attributes that are used before they are "         \
     "introduced")                                                             \
  V (UNUSED_DOC_COMMENTS, Warn,                                                \
     "detects doc comments that aren't used by rustdoc")                       \
  V (DISJOINT_CAPTURE_DROP_REORDER, Allow,                                     \
     "Drop reorder because of `capture_disjoint_fields`")                      \
  V (MISSING_ABI, Allow, "No declared ABI for extern declaration")             \
  V (INVALID_DOC_ATTRIBUTES, Warn, "detects invalid `#[doc(...)]` attributes") \
  V (PROC_MACRO_BACK_COMPAT, Warn,                                             \
     "detects usage of old versions of certain proc-macro crates")

class Builtin
{
public:
#define GEN_GETTER(BUILTIN, LEVEL, DESC)                                       \
  Lint *get_##BUILTIN ()                                                       \
  {                                                                            \
    static Lint *instance;                                                     \
    if (instance == nullptr)                                                   \
      instance = new Lint (#BUILTIN, LEVEL, DESC);                             \
    return instance;                                                           \
  }

  BUILTIN_LIST (GEN_GETTER)
#undef GEN_GETTER
};

} // namespace Lint
} // namespace Rust

#endif