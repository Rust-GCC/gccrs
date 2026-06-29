/* PR tree-optimization/112390 */
/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

#include <limits.h>

int f32_bitand(unsigned int a)
{
  return ((int)a >= 0) & ((a & (unsigned)INT_MAX) != 0);
}

int f32_truthand(unsigned int a)
{
  return ((int)a >= 0) && ((a & (unsigned)INT_MAX) != 0);
}

int f32_bitior(unsigned int a)
{
  return ((int)a < 0) | ((a & (unsigned)INT_MAX) == 0);
}

int f32_truthor(unsigned int a)
{
  return ((int)a < 0) || ((a & (unsigned)INT_MAX) == 0);
}

int f32s_bitand(int a)
{
  return (a >= 0) & ((a & INT_MAX) != 0);
}

int f32s_truthand(int a)
{
  return (a >= 0) && ((a & INT_MAX) != 0);
}

int f32s_bitior(int a)
{
  return (a < 0) | ((a & INT_MAX) == 0);
}

int f32s_truthor(int a)
{
  return (a < 0) || ((a & INT_MAX) == 0);
}

int f64_bitand(unsigned long long a)
{
  return ((long long)a >= 0) & ((a & (unsigned long long)LLONG_MAX) != 0);
}

int f64_truthand(unsigned long long a)
{
  return ((long long)a >= 0) && ((a & (unsigned long long)LLONG_MAX) != 0);
}

int f64_bitior(unsigned long long a)
{
  return ((long long)a < 0) | ((a & (unsigned long long)LLONG_MAX) == 0);
}

int f64_truthor(unsigned long long a)
{
  return ((long long)a < 0) || ((a & (unsigned long long)LLONG_MAX) == 0);
}

int f64s_bitand(long long a)
{
  return (a >= 0) & ((a & LLONG_MAX) != 0);
}

int f64s_truthand(long long a)
{
  return (a >= 0) && ((a & LLONG_MAX) != 0);
}

int f64s_bitior(long long a)
{
  return (a < 0) | ((a & LLONG_MAX) == 0);
}

int f64s_truthor(long long a)
{
  return (a < 0) || ((a & LLONG_MAX) == 0);
}

/* { dg-final { scan-tree-dump-times ">= 0" 0 "optimized" } } */
/* { dg-final { scan-tree-dump-times "< 0" 0 "optimized" } } */
/* { dg-final { scan-tree-dump-times "BIT_AND_EXPR" 0 "optimized" } } */
/* { dg-final { scan-tree-dump-not "bit_and" "optimized" } } */
/* { dg-final { scan-tree-dump-not "bit_ior" "optimized" } } */
/* { dg-final { scan-tree-dump-times "> 0" 8 "optimized" } } */
/* { dg-final { scan-tree-dump-times "<= 0" 8 "optimized" } } */
