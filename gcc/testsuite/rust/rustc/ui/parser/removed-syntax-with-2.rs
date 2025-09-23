fn main() {
    struct S {
        foo: (),
        bar: (),
    }

    let a = S { foo: (), bar: () };
    let b = S { foo: (), with a };
// { dg-error ".E0063." "" { target *-*-* } .-1 }
// { dg-error ".E0063." "" { target *-*-* } .-2 }
}

