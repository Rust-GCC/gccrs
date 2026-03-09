/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-forwprop1-details"  } */

struct s1
{
  char init;
  int value;
};

static inline struct s1 f()
{
  struct s1 t;
  t.init = 0;
  return t;
}

struct s1 g()
{
  return f();
}

/* { dg-final { scan-tree-dump-times "after previous" 1 "forwprop1" { target { { x86_64*-*-* i?86-*-* } && { ! ia32 } } || { aarch64*-*-* } } } } */
