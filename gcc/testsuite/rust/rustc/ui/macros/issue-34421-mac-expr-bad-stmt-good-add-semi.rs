macro_rules! make_item {
    ($a:ident) => {
        struct $a;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}

fn a() {
    make_item!(A)
}
fn b() {
    make_item!(B)
}

fn main() {}

