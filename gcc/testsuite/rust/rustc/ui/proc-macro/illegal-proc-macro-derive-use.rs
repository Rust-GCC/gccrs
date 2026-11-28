extern crate proc_macro;

#[proc_macro_derive(Foo)]
// { dg-error "" "" { target *-*-* } .-1 }
pub fn foo(a: proc_macro::TokenStream) -> proc_macro::TokenStream {
    a
}

// Issue #37590
#[proc_macro_derive(Foo)]
// { dg-error "" "" { target *-*-* } .-1 }
pub struct Foo {
}

fn main() {}

