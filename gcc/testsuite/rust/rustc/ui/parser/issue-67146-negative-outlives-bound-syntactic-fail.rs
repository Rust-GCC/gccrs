// run-rustfix

// In this regression test for #67146, we check that the
// negative outlives bound `!'a` is rejected by the parser.
// This regression was first introduced in PR #57364.

fn main() {}

pub fn f1<T: !'static>() {}
// { dg-error "" "" { target *-*-* } .-1 }
pub fn f2<'a, T: Ord + !'a>() {}
// { dg-error "" "" { target *-*-* } .-1 }
pub fn f3<'a, T: !'a + Ord>() {}
// { dg-error "" "" { target *-*-* } .-1 }

