#![feature(specialization)]
#![allow(incomplete_features)]

struct MyStruct {}

trait MyTrait {
    type MyType: Default;
}

impl MyTrait for i32 {
    default type MyType = MyStruct;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn main() {
    let _x: <i32 as MyTrait>::MyType = <i32 as MyTrait>::MyType::default();
}

