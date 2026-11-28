trait Foo {
    type Item;
}

trait Bar: Foo {}

struct S;

impl Foo for S {
    type Item = i32;
}
impl Bar for S {}

struct T;

impl Foo for T {
    type Item = u32;
}
impl Bar for T {}

fn bar() -> impl Bar {
    T
}

fn baz() -> impl Bar<Item = i32> {
// { dg-error ".E0271." "" { target *-*-* } .-1 }
    bar()
}

fn main() {
    let _ = baz();
}

