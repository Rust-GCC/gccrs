#![feature(negative_impls)]
#![feature(specialization)] // { dg-warning "" "" { target *-*-* } }

struct S;
struct Z;

default impl S {} // { dg-error "" "" { target *-*-* } }

default unsafe impl Send for S {} // { dg-error "" "" { target *-*-* } }
default impl !Send for Z {} // { dg-error ".E0750." "" { target *-*-* } }
// { dg-error ".E0750." "" { target *-*-* } .-1 }

trait Tr {}
default impl !Tr for S {} // { dg-error ".E0750." "" { target *-*-* } }

fn main() {}

