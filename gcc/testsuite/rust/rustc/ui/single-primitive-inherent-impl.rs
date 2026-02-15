// ignore-tidy-linelength

#![crate_type = "lib"]
#![feature(lang_items)]
#![no_std]

// OK
#[lang = "str_alloc"]
impl str {}

impl str {
// { dg-error ".E0390." "" { target *-*-* } .-1 }
}

