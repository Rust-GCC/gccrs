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

    // test for duplicate suggestions

    let &(X(_t), X(_u)) = &(x.clone(), x.clone());
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
    if let &(Either::One(_t), Either::Two(_u)) = &(e.clone(), e.clone()) { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
    while let &(Either::One(_t), Either::Two(_u)) = &(e.clone(), e.clone()) { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
    match &(e.clone(), e.clone()) {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
        &(Either::One(_t), Either::Two(_u)) => (),
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
        &(Either::Two(_t), Either::One(_u)) => (),
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
        _ => (),
    }
    match &(e.clone(), e.clone()) {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
        &(Either::One(_t), Either::Two(_u))
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
        | &(Either::Two(_t), Either::One(_u)) => (),
        // FIXME: would really like a suggestion here too
        _ => (),
    }
    match &(e.clone(), e.clone()) {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
        &(Either::One(_t), Either::Two(_u)) => (),
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
        &(Either::Two(ref _t), Either::One(ref _u)) => (),
        _ => (),
    }
    match &(e.clone(), e.clone()) {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
        &(Either::One(_t), Either::Two(_u)) => (),
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
        (Either::Two(_t), Either::One(_u)) => (),
        _ => (),
    }
    fn f5(&(X(_t), X(_u)): &(X, X)) { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }

    let &mut (X(_t), X(_u)) = &mut (xm.clone(), xm.clone());
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
    if let &mut (Either::One(_t), Either::Two(_u)) = &mut (em.clone(), em.clone()) { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
    while let &mut (Either::One(_t), Either::Two(_u)) = &mut (em.clone(), em.clone()) { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
    match &mut (em.clone(), em.clone()) {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
        &mut (Either::One(_t), Either::Two(_u)) => (),
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
        &mut (Either::Two(_t), Either::One(_u)) => (),
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
        _ => (),
    }
    match &mut (em.clone(), em.clone()) {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
        &mut (Either::One(_t), Either::Two(_u))
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
        | &mut (Either::Two(_t), Either::One(_u)) => (),
        // FIXME: would really like a suggestion here too
        _ => (),
    }
    match &mut (em.clone(), em.clone()) {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
        &mut (Either::One(_t), Either::Two(_u)) => (),
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
        &mut (Either::Two(ref _t), Either::One(ref _u)) => (),
        _ => (),
    }
    match &mut (em.clone(), em.clone()) {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
        &mut (Either::One(_t), Either::Two(_u)) => (),
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
        &mut (Either::Two(ref mut _t), Either::One(ref mut _u)) => (),
        _ => (),
    }
    match &mut (em.clone(), em.clone()) {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
        &mut (Either::One(_t), Either::Two(_u)) => (),
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
        (Either::Two(_t), Either::One(_u)) => (),
        _ => (),
    }
    fn f6(&mut (X(_t), X(_u)): &mut (X, X)) { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
}

