#![feature(rustc_attrs)]
#![feature(no_core)]
#![no_core]

#[macro_export]
#[rustc_builtin_macro]
macro_rules! assert {
    ($($arg:tt)*) => {};
}

const _: () = assert!(true);
// { dg-error "could not resolve macro invocation .panic." "" { target *-*-* } .-1 }

const _: () = assert!(true, "oops, {}", 12);
// { dg-error "could not resolve macro invocation .panic." "" { target *-*-* } .-1 }
