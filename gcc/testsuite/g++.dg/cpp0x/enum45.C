// PR c++/118374
// { dg-do compile { target c++11 } }

enum class X : int8_t; // { dg-error "does not name a type" }
