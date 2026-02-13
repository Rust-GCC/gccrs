// { dg-do compile { target lp64 } }

// PR tree-optimization/124086

int j;
int *v;
void
foo()
{
  int t;
  v = &t;
  auto t1 = __builtin_bit_cast(_Complex int, v);
  j = __real__ t1;
}
