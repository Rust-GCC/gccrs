#include "../../gcc.dg/analyzer/analyzer-decls.h"

template <typename Src, typename Dst>
Dst *
ptr_cast (Src *ptr)  __attribute__((noinline));

template <typename Src, typename Dst>
Dst *
ptr_cast (Src *ptr)
{
  return (Dst *)ptr;
}

void test_equality (unsigned *p)
{
  __analyzer_eval (p == (unsigned *)ptr_cast<unsigned, signed> (p)); // { dg-warning "TRUE" }

  unsigned char buf[16];
  __analyzer_eval (buf == &buf[0]); // { dg-warning "TRUE" }

  __analyzer_eval (buf == (unsigned char*)ptr_cast<unsigned char, signed char> (&buf[0])); // { dg-warning "TRUE" }
}
