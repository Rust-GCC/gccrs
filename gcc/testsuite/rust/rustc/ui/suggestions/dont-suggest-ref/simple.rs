#[derive(Clone)]
enum Either {
    One(X),
    Two(X),
}

#[derive(Clone)]
struct X(Y);

#[derive(Clone)]
struct Y;

pub fn main() {
    let e = Either::One(X(Y));
    let mut em = Either::One(X(Y));

    let r = &e;
    let rm = &mut Either::One(X(Y));

    let x = X(Y);
    let mut xm = X(Y);

    let s = &x;
    let sm = &mut X(Y);

    let ve = vec![Either::One(X(Y))];

    let vr = &ve;
    let vrm = &mut vec![Either::One(X(Y))];

    let vx = vec![X(Y)];

    let vs = &vx;
    let vsm = &mut vec![X(Y)];

    // move from Either/X place

    let X(_t) = *s;
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
    if let Either::One(_t) = *r { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
    while let Either::One(_t) = *r { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
    match *r {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
        Either::One(_t)
        | Either::Two(_t) => (),
    }
    match *r {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
        Either::One(_t) => (),
        Either::Two(ref _t) => (),
        // FIXME: should suggest removing `ref` too
    }

    let X(_t) = *sm;
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
    if let Either::One(_t) = *rm { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
    while let Either::One(_t) = *rm { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
    match *rm {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
        Either::One(_t)
        | Either::Two(_t) => (),
    }
    match *rm {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
        Either::One(_t) => (),
        Either::Two(ref _t) => (),
        // FIXME: should suggest removing `ref` too
    }
    match *rm {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
        Either::One(_t) => (),
        Either::Two(ref mut _t) => (),
        // FIXME: should suggest removing `ref` too
    }

    let X(_t) = vs[0];
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
    if let Either::One(_t) = vr[0] { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
    while let Either::One(_t) = vr[0] { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
    match vr[0] {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
        Either::One(_t)
        | Either::Two(_t) => (),
    }
    match vr[0] {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
        Either::One(_t) => (),
        Either::Two(ref _t) => (),
        // FIXME: should suggest removing `ref` too
    }

    let X(_t) = vsm[0];
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
    if let Either::One(_t) = vrm[0] { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
    while let Either::One(_t) = vrm[0] { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
    match vrm[0] {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
        Either::One(_t)
        | Either::Two(_t) => (),
    }
    match vrm[0] {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
        Either::One(_t) => (),
        Either::Two(ref _t) => (),
        // FIXME: should suggest removing `ref` too
    }
    match vrm[0] {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
        Either::One(_t) => (),
        Either::Two(ref mut _t) => (),
        // FIXME: should suggest removing `ref` too
    }

    // move from &Either/&X place

    let &X(_t) = s;
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
    if let &Either::One(_t) = r { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
    while let &Either::One(_t) = r { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
    match r {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
        &Either::One(_t)
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
        | &Either::Two(_t) => (),
        // FIXME: would really like a suggestion here too
    }
    match r {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
        &Either::One(_t) => (),
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
        &Either::Two(ref _t) => (),
    }
    match r {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
        &Either::One(_t) => (),
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
        Either::Two(_t) => (),
    }
    fn f1(&X(_t): &X) { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }

    let &mut X(_t) = sm;
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
    if let &mut Either::One(_t) = rm { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
    while let &mut Either::One(_t) = rm { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
    match rm {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
        &mut Either::One(_t) => (),
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
        &mut Either::Two(_t) => (),
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
    }
    match rm {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
        &mut Either::One(_t) => (),
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
        &mut Either::Two(ref _t) => (),
    }
    match rm {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
        &mut Either::One(_t) => (),
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
        &mut Either::Two(ref mut _t) => (),
    }
    match rm {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
        &mut Either::One(_t) => (),
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
        Either::Two(_t) => (),
    }
    fn f2(&mut X(_t): &mut X) { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }

    // move from tuple of &Either/&X

    // FIXME: These should have suggestions.

    let (&X(_t),) = (&x.clone(),);
// { dg-error ".E0507." "" { target *-*-* } .-1 }
    if let (&Either::One(_t),) = (&e.clone(),) { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
    while let (&Either::One(_t),) = (&e.clone(),) { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
    match (&e.clone(),) {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
        (&Either::One(_t),)
        | (&Either::Two(_t),) => (),
    }
    fn f3((&X(_t),): (&X,)) { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }

    let (&mut X(_t),) = (&mut xm.clone(),);
// { dg-error ".E0507." "" { target *-*-* } .-1 }
    if let (&mut Either::One(_t),) = (&mut em.clone(),) { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
    while let (&mut Either::One(_t),) = (&mut em.clone(),) { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
    match (&mut em.clone(),) {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
        (&mut Either::One(_t),) => (),
        (&mut Either::Two(_t),) => (),
    }
    fn f4((&mut X(_t),): (&mut X,)) { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }

    // move from &Either/&X value

    let &X(_t) = &x;
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
    if let &Either::One(_t) = &e { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
    while let &Either::One(_t) = &e { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
    match &e {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
        &Either::One(_t)
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
        | &Either::Two(_t) => (),
        // FIXME: would really like a suggestion here too
    }
    match &e {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
        &Either::One(_t) => (),
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
        &Either::Two(ref _t) => (),
    }
    match &e {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
        &Either::One(_t) => (),
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
        Either::Two(_t) => (),
    }

    let &mut X(_t) = &mut xm;
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
    if let &mut Either::One(_t) = &mut em { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
    while let &mut Either::One(_t) = &mut em { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
    match &mut em {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
        &mut Either::One(_t)
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
        | &mut Either::Two(_t) => (),
        // FIXME: would really like a suggestion here too
    }
    match &mut em {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
        &mut Either::One(_t) => (),
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
        &mut Either::Two(ref _t) => (),
    }
    match &mut em {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
        &mut Either::One(_t) => (),
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
        &mut Either::Two(ref mut _t) => (),
    }
    match &mut em {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
        &mut Either::One(_t) => (),
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
        Either::Two(_t) => (),
    }
}

