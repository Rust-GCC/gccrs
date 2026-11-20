// This checks that a path that cannot be resolved because of an indeterminate import
// does not trigger an ICE.

mod foo {
    pub use self::*; // { dg-error ".E0432." "" { target *-*-* } }
}

fn main() {
    foo::f(); // { dg-error ".E0425." "" { target *-*-* } }
}

