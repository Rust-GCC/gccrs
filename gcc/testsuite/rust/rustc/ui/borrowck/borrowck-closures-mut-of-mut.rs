// Tests that two closures cannot simultaneously both have mutable
// access to the variable. Related to issue #6801.

fn get(x: &isize) -> isize {
    *x
}

fn set(x: &mut isize) {
    *x = 4;
}

fn a(x: &mut isize) {
    let mut c1 = || set(&mut *x);
    let mut c2 = || set(&mut *x);
// { dg-error ".E0524." "" { target *-*-* } .-1 }
    c2(); c1();
}

fn main() {
}

