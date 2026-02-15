#[doc] // { dg-error "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-1 }
#[ignore()] // { dg-error "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-1 }
#[inline = ""] // { dg-error "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-1 }
#[link] // { dg-error "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-1 }
#[link = ""] // { dg-error "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-1 }

fn main() {}

