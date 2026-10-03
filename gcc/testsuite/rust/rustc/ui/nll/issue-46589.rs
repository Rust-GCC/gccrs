// This tests passes in Polonius mode, so is skipped in the automated compare-mode.
// We will manually check it passes in Polonius tests, as we can't have a test here
// which conditionally passes depending on a test revision/compile-flags.

// ignore-compare-mode-polonius

struct Foo;

impl Foo {
    fn get_self(&mut self) -> Option<&mut Self> {
        Some(self)
    }

    fn new_self(&mut self) -> &mut Self {
        self
    }

    fn trigger_bug(&mut self) {
        let other = &mut (&mut *self);

        *other = match (*other).get_self() {
            Some(s) => s,
            None => (*other).new_self()
// { dg-error ".E0499." "" { target *-*-* } .-1 }
        };

        let c = other;
    }
}

fn main() {}

