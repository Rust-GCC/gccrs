// Check that we report an error if an upcast box is moved twice.

trait Foo { fn dummy(&self); }

fn consume(_: Box<dyn Foo>) {
}

fn foo(b: Box<dyn Foo + Send>) {
    consume(b);
    consume(b); // { dg-error ".E0382." "" { target *-*-* } }
}

fn main() {
}

