#![feature(associated_type_defaults)]

trait Tr {
    type Y = u16;
    fn Y() {}
}
impl Tr for u8 {}

trait Dr {
    type X = u16;
    fn Z() {}
}
impl Dr for u8 {}

enum E { Y }
type A = u32;

fn main() {
    let _: <u8 as Tr>::N; // { dg-error ".E0576." "" { target *-*-* } }
    let _: <u8 as E>::N; // { dg-error ".E0576." "" { target *-*-* } }
    let _: <u8 as A>::N; // { dg-error ".E0576." "" { target *-*-* } }
    <u8 as Tr>::N; // { dg-error ".E0576." "" { target *-*-* } }
    <u8 as E>::N; // { dg-error ".E0576." "" { target *-*-* } }
    <u8 as A>::N; // { dg-error ".E0576." "" { target *-*-* } }
    let _: <u8 as Tr>::Y; // OK
    let _: <u8 as E>::Y; // { dg-error ".E0575." "" { target *-*-* } }
    <u8 as Tr>::Y; // OK
    <u8 as E>::Y; // { dg-error ".E0575." "" { target *-*-* } }

    let _: <u8 as Tr>::N::NN; // { dg-error ".E0576." "" { target *-*-* } }
    let _: <u8 as E>::N::NN; // { dg-error ".E0576." "" { target *-*-* } }
    let _: <u8 as A>::N::NN; // { dg-error ".E0576." "" { target *-*-* } }
    <u8 as Tr>::N::NN; // { dg-error ".E0576." "" { target *-*-* } }
    <u8 as E>::N::NN; // { dg-error ".E0576." "" { target *-*-* } }
    <u8 as A>::N::NN; // { dg-error ".E0576." "" { target *-*-* } }
    let _: <u8 as Tr>::Y::NN; // { dg-error ".E0223." "" { target *-*-* } }
    let _: <u8 as E>::Y::NN; // { dg-error ".E0575." "" { target *-*-* } }
    <u8 as Tr>::Y::NN; // { dg-error ".E0599." "" { target *-*-* } }
    <u8 as E>::Y::NN; // { dg-error ".E0575." "" { target *-*-* } }

    let _: <u8 as Tr::N>::NN; // { dg-error ".E0576." "" { target *-*-* } }
    let _: <u8 as E::N>::NN; // { dg-error ".E0576." "" { target *-*-* } }
    let _: <u8 as A::N>::NN; // { dg-error ".E0576." "" { target *-*-* } }
    <u8 as Tr::N>::NN; // { dg-error ".E0576." "" { target *-*-* } }
    <u8 as E::N>::NN; // { dg-error ".E0576." "" { target *-*-* } }
    <u8 as A::N>::NN; // { dg-error ".E0576." "" { target *-*-* } }
    let _: <u8 as Tr::Y>::NN; // { dg-error ".E0576." "" { target *-*-* } }
    let _: <u8 as E::Y>::NN; // { dg-error ".E0433." "" { target *-*-* } }
    <u8 as Tr::Y>::NN; // { dg-error ".E0576." "" { target *-*-* } }
    <u8 as E::Y>::NN; // { dg-error ".E0433." "" { target *-*-* } }

    let _: <u8 as Dr>::Z; // { dg-error ".E0575." "" { target *-*-* } }
    <u8 as Dr>::X; // { dg-error ".E0575." "" { target *-*-* } }
    let _: <u8 as Dr>::Z::N; // { dg-error ".E0575." "" { target *-*-* } }
    <u8 as Dr>::X::N; // { dg-error ".E0599." "" { target *-*-* } }
}

