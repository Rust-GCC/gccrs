// aux-build:two_macros.rs

#![allow(unused_macros)]

macro_rules! foo { () => {} }
macro_rules! macro_one { () => {} }
#[macro_use(macro_two)] extern crate two_macros;

macro_rules! m1 { () => {
    macro_rules! foo { () => {} }

    #[macro_use] // { dg-error "" "" { target *-*-* } }
    extern crate two_macros as __;
}}
m1!();

foo!(); // { dg-error ".E0659." "" { target *-*-* } }

macro_rules! m2 { () => {
    macro_rules! foo { () => {} }
    foo!();
}}
m2!();
//^ Since `foo` is not used outside this expansion, it is not a shadowing error.

fn main() {}

