// check-pass
// aux-build:lint_stability.rs
// aux-build:inherited_stability.rs
// aux-build:stability_cfg1.rs
// aux-build:stability-cfg2.rs
// ignore-tidy-linelength
#![warn(deprecated)]
#![feature(staged_api, unstable_test_feature)]

#![stable(feature = "rust1", since = "1.0.0")]

#[macro_use]
extern crate lint_stability;

mod cross_crate {
    extern crate stability_cfg1;
    extern crate stability_cfg2;

    use lint_stability::*;

    fn test() {
        type Foo = MethodTester;
        let foo = MethodTester;

        deprecated(); // { dg-warning "" "" { target *-*-* } }
        foo.method_deprecated(); // { dg-warning "" "" { target *-*-* } }
        Foo::method_deprecated(&foo); // { dg-warning "" "" { target *-*-* } }
        <Foo>::method_deprecated(&foo); // { dg-warning "" "" { target *-*-* } }
        foo.trait_deprecated(); // { dg-warning "" "" { target *-*-* } }
        Trait::trait_deprecated(&foo); // { dg-warning "" "" { target *-*-* } }
        <Foo>::trait_deprecated(&foo); // { dg-warning "" "" { target *-*-* } }
        <Foo as Trait>::trait_deprecated(&foo); // { dg-warning "" "" { target *-*-* } }

        deprecated_text(); // { dg-warning "" "" { target *-*-* } }
        foo.method_deprecated_text(); // { dg-warning "" "" { target *-*-* } }
        Foo::method_deprecated_text(&foo); // { dg-warning "" "" { target *-*-* } }
        <Foo>::method_deprecated_text(&foo); // { dg-warning "" "" { target *-*-* } }
        foo.trait_deprecated_text(); // { dg-warning "" "" { target *-*-* } }
        Trait::trait_deprecated_text(&foo); // { dg-warning "" "" { target *-*-* } }
        <Foo>::trait_deprecated_text(&foo); // { dg-warning "" "" { target *-*-* } }
        <Foo as Trait>::trait_deprecated_text(&foo); // { dg-warning "" "" { target *-*-* } }

        deprecated_unstable(); // { dg-warning "" "" { target *-*-* } }
        foo.method_deprecated_unstable(); // { dg-warning "" "" { target *-*-* } }
        Foo::method_deprecated_unstable(&foo); // { dg-warning "" "" { target *-*-* } }
        <Foo>::method_deprecated_unstable(&foo); // { dg-warning "" "" { target *-*-* } }
        foo.trait_deprecated_unstable(); // { dg-warning "" "" { target *-*-* } }
        Trait::trait_deprecated_unstable(&foo); // { dg-warning "" "" { target *-*-* } }
        <Foo>::trait_deprecated_unstable(&foo); // { dg-warning "" "" { target *-*-* } }
        <Foo as Trait>::trait_deprecated_unstable(&foo); // { dg-warning "" "" { target *-*-* } }

        deprecated_unstable_text(); // { dg-warning "" "" { target *-*-* } }
        foo.method_deprecated_unstable_text(); // { dg-warning "" "" { target *-*-* } }
        Foo::method_deprecated_unstable_text(&foo); // { dg-warning "" "" { target *-*-* } }
        <Foo>::method_deprecated_unstable_text(&foo); // { dg-warning "" "" { target *-*-* } }
        foo.trait_deprecated_unstable_text(); // { dg-warning "" "" { target *-*-* } }
        Trait::trait_deprecated_unstable_text(&foo); // { dg-warning "" "" { target *-*-* } }
        <Foo>::trait_deprecated_unstable_text(&foo); // { dg-warning "" "" { target *-*-* } }
        <Foo as Trait>::trait_deprecated_unstable_text(&foo); // { dg-warning "" "" { target *-*-* } }

        unstable();
        foo.method_unstable();
        Foo::method_unstable(&foo);
        <Foo>::method_unstable(&foo);
        foo.trait_unstable();
        Trait::trait_unstable(&foo);
        <Foo>::trait_unstable(&foo);
        <Foo as Trait>::trait_unstable(&foo);

        unstable_text();
        foo.method_unstable_text();
        Foo::method_unstable_text(&foo);
        <Foo>::method_unstable_text(&foo);
        foo.trait_unstable_text();
        Trait::trait_unstable_text(&foo);
        <Foo>::trait_unstable_text(&foo);
        <Foo as Trait>::trait_unstable_text(&foo);

        stable();
        foo.method_stable();
        Foo::method_stable(&foo);
        <Foo>::method_stable(&foo);
        foo.trait_stable();
        Trait::trait_stable(&foo);
        <Foo>::trait_stable(&foo);
        <Foo as Trait>::trait_stable(&foo);

        stable_text();
        foo.method_stable_text();
        Foo::method_stable_text(&foo);
        <Foo>::method_stable_text(&foo);
        foo.trait_stable_text();
        Trait::trait_stable_text(&foo);
        <Foo>::trait_stable_text(&foo);
        <Foo as Trait>::trait_stable_text(&foo);

        struct S1<T: TraitWithAssociatedTypes>(T::TypeUnstable);
        struct S2<T: TraitWithAssociatedTypes>(T::TypeDeprecated);
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
        type A = dyn TraitWithAssociatedTypes<
            TypeUnstable = u8,
            TypeDeprecated = u16,
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-warning "" "" { target *-*-* } .-3 }
        >;

