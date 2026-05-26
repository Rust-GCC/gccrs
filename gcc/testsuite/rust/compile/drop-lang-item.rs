#![feature(no_core)]
#![no_core]

#![feature(lang_items)]

#[lang = "sized"]
pub trait Sized {}

#[lang = "drop"]
pub trait Drop {
    fn drop(&mut self);
}

fn main() {}