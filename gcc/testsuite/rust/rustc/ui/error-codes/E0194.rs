trait Foo<T> {
    fn do_something(&self) -> T;
    fn do_something_else<T: Clone>(&self, bar: T);
// { dg-error ".E0403." "" { target *-*-* } .-1 }
}

fn main() {
}

