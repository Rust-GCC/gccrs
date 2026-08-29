trait Foo {
    fn method(&self) {}
}

fn call_method<T: std::fmt::Debug>(x: &T) {
    x.method() // { dg-error ".E0599." "" { target *-*-* } }
}

fn call_method_2<T>(x: T) {
    x.method() // { dg-error ".E0599." "" { target *-*-* } }
}

fn main() {}

