// DR 2229, Volatile unnamed bit-fields
// PR c++/123935

typedef const unsigned cu;
typedef volatile unsigned vu;
typedef const volatile unsigned cvu;

struct S {
  const unsigned : 1; // { dg-error "unnamed bit-field cannot be cv-qualified" }
  volatile unsigned : 1; // { dg-error "unnamed bit-field cannot be cv-qualified" }
  const volatile unsigned : 1; // { dg-error "unnamed bit-field cannot be cv-qualified" }
  unsigned : 1;
  const unsigned i1 : 1;
  volatile unsigned i2 : 1;
  const volatile unsigned i3 : 1;

  cu : 1;   // { dg-error "unnamed bit-field cannot be cv-qualified" }
  vu : 1;   // { dg-error "unnamed bit-field cannot be cv-qualified" }
  cvu : 1;  // { dg-error "unnamed bit-field cannot be cv-qualified" }
  cu i4: 1;
  vu i5 : 1;
  cvu i6 : 1;
};