        let _ = DeprecatedStruct { // { dg-warning "" "" { target *-*-* } }
            i: 0 // { dg-warning "" "" { target *-*-* } }
        };
        let _ = DeprecatedUnstableStruct {
// { dg-warning "" "" { target *-*-* } .-1 }
            i: 0 // { dg-warning "" "" { target *-*-* } }
        };
        let _ = UnstableStruct { i: 0 };
        let _ = StableStruct { i: 0 };

        let _ = DeprecatedUnitStruct; // { dg-warning "" "" { target *-*-* } }
        let _ = DeprecatedUnstableUnitStruct; // { dg-warning "" "" { target *-*-* } }
        let _ = UnstableUnitStruct;
        let _ = StableUnitStruct;

        let _ = Enum::DeprecatedVariant; // { dg-warning "" "" { target *-*-* } }
        let _ = Enum::DeprecatedUnstableVariant; // { dg-warning "" "" { target *-*-* } }
        let _ = Enum::UnstableVariant;
        let _ = Enum::StableVariant;

        let _ = DeprecatedTupleStruct (1); // { dg-warning "" "" { target *-*-* } }
        let _ = DeprecatedUnstableTupleStruct (1); // { dg-warning "" "" { target *-*-* } }
        let _ = UnstableTupleStruct (1);
        let _ = StableTupleStruct (1);

        // At the moment, the lint checker only checks stability in
        // in the arguments of macros.
        // Eventually, we will want to lint the contents of the
        // macro in the module *defining* it. Also, stability levels
        // on macros themselves are not yet linted.
        macro_test_arg!(deprecated_text()); // { dg-warning "" "" { target *-*-* } }
        macro_test_arg!(deprecated_unstable_text()); // { dg-warning "" "" { target *-*-* } }
        macro_test_arg!(macro_test_arg!(deprecated_text())); // { dg-warning "" "" { target *-*-* } }
    }

    fn test_method_param<Foo: Trait>(foo: Foo) {
        foo.trait_deprecated(); // { dg-warning "" "" { target *-*-* } }
        Trait::trait_deprecated(&foo); // { dg-warning "" "" { target *-*-* } }
        <Foo>::trait_deprecated(&foo); // { dg-warning "" "" { target *-*-* } }
        <Foo as Trait>::trait_deprecated(&foo); // { dg-warning "" "" { target *-*-* } }
        foo.trait_deprecated_text(); // { dg-warning "" "" { target *-*-* } }
        Trait::trait_deprecated_text(&foo); // { dg-warning "" "" { target *-*-* } }
        <Foo>::trait_deprecated_text(&foo); // { dg-warning "" "" { target *-*-* } }
        <Foo as Trait>::trait_deprecated_text(&foo); // { dg-warning "" "" { target *-*-* } }
        foo.trait_deprecated_unstable(); // { dg-warning "" "" { target *-*-* } }
        Trait::trait_deprecated_unstable(&foo); // { dg-warning "" "" { target *-*-* } }
        <Foo>::trait_deprecated_unstable(&foo); // { dg-warning "" "" { target *-*-* } }
        <Foo as Trait>::trait_deprecated_unstable(&foo); // { dg-warning "" "" { target *-*-* } }
        foo.trait_deprecated_unstable_text(); // { dg-warning "" "" { target *-*-* } }
        Trait::trait_deprecated_unstable_text(&foo); // { dg-warning "" "" { target *-*-* } }
        <Foo>::trait_deprecated_unstable_text(&foo); // { dg-warning "" "" { target *-*-* } }
        <Foo as Trait>::trait_deprecated_unstable_text(&foo); // { dg-warning "" "" { target *-*-* } }
        foo.trait_unstable();
        Trait::trait_unstable(&foo);
        <Foo>::trait_unstable(&foo);
        <Foo as Trait>::trait_unstable(&foo);
        foo.trait_unstable_text();
        Trait::trait_unstable_text(&foo);
        <Foo>::trait_unstable_text(&foo);
        <Foo as Trait>::trait_unstable_text(&foo);
        foo.trait_stable();
        Trait::trait_stable(&foo);
        <Foo>::trait_stable(&foo);
        <Foo as Trait>::trait_stable(&foo);
    }

    fn test_method_object(foo: &dyn Trait) {
        foo.trait_deprecated(); // { dg-warning "" "" { target *-*-* } }
        foo.trait_deprecated_text(); // { dg-warning "" "" { target *-*-* } }
        foo.trait_deprecated_unstable(); // { dg-warning "" "" { target *-*-* } }
        foo.trait_deprecated_unstable_text(); // { dg-warning "" "" { target *-*-* } }
        foo.trait_unstable();
        foo.trait_unstable_text();
        foo.trait_stable();
    }

    struct S;

    impl UnstableTrait for S { }
    impl DeprecatedTrait for S {} // { dg-warning "" "" { target *-*-* } }
    trait LocalTrait : UnstableTrait { }
    trait LocalTrait2 : DeprecatedTrait { } // { dg-warning "" "" { target *-*-* } }

    impl Trait for S {
        fn trait_stable(&self) {}
        fn trait_unstable(&self) {}
    }
}

