// gate-test-dropck_eyepatch

// Check that `may_dangle` is rejected if `dropck_eyepatch` feature gate is absent.

struct Pt<A>(A);
unsafe impl<#[may_dangle] A> Drop for Pt<A> {
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    fn drop(&mut self) { }
}

fn main() {}

