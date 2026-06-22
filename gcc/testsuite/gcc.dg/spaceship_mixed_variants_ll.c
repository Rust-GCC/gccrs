/* { dg-do compile } */
/* { dg-skip-if "DI mode spaceship optab not available" {  { ilp32 } || { int16 } } } */
/* { dg-require-effective-target spaceship } */
/* { dg-options "-O2 -fdump-tree-optimized" } */
/* { dg-final { scan-tree-dump-times {\.SPACESHIP \([^,]+, [^,]+, -1\)} 28 optimized } } */

signed char sp1_4 (signed char a, long long b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

signed char sp1_8 (short a, long long b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

signed char sp1_12 (int a, long long b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

signed char sp1_13 (long long a, signed char b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

signed char sp1_14 (long long a, short b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

signed char sp1_15 (long long a, int b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

signed char sp1_16 (long long a, long long b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

short sp2_4 (signed char a, long long b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

short sp2_8 (short a, long long b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

short sp2_12 (int a, long long b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

short sp2_13 (long long a, signed char b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

short sp2_14 (long long a, short b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

short sp2_15 (long long a, int b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

short sp2_16 (long long a, long long b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

int sp3_4 (signed char a, long long b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

int sp3_8 (short a, long long b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

int sp3_12 (int a, long long b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

int sp3_13 (long long a, signed char b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

int sp3_14 (long long a, short b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

int sp3_15 (long long a, int b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

int sp3_16 (long long a, long long b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

long long sp4_4 (signed char a, long long b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

long long sp4_8 (short a, long long b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

long long sp4_12 (int a, long long b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

long long sp4_13 (long long a, signed char b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

long long sp4_14 (long long a, short b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

long long sp4_15 (long long a, int b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

long long sp4_16 (long long a, long long b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}
