/* { dg-do compile } */
/* { dg-options "-Ofast -march=armv8-a+sve -mmax-vectorization -ffreestanding -fdump-tree-vect-details" } */

unsigned long f(const char *s)
{
    unsigned long i = 0;
    while (*s++)
        i++;
    return i;
}

/* { dg-final { scan-tree-dump "vectorized 1 loops in function" "vect" } } */
