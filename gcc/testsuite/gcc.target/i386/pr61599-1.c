/* PR target/61599 */
/* { dg-do run { target lp64 } } */
/* { dg-skip-if "PR90698" { *-*-darwin* } } */
/* { dg-additional-sources pr61599-2.c } */
/* { dg-options "-mcmodel=medium -fdata-sections -save-temps" } */

char a[1*1024*1024*1024];
char b[1*1024*1024*1024];
char c[1*1024*1024*1024];

extern int bar();
int main()
{
  return bar() + c[225];
}

/* { dg-final { scan-assembler-times {\.section[ \t]+\.lbss\..,"aw[hl]",@nobits} 3 } } */
