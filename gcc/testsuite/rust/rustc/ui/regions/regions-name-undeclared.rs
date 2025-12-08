// edition:2018
// Check that lifetime resolver enforces the lifetime name scoping
// rules correctly in various scenarios.

struct Foo<'a> {
    x: &'a isize
}

impl<'a> Foo<'a> {
    // &'a is inherited:
    fn m1(&self, arg: &'a isize) { }
    fn m2(&'a self) { }
    fn m3(&self, arg: Foo<'a>) { }

    // &'b is not:
    fn m4(&self, arg: &'b isize) { } // { dg-error ".E0261." "" { target *-*-* } }
    fn m5(&'b self) { } // { dg-error ".E0261." "" { target *-*-* } }
    fn m6(&self, arg: Foo<'b>) { } // { dg-error ".E0261." "" { target *-*-* } }
}

fn bar<'a>(x: &'a isize) {
    // &'a is visible to code:
    let y: &'a isize = x;

    // &'a is not visible to *items*:
    type X = Option<&'a isize>; // { dg-error ".E0261." "" { target *-*-* } }
    enum E {
        E1(&'a isize) // { dg-error ".E0261." "" { target *-*-* } }
    }
    struct S {
        f: &'a isize // { dg-error ".E0261." "" { target *-*-* } }
    }
    fn f(a: &'a isize) { } // { dg-error ".E0261." "" { target *-*-* } }

    // &'a CAN be declared on functions and used then:
    fn g<'a>(a: &'a isize) { } // OK
    fn h(a: Box<dyn for<'a> FnOnce(&'a isize)>) { } // OK
}

// Test nesting of lifetimes in fn type declarations
fn fn_types(a: &'a isize, // { dg-error ".E0261." "" { target *-*-* } }
            b: Box<dyn for<'a> FnOnce(&'a isize,
                                  &'b isize, // { dg-error ".E0261." "" { target *-*-* } }
                                  Box<dyn for<'b> FnOnce(&'a isize,
                                                     &'b isize)>,
                                  &'b isize)>, // { dg-error ".E0261." "" { target *-*-* } }
            c: &'a isize) // { dg-error ".E0261." "" { target *-*-* } }
{
}

struct Bug {}
impl Bug {
    async fn buggy(&self) -> &'a str { // { dg-error ".E0261." "" { target *-*-* } }
        todo!()
    }
}

pub fn main() {}

