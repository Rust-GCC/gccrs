/* PR middle-end/124056 */
/* { dg-do run } */
/* { dg-options "-O2 -masm=att" } */

int v;

[[gnu::noipa]] void
foo (void)
{
  v = 42;
}

[[gnu::noipa]] void
bar (void)
{
  v = 43;
}

[[gnu::noipa]] int
baz (void)
{
  bool err;
  __asm goto ("movl %1, %%ecx\n\t"
	       "test %%ecx, %%ecx\n\t"
	       "js 1f\n\t"
	       "jmp %l[lab]\n\t"
	    "1: cmpl $-1, %%ecx"
	       : "=@ccz" (err) : "m" (v) : "ecx" : lab);
  if (err)
    {
      bar ();
      return -1;
    }
  return 0;

 lab:
  if (err)
    {
      foo ();
      return -2;
    }

  return 1;
}

int
main ()
{
  v = 0;
  if (baz () != -2 || v != 42)
    __builtin_abort ();
  v = 1;
  if (baz () != 1 || v != 1)
    __builtin_abort ();
  v = -1;
  if (baz () != -1 || v != 43)
    __builtin_abort ();
  v = -2;
  if (baz () != 0 || v != -2)
    __builtin_abort ();
}
