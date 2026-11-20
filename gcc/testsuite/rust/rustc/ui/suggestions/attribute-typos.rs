#[deprcated] // { dg-error "" "" { target *-*-* } }
fn foo() {}

#[tests] // { dg-error "" "" { target *-*-* } }
fn bar() {}

#[rustc_err]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

fn main() {}

