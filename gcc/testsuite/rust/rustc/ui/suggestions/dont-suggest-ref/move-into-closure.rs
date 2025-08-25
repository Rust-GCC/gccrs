#[derive(Clone)]
enum Either {
    One(X),
    Two(X),
}

#[derive(Clone)]
struct X(Y);

#[derive(Clone)]
struct Y;

fn consume_fn<F: Fn()>(_f: F) { }

fn consume_fnmut<F: FnMut()>(_f: F) { }

pub fn main() { }

fn move_into_fn() {
    let e = Either::One(X(Y));
    let mut em = Either::One(X(Y));

    let x = X(Y);

    // move into Fn

    consume_fn(|| {
        let X(_t) = x;
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
        if let Either::One(_t) = e { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
        while let Either::One(_t) = e { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
        match e {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
            Either::One(_t)
            | Either::Two(_t) => (),
        }
        match e {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
            Either::One(_t) => (),
            Either::Two(ref _t) => (),
            // FIXME: should suggest removing `ref` too
        }

        let X(mut _t) = x;
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
        if let Either::One(mut _t) = em { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
        while let Either::One(mut _t) = em { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
        match em {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
            Either::One(mut _t)
            | Either::Two(mut _t) => (),
        }
        match em {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
            Either::One(mut _t) => (),
            Either::Two(ref _t) => (),
            // FIXME: should suggest removing `ref` too
        }
    });
}

fn move_into_fnmut() {
    let e = Either::One(X(Y));
    let mut em = Either::One(X(Y));

    let x = X(Y);

    // move into FnMut

    consume_fnmut(|| {
        let X(_t) = x;
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
        if let Either::One(_t) = e { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
        while let Either::One(_t) = e { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
        match e {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
            Either::One(_t)
            | Either::Two(_t) => (),
        }
        match e {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
            Either::One(_t) => (),
            Either::Two(ref _t) => (),
            // FIXME: should suggest removing `ref` too
        }

        let X(mut _t) = x;
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
        if let Either::One(mut _t) = em { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
        while let Either::One(mut _t) = em { }
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
        match em {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
            Either::One(mut _t)
            | Either::Two(mut _t) => (),
        }
        match em {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
            Either::One(mut _t) => (),
            Either::Two(ref _t) => (),
            // FIXME: should suggest removing `ref` too
        }
        match em {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { help ".E0507." "" { target *-*-* } .-2 }
// { suggestion ".E0507." "" { target *-*-* } .-3 }
            Either::One(mut _t) => (),
            Either::Two(ref mut _t) => (),
            // FIXME: should suggest removing `ref` too
        }
    });
}

