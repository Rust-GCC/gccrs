trait Foo {
    fn foo(self: Box<Self>);
}

impl Foo for isize {
    fn foo(self: Box<isize>) { }
}

fn main() {
    (&5isize as &dyn Foo).foo();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
}

