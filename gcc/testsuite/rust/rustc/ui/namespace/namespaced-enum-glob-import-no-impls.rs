mod m2 {
    pub enum Foo {
        A,
        B(isize),
        C { a: isize },
    }

    impl Foo {
        pub fn foo() {}
        pub fn bar(&self) {}
    }
}

mod m {
    pub use m2::Foo::*;
}

pub fn main() {
    use m2::Foo::*;

    foo(); // { dg-error ".E0425." "" { target *-*-* } }
    m::foo(); // { dg-error ".E0425." "" { target *-*-* } }
    bar(); // { dg-error ".E0425." "" { target *-*-* } }
    m::bar(); // { dg-error ".E0425." "" { target *-*-* } }
}