mod inheritance {
    extern crate inherited_stability;
    use self::inherited_stability::*;

    fn test_inheritance() {
        unstable();
        stable();

        stable_mod::unstable();
        stable_mod::stable();

        unstable_mod::deprecated(); // { dg-warning "" "" { target *-*-* } }
        unstable_mod::unstable();

        let _ = Unstable::UnstableVariant;
        let _ = Unstable::StableVariant;

        let x: usize = 0;
        x.unstable();
        x.stable();
    }
}

mod this_crate {
    #[unstable(feature = "unstable_test_feature", issue = "none")]
    #[rustc_deprecated(since = "1.0.0", reason = "text")]
    pub fn deprecated() {}
    #[unstable(feature = "unstable_test_feature", issue = "none")]
    #[rustc_deprecated(since = "1.0.0", reason = "text")]
    pub fn deprecated_text() {}

    #[unstable(feature = "unstable_test_feature", issue = "none")]
    pub fn unstable() {}
    #[unstable(feature = "unstable_test_feature", reason = "text", issue = "none")]
    pub fn unstable_text() {}

    #[stable(feature = "rust1", since = "1.0.0")]
    pub fn stable() {}
    #[stable(feature = "rust1", since = "1.0.0")]
    pub fn stable_text() {}

    #[stable(feature = "rust1", since = "1.0.0")]
    pub struct MethodTester;

    impl MethodTester {
        #[unstable(feature = "unstable_test_feature", issue = "none")]
        #[rustc_deprecated(since = "1.0.0", reason = "text")]
        pub fn method_deprecated(&self) {}
        #[unstable(feature = "unstable_test_feature", issue = "none")]
        #[rustc_deprecated(since = "1.0.0", reason = "text")]
        pub fn method_deprecated_text(&self) {}

        #[unstable(feature = "unstable_test_feature", issue = "none")]
        pub fn method_unstable(&self) {}
        #[unstable(feature = "unstable_test_feature", reason = "text", issue = "none")]
        pub fn method_unstable_text(&self) {}

        #[stable(feature = "rust1", since = "1.0.0")]
        pub fn method_stable(&self) {}
        #[stable(feature = "rust1", since = "1.0.0")]
        pub fn method_stable_text(&self) {}
    }

    pub trait Trait {
        #[unstable(feature = "unstable_test_feature", issue = "none")]
        #[rustc_deprecated(since = "1.0.0", reason = "text")]
        fn trait_deprecated(&self) {}
        #[unstable(feature = "unstable_test_feature", issue = "none")]
        #[rustc_deprecated(since = "1.0.0", reason = "text")]
        fn trait_deprecated_text(&self) {}

        #[unstable(feature = "unstable_test_feature", issue = "none")]
        fn trait_unstable(&self) {}
        #[unstable(feature = "unstable_test_feature", reason = "text", issue = "none")]
        fn trait_unstable_text(&self) {}

