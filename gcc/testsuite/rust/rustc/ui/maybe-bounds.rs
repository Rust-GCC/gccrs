trait Tr: ?Sized {}
// { dg-error "" "" { target *-*-* } .-1 }

type A1 = dyn Tr + (?Sized);
// { dg-error "" "" { target *-*-* } .-1 }
type A2 = dyn for<'a> Tr + (?Sized);
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

