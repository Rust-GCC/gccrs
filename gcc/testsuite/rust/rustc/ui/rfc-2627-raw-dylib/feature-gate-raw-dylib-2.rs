#[link(name="foo")]
extern {
    #[link_ordinal(42)]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    fn foo();
}

fn main() {}

