#![deny(dead_code)]

struct MyFoo;

impl MyFoo {
    const BAR: u32 = 1;
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() {
    let _: MyFoo = MyFoo;
}

