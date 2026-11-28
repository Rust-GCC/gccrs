#![feature(type_alias_impl_trait)]

fn main() {}

// don't reveal the concrete type
type NoReveal = impl std::fmt::Debug;

fn define_no_reveal() -> NoReveal {
    ""
}

fn no_reveal(x: NoReveal) {
    let _: &'static str = x; // { dg-error ".E0308." "" { target *-*-* } }
    let _ = x as &'static str; // { dg-error ".E0605." "" { target *-*-* } }
}

