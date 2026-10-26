// { dg-additional-options "-w" }
#![feature(no_core, lang_items)]
#![no_core]

// Rust-GCC/gccrs#4917: the `where Self: DoubleEndedIterator` on `rev` used
// to be added to the trait's shared Self parameter, so every default method
// of Iterator, like `count`, required DoubleEndedIterator and was not found.

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

    fn count(self) -> usize
    where
        Self: Sized,
    {
        0
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

pub struct MyIter;

impl Iterator for MyIter {
    type Item = u32;
    fn next(&mut self) -> Option<u32> {
        Option::None
    }
}

impl DoubleEndedIterator for MyIter {
    fn next_back(&mut self) -> Option<u32> {
        Option::None
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
    let _ = MyIter.rev().count();
    let _ = Forward.count();
}
