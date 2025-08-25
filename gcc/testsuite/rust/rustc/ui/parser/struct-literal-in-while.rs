struct Foo {
    x: isize,
}

impl Foo {
    fn hi(&self) -> bool {
        true
    }
}

fn main() {
    while Foo { // { dg-error "" "" { target *-*-* } }
        x: 3
    }.hi() {
        println!("yo");
    }
}

