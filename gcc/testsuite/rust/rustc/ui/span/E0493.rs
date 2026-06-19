struct Foo {
    a: u32
}

impl Drop for Foo {
    fn drop(&mut self) {}
}

struct Bar {
    a: u32
}

impl Drop for Bar {
    fn drop(&mut self) {}
}

const F : Foo = (Foo { a : 0 }, Foo { a : 1 }).1;
// { dg-error ".E0493." "" { target *-*-* } .-1 }

fn main() {
}

