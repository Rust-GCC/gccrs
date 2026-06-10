/* { dg-do compile } */
/* { dg-require-effective-target spaceship } */
/* { dg-options "-O2 -fdump-tree-optimized" } */
/* { dg-final { scan-tree-dump-times {\.SPACESHIP \([^,]+, [^,]+, -1\)} 64 optimized } } */

signed char sp1_1 (signed char a, signed char b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

signed char sp1_2 (signed char a, short b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

signed char sp1_3 (signed char a, int b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

signed char sp1_4 (signed char a, long long b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

signed char sp1_5 (short a, signed char b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

signed char sp1_6 (short a, short b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

signed char sp1_7 (short a, int b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

signed char sp1_8 (short a, long long b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

signed char sp1_9 (int a, signed char b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

signed char sp1_10 (int a, short b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

signed char sp1_11 (int a, int b)
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

short sp2_1 (signed char a, signed char b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

short sp2_2 (signed char a, short b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

short sp2_3 (signed char a, int b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

short sp2_4 (signed char a, long long b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

short sp2_5 (short a, signed char b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

short sp2_6 (short a, short b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

short sp2_7 (short a, int b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

short sp2_8 (short a, long long b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

short sp2_9 (int a, signed char b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

short sp2_10 (int a, short b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

short sp2_11 (int a, int b)
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

int sp3_1 (signed char a, signed char b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

int sp3_2 (signed char a, short b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

int sp3_3 (signed char a, int b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

int sp3_4 (signed char a, long long b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

int sp3_5 (short a, signed char b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

int sp3_6 (short a, short b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

int sp3_7 (short a, int b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

int sp3_8 (short a, long long b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

int sp3_9 (int a, signed char b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

int sp3_10 (int a, short b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

int sp3_11 (int a, int b)
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

long long sp4_1 (signed char a, signed char b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

long long sp4_2 (signed char a, short b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

long long sp4_3 (signed char a, int b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

long long sp4_4 (signed char a, long long b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

long long sp4_5 (short a, signed char b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

long long sp4_6 (short a, short b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

long long sp4_7 (short a, int b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

long long sp4_8 (short a, long long b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

long long sp4_9 (int a, signed char b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

long long sp4_10 (int a, short b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}

long long sp4_11 (int a, int b)
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
