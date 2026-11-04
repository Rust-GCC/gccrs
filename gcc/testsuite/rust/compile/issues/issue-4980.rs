// { dg-additional-options "-w -frust-compile-until=privacy" }
#![feature(no_core, lang_items)]
#![no_core]

// Rust-GCC/gccrs#4980: a bound check in H::g probed the derived Clone impl of
// CharSliceSearcher before the struct itself was visited. Resolving that impl
// pushed a lifetime resolver that was never popped, so the struct's own 'a
// and 'b could not be found once its field was resolved.
// Stops before codegen: the derived clone of a struct with an associated
// type field still ICEs there, which is a separate bug.

#[lang = "sized"]
pub trait Sized {}

pub mod clone {
    #[lang = "clone"]
    pub trait Clone: crate::Sized {
        fn clone(&self) -> Self;
    }
}

impl clone::Clone for u32 {
    fn clone(&self) -> Self {
        *self
    }
}

fn need<X: clone::Clone>(_x: X) {}

pub struct H;

impl H {
    pub fn g(x: u32) {
        need(x);
    }
}

pub trait Pattern<'a> {
    type Searcher;
}

pub trait MultiCharEq {}

struct MultiCharEqPattern<C: MultiCharEq>(C);

struct MultiCharEqSearcher<'a, C: MultiCharEq> {
    marker: &'a (),
    c: *const C,
}

impl<'a, C: MultiCharEq> Pattern<'a> for MultiCharEqPattern<C> {
    type Searcher = MultiCharEqSearcher<'a, C>;
}

impl<'x> MultiCharEq for &'x u8 {}

impl<'a, C: MultiCharEq> clone::Clone for MultiCharEqSearcher<'a, C> {
    fn clone(&self) -> Self {
        MultiCharEqSearcher {
            marker: self.marker,
            c: self.c,
        }
    }
}

#[derive(Clone)]
pub struct CharSliceSearcher<'a, 'b>(<MultiCharEqPattern<&'b u8> as Pattern<'a>>::Searcher);
