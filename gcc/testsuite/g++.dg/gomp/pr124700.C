/* { dg-do compile } */
/* { dg-options "-O3" } */

int a (int);
struct b
{
  b ();
  b (b &);
  ~b ();
};
template <typename c> b d (c);
int ab;
void ae (int);
struct e
{
  b g;
  template <typename c> e operator<< (c h)
  {
    d (h);
    return *this;
  }
  b f;
};
template <int = 0> struct i
{
  long ah;
  int j ()
  {
    e () << "" << aj << 1 << "" << ah << "" << ak << "";
    return ah;
  }
  char aj;
  int ak;
};
i<> k;
template <typename>
int
ao (bool h)
{
#pragma omp parallel
  {
    int as;
    for (; as;)
      {
	int au = k.j ();
	ae (au);
	if (h)
	  b ();
      }
  }
  return a (ab);
}
int l = ao<short> (true);
void
az ()
{
  ao<short> (true);
  __builtin_unreachable ();
}
template int ao<short> (bool);
