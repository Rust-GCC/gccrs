trait Foo {
    fn dummy(&self) { }
}

struct A;

impl Foo for A {}

struct B<'a>(&'a (dyn Foo + 'a));

fn foo<'a>(a: &dyn Foo) -> B<'a> {
    B(a)    // { dg-error ".E0621." "" { target *-*-* } }
}

fn main() {
    let _test = foo(&A);
}

