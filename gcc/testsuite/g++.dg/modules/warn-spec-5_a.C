// PR c++/124466
// { dg-additional-options "-fmodules" }
// { dg-module-cmi A }

export module A;

#define PUSH _Pragma("GCC diagnostic push")
#define POP _Pragma("GCC diagnostic pop")

PUSH
#pragma GCC diagnostic ignored "-Wvariadic-macros"
POP
