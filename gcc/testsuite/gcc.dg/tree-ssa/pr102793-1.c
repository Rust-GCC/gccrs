/* { dg-do compile } */
/* { dg-options "-O3 -fdump-tree-pre --param logical-op-non-short-circuit=1" } */

typedef __UINT64_TYPE__ uint64_t;

int foo(void);

int ccmp(uint64_t* s1, uint64_t* s2)
{
    uint64_t d1, d2, bar;
    d1 = *s1++;
    d2 = *s2++;
    bar = (d1 ^ d2) & 0xabcd;
    if (bar == 0 || d1 != d2)
      return foo();
    return 0;
}

int noccmp0(uint64_t* s1, uint64_t* s2)
{
    uint64_t d1, d2, bar;

    d1 = *s1++;
    d2 = *s2++;
    bar = (d1 ^ d2) & 0xabcd;
    if (bar == 0)
      return foo();
    if (d1 != d2)
      return foo();
    return 0;
}

int noccmp1(uint64_t* s1, uint64_t* s2)
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
      return foo();
    return 0;
}

/* Check for condition assignments for noccmp0 and noccmp1.  */
/* { dg-final { scan-tree-dump-times {_\d+ = d\d+_\d+ != d\d+_\d+;\n  _\d+ = bar_\d+ == 0;} 2 "pre" } } */
