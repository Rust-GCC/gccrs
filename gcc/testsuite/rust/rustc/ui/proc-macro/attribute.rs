// force-host
// no-prefer-dynamic

#![crate_type = "proc-macro"]

extern crate proc_macro;
use proc_macro::*;

#[proc_macro_derive] // { dg-error "" "" { target *-*-* } }
pub fn foo1(input: TokenStream) -> TokenStream { input }

#[proc_macro_derive = ""] // { dg-error "" "" { target *-*-* } }
pub fn foo2(input: TokenStream) -> TokenStream { input }

#[proc_macro_derive(d3, a, b)]
// { dg-error "" "" { target *-*-* } .-1 }
pub fn foo3(input: TokenStream) -> TokenStream { input }

#[proc_macro_derive(d4, attributes(a), b)]
// { dg-error "" "" { target *-*-* } .-1 }
pub fn foo4(input: TokenStream) -> TokenStream { input }

#[proc_macro_derive("a")]
// { dg-error "" "" { target *-*-* } .-1 }
pub fn foo5(input: TokenStream) -> TokenStream { input }

#[proc_macro_derive(d6 = "")]
// { dg-error "" "" { target *-*-* } .-1 }
pub fn foo6(input: TokenStream) -> TokenStream { input }

#[proc_macro_derive(m::d7)]
// { dg-error "" "" { target *-*-* } .-1 }
pub fn foo7(input: TokenStream) -> TokenStream { input }

#[proc_macro_derive(d8(a))]
// { dg-error "" "" { target *-*-* } .-1 }
pub fn foo8(input: TokenStream) -> TokenStream { input }

#[proc_macro_derive(self)]
// { dg-error "" "" { target *-*-* } .-1 }
pub fn foo9(input: TokenStream) -> TokenStream { input }

#[proc_macro_derive(PartialEq)] // OK
pub fn foo10(input: TokenStream) -> TokenStream { input }

#[proc_macro_derive(d11, a)]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
pub fn foo11(input: TokenStream) -> TokenStream { input }

#[proc_macro_derive(d12, attributes)]
// { dg-error "" "" { target *-*-* } .-1 }
pub fn foo12(input: TokenStream) -> TokenStream { input }

#[proc_macro_derive(d13, attributes("a"))]
// { dg-error "" "" { target *-*-* } .-1 }
pub fn foo13(input: TokenStream) -> TokenStream { input }

#[proc_macro_derive(d14, attributes(a = ""))]
// { dg-error "" "" { target *-*-* } .-1 }
pub fn foo14(input: TokenStream) -> TokenStream { input }

#[proc_macro_derive(d15, attributes(m::a))]
// { dg-error "" "" { target *-*-* } .-1 }
pub fn foo15(input: TokenStream) -> TokenStream { input }

#[proc_macro_derive(d16, attributes(a(b)))]
// { dg-error "" "" { target *-*-* } .-1 }
pub fn foo16(input: TokenStream) -> TokenStream { input }

#[proc_macro_derive(d17, attributes(self))]
// { dg-error "" "" { target *-*-* } .-1 }
pub fn foo17(input: TokenStream) -> TokenStream { input }

