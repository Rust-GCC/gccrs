/* { dg-do compile } */
/* { dg-options "-O3 -fdump-tree-pre --param logical-op-non-short-circuit=1" } */

typedef __UINT64_TYPE__ uint64_t;

int foo(void);

uint64_t noccmp0(uint64_t* s1, uint64_t* s2)
{
    uint64_t d1, d2, d3, d4, bar;
    d1 = *s1++;
    d2 = *s2++;
    d3 = *s1++;
    d4 = *s2++;
    bar = (d1 ^ d2) & 0xabcd;
    if (bar == 0)
      return foo();
    if (d3 != d4)
      d3++;
    else
      return foo();
    return d3;
}

uint64_t noccmp1(uint64_t* s1, uint64_t* s2)
{
    uint64_t d1, d2, d3, d4, bar;
    d1 = *s1++;
    d2 = *s2++;
    d3 = *s1++;
    d4 = *s2++;
    bar = (d1 ^ d2) & 0xabcd;
    if (bar == 0)
      d3++;
    else
      return foo();
    if (d3 > d4)
      d3++;
    else if (d1 != d2)
      return foo ();
    d3 = d3 + d4 + 1;
    return d3;
}

/* Check for condition assignments in the case that the transformation
   is applied.
   The transformation should not be applied on noccmp1, where the foo call is
   on the false branch of the first condition.  */
/* { dg-final { scan-tree-dump-times {_\d+ = d\d+_\d+ != d\d+_\d+;\n  _\d+ = bar_\d+ != 0;} 1 "pre" } } */
/* { dg-final { scan-tree-dump-times {if \(bar_\d+ == 0\)} 1 "pre" } } */
