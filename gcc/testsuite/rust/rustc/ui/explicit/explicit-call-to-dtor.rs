struct Foo {
    x: isize
}

impl Drop for Foo {
    fn drop(&mut self) {
        println!("kaboom");
    }
}

fn main() {
    let x = Foo { x: 3 };
    x.drop();   // { dg-error ".E0040." "" { target *-*-* } }
}

