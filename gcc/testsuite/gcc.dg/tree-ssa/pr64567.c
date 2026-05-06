/* { dg-do run } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

#define F1 0x01
#define F2 0x02

#define DECLS(n,VOL)			\
__attribute__((noinline,noclone))	\
unsigned foo##n(unsigned A)		\
{					\
  VOL unsigned flags = A;		\
  if (flags & (F1 | F2))		\
    flags &= ~(F1 | F2);		\
  return flags; 			\
}					\
__attribute__((noinline,noclone))	\
unsigned bar##n(unsigned A)		\
{					\
  VOL unsigned flags = A;		\
  if (!(flags & F1))			\
    flags |= F1;			\
  return flags; 			\
}					\

DECLS(0,)
DECLS(1,volatile)

int main ()
{
  for(int A = 0; A < 4; A++)
    {
      if (foo0 (A) != foo1 (A)) __builtin_abort();
      if (bar0 (A) != bar1 (A)) __builtin_abort();
    }
}

/* foo1 and bar1 will add 2 gotos each since they are not
   being optimized, 'main' will add +6 (2 gotos for the loop,
   2 gotos for each abort check).  */
/* { dg-final { scan-tree-dump-times " goto " 10 optimized } } */
