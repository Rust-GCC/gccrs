#![allow(dead_code)]
#[lang = "sized"]
pub trait Sized {}

#[lang = "fn_once"]
pub trait FnOnce<Args> {
    #[lang = "fn_once_output"]
    type Output;

    extern "rust-call" fn call_once(self, args: Args) -> Self::Output;
}
type A = fn();

type B = for<'static> fn();
// { dg-error "invalid lifetime parameter name: .static. .E0262." "" { target *-*-* } .-1 }

pub fn main() {}
