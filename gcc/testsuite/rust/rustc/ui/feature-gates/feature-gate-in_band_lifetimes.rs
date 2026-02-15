#![allow(warnings)]

fn foo(x: &'x u8) -> &'x u8 { x }
// { dg-error ".E0261." "" { target *-*-* } .-1 }
// { dg-error ".E0261." "" { target *-*-* } .-2 }

struct X<'a>(&'a u8);

impl<'a> X<'a> {
    fn inner(&self) -> &'a u8 {
        self.0
    }
}

impl<'a> X<'b> {
// { dg-error ".E0261." "" { target *-*-* } .-1 }
    fn inner_2(&self) -> &'b u8 {
// { dg-error ".E0261." "" { target *-*-* } .-1 }
        self.0
    }
}

impl X<'b> {
// { dg-error ".E0261." "" { target *-*-* } .-1 }
    fn inner_3(&self) -> &'b u8 {
// { dg-error ".E0261." "" { target *-*-* } .-1 }
        self.0
    }
}

struct Y<T>(T);

impl Y<&'a u8> {
// { dg-error ".E0261." "" { target *-*-* } .-1 }
    fn inner(&self) -> &'a u8 {
// { dg-error ".E0261." "" { target *-*-* } .-1 }
        self.0
    }
}

trait MyTrait<'a> {
    fn my_lifetime(&self) -> &'a u8;
    fn any_lifetime() -> &'b u8;
// { dg-error ".E0261." "" { target *-*-* } .-1 }
    fn borrowed_lifetime(&'b self) -> &'b u8;
// { dg-error ".E0261." "" { target *-*-* } .-1 }
// { dg-error ".E0261." "" { target *-*-* } .-2 }
}

impl MyTrait<'a> for Y<&'a u8> {
// { dg-error ".E0261." "" { target *-*-* } .-1 }
// { dg-error ".E0261." "" { target *-*-* } .-2 }
    fn my_lifetime(&self) -> &'a u8 { self.0 }
// { dg-error ".E0261." "" { target *-*-* } .-1 }
    fn any_lifetime() -> &'b u8 { &0 }
// { dg-error ".E0261." "" { target *-*-* } .-1 }
    fn borrowed_lifetime(&'b self) -> &'b u8 { &*self.0 }
// { dg-error ".E0261." "" { target *-*-* } .-1 }
// { dg-error ".E0261." "" { target *-*-* } .-2 }
}

fn main() {}

