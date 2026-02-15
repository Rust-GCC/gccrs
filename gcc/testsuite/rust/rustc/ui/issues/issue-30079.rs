struct SemiPriv;

mod m1 {
    struct Priv;
    impl ::SemiPriv {
        pub fn f(_: Priv) {} // { dg-warning "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-1 }
    }

    impl Priv {
        pub fn f(_: Priv) {} // ok
    }
}

mod m2 {
    struct Priv;
    impl ::std::ops::Deref for ::SemiPriv {
        type Target = Priv; // { dg-error ".E0446." "" { target *-*-* } }
        fn deref(&self) -> &Self::Target { unimplemented!() }
    }

    impl ::std::ops::Deref for Priv {
        type Target = Priv; // ok
        fn deref(&self) -> &Self::Target { unimplemented!() }
    }
}

trait SemiPrivTrait {
    type Assoc;
}

mod m3 {
    struct Priv;
    impl ::SemiPrivTrait for () {
        type Assoc = Priv; // { dg-error ".E0446." "" { target *-*-* } }
    }
}

fn main() {}

