// Test that immutable pattern bindings cannot be reassigned.

enum E {
    Foo(isize)
}

struct S {
    bar: isize,
}

pub fn main() {
    match 1 {
        x => {
            x += 1; // { dg-error ".E0384." "" { target *-*-* } }
        }
    }

    match E::Foo(1) {
        E::Foo(x) => {
            x += 1; // { dg-error ".E0384." "" { target *-*-* } }
        }
    }

    match (S { bar: 1 }) {
        S { bar: x } => {
            x += 1; // { dg-error ".E0384." "" { target *-*-* } }
        }
    }

    match (1,) {
        (x,) => {
            x += 1; // { dg-error ".E0384." "" { target *-*-* } }
        }
    }

    match [1,2,3] {
        [x,_,_] => {
            x += 1; // { dg-error ".E0384." "" { target *-*-* } }
        }
    }
}

