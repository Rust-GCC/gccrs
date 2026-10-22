// aux-build:private-inferred-type.rs

#![allow(warnings)]

extern crate private_inferred_type as ext;

mod m {
    struct Priv;
    pub type Alias = Priv;

    pub trait Trait { type X; }
    impl Trait for Priv { type X = u8; }
}

fn f(_: m::Alias) {} // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
fn f_ext(_: ext::Alias) {} // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }

trait Tr1 {}
impl m::Alias {} // { dg-error "" "" { target *-*-* } }
impl Tr1 for ext::Alias {} // { dg-error "" "" { target *-*-* } }
type A = <m::Alias as m::Trait>::X; // { dg-error "" "" { target *-*-* } }

trait Tr2<T> {}
impl<T> Tr2<T> for u8 {}
fn g() -> impl Tr2<m::Alias> { 0 } // { dg-error "" "" { target *-*-* } }
fn g_ext() -> impl Tr2<ext::Alias> { 0 } // { dg-error "" "" { target *-*-* } }

fn main() {}

