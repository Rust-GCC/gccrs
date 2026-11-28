#[derive(Copy(Bad))]
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
struct Test1;

#[derive(Copy="bad")]
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
struct Test2;

#[derive] // { dg-error "" "" { target *-*-* } }
struct Test4;

fn main() {}

