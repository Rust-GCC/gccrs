// Make sure we don't ICE when making an overloaded call with the
// wrong arity.

fn _foo<F: Fn()> (f: F) {
    |t| f(t); // { dg-error ".E0057." "" { target *-*-* } }
}

fn main() {}

