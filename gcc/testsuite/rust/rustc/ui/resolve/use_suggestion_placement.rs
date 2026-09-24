// ignore-cloudabi no std::path support

macro_rules! y {
    () => {}
}

mod m {
    pub const A: i32 = 0;
}

mod foo {
    #[derive(Debug)]
    pub struct Foo;

    // test whether the use suggestion isn't
    // placed into the expansion of `#[derive(Debug)]
    type Bar = Path; // { dg-error ".E0412." "" { target *-*-* } }
}

fn main() {
    y!();
    let _ = A; // { dg-error ".E0425." "" { target *-*-* } }
    foo();
}

fn foo() {
    type Dict<K, V> = HashMap<K, V>; // { dg-error ".E0412." "" { target *-*-* } }
}

