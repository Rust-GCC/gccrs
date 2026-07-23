// Test that slicing syntax gives errors if we have not implemented the trait.

struct Foo;

fn main() {
    let x = Foo;
    &x[..]; // { dg-error ".E0608." "" { target *-*-* } }
    &x[Foo..]; // { dg-error ".E0608." "" { target *-*-* } }
    &x[..Foo]; // { dg-error ".E0608." "" { target *-*-* } }
    &x[Foo..Foo]; // { dg-error ".E0608." "" { target *-*-* } }
}

