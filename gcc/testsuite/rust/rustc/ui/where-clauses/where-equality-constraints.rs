fn f() where u8 = u16 {}
// { dg-error "" "" { target *-*-* } .-1 }
fn g() where for<'a> &'static (u8,) == u16, {}
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

