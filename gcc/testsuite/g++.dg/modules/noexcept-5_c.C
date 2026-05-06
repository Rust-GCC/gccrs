// PR c++/124785
// { dg-do compile { target c++20 } }
// { dg-additional-options "-fmodules -fdump-lang-module-alias" }
// { dg-module-cmi M }

export module M;
export import :A;
export import :B;

// The noexcept-specifiers are equivalent, no need to merge.
// { dg-final { scan-lang-dump-not {Propagating instantiated noexcept} module } }
