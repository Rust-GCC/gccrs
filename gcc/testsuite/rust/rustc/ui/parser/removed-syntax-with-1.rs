fn main() {
    struct S {
        foo: (),
        bar: (),
    }

    let a = S { foo: (), bar: () };
    let b = S { foo: () with a, bar: () };
// { dg-error "" "" { target *-*-* } .-1 }
}

