/* { dg-skip-if "" { *-*-* } { "-fno-fat-lto-objects" } { "" } } */

// { dg-additional-options "-D_GLIBCXX_NO_ASSERTIONS" }
// { dg-additional-options "--param=analyzer-supernode-explosion-factor=10" }

#include "../../../gcc.dg/analyzer/analyzer-decls.h"
#include <memory>

void
test_null_deref_1 ()
{
  std::unique_ptr<int> p;
  __analyzer_eval (*p == 42); // { dg-warning "null-dereference" }
}

void
test_null_deref_2 ()
{
  auto p = std::make_unique<int> (42);
  __analyzer_eval (*p == 42); // { dg-warning "TRUE" }
  p = nullptr;
  __analyzer_eval (*p); // { dg-warning "null-dereference" }
}

void
test_use_after_free ()
{
  auto p = std::make_unique<int> (42);
  __analyzer_eval (*p == 42); // { dg-warning "TRUE" }
  int *ptr = p.get ();
  p = nullptr;
  __analyzer_eval (*ptr); // { dg-warning "use after 'delete'" }
}
