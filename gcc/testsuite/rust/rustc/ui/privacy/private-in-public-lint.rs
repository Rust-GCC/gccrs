mod m1 {
    pub struct Pub;
    struct Priv;

    impl Pub {
        pub fn f() -> Priv {Priv} // { dg-error ".E0446." "" { target *-*-* } }
    }
}

mod m2 {
    pub struct Pub;
    struct Priv;

    impl Pub {
        pub fn f() -> Priv {Priv} // { dg-error ".E0446." "" { target *-*-* } }
    }
}

fn main() {}

