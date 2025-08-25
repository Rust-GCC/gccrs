use std::cell::RefCell;

fn main() {
    let mut r = 0;
    let s = 0;
    let x = RefCell::new((&mut r,s));

    let val: &_ = x.borrow().0;
// { dg-error ".E0716." "" { target *-*-* } .-1 }
// { dg-note ".E0716." "" { target *-*-* } .-2 }
// { dg-note ".E0716." "" { target *-*-* } .-3 }
// { dg-note ".E0716." "" { target *-*-* } .-4 }
    println!("{}", val);
// { dg-error "" "" { target *-*-* } .-1 }
}

