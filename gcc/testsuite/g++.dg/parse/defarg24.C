// PR c++/50479
// { dg-do compile { target c++11 } }

void fn1 (int x = []{ return x; }()) {}	      // { dg-error "use of parameter outside function body" }
void fn2 (int x, int = []{ return x; }()) {}  // { dg-error "use of parameter outside function body" }

void
g ()
{
  void fn3 (int z = []{ return z; }());	      // { dg-error "use of parameter outside function body" }
  void fn4 (int z, int = []{ return z; }());  // { dg-error "use of parameter outside function body" }
}
