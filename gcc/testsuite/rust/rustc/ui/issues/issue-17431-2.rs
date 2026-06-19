struct Baz { q: Option<Foo> }
// { dg-error ".E0072." "" { target *-*-* } .-1 }

struct Foo { q: Option<Baz> }
// { dg-error ".E0072." "" { target *-*-* } .-1 }

impl Foo { fn bar(&self) {} }

fn main() {}

