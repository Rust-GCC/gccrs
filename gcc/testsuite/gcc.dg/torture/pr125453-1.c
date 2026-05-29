/* { dg-do compile } */
/* { dg-additional-options "-fgimple" } */
/* Disable all warnings since the code in h is undefined and could
   invoke -Wstringop-overflow warnigns and others. */
/* { dg-additional-options "-w" } */
/* PR tree-optimization/125453 */

int b[10];

_Bool __GIMPLE f(int *c)
{
    unsigned t;
    unsigned t1;
    int t2;
    _Bool t3;
    t = (unsigned)c;
    t1 = (unsigned)_Literal (char *)&b;
    t = t - t1;
    t2 = (int)t;
    t3 = t2 != 0;
    return t3;
}



int g1(void);
int f1(const char *);

void h(void) {
  char b[1], *c;
  int d, e;
  for (d = 0, c = b; d < 3; d++, c++) {
    g1();
    if (d && e)
      break;
  }
  *c = '\0';
  f1(b);
}
