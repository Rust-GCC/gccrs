#![feature(type_alias_impl_trait)]

fn main() {}

type Two<'a, 'b> = impl std::fmt::Debug;

fn one<'a>(t: &'a ()) -> Two<'a, 'a> { // { dg-error "" "" { target *-*-* } }
    t
}

