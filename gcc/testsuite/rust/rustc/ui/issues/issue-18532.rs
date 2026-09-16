// Test that overloaded call parameter checking does not ICE
// when a type error or unconstrained type variable propagates
// into it.

fn main() {
    (return)((),()); // { dg-error ".E0618." "" { target *-*-* } }
}

