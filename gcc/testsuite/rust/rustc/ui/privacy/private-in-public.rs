// Private types and traits are not allowed in public interfaces.
// This test also ensures that the checks are performed even inside private modules.

#![feature(associated_type_defaults)]

mod types {
    struct Priv;
    pub struct Pub;
    pub trait PubTr {
        type Alias;
    }

    pub const C: Priv = Priv; // { dg-error ".E0446." "" { target *-*-* } }
    pub static S: Priv = Priv; // { dg-error ".E0446." "" { target *-*-* } }
    pub fn f1(arg: Priv) {} // { dg-error ".E0446." "" { target *-*-* } }
    pub fn f2() -> Priv { panic!() } // { dg-error ".E0446." "" { target *-*-* } }
    pub struct S1(pub Priv); // { dg-error ".E0446." "" { target *-*-* } }
    pub struct S2 { pub field: Priv } // { dg-error ".E0446." "" { target *-*-* } }
    impl Pub {
        pub const C: Priv = Priv; // { dg-error ".E0446." "" { target *-*-* } }
        pub fn f1(arg: Priv) {} // { dg-error ".E0446." "" { target *-*-* } }
        pub fn f2() -> Priv { panic!() } // { dg-error ".E0446." "" { target *-*-* } }
    }
}

mod traits {
    trait PrivTr {}
    pub struct Pub<T>(T);
    pub trait PubTr {}

    pub enum E<T: PrivTr> { V(T) } // { dg-error ".E0445." "" { target *-*-* } }
    pub fn f<T: PrivTr>(arg: T) {} // { dg-error ".E0445." "" { target *-*-* } }
    pub struct S1<T: PrivTr>(T); // { dg-error ".E0445." "" { target *-*-* } }
    impl<T: PrivTr> Pub<T> { // { dg-error ".E0445." "" { target *-*-* } }
        pub fn f<U: PrivTr>(arg: U) {} // { dg-error ".E0445." "" { target *-*-* } }
    }
}

mod traits_where {
    trait PrivTr {}
    pub struct Pub<T>(T);
    pub trait PubTr {}

    pub enum E<T> where T: PrivTr { V(T) }
// { dg-error ".E0445." "" { target *-*-* } .-1 }
    pub fn f<T>(arg: T) where T: PrivTr {}
// { dg-error ".E0445." "" { target *-*-* } .-1 }
    pub struct S1<T>(T) where T: PrivTr;
// { dg-error ".E0445." "" { target *-*-* } .-1 }
    impl<T> Pub<T> where T: PrivTr {
// { dg-error ".E0445." "" { target *-*-* } .-1 }
        pub fn f<U>(arg: U) where U: PrivTr {}
// { dg-error ".E0445." "" { target *-*-* } .-1 }
    }
}

mod generics {
    struct Priv<T = u8>(T);
    pub struct Pub<T = u8>(T);
    trait PrivTr<T> {}
    pub trait PubTr<T> {}

    pub fn f1(arg: [Priv; 1]) {} // { dg-error ".E0446." "" { target *-*-* } }
    pub fn f2(arg: Pub<Priv>) {} // { dg-error ".E0446." "" { target *-*-* } }
    pub fn f3(arg: Priv<Pub>) {}
// { dg-error ".E0446." "" { target *-*-* } .-1 }
}

mod impls {
    struct Priv;
    pub struct Pub;
    trait PrivTr {
        type Alias;
    }
    pub trait PubTr {
        type Alias;
    }

    impl Pub {
        pub fn f(arg: Priv) {} // { dg-error ".E0446." "" { target *-*-* } }
    }
}

mod aliases_pub {
    struct Priv;
    mod m {
        pub struct Pub1;
        pub struct Pub2;
        pub struct Pub3;
        pub trait PubTr<T = u8> {
            type Check = u8;
        }
    }

    use self::m::Pub1 as PrivUseAlias;
    use self::m::PubTr as PrivUseAliasTr;
    type PrivAlias = m::Pub2;
    trait PrivTr {
        type Assoc = m::Pub3;
    }
    impl PrivTr for Priv {}

    // This should be OK, but associated type aliases are not substituted yet
    pub fn f3(arg: <Priv as PrivTr>::Assoc) {}
// { dg-error ".E0446." "" { target *-*-* } .-1 }
// { dg-error ".E0446." "" { target *-*-* } .-2 }

    impl PrivUseAlias {
        pub fn f(arg: Priv) {} // { dg-error ".E0446." "" { target *-*-* } }
    }
}

mod aliases_priv {
    struct Priv;

    struct Priv1;
    struct Priv2;
    struct Priv3;
    trait PrivTr1<T = u8> {
        type Check = u8;
    }

    use self::Priv1 as PrivUseAlias;
    use self::PrivTr1 as PrivUseAliasTr;
    type PrivAlias = Priv2;
    trait PrivTr {
        type Assoc = Priv3;
    }
    impl PrivTr for Priv {}

    pub fn f1(arg: PrivUseAlias) {} // { dg-error ".E0446." "" { target *-*-* } }
    pub fn f2(arg: PrivAlias) {} // { dg-error ".E0446." "" { target *-*-* } }
    pub fn f3(arg: <Priv as PrivTr>::Assoc) {}
// { dg-error ".E0446." "" { target *-*-* } .-1 }
// { dg-error ".E0446." "" { target *-*-* } .-2 }
}

mod aliases_params {
    struct Priv;
    type PrivAliasGeneric<T = Priv> = T;
    type Result<T> = ::std::result::Result<T, Priv>;

    pub fn f2(arg: PrivAliasGeneric) {}
// { dg-error ".E0446." "" { target *-*-* } .-1 }
    pub fn f3(arg: Result<u8>) {} // { dg-error ".E0446." "" { target *-*-* } }
}

fn main() {}

