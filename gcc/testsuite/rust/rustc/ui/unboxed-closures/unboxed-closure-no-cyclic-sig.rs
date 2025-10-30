// Test that unboxed closures cannot capture their own type.
//
// Also regression test for issue #21410.

fn g<F>(_: F) where F: FnOnce(Option<F>) {}

fn main() {
    g(|_| {  }); // { dg-error ".E0644." "" { target *-*-* } }
}

