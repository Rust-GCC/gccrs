// Tests that default impls do not have to supply all items but regular impls do.

#![feature(specialization)] // { dg-warning "" "" { target *-*-* } }

trait Foo {
    fn foo_one(&self) -> &'static str;
    fn foo_two(&self) -> &'static str;
}

struct MyStruct;

default impl<T> Foo for T {
    fn foo_one(&self) -> &'static str {
        "generic"
    }
}

impl Foo for MyStruct {}
// { dg-error ".E0046." "" { target *-*-* } .-1 }

fn main() {
    println!("{}", MyStruct.foo_one());
}

