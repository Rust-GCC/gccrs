#![warn(unused)]
#![allow(dead_code)]
#![deny(non_snake_case)]

mod foo {
    pub enum Foo { Foo }
}

struct Something {
    X: usize // { dg-error "" "" { target *-*-* } }
}

fn test(Xx: usize) { // { dg-error "" "" { target *-*-* } }
    println!("{}", Xx);
}

fn main() {
    let Test: usize = 0; // { dg-error "" "" { target *-*-* } }
    println!("{}", Test);

    match foo::Foo::Foo {
        Foo => {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-warning "" "" { target *-*-* } .-3 }
    }

    let Foo = foo::Foo::Foo;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-warning "" "" { target *-*-* } .-3 }

    fn in_param(Foo: foo::Foo) {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-warning "" "" { target *-*-* } .-3 }

    test(1);

    let _ = Something { X: 0 };
}

