#[derive(Copy, Clone, Debug, Hash, PartialEq, Eq)]
enum E {
    V { field: bool },
    I { field1: bool, field2: usize },
    J { field: isize },
    K { field: &'static str},
}
fn test_E(x: E) {
    let field = true;
    if x == E::V { field } {}
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
    if x == E::I { field1: true, field2: 42 } {}
// { dg-error "" "" { target *-*-* } .-1 }
    if x == E::V { field: false } {}
// { dg-error "" "" { target *-*-* } .-1 }
    if x == E::J { field: -42 } {}
// { dg-error "" "" { target *-*-* } .-1 }
    if x == E::K { field: "" } {}
// { dg-error "" "" { target *-*-* } .-1 }
    let y: usize = ();
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

fn main() {}

