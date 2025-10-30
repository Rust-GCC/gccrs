struct Foo {
    a: usize,
}

fn main() {
    let Foo {
        a: _,
        a: _
// { dg-error ".E0025." "" { target *-*-* } .-1 }
    } = Foo { a: 29 };

    let Foo {
        a,
        a: _
// { dg-error ".E0025." "" { target *-*-* } .-1 }
    } = Foo { a: 29 };

    let Foo {
        a,
        a: _,
// { dg-error ".E0025." "" { target *-*-* } .-1 }
        a: x
// { dg-error ".E0025." "" { target *-*-* } .-1 }
    } = Foo { a: 29 };
}

