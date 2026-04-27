#![feature(negative_impls)]
#![feature(specialization)]
// { dg-warning "" "" { target *-*-* } .-1 }

trait MyTrait {
    type Foo;
}

default impl !MyTrait for u32 {} // { dg-error ".E0750." "" { target *-*-* } }

fn main() {}

