enum Foo {
    Variant { x: usize }
}

fn main() {
    let f = Foo::Variant(42);
// { dg-error ".E0423." "" { target *-*-* } .-1 }
}

