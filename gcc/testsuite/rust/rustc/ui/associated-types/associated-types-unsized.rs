// run-rustfix
#![allow(dead_code, unused_variables)]

trait Get {
    type Value: ?Sized;
    fn get(&self) -> <Self as Get>::Value;
}

fn foo<T:Get>(t: T) {
    let x = t.get(); // { dg-error ".E0277." "" { target *-*-* } }
}

fn main() {
}

