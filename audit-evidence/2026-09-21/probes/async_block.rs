#![feature(no_core, lang_items)]
#![no_core]
#[lang="sized"] trait Sized {}
fn main() { let _ = async { 1 }; }
