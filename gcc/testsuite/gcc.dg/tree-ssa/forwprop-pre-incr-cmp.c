/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-forwprop2" } */

/* Verify that the X +- C1 CMP C2 -> X CMP C2 -+ C1 pattern fires
   when the folded constant is zero, even without single use.
   PR tree-optimization/120283.  */

void g ();

/* Unsigned EQ: ++*a == 1 -> *a == 0.  */
void f1 (unsigned int *a)
{
  if (++*a == 1)
    g ();
}

/* Unsigned NE: ++*a != 1 -> *a != 0.  */
void f2 (unsigned int *a)
{
  if (++*a != 1)
    g ();
}

/* Unsigned EQ with addend > 1: (*a += 3) == 3 -> *a == 0.  */
void f3 (unsigned int *a)
{
  if ((*a += 3) == 3)
    g ();
}

/* Positive: f1, f3 should fold to == 0, f2 should fold to != 0.  */
/* { dg-final { scan-tree-dump-times "== 0" 2 "forwprop2" } } */
/* { dg-final { scan-tree-dump-times "!= 0" 1 "forwprop2" } } */
