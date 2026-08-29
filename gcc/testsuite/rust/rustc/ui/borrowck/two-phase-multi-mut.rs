struct Foo {
}

impl Foo {
    fn method(&mut self, foo: &mut Foo) {
    }
}

fn main() {
    let mut foo = Foo { };
    foo.method(&mut foo);
// { dg-error ".E0499." "" { target *-*-* } .-1 }
// { dg-error ".E0499." "" { target *-*-* } .-2 }
}

