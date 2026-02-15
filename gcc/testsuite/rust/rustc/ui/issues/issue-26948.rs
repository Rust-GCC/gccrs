fn main() {
    enum Foo { A { x: u32 } }
    let orig = Foo::A { x: 5 };
    Foo::A { x: 6, ..orig };
// { dg-error ".E0436." "" { target *-*-* } .-1 }
}

