/* { dg-do compile } */
/* { dg-require-effective-target spaceship } */
/* { dg-options "-O2 -fdump-tree-optimized" } */
/* { dg-final { scan-tree-dump-times {\.SPACESHIP \([^,]+, [^,]+, -1\)} 24 optimized } } */

int sp1 (int a, int b)
{
    return a < b ? -1 : a > b ? 1 : 0;
}
int sp1_1 (int a, int b)
{
    return a < b ? -1 : a == b ? 0 : 1;
}
int sp1_2 (int a, int b)
{
    return a < b ? -1 : a != b ? 1 : 0;
}
int sp1_3 (int a, int b)
{
    return a < b ? -1 : a <= b ? 0 : 1;
}
int sp2 (int a, int b)
{
    return a == b ? 0 : a > b ? 1 : -1;
}
int sp2_1 (int a, int b)
{
    return a == b ? 0 : a < b ? -1 : 1;
}
int sp2_2 (int a, int b)
{
    return a == b ? 0 : a >= b ? 1 : -1;
}
int sp2_3 (int a, int b)
{
    return a == b ? 0 : a <= b ? -1 : 1;
}
int sp3 (int a, int b)
{
    return a != b ? (a > b ? 1 : -1) : 0;
}
int sp3_1 (int a, int b)
{
    return a != b ? (a < b ? -1 : 1) : 0;
}
int sp3_2 (int a, int b)
{
    return a != b ? (a >= b ? 1 : -1) : 0;
}
int sp3_3 (int a, int b)
{
    return a != b ? (a <= b ? -1 : 1) : 0;
}
int sp4 (int a, int b)
{
    return a > b ? 1 : a == b ? 0 : -1;
}
int sp4_1 (int a, int b)
{
    return a > b ? 1 : a < b ? -1 : 0;
}
int sp4_2 (int a, int b)
{
    return a > b ? 1 : a != b ? -1 : 0;
}
int sp4_3 (int a, int b)
{
    return a > b ? 1 : a >= b ? 0 : -1;
}
int sp5 (int a, int b)
{
    return a <= b ? (a == b ? 0 : -1) : 1;
}
int sp5_1 (int a, int b)
{
    return a <= b ? (a >= b ? 0 : -1) : 1;
}
int sp5_2 (int a, int b)
{
    return a <= b ? (a != b ? -1 : 0) : 1;
}
int sp5_3 (int a, int b)
{
    return a <= b ? (a < b ? -1 : 0) : 1;
}
int sp6 (int a, int b)
{
    return a >= b ? (a == b ? 0 : 1) : -1;
}
int sp6_1 (int a, int b)
{
    return a >= b ? (a != b ? 1 : 0) : -1;
}
int sp6_2 (int a, int b)
{
    return a >= b ? (a <= b ? 0 : 1) : -1;
}
int sp6_3 (int a, int b)
{
    return a >= b ? (a > b ? 1 : 0) : -1;
}
