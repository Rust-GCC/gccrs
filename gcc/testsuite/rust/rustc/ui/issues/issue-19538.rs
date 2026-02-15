trait Foo {
    fn foo<T>(&self, val: T);
}

trait Bar: Foo { }

pub struct Thing;

impl Foo for Thing {
    fn foo<T>(&self, val: T) { }
}

impl Bar for Thing { }

fn main() {
    let mut thing = Thing;
    let test: &mut dyn Bar = &mut thing;
// { dg-error ".E0038." "" { target *-*-* } .-1 }
// { dg-error ".E0038." "" { target *-*-* } .-2 }
}

