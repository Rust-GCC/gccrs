struct Foo {
    x: u32,
}
struct Bar;

fn main() {
    let x = Foo { x: 0 };
    let _ = x.foo; // { dg-error ".E0609." "" { target *-*-* } }

    let y = Bar;
    y.1; // { dg-error ".E0609." "" { target *-*-* } }
}

