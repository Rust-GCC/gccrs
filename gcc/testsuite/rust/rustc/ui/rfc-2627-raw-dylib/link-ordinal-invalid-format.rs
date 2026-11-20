#![feature(raw_dylib)]
// { dg-warning "" "" { target *-*-* } .-1 }

#[link(name="foo")]
extern {
    #[link_ordinal("JustMonika")]
// { dg-error "" "" { target *-*-* } .-1 }
    fn foo();
}

fn main() {}

