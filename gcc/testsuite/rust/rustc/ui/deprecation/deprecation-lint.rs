// aux-build:deprecation-lint.rs
// ignore-tidy-linelength

#![deny(deprecated)]
#![allow(warnings)]

#[macro_use]
extern crate deprecation_lint;

mod cross_crate {
    use deprecation_lint::*;

    fn test() {
        type Foo = MethodTester;
        let foo = MethodTester;

        deprecated(); // { dg-error "" "" { target *-*-* } }
        foo.method_deprecated(); // { dg-error "" "" { target *-*-* } }
        Foo::method_deprecated(&foo); // { dg-error "" "" { target *-*-* } }
        <Foo>::method_deprecated(&foo); // { dg-error "" "" { target *-*-* } }
        foo.trait_deprecated(); // { dg-error "" "" { target *-*-* } }
        Trait::trait_deprecated(&foo); // { dg-error "" "" { target *-*-* } }
        <Foo>::trait_deprecated(&foo); // { dg-error "" "" { target *-*-* } }
        <Foo as Trait>::trait_deprecated(&foo); // { dg-error "" "" { target *-*-* } }

        deprecated_text(); // { dg-error "" "" { target *-*-* } }
        foo.method_deprecated_text(); // { dg-error "" "" { target *-*-* } }
        Foo::method_deprecated_text(&foo); // { dg-error "" "" { target *-*-* } }
        <Foo>::method_deprecated_text(&foo); // { dg-error "" "" { target *-*-* } }
        foo.trait_deprecated_text(); // { dg-error "" "" { target *-*-* } }
        Trait::trait_deprecated_text(&foo); // { dg-error "" "" { target *-*-* } }
        <Foo>::trait_deprecated_text(&foo); // { dg-error "" "" { target *-*-* } }
        <Foo as Trait>::trait_deprecated_text(&foo); // { dg-error "" "" { target *-*-* } }

        let _ = DeprecatedStruct { // { dg-error "" "" { target *-*-* } }
            i: 0 // { dg-error "" "" { target *-*-* } }
        };

        let _ = DeprecatedUnitStruct; // { dg-error "" "" { target *-*-* } }

        let _ = Enum::DeprecatedVariant; // { dg-error "" "" { target *-*-* } }

        let _ = DeprecatedTupleStruct (1); // { dg-error "" "" { target *-*-* } }

        let _ = nested::DeprecatedStruct { // { dg-error "" "" { target *-*-* } }
            i: 0 // { dg-error "" "" { target *-*-* } }
        };

        let _ = nested::DeprecatedUnitStruct; // { dg-error "" "" { target *-*-* } }

        let _ = nested::Enum::DeprecatedVariant; // { dg-error "" "" { target *-*-* } }

        let _ = nested::DeprecatedTupleStruct (1); // { dg-error "" "" { target *-*-* } }

        // At the moment, the lint checker only checks stability in
        // in the arguments of macros.
        // Eventually, we will want to lint the contents of the
        // macro in the module *defining* it. Also, stability levels
        // on macros themselves are not yet linted.
        macro_test_arg!(deprecated_text()); // { dg-error "" "" { target *-*-* } }
        macro_test_arg!(macro_test_arg!(deprecated_text())); // { dg-error "" "" { target *-*-* } }
    }

    fn test_method_param<Foo: Trait>(foo: Foo) {
        foo.trait_deprecated(); // { dg-error "" "" { target *-*-* } }
        Trait::trait_deprecated(&foo); // { dg-error "" "" { target *-*-* } }
        <Foo>::trait_deprecated(&foo); // { dg-error "" "" { target *-*-* } }
        <Foo as Trait>::trait_deprecated(&foo); // { dg-error "" "" { target *-*-* } }
        foo.trait_deprecated_text(); // { dg-error "" "" { target *-*-* } }
        Trait::trait_deprecated_text(&foo); // { dg-error "" "" { target *-*-* } }
        <Foo>::trait_deprecated_text(&foo); // { dg-error "" "" { target *-*-* } }
        <Foo as Trait>::trait_deprecated_text(&foo); // { dg-error "" "" { target *-*-* } }
    }

