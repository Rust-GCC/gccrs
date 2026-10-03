struct A;

impl A {
    fn foo(self: Box<Self>) {}
}

fn main() {
    A.foo(); // { dg-error ".E0599." "" { target *-*-* } }
}

