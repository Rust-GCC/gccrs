#[derive(Copy, Clone)]
union Foo {
    a: isize,
    b: (),
}

enum Bar {
    Boo = [unsafe { Foo { b: () }.a }; 4][3],
// { dg-error ".E0080." "" { target *-*-* } .-1 }
}

fn main() {
    assert_ne!(Bar::Boo as isize, 0);
}

