/* { dg-do compile { target { ! ia32 } } } */
/* { dg-options "-O2" } */

void f(char c[])
{
    for (int i = 0; i < 8; i++)
        c[i] = c[i] ? 'a' : 'c';
}

/* { dg-final { scan-assembler-times "pcmpeqb" 1 } } */
