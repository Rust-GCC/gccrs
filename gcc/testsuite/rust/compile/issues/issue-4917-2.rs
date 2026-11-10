#![feature(no_core, lang_items)]
#![no_core]

// Rust-GCC/gccrs#4917: the `where Self: DoubleEndedIterator` on `rev` must
// still be checked for the item it is written on.

#[lang = "sized"]
pub trait Sized {}

pub enum Option<T> {
    None,
    Some(T),
}

pub trait Iterator {
    type Item;
    fn next(&mut self) -> Option<Self::Item>;

    fn rev(self) -> Rev<Self>
    where
        Self: Sized + DoubleEndedIterator,
    {
        Rev { iter: self }
    }
}

pub trait DoubleEndedIterator: Iterator {
    fn next_back(&mut self) -> Option<Self::Item>;
}

pub struct Rev<I> {
    iter: I,
}

impl<I: DoubleEndedIterator> Iterator for Rev<I> {
    type Item = I::Item;
    fn next(&mut self) -> Option<I::Item> {
        self.iter.next_back()
    }
}

pub struct Forward;

impl Iterator for Forward {
    type Item = u32;
    fn next(&mut self) -> Option<u32> {
        Option::None
    }
}

fn main() {
    let _ = Forward.rev();
    // { dg-error "not satisfied" "" { target *-*-* } .-1 }
}
