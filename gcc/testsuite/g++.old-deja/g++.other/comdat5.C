// PR target/17828
// { dg-do link { target fpic } }
// { dg-additional-sources " comdat5-aux.cc" }
// { dg-options "-O2 -fPIC" }

#include "comdat4.C"
