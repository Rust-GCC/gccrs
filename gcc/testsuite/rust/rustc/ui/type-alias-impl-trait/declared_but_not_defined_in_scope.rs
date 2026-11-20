#![feature(type_alias_impl_trait)]

fn main() {}

mod boo {
    // declared in module but not defined inside of it
    pub type Boo = impl ::std::fmt::Debug; // { dg-error "" "" { target *-*-* } }
}

fn bomp() -> boo::Boo {
    ""
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

