use std::any::Any;

fn foo<T: Any>(value: &T) -> Box<dyn Any> {
    Box::new(value) as Box<dyn Any> // { dg-error ".E0759." "" { target *-*-* } }
}

fn main() {
    let _ = foo(&5);
}

