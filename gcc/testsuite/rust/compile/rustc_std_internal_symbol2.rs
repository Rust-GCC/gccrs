#![feature(no_core, rustc_attrs)]
#![no_core]

#[rustc_std_internal_symbol(invalid_argument)] // { dg-error "malformed .rustc_std_internal_symbol. attribute input" }
pub fn _bar() -> i32 {
    0
}
