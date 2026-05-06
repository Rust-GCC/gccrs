// PR tree-optimization/124061
// { dg-do compile { target c++11 } }
// { dg-options "-O2 -fdump-tree-sccp" }

enum a : unsigned {};

int f(a limit)
{
  a i;
  for(i = (a)0; i < limit; i = a(i + 1))
   ;
  return i;
}

// { dg-final { scan-tree-dump "final value replacement:"  sccp } }
