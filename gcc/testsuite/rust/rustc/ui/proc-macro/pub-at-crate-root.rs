// force-host
// no-prefer-dynamic

#![crate_type = "proc-macro"]

extern crate proc_macro;

pub mod a { // { dg-error "" "" { target *-*-* } }
    use proc_macro::TokenStream;

    #[proc_macro_derive(B)]
    pub fn bar(a: TokenStream) -> TokenStream {
// { dg-error "" "" { target *-*-* } .-1 }
        a
    }
}

#[proc_macro_derive(B)]
fn bar(a: proc_macro::TokenStream) -> proc_macro::TokenStream {
// { dg-error "" "" { target *-*-* } .-1 }
    a
}

