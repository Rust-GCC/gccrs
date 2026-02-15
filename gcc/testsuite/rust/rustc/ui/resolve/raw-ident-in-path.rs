// Regression test for issue #63882.

type A = crate::r#break; // { dg-error ".E0412." "" { target *-*-* } }

fn main() {}

