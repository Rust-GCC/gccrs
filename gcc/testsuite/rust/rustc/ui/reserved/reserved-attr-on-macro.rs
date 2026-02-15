#[rustc_attribute_should_be_reserved]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

macro_rules! foo {
    () => (());
}

fn main() {
    foo!(); // { dg-error "" "" { target *-*-* } }
}

