fn main() {}

struct U;

fn slice() {
    let mut arr = [U, U, U, U, U];
    let hold_all = &arr;
    let [ref _x0_hold, _x1, ref xs_hold @ ..] = arr; // { dg-error ".E0505." "" { target *-*-* } }
    _x1 = U; // { dg-error ".E0384." "" { target *-*-* } }
    drop(hold_all);
    let [_x0, ..] = arr; // { dg-error ".E0505." "" { target *-*-* } }
    drop(_x0_hold);
    let [_, _, ref mut _x2, _x3, mut _x4] = arr;
// { dg-error ".E0505." "" { target *-*-* } .-1 }
// { dg-error ".E0505." "" { target *-*-* } .-2 }
// { dg-error ".E0505." "" { target *-*-* } .-3 }
    drop(xs_hold);
}

fn tuple() {
    let mut tup = (U, U, U, U);
    let (ref _x0, _x1, ref _x2, ..) = tup;
    _x1 = U; // { dg-error ".E0384." "" { target *-*-* } }
    let _x0_hold = &mut tup.0; // { dg-error ".E0502." "" { target *-*-* } }
    let (ref mut _x0_hold, ..) = tup; // { dg-error ".E0502." "" { target *-*-* } }
    *_x0 = U; // { dg-error ".E0594." "" { target *-*-* } }
    *_x2 = U; // { dg-error ".E0594." "" { target *-*-* } }
    drop(tup.1); // { dg-error ".E0382." "" { target *-*-* } }
    let _x1_hold = &tup.1; // { dg-error ".E0382." "" { target *-*-* } }
    let (.., ref mut _x3) = tup;
    let _x3_hold = &tup.3; // { dg-error ".E0502." "" { target *-*-* } }
    let _x3_hold = &mut tup.3; // { dg-error ".E0499." "" { target *-*-* } }
    let (.., ref mut _x4_hold) = tup; // { dg-error ".E0499." "" { target *-*-* } }
    let (.., ref _x4_hold) = tup; // { dg-error ".E0502." "" { target *-*-* } }
    drop(_x3);
}

fn closure() {
    let mut tup = (U, U, U);
    let c1 = || {
        let (ref _x0, _x1, _) = tup;
    };
    let c2 = || {
// { dg-error ".E0382." "" { target *-*-* } .-1 }
        let (ref mut _x0, _, _x2) = tup;
    };
    drop(c1);
}

