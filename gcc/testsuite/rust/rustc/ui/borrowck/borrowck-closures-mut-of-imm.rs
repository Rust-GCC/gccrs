// Tests that two closures cannot simultaneously have mutable
// and immutable access to the variable. Issue #6801.

fn set(x: &mut isize) {
    *x = 4;
}

fn a(x: &isize) {
    let mut c1 = || set(&mut *x);
// { dg-error ".E0596." "" { target *-*-* } .-1 }
    let mut c2 = || set(&mut *x);
// { dg-error ".E0524." "" { target *-*-* } .-1 }
// { dg-error ".E0524." "" { target *-*-* } .-2 }
    c2(); c1();
}

fn main() {
}

