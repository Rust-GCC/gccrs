#[link(name="foo", cfg())] // { dg-error "" "" { target *-*-* } }
extern {}

fn main() {}

