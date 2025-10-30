#![feature(decl_macro, associated_type_defaults)]
#![allow(unused, private_in_public)]

mod priv_trait {
    trait PrivTr {
        type AssocTy = u8;
    }
    pub trait PubTr: PrivTr {}

    pub macro mac1() {
        let _: Box<dyn PubTr<AssocTy = u8>>;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
        type InSignatureTy2 = Box<dyn PubTr<AssocTy = u8>>;
// { dg-error "" "" { target *-*-* } .-1 }
        trait InSignatureTr2: PubTr<AssocTy = u8> {}
// { dg-error "" "" { target *-*-* } .-1 }
    }
    pub macro mac2() {
        let _: Box<dyn PrivTr<AssocTy = u8>>;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
        type InSignatureTy1 = Box<dyn PrivTr<AssocTy = u8>>;
// { dg-error "" "" { target *-*-* } .-1 }
        trait InSignatureTr1: PrivTr<AssocTy = u8> {}
// { dg-error "" "" { target *-*-* } .-1 }
    }
}
fn priv_trait1() {
    priv_trait::mac1!();
}
fn priv_trait2() {
    priv_trait::mac2!();
}

mod priv_parent_substs {
    pub trait PubTrWithParam<T = Priv> {
        type AssocTy = u8;
    }
    struct Priv;
    pub trait PubTr: PubTrWithParam<Priv> {}

    pub macro mac() {
        let _: Box<dyn PubTrWithParam<AssocTy = u8>>;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
        let _: Box<dyn PubTr<AssocTy = u8>>;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
        pub type InSignatureTy1 = Box<dyn PubTrWithParam<AssocTy = u8>>;
// { dg-error "" "" { target *-*-* } .-1 }
        pub type InSignatureTy2 = Box<dyn PubTr<AssocTy = u8>>;
// { dg-error "" "" { target *-*-* } .-1 }
        trait InSignatureTr1: PubTrWithParam<AssocTy = u8> {}
// { dg-error "" "" { target *-*-* } .-1 }
        trait InSignatureTr2: PubTr<AssocTy = u8> {}
// { dg-error "" "" { target *-*-* } .-1 }
    }
}
fn priv_parent_substs() {
    priv_parent_substs::mac!();
}

fn main() {}

