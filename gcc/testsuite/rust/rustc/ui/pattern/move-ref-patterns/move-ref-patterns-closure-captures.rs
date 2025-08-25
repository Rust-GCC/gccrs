fn main() {
    struct U;
    fn accept_fn_once(_: &impl FnOnce()) {}
    fn accept_fn_mut(_: &impl FnMut()) {}
    fn accept_fn(_: &impl Fn()) {}

    let mut tup = (U, U, U);
    let (ref _x0, _x1, ref mut _x2) = tup;
    let c1 = || {
// { dg-error ".E0525." "" { target *-*-* } .-1 }
// { dg-error ".E0525." "" { target *-*-* } .-2 }
        drop::<&U>(_x0);
        drop::<U>(_x1);
        drop::<&mut U>(_x2);
    };
    accept_fn_once(&c1);
    accept_fn_mut(&c1);
    accept_fn(&c1);

    let c2 = || {
// { dg-error ".E0525." "" { target *-*-* } .-1 }
        drop::<&U>(_x0);
        drop::<&mut U>(_x2);
    };
    accept_fn_mut(&c2);
    accept_fn(&c2);

    let c3 = || {
        drop::<&U>(_x0);
    };
    accept_fn(&c3);
}

