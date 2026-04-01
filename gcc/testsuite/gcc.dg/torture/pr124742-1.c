/* { dg-do compile } */
/* PR tree-optimization/124742 */

typedef __SIZE_TYPE__ size_t;
struct S {
    int v[4];
} s;

void *memset(void *p, int c, size_t n)
{
    return memset(p, c, n);
}

int main()
{
    memset(s.v, 0, sizeof(s.v));
    return s.v[0];
}
