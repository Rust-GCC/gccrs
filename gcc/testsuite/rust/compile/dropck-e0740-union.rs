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
#[lang = "manually_drop"]
pub struct ManuallyDrop<T>(T);

struct NeedsDrop;

impl Drop for NeedsDrop {
    fn drop(&mut self) {}
}

union BadUnion {
    field: NeedsDrop, // { dg-error "field must implement Copy or be wrapped in ManuallyDrop to be used in a union .E0740." }
}

union GoodUnion {
    field: ManuallyDrop<NeedsDrop>,
}

fn main() {}
