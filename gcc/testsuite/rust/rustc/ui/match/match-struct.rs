struct S { a: isize }
enum E { C(isize) }

fn main() {
    match (S { a: 1 }) {
        E::C(_) => (),
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
        _ => ()
    }
}

