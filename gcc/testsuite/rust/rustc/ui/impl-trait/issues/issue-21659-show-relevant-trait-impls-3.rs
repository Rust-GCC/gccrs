trait Foo<A> {
    fn foo(&self, a: A) -> A {
        a
    }
}

trait NotRelevant<A> {
    fn nr(&self, a: A) -> A {
        a
    }
}

struct Bar;

impl NotRelevant<usize> for Bar {}

fn main() {
    let f1 = Bar;

    f1.foo(1usize);
// { dg-error ".E0599." "" { target *-*-* } .-1 }
}