    fn test_method_object(foo: &Trait) {
        foo.trait_deprecated(); // { dg-error "" "" { target *-*-* } }
        foo.trait_deprecated_text(); // { dg-error "" "" { target *-*-* } }
    }

    struct S;

    impl DeprecatedTrait for S {} // { dg-error "" "" { target *-*-* } }
    trait LocalTrait : DeprecatedTrait { } // { dg-error "" "" { target *-*-* } }

    pub fn foo() {
        let x = Stable {
            override2: 3,
// { dg-error "" "" { target *-*-* } .-1 }
        };

        let _ = x.override2;
// { dg-error "" "" { target *-*-* } .-1 }

        let Stable {
            override2: _
// { dg-error "" "" { target *-*-* } .-1 }
        } = x;
        // all fine
        let Stable { .. } = x;

        let x = Stable2(1, 2, 3);

        let _ = x.2;
// { dg-error "" "" { target *-*-* } .-1 }

        let Stable2(_,
                   _,
                   _)
// { dg-error "" "" { target *-*-* } .-1 }
            = x;
        // all fine
        let Stable2(..) = x;

        let x = Deprecated {
// { dg-error "" "" { target *-*-* } .-1 }
            inherit: 1,
// { dg-error "" "" { target *-*-* } .-1 }
        };

        let _ = x.inherit;
// { dg-error "" "" { target *-*-* } .-1 }

        let Deprecated {
// { dg-error "" "" { target *-*-* } .-1 }
            inherit: _,
// { dg-error "" "" { target *-*-* } .-1 }
        } = x;

        let Deprecated
// { dg-error "" "" { target *-*-* } .-1 }
            { .. } = x;

        let x = Deprecated2(1, 2, 3);
// { dg-error "" "" { target *-*-* } .-1 }

        let _ = x.0;
// { dg-error "" "" { target *-*-* } .-1 }
        let _ = x.1;
// { dg-error "" "" { target *-*-* } .-1 }
        let _ = x.2;
// { dg-error "" "" { target *-*-* } .-1 }

        let Deprecated2
// { dg-error "" "" { target *-*-* } .-1 }
            (_,
// { dg-error "" "" { target *-*-* } .-1 }
             _,
// { dg-error "" "" { target *-*-* } .-1 }
             _)
// { dg-error "" "" { target *-*-* } .-1 }
            = x;
        let Deprecated2
// { dg-error "" "" { target *-*-* } .-1 }
            // the patterns are all fine:
            (..) = x;
    }
}

mod inheritance {
    use deprecation_lint::*;

    fn test_inheritance() {
        deprecated_mod::deprecated(); // { dg-error "" "" { target *-*-* } }
    }
}

mod this_crate {
    #[deprecated(since = "1.0.0", note = "text")]
    pub fn deprecated() {}
    #[deprecated(since = "1.0.0", note = "text")]
    pub fn deprecated_text() {}

    #[deprecated(since = "99.99.99", note = "text")]
    pub fn deprecated_future() {}
    #[deprecated(since = "99.99.99", note = "text")]
    pub fn deprecated_future_text() {}

    pub struct MethodTester;

    impl MethodTester {
        #[deprecated(since = "1.0.0", note = "text")]
        pub fn method_deprecated(&self) {}
        #[deprecated(since = "1.0.0", note = "text")]
        pub fn method_deprecated_text(&self) {}
    }

    pub trait Trait {
        #[deprecated(since = "1.0.0", note = "text")]
        fn trait_deprecated(&self) {}
        #[deprecated(since = "1.0.0", note = "text")]
        fn trait_deprecated_text(&self) {}
    }

    impl Trait for MethodTester {}

    #[deprecated(since = "1.0.0", note = "text")]
    pub struct DeprecatedStruct {
        i: isize
    }
    pub struct UnstableStruct {
        i: isize
    }
    pub struct StableStruct {
        i: isize
    }

    #[deprecated(since = "1.0.0", note = "text")]
    pub struct DeprecatedUnitStruct;

