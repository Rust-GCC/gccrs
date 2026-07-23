// force-host
// no-prefer-dynamic

#![crate_type = "proc-macro"]

extern crate proc_macro;

use proc_macro::TokenStream;

#[proc_macro = "test"] // { dg-error "" "" { target *-*-* } }
pub fn a(a: TokenStream) -> TokenStream { a }

#[proc_macro()] // { dg-error "" "" { target *-*-* } }
pub fn c(a: TokenStream) -> TokenStream { a }

#[proc_macro(x)] // { dg-error "" "" { target *-*-* } }
pub fn d(a: TokenStream) -> TokenStream { a }

#[proc_macro_attribute = "test"] // { dg-error "" "" { target *-*-* } }
pub fn e(_: TokenStream, a: TokenStream) -> TokenStream { a }

#[proc_macro_attribute()] // { dg-error "" "" { target *-*-* } }
pub fn g(_: TokenStream, a: TokenStream) -> TokenStream { a }

#[proc_macro_attribute(x)] // { dg-error "" "" { target *-*-* } }
pub fn h(_: TokenStream, a: TokenStream) -> TokenStream { a }

