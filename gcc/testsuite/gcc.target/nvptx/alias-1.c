/* { dg-do link } */
/* { dg-do run { target nvptx_runtime_alias_ptx } } */
/* { dg-options "-save-temps" } */
/* { dg-add-options nvptx_alias_ptx } */

int v;

void __f ()
{
  v = 1;
}

void f () __attribute__ ((alias ("__f")));

int
main (void)
{
  if (v != 0)
    __builtin_abort ();
  f ();
  if (v != 1)
    __builtin_abort ();
  return 0;
}

/* { dg-final { scan-assembler-times {(?n)^// BEGIN GLOBAL FUNCTION DECL: __f$} 1 } }
   { dg-final { scan-assembler-times {(?n)^\.visible \.func __f;$} 1 } }
   { dg-final { scan-assembler-times {(?n)^// BEGIN GLOBAL FUNCTION DEF: __f$} 1 } }
   { dg-final { scan-assembler-times {(?n)^\.visible \.func __f$} 1 } } */

/* { dg-final { scan-assembler-times {(?n)^// BEGIN GLOBAL FUNCTION DECL: f$} 1 } }
   { dg-final { scan-assembler-times {(?n)^\.visible \.func f;$} 1 } }
   { dg-final { scan-assembler-times {(?n)^// BEGIN GLOBAL FUNCTION DEF: f$} 1 } }
   { dg-final { scan-assembler-times {(?n)^\.alias f,__f;$} 1 } } */

/* { dg-final { scan-assembler-times {(?n)\tcall __f;$} 0 } }
   { dg-final { scan-assembler-times {(?n)\tcall f;$} 1 } } */
