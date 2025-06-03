/* { dg-do compile } */
/* { dg-options "-Wno-old-style-definition -O1 -fdump-tree-dom2" } */

extern void abort (void);
struct rtx_def;
typedef struct rtx_def *rtx;
struct rtx_def
{
  int code;
};
void
foo (reg)
     rtx reg;
{
  reg->code = 42;
  if (reg->code != 42)
    abort ();
}
                                                                                
/* There should be no IF conditionals.  */
/* { dg-final { scan-tree-dump-times "if " 0 "dom2"} } */
