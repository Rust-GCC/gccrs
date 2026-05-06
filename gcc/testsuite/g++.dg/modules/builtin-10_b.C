// PR c++/124477
// { dg-additional-options "-fmodules -O3 -fdump-lang-module-alias" }

import M;
void foo(void *data) {
  memset(data, 0xFF, 4);
}

// { dg-final { scan-lang-dump {Deduping '::memset'} module } }
