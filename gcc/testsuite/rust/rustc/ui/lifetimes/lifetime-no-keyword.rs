fn foo<'a>(a: &'a isize) { }
fn bar(a: &'static isize) { }
fn baz<'let>(a: &'let isize) { } // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
fn zab<'self>(a: &'self isize) { } // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
fn main() { }

