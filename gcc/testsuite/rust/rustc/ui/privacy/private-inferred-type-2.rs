// aux-build:private-inferred-type.rs

extern crate private_inferred_type as ext;

mod m {
    struct Priv;
    pub struct Pub<T>(pub T);

    impl Pub<Priv> {
        pub fn get_priv() -> Priv { Priv }
        pub fn static_method() {}
    }
}

fn main() {
    m::Pub::get_priv; // { dg-error "" "" { target *-*-* } }
    m::Pub::static_method; // { dg-error "" "" { target *-*-* } }
    ext::Pub::static_method; // { dg-error "" "" { target *-*-* } }
}

