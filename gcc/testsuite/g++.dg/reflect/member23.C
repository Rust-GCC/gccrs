// PR c++/124614
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>
using namespace std::meta;

struct B { int id; };

void
f (int p)
{
  B b;
  b.[:reflect_constant (^^B::id):]; // { dg-error "access a class member" }
  b.[:reflect_constant (&B::id):];  // { dg-error "access a class member" }
  b.[:reflect_constant (data_member_spec (^^int, { .name = "i" })):]; // { dg-error "access a class member" }
  b.[:reflect_constant (false):];   // { dg-error "access a class member" }
  b.[:reflect_constant (42):];	    // { dg-error "access a class member" }
  b.[:reflect_constant (3.14f):];   // { dg-error "access a class member" }
  b.[:reflect_constant (&::f):];    // { dg-error "access a class member" }
  b.[:reflect_constant (^^::):];    // { dg-error "access a class member" }
  b.[:reflect_constant (info{}):];  // { dg-error "access a class member" }
  b.[:reflect_constant (^^p):];	    // { dg-error "access a class member" }
}
