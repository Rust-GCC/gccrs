const MAX_ITEM: usize = 10;

fn foo_bar() {}

fn foo(c: esize) {} // Misspelled primitive type name.
// { dg-error ".E0412." "" { target *-*-* } .-1 }

enum Bar { }

type A = Baz; // Misspelled type name.
// { dg-error ".E0412." "" { target *-*-* } .-1 }
type B = Opiton<u8>; // Misspelled type name from the prelude.
// { dg-error ".E0412." "" { target *-*-* } .-1 }

mod m {
    type A = Baz; // No suggestion here, Bar is not visible
// { dg-error ".E0412." "" { target *-*-* } .-1 }

    pub struct First;
    pub struct Second;
}

fn main() {
    let v = [0u32; MAXITEM]; // Misspelled constant name.
// { dg-error ".E0425." "" { target *-*-* } .-1 }
    foobar(); // Misspelled function name.
// { dg-error ".E0425." "" { target *-*-* } .-1 }
    let b: m::first = m::second; // Misspelled item in module.
// { dg-error ".E0425." "" { target *-*-* } .-1 }
// { dg-error ".E0425." "" { target *-*-* } .-2 }
}

