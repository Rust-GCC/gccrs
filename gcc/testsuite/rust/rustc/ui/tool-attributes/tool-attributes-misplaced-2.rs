#[derive(rustfmt::skip)] // { dg-error "" "" { target *-*-* } }
struct S;

fn main() {
    rustfmt::skip!(); // { dg-error "" "" { target *-*-* } }
}

