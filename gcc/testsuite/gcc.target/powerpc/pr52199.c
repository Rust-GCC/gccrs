/* { dg-do compile { target { powerpc*-*-* && lp64 } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } } */
/* { dg-options "-O3 -mdejagnu-cpu=power7 -fmerge-all-constants" } */
/* { dg-require-effective-target powerpc_vsx } */

struct locale_time_t
{
  const char *abday[7];
  const unsigned int *wabday[7];
};

static const unsigned int empty_wstr[1] = { 0 };

void
time_read (struct locale_time_t *time)
{
  int cnt;

  for (cnt=0; cnt < 7; cnt++)
    {
      time->abday[cnt] = "";
      time->wabday[cnt] = empty_wstr;
    }
}
