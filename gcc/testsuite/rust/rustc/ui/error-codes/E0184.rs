#[derive(Copy)] // { dg-error ".E0184." "" { target *-*-* } }
struct Foo;

impl Drop for Foo {
    fn drop(&mut self) {
    }
}

fn main() {
}

