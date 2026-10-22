mod a {
    pub struct Foo {
        x: isize
    }
}

fn main() {
    let s = a::Foo { x: 1 };    // { dg-error ".E0451." "" { target *-*-* } }
}

