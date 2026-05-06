/* { dg-do compile } */
/* { dg-options "-O2" } */

void foo(char c[])
{
    for (int i = 0; i < 8; i++)
        c[i] = c[i] != 'a' ? 'c' : 'e';
}

void bar(char c[])
{
    for (int i = 0; i < 8; i++)
        c[i] = c[i] == 'a' ? 'c' : 'e';
}

/* { dg-final { scan-assembler-not "\tnot\t" } } */
/* { dg-final { scan-assembler-times "\tcmeq\t" 2 } } */
