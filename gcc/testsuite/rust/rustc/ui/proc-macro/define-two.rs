// force-host
// no-prefer-dynamic

#![crate_type = "proc-macro"]

extern crate proc_macro;

use proc_macro::TokenStream;

#[proc_macro_derive(A)]
pub fn foo(input: TokenStream) -> TokenStream {
    input
}

#[proc_macro_derive(A)] // { dg-error ".E0428." "" { target *-*-* } }
pub fn bar(input: TokenStream) -> TokenStream {
    input
}

