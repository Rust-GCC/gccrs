/* { dg-skip-if "too many arguments in function call" { bpf-*-* } } */

foo (a, b, c, d, e, f, g, h, i, j, xx)
     double xx;
{
  return xx + 1.2345;
}
