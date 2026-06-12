// Test that { 0, 1 } * -{ 0, 1 } folds to -({ 0, 1 } & { 0, 1 }) and does not
// keep a multiply.  Also check the commutative (negated operand first) form.
// { dg-do compile }
// { dg-options "-O2 -fdump-tree-optimized" }

int f1 (int x, int y) { return (x > 5) * -(y > 5); }
long f2 (long x, long y) { return (long) (x > 5) * -(long) (y > 5); }
unsigned f3 (unsigned x, unsigned y) { return (x > 5u) * -(unsigned) (y > 5u); }
int f4 (int x, int y) { return -(y > 5) * (x > 5); }

// { dg-final { scan-tree-dump-not " \* " "optimized" } }
// { dg-final { scan-tree-dump-times " & " 4 "optimized" } }
