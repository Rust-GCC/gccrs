trait Foo {
    fn dummy(&self) { }
}

fn a(_x: Box<dyn Foo + Send>) {
}

fn c(x: Box<dyn Foo + Sync + Send>) {
    a(x);
}

fn d(x: Box<dyn Foo>) {
    a(x); // { dg-error ".E0308." "" { target *-*-* } }
}

fn main() { }

