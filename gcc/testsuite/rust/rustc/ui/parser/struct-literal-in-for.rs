struct Foo {
    x: isize,
}

impl Foo {
    fn hi(&self) -> bool {
        true
    }
}

fn main() {
    for x in Foo { // { dg-error ".E0277." "" { target *-*-* } }
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    }.hi() {
        println!("yo");
    }
}

