// { dg-additional-options "-w" }
#![feature(no_core, lang_items, unboxed_closures)]
#![no_core]

// Rust-GCC/gccrs#5074: CharPredicateSearcher gets resolved while the impl of
// Pattern for MultiCharEqPattern<C> is being checked, and its field used to
// come back as MultiCharEqSearcher<C> with the impl's C instead of F.

#[lang = "sized"]
pub trait Sized {}

#[lang = "fn_once"]
#[rustc_paren_sugar]
pub trait FnOnce<Args> {
    #[lang = "fn_once_output"]
    type Output;
    extern "rust-call" fn call_once(self, args: Args) -> Self::Output;
}
#[lang = "fn_mut"]
#[rustc_paren_sugar]
pub trait FnMut<Args>: FnOnce<Args> {
    extern "rust-call" fn call_mut(&mut self, args: Args) -> Self::Output;
}

pub unsafe trait Searcher<'a> {
    fn haystack(&self) -> &'a str;
}

pub trait Pattern<'a> {
    type Searcher: Searcher<'a>;
    fn into_searcher(self, haystack: &'a str) -> Self::Searcher;
}

pub trait MultiCharEq {
    fn matches(&mut self, c: char) -> bool;
}

impl<F> MultiCharEq for F
where
    F: FnMut(char) -> bool,
{
    fn matches(&mut self, c: char) -> bool {
        (*self)(c)
    }
}

struct MultiCharEqPattern<C: MultiCharEq>(C);

struct MultiCharEqSearcher<'a, C: MultiCharEq> {
    char_eq: C,
    haystack: &'a str,
}

impl<'a, C: MultiCharEq> Pattern<'a> for MultiCharEqPattern<C> {
    type Searcher = MultiCharEqSearcher<'a, C>;
    fn into_searcher(self, haystack: &'a str) -> MultiCharEqSearcher<'a, C> {
        MultiCharEqSearcher { char_eq: self.0, haystack }
    }
}

unsafe impl<'a, C: MultiCharEq> Searcher<'a> for MultiCharEqSearcher<'a, C> {
    fn haystack(&self) -> &'a str {
        self.haystack
    }
}

pub struct CharPredicateSearcher<'a, F>(<MultiCharEqPattern<F> as Pattern<'a>>::Searcher)
where
    F: FnMut(char) -> bool;

unsafe impl<'a, F> Searcher<'a> for CharPredicateSearcher<'a, F>
where
    F: FnMut(char) -> bool,
{
    fn haystack(&self) -> &'a str {
        self.0.haystack()
    }
}

impl<'a, F> Pattern<'a> for F
where
    F: FnMut(char) -> bool,
{
    type Searcher = CharPredicateSearcher<'a, F>;
    fn into_searcher(self, haystack: &'a str) -> CharPredicateSearcher<'a, F> {
        CharPredicateSearcher(MultiCharEqPattern(self).into_searcher(haystack))
    }
}
