enum Foo {
    Bar(isize)
}

fn main() {
    match Foo::Bar(1) {
        Foo { i } => () // { dg-error ".E0574." "" { target *-*-* } }
    }
}

