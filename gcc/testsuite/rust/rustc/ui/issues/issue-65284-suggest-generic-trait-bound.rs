trait Foo {
    fn foo(&self);
}

trait Bar {}

fn do_stuff<T : Bar>(t : T) {
    t.foo() // { dg-error ".E0599." "" { target *-*-* } }
}

fn main() {}

