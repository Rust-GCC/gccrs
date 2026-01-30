// { dg-additional-options "-frust-edition=2018" }
#![allow(unused)]
#![allow(incomplete_features)]

#![feature(no_core)]
#![no_core]

mod valid {
    pub fn unqualified_fn() {}

    const fn const_fn() {}

    pub async fn async_fn() {}

    pub unsafe fn unsafe_fn() {}

    async unsafe fn async_unsafe_fn() {}

    unsafe extern "C" fn unsafe_extern_fn() {}

    async unsafe extern "C" fn async_unsafe_extern_fn() {}

    const unsafe extern "C" fn const_unsafe_extern_fn() {}

    /*
    // `safe` is not implemented yet
    unsafe extern "C" {
        safe fn const_safe_extern_fn();
    }
    */
}

mod invalid {
    // `async` precedes `unsafe`
    unsafe async fn async_unsafe_fn() {}
    // { dg-error "unexpected token .async. in some sort of unsafe production" "" { target *-*-* } .-1 }
    // { dg-error "failed to parse item in module" "" { target *-*-* } .-2 }
}