    pub enum Enum {
        #[deprecated(since = "1.0.0", note = "text")]
        DeprecatedVariant,
    }

    #[deprecated(since = "1.0.0", note = "text")]
    pub struct DeprecatedTupleStruct(isize);

    mod nested {
        #[deprecated(since = "1.0.0", note = "text")]
        pub struct DeprecatedStruct {
            i: isize
        }

        #[deprecated(since = "1.0.0", note = "text")]
        pub struct DeprecatedUnitStruct;

        pub enum Enum {
            #[deprecated(since = "1.0.0", note = "text")]
            DeprecatedVariant,
        }

        #[deprecated(since = "1.0.0", note = "text")]
        pub struct DeprecatedTupleStruct(pub isize);
    }

    fn test() {
        use self::nested;

        // Only the deprecated cases of the following should generate
        // errors, because other stability attributes now have meaning
        // only *across* crates, not within a single crate.

        type Foo = MethodTester;
        let foo = MethodTester;

        deprecated(); // { dg-error "" "" { target *-*-* } }
        foo.method_deprecated(); // { dg-error "" "" { target *-*-* } }
        Foo::method_deprecated(&foo); // { dg-error "" "" { target *-*-* } }
        <Foo>::method_deprecated(&foo); // { dg-error "" "" { target *-*-* } }
        foo.trait_deprecated(); // { dg-error "" "" { target *-*-* } }
        Trait::trait_deprecated(&foo); // { dg-error "" "" { target *-*-* } }
        <Foo>::trait_deprecated(&foo); // { dg-error "" "" { target *-*-* } }
        <Foo as Trait>::trait_deprecated(&foo); // { dg-error "" "" { target *-*-* } }

        deprecated_text(); // { dg-error "" "" { target *-*-* } }
        foo.method_deprecated_text(); // { dg-error "" "" { target *-*-* } }
        Foo::method_deprecated_text(&foo); // { dg-error "" "" { target *-*-* } }
        <Foo>::method_deprecated_text(&foo); // { dg-error "" "" { target *-*-* } }
        foo.trait_deprecated_text(); // { dg-error "" "" { target *-*-* } }
        Trait::trait_deprecated_text(&foo); // { dg-error "" "" { target *-*-* } }
        <Foo>::trait_deprecated_text(&foo); // { dg-error "" "" { target *-*-* } }
        <Foo as Trait>::trait_deprecated_text(&foo); // { dg-error "" "" { target *-*-* } }

        // Future deprecations are only permitted for rustc_deprecated.
        deprecated_future(); // { dg-error "" "" { target *-*-* } }
        deprecated_future_text(); // { dg-error "" "" { target *-*-* } }

        let _ = DeprecatedStruct {
// { dg-error "" "" { target *-*-* } .-1 }
            i: 0 // { dg-error "" "" { target *-*-* } }
        };

        let _ = DeprecatedUnitStruct; // { dg-error "" "" { target *-*-* } }

        let _ = Enum::DeprecatedVariant; // { dg-error "" "" { target *-*-* } }

        let _ = DeprecatedTupleStruct (1); // { dg-error "" "" { target *-*-* } }

        let _ = nested::DeprecatedStruct {
// { dg-error "" "" { target *-*-* } .-1 }
            i: 0 // { dg-error "" "" { target *-*-* } }
        };

        let _ = nested::DeprecatedUnitStruct; // { dg-error "" "" { target *-*-* } }

        let _ = nested::Enum::DeprecatedVariant; // { dg-error "" "" { target *-*-* } }

        let _ = nested::DeprecatedTupleStruct (1); // { dg-error "" "" { target *-*-* } }
    }

    fn test_method_param<Foo: Trait>(foo: Foo) {
        foo.trait_deprecated(); // { dg-error "" "" { target *-*-* } }
        Trait::trait_deprecated(&foo); // { dg-error "" "" { target *-*-* } }
        <Foo>::trait_deprecated(&foo); // { dg-error "" "" { target *-*-* } }
        <Foo as Trait>::trait_deprecated(&foo); // { dg-error "" "" { target *-*-* } }
        foo.trait_deprecated_text(); // { dg-error "" "" { target *-*-* } }
        Trait::trait_deprecated_text(&foo); // { dg-error "" "" { target *-*-* } }
        <Foo>::trait_deprecated_text(&foo); // { dg-error "" "" { target *-*-* } }
        <Foo as Trait>::trait_deprecated_text(&foo); // { dg-error "" "" { target *-*-* } }
    }

