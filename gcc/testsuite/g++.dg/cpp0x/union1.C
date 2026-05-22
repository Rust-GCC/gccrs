// Negative test for C++11 unrestricted unions
// { dg-do compile { target c++11 } }
// { dg-prune-output "implicitly deleted because" }

struct A
{
  A();
  A(const A&);
  ~A();
};

union B
{
  A a;				// { dg-error "union member" }
};

B b;				// { dg-error "B::B\\(\\)" "B::B" { target c++23_down } }
B b2(b);			// { dg-error "B::B\\(const B&\\)" "B::B" }

struct C
{
  union
  {
    A a;			// { dg-error "union member" }
  };
};

C c;				// { dg-error "C::C\\(\\)" "C::C" { target c++23_down } }
C c2(c);			// { dg-error "C::C\\(const C&\\)" "C::C" }

// { dg-error "B::~B" "B::~B" { target c++23_down } 17 }
// { dg-error "B::~B" "B::~B" { target c++23_down } 18 }
// { dg-error "C::~C" "C::~C" { target c++23_down } 28 }
// { dg-error "C::~C" "C::~C" { target c++23_down } 29 }
