struct Foo;

impl Drop for Foo {
    fn drop(&mut self) {}
}

fn main() {
    Drop::drop(&mut Foo) // { dg-error ".E0040." "" { target *-*-* } }
}

