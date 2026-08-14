use a::Foo;

mod a {
    pub struct Foo {
        x: isize
    }

    pub fn make() -> Foo {
        Foo { x: 3 }
    }
}

fn main() {
    match a::make() {
        Foo { x: _ } => {}  // { dg-error ".E0451." "" { target *-*-* } }
    }
}

