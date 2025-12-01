#![deny(deprecated)]
#![allow(warnings)]

#[deprecated]
fn issue_35128() {
    format_args!("foo");
}

#[deprecated]
fn issue_35128_minimal() {
    static FOO: &'static str = "foo";
    let _ = FOO;
}

#[deprecated]
mod silent {
    type DeprecatedType = u8;
    struct DeprecatedStruct;
    fn deprecated_fn() {}
    trait DeprecatedTrait {}
    static DEPRECATED_STATIC: u8 = 0;
    const DEPRECATED_CONST: u8 = 1;

    struct Foo(DeprecatedType);

    impl DeprecatedTrait for Foo {}

    impl Foo {
        fn bar<T: DeprecatedTrait>() {
            deprecated_fn();
        }
    }

    fn foo() -> u8 {
        DEPRECATED_STATIC +
        DEPRECATED_CONST
    }
}

#[deprecated]
mod loud {
    #[deprecated]
    type DeprecatedType = u8;
    #[deprecated]
    struct DeprecatedStruct;
    #[deprecated]
    fn deprecated_fn() {}
    #[deprecated]
    trait DeprecatedTrait {}
    #[deprecated]
    static DEPRECATED_STATIC: u8 = 0;
    #[deprecated]
    const DEPRECATED_CONST: u8 = 1;

    struct Foo(DeprecatedType); // { dg-error "" "" { target *-*-* } }

    impl DeprecatedTrait for Foo {} // { dg-error "" "" { target *-*-* } }

    impl Foo {
        fn bar<T: DeprecatedTrait>() { // { dg-error "" "" { target *-*-* } }
            deprecated_fn(); // { dg-error "" "" { target *-*-* } }
        }
    }

    fn foo() -> u8 {
        DEPRECATED_STATIC + // { dg-error "" "" { target *-*-* } }
        DEPRECATED_CONST // { dg-error "" "" { target *-*-* } }
    }
}

fn main() {}

