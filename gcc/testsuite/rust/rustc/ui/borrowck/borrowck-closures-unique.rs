// Tests that a closure which requires mutable access to the referent
// of an `&mut` requires a "unique" borrow -- that is, the variable to
// be borrowed (here, `x`) will not be borrowed *mutably*, but
//  may be *immutable*, but we cannot allow
// multiple borrows.



fn get(x: &isize) -> isize {
    *x
}

fn set(x: &mut isize) -> isize {
    *x
}

fn a(x: &mut isize) {
    let c1 = || get(x);
    let c2 = || get(x);
    c1();
    c2();
}

fn b(x: &mut isize) {
    let c1 = || get(x);
    let c2 = || set(x); // { dg-error ".E0500." "" { target *-*-* } }
    c1;
}

fn c(x: &mut isize) {
    let c1 = || get(x);
    let c2 = || { get(x); set(x); }; // { dg-error ".E0500." "" { target *-*-* } }
    c1;
}

fn d(x: &mut isize) {
    let c1 = || set(x);
    let c2 = || set(x); // { dg-error ".E0524." "" { target *-*-* } }
    c1;
}

fn e(x: &'static mut isize) {
    let c1 = |y: &'static mut isize| x = y;
// { dg-error ".E0594." "" { target *-*-* } .-1 }
    c1;
}

fn f(x: &'static mut isize) {
    let c1 = || x = panic!(); // OK assignment is unreachable.
    c1;
}

fn main() {
}

