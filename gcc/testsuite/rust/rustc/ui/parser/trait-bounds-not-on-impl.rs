trait Foo {}

struct Bar;

impl Foo + Owned for Bar {} // { dg-error "" "" { target *-*-* } }

fn main() { }

