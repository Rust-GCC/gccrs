// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }
// Test feature test macros.

#ifndef __cpp_impl_reflection
#  error "__cpp_impl_reflection"
#elif __cpp_impl_reflection != 202603
#  error "__cpp_impl_reflection != 202603"
#endif

#include <meta>

#ifndef __cpp_lib_reflection
#  error "__cpp_lib_reflection"
#elif __cpp_lib_reflection != 202603
#  error "__cpp_lib_reflection != 202603"
#endif