        #[stable(feature = "rust1", since = "1.0.0")]
        fn trait_stable(&self) {}
        #[stable(feature = "rust1", since = "1.0.0")]
        fn trait_stable_text(&self) {}
    }

    impl Trait for MethodTester {}

    #[unstable(feature = "unstable_test_feature", issue = "none")]
    #[rustc_deprecated(since = "1.0.0", reason = "text")]
    pub struct DeprecatedStruct {
        #[stable(feature = "stable_test_feature", since = "1.0.0")] i: isize
    }
    #[unstable(feature = "unstable_test_feature", issue = "none")]
    pub struct UnstableStruct {
        #[stable(feature = "stable_test_feature", since = "1.0.0")] i: isize
    }
    #[stable(feature = "rust1", since = "1.0.0")]
    pub struct StableStruct {
        #[stable(feature = "stable_test_feature", since = "1.0.0")] i: isize
    }

    #[unstable(feature = "unstable_test_feature", issue = "none")]
    #[rustc_deprecated(since = "1.0.0", reason = "text")]
    pub struct DeprecatedUnitStruct;
    #[unstable(feature = "unstable_test_feature", issue = "none")]
    pub struct UnstableUnitStruct;
    #[stable(feature = "rust1", since = "1.0.0")]
    pub struct StableUnitStruct;

    pub enum Enum {
        #[unstable(feature = "unstable_test_feature", issue = "none")]
        #[rustc_deprecated(since = "1.0.0", reason = "text")]
        DeprecatedVariant,
        #[unstable(feature = "unstable_test_feature", issue = "none")]
        UnstableVariant,

        #[stable(feature = "rust1", since = "1.0.0")]
        StableVariant,
    }

    #[unstable(feature = "unstable_test_feature", issue = "none")]
    #[rustc_deprecated(since = "1.0.0", reason = "text")]
    pub struct DeprecatedTupleStruct(isize);
    #[unstable(feature = "unstable_test_feature", issue = "none")]
    pub struct UnstableTupleStruct(isize);
    #[stable(feature = "rust1", since = "1.0.0")]
    pub struct StableTupleStruct(isize);

    fn test() {
        // Only the deprecated cases of the following should generate
        // errors, because other stability attributes now have meaning
        // only *across* crates, not within a single crate.

        type Foo = MethodTester;
        let foo = MethodTester;

        deprecated(); // { dg-warning "" "" { target *-*-* } }
        foo.method_deprecated(); // { dg-warning "" "" { target *-*-* } }
        Foo::method_deprecated(&foo); // { dg-warning "" "" { target *-*-* } }
        <Foo>::method_deprecated(&foo); // { dg-warning "" "" { target *-*-* } }
        foo.trait_deprecated(); // { dg-warning "" "" { target *-*-* } }
        Trait::trait_deprecated(&foo); // { dg-warning "" "" { target *-*-* } }
        <Foo>::trait_deprecated(&foo); // { dg-warning "" "" { target *-*-* } }
        <Foo as Trait>::trait_deprecated(&foo); // { dg-warning "" "" { target *-*-* } }

        deprecated_text(); // { dg-warning "" "" { target *-*-* } }
        foo.method_deprecated_text(); // { dg-warning "" "" { target *-*-* } }
        Foo::method_deprecated_text(&foo); // { dg-warning "" "" { target *-*-* } }
        <Foo>::method_deprecated_text(&foo); // { dg-warning "" "" { target *-*-* } }
        foo.trait_deprecated_text(); // { dg-warning "" "" { target *-*-* } }
        Trait::trait_deprecated_text(&foo); // { dg-warning "" "" { target *-*-* } }
        <Foo>::trait_deprecated_text(&foo); // { dg-warning "" "" { target *-*-* } }
        <Foo as Trait>::trait_deprecated_text(&foo); // { dg-warning "" "" { target *-*-* } }

        unstable();
        foo.method_unstable();
        Foo::method_unstable(&foo);
        <Foo>::method_unstable(&foo);
        foo.trait_unstable();
        Trait::trait_unstable(&foo);
        <Foo>::trait_unstable(&foo);
        <Foo as Trait>::trait_unstable(&foo);

        unstable_text();
        foo.method_unstable_text();
        Foo::method_unstable_text(&foo);
        <Foo>::method_unstable_text(&foo);
        foo.trait_unstable_text();
        Trait::trait_unstable_text(&foo);
        <Foo>::trait_unstable_text(&foo);
        <Foo as Trait>::trait_unstable_text(&foo);

        stable();
        foo.method_stable();
        Foo::method_stable(&foo);
        <Foo>::method_stable(&foo);
        foo.trait_stable();
        Trait::trait_stable(&foo);
        <Foo>::trait_stable(&foo);
        <Foo as Trait>::trait_stable(&foo);

        stable_text();
        foo.method_stable_text();
        Foo::method_stable_text(&foo);
        <Foo>::method_stable_text(&foo);
        foo.trait_stable_text();
        Trait::trait_stable_text(&foo);
        <Foo>::trait_stable_text(&foo);
        <Foo as Trait>::trait_stable_text(&foo);

        let _ = DeprecatedStruct {
// { dg-warning "" "" { target *-*-* } .-1 }
            i: 0 // { dg-warning "" "" { target *-*-* } }
        };
        let _ = UnstableStruct { i: 0 };
        let _ = StableStruct { i: 0 };

        let _ = DeprecatedUnitStruct; // { dg-warning "" "" { target *-*-* } }
        let _ = UnstableUnitStruct;
        let _ = StableUnitStruct;

        let _ = Enum::DeprecatedVariant; // { dg-warning "" "" { target *-*-* } }
        let _ = Enum::UnstableVariant;
        let _ = Enum::StableVariant;

        let _ = DeprecatedTupleStruct (1); // { dg-warning "" "" { target *-*-* } }
        let _ = UnstableTupleStruct (1);
        let _ = StableTupleStruct (1);
    }

