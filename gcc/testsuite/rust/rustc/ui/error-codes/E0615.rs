struct Foo {
    x: u32,
}

impl Foo {
    fn method(&self) {}
}

fn main() {
    let f = Foo { x: 0 };
    f.method; // { dg-error ".E0615." "" { target *-*-* } }
}