    fn test_method_object(foo: &Trait) {
        foo.trait_deprecated(); // { dg-error "" "" { target *-*-* } }
        foo.trait_deprecated_text(); // { dg-error "" "" { target *-*-* } }
    }

    #[deprecated(since = "1.0.0", note = "text")]
    fn test_fn_body() {
        fn fn_in_body() {}
        fn_in_body();
    }

    fn test_fn_closure_body() {
        let _ = || {
            #[deprecated]
            fn bar() { }
            bar(); // { dg-error "" "" { target *-*-* } }
        };
    }

    impl MethodTester {
        #[deprecated(since = "1.0.0", note = "text")]
        fn test_method_body(&self) {
            fn fn_in_body() {}
            fn_in_body();
        }
    }

    #[deprecated(since = "1.0.0", note = "text")]
    pub trait DeprecatedTrait {
        fn dummy(&self) { }
    }

    struct S;

    impl DeprecatedTrait for S { } // { dg-error "" "" { target *-*-* } }

    trait LocalTrait : DeprecatedTrait { } // { dg-error "" "" { target *-*-* } }
}

mod this_crate2 {
    struct Stable {
        #[deprecated(since = "1.0.0", note = "text")]
        override2: u8,
    }

    struct Stable2(u8,
                   u8,
                   #[deprecated(since = "1.0.0", note = "text")] u8);

    #[deprecated(since = "1.0.0", note = "text")]
    struct Deprecated {
        inherit: u8,
    }

    #[deprecated(since = "1.0.0", note = "text")]
    struct Deprecated2(u8,
                       u8,
                       u8);

    pub fn foo() {
        let x = Stable {
            override2: 3,
// { dg-error "" "" { target *-*-* } .-1 }
        };

        let _ = x.override2;
// { dg-error "" "" { target *-*-* } .-1 }

        let Stable {
            override2: _
// { dg-error "" "" { target *-*-* } .-1 }
        } = x;
        // all fine
        let Stable { .. } = x;

        let x = Stable2(1, 2, 3);

        let _ = x.2;
// { dg-error "" "" { target *-*-* } .-1 }

        let Stable2(_,
                   _,
                   _)
// { dg-error "" "" { target *-*-* } .-1 }
            = x;
        // all fine
        let Stable2(..) = x;

        let x = Deprecated {
// { dg-error "" "" { target *-*-* } .-1 }
            inherit: 1,
// { dg-error "" "" { target *-*-* } .-1 }
        };

        let _ = x.inherit;
// { dg-error "" "" { target *-*-* } .-1 }

        let Deprecated {
// { dg-error "" "" { target *-*-* } .-1 }
            inherit: _,
// { dg-error "" "" { target *-*-* } .-1 }
        } = x;

        let Deprecated
// { dg-error "" "" { target *-*-* } .-1 }
            // the patterns are all fine:
            { .. } = x;

        let x = Deprecated2(1, 2, 3);
// { dg-error "" "" { target *-*-* } .-1 }

        let _ = x.0;
// { dg-error "" "" { target *-*-* } .-1 }
        let _ = x.1;
// { dg-error "" "" { target *-*-* } .-1 }
        let _ = x.2;
// { dg-error "" "" { target *-*-* } .-1 }

        let Deprecated2
// { dg-error "" "" { target *-*-* } .-1 }
            (_,
// { dg-error "" "" { target *-*-* } .-1 }
             _,
// { dg-error "" "" { target *-*-* } .-1 }
             _)
// { dg-error "" "" { target *-*-* } .-1 }
            = x;
        let Deprecated2
// { dg-error "" "" { target *-*-* } .-1 }
            // the patterns are all fine:
            (..) = x;
    }
}

fn main() {}

