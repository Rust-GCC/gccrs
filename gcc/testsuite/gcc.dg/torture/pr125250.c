/* { dg-do run } */

short __chk, g9, g6;
_Bool g0, g1;
void *g4;

void __attribute__((noipa)) f5(_Bool a2, int a5)
{
  void *a1 = &a2;
lbl_br11:
  *(_Bool *)a1 = 0;
  a1 = &a5;
  if (!a2)
    {
      g1 = *(_Bool *)g4;
      if (g1)
        {
          g9 = -g9;
          goto lbl_br11;
        }
    }
  g6 = a5;
}

int main()
{
  g4 = &g0;
  f5(0, 8);
  __chk = g6;
  if (__chk != 8)
    __builtin_abort ();
}
