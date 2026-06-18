/* Verify that unary operators that rvalue-convert a counted_by-annotated
   pointer access do not ICE.  The original PR123569 fix suppressed the
   .ACCESS_WITH_SIZE wrapper creation for ++/-- (which keep an lvalue);
   the rvalue-consuming ops (!, -, +) still receive the wrapper and must
   strip it in build_unary_op.  */
/* { dg-do run } */
/* { dg-options "-std=c99" } */

struct s {
    int n;
    char *p __attribute__((__counted_by__(n)));
};

int test_not (struct s *o)    { return !o->p; }
__INTPTR_TYPE__ test_neg (struct s *o)   { return -(__INTPTR_TYPE__)o->p; }
__INTPTR_TYPE__ test_plus (struct s *o)  { return +(__INTPTR_TYPE__)o->p; }

extern void abort (void);

int main (void) {
    char buf[4] = { 0 };
    struct s s = { .n = 4, .p = buf };
    if (test_not (&s) != 0)  abort ();    /* p is non-null */

    struct s e = { .n = 0, .p = (void*)0 };
    if (test_not (&e) != 1)  abort ();    /* p is null */

    /* test_neg / test_plus must just compile and run without ICE.  */
    (void) test_neg (&s);
    (void) test_plus (&s);
    return 0;
}
