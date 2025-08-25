#![feature(decl_macro, associated_type_defaults)]
#![allow(unused, private_in_public)]

mod priv_nominal {
    pub struct Pub;
    impl Pub {
        fn method(&self) {}
        const CONST: u8 = 0;
        // type AssocTy = u8;
    }

    pub macro mac() {
        let value = Pub::method;
// { dg-error "" "" { target *-*-* } .-1 }
        value;
// { dg-error "" "" { target *-*-* } .-1 }
        Pub.method();
// { dg-error "" "" { target *-*-* } .-1 }
        Pub::CONST;
// { dg-error "" "" { target *-*-* } .-1 }
        // let _: Pub::AssocTy;
        // pub type InSignatureTy = Pub::AssocTy;
    }
}
fn priv_nominal() {
    priv_nominal::mac!();
}

mod priv_signature {
    struct Priv;
    pub struct Pub;
    impl Pub {
        pub fn method(&self, arg: Priv) {}
    }

    pub macro mac() {
        let value = Pub::method;
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
    struct Priv;
    pub struct Pub;
    impl Pub {
        pub fn method<T>(&self) {}
    }

    pub macro mac() {
        let value = Pub::method::<Priv>;
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
    struct Priv;
    pub struct Pub<T = Priv>(T);
    impl Pub<Priv> {
        pub fn method(&self) {}
        pub fn static_method() {}
        pub const CONST: u8 = 0;
        // pub type AssocTy = u8;
    }

    pub macro mac() {
        let value = <Pub>::method;
// { dg-error "" "" { target *-*-* } .-1 }
        value;
// { dg-error "" "" { target *-*-* } .-1 }
        let value = Pub::method;
// { dg-error "" "" { target *-*-* } .-1 }
        value;
// { dg-error "" "" { target *-*-* } .-1 }
        let value = <Pub>::static_method;
// { dg-error "" "" { target *-*-* } .-1 }
        value;
// { dg-error "" "" { target *-*-* } .-1 }
        let value = Pub::static_method;
// { dg-error "" "" { target *-*-* } .-1 }
        value;
// { dg-error "" "" { target *-*-* } .-1 }
        Pub(Priv).method();
// { dg-error "" "" { target *-*-* } .-1 }

        <Pub>::CONST;
// { dg-error "" "" { target *-*-* } .-1 }
        Pub::CONST;
// { dg-error "" "" { target *-*-* } .-1 }

        // let _: Pub::AssocTy;
        // pub type InSignatureTy = Pub::AssocTy;
    }
}
fn priv_parent_substs() {
    priv_parent_substs::mac!();
}

fn main() {}

