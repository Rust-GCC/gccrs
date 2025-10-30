#![feature(box_patterns)]
#![feature(box_syntax)]

struct S {
    x: Box<E>
}

enum E {
    Foo(Box<S>),
    Bar(Box<isize>),
    Baz
}

fn f<G>(s: &S, g: G) where G: FnOnce(&S) {
    g(s)
}

fn main() {
    let s = S { x: box E::Bar(box 42) };
    loop {
        f(&s, |hellothere| {
            match hellothere.x { // { dg-error ".E0507." "" { target *-*-* } }
                box E::Foo(_) => {}
                box E::Bar(x) => println!("{}", x.to_string()),
                box E::Baz => {}
            }
        })
    }
}

