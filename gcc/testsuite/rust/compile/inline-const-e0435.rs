#![feature(no_core, lang_items, unboxed_closures)]
#![no_core]

#[lang = "sized"]
pub trait Sized {}

#[lang = "fn_once"]
#[rustc_paren_sugar]
pub trait FnOnce<Args> {
    #[lang = "fn_once_output"]
    type Output;
    extern "rust-call" fn call_once(self, args: Args) -> Self::Output;
}

fn local() {
    let x = 1;
    let _ = const { x + 1 }; // { dg-error "attempt to use a non-constant value in a constant .E0435." }
}

fn param(p: i32) {
    let _ = const { p }; // { dg-error "attempt to use a non-constant value in a constant .E0435." }
}

fn in_closure() {
    let x = 1;
    let _c = || const { x }; // { dg-error "attempt to use a non-constant value in a constant .E0435." }
}

fn main() {}
