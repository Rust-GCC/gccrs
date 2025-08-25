#![feature(raw_dylib)]
// { dg-warning "" "" { target *-*-* } .-1 }

#[link(name="foo")]
extern {
    #[link_ordinal(18446744073709551616)]
// { dg-error "" "" { target *-*-* } .-1 }
    fn foo();
}

fn main() {}

