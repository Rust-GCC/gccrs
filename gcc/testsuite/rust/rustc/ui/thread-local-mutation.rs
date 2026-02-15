// Regression test for #54901: immutable thread locals could be mutated. See:
// https://github.com/rust-lang/rust/issues/29594#issuecomment-328177697
// https://github.com/rust-lang/rust/issues/54901

#![feature(thread_local)]

#[thread_local]
static S: &str = "before";

fn set_s() {
    S = "after"; // { dg-error ".E0594." "" { target *-*-* } }
}

fn main() {
    println!("{}", S);
    set_s();
    println!("{}", S);
}

