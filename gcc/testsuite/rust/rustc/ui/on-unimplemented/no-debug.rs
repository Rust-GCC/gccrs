// aux-build:no_debug.rs

extern crate no_debug;

use no_debug::Bar;

struct Foo;

fn main() {
    println!("{:?} {:?}", Foo, Bar);
    println!("{} {}", Foo, Bar);
}
// { dg-error ".E0277." "" { target *-*-* } .-3 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
// { dg-error ".E0277." "" { target *-*-* } .-4 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }

