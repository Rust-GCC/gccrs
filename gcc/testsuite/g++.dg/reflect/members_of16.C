// PR c++/125889
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>

using L = decltype([]{});
/* There should be at least operator().  */
static_assert(members_of(^^L, std::meta::access_context::unchecked()).size() > 0);
/* This is lazy; there can be more members than the call operator:
   "It is implementation-defined whether declarations of other members of
   a closure type Q are Q-members-of-eligible."  At least we'll know when
   something changes, though.  */
static_assert(is_operator_function(members_of(^^L, std::meta::access_context::unchecked())[0]));
