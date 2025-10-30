// Test that borrow check considers all choices in an or pattern, even the
// unreachable ones.

#![feature(or_patterns)]

fn or_pattern_moves_all(x: ((String, String),)) {
    match x {
        ((y, _) | (_, y),) => (),
    }
    &x.0 .0;
// { dg-error ".E0382." "" { target *-*-* } .-1 }
    &x.0 .1;
// { dg-error ".E0382." "" { target *-*-* } .-1 }
}

fn or_pattern_borrows_all(mut x: ((String, String),)) {
    let r = match x {
        ((ref y, _) | (_, ref y),) => y,
    };
    &mut x.0 .0;
// { dg-error ".E0502." "" { target *-*-* } .-1 }
    &mut x.0 .1;
// { dg-error ".E0502." "" { target *-*-* } .-1 }
    drop(r);
}

fn or_pattern_borrows_all_mut(mut x: ((String, String),)) {
    let r = match x {
        ((ref mut y, _) | (_, ref mut y),) => y,
    };
    &x.0 .0;
// { dg-error ".E0502." "" { target *-*-* } .-1 }
    &x.0 .1;
// { dg-error ".E0502." "" { target *-*-* } .-1 }
    drop(r);
}

fn let_or_pattern_moves_all(x: ((String, String),)) {
    let ((y, _) | (_, y),) = x;
    &x.0 .0;
// { dg-error ".E0382." "" { target *-*-* } .-1 }
    &x.0 .1;
// { dg-error ".E0382." "" { target *-*-* } .-1 }
}

fn let_or_pattern_borrows_all(mut x: ((String, String),)) {
    let ((ref r, _) | (_, ref r),) = x;
    &mut x.0 .0;
// { dg-error ".E0502." "" { target *-*-* } .-1 }
    &mut x.0 .1;
// { dg-error ".E0502." "" { target *-*-* } .-1 }
    drop(r);
}

fn let_or_pattern_borrows_all_mut(mut x: ((String, String),)) {
    let ((ref mut r, _) | (_, ref mut r),) = x;
    &x.0 .0;
// { dg-error ".E0502." "" { target *-*-* } .-1 }
    &x.0 .1;
// { dg-error ".E0502." "" { target *-*-* } .-1 }
    drop(r);
}

fn main() {}

