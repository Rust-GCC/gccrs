#![deny(non_snake_case)]
#![allow(unused_variables)]
#![allow(dead_code)]

enum Foo {
    Bad {
        lowerCamelCaseName: bool,
// { dg-error "" "" { target *-*-* } .-1 }
    },
    Good {
        snake_case_name: bool,
    },
}

fn main() {
    let b = Foo::Bad { lowerCamelCaseName: true };

    match b {
        Foo::Bad { lowerCamelCaseName } => {}
        Foo::Good { snake_case_name: lowerCamelCaseBinding } => { }
// { dg-error "" "" { target *-*-* } .-1 }
    }

    if let Foo::Good { snake_case_name: anotherLowerCamelCaseBinding } = b { }
// { dg-error "" "" { target *-*-* } .-1 }

    if let Foo::Bad { lowerCamelCaseName: yetAnotherLowerCamelCaseBinding } = b { }
// { dg-error "" "" { target *-*-* } .-1 }
}

