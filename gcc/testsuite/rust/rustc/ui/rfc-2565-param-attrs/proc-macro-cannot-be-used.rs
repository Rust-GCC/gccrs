// aux-build:ident-mac.rs

#![feature(c_variadic)]

extern crate ident_mac;
use ident_mac::id;

struct W(u8);

extern "C" { fn ffi(#[id] arg1: i32, #[id] ...); }
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

unsafe extern "C" fn cvar(arg1: i32, #[id] mut args: ...) {}
// { dg-error "" "" { target *-*-* } .-1 }

type Alias = extern "C" fn(#[id] u8, #[id] ...);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

fn free(#[id] arg1: u8) {
// { dg-error "" "" { target *-*-* } .-1 }
    let lam = |#[id] W(x), #[id] y: usize| ();
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}

impl W {
    fn inherent1(#[id] self, #[id] arg1: u8) {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    fn inherent2(#[id] &self, #[id] arg1: u8) {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    fn inherent3<'a>(#[id] &'a mut self, #[id] arg1: u8) {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    fn inherent4<'a>(#[id] self: Box<Self>, #[id] arg1: u8) {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    fn issue_64682_associated_fn<'a>(#[id] arg1: u8, #[id] arg2: u8) {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}

trait A {
    fn trait1(#[id] self, #[id] arg1: u8);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    fn trait2(#[id] &self, #[id] arg1: u8);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    fn trait3<'a>(#[id] &'a mut self, #[id] arg1: u8);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    fn trait4<'a>(#[id] self: Box<Self>, #[id] arg1: u8, #[id] Vec<u8>);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
    fn issue_64682_associated_fn<'a>(#[id] arg1: u8, #[id] arg2: u8);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}

fn main() {}

