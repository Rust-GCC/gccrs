#![feature(decl_macro)]

struct X;

macro_rules! define_f_legacy { () => {
    fn f() {}
}}
macro define_g_modern() {
    fn g() {}
}

impl X {
   fn f() {} // { dg-error ".E0592." "" { target *-*-* } }
   fn g() {} // OK
}
impl X {
    define_f_legacy!();
}
impl X {
    define_g_modern!();
}

fn main() {}

