/* { dg-do compile } */
/* { dg-options "-O -masm=att" } */

typedef union { unsigned char x; signed char y; }
__attribute__((__transparent_union__)) T;
void bar(int, T x);
void foo()
{
  bar(0, (T) { 42 });
  bar(1, (T) { -42 });
}

/* PROMOTE_PROTOTYPES should make us pass 42 and 214, not -42, both would
   be valid according to the ABI.  */
/* { dg-final { scan-assembler "movl\[ \t\]+\\\$42, %esi" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "movl\[ \t\]+\\\$214, %esi" { target { ! ia32 } } } } */
