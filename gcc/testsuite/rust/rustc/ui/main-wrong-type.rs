struct S {
    x: isize,
    y: isize,
}

fn main(foo: S) {
// { dg-error ".E0580." "" { target *-*-* } .-1 }
}

