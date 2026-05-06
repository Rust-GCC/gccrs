/* { dg-do run } */
/* { dg-options "-O2" } */

typedef unsigned long NI;

struct inner_t_Content
{
  int x[8], y[16];
  char a[10], b[20], c[30];
};

struct inner_t {
  NI len;
  struct inner_t_Content* p;
};

struct outer_t {
  struct inner_t bytes;
  NI position;
};

struct localo_t {
  struct outer_t fo;
  char stuff[222];
};

struct alien_t {
  NI Field0;
  struct inner_t Field1;
};

static struct inner_t_Content gic;
struct inner_t gis;
struct outer_t gos;
volatile int vgi;

int __attribute__((noipa))
get_int (void)
{
  return 0;
}

struct inner_t __attribute__((noipa))
get_inner_t ()
{
  struct inner_t v = {0, (void *)0};
  return v;
}

void __attribute__((noipa))
use_inner_t (struct inner_t v)
{
  gis = v;
}

void __attribute__((noipa))
init (void)
{
  gos.bytes.len = 15;
  gos.bytes.p = &gic;
}

void __attribute__((noipa))
check (void)
{
  if (gis.p != &gic)
    __builtin_abort ();
}

static void foo (int m, struct outer_t *a)
{
  struct inner_t lis;

  if (m == 20)
      ((struct alien_t *) a)->Field1.len = 0;
  if (m == 21)
    lis = get_inner_t ();
  else
    lis = a->bytes;

  vgi = get_int ();
  if (m < 1)
    use_inner_t (lis);
  if (get_int ())
    vgi = ((struct alien_t *) a)->Field1.len;

}

void bar (struct outer_t pos)
{
  struct localo_t los;
  los.fo = pos;
  los.fo.bytes.len = get_int ();
  foo (get_int (), &los.fo);
  gos = los.fo;
}

int main (int argc, char **argv)
{
  init ();
  for (int i = 0; i <= get_int (); i++)
    bar (gos);
  check ();
  return 0;
}
