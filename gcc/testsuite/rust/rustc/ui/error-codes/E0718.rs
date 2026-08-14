#![feature(lang_items)]

// Box is expected to be a struct, so this will error.
#[lang = "owned_box"] // { dg-error ".E0718." "" { target *-*-* } }
static X: u32 = 42;

fn main() {}

