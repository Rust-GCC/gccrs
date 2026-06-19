// Test that `dyn ... + ?Sized + ...` is okay (though `?Sized` has no effect in trait objects).

trait Foo {}

type _0 = dyn ?Sized + Foo;
// { dg-error "" "" { target *-*-* } .-1 }

type _1 = dyn Foo + ?Sized;
// { dg-error "" "" { target *-*-* } .-1 }

type _2 = dyn Foo + ?Sized + ?Sized;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

type _3 = dyn ?Sized + Foo;
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

