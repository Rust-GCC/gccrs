struct Foo {
    x: isize,
    y: isize,
}

fn main() {
    let f = Foo {
        x: 1,
        y: 2,
    };
    f.x::<isize>;
// { dg-error "" "" { target *-*-* } .-1 }
    f.x::<>;
// { dg-error "" "" { target *-*-* } .-1 }
    f.x::();
// { dg-error "" "" { target *-*-* } .-1 }
}

