// aux-build:namespaced_enums.rs
extern crate namespaced_enums;

fn main() {
    let _ = namespaced_enums::A;
// { dg-error ".E0425." "" { target *-*-* } .-1 }
    let _ = namespaced_enums::B(10);
// { dg-error ".E0425." "" { target *-*-* } .-1 }
    let _ = namespaced_enums::C { a: 10 };
// { dg-error ".E0422." "" { target *-*-* } .-1 }
}

