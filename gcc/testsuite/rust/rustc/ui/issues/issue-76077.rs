pub mod foo {
    pub struct Foo {
        you_cant_use_this_field: bool,
    }
}

fn main() {
    foo::Foo {};
// { dg-error "" "" { target *-*-* } .-1 }
}

