// Test that certain pattern-match type errors are non-fatal

enum A {
    B(isize, isize),
    C(isize, isize, isize),
    D
}

struct S {
    a: isize
}

fn f(_c: char) {}

fn main() {
    match A::B(1, 2) {
        A::B(_, _, _) => (), // { dg-error ".E0023." "" { target *-*-* } }
        A::D(_) => (), // { dg-error ".E0532." "" { target *-*-* } }
        _ => ()
    }
    match 'c' {
        S { .. } => (),
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }

        _ => ()
    }
    f(true);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }

    match () {
        E::V => {} // { dg-error ".E0433." "" { target *-*-* } }
    }
}

