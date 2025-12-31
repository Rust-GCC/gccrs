// checks that this attribute is caught on non-macro items.
// this needs a different test since this is done after expansion

#[allow_internal_unstable()] // { dg-error ".E0658." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
struct S;

fn main() {}

