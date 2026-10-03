struct Foo (
    fn([u8; |x: u8| {}]), // { dg-error ".E0308." "" { target *-*-* } }
);

fn main() {}

