const _: () = ();

fn main() {
    a // Shouldn't suggest underscore
// { dg-error ".E0425." "" { target *-*-* } .-1 }
}

trait Unknown {}

#[allow(unused_imports)]
use Unknown as _;

fn foo<T: A>(x: T) {} // Shouldn't suggest underscore
// { dg-error ".E0405." "" { target *-*-* } .-1 }

