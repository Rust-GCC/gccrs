#![feature(lang_items, start, no_core)]
#![no_core] // makes debugging this test *a lot* easier (during resolve)

#[lang="sized"]
pub trait Sized {}

#[lang="copy"]
pub trait Copy {}

#[lang="deref"]
pub trait Deref {
    type Target;
}

#[lang="receiver"]
pub trait Receiver: Deref {}

impl<'a, T> Deref for &'a T {
    type Target = T;
}

impl<'a, T> Receiver for &'a T {}

mod bar {
    // shouldn't bring in too much
    pub use self::glob::*;

    // can't publicly re-export private items
    pub use self::baz::{foo, bar};

    pub struct A;
    impl A {
        pub fn foo() {}
        fn bar() {}

        pub fn foo2(&self) {}
        fn bar2(&self) {}
    }

    trait B {
        fn foo() -> Self;
    }

    impl B for isize { fn foo() -> isize { 3 } }

    pub enum Enum {
        Pub
    }

    mod baz {
        pub struct A;
        impl A {
            pub fn foo() {}
            fn bar() {}

            pub fn foo2(&self) {}
            fn bar2(&self) {}
        }

        pub fn foo() {}
        pub fn bar() {}
    }

    extern {
        fn epriv();
        pub fn epub();
    }

    fn test() {
        self::Enum::Pub;
        unsafe {
            epriv();
            epub();
        }
        self::baz::A;
        self::baz::A::foo();
        self::baz::A::bar(); // { dg-error ".E0624." "" { target *-*-* } }
        self::baz::A.foo2();

        // this used to cause an ICE in privacy traversal.
        super::gpub();
    }

    mod glob {
        pub fn gpub() {}
        fn gpriv() {}
    }
}

pub fn gpub() {}

fn lol() {
    bar::A;
    bar::A::foo();
    bar::A::bar(); // { dg-error ".E0624." "" { target *-*-* } }
    bar::A.foo2();
}

mod foo {
    fn test() {
        ::bar::A::foo();
        ::bar::A::bar();        // { dg-error ".E0624." "" { target *-*-* } }
        ::bar::A.foo2();
        ::bar::baz::A::foo();   // { dg-error ".E0603." "" { target *-*-* } }
        ::bar::baz::A::bar();   // { dg-error ".E0624." "" { target *-*-* } }
// { dg-error ".E0624." "" { target *-*-* } .-1 }
        ::bar::baz::A.foo2();   // { dg-error ".E0603." "" { target *-*-* } }
        ::bar::baz::A.bar2();   // { dg-error ".E0624." "" { target *-*-* } }
// { dg-error ".E0624." "" { target *-*-* } .-1 }

        let _: isize =
        ::bar::B::foo();        // { dg-error ".E0603." "" { target *-*-* } }
        ::lol();

        ::bar::Enum::Pub;

        unsafe {
            ::bar::epriv(); // { dg-error ".E0603." "" { target *-*-* } }
            ::bar::epub();
        }

        ::bar::foo();
        ::bar::bar();

        ::bar::gpub();

        ::bar::baz::foo(); // { dg-error ".E0603." "" { target *-*-* } }
        ::bar::baz::bar(); // { dg-error ".E0603." "" { target *-*-* } }
    }

    fn test2() {
        use bar::baz::{foo, bar};
// { dg-error ".E0603." "" { target *-*-* } .-1 }
// { dg-error ".E0603." "" { target *-*-* } .-2 }

        foo();
        bar();
    }

    fn test3() {
        use bar::baz;
// { dg-error ".E0603." "" { target *-*-* } .-1 }
    }

    fn test4() {
        use bar::{foo, bar};
        foo();
        bar();
    }

    fn test5() {
        use bar;
        bar::foo();
        bar::bar();
    }

    impl ::bar::B for f32 { fn foo() -> f32 { 1.0 } }
// { dg-error ".E0603." "" { target *-*-* } .-1 }
}

pub mod mytest {
    // Even though the inner `A` struct is a publicly exported item (usable from
    // external crates through `foo::foo`, it should not be accessible through
    // its definition path (which has the private `i` module).
    use self::foo::i::A; // { dg-error ".E0603." "" { target *-*-* } }

    pub mod foo {
        pub use self::i::A as foo;

        mod i {
            pub struct A;
        }
    }
}

#[start] fn main(_: isize, _: *const *const u8) -> isize { 3 }

