// PR c++/109521
// { dg-do compile { target c++11 } }
// { dg-options "-O2 -Wall" }

struct vector {
  vector& operator=(vector&&)
  {
    return *this;
  }
};
struct T {	// { dg-warning "is used uninitialized" }
  vector v;
  unsigned char pad[7];
  T(){}
};
T t;
void
foo ()
{
  t = T();
}
