// aux-build:define-macro.rs

macro_rules! bar { () => {} }
define_macro!(bar);
bar!(); // { dg-error ".E0659." "" { target *-*-* } }

macro_rules! m { () => { #[macro_use] extern crate define_macro; } }
m!();

fn main() {}

