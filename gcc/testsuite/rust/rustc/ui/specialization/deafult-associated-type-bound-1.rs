// Check that we check that default associated types satisfy the required
// bounds on them.
// ignore-compare-mode-chalk

#![feature(specialization)]
// { dg-warning "" "" { target *-*-* } .-1 }

trait X {
    type U: Clone;
    fn unsafe_clone(&self, x: Option<&Self::U>) {
        x.cloned();
    }
}

// We cannot normalize `<T as X>::U` to `str` here, because the default could
// be overridden. The error here must therefore be found by a method other than
// normalization.
impl<T> X for T {
    default type U = str;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

pub fn main() {
    1.unsafe_clone(None);
}

