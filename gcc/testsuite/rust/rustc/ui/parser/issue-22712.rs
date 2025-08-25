struct Foo<B> {
    buffer: B
}

fn bar() {
    let Foo<Vec<u8>>  // { dg-error "" "" { target *-*-* } }
}

fn main() {}

