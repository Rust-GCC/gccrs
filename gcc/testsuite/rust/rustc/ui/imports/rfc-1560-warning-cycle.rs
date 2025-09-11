pub struct Foo;

mod bar {
    struct Foo;

    mod baz {
        use *;
        use bar::*;
        fn f(_: Foo) {} // { dg-error ".E0659." "" { target *-*-* } }
    }
}

fn main() {}

