#![feature(no_core)]
#![feature(lang_items)]
#![no_core]
#[lang = "sized"]
pub trait Sized {}
#[lang = "copy"]
pub trait Copy {}
#[lang = "drop"]
pub trait Drop {
    fn drop(&mut self);
}

struct CopyThenDrop;

impl Copy for CopyThenDrop {}

impl Drop for CopyThenDrop { // { dg-error "the trait Copy cannot be implemented for type that has a destructor .E0184." }
    fn drop(&mut self) {}
}

struct DropThenCopy;

impl Drop for DropThenCopy {
    fn drop(&mut self) {}
}

impl Copy for DropThenCopy {} // { dg-error "the trait Copy cannot be implemented for type that has a destructor .E0184." }

fn main() {}
