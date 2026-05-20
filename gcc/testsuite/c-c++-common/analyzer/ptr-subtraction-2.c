#include "analyzer-decls.h"

typedef __PTRDIFF_TYPE__ ptrdiff_t;
typedef __SIZE_TYPE__ size_t;

ptrdiff_t
ptrdiff_hidden_from_optimizer (const char *, const char *)
  __attribute__((noinline));

ptrdiff_t
ptrdiff_hidden_from_optimizer (const char *p, const char *q)
{
  return p - q;
}

void
test_concrete (void)
{
  const char *abc = "abc";
  __analyzer_eval (ptrdiff_hidden_from_optimizer (abc + 3, abc) == 3); // { dg-warning "TRUE" }
}

void
test_symbolic (size_t sz)
{
  const char *abc = "abc";
  __analyzer_eval (ptrdiff_hidden_from_optimizer (abc + sz, abc) == sz); // { dg-warning "TRUE" }
}
