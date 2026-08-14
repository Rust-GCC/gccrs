// force-host
// no-prefer-dynamic

#![feature(decl_macro)]
#![crate_type = "proc-macro"]

extern crate proc_macro;
use proc_macro::*;

#[proc_macro]
pub fn my_macro(input: TokenStream) -> TokenStream {
    input
}

#[proc_macro_attribute]
pub fn my_macro_attr(input: TokenStream, _: TokenStream) -> TokenStream {
    input
}

#[proc_macro_derive(MyTrait)]
pub fn my_macro_derive(input: TokenStream) -> TokenStream {
    input
}

macro my_macro() {} // { dg-error ".E0428." "" { target *-*-* } }
macro my_macro_attr() {} // { dg-error ".E0428." "" { target *-*-* } }
macro MyTrait() {} // { dg-error ".E0428." "" { target *-*-* } }

#[proc_macro_derive(SameName)]
pub fn foo(input: TokenStream) -> TokenStream {
    input
}

#[proc_macro]
pub fn SameName(input: TokenStream) -> TokenStream {
// { dg-error ".E0428." "" { target *-*-* } .-1 }
    input
}

