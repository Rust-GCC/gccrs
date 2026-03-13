// PR c++/124466
// { dg-additional-options "-fmodules -fdump-lang-module" }
// { dg-module-cmi B }

export module B;
export import A;

// { dg-final { scan-lang-dump {Diagnostic changes: 0} module } }
