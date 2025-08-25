// edition:2018
// aux-build:edition-kw-macro-2018.rs

#![feature(async_closure)]

fn main() {}

#[macro_use]
extern crate edition_kw_macro_2018;

mod module {
    pub fn r#async() {}
}

pub fn check_async() {
    let mut async = 1; // { dg-error "" "" { target *-*-* } }
    let mut r#async = 1; // OK

    r#async = consumes_async!(async); // OK
    r#async = consumes_async!(r#async); // { dg-error "" "" { target *-*-* } }
    r#async = consumes_async_raw!(async); // { dg-error "" "" { target *-*-* } }
    r#async = consumes_async_raw!(r#async); // OK

    if passes_ident!(async) == 1 {}
    if passes_ident!(r#async) == 1 {} // OK
    module::async(); // { dg-error "" "" { target *-*-* } }
    module::r#async(); // OK

    let _recovery_witness: () = 0; // { dg-error ".E0308." "" { target *-*-* } }
}

