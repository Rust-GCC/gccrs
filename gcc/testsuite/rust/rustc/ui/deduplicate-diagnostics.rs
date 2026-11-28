// revisions: duplicate deduplicate
//[deduplicate] compile-flags: -Z deduplicate-diagnostics=yes

#[derive(Unresolved)] // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
struct S;

#[deny("literal")] // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
fn main() {}

