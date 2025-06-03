/* { dg-do compile } */
/* { dg-options "-O2" } */
/* { dg-require-effective-target int32plus } */

extern double pow(double x, double y);

struct S {
    unsigned int a : 3, b : 8, c : 21;
};

void foo (struct S *p)
{
  pow (p->c, 42);
}
