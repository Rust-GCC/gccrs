// Test that the parser does not attempt to parse struct literals
// within assignments in if expressions.

#![allow(unused_parens)]

struct Foo {
    foo: usize
}

fn main() {
    let x = 1;
    let y: Foo;

    // `x { ... }` should not be interpreted as a struct literal here
    if x = x {
// { dg-error ".E0308." "" { target *-*-* } .-1 }
        println!("{}", x);
    }
    // Explicit parentheses on the left should match behavior of above
    if (x = x) {
// { dg-error ".E0308." "" { target *-*-* } .-1 }
        println!("{}", x);
    }
    // The struct literal interpretation is fine with explicit parentheses on the right
    if y = (Foo { foo: x }) {
// { dg-error ".E0308." "" { target *-*-* } .-1 }
        println!("{}", x);
    }
    // "invalid left-hand side of assignment" error is suppresed
    if 3 = x {
// { dg-error ".E0308." "" { target *-*-* } .-1 }
        println!("{}", x);
    }
    if (
        if true {
            x = 4 // { dg-error ".E0308." "" { target *-*-* } }
        } else {
            x = 5 // { dg-error ".E0308." "" { target *-*-* } }
        }
    ) {
        println!("{}", x);
    }
}

