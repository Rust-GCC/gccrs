struct Foo;
struct Bar;

impl Foo for Bar {} // { dg-error ".E0404." "" { target *-*-* } }

fn main() {}

fn baz<T: Foo>(_: T) {} // { dg-error ".E0404." "" { target *-*-* } }

