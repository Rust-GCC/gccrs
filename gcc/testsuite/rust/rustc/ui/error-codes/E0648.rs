#[export_name="\0foo"] // { dg-error ".E0648." "" { target *-*-* } }
pub fn bar() {}

fn main() {}

