// Test that we still check constants are well-formed, even when we there's no
// type annotation to check.

const FUN: fn(&'static ()) = |_| {};
struct A;
impl A {
    const ASSOCIATED_FUN: fn(&'static ()) = |_| {};
}

struct B<'a>(&'a ());
impl B<'static> {
    const ALSO_ASSOCIATED_FUN: fn(&'static ()) = |_| {};
}

trait Z: 'static {
    const TRAIT_ASSOCIATED_FUN: fn(&'static Self) = |_| ();
}

impl Z for () {}

fn main() {
    let x = ();
    FUN(&x);                        // { dg-error ".E0597." "" { target *-*-* } }
    A::ASSOCIATED_FUN(&x);          // { dg-error ".E0597." "" { target *-*-* } }
    B::ALSO_ASSOCIATED_FUN(&x);     // { dg-error ".E0597." "" { target *-*-* } }
    <_>::TRAIT_ASSOCIATED_FUN(&x);  // { dg-error ".E0597." "" { target *-*-* } }
}

