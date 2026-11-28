// Tests that auto-ref can't create mutable aliases to immutable memory.

struct Foo {
    x: isize
}

impl Foo {
    pub fn printme(&mut self) {
        println!("{}", self.x);
    }
}

fn main() {
    let x = Foo { x: 3 };
    x.printme();    // { dg-error ".E0596." "" { target *-*-* } }
}

