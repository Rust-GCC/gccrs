use std::sync::Mutex;

enum Foo { X(Mutex<Option<Foo>>) }
// { dg-error ".E0072." "" { target *-*-* } .-1 }

impl Foo { fn bar(self) {} }

fn main() {}

