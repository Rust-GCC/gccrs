pub mod Mod {
    pub struct Foo {}
    impl Foo {
        pub const BAR: usize = 42;
    }
}

fn foo(_: usize) {}

fn main() {
    foo(Mod::Foo.Bar);
// { dg-error ".E0423." "" { target *-*-* } .-1 }
}

