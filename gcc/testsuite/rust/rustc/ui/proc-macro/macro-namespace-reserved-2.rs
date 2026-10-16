// force-host
// no-prefer-dynamic

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

fn check_bang1() {
    my_macro!(); // { dg-error "" "" { target *-*-* } }
}
fn check_bang2() {
    my_macro_attr!(); // { dg-error "" "" { target *-*-* } }
    crate::my_macro_attr!(); // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
}
fn check_bang3() {
    MyTrait!(); // { dg-error "" "" { target *-*-* } }
    crate::MyTrait!(); // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
}

#[my_macro] // { dg-error "" "" { target *-*-* } }
#[crate::my_macro] // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
fn check_attr1() {}
#[my_macro_attr] // { dg-error "" "" { target *-*-* } }
fn check_attr2() {}
#[MyTrait] // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
fn check_attr3() {}

#[derive(my_macro)] // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
#[derive(crate::my_macro)] // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
struct CheckDerive1;
#[derive(my_macro_attr)] // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
struct CheckDerive2;
#[derive(MyTrait)] // { dg-error "" "" { target *-*-* } }
struct CheckDerive3;

