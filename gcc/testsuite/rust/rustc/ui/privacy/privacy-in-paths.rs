mod foo {
    pub use self::bar::S;
    mod bar {
        pub struct S;
        pub use baz;
    }

    trait T {
        type Assoc;
    }
    impl T for () {
        type Assoc = S;
    }
}

impl foo::S {
    fn f() {}
}

pub mod baz {
    fn f() {}

    fn g() {
        ::foo::bar::baz::f(); // { dg-error ".E0603." "" { target *-*-* } }
        ::foo::bar::S::f(); // { dg-error ".E0603." "" { target *-*-* } }
        <() as ::foo::T>::Assoc::f(); // { dg-error ".E0603." "" { target *-*-* } }
    }
}

fn main() {}

