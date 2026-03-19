// PR c++/124483
// { dg-additional-options "-fmodules -Wunused-parameter" }
// { dg-module-cmi B }

export module B;

export template <typename T = void>
void foo([[maybe_unused]] int x) {}
