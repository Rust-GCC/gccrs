// force-host
// no-prefer-dynamic

#![crate_type = "proc-macro"]

extern crate proc_macro;
use proc_macro::*;

fn foo(arg: TokenStream) -> TokenStream {
    #[proc_macro]
    pub fn foo(arg: TokenStream) -> TokenStream { arg }
// { dg-error "" "" { target *-*-* } .-1 }

    arg
}

