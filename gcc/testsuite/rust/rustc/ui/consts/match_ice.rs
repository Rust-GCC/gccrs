// https://github.com/rust-lang/rust/issues/53708

struct S;

#[derive(PartialEq, Eq)]
struct T;

fn main() {
    const C: &S = &S;
    match C {
        C => {}
// { dg-error "" "" { target *-*-* } .-1 }
    }
    const K: &T = &T;
    match K {
        K => {}
    }
}

