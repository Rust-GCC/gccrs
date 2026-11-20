// run-rustfix

#![allow(nonstandard_style, unused_variables, unused_mut)]
#![deny(non_shorthand_field_patterns)]

struct Foo {
    x: isize,
    y: isize,
}

fn main() {
    {
        let Foo {
            x: x, // { dg-error "" "" { target *-*-* } }
            y: ref y, // { dg-error "" "" { target *-*-* } }
        } = Foo { x: 0, y: 0 };

        let Foo {
            x,
            ref y,
        } = Foo { x: 0, y: 0 };
    }

    {
        const x: isize = 1;

        match (Foo { x: 1, y: 1 }) {
            Foo { x: x, ..} => {},
            _ => {},
        }
    }

    {
        struct Bar {
            x: x,
        }

        struct x;

        match (Bar { x: x }) {
            Bar { x: x } => {},
        }
    }

    {
        struct Bar {
            x: Foo,
        }

        enum Foo { x }

        match (Bar { x: Foo::x }) {
            Bar { x: Foo::x } => {},
        }
    }

    {
        struct Baz {
            x: isize,
            y: isize,
            z: isize,
        }

        let Baz {
            x: mut x, // { dg-error "" "" { target *-*-* } }
            y: ref y, // { dg-error "" "" { target *-*-* } }
            z: ref mut z, // { dg-error "" "" { target *-*-* } }
        } = Baz { x: 0, y: 0, z: 0 };
    }
}

