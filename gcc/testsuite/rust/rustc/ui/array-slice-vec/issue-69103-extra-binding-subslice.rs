// We used to not lower the extra `b @ ..` into `b @ _` which meant that no type
// was registered for the binding `b` although it passed through resolve.
// This resulted in an ICE (#69103).

fn main() {
    let [a @ .., b @ ..] = &mut [1, 2];
// { dg-error "" "" { target *-*-* } .-1 }
    b;

    let [.., c @ ..] = [1, 2];
// { dg-error "" "" { target *-*-* } .-1 }
    c;

    // This never ICEd, but let's make sure it won't regress either.
    let (.., d @ ..) = (1, 2);
// { dg-error "" "" { target *-*-* } .-1 }
    d;
}

