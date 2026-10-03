// run-rustfix
#![allow(dead_code, unused_variables)]

pub mod foo {
    #[derive(Default)]
    pub struct Foo { invisible: bool, }

    #[derive(Default)]
    pub struct Bar { pub visible: bool, invisible: bool, }
}

fn main() {
    let foo::Foo {} = foo::Foo::default();
// { dg-error "" "" { target *-*-* } .-1 }

    let foo::Bar { visible } = foo::Bar::default();
// { dg-error "" "" { target *-*-* } .-1 }
}

