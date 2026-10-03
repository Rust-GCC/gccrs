// ignore-tidy-linelength

#![feature(decl_macro, associated_type_defaults)]
#![allow(unused, private_in_public)]

mod priv_trait {
    trait PrivTr {
        fn method(&self) {}
        const CONST: u8 = 0;
        type AssocTy = u8;
    }
    pub struct Pub;
    impl PrivTr for Pub {}
    pub trait PubTr: PrivTr {}

    pub macro mac() {
        let value = <Pub as PrivTr>::method;
// { dg-error "" "" { target *-*-* } .-1 }
        value;
// { dg-error "" "" { target *-*-* } .-1 }
        Pub.method();
// { dg-error "" "" { target *-*-* } .-1 }
        <Pub as PrivTr>::CONST;
// { dg-error "" "" { target *-*-* } .-1 }
        let _: <Pub as PrivTr>::AssocTy;
// { dg-error "" "" { target *-*-* } .-1 }
        pub type InSignatureTy = <Pub as PrivTr>::AssocTy;
// { dg-error "" "" { target *-*-* } .-1 }
        pub trait InSignatureTr: PrivTr {}
// { dg-error "" "" { target *-*-* } .-1 }
        impl PrivTr for u8 {}
// { dg-error "" "" { target *-*-* } .-1 }
    }
}
fn priv_trait() {
    priv_trait::mac!();
}

mod priv_signature {
    pub trait PubTr {
        fn method(&self, arg: Priv) {}
    }
    struct Priv;
    pub struct Pub;
    impl PubTr for Pub {}

    pub macro mac() {
        let value = <Pub as PubTr>::method;
// { dg-error "" "" { target *-*-* } .-1 }
        value;
// { dg-error "" "" { target *-*-* } .-1 }
        Pub.method(loop {});
// { dg-error "" "" { target *-*-* } .-1 }
    }
}
fn priv_signature() {
    priv_signature::mac!();
}

mod priv_substs {
    pub trait PubTr {
        fn method<T>(&self) {}
    }
    struct Priv;
    pub struct Pub;
    impl PubTr for Pub {}

    pub macro mac() {
        let value = <Pub as PubTr>::method::<Priv>;
// { dg-error "" "" { target *-*-* } .-1 }
        value;
// { dg-error "" "" { target *-*-* } .-1 }
        Pub.method::<Priv>();
// { dg-error "" "" { target *-*-* } .-1 }
    }
}
fn priv_substs() {
    priv_substs::mac!();
}

mod priv_parent_substs {
    pub trait PubTr<T = Priv> {
        fn method(&self) {}
        const CONST: u8 = 0;
        type AssocTy = u8;
    }
    struct Priv;
    pub struct Pub;
    impl PubTr<Priv> for Pub {}
    impl PubTr<Pub> for Priv {}

    pub macro mac() {
        let value = <Pub as PubTr>::method;
// { dg-error "" "" { target *-*-* } .-1 }
        value;
// { dg-error "" "" { target *-*-* } .-1 }
        let value = <Pub as PubTr<_>>::method;
// { dg-error "" "" { target *-*-* } .-1 }
        value;
// { dg-error "" "" { target *-*-* } .-1 }
        Pub.method();
// { dg-error "" "" { target *-*-* } .-1 }

        let value = <Priv as PubTr<_>>::method;
// { dg-error "" "" { target *-*-* } .-1 }
        value;
// { dg-error "" "" { target *-*-* } .-1 }
        Priv.method();
// { dg-error "" "" { target *-*-* } .-1 }

        <Pub as PubTr>::CONST;
// { dg-error "" "" { target *-*-* } .-1 }
        <Pub as PubTr<_>>::CONST;
// { dg-error "" "" { target *-*-* } .-1 }
        <Priv as PubTr<_>>::CONST;
// { dg-error "" "" { target *-*-* } .-1 }

        let _: <Pub as PubTr>::AssocTy; // FIXME no longer an error?!
        let _: <Pub as PubTr<_>>::AssocTy;
// { dg-error "" "" { target *-*-* } .-1 }
        let _: <Priv as PubTr<_>>::AssocTy;
// { dg-error "" "" { target *-*-* } .-1 }

        pub type InSignatureTy1 = <Pub as PubTr>::AssocTy;
// { dg-error "" "" { target *-*-* } .-1 }
        pub type InSignatureTy2 = <Priv as PubTr<Pub>>::AssocTy;
// { dg-error "" "" { target *-*-* } .-1 }
        impl PubTr for u8 {}
// { dg-error "" "" { target *-*-* } .-1 }
    }
}
fn priv_parent_substs() {
    priv_parent_substs::mac!();
}

fn main() {}

