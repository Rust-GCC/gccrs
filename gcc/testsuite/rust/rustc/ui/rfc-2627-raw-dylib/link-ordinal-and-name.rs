#![feature(raw_dylib)]
// { dg-warning "" "" { target *-*-* } .-1 }

#[link(name="foo")]
extern {
    #[link_name="foo"]
    #[link_ordinal(42)]
// { dg-error "" "" { target *-*-* } .-1 }
    fn foo();
}

fn main() {}

