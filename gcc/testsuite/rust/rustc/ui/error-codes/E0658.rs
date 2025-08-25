#[repr(u128)]
enum Foo { // { dg-error ".E0658." "" { target *-*-* } }
    Bar(u64),
}

fn main() {}

