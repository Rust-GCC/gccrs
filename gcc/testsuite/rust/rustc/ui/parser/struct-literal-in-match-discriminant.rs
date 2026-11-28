struct Foo {
    x: isize,
}

fn main() {
    match Foo { // { dg-error "" "" { target *-*-* } }
        x: 3
    } {
        Foo {
            x: x
        } => {}
    }
}

