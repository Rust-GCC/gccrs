// PR c++/62244
// { dg-do compile { target c++11 } }

int a;
void f (int a = a); // { dg-error "parameter .a. may not appear in this context" }

int foo(int x = (decltype(x)(42))) { return 0; }

static int value;

int
bar (int &value = value)  // { dg-error "parameter .value. may not appear in this context" }
{
  return value;
}

void
g ()
{
  bar ();
}

