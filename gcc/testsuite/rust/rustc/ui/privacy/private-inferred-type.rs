#![feature(decl_macro)]
#![allow(private_in_public)]

mod m {
    fn priv_fn() {}
    static PRIV_STATIC: u8 = 0;
    enum PrivEnum { Variant }
    pub enum PubEnum { Variant }
    trait PrivTrait { fn method() {} }
    impl PrivTrait for u8 {}
    pub trait PubTrait { fn method() {} }
    impl PubTrait for u8 {}
    struct PrivTupleStruct(u8);
    pub struct PubTupleStruct(u8);
    impl PubTupleStruct { fn method() {} }

    #[derive(Clone, Copy)]
    struct Priv;
    pub type Alias = Priv;
    pub struct Pub<T = Alias>(pub T);

    impl Pub<Priv> {
        pub fn static_method() {}
        pub const INHERENT_ASSOC_CONST: u8 = 0;
    }
    impl<T> Pub<T> {
        pub fn static_method_generic_self() {}
        pub const INHERENT_ASSOC_CONST_GENERIC_SELF: u8 = 0;
    }
    impl Pub<u8> {
        fn priv_method(&self) {}
        pub fn method_with_substs<T>(&self) {}
        pub fn method_with_priv_params(&self, _: Priv) {}
    }
    impl TraitWithAssocConst for Priv {}
    impl TraitWithAssocTy for Priv { type AssocTy = u8; }

    pub macro m() {
        priv_fn; // { dg-error "" "" { target *-*-* } }
        PRIV_STATIC; // OK, not cross-crate
        PrivEnum::Variant; // { dg-error "" "" { target *-*-* } }
        PubEnum::Variant; // OK
        <u8 as PrivTrait>::method; // { dg-error "" "" { target *-*-* } }
        <u8 as PubTrait>::method; // OK
        PrivTupleStruct;
// { dg-error "" "" { target *-*-* } .-1 }
        PubTupleStruct;
// { dg-error "" "" { target *-*-* } .-1 }
        Pub(0u8).priv_method();
// { dg-error "" "" { target *-*-* } .-1 }
    }

    trait Trait {}
    pub trait TraitWithTyParam<T> {}
    pub trait TraitWithTyParam2<T> { fn pub_method() {} }
    pub trait TraitWithAssocTy { type AssocTy; }
    pub trait TraitWithAssocConst { const TRAIT_ASSOC_CONST: u8 = 0; }
    impl Trait for u8 {}
    impl<T> TraitWithTyParam<T> for u8 {}
    impl TraitWithTyParam2<Priv> for u8 {}
    impl TraitWithAssocTy for u8 { type AssocTy = Priv; }
// { dg-error ".E0446." "" { target *-*-* } .-1 }

    pub fn leak_anon1() -> impl Trait + 'static { 0 }
    pub fn leak_anon2() -> impl TraitWithTyParam<Alias> { 0 }
    pub fn leak_anon3() -> impl TraitWithAssocTy<AssocTy = Alias> { 0 }

    pub fn leak_dyn1() -> Box<dyn Trait + 'static> { Box::new(0) }
    pub fn leak_dyn2() -> Box<dyn TraitWithTyParam<Alias>> { Box::new(0) }
    pub fn leak_dyn3() -> Box<dyn TraitWithAssocTy<AssocTy = Alias>> { Box::new(0) }
}

mod adjust {
    // Construct a chain of derefs with a private type in the middle
    use std::ops::Deref;

    pub struct S1;
    struct S2;
    pub type S2Alias = S2;
    pub struct S3;

    impl Deref for S1 {
        type Target = S2Alias; // { dg-error ".E0446." "" { target *-*-* } }
        fn deref(&self) -> &Self::Target { loop {} }
    }
    impl Deref for S2 {
        type Target = S3;
        fn deref(&self) -> &Self::Target { loop {} }
    }

    impl S3 {
        pub fn method_s3(&self) {}
    }
}

fn main() {
    let _: m::Alias; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    let _: <m::Alias as m::TraitWithAssocTy>::AssocTy; // { dg-error "" "" { target *-*-* } }
    m::Alias {}; // { dg-error "" "" { target *-*-* } }
    m::Pub { 0: m::Alias {} }; // { dg-error "" "" { target *-*-* } }
    m::Pub { 0: loop {} }; // OK, `m::Pub` is in value context, so it means Pub<_>, not Pub<Priv>
    m::Pub::static_method; // { dg-error "" "" { target *-*-* } }
    m::Pub::INHERENT_ASSOC_CONST; // { dg-error "" "" { target *-*-* } }
    m::Pub(0u8).method_with_substs::<m::Alias>(); // { dg-error "" "" { target *-*-* } }
    m::Pub(0u8).method_with_priv_params(loop{}); // { dg-error "" "" { target *-*-* } }
    <m::Alias as m::TraitWithAssocConst>::TRAIT_ASSOC_CONST; // { dg-error "" "" { target *-*-* } }
    <m::Pub<m::Alias>>::INHERENT_ASSOC_CONST; // { dg-error "" "" { target *-*-* } }
    <m::Pub<m::Alias>>::INHERENT_ASSOC_CONST_GENERIC_SELF; // { dg-error "" "" { target *-*-* } }
    <m::Pub<m::Alias>>::static_method_generic_self; // { dg-error "" "" { target *-*-* } }
    use m::TraitWithTyParam2;
    u8::pub_method; // { dg-error "" "" { target *-*-* } }

    adjust::S1.method_s3(); // { dg-error "" "" { target *-*-* } }

    m::m!();

    m::leak_anon1(); // { dg-error "" "" { target *-*-* } }
    m::leak_anon2(); // { dg-error "" "" { target *-*-* } }
    m::leak_anon3(); // { dg-error "" "" { target *-*-* } }

    m::leak_dyn1(); // { dg-error "" "" { target *-*-* } }
    m::leak_dyn2(); // { dg-error "" "" { target *-*-* } }
    m::leak_dyn3(); // { dg-error "" "" { target *-*-* } }

    // Check that messages are not duplicated for various kinds of assignments
    let a = m::Alias {}; // { dg-error "" "" { target *-*-* } }
    let mut b = a; // { dg-error "" "" { target *-*-* } }
    b = a; // { dg-error "" "" { target *-*-* } }
    match a { // { dg-error "" "" { target *-*-* } }
        _ => {}
    }
}

