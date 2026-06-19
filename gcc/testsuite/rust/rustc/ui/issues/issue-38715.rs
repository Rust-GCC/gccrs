#[macro_export]
macro_rules! foo { ($i:ident) => {} }

#[macro_export]
macro_rules! foo { () => {} } // { dg-error ".E0428." "" { target *-*-* } }

fn main() {}

