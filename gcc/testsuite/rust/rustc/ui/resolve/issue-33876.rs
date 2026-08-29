use std::any::Any;

struct Foo;

trait Bar {}

impl Bar for Foo {}

fn main() {
    let any: &dyn Any = &Bar; // { dg-error ".E0423." "" { target *-*-* } }
    if any.is::<u32>() { println!("u32"); }
}

