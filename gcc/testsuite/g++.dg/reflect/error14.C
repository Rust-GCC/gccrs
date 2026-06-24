// PR c++/125922
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

void
f ()
{
  consteval {
    (void) ^^struct { }; // { dg-error "types may not be defined" }
    (void) ^^struct S { }; // { dg-error "types may not be defined" }
    (void) ^^struct A { struct B { }; }; // { dg-error "types may not be defined" }
    (void) ^^enum E { }; // { dg-error "types may not be defined" }
    (void) ^^union U { }; // { dg-error "types may not be defined" }
  }
}
