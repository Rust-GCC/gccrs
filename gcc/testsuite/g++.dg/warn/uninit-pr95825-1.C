// { dg-do compile }
// { dg-options "-O1 -fsanitize=address" }

// PR tree-optimization/95825

struct s1
{
  char init;
  int value;
};

static struct s1 f()
{
  struct s1 t;
  t.init = 0;
  return t; /* { dg-bogus "is used uninitialized" } */
}

struct s1 g()
{
  return f();
}
