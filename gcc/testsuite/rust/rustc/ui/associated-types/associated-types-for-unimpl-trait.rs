// run-rustfix
#![allow(unused_variables)]

trait Get {
    type Value;
    fn get(&self) -> <Self as Get>::Value;
}

trait Other {
    fn uhoh<U:Get>(&self, foo: U, bar: <Self as Get>::Value) {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn main() {
}

