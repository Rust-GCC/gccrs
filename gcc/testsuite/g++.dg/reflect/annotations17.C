// CWG3124 - Disallow annotations on block-scope externs and non-unique friend declarations
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

void
foo ()
{
  [[=1]] extern int a;			// { dg-error "annotation applied to block scope extern 'a'" }
  extern int b [[=2]];			// { dg-error "annotation applied to block scope extern 'b'" }
  [[=3]] extern int bar (int);		// { dg-error "annotation applied to block scope extern 'int bar\\\(int\\\)'" }
  extern int baz [[=4]] (int);		// { dg-error "annotation applied to block scope extern 'int baz\\\(int\\\)'" }
  extern int qux (int a[[=5]]);		// { dg-error "annotation applied to parameter 'a' of block scope extern" }
}

class S
{
  int s;
  S ();
  [[=6]] friend void corge ();		// { dg-error "annotation applied to non-defining friend declaration 'void corge\\\(\\\)'" }
  friend void garply ([[=7]] int a);	// { dg-error "annotation applied to parameter 'a' of non-defining friend declaration" }
  [[=8]] friend void fred () {}
  friend void xyzzy (int a[[=9]]) {}
  template <typename T>
  [[=10]] friend void quux ();		// { dg-error "annotation applied to non-defining friend declaration 'void quux\\\(\\\)'" }
  template <typename T>
  friend void grault ([[=11]] int a);	// { dg-error "annotation applied to parameter 'a' of non-defining friend declaration" }
  template <typename T>
  [[=12]] friend void waldo () {}
  template <typename T>
  friend void thud (int a[[=13]]) {}
};

template <typename T>
class U
{
  int u;
  [[=14]] friend void corge (long);	// { dg-error "annotation applied to non-defining friend declaration 'void corge\\\(long int\\\)'" }
  friend void garply (long, [[=16]] int a);	// { dg-error "annotation applied to parameter 'a' of non-defining friend declaration" }
  [[=16]] friend void fred (long) {}
  friend void xyzzy (long, int a[[=17]]) {}
  template <typename V>
  [[=18]] friend void quux (long);	// { dg-error "annotation applied to non-defining friend declaration 'void quux\\\(long int\\\)'" }
  template <typename V>
  friend void grault (long, [[=19]] int a);	// { dg-error "annotation applied to parameter 'a' of non-defining friend declaration" }
  template <typename V>
  [[=20]] friend void waldo (long) {}
  template <typename V>
  friend void thud (long, int a[[=21]]) {}
public:
  U () : u (42) {}
};

U <int> u;
