#![feature(negative_impls)]

trait MyTrait {
    type Foo;
}

impl !MyTrait for u32 {
    type Foo = i32; // { dg-error ".E0749." "" { target *-*-* } }
}

fn main() {}

