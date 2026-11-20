#[derive(Send)]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
struct Test;

#[derive(Sync)]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
struct Test1;

pub fn main() {}