    fn test_method_param<Foo: Trait>(foo: Foo) {
        foo.trait_deprecated(); // { dg-warning "" "" { target *-*-* } }
        Trait::trait_deprecated(&foo); // { dg-warning "" "" { target *-*-* } }
        <Foo>::trait_deprecated(&foo); // { dg-warning "" "" { target *-*-* } }
        <Foo as Trait>::trait_deprecated(&foo); // { dg-warning "" "" { target *-*-* } }
        foo.trait_deprecated_text(); // { dg-warning "" "" { target *-*-* } }
        Trait::trait_deprecated_text(&foo); // { dg-warning "" "" { target *-*-* } }
        <Foo>::trait_deprecated_text(&foo); // { dg-warning "" "" { target *-*-* } }
        <Foo as Trait>::trait_deprecated_text(&foo); // { dg-warning "" "" { target *-*-* } }
        foo.trait_unstable();
        Trait::trait_unstable(&foo);
        <Foo>::trait_unstable(&foo);
        <Foo as Trait>::trait_unstable(&foo);
        foo.trait_unstable_text();
        Trait::trait_unstable_text(&foo);
        <Foo>::trait_unstable_text(&foo);
        <Foo as Trait>::trait_unstable_text(&foo);
        foo.trait_stable();
        Trait::trait_stable(&foo);
        <Foo>::trait_stable(&foo);
        <Foo as Trait>::trait_stable(&foo);
    }

    fn test_method_object(foo: &dyn Trait) {
        foo.trait_deprecated(); // { dg-warning "" "" { target *-*-* } }
        foo.trait_deprecated_text(); // { dg-warning "" "" { target *-*-* } }
        foo.trait_unstable();
        foo.trait_unstable_text();
        foo.trait_stable();
    }

    #[unstable(feature = "unstable_test_feature", issue = "none")]
    #[rustc_deprecated(since = "1.0.0", reason = "text")]
    fn test_fn_body() {
        fn fn_in_body() {}
        fn_in_body(); // { dg-warning "" "" { target *-*-* } }
    }

    impl MethodTester {
        #[unstable(feature = "unstable_test_feature", issue = "none")]
        #[rustc_deprecated(since = "1.0.0", reason = "text")]
        fn test_method_body(&self) {
            fn fn_in_body() {}
            fn_in_body(); // { dg-warning "" "" { target *-*-* } }
        }
    }

    #[unstable(feature = "unstable_test_feature", issue = "none")]
    #[rustc_deprecated(since = "1.0.0", reason = "text")]
    pub trait DeprecatedTrait {
        fn dummy(&self) { }
    }

    struct S;

    impl DeprecatedTrait for S { } // { dg-warning "" "" { target *-*-* } }

    trait LocalTrait : DeprecatedTrait { } // { dg-warning "" "" { target *-*-* } }
}

fn main() {}

