#[link()] // { dg-error ".E0459." "" { target *-*-* } }
#[link(name = "")] // { dg-error ".E0454." "" { target *-*-* } }
#[link(name = "foo")]
#[link(name = "foo", kind = "bar")] // { dg-error ".E0458." "" { target *-*-* } }
extern {}

fn main() {}

