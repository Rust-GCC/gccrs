#![feature(no_core)]
#![feature(rustc_attrs)]
#![no_core]

#[rustc_builtin_macro]
macro_rules! concat {
    ($($e:expr),* $(,)?) => {{ /* built in */ }}
}

// { dg-error "could not resolve" "" { target *-*-* } .+1 }
pub const X: &'static str = concat!("A", stringify!(B), "C", stringify!(D), "E");
