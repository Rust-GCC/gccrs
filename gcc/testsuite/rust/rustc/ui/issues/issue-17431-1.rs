struct Foo { foo: Option<Option<Foo>> }
// { dg-error ".E0072." "" { target *-*-* } .-1 }

impl Foo { fn bar(&self) {} }

fn main() {}

