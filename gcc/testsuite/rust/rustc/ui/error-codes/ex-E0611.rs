mod a {
    pub struct Foo(u32);

    impl Foo {
        pub fn new() -> Foo { Foo(0) }
    }
}

fn main() {
   let y = a::Foo::new();
   y.0; // { dg-error ".E0616." "" { target *-*-* } }
}

