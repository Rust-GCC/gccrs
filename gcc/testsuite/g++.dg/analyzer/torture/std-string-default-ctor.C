/* Verify that the analyzer is silent on correct usage of std::string,
   using <string>.  */

// { dg-skip-if "" { *-*-* } { "-fno-fat-lto-objects" } { "" } }

/* Leaving assertions at default: we seem to need assertions at -O0
   for the test to pass.  */

#include "../../../gcc.dg/analyzer/analyzer-decls.h"
#include <string>

void
test_ctor_empty ()
{
  std::string s;
  __analyzer_eval (s.length () == 0); // { dg-warning "TRUE" }
  __analyzer_eval (s.size () == 0); // { dg-warning "TRUE" }
  __analyzer_eval (__builtin_strlen (s.c_str()) == 0); // { dg-warning "TRUE" }
}
