short s;

short
foo()
{
  short t = 0;
  __atomic_fetch_sub(&t, s, 0);
  return t ?: 5;
}
