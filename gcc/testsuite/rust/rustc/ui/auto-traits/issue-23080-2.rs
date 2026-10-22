#![feature(optin_builtin_traits)]
#![feature(negative_impls)]

unsafe auto trait Trait {
    type Output; // { dg-error ".E0380." "" { target *-*-* } }
}

fn call_method<T: Trait>(x: T) {}

fn main() {
    // ICE
    call_method(());
}

