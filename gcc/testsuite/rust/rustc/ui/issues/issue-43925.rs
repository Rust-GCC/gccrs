#[link(name="foo", cfg("rlib"))] // { dg-error "" "" { target *-*-* } }
extern {}

fn main() {}

