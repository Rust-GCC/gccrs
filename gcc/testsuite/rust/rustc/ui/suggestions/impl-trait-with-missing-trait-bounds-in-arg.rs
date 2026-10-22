// run-rustfix

trait Foo {}

trait Bar {
    fn hello(&self) {}
}

struct S;

impl Foo for S {}
impl Bar for S {}

fn test(foo: impl Foo) {
    foo.hello(); // { dg-error ".E0599." "" { target *-*-* } }
}

fn main() {
    test(S);
}

