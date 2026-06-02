#![feature(no_core)]
#![feature(lang_items)]
#![no_core]

#[lang = "sized"]
pub trait Sized {}

#[lang = "drop"]
pub trait Drop {
    fn drop (&mut self);
}

struct Droppable;

impl Drop for Droppable {
    fn drop(&mut self) {}
}

fn main() {
    {
        let ref _x = Droppable; // { dg-message "sorry, unimplemented: drop trait not supported for subpatterns and ref patterns" }
    }
}