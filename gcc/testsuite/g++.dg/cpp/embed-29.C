// PR c++/124531
// { dg-do run { target c++11 } }
// { dg-options "-O2 --embed-dir=${srcdir}/c-c++-common/cpp/embed-dir" }

unsigned char *
foo ()
{
  return new unsigned char [] {
#embed <magna-carta.txt> limit (231)
    };
}

int
main ()
{
  unsigned char *p = foo ();
  static unsigned char q[] = {
#embed <magna-carta.txt> limit (231)
  };
  for (int i = 0; i < 231; ++i)
    if (p[i] != q[i])
      __builtin_abort ();
  delete [] p;
}
