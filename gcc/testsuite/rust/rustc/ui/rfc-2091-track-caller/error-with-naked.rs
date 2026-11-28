#![feature(naked_functions)]

#[track_caller] // { dg-error ".E0736." "" { target *-*-* } }
#[naked]
fn f() {}

struct S;

impl S {
    #[track_caller] // { dg-error ".E0736." "" { target *-*-* } }
    #[naked]
    fn g() {}
}

extern "Rust" {
    #[track_caller] // { dg-error ".E0736." "" { target *-*-* } }
    #[naked]
    fn h();
}

fn main() {}

