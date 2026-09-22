#![feature(no_core)]
#![no_core]

trait Tr {}

struct Foo<'a, 'a>(&'a u8); // { dg-error "already used for a generic parameter" }

fn foo<'a, 'a>() {} // { dg-error "already used for a generic parameter" }

trait Bar<'a, 'a> {} // { dg-error "already used for a generic parameter" }

type Al<'a, 'a> = &'a u8; // { dg-error "already used for a generic parameter" }

struct I<'a>(&'a u8);
impl<'a, 'a> I<'a> {} // { dg-error "already used for a generic parameter" }

struct H where for<'b, 'b> u8: Tr; // { dg-error "already used for a generic parameter" }

// An inner binder may reuse an outer lifetime name - that is lifetime
// shadowing (E0496), not a duplicate parameter, and is out of scope here.
struct A<'a>(&'a u8) where for<'a> &'a u8: Tr;

// Distinct lifetimes are fine.
struct Ok1<'a, 'b>(&'a u8, &'b u8);
fn ok2<'a, 'b>(_: &'a u8, _: &'b u8) {}
